#ifdef ARDUINO_TEENSY41

#include <Arduino.h>
#include <NativeEthernet.h>
#include <micro_ros_arduino.h>

extern "C"
{
  static EthernetUDP udp_client;

  bool arduino_native_ethernet_udp_transport_open(struct uxrCustomTransport * transport)
  {
    struct micro_ros_agent_locator * locator = (struct micro_ros_agent_locator *) transport->args;
    udp_client.begin(locator->port);
    return true;
  }

  bool arduino_native_ethernet_udp_transport_close(struct uxrCustomTransport * transport)
  {
    udp_client.stop();
    return true;
  }

  size_t arduino_native_ethernet_udp_transport_write(struct uxrCustomTransport * transport, const uint8_t *buf, size_t len, uint8_t *errcode)
  {
    (void)errcode;
    struct micro_ros_agent_locator * locator = (struct micro_ros_agent_locator *) transport->args;

    udp_client.beginPacket(locator->address, locator->port);
    size_t sent = udp_client.write(buf, len);
    udp_client.endPacket();
    udp_client.flush();

    return sent;
  }

  size_t arduino_native_ethernet_udp_transport_read(struct uxrCustomTransport * transport, uint8_t *buf, size_t len, int timeout, uint8_t *errcode)
  {
    (void) errcode;
	uint32_t start_time = millis();

    while(millis() - start_time < ((uint32_t) timeout) && udp_client.parsePacket() == 0){
      delay(1);
    }

    size_t readed = udp_client.read(buf, len);
    return (readed < 0) ? 0 : readed;
  }
}

#endif // ARDUINO_TEENSY41
