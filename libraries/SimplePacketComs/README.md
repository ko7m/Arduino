# SimplePacketComs
A simple packet coms library. This is the definition page and C++ implementation of SimplePacketComs. 

A small update: This library is not dead or stagnent, just stable. Still in active daily use. 

# Theory Of Operation

SimplePacketComs is a protocol spec for transmitting data from one endpoint to another. The goal is to allow many different commands to be sent and received on a single communication channel, with an easy to extend framework. The assumptions that we made is that the physical layer will send and receive aligned, checksummed packets of data. 

## Packet Structure

Packets consist of 4 bytes to define the command ID and N bytes (detected from the PHY) of data. The Command ID is parsed as a 32 bit unsigned int. The data can be sent as raw bytes or 32 bit floating point numbers. 

By convention, a standard packet is 64 bytes. If both sides of an implementation change this, it does not break spec, but for "Simplicity" this is not transmitted in packet data and must be agreed upon ahead of time by both sides of the command stack. 

### Byte Packets

An example byte packet from the Game Controller looks like this:

| |ID | byte 0 |byte 1 |byte 2 |byte 3 |byte 4 |byte 5 |byte 6 |byte 7 |byte 8 |byte 9 |byte 10 |byte 11 |byte 12 |byte 13 |byte 14 |byte 15 |byte 16 |byte 17 |byte 18 |byte 19 |
|--- |--- | --- |--- | --- |--- | --- | --- | --- |--- | --- |--- | --- | --- | --- |--- | --- |--- | --- | --- |--- |--- |
| downstream Bytes |4 | 1 | 1 | 1 | 1 | 1 | 1 |1 | 1 | 1 | 1 | 1 | 1 |1 | 1 | 1 | 1 | 1 | 1 |1|1|
| Contents downstream |1970 | Controller ID| JoyXLeft | JoyYLeft | JoyXRight | JoyYRight | TriggerLeft | TriggerRight |PadRight/PadLeft| PadDown/PadUp |--- |--- |ButtonX|ButtonY|ButtonA|ButtonB|ButtonMinus/getButtonPlus|ButtonHome|---|ButtonZLeft|ButtonZRight|
| upstream Bytes |4 | 60 |
| Contents upstream |1970 | Byte data to be printed by the game controller server|


### Float packets

An example float packet from the warehouse robot looks like this:

| |ID | float |float |float |float |float |float |
|--- |--- | --- |--- | --- |--- | --- | --- |
| downstream Bytes |4 | 4 | 4 | 4 | 4 | 4 | 4 |
| Contents downstream |1936 | pickup area | pickup x | pickup z | drop off area | drop off  x | drop off  z |
| upstream Bytes |4 | 0 |
| Contents upstream |1936 | ---|



# Known Stack implementations

## Arduino

In Library Manager search for SimplePacketComs

## C++

https://github.com/madhephaestus/SimplePacketComs/

## Java
![](https://img.shields.io/nexus/r/https/oss.sonatype.org/com.neuronrobotics/SimplePacketComsJava.svg?style=flat) 

# Gradle 

```
repositories {
//com.neuronrobotics hosting point
maven { url 'https://oss.sonatype.org/content/repositories/staging/' }
}
dependencies {
compile "com.neuronrobotics:SimplePacketComsJava:SEE BADGE FOR VERSION "
}
```
### Source

https://github.com/madhephaestus/SimplePacketComsJava

## Matlab (Experimental)

https://github.com/WPIRoboticsEngineering/RBE3001_Matlab

# Known Microcontroller Phy's

## MBED C++ USB HID (Example using Mbed HID)

https://github.com/WPIRoboticsEngineering/RBE3001_nucleo_firmware

## Arduino Teensy 3.2-6 HID

In Library Manager search for Teensy32SimplePacketComs

## Arduino ESP32 WiFi UDP

In Library Manager search for Esp32SimplePacketComs


# Code Examples

For a code example, we will use the C++ Stack in Arduino, the Esp32SimplePacketComs Phy, and the BNO055Server on the device side. On the receive side we will use SimplePacketComsJava and a Groovy script to connect to the device. 

## Arduino Setup

Install from the Library Manager

```
Esp32SimplePacketComs
SimplePacketComs
BNO055SimplePacketComs
```

Open the Example -> BNO055SimplePacketComs -> BNO055Server and program your ESP32. If you have never used a WiFiManager before, open the serial monitor (make sure no line endings is enabled) and type your Wifi SSID and hit enter. Wait to be prompted for the password. Then type the password and hit enter. Once connected to the Wifi the IP address will be written out to the non-volatile memory. This device will connect automatically from now on. 

Get a BNO055 sensor from Adafruit or Digikey and connect it to the I2C bus on your ESP32. 

The Arduino is now set up.

## Groovy setup

In BowlerStudio, run https://gist.github.com/5931b62a4f02216136583ec67dd993ff.git BNO055Visualizer.groovy

You will see the device connect and a small servo display on the screen. The orientation of the servo is controlled by the BNO055.
# HOWTO use the C++ stack

## New Servers C++
```
class ExampleServer: public PacketEventAbstract {
public:
    // Packet ID needs to be set
    ExampleServer() :
            PacketEventAbstract(1871) // Address of this event
    {
    }
    //User function to be called when a packet comes in
    // Buffer contains data from the packet coming in at the start of the function
    // User data is written into the buffer to send it back
    void event(float * buffer) {
        // read from buffer to get incomming data
    // write to buffer to send data back
    }
};
UDPSimplePacket coms;
WifiManager manager;
manager.setup();
coms.attach( new ExampleServer());
coms.attach(new NameCheckerServer("MyRobotName")); 
while(true){
 manager.loop();
 if(manager.getState()==Connected)
    coms.server();
}
```
## New clients C++
```
WifiManager manager;
manager.setup();
this event, Size of packet
class ExampleClientMessageHandeler:  public IPacketResponseEvent {
public:
    ExampleClientMessageHandeler();
    virtual ~ExampleClientMessageHandeler();
    void onResponse(int timeBetweenSendAndRecive){
    Serial.println("Responce!");
  }
    void onTimeout(int timeBetweenSendAndRecive){
    Serial.println("Timeout!");
  }
};

boolean once = false;
while(true){
  manager.loop();
  if (manager.getState()==Connected) {
    if(!once){
      static IPAddress broadcast=WiFi.localIP();// Get the WiFI connection IP address
      UDPSimplePacketComs * coms = new UDPSimplePacketComs(&broadcast,true);// Create a Coms device
      AbstractPacketType * ExampleClient =new AbstractPacketType(1871, 64);//Create a client for the server above, Address of 
      ExampleClient->setResponseListener(new ExampleClientMessageHandeler());
      coms->addPollingPacket(ExampleClient); // Add the packet to a polling queue
      once = true;
    }
    coms->loop(millis(), 100);// Loop the server and look for responses
  }
}

```

# HOWTO Use the Java Stack

## New Clients Java

```
public class ExampleClient extends UDPSimplePacketComs {
    private FloatPacketType IMU = new FloatPacketType(1871, 64);
    double[] data = new double[15];
    public ExampleClient(String name) throws Exception {
        super(name);
        addPollingPacket(IMU);
        addEvent(1871,()->{
            readFloats(1871, data);
        });
    }
    public double get(int index){
    	return data[index];
    }
    public void send(int index,double d){
    	IMU.getDownstream()[index]=d;
    }
}

```

## New Servers Java

```
public class ExampleServer extends UdpServer {
    public ExampleServer(String name, int index) {
        super(name);
        addServer(new FloatServer(1871) {
            public boolean event(float[] packet) {
            }
        });
    }

}

```
