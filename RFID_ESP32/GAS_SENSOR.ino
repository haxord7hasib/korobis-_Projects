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
