#include "BioLogic_STM32.h"
#include <HardwareTimer.h>

const uint8_t BioLogic::relayPins[4] = {PB11, PB12, PB13, PB14};
const uint8_t BioLogic::pwmPins[4] = {PA8, PA9, PA10, PA11};
const uint8_t BioLogic::inputPins[8] = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7};
const uint8_t BioLogic::COMM_LED = PC13;

HardwareTimer* _timer1 = nullptr;

BioLogic::BioLogic() {
    _pwmTimer = nullptr;
    _initialized = false;
}

void BioLogic::begin() {
    // Configurar LED
    pinMode(COMM_LED, OUTPUT);
    digitalWrite(COMM_LED, LOW);
    
    // Configurar relés
    for (int i = 0; i < 4; i++) {
        pinMode(relayPins[i], OUTPUT);
        digitalWrite(relayPins[i], LOW);
    }
    
    // Configurar entradas
    analogReadResolution(12);
    for (int i = 0; i < 8; i++) {
        pinMode(inputPins[i], INPUT_ANALOG);
    }
    
    // Configurar PWM con Timer1
    _timer1 = new HardwareTimer(TIM1);
    _timer1->setOverflow(1000, HERTZ_FORMAT);
    
    // Configurar los 4 canales PWM
    _timer1->setMode(1, TIMER_OUTPUT_COMPARE_PWM1, pwmPins[0]);
    _timer1->setMode(2, TIMER_OUTPUT_COMPARE_PWM1, pwmPins[1]);
    _timer1->setMode(3, TIMER_OUTPUT_COMPARE_PWM1, pwmPins[2]);
    _timer1->setMode(4, TIMER_OUTPUT_COMPARE_PWM1, pwmPins[3]);
    
    _timer1->setCaptureCompare(1, 0, PERCENT_COMPARE_FORMAT);
    _timer1->setCaptureCompare(2, 0, PERCENT_COMPARE_FORMAT);
    _timer1->setCaptureCompare(3, 0, PERCENT_COMPARE_FORMAT);
    _timer1->setCaptureCompare(4, 0, PERCENT_COMPARE_FORMAT);
    
    _timer1->resume();
    
    _initialized = true;
    
    // LED de confirmación
    digitalWrite(COMM_LED, HIGH);
    delay(100);
    digitalWrite(COMM_LED, LOW);
    delay(100);
    digitalWrite(COMM_LED, HIGH);
}

void BioLogic::pinMode(uint8_t pin, uint8_t mode) {
    uint8_t physicalPin = _logicalToPhysical(pin);
    
    if (physicalPin == 255) {
        return;
    }
    
    ::pinMode(physicalPin, mode);
}

void BioLogic::digitalWrite(uint8_t pin, uint8_t value) {
    uint8_t physicalPin = _logicalToPhysical(pin);
    
    if (physicalPin == 255) {
        return;
    }
    
    ::digitalWrite(physicalPin, value);
    
    // LED de actividad
    digitalWrite(COMM_LED, LOW);
    delayMicroseconds(100);
    digitalWrite(COMM_LED, HIGH);
}

void BioLogic::analogWrite(uint8_t pin, uint8_t value) {
    if (pin < q1 || pin > q4) {
        return;
    }
    
    int pwmIndex = pin - q1;
    
    if (_timer1 != nullptr) {
        // Convertir a porcentaje para el timer
        uint8_t percent = map(value, 0, 255, 0, 100);
        _timer1->setCaptureCompare(pwmIndex + 1, percent, PERCENT_COMPARE_FORMAT);
    }
}

uint8_t BioLogic::digitalRead(uint8_t pin) {
    uint8_t physicalPin = _logicalToPhysical(pin);
    uint8_t value = ::digitalRead(physicalPin);
    
    return value;
}

uint16_t BioLogic::analogRead(uint8_t pin) {
    uint8_t physicalPin = _logicalToPhysical(pin);   
    uint16_t value = ::analogRead(physicalPin);   
    return value;
}

void BioLogic::relayOn(uint8_t relayNum) {
    if (relayNum <= r4) {
        digitalWrite(relayNum, HIGH);
    }
}

void BioLogic::relayOff(uint8_t relayNum) {
    if (relayNum <= r4) {
        digitalWrite(relayNum, LOW);
    }
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
        uint8_t value = map(percent, 0, 100, 0, 255);
        analogWrite(pwmNum, value);
    }
}

float BioLogic::readVoltage(uint8_t inputNum) {
    if (inputNum >= in1 && inputNum <= in8) {
        uint16_t adcValue = analogRead(inputNum);
        float voltage = (adcValue * 3.3) / 4095.0;
        return voltage;
    }
    return 0.0;
}

String BioLogic::getVersion() {
    return String(BIOLOGIC_VERSION);
}

String BioLogic::getAuthor() {
    return String(BIOLOGIC_AUTHOR);
}

void BioLogic::resetBoard() {
    // Apagar todo
    for (int i = r1; i <= r4; i++) {
        digitalWrite(i, LOW);
    }
    
    for (int i = q1; i <= q4; i++) {
        analogWrite(i, 0);
    }
    
    // LED de reset
    for (int i = 0; i < 5; i++) {
        digitalWrite(COMM_LED, LOW);
        delay(50);
        digitalWrite(COMM_LED, HIGH);
        delay(50);
    }
    
    delay(1000);
}

uint8_t BioLogic::_logicalToPhysical(uint8_t logicalPin) {
    if (logicalPin <= r4) {
        return relayPins[logicalPin];
    }
    else if (logicalPin >= q1 && logicalPin <= q4) {
        return pwmPins[logicalPin - q1];
    }
    else if (logicalPin >= in1 && logicalPin <= in8) {
        return inputPins[logicalPin - in1];
    }
    
    return 255;
}