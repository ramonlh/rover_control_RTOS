
// definidos en fichero user_setup.h de la libreira TFT_eSPI
// para PLACA EXPERIMENTAL V1
//#define TFT_MISO   19
//#define TFT_MOSI   23
//#define TFT_SCLK   18
//#define TFT_CS     15
//#define TFT_DC      2
//#define TFT_RST     4    // Set TFT_RST to -1 if the display RESET is connected to RST or 3.3V
//#define TOUCH_CS    5

#define WIFI_TRIGGER_PIN 0    // gpio para reset WiFi y acceder al menu WifiManager
#define pin_radio_control 32
#define pin_US1_receptor 26
#define pin_US1_emisor 25
#define pin_led_7colores 27
#define pin_radar 14
#define pin_SDA 21
#define pin_SCL 22
#define pin_DHT11 13

const IPAddress WIFI_IP(192,168,11,91);
const IPAddress WIFI_GW(192,168,11,1);
const IPAddress WIFI_MASK(255,255,255,0);

#define PORT_WEBSERVER 80
#define PORT_WEBSOCKET 81
#define PORT_TCPSOCKET 82
#define password_OTA "1892"

#define i2caddress_giroscopio 0x50

