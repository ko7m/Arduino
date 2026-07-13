/*
 * PacketType.cpp
 *
 *  Created on: Mar 7, 2018
 *      Author: hephaestus
 */

#include "AbstractPacketType.h"

AbstractPacketType::AbstractPacketType(int id, int size) {
	packetSize = size;
	downstream = new uint8_t[size];
	upstream = new uint8_t[size];
	getDownstreamIdPointer()[0] = id;
	runCount = 0;
	maxRuns = LONG_MAX;

}
AbstractPacketType::~AbstractPacketType() {
}

uint32_t * AbstractPacketType::getUpstreamIdPointer() {
	return (uint32_t *) upstream;
}
uint32_t * AbstractPacketType::getDownstreamIdPointer() {
	return (uint32_t *) downstream;
}
uint32_t AbstractPacketType::getId() {
	return getDownstreamIdPointer()[0];
}
uint8_t * AbstractPacketType::getUpstreamBytes() {
	return (uint8_t *) (upstream + 4);
}
uint8_t * AbstractPacketType::getDownstreamBytes() {
	return (uint8_t *) (downstream + 4);
}
float * AbstractPacketType::getUpstreamFloats() {
	return (float *) (upstream + 4);
}
float * AbstractPacketType::getDownstreamFloats() {
	return (float *) (downstream + 4);
}
/**
 * Set the listening event object
 */
void AbstractPacketType::setResponseListener(
		IPacketResponseEvent * newresponseListener) {
	responseListener = newresponseListener;
}
