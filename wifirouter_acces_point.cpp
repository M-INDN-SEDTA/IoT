#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "YourWiFiSSID";
const char *password = "YourWiFiPassword";
const char *ap_ssid = "ESP8266-AccessPoint";
const char *ap_password = "1234@1#";

ESP8266WebServer server(80);

// Pins for lights
const int lightsPins[] = {D0, D1, D2, D3}; // Replace with actual pin numbers
const int numLights = 4;

// Pins for fans
const int fansPins[] = {D4, D5}; // Replace with actual pin numbers
const int numFans = 2;

// Pins for ex equipment
const int exPins[] = {D6, D7, D8, D9}; // Replace with actual pin numbers
const int numEx = 4;

// Pins for miscellaneous
const int miscPins[] = {D10, D11}; // Replace with actual pin numbers
const int numMisc = 2;

bool relayStates[numLights + numFans + numEx + numMisc] = {false};

void setup() {
  Serial.begin(115200);

  // Connect to WiFi or create an access point
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Start the server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/toggle", HTTP_POST, handleToggle);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<html><head>";
  html += "<style>"
          "body {font-family: Arial, sans-serif;}"
          ".button {"
          "  display: inline-block;"
          "  background-color: #4CAF50;"
          "  border: none;"
          "  color: white;"
          "  padding: 15px 32px;"
          "  text-align: center;"
          "  text-decoration: none;"
          "  display: inline-block;"
          "  font-size: 16px;"
          "  margin: 4px 2px;"
          "  cursor: pointer;"
          "}"
          ".button-on {background-color: #4CAF50;}"
          ".button-off {background-color: #f44336;}"
          "</style>";
  html += "</head><body>";

  // Lights section
  html += "<h2>Lights</h2>";
  for (int i = 0; i < numLights; i++) {
    html += "<button class='button ";
    html += relayStates[i] ? "button-on" : "button-off";
    html += "' onclick='toggleRelay(" + String(i) + ")'>Light " + String(i + 1) + "</button><br>";
  }

  // Fans section
  html += "<h2>Fans</h2>";
  for (int i = numLights; i < numLights + numFans; i++) {
    html += "<button class='button ";
    html += relayStates[i] ? "button-on" : "button-off";
    html += "' onclick='toggleRelay(" + String(i) + ")'>Fan " + String(i - numLights + 1) + "</button><br>";
  }

  // Ex section
  html += "<h2>Ex Equipment</h2>";
  for (int i = numLights + numFans; i < numLights + numFans + numEx; i++) {
    html += "<button class='button ";
    html += relayStates[i] ? "button-on" : "button-off";
    html += "' onclick='toggleRelay(" + String(i) + ")'>Ex " + String(i - numLights - numFans + 1) + "</button><br>";
  }

  // Miscellaneous section
  html += "<h2>Miscellaneous</h2>";
  for (int i = numLights + numFans + numEx; i < numLights + numFans + numEx + numMisc; i++) {
    html += "<button class='button ";
    html += relayStates[i] ? "button-on" : "button-off";
    html += "' onclick='toggleRelay(" + String(i) + ")'>Misc " + String(i - numLights - numFans - numEx + 1) + "</button><br>";
  }

  html += "<script>"
          "function toggleRelay(relay) {"
          "  var xhttp = new XMLHttpRequest();"
          "  xhttp.onreadystatechange = function() {"
          "    if (this.readyState == 4 && this.status == 200) {"
          "      location.reload();"
          "    }"
          "  };"
          "  xhttp.open('POST', '/toggle', true);"
          "  xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');"
          "  xhttp.send('relay=' + relay);"
          "}"
          "</script>";

  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleToggle() {
  if (server.hasArg("relay")) {
    int relay = server.arg("relay").toInt();
    if (relay >= 0 && relay < numLights + numFans + numEx + numMisc) {
      relayStates[relay] = !relayStates[relay];
      // Add your code here to control the relay based on relayStates[relay]
    }
  }
  server.send(200, "text/plain", "OK");
}
