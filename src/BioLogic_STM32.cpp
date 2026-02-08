#include "BioLogic_STM32.h"


// Definición del timer estático (optimización clave)
HardwareTimer BioLogic::_timer1(TIM1);
    static HardwareTimer _timer1;
    bool _initialized;

// Constructor optimizado
BioLogic::BioLogic() {
    
}
void BioLogic::pwmbegin(){
    _timer1.setOverflow(1000, HERTZ_FORMAT);
    _timer1.setMode(1, TIMER_OUTPUT_COMPARE_PWM1, q1);
    _timer1.setMode(2, TIMER_OUTPUT_COMPARE_PWM1, q2);
    _timer1.setMode(3, TIMER_OUTPUT_COMPARE_PWM1, q3);
    _timer1.setMode(4, TIMER_OUTPUT_COMPARE_PWM1, q4);
    
    _timer1.setCaptureCompare(1, 0, PERCENT_COMPARE_FORMAT);
    _timer1.setCaptureCompare(2, 0, PERCENT_COMPARE_FORMAT);
    _timer1.setCaptureCompare(3, 0, PERCENT_COMPARE_FORMAT);
    _timer1.setCaptureCompare(4, 0, PERCENT_COMPARE_FORMAT);
    
    _timer1.resume();
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
        uint8_t value = (percent * 255UL) / 100;
        analogWrite(pwmNum, value);
    }
}

uint16_t BioLogic::readVoltage(uint8_t inputNum) {
    if (inputNum >= in1 && inputNum <= in8) {
        return (analogRead(inputNum) * 3300UL) / 4095;
    }
    return 0;
}