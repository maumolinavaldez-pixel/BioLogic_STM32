#include <BioLogic_STM32.h>

BioLogic board;

void setup() {
  Serial.begin(115200);
  board.begin();
  Serial.println("BioLogic STM32 funcionando");
  delay(2000);
}

void loop() {
  board.relayOn(r1);
  delay(1000);
  board.relayOff(r1);
  delay(100);
  
  for (int a = 0; a<= 100 ; a = a + 10){
      board.pwmPercent(q1, a);
      delay(100);
  }
  delay(1000);
  board.pwmPercent(q1, 0);
  
  float volt = board.readVoltage(in1);
  Serial.print("Voltaje in1: ");
  Serial.println(volt);
  board.pinMode(r2, OUTPUT);
  board.digitalWrite(r2, HIGH);
  delay(500);
  board.digitalWrite(r2, LOW);
  delay(2000);
}