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
   Serial2.print("AT+CMGF=1\r"); 
  delay(1000);
  Serial2.println("AT + CMGS = \"+8801976444511\""); 
  delay(1000);
  Serial2.println("Fire DETECTED FROM flame sensor ");
  Serial2.print( F(" tap to see the location https://maps.google.com/maps/place/") );
  Serial2.print(gps.location.lat(), 5 );
  Serial2.print( F(", ") );
  Serial2.println(gps.location.lng(), 5 );
  delay(50);
  Serial2.println((char)26); 
  delay(1000);
  Serial2.println();
  // Give module time to send SMS
  delay(1000);
  while (Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }
    
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
