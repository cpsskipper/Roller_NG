void motor_init(){
  pinMode(MOTOR, OUTPUT);
  digitalWrite(MOTOR, LOW);
  pinMode(MOTOR_CW, OUTPUT); 
  digitalWrite(MOTOR_CW, LOW);
  pinMode(HALL_SENSOR, INPUT_PULLUP); 
  
}

int convert_percent_to_steps(int percent){ // возвращает количество шагов со знаком минус для закрытия и плюс для открытия
  // 0 - закрыто, 100 открыто
    int steps = (COUNT_STEPS * percent /100) - motor_position_abs;
    if (debug) mqtt_send_debug("Calculated steps "+String(steps)+" , Percent "+String(percent));
    return steps;
}

int position_to_percent(int position_abs){ // возвращает процент открытия шторы. 100 - открыто, 0 закрыто
    int percent = position_abs * 100 / COUNT_STEPS;
    return percent;
}

void motor_move(int steps){ //направление определяется знаком у переменной steps. Минус для закрытия и плюс для открытия
   motor_stop();
   mqtt_state_timer.detach();
   target_position_abs = motor_position_abs + steps;
   if (steps > 0){
    set_motor_direction(true); // Конфигурация пинов 
    attachInterrupt (digitalPinToInterrupt(HALL_SENSOR), move_up_interrupt, FALLING);
    digitalWrite(MOTOR, HIGH);    // Запуск двигателя
    mqtt_state_timer.attach(2, mqtt_status);
   }
   else if (steps < 0){
    set_motor_direction(false); // Конфигурация пинов 
    attachInterrupt (digitalPinToInterrupt(HALL_SENSOR), move_down_interrupt, FALLING);
    digitalWrite(MOTOR, HIGH);    // Запуск двигателя
    mqtt_state_timer.attach(2, mqtt_status);
   }
   
}


void motor_stop(){
  digitalWrite(MOTOR, LOW);
  digitalWrite(MOTOR_CW, LOW);
  detachInterrupt(digitalPinToInterrupt(HALL_SENSOR)); 
  mqtt_state_timer.detach();
  mqtt_state_timer.attach(60, mqtt_status);
  
  //режим настройки шторы
  if (setup_roller == true && setup_roller_zero == true){  // Установка нулевого положения
    setup_roller_zero = false;
    motor_position_abs = 0;
    set_motor_direction(true); // Конфигурация пинов 
    target_position_abs = 100000; //Ну типа большое число
    attachInterrupt (digitalPinToInterrupt(HALL_SENSOR), move_up_interrupt, FALLING);
    digitalWrite(MOTOR, HIGH);    // Запуск двигателя
    mqtt_send_state();
    if (debug) mqtt_send_debug("Set zero position. Send STOP to CONTROL TOPIC wher ROLLER will TOP");
  }
  else if (setup_roller == true && setup_roller_zero == false){ // Установка длины шторы
    setup_roller = false;
    COUNT_STEPS = motor_position_abs;
    EEPROMWriteInt(address_steps, COUNT_STEPS);
    mqtt_send_state();
    if (debug) mqtt_send_debug("Set COUNT_STEPS = " + String(motor_position_abs));
    if (debug) mqtt_send_debug("ROLLER SETUP SUCCESSFUL");
  }
  
  //Save EEPROM
  EEPROM.write(addres_position, position_to_percent(motor_position_abs));
  EEPROM.commit(); 
}

void set_motor_direction(boolean direct){ //direction true - вверх, false - вниз
  if (direct == true){
    if (CW == 0){                     //0 -  открывается против часовой стрелки если смотреть на двигатель 1- открывается по часовой стрелке
      digitalWrite(MOTOR_CW, LOW);
    }
    else {
      digitalWrite(MOTOR_CW, HIGH);    
    }  
  }
  else if(direct == false){
    if (CW == 0){
      digitalWrite(MOTOR_CW, HIGH);
    }
    else {
      digitalWrite(MOTOR_CW, LOW);    
    }  
  }
}
