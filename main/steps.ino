/* ----- STEP MOTOR ----- */
/* Initialization of the 2 step motors */
void step_init(const uint16_t maxSpeed) {
  STEP_MOTOR_MIX.setMaxSpeed(maxSpeed);
  STEP_MOTOR_MIX.setAcceleration(4000);

  STEP_MOTOR_HEIGHT.setMaxSpeed(maxSpeed);
  STEP_MOTOR_HEIGHT.setCurrentPosition(0);
  STEP_MOTOR_HEIGHT.setAcceleration(4000);
}


/* Power for mixing step motor */
// Setting mix step motor power, controlls value to have better performance when mixing
void step_receivedValue(uint16_t step_rpm) {
  if (step_rpm < 200) {
    step_rpm = 200;
  } else if (step_rpm > 400) {
    step_rpm = 400;
  }

  step_speed = step_rpm * 3.33;

  digitalWrite(STEP_ENA, LOW);
  STEP_MOTOR_MIX.setSpeed(step_speed);

  step_signal = true;
}


/* Turning off mix step motor */
void step_off() {
  STEP_MOTOR_MIX.setSpeed(0);
  digitalWrite(STEP_ENA, HIGH);

  encoder_rpm = 0;
  step_signal = false;
}


/* Step motor 2 goes down */
// Height step motor goes down to a referenced value
void motor2_goDown(){
  digitalWrite(STEP_ENA_2, LOW);
  
  STEP_MOTOR_HEIGHT.moveTo(-2800);
  STEP_MOTOR_HEIGHT.runToPosition();

  digitalWrite(STEP_ENA_2, HIGH);

  step2_signal = true;
}


/* Step motor goes up */
// Height step motor goes up to its zero value. Measured in code initialization
void motor2_gouP(){
  digitalWrite(STEP_ENA_2, LOW);

  STEP_MOTOR_HEIGHT.moveTo(0);
  STEP_MOTOR_HEIGHT.runToPosition();

  digitalWrite(STEP_ENA_2, HIGH);
  step_off();

  step2_signal = false;
}