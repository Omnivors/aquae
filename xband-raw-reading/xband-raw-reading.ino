// Reading X-Band
// 10.525 GHz X-Band Motion Detector (#32213) radar by www.parallax.com.

// Constants.

const int baudRate = 9600;
const int ledPin = 13;
const int usLoopDelay = 50;
const int xBandPin = 8;

// Initial code to run once.

void setup()
{
  // Assign modes to pins.

  pinMode(xBandPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Start serial communication to Max 7.

  Serial.begin(baudRate);
}


// Loop code to run perpetually.

void loop()
{

// Read value of motion sensor.

int sensorValue = digitalRead(xBandPin);

// LED shows current status of sensorValue.

digitalWrite(ledPin, sensorValue ? HIGH : LOW);

// A small delay keeps things tidy.

delayMicroseconds(usLoopDelay);

Serial.print(sensorValue);

}
