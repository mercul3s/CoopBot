#include <EthernetClient.h>
#include <Ethernet.h>
#include <Dhcp.h>
#include <EthernetServer.h>
#include <Dns.h>
#include <EthernetUdp.h>

#include <NTPClient.h>
#include <Time.h>
#include <TimeLib.h>

#include <Dusk2Dawn.h>

int mSunrise, mSunset;
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

IPAddress ip(10, 0, 80, 120);

EthernetServer server(80);

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at: ");
  Serial.println(Ethernet.localIP());
  // Create a lat/long/timezone offset
  // Figure out how to pull this info from a file that's not checked in to github
  // so we don't inadvertently expose our coordinates to the internet
  Dusk2Dawn molalla(45.062877, -122.662111, -8);

  // get current time and pass to sunrise/sunset functions
  // year, month, day, and if daylight savings is in effect.
  mSunrise = molalla.sunrise(2018, 3, 11, true);
  mSunset = molalla.sunset(2018, 3, 11, true);  
}

void loop() {
  
  // this prints epoch time, but must be synced with ntp to be accurate
  Serial.println(now());
  char time[6];
  Dusk2Dawn::min2str(time, mSunrise);
  Serial.println("Sunrise:");
  Serial.println(time);
  Dusk2Dawn::min2str(time, mSunset);
  Serial.println("Sunset:");
  Serial.println(time);
  delay(10000);
  // put your main code here, to run repeatedly:

}
