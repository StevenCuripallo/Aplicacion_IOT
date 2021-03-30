#include <Ubidots.h>  //Libreria de Ubidots
#include <DHT.h> //Libreria Sensor DHT22

#define DEVICE_LABEL "Node_MCU_8266"  //Creo una constante para almacenar el nombre del dispositivo empleado en Ubidots

#define LAB_LED "led"
#define LAB_TEMP "temperatura"
#define LAB_HUM "humedad"
#define LAB_POT "potenciometro"



#define PIN_LED D0
#define PIN_DHT D2



const char* UBIDOTS_TOKEN = "BBFF-HSVxN87fAgwlE5WH0VJcxQimEMEst8";  //Token Cuenta de Ubidots
const char* WIFI_SSID = "NETLIFE-Creareco.";  // SSID Red Wifi
const char* WIFI_PASS = "#JaqueDavalos1992#";  // Contraseña del Wifi

Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP); //Creamos un objeto para enviar datos a Ubidots
DHT dht(PIN_DHT, DHT22); //Creamos un objeto para manejar el sensor DHT22 seteando el tipo de sensor y el pin en el que se encuentra conectado

int poten, led, humedad; //Variable para almacenar el valor recibido del potenciometro y el sensor DH22, ademas de la variable para almacenar el estado del LED
float temperatura;

void setup() {

  Serial.begin(115200); //Configuro la velocidad de trasferencia de datos para la comunicacion serial
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS); //Llamamos al objeto ubidots con el metodo wificonnect enviandole los datos de la red wifi 
  Serial.println("");
  Serial.println("*****************************************************");
  Serial.print("Conectado a la red Wi-fi: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.println("*****************************************************");

 dht.begin();  //Inicializamos el sensor DHT22
  
  poten = 0;
  temperatura = 0;
  humedad = 0;
  led = 0;
  
  

  pinMode(PIN_LED,OUTPUT);  //Indicamos que el pin al que se encuentra conectado el LED como salida
  digitalWrite(PIN_LED,led);
  
 
  
}

void loop() {
  led = ubidots.get(DEVICE_LABEL, LAB_LED);
  poten = analogRead(A0);  //Lectura del potenciometro en el puerto Analogico 0 del Node MCU ESP8266
  temperatura = dht.readTemperature(); //Leemos el valor de temperatura del sensor DHT22 y lo almacenamos en la variable temperatura
  humedad = dht.readHumidity(); //Leemos el valor de humedad del sensor DHT22 y lo almacenamos en la variable temperatura
  Serial.println("Temperatura="+String(temperatura)+", Humedad="+String(humedad)+", Potenciometro="+String(poten)); //Imprimimos en el monitor serie los valores de temperatura, humedad y del potenciometro
  
  digitalWrite(PIN_LED,led);
  

    
  ubidots.add(LAB_POT, poten); //Llamamos al objeto ubidots con el metodo add para enviar la etiqueta y el valor de la variable del potenciometro a Ubidots
  ubidots.add(LAB_TEMP, temperatura); //Llamamos al objeto ubidots con el metodo add para enviar la etiqueta y el valor de la variable temperatura a Ubidots
  ubidots.add(LAB_HUM, humedad); //Llamamos al objeto ubidots con el metodo add para enviar la etiqueta y el valor de la variable thumedad a Ubidots
  ubidots.add(LAB_LED, led); //Llamamos al objeto ubidots con el metodo add para enviar la etiqueta y el valor de la variable led a Ubidots
  
  
  bool bufferSent = false; //Declaracion de una variable booleana bufferSent para tener un metodo de confirmacion del envio de los datos
  bufferSent = ubidots.send(DEVICE_LABEL); //Envia todos los datos que se hayan empleado con el metodo add

 if (bufferSent){
  Serial.println("Valores enviados por el dispositivo"); //Muestra un mensaje de confirmacion del envio de los datos 
    }
 delay(1000);  //Espera de un segundo
}
