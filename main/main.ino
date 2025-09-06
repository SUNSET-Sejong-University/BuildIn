#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#include "secrets.h"

/* WiFi Credentials */
char ssid[] = SSID;           // the network SSID (name)
char pass[] = PASSWORD;       // the network password
int wifiStatus = WL_IDLE_STATUS;  // the WiFi radio's status

/* GitHub API */
char server[] = "api.github.com";
String owner = "Prithwis-2023";
String repo = "BuildIn";
String token= GH_TOKEN;

const int bluePin = 0;
const int yellowPin = 7;
const int redPin = 9;
const int greenPin = 6;

WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, server, 443);

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);

  // attempt to connect to WiFi network
  while (wifiStatus != WL_CONNECTED)
  {
    wifiStatus = WiFi.begin(ssid, pass);  // connect to the network
    delay(10000);                     // wait 10 seconds for conection
  }
  Serial.println("Connected to WiFi!");
}

void loop()
{
  getBuildStatus();
  delay(5000);
}

void getBuildStatus()
{
  String path = "/repos/" + owner + "/" + repo + "/actions/runs?per_page=1";
  client.beginRequest();
  client.get(path);
  client.sendHeader("User-Agent", "ArduinoMKR");
  client.sendHeader("Authorization", "Bearer " + token);
  client.endRequest();

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  if (statusCode == 200)
  {
    StaticJsonDocument<1024> doc;
    deserializeJson(doc, response);

    const char* status = doc["workflow_runs"][0]["status"];
    const char* conclusion = doc["workflow_runs"][0]["conclusion"];

    Serial.print("Status: "); Serial.println(status);
    Serial.print("Conclusion: "); Serial.println(conclusion);

    updateLED(status, conclusion);
  }
  else
  {
    Serial.print("Error: "); Serial.println(statusCode);
  }
}

void updateLED (const char* status, const char* conclusion)
{
  digitalWrite(bluePin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);

  if (strcmp(status, "queued") == 0)
  {
    digitalWrite(bluePin, HIGH);
  }
  else if (strcmp(status, "in_progress") == 0)
  {
    digitalWrite(yellowPin, HIGH);
  }
  else if (strcmp(status, "completed") == 0 && strcmp(conclusion, "success") == 0)
  {
    digitalWrite(greenPin, HIGH);
  }
  else if (strcmp(status, "completed") == 0 && strcmp(conclusion, "failure") == 0)
  {
    digitalWrite(redPin, HIGH);
  }
}