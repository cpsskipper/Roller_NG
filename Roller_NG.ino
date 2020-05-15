#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <Ticker.h>   // ESP8266 Timer
#include <EEPROM.h>


#include "config.h"

//----------------------------------------------------------MQTT
WiFiClient espClient;
PubSubClient client(espClient);
Ticker mqtt_state_timer;
volatile boolean mqtt_state = false;

//---------------------------------------------------------Motor
volatile int motor_position_abs = 0;
volatile int target_position_abs = 0;
volatile boolean position_flag = false;
volatile boolean rotate_flag = false;


/*

int motor_position_target = 0;
byte rotate = 0;   // 0 не вращается, 1 - закрытие (от 100% к 0%), 2 - открытие (от 0 к 100%)
volatile int rotate_target = 0;
volatile boolean rotate_flag = false;

//---------------------------------------------------------Motor timer
//таймер общего времени работы мотора
Ticker motor_timer;
boolean motor_work = false;
volatile boolean motor_need_stop =false;

//таймер текущей позиции шторы
Ticker roller_position_timer;


volatile int rotate_count = 0;
*/
//---------------------------------------------------------EEPROM
// адреса в постоянной памяти
int adr_init_eeprom = 500;
int addres_position = 5;

//---------------------------------------------------------DEBUG
boolean debug = true;

//------------------------------------------------------------------------------------------INTERRUPT FUNCTIONS 
/*
//Событие срабатывает, когда двигатель вращается заданное количество времени
void ICACHE_RAM_ATTR motor_timer_interrupt(){
 // motor_need_stop = true;
}
*/

//Событие срабатывает, для отправки текущей позиции во время движения шторы
void ICACHE_RAM_ATTR roller_position_timer_interrupt(){
  position_flag = true; 
}


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

/*
//----------------------------------- Остановка двигателя
if (motor_need_stop == true){
  motor_need_stop = false;
motor_stop();
motor_position = motor_position_target;
mqtt_send_state();

//Save EEPROM
      EEPROM.write(addres_position, motor_position);
      EEPROM.commit();
}
*/

//----------------------------------- Остановка двигателя по Холлу
if (rotate_flag == true){
  rotate_flag = false;
  motor_stop();
  //motor_position = motor_position_target;
  mqtt_send_state();

  //Save EEPROM
      EEPROM.write(addres_position, position_to_percent(motor_position_abs));
      EEPROM.commit();
  mqtt_send_debug("Position: "+String(motor_position_abs));
  }


//------------------------------------ Расчет и отправка текущей позиции
if (position_flag == true){
  position_flag = false;
  
  //send status mqtt
  mqtt_send_state();
}



if (mqtt_state == true){
  mqtt_state = false; 
  //send status mqtt
  mqtt_send_state();
}

 
}
