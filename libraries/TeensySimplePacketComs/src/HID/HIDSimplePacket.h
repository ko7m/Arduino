#ifndef HIDPACKET
#define HIDPACKET
#include <SimplePacketComs.h>
#include <usb_rawhid.h>
class HIDSimplePacket : public SimplePacketComsAbstract{
private:
	// RawHID packets are always 64 bytes
public:
  HIDSimplePacket();
  /**
  * Non blocking function to check if a packet is availible from the physical layer
  */
   bool isPacketAvailible();
  /**
  * User function that fills the buffer from the data in the physical layer
  * this data should already be framed, checksummed and validated as a valid packet
  */
   int32_t getPacket(uint8_t * buffer,uint32_t numberOfBytes);
  /**
  * User function that sends the buffer to the physical layer
  * this data should already be framed, checksummed and validated as a valid packet
  */
   int32_t sendPacket(uint8_t * buffer,uint32_t numberOfBytes);
};

#endif /* end of include guard: HIDPACKET */
