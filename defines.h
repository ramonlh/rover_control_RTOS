
// definidos en fichero user_setup.h de la libreira TFT_eSPI
// para PLACA EXPERIMENTAL V1
//#define TFT_MISO   19
//#define TFT_MOSI   23
//#define TFT_SCLK   18
//#define TFT_CS     15
//#define TFT_DC      2
//#define TFT_RST     4    // Set TFT_RST to -1 if the display RESET is connected to RST or 3.3V
//#define TOUCH_CS    5

constexpr uint8_t WIFI_TRIGGER_PIN = 0;    
constexpr uint8_t pin_radio_control = 32;
constexpr uint8_t pin_US1_receptor = 34;
constexpr uint8_t pin_US1_emisor = 33; 
constexpr uint8_t pin_led_7colores = 27;
constexpr uint8_t pin_radar = 14;
constexpr uint8_t pin_SDA = 21;
constexpr uint8_t pin_SCL = 22;
constexpr uint8_t pin_DHT11 = 13;

const IPAddress WIFI_IP(192,168,11,91);
const IPAddress WIFI_GW(192,168,11,1);
const IPAddress WIFI_MASK(255,255,255,0);

constexpr uint16_t PORT_WEBSERVER = 80;
constexpr uint16_t PORT_WEBSOCKET = 81;
constexpr uint16_t PORT_TCPSOCKET = 82;
constexpr char password_OTA[] = "1892";  // Mejor con array de caracteres

constexpr uint8_t i2caddress_giroscopio = 0x50;

// parámetros tareas
const int periodo_task_DHT11 = 10000;
const int periodo_task_radar = 1000;
const int periodo_task_ultrasonidos = 500;
const int periodo_task_radiocontrol = 10;
const int periodo_task_giroscopio = 500;
const int periodo_task_websockets = 500;
const int periodo_task_servomotores = 100;
const int periodo_task_motores = 100;

const int prioridad_task_DHT = 1;           // a mayor número, más prioridad de la tarea
const int prioridad_task_radar = 1;
const int prioridad_task_ultrasonidos= 2;
const int prioridad_task_radiocontrol = 2;
const int prioridad_task_giroscopio = 3;
const int prioridad_task_websockets = 3;
const int prioridad_task_servomotores = 3;
const int prioridad_task_motores = 3;

const int tamano_task_DHT = 2048;
const int tamano_task_radar = 1024;
const int tamano_task_ultrasonidos= 2048;
const int tamano_task_radiocontrol = 2048;
const int tamano_task_giroscopio = 2048;
const int tamano_task_websockets = 3000;
const int tamano_task_servomotores = 2048;
const int tamano_task_motores = 3000;

int modo_conex = 1;   // 0=STA+AP+RC,  1= AP+RC,  2,  RC solamente

