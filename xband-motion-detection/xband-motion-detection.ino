/*************************************************************************
 * Basic Doppler Frequency motion sensing program:                       *
 *      A program to process an input waveform from a Doppler motion     *
 *      sensing module, such as the HB100 or Parallax X-Band Radar       *
 *      motion detector. It uses the FreqMeasure library, which needs    *
 *      digital pin 8 or the Arduino UNO (or 49 on the MEGA)             *                                                                 *
 *                                                                       *
 *      Author: B.D. Fraser                                              *
 *                                                                       *
 *        Last modified: 12/06/2017                                      *
 *                                                                       *
 *************************************************************************/

// Freq Measure lib - uses digital pin 8 of Arduino Uno for measurement
#include <FreqMeasure.h>

#include <TimeLib.h>

// start now time variable in seconds
int time_now = now();

void setup() {
  // initialise serial monitor for display messages
  Serial.begin(9600);

  // initialise frequency measurement on digital pin 8
  FreqMeasure.begin();
}

// global variables for calculating Doppler freq
double total=0;
int counter=0;
const int min_readings = 5;
const int min_freq = 0;



void loop() {

  // Reset the time_now to now
  time_now = now();
  
  if (FreqMeasure.available()) {
    
    // take an average of frequency readings
    total += FreqMeasure.read();
    counter++;
    // after min_readings freq readings - take average and print result
    if (counter > min_readings) {
      float dopplerFreq = FreqMeasure.countToFrequency(total/counter);
      if (dopplerFreq > min_freq) {
        Serial.print(time_now);
        Serial.print(" frequency: ");
        Serial.println(dopplerFreq);
      }
      total = 0;
      counter = 0;
    }
  }
}
