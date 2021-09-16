
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
