// UDP NTP Client - Implements an NTP set clock
//
// Displays UTC and Pacific time and date on 16x2 LCD display synchronized to internet time server time.nist.gov.
// Calculates daylights savings time, handles leap years.  Currently only set up to display Pacific and UTC zones.
// The local time zone can be changed by changing localTimeZoneOffset to the appropriate value to be added to
// UTC time.
//
// Jeff Whitlatch - ko7m
// 23 Mar 2017

#include <Ethernet.h>
#include <EthernetUdp.h>
#include <LiquidCrystal.h>

// Defines
#define localTimeZoneOffset (-8)

// Freetronics 16x2 version
LiquidCrystal lcd(8,9,4,5,6,7);

// MAC address to use for Ethernet adapter
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE };

unsigned int localPort = 8888;       // local port to listen for UDP packets

char timeServer[] = "time.nist.gov"; // time.nist.gov NTP server

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

const char *szMonth[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    Serial.println("Setting up ethernet");

    // Init the LCD display
    lcd.begin(16, 2);

    // start Ethernet and UDP
    if (Ethernet.begin(mac) == 0)
    {
        Serial.println("Failed to configure Ethernet using DHCP");
        // no point in carrying on, so do nothing forevermore:
        for (;;);
    }
    Udp.begin(localPort);
    Serial.println("End of setup");
}

unsigned long secsSince1900 = 0;

int hour = 0;
int minute = 59;
int second = 58;
int day = 0;
int month = 0;
int year = 1900;
int dow = 0;
boolean fDST = false;

char buf[256];

void loop()
{
    // Once every hour, update from the net, this takes a couple seconds so we don't do every time thru loop
    if (minute == 59 && second == 58)
    {
        sendNTPpacket(timeServer); // send an NTP packet to a time server

        delay(1000);

        if (Udp.parsePacket())
        {
            // We've received a packet, read the data from it
            Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

            // the timestamp starts at byte 40 of the received packet and is four bytes,
            // or two words, long. First, extract the two words:

            unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
            unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
            // combine the four bytes (two words) into a long integer
            // this is NTP time (seconds since Jan 1 1900):
            secsSince1900 = highWord << 16 | lowWord;
            Serial.print("Seconds since Jan 1 1900 = ");
            Serial.println(secsSince1900);
        }
    }

    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;

    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;

    // Calculate UTC time
    hour = (epoch / 60 / 60) % 24;
    minute = (epoch / 60) % 60;
    second = epoch % 60;

    // Adjust epoch to PST (minus 8 hours)  So we calculate date based on local time.
    epoch += (localTimeZoneOffset * 60 * 60);

    // Algorithm from http://howardhinnant.github.io/date_algorithms.html#civil_from_days
    // an era is a 400 year period starting 1 Mar 0000.  
    long z = epoch / 86400 + 719468;
    long era = (z >= 0 ? z : z - 146096) / 146097;
    unsigned long doe = static_cast<unsigned long>(z - era * 146097);     // Day of era
    unsigned long yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;  // Year of era
    unsigned long doy = doe - (365*yoe + yoe/4 - yoe/100);
    unsigned long mp = (5*doy + 2)/153;

    month = mp + (mp < 10 ? 3 : -9);
    day = doy - (153*mp+2)/5 + 1;
    year = static_cast<int>(yoe) + era * 400;
    year += (month <= 2);

    dow = dowFromDate(day, month, year);    // Day of week
    fDST = isDST(day, month, dow);          // Daylight savings time true/false

    // Build the print buffer
    sprintf(buf, "%2d:%02d UTC %2d %s", hour, minute, day, szMonth[month-1]);
    lcd.setCursor(0, 0);
    lcd.print(buf);

    // Recalculate the local hour
    hour = (epoch / 60 / 60) % 24;

    // Print the second time zone
    sprintf(buf, "%2d:%02d %s  %d", hour + (fDST ? 1 : 0), minute, fDST ? "PDT" : "PST", year);
    lcd.setCursor(0, 1);
    lcd.print(buf);

    // Keep the network alive every 60 seconds
    if (second % 60 == 0)
        Ethernet.maintain();

    // Accuracy depends on system clock accuracy, corrected once an hour.
    delay(1000);
    secsSince1900++;
}

// send an NTP request to the time server at the given address
void sendNTPpacket(char* address)
{
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);

    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision

    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12]  = 49;
    packetBuffer[13]  = 0x4E;
    packetBuffer[14]  = 49;
    packetBuffer[15]  = 52;

    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    Udp.beginPacket(address, 123); //NTP requests are to port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}

// In most of the US, DST starts on the second Sunday of March and ends on the first 
// Sunday of November at 2 am both times.
bool isDST(int day, int month, int dow)
{
    if (month < 3 || month > 11) return false;
    if (month > 3 && month < 11) return true;

    int prevSunday = day - dow;

    // In March, we are DST if our previous sunday was on or after the 8th.
    if (month == 3) return prevSunday >= 8;

    // In November, we must be before the first Sunday to be DST which means the
    // previous sunday must be before the 1st.
    return prevSunday <=0;
}

// Day of week calculation
inline int dowFromDate(int d, int m, int y)
{
    return (d += m < 3 ? y-- : y - 2, 23*m/9 + d + 4 + y/4- y/100 + y/400)%7;
}
