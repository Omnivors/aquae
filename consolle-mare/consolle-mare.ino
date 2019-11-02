// codice per animazione consolle con normali capsule ultrasuoni e lettore MP3 VS1053, su github si trovano le librerie e gli esempi di utilizzo.
// utilizzando la mediana delle misurazioni di distanza invece della media si eliminano quasi tutti i falsi rilevamenti

#define TRG_PIN 4
#define ECHO_PIN 3
#define MAX_DISTANCE 150      // massima distanza oltre la quale l'opera è inattiva
#define PLAY_DISTANCE 100           // distanza alla quale deve trovarsi lo spettatore per attivare l'opera
#define SHOW_TIME 60          // tempo per la riproduzione del contenuto multimediale
#define INIBIT_TIME 1        // tempo di attesa dopo la riproduzione del contenuto multimediale
#define SOLENOID_PUSH 500     // tempo di pressione del solenoide

#define SAMPLES_NUMBER 6 // of available samples

#include <NewPing.h>
#include <RunningMedian.h>
#include <SPI.h>

#include <SdFat.h>
//#include <SdFatUtil.h>
#include <SFEMP3Shield.h>

SdFat sd;
SFEMP3Shield MP3player;

int distance = 0;
NewPing sonar(TRG_PIN, ECHO_PIN, MAX_DISTANCE);

enum sound_state {SOUND_ON, SOUND_OFF};
sound_state sound = SOUND_OFF; // never used

// CYCLE THROUGH SAMPLES FILES
int sample = 0;
String filename_string;
char filename[7];

void setup() {

  uint8_t result; // result code from some function as to be tested at later time.
  Serial.begin(115200);
  Serial.print(F("F_CPU = "));
  Serial.println(F_CPU);
  Serial.print(F("Free RAM = ")); // available in Version 1.0 F() bases the string to into Flash, to use less SRAM.

  // Serial.print(FreeRam(), DEC);  // FreeRam() is provided by SdFatUtil.h

  Serial.println(F(" Should be a base line of 1028, on ATmega328 when using INTx"));


  // Initialize the SdCard.
  if(!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
  // depending upon your SdCard environment, SPI_HAVE_SPEED may work better.
  if(!sd.chdir("/")) sd.errorHalt("sd.chdir");

  // Initialize the MP3 Player Shield
  result = MP3player.begin();
  // check result, see readme for error codes.
  if (result != 0) {
    Serial.print(F("Error code: "));
    Serial.print(result);
    Serial.println(F(" when trying to start MP3 player"));
    if ( result == 6 ) {
      Serial.println(F("Warning: patch file not found, skipping.")); // can be removed for space, if needed.
      Serial.println(F("Use the \"d\" command to verify SdCard can be read")); // can be removed for space, if needed.
    }
  }

  pinMode(TRG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRG_PIN, LOW);

}

void loop() {

  //delay(60000); //abilitare se si vuole tempo di attesa di 1 minuto

  unsigned int uS = sonar.ping_median(10);
  distance = uS / US_ROUNDTRIP_CM;

  //Serial.println("Distanza: " + String(distance) + "cm");
  //delay(500);

  if (distance < PLAY_DISTANCE) {

    if (!MP3player.isPlaying()) {

      filename_string = "/" + String(sample) + ".mp3";
      filename_string.toCharArray(filename, 7);

      Serial.print("Play file ");
      Serial.println(filename);

      MP3player.playMP3(filename, 0);

      sample++;
      if (sample == SAMPLES_NUMBER) {
        sample = 0;
      }
    }

    /*for (int j = 0; j < SHOW_TIME; j++) {
      delay(1000);
    }*/

    sound = SOUND_ON;
  }

  /*if (distance > PLAY_DISTANCE) {
    MP3player.stopTrack();

    for(int j = 0; j < INIBIT_TIME; j++) {
      delay(1000);
    }
    sound = SOUND_OFF;
  }*/

}
