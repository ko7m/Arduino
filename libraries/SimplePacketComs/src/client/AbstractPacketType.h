/*
 * AbstractPacketType.h
 *
 *  Created on: Mar 6, 2018
 *      Author: hephaestus
 */

#ifndef ABSTRACTPACKETTYPE
#define ABSTRACTPACKETTYPE
#include <stdint.h>
#include <stdio.h>
#include "IPacketResponseEvent.h"
#include <limits.h>
class AbstractPacketType {

public:
	IPacketResponseEvent * responseListener = NULL;

	int packetSize;
	uint8_t * downstream;
	uint8_t * upstream;
	long runCount = 0;
	long maxRuns = LONG_MAX;
	AbstractPacketType(int id, int size);
	virtual ~AbstractPacketType();

	uint32_t * getUpstreamIdPointer();
	uint32_t * getDownstreamIdPointer();
	uint32_t getId();
	uint8_t * getUpstreamBytes();
	uint8_t * getDownstreamBytes();
	float * getUpstreamFloats();
	float * getDownstreamFloats();
	/**
	 * Set the listening event object
	 */
	void setResponseListener(IPacketResponseEvent * newresponseListener);
};

#endif /* LIBRARIES_SIMPLEPACKETCOMS_SRC_CLIENT_ABSTRACTPACKETTYPE_H_ */
