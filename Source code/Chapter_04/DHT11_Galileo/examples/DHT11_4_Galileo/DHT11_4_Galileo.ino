#include <DHT_4_Galileo.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("start......");
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly: 
  
  Serial.println("send command..");
  Serial.println();
  
  DHT_4_Galileo dht(3, 2);
  
  dht.sendCommand();
  
  int response = dht.read();
  if (response == DHT_4_Galileo::DHT_READ_SUCCESS)
  {
     Serial.println("RESULTS:");
     Serial.println("-----------------");
     Serial.print("Humidity:");
     Serial.print(dht.humidity,1);
     Serial.print(",\t");
     Serial.print("Temperature (C):");
     Serial.println(dht.temperature,1);
  }
  else 
  {
     Serial.print("there is an error:");
     Serial.println(response);
  }
  delay(5000);
  
}