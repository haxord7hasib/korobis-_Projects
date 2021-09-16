void gps_neo(){
  GPSDelay(10);
  unsigned long start;
  double lat_val, lng_val;
  bool loc_valid;
  lat_val = gps.location.lat();        //Gets the latitude
  loc_valid = gps.location.isValid(); 
  lng_val = gps.location.lng();  

        //Serial.print(" Lognitude \n");Serial.print(lat_val, 7);
        //Serial.print(" LATitude \n");Serial.print(lng_val, 7 );
      // delay(1000);

}
static void GPSDelay(unsigned long ms)          //Delay for receiving data from GPS
{
  unsigned long start = millis();
  do
  {
    while (Serial2.available()) 
    gps.encode(Serial2.read());
  } while (millis() - start < ms);
}
