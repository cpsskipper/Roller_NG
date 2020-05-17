#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <Ticker.h>   // ESP8266 Timer
#include <EEPROM.h>

#include "secret.h" // Для заполнения настроек подключения к WI-FI и MQTT. При заполнении настроек в config.h нужно закомментировать эту строчку
#include "config.h"

//----------------------------------------------------------MQTT
WiFiClient espClient;
PubSubClient client(espClient);
Ticker mqtt_state_timer;
volatile boolean mqtt_state = false;

//---------------------------------------------------------Motor
volatile int motor_position_abs = 0;
volatile int target_position_abs = 0;
volatile boolean rotate_flag = false;

//режим настройки шторы
boolean setup_roller = false;
boolean setup_roller_zero = false;

//---------------------------------------------------------EEPROM
// адреса в постоянной памяти
#define adr_init_eeprom  500
#define addres_position  5
#define address_steps    50

//---------------------------------------------------------DEBUG
boolean debug = true;

//------------------------------------------------------------------------------------------INTERRUPT FUNCTIONS 



//Событие срабатывает каждые 60 секунд для отправки статуса шторы на сервер
void ICACHE_RAM_ATTR mqtt_status(){
  mqtt_state = true;
}


//События срабатывает по датчику Холла

// Опускание шторы. Внизу абсолютная позиция  0
void ICACHE_RAM_ATTR move_down_interrupt(){
  motor_position_abs--;
  if (motor_position_abs <= target_position_abs){
      rotate_flag = true;
  }
}


// Поднятие шторы. Вверху абсолютная позиция  100
void ICACHE_RAM_ATTR move_up_interrupt(){
  motor_position_abs++;
  if (motor_position_abs >= target_position_abs){
      rotate_flag = true;
  }
}


//------------------------------------------------------------------------------------------SETUP
void setup() {  
 //Serial.begin(9600);
 // Serial.println("Hello ESP8266");
  motor_init();
  init_wifi();
  ota_init();
  EEPROM_init();
  int i = motor_position_abs;
  mqtt_init();
  if (debug) mqtt_send_debug("Position abs load: "+String(i));
  if (debug) mqtt_send_debug("STEPS at load: "+String(COUNT_STEPS));
   
}


//------------------------------------------------------------------------------------------LOOP
void loop() {
 
  
//----------------------------------- Проверка WiFi
  if (WiFi.status() != WL_CONNECTED){
   init_wifi();
  }
  
//----------------------------------- Обработка прошивки по воздуху
  ArduinoOTA.handle();
  
//----------------------------------- Проверка MQTT
  if (!client.connected()) {
    Serial.println("Attempting to reconnect MQTT...");
    mqtt_init();
  }

//----------------------------------- считывание очереди MQTT
client.loop();

//----------------------------------- Остановка двигателя по Холлу
if (rotate_flag == true){
  rotate_flag = false;
  motor_stop();
  mqtt_send_state();
  if (debug) mqtt_send_debug("Position: "+String(motor_position_abs));
  }


//------------------------------------ Расчет и отправка текущей позиции

if (mqtt_state == true){
  mqtt_state = false; 
  //send status mqtt
  mqtt_send_state();
}

 
}
