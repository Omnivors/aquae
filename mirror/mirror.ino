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

void setup() {
  // initialise serial monitor for display messages
  Serial.begin(9600);

  // initialise frequency measurement on digital pin 8
  FreqMeasure.begin();
}

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

bool motor_is_running = false;

void loop() {

  // Reset the time_now to now
  time_now = now();

  // simulate motor ON/OFF every 5 seconds. 
  // These lines should not exist in the final sketch!
  if (time_before - time_now > 5) {
    motor_is_running = not motor_is_running;
    time_before = time_now;
  }

  // If the motor is not rotating, check if someone is moving with X-Band motion detector
  if (not motor_is_running) {
    movement_detected = check_movements();
  
    if (movement_detected) {
      Serial.print('Someone is moving! Play an audio sample');
      play_audio_sample();
    }  
  }

  // the motor is running, so just wait for it to stop before checking proximity again
  else{
    Serial.print('Waiting for the motor to stop running');
    delay(250);
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
void play_audio_sample() {}
