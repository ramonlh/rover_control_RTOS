
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


