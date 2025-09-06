#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#include "secrets.h"

/* WiFi Credentials */
char ssid[] = SSID;           // the network SSID (name)
char pass[] = PASSWORD;       // the network password
int status = WL_IDLE_STATUS;  // the WiFi radio's status

/* GitHub API */
char server[] = "api.github.com"
String owner = "";
String repo = "";
String token= "";


void setup()
{
  Serial.begin(9600);
  while (!Serial);

  // attempt to connect to WiFi network
  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(ssid, pass);  // connect to the network
    delay(10000);                     // wait 10 seconds for conection
  }

  Serial.println("Connected to WiFi!");
}