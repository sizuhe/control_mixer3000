/* ----- PUMP ----- */
/* Turning on pump */
void pump_on() {
  analogWrite(PUMP_P1, pump_power);
  digitalWrite(PUMP_P2, LOW);
}


/* Setting power for pump */
// NEW CODE (mensaje): Encontrar mejor valor de funcionamiento y solo usar ese
void pump_receivedValue(uint8_t pump_pwm) {
  if (pump_pwm < 0) {
    pump_pwm = 0;
  } else if (pump_pwm > 150) {
    pump_pwm = 150;
  }

  pump_power = pump_pwm;
}


/* Ending power */
// Ending power to have more precision when dumping water
void pump_ending(const uint8_t PUMP_PWMENDING) {
  analogWrite(PUMP_P1, PUMP_PWMENDING);
  digitalWrite(PUMP_P2, LOW);
}


/* Turning off pump */
// NEW CODE
void pump_off() {
  analogWrite(PUMP_P1, 0);
  
  digitalWrite(PUMP_P1, LOW);
  digitalWrite(PUMP_P2, LOW);

  pump_power = 0;
  pump_signal = false;
}