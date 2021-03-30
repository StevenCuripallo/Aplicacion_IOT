#include <CayenneMQTTESP8266.h>
#include <DHT.h>
#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#define VIRTUAL_CHANNEL_FOCO 1
#define VIRTUAL_CHANNEL_LED 2
#define VIRTUAL_CHANNEL_POT 3
#define FOCO_PIN D1
#define LED_PIN D0
#define POT_PIN A0

char ssid[] = "NETLIFE-Creareco.";
char password[] = "#JaqueDavalos1992#";

char username[] = "360d7000-814c-11eb-a2e4-b32ea624e442";
char mqtt_password[] = "937f1cb958494f4bc8392dbfac9c108ef618a0fa";
char client_id[] = "66ea97c0-814c-11eb-b767-3f1a8f1211ba";

DHT dht(D2, DHT22);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Cayenne.begin(username, mqtt_password, client_id, ssid, password);
  pinMode(FOCO_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode (POT_PIN, INPUT);
  }

void loop() {
  // put your main code here, to run repeatedly:
  Cayenne.loop();
  float temp = (dht.readTemperature(true)-32)/1.8000;
  float hum = dht.readHumidity();
  Cayenne.virtualWrite(1, temp, TYPE_TEMPERATURE, UNIT_CELSIUS);
  Cayenne.virtualWrite(2, hum, TYPE_RELATIVE_HUMIDITY, UNIT_PERCENT);
  Cayenne.virtualWrite(VIRTUAL_CHANNEL_POT, analogRead(POT_PIN), "analog_sensor", "null");
  Serial.println(analogRead(POT_PIN));
}

CAYENNE_IN(VIRTUAL_CHANNEL_FOCO)
{
  int value = getValue.asInt();
  CAYENNE_LOG("Channel %d, pin %d, value %d", VIRTUAL_CHANNEL_FOCO, FOCO_PIN, value);
  // Write the value received to the digital pin.

  if (value == 0)
  {digitalWrite(FOCO_PIN, HIGH);
}
  else if (value == 1){
    digitalWrite(FOCO_PIN, LOW);
  }
 
}

CAYENNE_IN(VIRTUAL_CHANNEL_LED)
{
  int value1 = getValue.asInt();
  CAYENNE_LOG("Channel %d, pin %d, value %d", VIRTUAL_CHANNEL_LED, LED_PIN, value1);
  // Write the value received to the digital pin.

  digitalWrite(LED_PIN, value1);
}





// This function is called at intervals to send data to Cayenne and keep the device online.
// Will create a temporary green widget on Channel 0, make it permanent by clicking on '+'. 
CAYENNE_OUT(0)
{
  CAYENNE_LOG("Send data for Virtual Channel 0");
  // This command writes the device's uptime in seconds to the Virtual Channel. 
  Cayenne.virtualWrite(0, millis() / 1000);
}
