/* ----------------------------------------------------------------------
   Pin layout should be as follows:
   Signal     Pin              Pin               Pin              Pin
            ESP8266          LCD I2C         MFRC522 board     Servo SG90
   ----------------------------------------------------------------------
   Reset      RST                                RST
   SPI SS     D4-5                                 SDA
   SPI MOSI   D5-23                                 MOSI
   SPI MISO   D7-19                                 MISO
   SPI SCK    D6-18                                SCK
   SERVO      D3                                                 SIGNAL
   SDA        D2-21              SDA
   SCL        D1-22              SCL
   5V         VIN             VIN                                VIN
   3.3V       3V3                                3.3V
   GND        GND             GND                GND             GND
*/
                                                               
#include <Wire.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <time.h>
#include "DHT.h"
#include <SPI.h>

#include <TinyGPS++.h> 
#define TXD1 25 //GSM
#define RXD1 26
int z =2;
int y =2;
#define TXD2 21 //GPS
#define RXD2 19
TinyGPSPlus gps;


#define DHTPIN 2   
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);
const char* ssid     = "Tenda_68A6C8"; // Your ssid
const char* password = "awse2313"; // Your Password
char status;
WiFiServer server(80);



#define UNLOCK_POS 10
#define LOCK_POS 100

#define SS_PIN 5
#define RST_PIN 3
int Buzzer = 33;        // used for ESP32
int Gas_analog = 32;   // used for ESP32
int Gas_digital = 12;


Servo myservo;
LiquidCrystal_I2C lcd(0x27, 20, 4);
// NodeMCU Dev Kit => D1 = SCL, D2 = SDA


int x = 0;
int timezone = 6;
char ntp_server1[20] = "3.th.pool.ntp.org";
char ntp_server2[20] = "1.asia.pool.ntp.org";
char ntp_server3[20] = "0.asia.pool.ntp.org";

int dst = 0;
bool displayTime = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);
unsigned long uidDec, uidDecTemp;
int ARRAYindexUIDcard;
int EEPROMstartAddr;
long adminID =3913980526;
int LockSwitch;
unsigned long CardUIDeEPROMread[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

void setup() {
  Serial.begin(115200); 
  
  Serial2.begin(9600, SERIAL_8N2, RXD1, TXD1);

  dht.begin();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi is connected");
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
  
  Wire.begin();
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  myservo.attach(14);
  myservo.write(LOCK_POS);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.print("SERVER_STARTING");
  delay(100);
  SPI.begin();
  mfrc522.PCD_Init();
  EEPROM.begin(512);

  WiFi.begin(ssid,password);
  delay(1000);
  configTime(timezone * 3600, dst, ntp_server1, ntp_server2, ntp_server3);
  delay(1000);

  lcd.clear();
  DisplayWAiT_CARD();
  EEPROMreadUIDcard();

 
  pinMode(Buzzer, OUTPUT);      
  pinMode(Gas_digital, INPUT);

}

void loop() {


  if (x == 0) {
   gas_sens();
   dht11();
   gps_neo();
 }

 
  time_t now = time(nullptr);
  struct tm* newtime = localtime(&now);

  if (displayTime == 0) {

    if (!time(nullptr)) {
      lcd.setCursor(0, 0);
      lcd.print("    WELCOME     ");
      delay (500);
    }
    else
    {
      lcd.setCursor(4, 0);
      if ((newtime->tm_hour) < 10)lcd.print("0");
      lcd.print(newtime->tm_hour);
      lcd.print(":");

      lcd.setCursor(7, 0);
      if ((newtime->tm_min) < 10)lcd.print("0");
      lcd.print(newtime->tm_min);
      lcd.print(":");

      lcd.setCursor(10, 0);
      if ((newtime->tm_sec) < 10)lcd.print("0");
      lcd.print(newtime->tm_sec);
    }
  }


  //Look for new cards.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  //Select one of the cards.
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  //Read "UID".
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec * 256 + uidDecTemp;
  }

  if (uidDec == adminID || LockSwitch > 0)EEPROMwriteUIDcard();

  if (LockSwitch == 0)
  {
    for (ARRAYindexUIDcard = 0; ARRAYindexUIDcard <= 3; ARRAYindexUIDcard++)
    {
      if (CardUIDeEPROMread[ARRAYindexUIDcard] > 0)
      {
        if (CardUIDeEPROMread[ARRAYindexUIDcard] == uidDec)
        {
          
          unlock();
          CommandsCARD();
          break;
        }
      }
    }
    if (ARRAYindexUIDcard == 3)lcd.setCursor(0, 0), lcd.print(" Card not Found"), lcd.setCursor(0, 1), lcd.print("                "), lcd.setCursor(0, 1), lcd.print("ID "); lcd.print(uidDec);
    delay(2000);
    lock();
    ARRAYindexUIDcard = 0;
    DisplayWAiT_CARD();
  }
  
 while (Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }
  
}
