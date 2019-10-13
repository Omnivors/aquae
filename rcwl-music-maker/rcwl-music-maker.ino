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
#define DREQ 2      // VS1053 Data request, ideally an Interrupt pin

#define INPUT_PIN 2
#define LED_PIN 7

// create shield-example object!
Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

int state = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

// RNDOM FILE
int sample = 0;
String filename_string;
char filename[7];


void setup() {
  
  Serial.begin(9600);
  
  Serial.println("Music Maker + RCWL");

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

  // Serial.println(VS1053_FILEPLAYER_TIMER0_INT);
  
  // musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT);

  /*if (! musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT)) {
    Serial.println(F("DREQ pin is not an interrupt pin"));
   } else {
     Serial.println("Interrupt ok");
  }*/

  musicPlayer.GPIO_pinMode(INPUT_PIN, INPUT);

  //musicPlayer.GPIO_pinMode(LED_PIN, OUTPUT);

  
  Serial.println("GO!");

  musicPlayer.sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working

}

void loop() {

  // read sensor value
  //val = digitalRead(INPUT_PIN);
  val = musicPlayer.GPIO_digitalRead(INPUT_PIN);

  Serial.println(val);
    
  if (val == HIGH) {            
      
      //digitalWrite(LED_PIN, HIGH);
      
      Serial.println("Motion detected!");

      //musicPlayer.GPIO_digitalWrite(LED_PIN, HIGH);
            
      filename_string = "/" + String(sample) + ".wav";
      filename_string.toCharArray(filename, 7);

      Serial.println(filename);
      
      musicPlayer.playFullFile(filename);

      Serial.println("Done playing sample " + sample);

      //musicPlayer.GPIO_digitalWrite(LED_PIN, LOW);

      sample++;
      if (sample == 5) {
        sample = 0;
      }

  } 
  
  /*else {
    // musicPlayer.GPIO_digitalWrite(LED_PIN, LOW); // turn LED OFF
    if (state == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      //fadeOut();
      // We only want to print on the output change, not state
      state = LOW;
    }
  }*/
  
  delay(100);
  
}
