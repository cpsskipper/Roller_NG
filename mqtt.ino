
void mqtt_init(){ 
  client.setServer(mqtt_server, mqtt_port);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");  
    if (client.connect(client_id, mqtt_username, mqtt_password )) {
       Serial.println("connected"); 
    } else { 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000); 
    }
  }
  client.setCallback(callback);
  client.subscribe(controlTopic);
  
   client.publish(online_topic, "Roller_NG_1 online");

    mqtt_state_timer.attach(60, mqtt_status);
}



void callback(char* topic, byte *payload, unsigned int length) {
  
  // Управление в процентах
  if (strcmp(topic, command_topic) == 0) {
//----------------------------------------- Разбор команды  
    String s = "";
    for (int i = 0; i < length; i++) {
      s = s + (char)payload[i];
    }
    if (isNumeric(s) == true) {
      int i = convert_percent_to_steps(s.toInt());
      if (i != 0){
         //Вращение двигателя в %
         motor_move(i); 
         if (debug) mqtt_send_debug("move to: "+String(convert_percent_to_steps(s.toInt()))); 
      }
    }
  }
  //Установка позиции в абсолютных значениях
  if (strcmp(topic, command_topic_position) == 0) {
//----------------------------------------- Разбор команды  
    String s = "";
    for (int i = 0; i < length; i++) {
      s = s + (char)payload[i];
    }
    if (isNumeric(s) == true) {
      //Вращение двигателя в %
      motor_position_abs = steps_count * s.toInt() / 100;
      EEPROM.write(addres_position,s.toInt());
      EEPROM.commit();
      mqtt_send_state();
      if (debug) mqtt_send_debug("Position_abs set to: "+String(motor_position_abs));
      if (debug) mqtt_send_debug("EEPROM: "+String(EEPROM.read(addres_position)));
      
    }
  }  
}


void mqtt_send_state() {

  String json = "{\"position\": \"" + String(position_to_percent(motor_position_abs))+ "\", \"status\": \"ON\" }";
  char ch_json[200];
  json.toCharArray(ch_json, 200); // Размер string 199 символов
  client.publish(position_topic, ch_json);
  
}

void mqtt_send_debug(String s){

  char ch_json[200];
  s.toCharArray(ch_json, 200); // Размер string 199 символов
  client.publish(debug_topic,ch_json);
}
