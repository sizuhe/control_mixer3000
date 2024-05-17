/* ----- ENCODER -----*/
/* Encoder measuring */
// Encoder lecture from mix step motor
void encoder_measure(const uint8_t ENCODER_HOLES) {
  // encoder_output = digitalRead(ENCODER_PIN);
  encoder_output = PIND & (1 << ENCODER_PIN);

  if(!encoder_output && encoder_status) {
    encoder_status = false;
  } else if(encoder_output && !encoder_status) {
    encoder_pulses ++;
    encoder_status = true;
  }

  if(encoder_pulses >= ENCODER_HOLES) {
    encoder_pulses = 0; 
    encoder_timeFinal = millis();
    encoder_rpm = 60000 / (encoder_timeFinal - encoder_timeInitial);
    encoder_timeInitial = encoder_timeFinal;
  }
}