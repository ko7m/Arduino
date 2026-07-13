/*
 * AbstractSimpleComsDevice.h
 *
 *  Created on: Mar 6, 2018
 *      Author: hephaestus
 */

#ifndef ABSTRACTSIMPLECOMSDEVICE
#define ABSTRACTSIMPLECOMSDEVICE
#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "AbstractPacketType.h"
#include "IPacketResponseEvent.h"

enum TranceiveState {
	Start, SkipCheck, Process, WaitingForResponse, GotResponse, Timeout, Reset
};
class AbstractSimpleComsDevice {
private:
	std::vector<AbstractPacketType*> pollingQueue;
	TranceiveState state = Start;
	std::vector<AbstractPacketType*>::iterator it;
	AbstractPacketType * tmpPacket;
	unsigned long tranceiveTime = 0;
	/**
	 * Write one packet through the device
	 * If the device is not ready this will return fast
	 * Returns the success of the transaction
	 */
	bool writePacket(AbstractPacketType * packet);
	/**
	 * Read one packet through the device
	 * If the device is not ready this will return fast
	 * Returns the success of the transaction
	 */
	bool readPacket(AbstractPacketType * packet, int timeBetweenSendAndRecive);
public:
	AbstractSimpleComsDevice() {
	}
	virtual ~AbstractSimpleComsDevice();
	/**
	 * call to shut down the device
	 */
	void disconnect();
	/**
	 * Call to set up the device
	 */
	bool connect();
	/**
	 * Add a packet to the polling queue
	 */
	void addPollingPacket(AbstractPacketType * packet);

	/**
	 * Pass each item in the polling queue through the processor
	 * This is a blocking synchronous function
	 * Takes as a parameter the current time since start of chip in milliseconds
	 */
	void loop(long millis, long timout);

protected:
	/**
	 * Abstract methods
	 */
	/**
	 * Set up the device connection
	 */
	virtual bool connectDeviceImp()=0;
	/**
	 * Shut down the device connection to be ready to connect again
	 */
	virtual bool disconnectDeviceImp()=0;
	/**
	 * A call back to the device to check its read state
	 * If the device is not ready no other opperations should take place
	 */
	virtual bool isReadyDeviceImp()=0;
	/**
	 * Blocking read with timeout. This should wait for a reasonably long amount of time (200 ms)
	 * If time is exceeded return 0
	 */
	virtual int read(uint8_t* message)=0;
	/**
	 * Blocking write with timeout. This should wait for a reasonably long amount of time (200 ms)
	 * If time is exceeded return 0
	 */
	virtual int write(uint8_t * message, int length)=0;
	/**
	 * Check device to see if a response is ready
	 */
	virtual bool isResponseReady()=0;

};

#endif /* LIBRARIES_SIMPLEPACKETCOMS_SRC_CLIENT_ABSTRACTSIMPLECOMSDEVICE_H_ */
