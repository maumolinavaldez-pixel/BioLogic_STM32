#ifndef BIOLOGIC_STM32_H
#define BIOLOGIC_STM32_H

#include <Arduino.h>
#include <HardwareTimer.h>

// Constantes para los pines lógicos
const uint8_t r1 = 0;
const uint8_t r2 = 1;
const uint8_t r3 = 2;
const uint8_t r4 = 3;
const uint8_t q1 = 4;
const uint8_t q2 = 5;
const uint8_t q3 = 6;
const uint8_t q4 = 7;
const uint8_t in1 = 8;
const uint8_t in2 = 9;
const uint8_t in3 = 10;
const uint8_t in4 = 11;
const uint8_t in5 = 12;
const uint8_t in6 = 13;
const uint8_t in7 = 14;
const uint8_t in8 = 15;

// Versión de la biblioteca
#define BIOLOGIC_VERSION "1.0.2"
#define BIOLOGIC_AUTHOR "Mau Molina Valdez"

class BioLogic {
private:
    // Variables estáticas compartidas por todas las instancias
    static const uint8_t relayPins[4];
    static const uint8_t pwmPins[4];
    static const uint8_t inputPins[8];
    static const uint8_t COMM_LED;
    
    // Variables de instancia (no estáticas)
    HardwareTimer* _pwmTimer;
    bool _initialized;
    
    // Método privado para conversión de pines
    uint8_t _logicalToPhysical(uint8_t logicalPin);

public:
    // Constructor
    BioLogic();
    
    // Método de inicialización único (sin sobrecarga para I2C)
    void begin();
    
    // Métodos de control básicos
    void pinMode(uint8_t pin, uint8_t mode);
    void digitalWrite(uint8_t pin, uint8_t value);
    uint8_t digitalRead(uint8_t pin);
    
    // Métodos de control avanzado
    void analogWrite(uint8_t pin, uint8_t value);
    uint16_t analogRead(uint8_t pin);
    
    // Control de relés
    void relayOn(uint8_t relayNum);
    void relayOff(uint8_t relayNum);
    void relayToggle(uint8_t relayNum);
    void relayTimed(uint8_t relayNum, uint32_t durationMs);
    
    // Control PWM
    void pwmPercent(uint8_t pwmNum, uint8_t percent);
    
    // Lectura de sensores
    float readVoltage(uint8_t inputNum);
    
    // Información del sistema
    String getVersion();
    String getAuthor();
    void resetBoard();
};

#endif // BIOLOGIC_STM32_H