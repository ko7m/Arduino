#include <HID/HIDSimplePacket.h>

HIDSimplePacket::HIDSimplePacket() {

}

/**
 * Non blocking function to check if a packet is availible from the physical layer
 */
bool HIDSimplePacket::isPacketAvailible() {
	return usb_rawhid_available();
}
/**
 * User function that fills the buffer from the data in the physical layer
 * this data should already be framed, checksummed and validated as a valid packet
 */
int32_t HIDSimplePacket::getPacket(uint8_t * buffer, uint32_t numberOfBytes) {
	return usb_rawhid_recv(buffer, 0);
}
/**
 * User function that sends the buffer to the physical layer
 * this data should already be framed, checksummed and validated as a valid packet
 */
int32_t HIDSimplePacket::sendPacket(uint8_t * buffer, uint32_t numberOfBytes) {
	return usb_rawhid_send(buffer, 100);
}
