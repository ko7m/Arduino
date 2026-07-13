 /* Basic Raw HID Example
   Teensy can send/receive 64 byte packets with a
   dedicated program running on a PC or Mac.

   You must select Raw HID from the "Tools > USB Type" menu

   Optional: LEDs should be connected to pins 0-7,
   and analog signals to the analog inputs.

   This example code is in the public domain.
*/
#include <SimplePacketComs.h>
// NOTE you need to add -lsupc++_s -lstdc++_s to "append to link" compile options
#include <TeensySimplePacketComs.h>

class SetServos: public PacketEventAbstract {
public:
	// Packet ID needs to be set
	SetServos() :
			PacketEventAbstract(1962)	// Address of this event
	{
	}
	//User function to be called when a packet comes in
	// Buffer contains data from the packet coming in at the start of the function
	// User data is written into the buffer to send it back
	void event(float * buffer) {
		uint8_t * bytes = (uint8_t *) buffer;
		int numberOfBytes = 60;
		for (int i = 1; i < numberOfBytes; i++) {
			///bytes[i] = 0;	// write a value to the controller
		}
	}
};

class GetIMU: public PacketEventAbstract {
public:
	// Packet ID needs to be set
	GetIMU() :
			PacketEventAbstract(1804)	// Address of this event
	{
	}
	//User function to be called when a packet comes in
	// Buffer contains data from the packet coming in at the start of the function
	// User data is written into the buffer to send it back
	void event(float * buffer) {
		int numberOfBytes = 15;
		for (int i = 0; i < numberOfBytes; i++) {
			buffer[i]=0;
		}
	}
};
HIDSimplePacket coms;

void setup() {
	coms.attach(new SetServos());
	coms.attach(new GetIMU());

}

void loop() {
	coms.server();
}
