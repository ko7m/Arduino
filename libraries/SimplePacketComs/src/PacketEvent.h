#ifndef  PacketEvent_HEADER
#define PacketEvent_HEADER
#include <stdint.h>

class PacketEvent {
protected:
	// Unique id of the function being called
	uint32_t myId;
public:
	bool noResponse = false;
	// Packet ID needs to be set
	PacketEvent(uint32_t id);
	virtual ~PacketEvent(){}
	//User function to be called when a packet comes in
	// Buffer contains data from the packet coming in at the start of the function
	// User data is written into the buffer to send it back 
	virtual void event(float * buffer)=0;
	/**
	 * An optional loop function
	 * If your server needs a pulse to keep it updated, implement this function
	 */
	virtual bool loop()=0;

	uint32_t getId();
};

class PacketEventAbstract : public PacketEvent{
public:
	// Packet ID needs to be set
	PacketEventAbstract(uint32_t id);
	virtual ~PacketEventAbstract() {}
	/**
	 * An optional loop function
	 * If your server needs a pulse to keep it updated, implement this function
	 */
	virtual bool loop(){return false;};
};


#endif /* end of include guard: PacketEvent */
