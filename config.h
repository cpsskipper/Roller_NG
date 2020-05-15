const char* client_id = "Roller_NG_1"; // Must be unique on the MQTT network

// wifi settings
const char* ssid     = "you wifi ssid";
const char* password = "wifi pass";

// mqtt server settings
const char* mqtt_server   = "mqtt_server";
const int mqtt_port       = 1883;
const char* mqtt_username = "mqtt_user";
const char* mqtt_password = "mqtt_pass";

char* controlTopic = "home/hall/cover/Roller_NG_1/#";

// mqtt client settings
const char* ha_config_topic         = "homeassistant/cover/Roller_NG_1/config";
const char* online_topic            = "home/online";
const char* command_topic           = "home/hall/cover/Roller_NG_1/set";
const char* command_topic_abs       = "home/hall/cover/Roller_NG_1/set_abs";
const char* command_topic_position  = "home/hall/cover/Roller_NG_1/set_position";
const char* position_topic          = "home/hall/cover/Roller_NG_1/state";
const char* debug_topic             = "home/hall/cover/Roller_NG_1/debug";

// pinouts for Roller_1 и roller 3

const int motor    = 13; 
const int motor_cw = 12;
const int hall = 14;
const byte pin_gerkon = 5;
 


// Направление открытия шторы
const int cw = 0;  //0 -  открывается против часовой стрелки если смотреть на двигатель 1- открывается по часовой стрелке

const int steps_count = 18750;
