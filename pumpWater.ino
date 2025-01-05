/* ----- PUMP ----- */
/* Turning on pump */
void pump_on(uint8_t pump_pwm) {
  analogWrite(PUMP_P1, pump_pwm);
  digitalWrite(PUMP_P2, LOW);

  pump_signal = true;
}


/* Ending power */
// Ending power to have more precision when dumping water
void pump_ending(const uint8_t PUMP_PWMENDING) {
  analogWrite(PUMP_P1, PUMP_PWMENDING);
  digitalWrite(PUMP_P2, LOW);
}


/* Turning off pump */
void pump_off() {
  analogWrite(PUMP_P1, 0);
  
  digitalWrite(PUMP_P1, LOW);
  digitalWrite(PUMP_P2, LOW);

  pump_power = 0;
  pump_signal = false;
}