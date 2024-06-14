#include <WiFi.h>
#include <HTTPClient.h>
#include "MQ135.h"

#define MQ135_PIN 34  // Pin di mana sensor MQ135 terhubung

MQ135 gasSensor = MQ135(MQ135_PIN);

const char* ssid = "AHR"; // Alamat SSID menyesuaikan
const char* password = "12345678"; // Alamat Password menyesuaikan
const char* serverName = "http://192.168.15.162:5000/sensor/data"; // Alamat IP menyesuaikan

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(5000);
}

void loop() {
  int gasLevel = analogRead(34);

  if (isnan(gasLevel)) {
    Serial.println("Failed to read from MQ135 sensor!");
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData = "gas_level=" + String(gasLevel);

    int httpResponseCode = http.POST(postData);
    Serial.println(http.POST(postData));
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }

  delay(1000); // Kirim data setiap 1 detik
}
