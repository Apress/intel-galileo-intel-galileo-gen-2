// some tones in hertz
#define NOTE_C4  262
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247

// melody on pin 0:
int melody_pin0[] = {
   NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};

// melody on pin 1:
int melody_pin1[] = {
   NOTE_G3, NOTE_C4,NOTE_G3, NOTE_G3, NOTE_A3,0, NOTE_C4, NOTE_B3};


void setup() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < sizeof(melody_pin0)/sizeof(int); thisNote++) {

    //the duration is not specified to make a non-blocking call.
    tone(0, melody_pin0[thisNote]);
    tone(1, melody_pin1[thisNote]);

    // small delay
    delay(500);
    
    // stop the tone playing:
    noTone(0);
    noTone(1);
  }
}

void loop() {
  // no need to repeat the melody.
}
