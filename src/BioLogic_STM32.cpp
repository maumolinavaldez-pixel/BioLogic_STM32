#include "BioLogic_STM32.h"

// Definición de las variables estáticas de la clase
const uint8_t BioLogic::relayPins[4] = {PB11, PB12, PB13, PB14};
const uint8_t BioLogic::pwmPins[4] = {PA8, PA9, PA10, PA11};
const uint8_t BioLogic::inputPins[8] = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7};
const uint8_t BioLogic::COMM_LED = PC13;

// Definición del timer estático (optimización clave)
HardwareTimer BioLogic::_timer1(TIM1);

// Constructor optimizado
BioLogic::BioLogic() {
    _initialized = false;
}

void BioLogic::begin() {
    // Configurar LED
    pinMode(COMM_LED, OUTPUT);
    digitalWrite(COMM_LED, LOW);
    
    // Configurar relés (sin escritura inicial para ahorrar ciclos)
    for (uint8_t i = 0; i < 4; i++) {
        pinMode(relayPins[i], OUTPUT);
    }
    
    // Configurar entradas
    analogReadResolution(12);
    for (uint8_t i = 0; i < 8; i++) {
        pinMode(inputPins[i], INPUT_PULLUP);
    }
    
    // Configurar PWM con Timer1 (ya instanciado estáticamente)
    _timer1.setOverflow(1000, HERTZ_FORMAT);
    
    // Configurar los 4 canales PWM
    _timer1.setMode(1, TIMER_OUTPUT_COMPARE_PWM1, pwmPins[0]);
    _timer1.setMode(2, TIMER_OUTPUT_COMPARE_PWM1, pwmPins[1]);
    _timer1.setMode(3, TIMER_OUTPUT_COMPARE_PWM1, pwmPins[2]);
    _timer1.setMode(4, TIMER_OUTPUT_COMPARE_PWM1, pwmPins[3]);
    
    _timer1.setCaptureCompare(1, 0, PERCENT_COMPARE_FORMAT);
    _timer1.setCaptureCompare(2, 0, PERCENT_COMPARE_FORMAT);
    _timer1.setCaptureCompare(3, 0, PERCENT_COMPARE_FORMAT);
    _timer1.setCaptureCompare(4, 0, PERCENT_COMPARE_FORMAT);
    
    _timer1.resume();
    
    _initialized = true;
    
    // LED de confirmación optimizado
    digitalWrite(COMM_LED, HIGH);
    delay(100);
    digitalWrite(COMM_LED, LOW);
    delay(100);
    digitalWrite(COMM_LED, HIGH);
}

void BioLogic::pinMode(uint8_t pin, uint8_t mode) {
    uint8_t physicalPin = _logicalToPhysical(pin);
    if (physicalPin == 255) return;
    ::pinMode(physicalPin, mode);
}

void BioLogic::digitalWrite(uint8_t pin, uint8_t value) {
    uint8_t physicalPin = _logicalToPhysical(pin);
    if (physicalPin == 255) return;
    
    ::digitalWrite(physicalPin, value);
    
    // LED de actividad con tiempo reducido
    digitalWrite(COMM_LED, LOW);
    delayMicroseconds(50);  // Reducido de 100μs
    digitalWrite(COMM_LED, HIGH);
}

void BioLogic::analogWrite(uint8_t pin, uint8_t value) {
    if (pin < q1 || pin > q4) return;
    
    // Conversión directa sin map() - optimización clave
    uint8_t percent = (value * 100UL) / 255;
    _timer1.setCaptureCompare((pin - q1) + 1, percent, PERCENT_COMPARE_FORMAT);
}

uint8_t BioLogic::digitalRead(uint8_t pin) {
    uint8_t physicalPin = _logicalToPhysical(pin);
    return ::digitalRead(physicalPin);
}

uint16_t BioLogic::analogRead(uint8_t pin) {
    uint8_t physicalPin = _logicalToPhysical(pin);   
    return ::analogRead(physicalPin);
}

void BioLogic::relayOn(uint8_t relayNum) {
    if (relayNum <= r4) digitalWrite(relayNum, HIGH);
}

void BioLogic::relayOff(uint8_t relayNum) {
    if (relayNum <= r4) digitalWrite(relayNum, LOW);
}

void BioLogic::relayToggle(uint8_t relayNum) {
    if (relayNum <= r4) {
        uint8_t currentState = digitalRead(relayNum);
        digitalWrite(relayNum, !currentState);
    }
}

void BioLogic::relayTimed(uint8_t relayNum, uint32_t durationMs) {
    if (relayNum <= r4) {
        relayOn(relayNum);
        delay(durationMs);
        relayOff(relayNum);
    }
}

void BioLogic::pwmPercent(uint8_t pwmNum, uint8_t percent) {
    if (pwmNum >= q1 && pwmNum <= q4) {
        if (percent > 100) percent = 100;
        // Conversión directa más eficiente
        uint8_t value = (percent * 255UL) / 100;
        analogWrite(pwmNum, value);
    }
}

uint16_t BioLogic::readVoltage(uint8_t inputNum) {
    if (inputNum >= in1 && inputNum <= in8) {
        // Cálculo entero para evitar float - optimización clave
        return (analogRead(inputNum) * 3300UL) / 4095;
    }
    return 0;
}

const char* BioLogic::getVersion() { 
    return BIOLOGIC_VERSION; 
}

const char* BioLogic::getAuthor() { 
    return BIOLOGIC_AUTHOR; 
}

void BioLogic::resetBoard() {
    // Apagar todo
    for (uint8_t i = r1; i <= r4; i++) {
        digitalWrite(i, LOW);
    }
    
    for (uint8_t i = q1; i <= q4; i++) {
        analogWrite(i, 0);
    }
    
    // LED de reset optimizado
    for (uint8_t i = 0; i < 5; i++) {
        digitalWrite(COMM_LED, LOW);
        delay(30);  // Reducido de 50ms
        digitalWrite(COMM_LED, HIGH);
        delay(30);  // Reducido de 50ms
    }
    
    delay(500);  // Reducido de 1000ms
}

uint8_t BioLogic::_logicalToPhysical(uint8_t logicalPin) {
    if (logicalPin <= r4) return relayPins[logicalPin];
    if (logicalPin <= q4) return pwmPins[logicalPin - q1];
    if (logicalPin <= in8) return inputPins[logicalPin - in1];
    return 255;
}