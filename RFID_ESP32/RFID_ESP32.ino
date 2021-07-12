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

const char* WIFI_SSID = "hioopo";
const char* WIFI_PWD = "awse2313";
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
  myservo.attach(14);
  myservo.write(LOCK_POS);
  
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.print("SERVER_STARTING");
  delay(100);
  SPI.begin();
  mfrc522.PCD_Init();
  EEPROM.begin(512);

  WiFi.begin(WIFI_SSID, WIFI_PWD);
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

}
void gas_sens()
{
  int gassensorAnalog = analogRead(Gas_analog);
  int gassensorDigital = digitalRead(Gas_digital);

  Serial.print("Gas Sensor: ");
  Serial.print(gassensorAnalog);
  Serial.print("\t");
  Serial.print("Gas Class: ");
  Serial.print(gassensorDigital);
  Serial.print("\t");
  Serial.print("\t");
  
  if (gassensorAnalog > 1000) {
    Serial.println("Gas");
    digitalWrite (Buzzer, HIGH) ; //send tone
    delay(1000);
    digitalWrite (Buzzer, LOW) ;  //no tone

    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("   FIRE!!!!!! ");
    lcd.display();
    delay(500);
    
  }
  else {
    Serial.println("No Gas");
   
  }
  delay(100);

}

// ******************************************

void EEPROMwriteUIDcard() {

  if (LockSwitch == 0)
  {
    displayTime = 1;
    lcd.setCursor(0, 0);
    lcd.print("                 ");
    lcd.setCursor(0, 0);
    lcd.print("Start Record Card");
    lcd.display();
    delay(500);
  }

  if (LockSwitch > 0)
  {
    if (uidDec == adminID)
    {
      displayTime = 1;
      lcd.setCursor(0, 0);
      lcd.print("                 ");
      lcd.setCursor(0, 0);
      lcd.print("   SKIP RECORD   ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("   Label : ");
      lcd.print(EEPROMstartAddr / 5);
      EEPROMstartAddr += 5;
      delay(200);
    }
    else
    {
      EEPROM.write(EEPROMstartAddr, uidDec & 0xFF);
      EEPROM.write(EEPROMstartAddr + 1, (uidDec & 0xFF00) >> 8);
      EEPROM.write(EEPROMstartAddr + 2, (uidDec & 0xFF0000) >> 16);
      EEPROM.write(EEPROMstartAddr + 3, (uidDec & 0xFF000000) >> 24);
      EEPROM.commit();
      delay(10);
      displayTime = 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  RECORD  OK!  ");
      lcd.setCursor(0, 1);
      lcd.print("  MEMORY : ");
      lcd.print(EEPROMstartAddr / 5);
      EEPROMstartAddr += 5;
      delay(500);
    }
  }

  LockSwitch++;

  if (EEPROMstartAddr / 5 == 3)
  {
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" RECORD FINISH ");
    EEPROMstartAddr = 0;
    uidDec = 0;
    ARRAYindexUIDcard = 0;
    EEPROMreadUIDcard();
    displayTime = 0;
  }
}

// -------------------------------------------

void EEPROMreadUIDcard()
{
  for (int i = 0; i <= 9; i++)
  {
    byte val = EEPROM.read(EEPROMstartAddr + 3);
    CardUIDeEPROMread[ARRAYindexUIDcard] = (CardUIDeEPROMread[ARRAYindexUIDcard] << 8) | val;
    val = EEPROM.read(EEPROMstartAddr + 2);
    CardUIDeEPROMread[ARRAYindexUIDcard] = (CardUIDeEPROMread[ARRAYindexUIDcard] << 8) | val;
    val = EEPROM.read(EEPROMstartAddr + 1);
    CardUIDeEPROMread[ARRAYindexUIDcard] = (CardUIDeEPROMread[ARRAYindexUIDcard] << 8) | val;
    val = EEPROM.read(EEPROMstartAddr);
    CardUIDeEPROMread[ARRAYindexUIDcard] = (CardUIDeEPROMread[ARRAYindexUIDcard] << 8) | val;

    ARRAYindexUIDcard++;
    EEPROMstartAddr += 5;
  }

  ARRAYindexUIDcard = 0;
  EEPROMstartAddr = 0;
  uidDec = 0;
  LockSwitch = 0;
  displayTime = 0;
  DisplayWAiT_CARD();
}

// -------------------------------------------

void DisplayWAiT_CARD()
{
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(" TAG THE CARD");
}

// -------------------------------------------

void CommandsCARD()
{
  lcd.clear(), lcd.setCursor(0, 0), lcd.print("#WELCOME GET IN#"), lcd.print(ARRAYindexUIDcard), lcd.print("        ");
  lcd.setCursor(0, 1), lcd.print("ID "), lcd.print(uidDec) ;

  if (ARRAYindexUIDcard == 0)
  {
    lcd.setCursor(0, 1), lcd.print("  Shahana Korobi  ");
    delay(1000);
  }

  else if (ARRAYindexUIDcard == 1)
  {
    lcd.setCursor(0, 1), lcd.print("Korbi            ");
  }

  else if (ARRAYindexUIDcard == 2)
  {
    lcd.setCursor(5, 0), lcd.print("Korbi2           ");
  }

  
}




void lock() {
  myservo.write(LOCK_POS);
}

void unlock() {
  myservo.write(UNLOCK_POS);
}
