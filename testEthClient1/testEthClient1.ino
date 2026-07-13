#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

char serverName[] = "whitco.com";

EthernetClient client;

void setup() {

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    while (true);
  }

  Serial.begin(115200);
  Serial.print("Local IP address = ");
  Serial.println(Ethernet.localIP());
  Serial.println();
  sendGET();
}

void loop()
{

}

void sendGET()
{
  if (client.connect(serverName, 80))
  {
    Serial.println("connected");
    client.println("GET /clock.html HTTP/1.1");
    client.println("Host: www.whitco.com");
    client.println("Connection: close");
    client.println();
  }
  else
  {
    Serial.println("connection failed");
    Serial.println();
  }

  // Wait for client connection and data returned.
  while (client.connected() && !client.available()) delay(1);
  while (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }

  client.stop();
  Serial.println("client disconnected.");
  Serial.println();
}
