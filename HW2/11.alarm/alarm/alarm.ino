#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

#define BUZZER_PIN D1

bool alarm_set = false;

const char *ssid = "ssid";
const char *password = "pass";

int hour = 10, minute = 0, second = 0;
int volume = 50;

ESP8266WebServer server(80);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 16200);

String baseIndex()
{
  String index_html = "";
  index_html += "";
  index_html += "<!DOCTYPE html>";
  index_html += "<html><head><title>Alarm</title></head>";
  index_html += "<body>";
  index_html += "<form method=\"get\" action=\"/\"><h2>Alarm</h2>";
  index_html += "Time: <input name=\"time\" type=\"time\" step=\"1\" value=\"" + String(hour) + ":" + String(minute) + ":" + String(second) + "\" onchange=\"this.form.submit()\" /><br />";
  index_html += "sound volume: <input name=\"volume\" type=\"range\" value=\"" + String(volume) + "\" onchange=\"this.form.submit()\" /><br />";
  index_html += "<button type=\"submit\" formaction=\"/stop\">Stop</button>";
  index_html += "<button type=\"submit\" formaction=\"/snooze\">Snooze</button>";
  index_html += "</form></body></html>";

  return index_html;
}

void handleRoot()
{
  hour = server.arg("time").substring(0, 2).toInt();
  minute = server.arg("time").substring(3, 5).toInt();
  second = server.arg("time").substring(6).toInt();

  volume = server.arg("volume").toInt();

  alarm_set = true;

  Serial.printf("Time set %.2d:%.2d:%.2d with volume %d\n", hour, minute, second, volume);

  server.send(200, "text/html", baseIndex());
}

void handleSnooze()
{
  analogWrite(BUZZER_PIN, 0);
  second += 10;
  if (second >= 60)
  {
    minute += second / 60;
    second %= 60;
    if (minute >= 60)
    {
      hour += minute / 60;
      minute %= 60;
      hour %= 24;
    }
  }

  alarm_set = true;

  Serial.printf("Alarm snoozed for %.2d:%.2d:%.2d with volume %d\n", hour, minute, second, volume);

  server.send(200, "text/html", baseIndex());
}

void handleStop()
{
  analogWrite(BUZZER_PIN, 0);
  Serial.println("Alarm Stoped.");
  server.send(200, "text/html", baseIndex());
}

void setup()
{
  delay(1000);
  Serial.begin(9600);
  Serial.println();
  Serial.print("Configuring access point...");

  // WiFi.softAP(ssid, password);

  // Serial.print("AP IP address: ");
  // Serial.println(WiFi.softAPIP());

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(BUZZER_PIN, OUTPUT);

  timeClient.begin();

  server.on("/", handleRoot);
  server.on("/stop", handleStop);
  server.on("/snooze", handleSnooze);
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
  timeClient.update();
//  Serial.printf("%.2d:%.2d:%.2d  -  %.2d:%.2d:%.2d\n", hour, minute, second, timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds());
  if (timeClient.getHours() == hour && timeClient.getMinutes() == minute && timeClient.getSeconds() == second && alarm_set)
  {
    Serial.println("Alarm!!!!!!!!!!!!");
    analogWrite(BUZZER_PIN, map(volume, 0, 100, 0, 255));
    alarm_set = false;
  }
}
