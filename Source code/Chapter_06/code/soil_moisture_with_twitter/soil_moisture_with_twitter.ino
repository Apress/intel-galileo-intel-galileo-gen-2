
// Author: Manoel Carlos Ramon
//  email: manoel.c.ramon@intel.com

#define DEBUG                    1

/* Pins that define if system is ON */
#define PIN_LEFT_EYE             10
#define PIN_RIGHT_EYE            11

/* Sensor switch button */
#define PIN_SWITCH_SYSTEM        13


/* Moisture sensor - Analog Input */
#define  ANALOG_MOISTURE_SENSOR_1         A0
#define  ANALOG_MOISTURE_SENSOR_2         A1
int sensor_value = 0;

/* The analog reading boundary when soil is good */
#define  SOIL_IS_GOOD                350

void clear();
int current_sensor = 0;
int button_state = 0;

int array_happy_face[2][4] = {{1, 0, 0, 1},     /* line 1 */
                              {0, 1, 1, 0}};    /* line 0 */
                        
int array_sad_face[2][4]   = {{0, 1, 1, 0},     /* line 1 */
                              {1, 0, 0, 1}};    /* line 0 */
                        


/*  THE MOUTH - back view
                             _____________   led 0 - pin 6
                             |  __________   led 1 = pin 7
                             |  |  _______   led 2 = pin 8
                             |  |  |  ______ led 3 = pin 9
                             |  |  |  |      


                             O  O  O  O      line 1
                             0  0  0  0      line 0                       


                             ^  ^  ^  ^
                             |  |  |  |__  led 3 = pin 5
                             |  |  |______ led 2 = pin 4
                             |  |________  led 1 = pin 3
                             |____________ led 0 = pin 2
*/


int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 500;   // adjust this value if necessary to avoid flickering 

// for twitter
int old_day[2]={0,0};  
String plant_name;
String message;

char cmd_rsp[256];

/* Add here your consumer and OAuth string codes */
String consumer_key="kTTipvgVybC4L65OKGSMMQ";
String consumer_secret="vjl6IuyOY5hME0a7RvVJQoyYzowK4P82fyNSIYuSpE0";
String oauth_token="2282830086-yz9n5pcIg9uxRREHjd1rtf4CRqrxRT34z1tkvnZ";
String oauth_secret="p0sEMLKb9zJ5vzQTmSWNCnQ7TcSTEPQfsoyd3MwKvosyq";

/* this is the name of script in the target 
   you need to push this script to the target before run this software */
String script_name = ". /home/root/twitter.sh ";
  
/* prototype */  
boolean post(String message);  


void clear()
{
   int pin = 0;
   for (pin = 0; pin < 12; pin++)
   {
     digitalWrite(pin, LOW);
   }
}


void drawMatrix(int array[2][4]) 
{
  int line = 0;
  int pin = 2;
  int c = 0;
  int level = LOW;
 
  while (line < 2)
  {
     digitalWrite(line, LOW);
  
     while (c <= 3)
     {
       
        level = array[line][c];
  
        digitalWrite(pin, level);
        c++;pin++;
     }
     c=0;
     line++;
     delay(10);
  } 
  
}


boolean should_post_today(int * day)
{
  

  boolean mustTweet = false;
  time_t rawtime;
  struct tm *timeinfo;
  
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  
  if (old_day[current_sensor] == 0) old_day[current_sensor] = timeinfo->tm_mday;
  
  if (old_day[current_sensor] != timeinfo->tm_mday)
  {
     old_day[current_sensor] = timeinfo->tm_mday;
     mustTweet = true;    
  }
  
  if (day != NULL) *day = timeinfo->tm_mday;
   
  return mustTweet;
}

void checkButtonState()
{
  
  // read the state of the switch into a local variable:
  int reading = digitalRead(PIN_SWITCH_SYSTEM);
  
  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer 
     
    // if the button state has changed:
    if (reading != button_state) {
      button_state = reading;

    }
  }
  
  lastButtonState = reading;

  return;
}



boolean post(String message)
{
  
  boolean result = true;
  
  String command = script_name;
  
  command += consumer_key;
  command += " ";
  
  command += consumer_secret;
  command += " ";

  command += oauth_token;
  command += " ";

  command += oauth_secret;  
  command += " ";
  
  command += "'";
  command += message;
  command += "'";

   // buffer to be used with popen
   char cmd_char[1024];  
   
   // clear message buffer
   memset((void *)cmd_char, sizeof(cmd_char), 0);
   
   // convert the message to char array
   command.toCharArray(cmd_char, sizeof(cmd_char), 0);
   
   Serial.println(cmd_char);
   
   FILE *fpipe;
   
   if ( !(fpipe = (FILE*)popen((char *)cmd_char,"r")) ) 
      {  // If fpipe is NULL
      Serial.println("Problems with pipe");
      result = false;
      
   }
   else
   {
   
     while ( fgets( cmd_rsp, sizeof(cmd_rsp), fpipe)) {}
   
     pclose(fpipe);
   
     // checking for errors
     if ((strstr(cmd_rsp, "{\"errors\":[{\"message\"") != NULL) ||
         (strstr(cmd_rsp, "ERROR: No internet connection") != NULL))
     {
        Serial.println("OPS!!! ERROR!!");
        Serial.println(cmd_rsp);
        result = false;
     }
     else
     {
        Serial.println("POSTED WITH SUCCESS!");
        Serial.println(cmd_rsp);
     }  
   }
   
   return result;
}


void setup() {
  
  if (DEBUG) Serial.begin(9600);
  // put your setup code here, to run once:
   int pin = 0;
   for (pin = 2; pin < 12; pin++)
   {
     pinMode(pin, OUTPUT);
     delay(10);
   }
   
   // switch button
   pinMode(PIN_SWITCH_SYSTEM, INPUT);

   // turn off all leds   
   clear();

}

void loop() {

  int day; 
  
  // reading the button state
  checkButtonState();
   
  if( button_state != lastButtonState)
   {
      // button pressed
      current_sensor++;
      if (current_sensor > 1) current_sensor = 0;
  }

   
  // reading the sensor
  switch (current_sensor)
  {
      case 0:
          sensor_value = analogRead(ANALOG_MOISTURE_SENSOR_1);  
          
          // first sensor - one LED ON
          digitalWrite(PIN_RIGHT_EYE, HIGH);
          digitalWrite(PIN_LEFT_EYE, LOW);

      break;
          
      case 1:
          sensor_value = analogRead(ANALOG_MOISTURE_SENSOR_2);  
          
          // second sensor - two LEDs ON
          digitalWrite(PIN_RIGHT_EYE, HIGH);
          digitalWrite(PIN_LEFT_EYE, HIGH);

      break; 
  }
  
  
  switch (current_sensor)
  {
    case 0: plant_name = "orquidea";
    break;
    
    case 1: plant_name = "sunflower";
    break;
    
    default: 
        Serial.println("There is an error in your logic!");
    break;
    
  }
  
  if (DEBUG) 
  {
      Serial.print("current_sensor:");
      Serial.println(current_sensor);
      
      Serial.print("   sensor_value:");
      Serial.println(sensor_value);
  }
  
  if (sensor_value >= SOIL_IS_GOOD)
  {
     drawMatrix(array_happy_face);
     if (should_post_today(&day)) 
     {
        char strDay[2];
        sprintf(strDay, "%d", day);
       
        message = "This is ";
        message += plant_name;
        message += " I am ok thanks! Day:";
        message += strDay;
        post(message);
     }
  }
  else
  {
     drawMatrix(array_sad_face);
     if (should_post_today(&day))
     {
        char strDay[2];
        sprintf(strDay, "%d", day);
        
        message = "This is ";
        message += plant_name;
        message += " HEY! I NEED WATER! Day:";
        message += strDay;
        post(message);
     }
     
  }
  delay(1000);
  
}
