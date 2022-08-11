#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <Servo.h>

#include <HCSR04.h>

#define BUZZER_PIN D1
#define LED_PIN D2
#define SERVO_PIN D3

#define TRIG_PIN D5
#define ECHO_PIN D6

UltraSonicDistanceSensor distanceSensor(TRIG_PIN, ECHO_PIN);

enum State
{
  Open,
  Close,
  Opening,
  Closing,
  Obstacle
};

State command = Close;
unsigned long open_time;

const char *ssid = "ssid";
const char *password = "pass";

ESP8266WebServer server(80);

Servo servo;
#define CLOSE_DEG 10
#define OPEN_DEG 170
#define STEPS 2
int servo_deg = CLOSE_DEG;

String baseIndex()
{
  String index_html = "";
  index_html += "<!DOCTYPE html>";
  index_html += "<html><head><title>Parking Door</title></head>";
  index_html += "<body><h2>Parking Door</h2>";
  index_html += "<button onclick=\"fetch('/open');\">Open</button>";
  index_html += "<button onclick=\"fetch('/close');\">Close</button>";
  index_html += "<p>Message: <span id=\"message\"></span></p>";
  index_html += "<script>";
  index_html += "setMessage = (message) => document.getElementById(\"message\").textContent = message;";
  index_html += "loader = () => fetch(\"/status\").then(response => response.text().then(setMessage))";
  index_html += ".catch(() => console.log(\"Error\")).finally(() => setTimeout(loader, 500));";
  index_html += "loader();";
  index_html += "</script>";
  index_html += "</body></html>";

  return index_html;
}

void handleRoot()
{
  server.send(200, "text/html", baseIndex());
}

void handleStatus()
{
  String status;
  switch (command)
  {
  case Open:
    status = "Open";
    break;
  case Close:
    status = "Close";
    break;
  case Opening:
    status = "Opening";
    break;
  case Closing:
    status = "Closing";
    break;
  case Obstacle:
    status = "Obstacle";
    break;
  default:
    break;
  }
  server.send(200, "text/plain", status);
}

void handleOpen()
{
  command = Opening;
  server.send(200, "text/plain", "OK");
}

void handleClose()
{
  command = Closing;
  server.send(200, "text/plain", "OK");
}

void setup()
{
  delay(1000);
  Serial.begin(9600);
  Serial.println();

  // Serial.print("Configuring access point...");
  // WiFi.softAP(ssid, password);
  // Serial.print("AP IP address: ");
  // Serial.println(WiFi.softAPIP());

  Serial.print("Connecting to ");
  Serial.print(ssid);
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

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  servo.attach(SERVO_PIN);
  servo.write(servo_deg);

  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/open", handleOpen);
  server.on("/close", handleClose);
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
  if (command == Opening)
  {
    digitalWrite(LED_PIN, HIGH);
    if (servo_deg >= OPEN_DEG)
    {
      command = Open;
      open_time = millis();
    }
    else
      servo_deg += STEPS;
  }
  else if (command == Closing || command == Obstacle)
  {
    if (CLOSE_DEG >= servo_deg)
    {
      command = Close;
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
    }
    else if (distanceSensor.measureDistanceCm() < 10.0)
    {
      servo_deg = min(servo_deg + STEPS, OPEN_DEG);
      digitalWrite(BUZZER_PIN, HIGH);
      command = Obstacle;
    }
    else
    {
      servo_deg -= STEPS;
      command = Closing;
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
  if (command == Open && millis() - open_time > 20000)
    command = Closing;
  servo.write(servo_deg);
  delay(40);
}#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <Servo.h>

#include <HCSR04.h>

#define BUZZER_PIN D1
#define LED_PIN D2
#define SERVO_PIN D3

#define TRIG_PIN D5
#define ECHO_PIN D6

UltraSonicDistanceSensor distanceSensor(TRIG_PIN, ECHO_PIN);

enum State
{
  Open,
  Close,
  Opening,
  Closing,
  Obstacle
};

State command = Close;
unsigned long open_time;

const char *ssid = "ssid";
const char *password = "pass";

ESP8266WebServer server(80);

Servo servo;
#define CLOSE_DEG 10
#define OPEN_DEG 170
#define STEPS 2
int servo_deg = CLOSE_DEG;

String baseIndex()
{
  String index_html = "";
  index_html += "<!DOCTYPE html>";
  index_html += "<html><head><title>Parking Door</title></head>";
  index_html += "<body><h2>Parking Door</h2>";
  index_html += "<button onclick=\"fetch('/open');\">Open</button>";
  index_html += "<button onclick=\"fetch('/close');\">Close</button>";
  index_html += "<p>Message: <span id=\"message\"></span></p>";
  index_html += "<script>";
  index_html += "setMessage = (message) => document.getElementById(\"message\").textContent = message;";
  index_html += "loader = () => fetch(\"/status\").then(response => response.text().then(setMessage))";
  index_html += ".catch(() => console.log(\"Error\")).finally(() => setTimeout(loader, 500));";
  index_html += "loader();";
  index_html += "</script>";
  index_html += "</body></html>";

  return index_html;
}

void handleRoot()
{
  server.send(200, "text/html", baseIndex());
}

void handleStatus()
{
  String status;
  switch (command)
  {
  case Open:
    status = "Open";
    break;
  case Close:
    status = "Close";
    break;
  case Opening:
    status = "Opening";
    break;
  case Closing:
    status = "Closing";
    break;
  case Obstacle:
    status = "Obstacle";
    break;
  default:
    break;
  }
  server.send(200, "text/plain", status);
}

void handleOpen()
{
  command = Opening;
  server.send(200, "text/plain", "OK");
}

void handleClose()
{
  command = Closing;
  server.send(200, "text/plain", "OK");
}

void setup()
{
  delay(1000);
  Serial.begin(9600);
  Serial.println();

  // Serial.print("Configuring access point...");
  // WiFi.softAP(ssid, password);
  // Serial.print("AP IP address: ");
  // Serial.println(WiFi.softAPIP());

  Serial.print("Connecting to ");
  Serial.print(ssid);
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

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  servo.attach(SERVO_PIN);
  servo.write(servo_deg);

  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/open", handleOpen);
  server.on("/close", handleClose);
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
  if (command == Opening)
  {
    digitalWrite(LED_PIN, HIGH);
    if (servo_deg >= OPEN_DEG)
    {
      command = Open;
      open_time = millis();
    }
    else
      servo_deg += STEPS;
  }
  else if (command == Closing || command == Obstacle)
  {
    if (CLOSE_DEG >= servo_deg)
    {
      command = Close;
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
    }
    else if (distanceSensor.measureDistanceCm() < 10.0)
    {
      servo_deg = min(servo_deg + STEPS, OPEN_DEG);
      digitalWrite(BUZZER_PIN, HIGH);
      command = Obstacle;
    }
    else
    {
      servo_deg -= STEPS;
      command = Closing;
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
  if (command == Open && millis() - open_time > 20000)
    command = Closing;
  servo.write(servo_deg);
  delay(40);
}
