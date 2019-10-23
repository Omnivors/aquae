/***************************************************
 * 
 * Arduino sketch for "Sulfur" artwork, Aquae Collection, Raoul Gilioli
 * October 2019
 * Artwork by Raoul Gilioli
 * Technical support by Omnivors & Luigi Gagliardi
 * 
 * Ultrasonic sensor
 * Infrared transmitter (NEC protocol)
 * 
 * 
 * !!! IMPORTANT !!!
 * 
 * IRRemote and NewPing use the same timer (timer 2), thies leads to compiling errors
 * But if you don't call the ping_time() method (such as this sketch), NewPing library does not use timer2, so we can disable it
 * 
 * To avoid complier errors:
 * 1. update NewPing to version 1.6 or higher
 * 2. open NewPing.h file (Arduino -> libraries -> NewPing -> src -> NewPing.h)
 * 3. set TIMER_ENABLED = false
 * 
 * More info: https://bitbucket.org/teckel12/arduino-new-ping/wiki/Multiple%20Definition%20of%20%22__vector_7%22%20Error
 * 
 ***************************************************/
 
#include <IRremote.h>
#include <NewPing.h>
#include <RunningMedian.h>

// define vaporizer NEC IR messages
#define ON_OFF 0xFF807F
#define DISPLAY 0xFF40BF
#define AUTO 0xFFA05F
#define MIST_LEVEL 0xFF10EF
#define HUMIDITY 0xFF58A7
#define TIMER 0xFF50AF
#define WARM 0xFF7887

// massima distanza oltre la quale l'opera è inattiva
#define MAX_DISTANCE 150  
// distanza alla quale deve trovarsi lo spettatore per attivare l'opera    
#define PLAY_DISTANCE 100    

#define TRG_PIN 4
#define ECHO_PIN 3

// IR transmitter instance
IRsend irsend;

// true if vaporizer is running, false if not
boolean vapor = false;

// sonar management
NewPing sonar(TRG_PIN, ECHO_PIN, MAX_DISTANCE);
int distance = 0;

void setup() { 
  Serial.begin(9600);

  pinMode(TRG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRG_PIN, LOW);

  delay(500);
  
  Serial.println("Sulfur - artwork based on sonar and IR transmitter");
}

void loop() {

  // delay(60000);

  // get data from sonar
  unsigned int uS = sonar.ping_median(10);
  distance = uS / US_ROUNDTRIP_CM;
    
  if (distance <= PLAY_DISTANCE) {
    // check if vaporizer is off
    if (!vapor) {
      // run vaporizer
      // sendNec(message, number_of_bits) -> ON_OFF is 6 hex digits = 24 bits
      irsend.sendNEC(ON_OFF, 24);
      // Serial.println("Start vaporizer!");
    }
    // set state to true
    vapor = true;
  } else if (distance > PLAY_DISTANCE) {
    // check if running
    if (vapor) {
      // stop vaporizer
      irsend.sendNEC(ON_OFF, 24);
      // Serial.println("Stop vaporizer!");
    }
    // set state to false
    vapor = false;
  }

  delay(100);
}
