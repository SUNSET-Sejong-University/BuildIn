#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#include "secrets.h"

/* WiFi Credentials */
char ssid[] = SSID;           // the network SSID (name)
char pass[] = PASSWORD;       // the network password
int wifiStatus = WL_IDLE_STATUS;  // the WiFi radio's status

/* GitHub API */
char server[] = "img.shields.io";
String owner = "Prithwis-2023";
String repo = "BuildIn";
String token= GH_TOKEN;
int repoID = 1051497725;

const int bluePin = 2;
const int yellowPin = 7;
const int redPin = 5;
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
    
    Serial.println("Attempting...");
    wifiStatus = WiFi.begin(ssid, pass);  // connect to the network
    delay(10000);                     // wait 10 seconds for conection
  }
  Serial.println("Connected to WiFi!");
}

void loop()
{
  getBuildStatus();
  delay(10000);
}

void getBuildStatus()
{
  String path = "/github/actions/workflow/status/SUNSET-Sejong-University/BuildIn/main.yml";
  client.beginRequest();
  client.get(path);
  client.sendHeader("User-Agent", "ArduinoMKR");
  client.sendHeader("Authorization", "Bearer " + token);
  //client.sendHeader("Accept", "application/vnd.github+json");
  client.endRequest();

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("HTTP status: ");
  Serial.println(statusCode);

  if (statusCode != 200) 
  {
      Serial.println("Error: Bad HTTP response");
      return;
  }

  response.toLowerCase();

  if (response.indexOf("passing") >= 0) {
      digitalWrite(greenPin, HIGH);
      digitalWrite(redPin, LOW);
      digitalWrite(yellowPin, LOW);
  } 
  else if (response.indexOf("failing") >= 0) {
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      digitalWrite(yellowPin, LOW);
  } 
  else {
      digitalWrite(yellowPin, HIGH); // unknown/error
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
  }

  // StaticJsonDocument<4096> doc;  
  // if (deserializeJson(doc, response) != DeserializationError::Ok) 
  // {
  //     Serial.println("JSON parse failed!");
  //     return;
  // }

  // const char* runStatus = doc["workflow_runs"][0]["status"];
  // const char* conclusion = doc["workflow_runs"][0]["conclusion"];
  
  // Serial.print("Status: ");
  // Serial.println(runStatus ? runStatus : "null");

  // Serial.print("Conclusion: ");
  // Serial.println(conclusion ? conclusion : "null");

  // updateLED(runStatus, conclusion);
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