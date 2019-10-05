#include <IRremote.h>

IRsend irsend;

String ir[3];

unsigned int irBuf[100];
unsigned int irBufLen;
unsigned int irBufType;

String playPause = "16720095";

void setup() {
  Serial.begin(9600);
}

void loop() {
  irCommand(playPause);
  sendCode();
  delay(5000);
}

void sendCode() {
  Serial.println("send code");
  irsend.sendRaw(irBuf, irBufLen, 38);
  delay(50);
}

void irCommand(String codeValue) {

  irBufLen = codeValue.toInt();
    
  stringToIntArry(codeValue);
  
  Serial.println("Command" + codeValue);  
}

void stringToIntArry(String irRawString) {
  String dataShort = "";
  int counter = 0;
  for (int i = 0; i <= irRawString.length(); i++) {
    dataShort += irRawString[i];
    if (irRawString[i] == ',') {
      dataShort = dataShort.substring(0, dataShort.length() - 1 );
      irBuf[counter] = dataShort.toInt();
      counter = counter + 1;
      dataShort = "";
    }
  }
}
