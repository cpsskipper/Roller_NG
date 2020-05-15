void motor_init(){
  pinMode(motor, OUTPUT);
  digitalWrite(motor, LOW);
  pinMode(motor_cw, OUTPUT); 
  digitalWrite(motor_cw, LOW);
  pinMode(hall, INPUT_PULLUP); 
  
}

int convert_percent_to_steps(int percent){ // возвращает количество шагов со знаком минус для закрытия и плюс для открытия
  // 0 - закрыто, 100 открыто
    int steps = (steps_count * percent /100) - motor_position_abs;
    if (debug) mqtt_send_debug("Calculated steps "+String(steps)+" , Percent "+String(percent));
    return steps;
}

int position_to_percent(int position_abs){ // возвращает процент открытия шторы. 100 - открыто, 0 закрыто
    int percent = position_abs * 100 / steps_count;
    return percent;
}

void motor_move(int steps){ //направление определяется знаком у переменной steps. Минус для закрытия и плюс для открытия
   motor_stop();
   target_position_abs = motor_position_abs + steps;
   if (steps > 0){
    set_motor_direction(true); // Конфигурация пинов 
    attachInterrupt (digitalPinToInterrupt(hall), move_up_interrupt, FALLING);
    digitalWrite(motor, HIGH);    // Запуск двигателя
   }
   else if (steps < 0){
    set_motor_direction(false); // Конфигурация пинов 
    attachInterrupt (digitalPinToInterrupt(hall), move_down_interrupt, FALLING);
    digitalWrite(motor, HIGH);    // Запуск двигателя
   }
   
}


void motor_stop(){
  digitalWrite(motor, LOW);
  digitalWrite(motor_cw, LOW);
  detachInterrupt(digitalPinToInterrupt(hall));  
}

void set_motor_direction(boolean direct){ //direction true - вверх, false - вниз
  if (direct == true){
    if (cw == 0){                     //0 -  открывается против часовой стрелки если смотреть на двигатель 1- открывается по часовой стрелке
      digitalWrite(motor_cw, LOW);
    }
    else {
      digitalWrite(motor_cw, HIGH);    
    }  
  }
  else if(direct == false){
    if (cw == 0){
      digitalWrite(motor_cw, HIGH);
    }
    else {
      digitalWrite(motor_cw, LOW);    
    }  
  }
}
