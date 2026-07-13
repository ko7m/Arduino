#ifndef SimplePacketComs
#define SimplePacketComs
#include <stdint.h>
#include <stdio.h>
#include <map>
#include <iostream>
#include "PacketEvent.h"
#include "client/AbstractSimpleComsDevice.h"
#include "client/AbstractPacketType.h"
#include "client/IPacketResponseEvent.h"

#define DEFAULT_PACKET_SIZE_SIMPLE_PACKET 64

class SimplePacketComsAbstract {
private:

	uint32_t numberOfBytes;
	uint8_t * buffer;
	std::map<uint32_t, PacketEvent*> fmap;
public:
	SimplePacketComsAbstract();
	/**
	 * Non blocking function to check if a packet is availible from the physical layer
	 */
	virtual bool isPacketAvailible()=0;
	/**
	 * User function that fills the buffer from the data in the physical layer
	 * this data should already be framed, checksummed and validated as a valid packet
	 */
	virtual int32_t getPacket(uint8_t * buffer, uint32_t numberOfBytes)=0;
	/**
	 * User function that sends the buffer to the physical layer
	 * this data should already be framed, checksummed and validated as a valid packet
	 */
	virtual int32_t sendPacket(uint8_t * buffer, uint32_t numberOfBytes)=0;
	/**
	 Gets the numbetr of bytes the PHY uses to transfer information
	 */
	uint32_t getNumberOfBytesInPacket();
	/**
	 Allow the user to set the number of bytes to transfer by the phy
	 */
	void setNumberOfBytesInPacket(uint32_t newNumberOfBytes);
	/**
	 The number of data values the phy transfers in the data transaction
	 */
	uint32_t getNumberOfFloatsInPacket();

	/**
	 * Attach a function to a packet event
	 */
	void attach(PacketEvent * eventImplementation);
	/**
	 * Detach a function from a packet event.
	 *
	 * @param id The packet id.
	 * @return The function that was attached.
	 */
	PacketEvent * detach(uint32_t id);
	/**
	 * This runs the packet server and calls all events if a backet comes in
	 */
	void server();
	/**
	 * This pushes a packet upstream
	 */
	void push(uint32_t id, float * buffer);

	uint32_t * getIdPointer() {
		return (uint32_t *) buffer;
	}
	float * getDataPointer() {
		return (float *) (buffer + 4);
	}
	std::map<uint32_t, PacketEvent*> * getfMap() {
		return &fmap;
	}

};

#endif /* end of include guard: SimplePacketComs
 */
