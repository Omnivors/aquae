/*************************************************************************
 * Basic Doppler Frequency motion sensing program:                       *
 *      A program to process an input waveform from a Doppler motion     *
 *      sensing module, such as the HB100 or Parallax X-Band Radar       *
 *      motion detector. It uses the FreqMeasure library, which needs    *
 *      digital pin 8 or the Arduino UNO (or 49 on the MEGA).            *
 *                                                                       *
 *                                                                       *
 *************************************************************************/

// Freq Measure lib - uses digital pin 8 of Arduino Uno for measurement
#include <FreqMeasure.h>
#include <TimeLib.h>

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
#define LED_PIN 6

// create shield-example object!
Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

int state = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

// RNDOM FILE
int sample = 0;
String filename_string;
char filename[7];

// start now time variable in seconds
int time_now = now();
int time_before = 0;

// global variables for calculating Doppler freq
double total = 0;
int counter = 0;
float dopplerFreq = 0.0;
bool movement_detected = false;

const int min_readings = 5;
const int min_freq = 0;


void setup() {
  // initialise serial monitor for display messages
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

  // initialise frequency measurement on digital pin 8
  FreqMeasure.begin();
}



void loop() {

  // Reset the time_now to now
  time_now = now();

  // check if someone is moving with X-Band motion detector
  movement_detected = check_movements();
  
  if (movement_detected) {
    Serial.print("Someone is moving! Play song");
    play_audio_sample();
  } 
}

// frequency reading
bool check_movements(){
  if (FreqMeasure.available()) {
      // take an average of frequency readings
      total += FreqMeasure.read();
      counter++;
      // after min_readings freq readings - take average and print result
      if (counter > min_readings) {
        dopplerFreq = FreqMeasure.countToFrequency(total/counter);
        if (dopplerFreq > min_freq) {
          movement_detected = true;
        }
        else{
          movement_detected = false;
        }
        Serial.print(time_now); Serial.print(" frequency: ");  Serial.println(dopplerFreq);
        total = 0;
        counter = 0;
      }
    }
    
  return movement_detected;
}

// to implement/copy from other sketches
void play_audio_sample() {
  filename_string = "/" + String(sample) + ".mp3";
  filename_string.toCharArray(filename, 7);

  Serial.println(filename);
      
  musicPlayer.playFullFile(filename);

  Serial.println("Done playing sample " + sample);

  //musicPlayer.GPIO_digitalWrite(LED_PIN, LOW);

  sample++;
  if (sample == 6) {
    sample = 0;
  }
}
