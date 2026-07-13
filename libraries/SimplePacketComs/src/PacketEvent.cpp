#include "PacketEvent.h"

PacketEvent::PacketEvent(uint32_t id) {
	myId = id;
}
PacketEventAbstract::PacketEventAbstract(uint32_t id) :
		PacketEvent(id) {
}
uint32_t PacketEvent::getId() {
	return myId;
}
