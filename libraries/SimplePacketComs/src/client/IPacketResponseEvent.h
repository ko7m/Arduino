/*
 * IPacketResponseEvent.h
 *
 *  Created on: Mar 7, 2018
 *      Author: hephaestus
 */

#ifndef IPACKETRESPONSEEVENT
#define IPACKETRESPONSEEVENT

class IPacketResponseEvent {
public:
	virtual void onResponse(int timeBetweenSendAndRecive)=0;
	virtual void onTimeout(int timeBetweenSendAndRecive)=0;
};

#endif /* LIBRARIES_SIMPLEPACKETCOMS_SRC_CLIENT_IPACKETRESPONSEEVENT_H_ */
