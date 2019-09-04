// This example uses an Adafruit Huzzah ESP8266
// to connect to shiftr.io.
//
// You can check on your device after a successful
// connection here: https://shiftr.io/try.
//
// by Joël Gähwiler
// https://github.com/256dpi/arduino-mqtt

// Include specific libraries for wifi, mqtt & DHT sensor.
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

// pin connected to DH11 data line
#define DATA_PIN 14
DHT_Unified dht(DATA_PIN, DHT11);

// my wifi netowrk ssid and password
const char ssid[] = "CTN @ AWESO 2";
const char pass[] = "aweso4321";

// instantiate mqtt and wifi clients
WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;
char startProcess = true;

// connection function to confirm wifi and mqtt connections, also subscribe to mqtt topics
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("Feather-MO-WIFI-3 #", "83fc5700", "52909f262ae48ccd")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/data/temperature");
  client.subscribe("/data/humidity");
  client.subscribe("/control/switch");
  // client.unsubscribe("/example");
}

// function to handle a received message
void messageReceived(String &topic, String &payload) {
  Serial.println("\nIncoming from mosca broker server: " + topic + " - " + payload);
  if (topic == "/control/switch") {
    Serial.println("\nreceiving control switch message");
    if (payload == "1") {
      Serial.println("setting process to true");
      startProcess = true;
    } else if (payload == "0") {
      Serial.println("setting process to false");
      startProcess = false;
    }
  }
}

void setup() {
  // initialize serial port and wifi.
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  // initialize dht11
  dht.begin();

// Connect to mqtt broker
//  Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
//  You need to set the IP address directly.
  client.begin("192.168.1.71", 1883, net);
  //client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

  // Read DHT-11 sensor
  sensors_event_t event;
  dht.temperature().getEvent(&event);

  float celsius = event.temperature;
  float fahrenheit = (celsius * 1.8) + 32;

  Serial.print("\ncelsius: ");
  Serial.print(celsius);
  Serial.println("C");

  Serial.print("\nfahrenheit: ");
  Serial.print(fahrenheit);
  Serial.println("F");

  dht.humidity().getEvent(&event);

  float humidity = event.relative_humidity;
  Serial.print("\nhumidity: ");
  Serial.print(humidity);
  Serial.println("%");
  if (startProcess == true) {
    // piublish readings to mqtt server
    const String tempString = "{\"board\":\"Feather1\",\"temperature\":" + String(celsius) + "}";
    const String humString = "{\"board\":\"Feather1\",\"humidity\":" + String(humidity) + "}";
    client.publish("/data/temperature", tempString);
    client.publish("/data/humidity", humString);
  }


  // wait 5 seconds (5000 milliseconds == 5 seconds)
  delay(5000);
}