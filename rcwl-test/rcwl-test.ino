/*
 * RCWL-0516 - doppler radar microwave motion sensor
 */

#define INPUT_PIN 2
#define LED_PIN 13

int state = LOW;
int val = 0;

void setup() {
  
  pinMode(INPUT_PIN, INPUT);
  
  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(9600);

}

void loop() {
  
  val = digitalRead(INPUT_PIN);
  
  Serial.println("Sensor read");
  Serial.println(val);

  if (val == HIGH) {
    digitalWrite(LED_PIN, HIGH);
    if (state == LOW) {
      Serial.println("MOTION DETECTED");
      state = HIGH;
    }
  } else {
    digitalWrite(LED_PIN, LOW);
    if (state == HIGH) {
      Serial.println("MOTION ENDED");
      state = LOW;
    }
  }
}
