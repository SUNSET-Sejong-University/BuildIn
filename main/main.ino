#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

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
  delay(2000);
}

void getBuildStatus()
{
  String path = "/repositories/1051497725/actions/runs?per_page=1";
  client.beginRequest();
  client.get(path);
  client.sendHeader("User-Agent", "ArduinoMKR");
  client.sendHeader("Authorization", "Bearer " + token);
  client.sendHeader("Accept", "application/vnd.github+json");
  client.endRequest();

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  response.toLowerCase();

  Serial.print("HTTP status: ");
  Serial.println(statusCode);

  if (statusCode != 200) 
  {
      Serial.println("Error: Bad HTTP response");
      client.stop();
      return;
  }

  String runStatus = getValue(response, "status");
  String conclusion = getValue(response, "conclusion");

  Serial.print("Status: "); Serial.println(runStatus);
  Serial.print("Conclusion: "); Serial.println(conclusion);
  
  updateLED(runStatus, conclusion);
  client.stop();
}

void updateLED(String status, String conclusion)
{
  digitalWrite(bluePin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);

  if (status == "queued")
  {
    digitalWrite(bluePin, HIGH);
  }
  else if (status == "in_progress")
  {
    digitalWrite(yellowPin, HIGH);
  }
  else if ((status == "completed") && (conclusion == "success"))
  {
    digitalWrite(greenPin, HIGH);
  }
  else if ((status == "completed") && (conclusion == "failure"))
  {
    digitalWrite(redPin, HIGH);
  }
}

String getValue(String &json, const char *key)
{
  int keyIndex = json.indexOf(String("\"") + key + "\":");
  if (keyIndex == -1) return "";
  
  int start = json.indexOf("\"", keyIndex + strlen(key) + 3); // start of value. the 3 is because of the two double quotes and the colon
  int end = json.indexOf("\"", start + 1);
  if (start == -1 || end == -1) return "";

  return json.substring(start + 1, end);
}
