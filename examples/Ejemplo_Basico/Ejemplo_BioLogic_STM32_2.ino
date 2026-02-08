#include <BioLogic_STM32.h>

BioLogic board;

void setup() {
  Serial.begin(115200);
  pinMode(r1, OUTPUT);
  delay(10);
}

void loop() {
   board.relayOn(r1);
   delay(1000);
   board.relayOff(r1);
   delay(1000);
}