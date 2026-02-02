#ifndef BIOLOGIC_H
#define BIOLOGIC_H

#include <Arduino.h>

#define BIOLOGIC_VERSION "1.0.0"
#define BIOLOGIC_AUTHOR "@teoriademau"

// Pines lógicos EXACTAMENTE iguales
#define r1 0
#define r2 1
#define r3 2
#define r4 3

#define q1 4
#define q2 5
#define q3 6
#define q4 7

#define in1 8
#define in2 9
#define in3 10
#define in4 11
#define in5 12
#define in6 13
#define in7 14
#define in8 15

class BioLogic {
public:
    // Constructores EXACTAMENTE iguales
    BioLogic();
    
    // Métodos EXACTAMENTE iguales
    void begin();
    
    void pinMode(uint8_t pin, uint8_t mode);
    void digitalWrite(uint8_t pin, uint8_t value);
    void analogWrite(uint8_t pin, uint8_t value);
    uint8_t digitalRead(uint8_t pin);
    uint16_t analogRead(uint8_t pin);
    
    void relayOn(uint8_t relayNum);
    void relayOff(uint8_t relayNum);
    void relayToggle(uint8_t relayNum);
    void relayTimed(uint8_t relayNum, uint32_t durationMs);
    
    void pwmPercent(uint8_t pwmNum, uint8_t percent);
    float readVoltage(uint8_t inputNum);    
    void resetBoard();

private:
    // Variables privadas EXACTAMENTE iguales
    
    // Pines físicos (los que diste)
    static const uint8_t relayPins[4];
    static const uint8_t pwmPins[4];
    static const uint8_t inputPins[8];
    static const uint8_t COMM_LED;
    
    // Timer para PWM
    void* _pwmTimer;
    
    // Métodos internos
    uint8_t _logicalToPhysical(uint8_t logicalPin);
};

#endif