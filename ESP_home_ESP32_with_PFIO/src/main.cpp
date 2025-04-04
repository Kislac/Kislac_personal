#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "Kis-Szikora";
const char* password = "Kishangya";
const char* mqtt_server = "192.168.0.212"; // MQTT broker IP (pl. HA-n Mosquitto)

int last_brightness = 0; // Az utolsó fényerő érték tárolására
WiFiClient espClient;
PubSubClient client(espClient);

const int ledPin = 2; // vagy ahol a LED van

void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) msg += (char)payload[i];

  Serial.print("Üzenet érkezett a témára: ");
  Serial.print(topic);
  Serial.print(" - Üzenet: ");
  Serial.println(msg);

  if (String(topic) == "esp32/led") {
    static String state = "OFF";
    static int brightness = 0;

    // Próbáljuk meg az üzenetet számként értelmezni (fényerősség)
    bool isNumeric = true;
    for (char c : msg) {
      if (!isDigit(c)) {
        isNumeric = false;
        break;
      }
    }

    if (isNumeric) {
      brightness = constrain(msg.toInt(), 0, 255);
      last_brightness = brightness; // Frissítjük az utolsó fényerő értéket
      if(brightness > 0) {
        state = "ON"; // Ha a fényerő nagyobb mint 0, akkor ON
      } else {
        state = "OFF"; // Ha a fényerő 0, akkor OFF
      }
    } else {
      // Ha nem szám, akkor ON/OFF állapotot várunk
      if (msg == "ON") {
        state = "ON";
        brightness = last_brightness; // Visszaállítjuk az utolsó fényerő értéket
        if (last_brightness == 0) {
          brightness = 255; // Ha az utolsó fényerő 0 volt, akkor max fényerő
        }
      } else if (msg == "OFF") {
        state = "OFF";
        last_brightness = brightness;
        brightness = 0; // OFF esetén a fényerő 0
      } else {
        Serial.println("Ismeretlen üzenet!");
        return;
      }
    }

    // Állapot alkalmazása
    analogWrite(ledPin, brightness);
    Serial.print("LED állapot: ");
    Serial.print(state);
    Serial.print(", fényerő: ");
    Serial.println(brightness);

    // JSON visszajelzés a HA-nak
    char json[64];
    snprintf(json, sizeof(json), "{\"state\":\"%s\",\"brightness\":%d}", state.c_str(), brightness);
    Serial.print("JSON üzenet: ");
    Serial.println(json);
    client.publish("esp32/led/state", json, true);
  }
}

void publishDiscoveryMessages() {
  if (!client.connected()) return;  // Ne próbáljunk publish-olni, ha nincs kapcsolat

  Serial.println("Publishing discovery...");
  // LED automatikus felismerés (light)
  const char* light_discovery_topic = "homeassistant/light/esp32_led/config";
  const char* light_discovery_payload =
  "{"
    "\"name\":\"ESP32 LED\","
    "\"uniq_id\":\"esp32_led_1\","
    "\"state_topic\":\"esp32/led/state\","
    "\"command_topic\":\"esp32/led\","
    "\"brightness_state_topic\":\"esp32/led/state\","
    "\"brightness_command_topic\":\"esp32/led\","
    "\"state_value_template\":\"{{ value_json.state }}\","
    "\"brightness_value_template\":\"{{ value_json.brightness }}\","
    "\"brightness\":true,"
    "\"brightness_scale\":255,"
    "\"payload_on\":\"ON\","
    "\"payload_off\":\"OFF\","
    "\"device\":{"
      "\"name\":\"ESP32 LED Modul\","
      "\"identifiers\":[\"esp32_led_device\"],"
      "\"manufacturer\":\"SajatProjekt\","
      "\"model\":\"CustomESP32\","
      "\"sw_version\":\"1.0\""
    "}"
  "}";
  bool result_led = client.publish(light_discovery_topic, light_discovery_payload, true);
  Serial.print("LED discovery publish result: ");
  Serial.println(result_led ? "OK" : "FAILED");
  // RSSI automatikus felismerés (sensor)
  const char* rssi_discovery_topic = "homeassistant/sensor/esp32_wifi_rssi/config";
  const char* rssi_discovery_payload =
    "{"
    "\"name\":\"ESP32 WiFi RSSI\","
    "\"uniq_id\":\"esp32_wifi_rssi_1\","
    "\"state_topic\":\"esp32/wifi/rssi\","
    "\"unit_of_measurement\":\"dBm\","
    "\"device_class\":\"signal_strength\","
    "\"device\":{"
      "\"name\":\"ESP32 LED Modul\","
      "\"identifiers\":[\"esp32_led_device\"]"
    "}"
    "}";
  bool result_rssi = client.publish(rssi_discovery_topic, rssi_discovery_payload, true);
  Serial.print("RSSI discovery publish result: ");
  Serial.println(result_rssi ? "OK" : "FAILED");
}

bool MQTT_Connect() {
  if (client.connect("esp32-led", "mqtt_esp", "mqtt_esp")) {
    Serial.println("MQTT kapcsolat sikeres!");
    client.subscribe("esp32/led");

    // Discovery topicok küldése
    publishDiscoveryMessages();
    return true;
  } else {
    Serial.print("MQTT kapcsolat sikertelen, hiba kód: ");
    Serial.println(client.state());
    return false;
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Kapcsolódás az MQTT szerverhez: ");
    Serial.println(mqtt_server);

    if (MQTT_Connect()) {
      Serial.println("Kapcsolódás sikeres!");
      break;
    } else {
      Serial.print("MQTT kapcsolat sikertelen, hiba kód: ");
      Serial.print(client.state());
      Serial.println(". Újrapróbálkozás 5 másodperc múlva...");
      delay(5000);
    }
  }
}

void publishWiFiRSSI() {
  long rssi = WiFi.RSSI(); // Lekérdezi az aktuális WiFi jel erősséget
  char rssiStr[10];
  snprintf(rssiStr, sizeof(rssiStr), "%ld", rssi); // Átalakítja szöveggé
  Serial.print("WiFi RSSI: ");
  Serial.println(rssiStr); // Kiírja a soros monitorra
  // MQTT üzenet publikálása
  client.publish("esp32/wifi/rssi", rssiStr, true); // Publikálja az MQTT szerverre
 
}

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200); // Soros kommunikáció indítása
  WiFi.begin(ssid, password);
  // WiFi állapot kiírása
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Csatlakozás a WiFi hálózathoz...");
    delay(500);
  }

  Serial.println("WiFi csatlakoztatva!");
  Serial.print("IP cím: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // MQTT állapot ellenőrzése
  Serial.print("Kapcsolódás az MQTT szerverhez: ");
  Serial.println(mqtt_server);

  if (MQTT_Connect()) {
    Serial.println("Kapcsolódás sikeres!");
  } else {
    Serial.print("MQTT kapcsolat sikertelen, hiba kód: ");
    Serial.println(client.state());
  }
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  static unsigned long lastRSSIPublish = 0;
  if (millis() - lastRSSIPublish > 5000) { // 5 másodpercenként publikálja az RSSI-t
    publishWiFiRSSI();
    lastRSSIPublish = millis();
  }


}
