#include <Ethernet.h>
#include <SPI.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 254, 99 };         // Make sure this IP address is available AND appropriate or change it to one that is.
byte server[] = { 192, 168, 254, 25 };  // Google

EthernetClient client;

void setup()
{
  Ethernet.begin(mac, ip);
  Serial.begin(115200);

  delay(1000);

  Serial.println("connecting...");

  if (client.connect(server, 80)) {
    client.println("GET / HTTP/1.0");
    client.println();
  } else {
    Serial.println("connection failed");
  }
}

void loop()
{
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    for(;;)
      ;
  }
}
