//Unique name of device
#define CLIENT_ID        "Roller_NG_1" // Must be unique!


// wifi settings from secret.h 
#define WIFI_SSID        wifi_ssid
#define WIFI_PASSWORD    wifi_password

// mqtt server settings from secret.h
#define MQTT_SERVER       mqtt_server
#define MQTT_PORT         mqtt_port
#define MQTT_USER         mqtt_user
#define MQTT_PASSWORD     mqtt_password

#define MAIN_TOPIC              "home"
#define ROOM                    "hall"
#define DEVICE_TYPE             "cover"
#define HOME_ASSISTANT_TOPIC    "homeassistant"

//Топики                     
#define CONTROL_TOPIC            MAIN_TOPIC "/" ROOM "/" DEVICE_TYPE "/" CLIENT_ID "/#"  //"home/hall/cover/Roller_NG_1/#"
#define HA_CONFIG_TOPIC          HOME_ASSISTANT_TOPIC "/" DEVICE_TYPE "/" CLIENT_ID "/config"
#define ONLINE_TOPIC             MAIN_TOPIC "/online"
#define COMMAND_TOPIC            MAIN_TOPIC "/" ROOM "/" DEVICE_TYPE "/" CLIENT_ID "/set"
#define COMMAND_TOPIC_POSITION   MAIN_TOPIC "/" ROOM "/" DEVICE_TYPE "/" CLIENT_ID "/set_position"
#define STATE_TOPIC              MAIN_TOPIC "/" ROOM "/" DEVICE_TYPE "/" CLIENT_ID "/state"
#define DEBUG_TOPIC              MAIN_TOPIC "/" ROOM "/" DEVICE_TYPE "/" CLIENT_ID "/debug"
#define SETUP_ROLLER_TOPIC       MAIN_TOPIC "/" ROOM "/" DEVICE_TYPE "/" CLIENT_ID "/setup"  //Запуск режима настройки начального положения шторы. Необходимо нажимать стоп на крайних положениях

/*
ha_config_topic         = "homeassistant/cover/Roller_NG_1/config";
online_topic            = "home/online";
command_topic           = "home/hall/cover/Roller_NG_1/set";
command_topic_abs       = "home/hall/cover/Roller_NG_1/set_abs";
command_topic_position  = "home/hall/cover/Roller_NG_1/set_position";
position_topic          = "home/hall/cover/Roller_NG_1/state";
debug_topic             = "home/hall/cover/Roller_NG_1/debug";
*/
// pinouts for Roller_1 и roller 3

 #define MOTOR        13 
 #define MOTOR_CW     12
 #define HALL_SENSOR  14

 
// pinouts for Roller_2
//#define MOTOR        12 
//#define MOTOR_CW     14
//#define HALL_SENSOR  5
//#define GERKON       13


// Направление открытия шторы
#define CW           0  //0 -  открывается против часовой стрелки если смотреть на двигатель 1- открывается по часовой стрелке
int COUNT_STEPS = 18750;
