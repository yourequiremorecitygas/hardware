#include <Wire.h>

#include "WiFiEsp.h"
#include "PubSubClient.h"

#define _ESPLOGLEVEL_ 1

const char* DEVICE_ID = "3";

// NETWORK
const char* ssid = "capstone02";            // your network SSID (name)
const char* pass = "123456789";        // your network password
const char* mqtt_server = "52.194.252.52";
const char* mqtt_topic = "/Seoul/Dongjak/3";
const char* tcp_test = "52.194.252.52";


bool isEnd = false;
int status = WL_IDLE_STATUS;     // the Wifi radio's status

WiFiEspClient client;
PubSubClient mqttClient(client);

int wifi_status = WL_IDLE_STATUS;
long lastReconnectAttempt = 0;

long lastMsg = 0;
char msg[50];
int value = 0;
////////

const int chipSelect = 48;        
const int HardwareSSPin = 53;  // For Arduino Mega

int PhotoTakenCount = 0;

// Serial Input 
const int BUFFERLENGTH = 255;
char IncomingByte[BUFFERLENGTH];   // for incoming serial data

// VGA Default 
int PHOTO_WIDTH  =  1280; //640   
int PHOTO_HEIGHT =  240; //480
int PHOTO_BYTES_PER_PIXEL = 2;

// Command and Parameter related Strings
String RawCommandLine = "";
String Command  = "VGAP";
String FPSParam = "ThirtyFPS";
String AWBParam = "SAWB";
String AECParam = "HistAEC";
String YUVMatrixParam = "YUVMatrixOn";
String DenoiseParam = "DenoiseYes";
String EdgeParam = "EdgeYes";
String ABLCParam = "AblcON";


enum ResolutionType
{
  VGA,
  VGAP,
  QVGA,
  QQVGA,
  None
};

ResolutionType Resolution = None; 

// Camera input/output pin connection to Arduino
#define WRST  22      // Output Write Pointer Reset
#define RRST  23      // Output Read Pointer Reset
#define WEN   24      // Output Write Enable
#define VSYNC 25      // Input Vertical Sync marking frame capture
#define RCLK  26      // Output FIFO buffer output clock
// set OE to low gnd

// FIFO Ram input pins
#define DO7   28     
#define DO6   29   
#define DO5   30   
#define DO4   31   

#define DO3   32   
#define DO2   33   
#define DO1   34
#define DO0   35 

#define Led_Red   42
#define Led_Green 44
#define Led_Blue  46

// Register addresses and values
#define CLKRC                 0x11 
#define CLKRC_VALUE_VGA       0x01  // Raw Bayer
#define CLKRC_VALUE_QVGA      0x01
#define CLKRC_VALUE_QQVGA     0x01
#define CLKRC_VALUE_NIGHTMODE_FIXED   0x03 // Fixed Frame
#define CLKRC_VALUE_NIGHTMODE_AUTO    0x80 // Auto Frame Rate Adjust

#define COM7                                   0x12 
#define COM7_VALUE_VGA                         0x01   // Raw Bayer
#define COM7_VALUE_VGA_COLOR_BAR               0x03   // Raw Bayer
#define COM7_VALUE_VGA_PROCESSED_BAYER         0x05   // Processed Bayer
#define COM7_VALUE_QVGA                        0x00
#define COM7_VALUE_QVGA_COLOR_BAR              0x02
#define COM7_VALUE_QVGA_PREDEFINED_COLOR_BAR   0x12
#define COM7_VALUE_QQVGA                       0x00
#define COM7_VALUE_QQVGA_COLOR_BAR             0x02   
#define COM7_VALUE_QCIF                        0x08     // Predefined QCIF format
#define COM7_VALUE_COLOR_BAR_QCIF              0x0A     // Predefined QCIF Format with ColorBar
#define COM7_VALUE_RESET                       0x80


#define COM3                            0x0C 
#define COM3_VALUE_VGA                  0x00 // Raw Bayer
#define COM3_VALUE_QVGA                 0x04
#define COM3_VALUE_QQVGA                0x04  // From Docs
#define COM3_VALUE_QQVGA_SCALE_ENABLED  0x0C  // Enable Scale and DCW
#define COM3_VALUE_QCIF                 0x0C  // Enable Scaling and enable DCW

#define COM14                            0x3E 
#define COM14_VALUE_VGA                  0x00 // Raw Bayer
#define COM14_VALUE_QVGA                 0x19
#define COM14_VALUE_QQVGA                0x1A
#define COM14_VALUE_MANUAL_SCALING       0x08   // Manual Scaling Enabled
#define COM14_VALUE_NO_MANUAL_SCALING    0x00   // Manual Scaling DisEnabled

#define SCALING_XSC                                  0x70
#define SCALING_XSC_VALUE_VGA                        0x3A  // Raw Bayer
#define SCALING_XSC_VALUE_QVGA                       0x3A
#define SCALING_XSC_VALUE_QQVGA                      0x3A
#define SCALING_XSC_VALUE_QQVGA_SHIFT1               0x3A
#define SCALING_XSC_VALUE_COLOR_BAR                  0xBA  
#define SCALING_XSC_VALUE_QCIF_COLOR_BAR_NO_SCALE    0x80 // Predefined QCIF with Color Bar and NO Scaling

#define SCALING_YSC                                   0x71 
#define SCALING_YSC_VALUE_VGA                         0x35 // Raw Bayer 
#define SCALING_YSC_VALUE_QVGA                        0x35
#define SCALING_YSC_VALUE_QQVGA                       0x35
#define SCALING_YSC_VALUE_COLOR_BAR                   0x35  // 8 bar color bar
#define SCALING_YSC_VALUE_COLOR_BAR_GREY              0xB5  // fade to grey color bar
#define SCALING_YSC_VALUE_COLOR_BAR_SHIFT1            0xB5  // fade to grey color bar
#define SCALING_YSC_VALUE_QCIF_COLOR_BAR_NO_SCALE     0x00  // Predefined QCIF with Color Bar and NO Scaling

#define SCALING_DCWCTR               0x72 
#define SCALING_DCWCTR_VALUE_VGA     0x11  // Raw Bayer
#define SCALING_DCWCTR_VALUE_QVGA    0x11
#define SCALING_DCWCTR_VALUE_QQVGA   0x22  

#define SCALING_PCLK_DIV              0x73  
#define SCALING_PCLK_DIV_VALUE_VGA    0xF0 // Raw Bayer
#define SCALING_PCLK_DIV_VALUE_QVGA   0xF1
#define SCALING_PCLK_DIV_VALUE_QQVGA  0xF2

#define SCALING_PCLK_DELAY              0xA2
#define SCALING_PCLK_DELAY_VALUE_VGA    0x02 // Raw Bayer
#define SCALING_PCLK_DELAY_VALUE_QVGA   0x02
#define SCALING_PCLK_DELAY_VALUE_QQVGA  0x02


// Controls YUV order Used with COM13
// Need YUYV format for Android Decoding- Default value is 0xD
#define TSLB                                         0x3A
#define TSLB_VALUE_YUYV_AUTO_OUTPUT_WINDOW_ENABLED   0x01 // No custom scaling
#define TSLB_VALUE_YUYV_AUTO_OUTPUT_WINDOW_DISABLED  0x00 // For adjusting HSTART, etc. YUYV format
#define TSLB_VALUE_UYVY_AUTO_OUTPUT_WINDOW_DISABLED  0x08 
#define TSLB_VALUE_TESTVALUE                         0x04 // From YCbCr Reference 


// Default value is 0x88
// ok if you want YUYV order, no need to change
#define COM13                      0x3D
#define COM13_VALUE_DEFAULT        0x88
#define COM13_VALUE_NOGAMMA_YUYV   0x00
#define COM13_VALUE_GAMMA_YUYV     0x80
#define COM13_VALUE_GAMMA_YVYU     0x82
#define COM13_VALUE_YUYV_UVSATAUTOADJ_ON 0x40



// Works with COM4
#define COM17                                 0x42
#define COM17_VALUE_AEC_NORMAL_NO_COLOR_BAR   0x00
#define COM17_VALUE_AEC_NORMAL_COLOR_BAR      0x08 // Activate Color Bar for DSP

#define COM4   0x0D

// RGB Settings and Data format
#define COM15    0x40


// Night Mode
#define COM11                             0x3B
#define COM11_VALUE_NIGHTMODE_ON          0x80     // Night Mode
#define COM11_VALUE_NIGHTMODE_OFF         0x00 
#define COM11_VALUE_NIGHTMODE_ON_EIGHTH   0xE0     // Night Mode 1/8 frame rate minimum
#define COM11_VALUE_NIGHTMODE_FIXED       0x0A 
#define COM11_VALUE_NIGHTMODE_AUTO        0xEA     // Night Mode Auto Frame Rate Adjust


// Color Matrix Control YUV
#define MTX1      0x4f 
#define MTX1_VALUE  0x80

#define MTX2      0x50 
#define MTX2_VALUE  0x80

#define MTX3      0x51 
#define MTX3_VALUE  0x00

#define MTX4      0x52 
#define MTX4_VALUE  0x22

#define MTX5      0x53 
#define MTX5_VALUE  0x5e

#define MTX6      0x54 
#define MTX6_VALUE  0x80

#define CONTRAS     0x56 
#define CONTRAS_VALUE 0x40

#define MTXS      0x58 
#define MTXS_VALUE  0x9e



// COM8
#define COM8                    0x13
#define COM8_VALUE_AWB_OFF      0xE5
#define COM8_VALUE_AWB_ON       0xE7




// Automatic White Balance
#define AWBC1     0x43 
#define AWBC1_VALUE 0x14

#define AWBC2     0x44 
#define AWBC2_VALUE 0xf0

#define AWBC3     0x45 
#define AWBC3_VALUE   0x34

#define AWBC4     0x46 
#define AWBC4_VALUE 0x58

#define AWBC5         0x47 
#define AWBC5_VALUE 0x28

#define AWBC6     0x48 
#define AWBC6_VALUE 0x3a

#define AWBC7           0x59
#define AWBC7_VALUE     0x88

#define AWBC8          0x5A
#define AWBC8_VALUE    0x88

#define AWBC9          0x5B
#define AWBC9_VALUE    0x44

#define AWBC10         0x5C
#define AWBC10_VALUE   0x67

#define AWBC11         0x5D
#define AWBC11_VALUE   0x49

#define AWBC12         0x5E
#define AWBC12_VALUE   0x0E

#define AWBCTR3        0x6C
#define AWBCTR3_VALUE  0x0A

#define AWBCTR2        0x6D
#define AWBCTR2_VALUE  0x55

#define AWBCTR1        0x6E
#define AWBCTR1_VALUE  0x11

#define AWBCTR0                0x6F
#define AWBCTR0_VALUE_NORMAL   0x9F
#define AWBCTR0_VALUE_ADVANCED 0x9E


// Gain
#define COM9                        0x14
#define COM9_VALUE_MAX_GAIN_128X    0x6A
#define COM9_VALUE_4XGAIN           0x10    // 0001 0000

#define BLUE          0x01    // AWB Blue Channel Gain
#define BLUE_VALUE    0x40

#define RED            0x02    // AWB Red Channel Gain
#define RED_VALUE      0x40

#define GGAIN            0x6A   // AWB Green Channel Gain
#define GGAIN_VALUE      0x40

#define COM16     0x41 
#define COM16_VALUE 0x08 // AWB Gain on

#define GFIX      0x69 
#define GFIX_VALUE  0x00

// Edge Enhancement Adjustment
#define EDGE      0x3f 
#define EDGE_VALUE  0x00

#define REG75     0x75 
#define REG75_VALUE 0x03

#define REG76     0x76 
#define REG76_VALUE 0xe1

// DeNoise 
#define DNSTH     0x4c 
#define DNSTH_VALUE 0x00

#define REG77     0x77 
#define REG77_VALUE 0x00

// Denoise and Edge Enhancement
#define COM16_VALUE_DENOISE_OFF_EDGE_ENHANCEMENT_OFF_AWBGAIN_ON     0x08 // Denoise off, AWB Gain on
#define COM16_VALUE_DENOISE_ON__EDGE_ENHANCEMENT_OFF__AWBGAIN_ON    0x18
#define COM16_VALUE_DENOISE_OFF__EDGE_ENHANCEMENT_ON__AWBGAIN_ON    0x28
#define COM16_VALUE_DENOISE_ON__EDGE_ENHANCEMENT_ON__AWBGAIN_ON     0x38 // Denoise on,  Edge Enhancement on, AWB Gain on


// 30FPS Frame Rate , PCLK = 24Mhz
#define CLKRC_VALUE_30FPS  0x80

#define DBLV               0x6b
#define DBLV_VALUE_30FPS   0x0A

#define EXHCH              0x2A
#define EXHCH_VALUE_30FPS  0x00

#define EXHCL              0x2B
#define EXHCL_VALUE_30FPS  0x00

#define DM_LNL               0x92
#define DM_LNL_VALUE_30FPS   0x00

#define DM_LNH               0x93
#define DM_LNH_VALUE_30FPS   0x00

#define COM11_VALUE_30FPS    0x0A   


// Saturation Control
#define SATCTR      0xc9 
#define SATCTR_VALUE  0x60


// AEC/AGC - Automatic Exposure/Gain Control
#define GAIN    0x00 
#define GAIN_VALUE  0x00

#define AEW     0x24 
#define AEW_VALUE 0x95

#define AEB     0x25 
#define AEB_VALUE 0x33

#define VPT     0x26 
#define VPT_VALUE 0xe3


/*
// Gamma
#define SLOP      0x7a 
#define SLOP_VALUE  0x20

#define GAM1      0x7b 
#define GAM1_VALUE  0x10

#define GAM2      0x7c 
#define GAM2_VALUE      0x1e

#define GAM3      0x7d 
#define GAM3_VALUE  0x35

#define GAM4      0x7e 
#define GAM4_VALUE  0x5a

#define GAM5      0x7f 
#define GAM5_VALUE  0x69

#define GAM6      0x80 
#define GAM6_VALUE  0x76

#define GAM7      0x81 
#define GAM7_VALUE  0x80

#define GAM8      0x82 
#define GAM8_VALUE  0x88

#define GAM9      0x83 
#define GAM9_VALUE  0x8f

#define GAM10     0x84 
#define GAM10_VALUE 0x96

#define GAM11     0x85 
#define GAM11_VALUE 0xa3

#define GAM12     0x86 
#define GAM12_VALUE 0xaf

#define GAM13     0x87 
#define GAM13_VALUE 0xc4

#define GAM14     0x88 
#define GAM14_VALUE 0xd7

#define GAM15     0x89 
#define GAM15_VALUE 0xe8
*/



// AEC/AGC Control- Histogram
#define HAECC1      0x9f 
#define HAECC1_VALUE  0x78

#define HAECC2      0xa0 
#define HAECC2_VALUE  0x68

#define HAECC3      0xa6 
#define HAECC3_VALUE  0xd8

#define HAECC4      0xa7 
#define HAECC4_VALUE  0xd8

#define HAECC5      0xa8 
#define HAECC5_VALUE  0xf0

#define HAECC6      0xa9 
#define HAECC6_VALUE  0x90

#define HAECC7                          0xaa  // AEC Algorithm selection
#define HAECC7_VALUE_HISTOGRAM_AEC_ON 0x94 
#define HAECC7_VALUE_AVERAGE_AEC_ON     0x00



/*
// Gamma
#define SLOP      0x7a 
#define SLOP_VALUE  0x20

#define GAM1      0x7b 
#define GAM1_VALUE  0x10

#define GAM2      0x7c 
#define GAM2_VALUE      0x1e

#define GAM3      0x7d 
#define GAM3_VALUE  0x35

#define GAM4      0x7e 
#define GAM4_VALUE  0x5a

#define GAM5      0x7f 
#define GAM5_VALUE  0x69

#define GAM6      0x80 
#define GAM6_VALUE  0x76

#define GAM7      0x81 
#define GAM7_VALUE  0x80

#define GAM8      0x82 
#define GAM8_VALUE  0x88

#define GAM9      0x83 
#define GAM9_VALUE  0x8f

#define GAM10     0x84 
#define GAM10_VALUE 0x96

#define GAM11     0x85 
#define GAM11_VALUE 0xa3

#define GAM12     0x86 
#define GAM12_VALUE 0xaf

#define GAM13     0x87 
#define GAM13_VALUE 0xc4

#define GAM14     0x88 
#define GAM14_VALUE 0xd7

#define GAM15     0x89 
#define GAM15_VALUE 0xe8

*/

// Array Control
#define CHLF      0x33 
#define CHLF_VALUE  0x0b

#define ARBLM     0x34 
#define ARBLM_VALUE 0x11



// ADC Control
#define ADCCTR1     0x21 
#define ADCCTR1_VALUE 0x02

#define ADCCTR2     0x22 
#define ADCCTR2_VALUE 0x91

#define ADC     0x37 
#define ADC_VALUE       0x1d

#define ACOM      0x38 
#define ACOM_VALUE  0x71

#define OFON      0x39 
#define OFON_VALUE  0x2a


// Black Level Calibration
#define ABLC1     0xb1 
#define ABLC1_VALUE 0x0c

#define THL_ST    0xb3 
#define THL_ST_VALUE  0x82


// Window Output 
#define HSTART               0x17
#define HSTART_VALUE_DEFAULT 0x11
#define HSTART_VALUE_VGA     0x13     
#define HSTART_VALUE_QVGA    0x13   
#define HSTART_VALUE_QQVGA   0x13   // Works

#define HSTOP                0x18
#define HSTOP_VALUE_DEFAULT  0x61
#define HSTOP_VALUE_VGA      0x01   
#define HSTOP_VALUE_QVGA     0x01  
#define HSTOP_VALUE_QQVGA    0x01   // Works 

#define HREF                  0x32
#define HREF_VALUE_DEFAULT    0x80
#define HREF_VALUE_VGA        0xB6   
#define HREF_VALUE_QVGA       0x24
#define HREF_VALUE_QQVGA      0xA4  

#define VSTRT                0x19
#define VSTRT_VALUE_DEFAULT  0x03
#define VSTRT_VALUE_VGA      0x02
#define VSTRT_VALUE_QVGA     0x02
#define VSTRT_VALUE_QQVGA    0x02  
 
#define VSTOP                0x1A
#define VSTOP_VALUE_DEFAULT  0x7B
#define VSTOP_VALUE_VGA      0x7A
#define VSTOP_VALUE_QVGA     0x7A
#define VSTOP_VALUE_QQVGA    0x7A  

#define VREF                 0x03
#define VREF_VALUE_DEFAULT   0x03
#define VREF_VALUE_VGA       0x0A   
#define VREF_VALUE_QVGA      0x0A
#define VREF_VALUE_QQVGA     0x0A  


// I2C 
#define OV7670_I2C_ADDRESS                 0x21
#define I2C_ERROR_WRITING_START_ADDRESS      11
#define I2C_ERROR_WRITING_DATA               22

#define DATA_TOO_LONG                  1      // data too long to fit in transmit buffer 
#define NACK_ON_TRANSMIT_OF_ADDRESS    2      // received NACK on transmit of address 
#define NACK_ON_TRANSMIT_OF_DATA       3      // received NACK on transmit of data 
#define OTHER_ERROR                    4      // other error 

#define I2C_READ_START_ADDRESS_ERROR        33
#define I2C_READ_DATA_SIZE_MISMATCH_ERROR   44


long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

////////////////////// MQTT //////////////////
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
  
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == 'A' && (char)payload[1] == 'T' && (char)payload[2] != '+') {
      // Take Photo
      mqttClient.publish(mqtt_topic, "AT+ONGOING");
      if( client.connect(tcp_test, 8110) ){ 
        //Serial.println(F("\nGoing to take photo with current command:")); 
        //DisplayCurrentCommand(); 
           
        // Take Photo
        ExecuteCommand(Command);
        //PhotoTakenCount++;
      } else {
        mqttClient.publish(mqtt_topic, "AT-ONGOING");
      }
    }
    else if ((char)payload[0] == 'B' && (char)payload[1] == 'T'){
      char buf[16];
      long lval;
      ltoa(readVcc(),buf,10);
      mqttClient.publish(mqtt_topic, buf);
    } 
    else if((char)payload[0] == 'C' && (char)payload[1] == 'T' && (char)payload[2] == '+' ) {
      turnLED(true);
      mqttClient.publish(mqtt_topic, "ACT+");
    } else if ((char)payload[0] == 'C' && (char)payload[1] == 'T' && (char)payload[2] == '-'){
      turnLED(false);
      mqttClient.publish(mqtt_topic, "ACT-");
    }
}

boolean reconnect() {
  Serial.println("mqtt reconnect");
  client.stop();
  
  while ( wifi_status != WL_CONNECTED) {
     Serial.print(".");
     wifi_status = WiFi.begin(ssid, pass);
     Serial.print(status);
  }
  
  if (mqttClient.connect(DEVICE_ID, "/stats/connected/3", 0, true, "0:/Seoul/Dongjak/3")) {
    
    // Once connected, publish an announcement...
    mqttClient.publish(mqtt_topic,"AT+HELLO");
    mqttClient.publish("/stats/connected/3", "1:/Seoul/Dongjak/3", true);
    
    if( isEnd ){
      mqttClient.publish(mqtt_topic, "AT+IMGEND");
      isEnd = false;
    }
    // ... and resubscribe
    mqttClient.subscribe(mqtt_topic);
  }
  return mqttClient.connected();
}
////////////////////



void initLED()
{
  // Output pin initialization for the LEDs
  pinMode (Led_Red, OUTPUT); 
  pinMode (Led_Green, OUTPUT);
  pinMode (Led_Blue, OUTPUT); 
}

void turnLED(int b)
{
  if( b == 1 ){
    analogWrite (Led_Red, 20);   // LED will be switched on
    analogWrite (Led_Green, 20);  // LED will be switched off
    analogWrite (Led_Blue, 20);   // LED will be switched off
  } else {
    analogWrite (Led_Red, 0);   // LED will be switched on
    analogWrite (Led_Green, 0);  // LED will be switched off
    analogWrite (Led_Blue, 0);   // LED will be switched off
  }
}



void setup()
{
     // Initialize Serial
     Serial.begin(115200); 
     Serial1.begin(115200);
     
     Serial.println(F("Arduino SERIAL_MONITOR_CONTROLLED CAMERA ... Using ov7670 Camera"));
     Serial.println();
     
     // WIFI CONNECT
     WiFi.init(&Serial1);// WIFI Init
     WiFi.begin(ssid, pass);
     
     Serial.println(ssid);
     
     // attempt to connect to WiFi network
      while (WiFi.status() != WL_CONNECTED) {
        //WiFi.init(&Serial1);
        WiFi.begin(ssid, pass);
      }

        // you're connected now, so print out the data
      Serial.println("You're connected to the network");
      
      printWifiStatus();
    
      mqttClient.setServer(mqtt_server, 1883);
      mqttClient.setCallback(callback);

      reconnect();

     // LED TEST
     initLED();
    
     // Setup the OV7670 Camera for use in taking still photos
     Wire.begin();
     Serial.println(F("----------------------------- Camera Registers ----------------------------"));
     ResetCameraRegisters();
     ReadRegisters();
     Serial.println(F("---------------------------------------------------------------------------"));
     SetupCamera();    
     Serial.println(F("FINISHED INITIALIZING CAMERA ..."));
     Serial.println();
     Serial.println();

}






void loop()
{ 
    if (!mqttClient.connected()) {
      long now = millis();
      if (now - lastReconnectAttempt > 5000) {
        lastReconnectAttempt = now;
        // Attempt to reconnect
        if (reconnect()) {
          lastReconnectAttempt = 0;
        }
      }
    } else {
      // Client connected
      mqttClient.loop();
      delay(100);
    }
      
      

      
  
     // Wait for Command
     /*Serial.println(F("Ready to Accept new Command => "));
     while (1)
     {
       if (Serial.available() > 0) 
       {
         int NumberCharsRead = Serial.readBytesUntil('\n', IncomingByte, BUFFERLENGTH);  
         for (int i = 0; i < NumberCharsRead; i++)
         {
           RawCommandLine += IncomingByte[i];
         } 
         break;
       }
     }
        
     // Print out the command from Android
     Serial.print(F("Raw Command from Serial Monitor: "));
     Serial.println(RawCommandLine);
     
     if ((RawCommandLine == "h")||
         (RawCommandLine == "help"))
     {
       DisplayHelpMenu();
     }
     else
     if (RawCommandLine == "help camera")
     {       
       DisplayHelpCommandsParams();
     }
     else
     if (RawCommandLine == "d")
     {
        DisplayCurrentCommand();       
     }
     else
     if (RawCommandLine == "s")
     {
         // Take Photo
         Serial.println(F("\nGoing to take photo with current command:")); 
         DisplayCurrentCommand(); 
         
         // Take Photo
         ExecuteCommand(Command);
         Serial.println(F("Photo Taken and Saved to Arduino SD CARD ..."));
         
         String Testfile = CreatePhotoFilename();
         Serial.print(F("Image Output Filename :"));
         Serial.println(Testfile);
         PhotoTakenCount++;
         
     }
     else
     {
        Serial.println(F("Changing command or parameters according to your input:"));
         // Parse Command Line and Set Command Line Elements
         // Parse Raw Command into Command and Parameters
         ParseRawCommand(RawCommandLine);
         
         // Display new changed camera command with parameters
         DisplayCurrentCommand();
     }
    
     // Reset Command Line
     RawCommandLine = "";
     
     Serial.println();
     Serial.println();
     */
}

void printWifiData()
{
  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print your MAC address
  byte mac[6];
  WiFi.macAddress(mac);
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  Serial.print("MAC address: ");
  Serial.println(buf);
}

void printCurrentNet()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to
  byte bssid[6];
  WiFi.BSSID(bssid);
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", bssid[5], bssid[4], bssid[3], bssid[2], bssid[1], bssid[0]);
  Serial.print("BSSID: ");
  Serial.println(buf);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.println(rssi);
}

void printWifiStatus()
{
  printCurrentNet();
  printWifiData();
}
