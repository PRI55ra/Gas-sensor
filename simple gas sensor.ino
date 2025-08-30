#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "xxxxxxxxxxx";
const char* password = "hello xxxxx";

// Telegram details
String botToken = "000000:00000000000000000";  // From BotFather
String chatID   = "00000000000000000000";    // From @userinfobot

int gasPin = 34;   // MQ2 sensor analog pin
int threshold = 2000;  // Adjust after testing your sensor

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

void loop() {
  int gasValue = analogRead(gasPin);
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  if (gasValue > threshold) {
    sendTelegram("⚠️ ALERT! Gas Leak Detected. Please check immediately!");
    delay(15000); // wait 15 sec before sending next alert
  }

  delay(1000);
}

void sendTelegram(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.telegram.org/bot" + botToken + "/sendMessage?chat_id=" + chatID + "&text=" + message;
    
    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.println("Telegram Message Sent!");
      Serial.println(http.getString());
    } else {
      Serial.print("Error sending message: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
}
