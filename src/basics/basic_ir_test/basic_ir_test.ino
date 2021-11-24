/*
 * Author: Bors David <daviddvd267@gmail.com>
 * A very basic IR Reciever tester. It works for my setup, but the button probably won't work for you.
 * I would recommend commenting the if that switches the led to first map at least one of your buttons.
 * The program switches a led based on an IR recieved command.
 */

#include <IRremote.h>

const int rpin = 11;
const int lpin = 2;
bool pin_mode = LOW;

decode_results results;

void setup() {
  // will use serial print for getting all my remote's codes
  Serial.begin(9600);
  // prepare the receiver
  IrReceiver.begin(rpin, ENABLE_LED_FEEDBACK);
  // prepare the led
  pinMode(2, OUTPUT);
  Serial.println("started");
}

void _switch_led () {
  pin_mode ^= 1;
  digitalWrite(lpin, pin_mode);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (IrReceiver.decode()) {
      if (IrReceiver.decodedIRData.decodedRawData == 0xB946FF00)
        _switch_led();
      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
      IrReceiver.printIRResultShort(&Serial); // optional use new print version
      IrReceiver.resume(); // Enable receiving of the next value
  }
  delay(300);
}
