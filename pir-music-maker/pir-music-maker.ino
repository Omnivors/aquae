/***************************************************
 * 
 * Example using interrupts and PIR sensor
 * Chooses a random file (0-6).wav when motion is detected
 * sops when motion ends
 * 
 * 
 * TODO: 
 * 1. better interrupt management
 * 2. fade out function
 * 3. when the sate remains HIGH e for 5/6 seconds meens we have continuous movement -> trigger a new sound!
 * 
 ****************************************************/

#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 2       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer =
  // create breakout-example object!
  //Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

int ledPin = 13;                // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status


void setup() {
  Serial.begin(9600);
  Serial.println("VS1053 GPIO test");

  // disable the card (we won't be using it)
  pinMode(CARDCS, OUTPUT);
  digitalWrite(CARDCS, HIGH);

  // initialise the music player
  if (!musicPlayer.begin()) {
    Serial.println("VS1053 not found");
    while (1);  // don't do anything more
  }

  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  Serial.println(VS1053_FILEPLAYER_TIMER0_INT);

  if (! musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT)) {
    Serial.println(F("DREQ pin is not an timer pin"));
   } else {
     Serial.println("TIMER ok");
  }

  /*if (! musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT)) {
    Serial.println(F("DREQ pin is not an interrupt pin"));
   } else {
     Serial.println("Interrupt ok");
  }*/

  musicPlayer.GPIO_pinMode(inputPin, INPUT);

  Serial.println("Caribrating...");

  delay(10000); // warm up and calibration

  Serial.println("GO!");

  musicPlayer.sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working

}

int sample;
String filename_string;
char filename[7];

void loop() {

  val = musicPlayer.GPIO_digitalRead(inputPin);  // read input value

  if (val == HIGH) {            // check if the input is HIGH
    // digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");

      int sample = random(0, 7);
      String filename_string = "/" + String(sample) + ".wav";
      char filename[7];
      filename_string.toCharArray(filename, 7);

      Serial.println(filename);
      
     
      musicPlayer.startPlayingFile(filename);

      while (musicPlayer.playingMusic) {
        // file is now playing in the 'background' so now's a good time
        // to do something else like handling LEDs or buttons :)
        Serial.println("Playing file");

        val = musicPlayer.GPIO_digitalRead(inputPin);
        Serial.println(val);

        if (val == LOW) {        
          Serial.println("stopping...");
          
          musicPlayer.stopPlaying();
          pirState = LOW;
        }

        delay(100);
        
       }

       Serial.println("Done playing music");

      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      //fadeOut();
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }

  delay(100);

}

char * chooseRandomFile() {
  int sample = random(0, 7);
  String filename_string = "/" + String(sample) + ".wav";
  char filename[7];
  filename_string.toCharArray(filename, 7);
  Serial.println(filename);
  return filename;
}
