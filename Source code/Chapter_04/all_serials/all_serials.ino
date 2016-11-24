String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);    // Serial console debugger
   Serial1.begin(115200);   // PIN 0->RX and 1->TX
   Serial2.begin(115200);   // PIN 2->RX and 3->TX

}

void loop() {
  
  
  Serial.println("Transmitting from Serial 1 to Serial 2");
  Serial1.println("Intel"); // this will transmitt using PIN 1
  inputString="";   
  
  // Serial2 will wait for something
  while (Serial2.available()) {
    // get the new byte:
    char inChar = (char)Serial2.read();  // receiving by Serial2 on pin 2
  
    // add it to the inputString:
    inputString += inChar;
  
    if (inChar == '\n') {
      stringComplete = true;
      
    } 
  }
 
  if (stringComplete == true) {
    
    // Serial 2 receive the word "Intel" 
    // let's send the word "Galileo" back to Serial 1
    
    Serial.print("Message received from Serial 1:");
    Serial.println(inputString);
    inputString = "";
    
    
    // transmitting another word to Serial 2
    Serial.println("Transmitting from Serial 2 to Serial 1");    
    Serial2.println("Galileo");  // transmitting by Serial2 using pin 3
    
    stringComplete=false;
    
  }

  // Serial1 will wait for something  
  while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read();  // receiving by Serial1 using pin 0
  
    // add it to the inputString:
    inputString += inChar;
  
    if (inChar == '\n') {
      stringComplete = true;
      
    } 
  }


  if (stringComplete == true) {
       
    Serial.print("Message received from Serial 2:");
    Serial.println(inputString);
    inputString = "";   
    stringComplete=false;
    
  }
  
  delay(2000);
}
