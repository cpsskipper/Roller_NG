
void mqtt_init(){ 
  client.setServer(MQTT_SERVER, MQTT_PORT);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");  
    if (client.connect(CLIENT_ID, MQTT_USER, MQTT_PASSWORD )) {
       Serial.println("connected"); 
    } else { 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000); 
    }
  }
  client.setCallback(callback);
  client.subscribe(CONTROL_TOPIC);
  
   client.publish(ONLINE_TOPIC, CLIENT_ID "online");

    mqtt_state_timer.attach(60, mqtt_status);
}



void callback(char* topic, byte *payload, unsigned int length) {
  
  // Управление в процентах
  if (strcmp(topic, COMMAND_TOPIC) == 0) {
//----------------------------------------- Разбор команды  
    String s = "";
    for (int i = 0; i < length; i++) {
      s = s + (char)payload[i];
    }
    if (s == "STOP"){
      motor_stop();
    }
    else if (isNumeric(s) == true) {
      int i = convert_percent_to_steps(s.toInt());
      if (i != 0){
         //Вращение двигателя в %
         motor_move(i); 
         if (debug) mqtt_send_debug("move to: "+String(convert_percent_to_steps(s.toInt()))); 
      }
    }
  }
  //Установка позиции в процентах
  else if (strcmp(topic, COMMAND_TOPIC_POSITION) == 0) {
//----------------------------------------- Разбор команды  
    String s = "";
    for (int i = 0; i < length; i++) {
      s = s + (char)payload[i];
    }
    if (isNumeric(s) == true) {
      //Вращение двигателя в %
      motor_position_abs = COUNT_STEPS * s.toInt() / 100;
      EEPROM.write(addres_position,s.toInt());
      EEPROM.commit();
      mqtt_send_state();
      if (debug) mqtt_send_debug("Position_abs set to: "+String(motor_position_abs));
      if (debug) mqtt_send_debug("EEPROM: "+String(EEPROM.read(addres_position)));
      
    }
  }
  //Запуск режима настройки шторы
  else if (strcmp(topic, SETUP_ROLLER_TOPIC) == 0) {
      if (debug) mqtt_send_debug("START SETUP ROLLER. Send STOP to CONTROL TOPIC wher ROLLER will DOWN");
      setup_roller = true;
      set_motor_direction(false); // Конфигурация пинов 
      digitalWrite(MOTOR, HIGH);    // Запуск двигателя
      setup_roller_zero = true; //признак, что по команде стоп надо установить нулевое положение motor_position_abs = 0;      
  }
}


void mqtt_send_state() {

  String json = "{\"position\": \"" + String(position_to_percent(motor_position_abs))+ "\", \"status\": \"ON\" }";
  char ch_json[200];
  json.toCharArray(ch_json, 200); // Размер string 199 символов
  client.publish(STATE_TOPIC, ch_json);
  
}

void mqtt_send_debug(String s){

  char ch_json[200];
  s.toCharArray(ch_json, 200); // Размер string 199 символов
  client.publish(DEBUG_TOPIC,ch_json);
}
