#include "SimplePacketComs.h"

SimplePacketComsAbstract::SimplePacketComsAbstract() {
	setNumberOfBytesInPacket(DEFAULT_PACKET_SIZE_SIMPLE_PACKET);

}
uint32_t SimplePacketComsAbstract::getNumberOfBytesInPacket() {
	return numberOfBytes;
}
void SimplePacketComsAbstract::setNumberOfBytesInPacket(
		uint32_t newNumberOfBytes) {
	numberOfBytes = newNumberOfBytes;
	buffer = new uint8_t[numberOfBytes];
}
uint32_t SimplePacketComsAbstract::getNumberOfFloatsInPacket() {
	return (numberOfBytes / 4) - 1;
}

void SimplePacketComsAbstract::attach(
		PacketEvent * eventImplementation) {
	fmap[eventImplementation->getId()] = eventImplementation;
}

PacketEvent * SimplePacketComsAbstract::detach(uint32_t id) {
	PacketEvent *event = fmap[id];
	fmap.erase(id);
	return event;
}

/**
 * This runs the packet server and calls all events if a backet comes in
 */
void SimplePacketComsAbstract::server() {
	for (std::map<uint32_t, PacketEvent*>::iterator it = fmap.begin();
			it != fmap.end(); ++it) {
		it->second->loop();
	}
	if (isPacketAvailible()) {
		getPacket(buffer, numberOfBytes);
		uint32_t currentId = getIdPointer()[0];
		for (std::map<uint32_t, PacketEvent*>::iterator it =
				fmap.begin(); it != fmap.end(); ++it) {
			if (it->second->getId() == currentId) {
				it->second->noResponse = false; // reset the response flag
				it->second->event(getDataPointer());

				if (it->second->noResponse == false) {
					// respond unless the no response flag is set
					sendPacket(buffer, numberOfBytes);
				}

				return; // packet is responded to, fast return
			}
		}

		//printf("\nUnknown packet %i ",currentId);
		for (int i = 0; i < 4; i++) {
			//copy the incoming missing ID into data
			buffer[i + 4] = buffer[i];
		}
		//write in error ID
		buffer[0] = 99;
		buffer[1] = 0;
		buffer[2] = 0;
		buffer[3] = 0; //error packet
		// Zero out the rest of packet
		for (int i = 8; i < numberOfBytes; i++) {
			buffer[i] = 0;
		}

		sendPacket(buffer, numberOfBytes);
		return;
	}
}
/**
 * This pushes a packet upstream
 */
void SimplePacketComsAbstract::push(uint32_t id, float * bufferofFloats) {
	getIdPointer()[0] = id;
	float * outgoingBuff = getDataPointer();
	for (uint32_t i = 0; i < getNumberOfFloatsInPacket(); i++) {
		outgoingBuff[i] = bufferofFloats[i];
	}
	sendPacket(buffer, numberOfBytes);
}
