#ifndef BIOLOGIC_STM32_H
#define BIOLOGIC_STM32_H

#include <Arduino.h>
#include <HardwareTimer.h>

// Constantes para los pines lógicos
const uint8_t r1 = PB11;
const uint8_t r2 = PB12;
const uint8_t r3 = PB13;
const uint8_t r4 = PB14;
const uint8_t q1 = PA8;
const uint8_t q2 = PA9;
const uint8_t q3 = PA10;
const uint8_t q4 = PA11;
const uint8_t in1 = PA0;
const uint8_t in2 = PA1;
const uint8_t in3 = PA2;
const uint8_t in4 = PA3;
const uint8_t in5 = PA4;
const uint8_t in6 = PA5;
const uint8_t in7 = PA6;
const uint8_t in8 = PA7;
const uint8_t COMM_LED = PC13;


class BioLogic {
private:    
    // Variables de instancia
    static HardwareTimer _timer1;

public:
    // Constructor
    BioLogic();
    
    // Control de relés
    void pwmbegin();
    void relayOn(uint8_t relayNum);
    void relayOff(uint8_t relayNum);
    void relayToggle(uint8_t relayNum);
    void relayTimed(uint8_t relayNum, uint32_t durationMs);
    
    // Control PWM
    void pwmPercent(uint8_t pwmNum, uint8_t percent);
    
    // Lectura de sensores (optimizado: devuelve milivoltios enteros)
    uint16_t readVoltage(uint8_t inputNum);
};

#endif // BIOLOGIC_STM32_H