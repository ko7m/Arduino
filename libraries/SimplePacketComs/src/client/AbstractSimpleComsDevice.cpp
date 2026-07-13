/*
 * AbstractSimpleComsDevice.cpp
 *
 *  Created on: Mar 6, 2018
 *      Author: hephaestus
 */

#include <client/AbstractSimpleComsDevice.h>
/**
 * call to shut down the device
 */
void AbstractSimpleComsDevice::disconnect() {
	disconnectDeviceImp();
}
/**
 * Call to set up the device
 */
bool AbstractSimpleComsDevice::connect() {
	return connectDeviceImp();
}
/**
 * Add a packet to the polling queue
 * This packet will be polled evey iteration of loop
 */
void AbstractSimpleComsDevice::addPollingPacket(AbstractPacketType * packet) {
	pollingQueue.push_back(packet);
}
/**
 * Process one packet through the device
 * If the device is not ready this will return fast
 */
bool AbstractSimpleComsDevice::writePacket(AbstractPacketType * packet) {
	state = WaitingForResponse;
	int writen = write(packet->downstream, packet->packetSize);
	if (writen > 0) {
		return true;
	}
	return false;
}
/**
 * Process one packet through the device
 * If the device is not ready this will return fast
 */
bool AbstractSimpleComsDevice::readPacket(AbstractPacketType * packet,
		int timeBetweenSendAndRecive) {
	int r = read(packet->upstream);
	if (r > 0) {
		if (packet->responseListener != NULL)
			packet->responseListener->onResponse(timeBetweenSendAndRecive);
		return true;
	}

	return false;
}
/**
 * Pass each item in the polling queue through the processor
 * This is a blocking synchronous function
 */
void AbstractSimpleComsDevice::loop(long millis, long timout) {
	if (!isReadyDeviceImp())
		state = Start;

	switch (state) {
	case Start:
		if (isReadyDeviceImp()) {
			state = SkipCheck;
			it = pollingQueue.begin();
		}
		//Serial.println("AbstractSimpleComsDevice::Start");
		break;
	case SkipCheck:

		tmpPacket = (*it);
		if (tmpPacket->runCount > tmpPacket->maxRuns) {
			state = Reset; //skip this packet, it has run too often
			break;
		}

		if (tmpPacket->runCount == LONG_MAX) {
			tmpPacket->runCount = 0; //reset for roll over of continuous polling
			break;
		}
		//Serial.println("AbstractSimpleComsDevice::SkipCheck");
		tmpPacket->runCount++;
		state = Process;
		//no break
	case Process:
		//Serial.println("AbstractSimpleComsDevice::Process");
		tranceiveTime = millis;
		writePacket(tmpPacket);
		break;
	case WaitingForResponse:
		//Serial.println("AbstractSimpleComsDevice::WaitingForResponse");

		if ((millis - tranceiveTime) > timout) {
			state = Timeout;
			break;
		}
		if (isResponseReady())
			state = GotResponse; // quickly process available packet
		else
			break; // waiting for response still, come back later
		//no break
	case GotResponse:
		//Serial.println("AbstractSimpleComsDevice::GotResponse");

		readPacket(tmpPacket, millis - tranceiveTime);
		state = Reset;
		break;
	case Timeout:
		//Serial.println("AbstractSimpleComsDevice::Timeout");
		//Serial.println("Packet timed out");
		if (tmpPacket->responseListener != NULL)
			tmpPacket->responseListener->onTimeout(millis - tranceiveTime);
		state = Reset;
		//no break
	case Reset:
		//Serial.println("AbstractSimpleComsDevice::Reset");
		++it;
		if (it == pollingQueue.end())
			state = Start;
		else
			state = SkipCheck;
		break;
	}
}

AbstractSimpleComsDevice::~AbstractSimpleComsDevice() {
	disconnect();
}

