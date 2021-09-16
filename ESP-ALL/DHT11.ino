void dht11() {

float h = dht.readHumidity();

  float t = dht.readTemperature();

  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
 
  WiFiClient client = server.available();
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: close");  
client.println("Refresh: 10");  // 
client.println();
client.println("<!DOCTYPE HTML>");
client.println("<html>");
client.println("<head>");
client.println("<style>html { font-family: Fantasy; display: block; margin: 0px auto; text-align: center;color: #333333; background-color: #acc6fa;}");
client.println("body{margin-top: 50px;}");
client.println("h1 {margin: 50px auto 30px; font-size: 50px; text-align: center;}");
client.println(".side_adjust{display: inline-block;vertical-align: middle;position: relative;}");
client.println(".text1{font-weight: 180; padding-left: 15px; font-size: 50px; width: 170px; text-align: left; color: #3498db;}");
client.println(".data1{font-weight: 180; padding-left: 80px; font-size: 50px;color: #3498db;}");
client.println(".text2{font-weight: 180; font-size: 50px; width: 170px; text-align: left; color: #3498db;}");
client.println(".data2{font-weight: 180; padding-left: 150px; font-size: 50px;color: #3498db;}");
client.println(".data{padding: 10px;}");
client.println("</style>");
client.println("</head>");
client.println("<body>");
client.println("<div id=\"webpage\">");   
client.println("<h1>Korobis Weather System </h1>");
client.println("<div class=\"data\">");
client.println("<div class=\"side_adjust text1\">Humidity:</div>");
client.println("<div class=\"side_adjust data1\">");
client.print(h);
client.println("<div class=\"side_adjust text1\">%</div>");
client.println("</div>");
client.println("<div class=\"data\">");
client.println("<div class=\"side_adjust text2\">Temperature:</div>");
client.println("<div class=\"side_adjust data2\">");
client.print(t);
client.println("<div class=\"side_adjust text2\">*C</div>");
client.print(f);
client.println("<div class=\"side_adjust text2\">F</div>");
client.println("</div>");
client.println("<div class=\"data\">");



client.println("</div>");
client.println("</body>");
client.println("</html>");

}
