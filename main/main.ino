#include "HX711.h"
#include <AccelStepper.h>
#include <OneWire.h>                
#include <DallasTemperature.h>
 

// ----- PIN VARIABLES -----
// Load cell
#define DOUT A0
#define CLK A1

// Water pump
#define PUMP_P1 9
#define PUMP_P2 10

// Step motor
#define STEP_PIN 2
#define STEP_DIR 5
#define STEP_ENA 6

// Second Step motor
#define STEP_PIN_2 3
#define STEP_DIR_2 4
#define STEP_ENA_2 8

// Encoder
#define ENCODER_PIN 7

// Temperature
#define TEMP_PIN 11


// ----- CODE VARIABLES -----
// Others
uint16_t serialCounter = 0;

// Load cell
HX711 LOAD_CELL;
float loadCell_weightDefined, loadCell_weightDefined_real, loadCell_weightMeasured;

// Water pump
uint16_t pump_power;
bool pump_signal = false;
float loadCell_weightMeasured_compare = 0.0;

// Step motor
uint8_t stepValue = 0;
bool step_signal = false;
bool step2_signal = false;

// Encoder
uint16_t encoder_rpm;
unsigned long encoder_timeFinal;
unsigned long encoder_timeInitial = 0;
float step_speed = 0.0;
bool encoder_output = LOW;
bool encoder_status = true;
uint8_t encoder_pulses = 0;

// Serial update and reading of sensors
unsigned long previousTime = 0;


// Instance receives pins (STEP, DIR)
AccelStepper STEP_MOTOR_MIX(1, STEP_PIN, STEP_DIR); 
AccelStepper STEP_MOTOR_HEIGHT(1, STEP_PIN_2, STEP_DIR_2);

OneWire ourWire(TEMP_PIN);
DallasTemperature TEMP_SENSOR(&ourWire);

/*
----- CAUDALS -----
050 PWM = 3.49 g/s
070 PWM = 5.53 g/s
090 PWM = 8.34 g/s
110 PWM = 9.67 g/s
130 PWM = 11.38 g/s
150 PWM = 12.78 g/s
170 PWM = 14.56 g/s
190 PWM = 16.02 g/s
210 PWM = 16.86 g/s
230 PWM = 17.67 g/s
255 PWM = 21.99 g/s
*/

 

void setup() {
  Serial.begin(9600);

  // Load cell config
  LOAD_CELL.begin(DOUT, CLK);
  loadCell_init();

  // Water pump
  pinMode(PUMP_P1, OUTPUT);
  pinMode(PUMP_P2, OUTPUT);

  // Encoder
  pinMode(ENCODER_PIN,INPUT);

  // Step motors
  step_init(2000);

  pinMode(STEP_ENA, OUTPUT);
  pinMode(STEP_ENA_2, OUTPUT);
  digitalWrite(STEP_ENA, HIGH);
  digitalWrite(STEP_ENA_2, HIGH);

  // Temperature
  TEMP_SENSOR.begin();
  TEMP_SENSOR.setResolution(9);
  TEMP_SENSOR.setWaitForConversion(false);
}


void loop() {
  uint16_t dataSerial = 0;
  uint16_t dataValue = 0;
  uint8_t dataCode = 0;
  uint16_t pump_endingValue_signal = loadCell_weightDefined - 30;
  uint16_t pump_finishValue_signal = loadCell_weightDefined - 3;    // Minus max caudal value

  loadCell_weightMeasured = loadCell_measure();
  encoder_measure(40);    // Receives (ENCODER_HOLES)

  // Bluetooth readings are done each second.
  unsigned long currentTime = millis();
  if (currentTime - previousTime >= 1000) {
    /*----- SERIAL READING ----- */
    /* Reads a 5 digit number and splits it into two values: DATACODE (0:1) and DATAVALUE (2:4).
        Eg: 25600 read as 25 (CODE) and 600 (VALUE) 

      Serial.parseInt returns a Zero after reading, this is due to newline ('\n') or a carriage
      return ('\r') value at the end on the line.
      Serial.read kills its 1 second wait, improving code performance.
      This code may be memory optimized if only Serial.read was used. 
    */
    if (Serial.available() > 0) {
      dataSerial = Serial.parseInt();
      if (Serial.read() == '\n') {}    // Revisar configuracion del Serial

      if (dataSerial > 99){
        dataCode = dataSerial / 1000;
        dataValue = dataSerial % 1000;
      } else {
        dataCode = dataSerial;
      }

      switch (dataCode) {
        case 99:
          step_off();
          pump_off();
          motor2_gouP();
          break;

        case 10:
          loadCell_calibration(dataValue, 100);    // Receives (KNOWN_WEIGHT, TARE_VALUE)
          break;

        case 11:
          loadCell_tare(100);
          break;

        case 20:
          loadCell_weightDefined = dataValue;
          pump_on(125);
          break;

        case 21:
          pump_off();
          break;

        case 30:
          if (!step2_signal) {
            motor2_goDown();
          }
          step_receivedValue(dataValue);
          stepValue = dataValue;
          break;

        case 31:
          step_off();
          break;
        
        case 40:
          motor2_goDown();
          break;
        
        case 41:
          motor2_gouP();
          break;
      }
    }

    TEMP_SENSOR.requestTemperatures();
    float temp_value = TEMP_SENSOR.getTempCByIndex(0);

    Serial.print(loadCell_weightMeasured, 1);
    Serial.print(F(","));
    Serial.print(encoder_rpm);
    Serial.print(F(","));
    Serial.println(temp_value);
    
    previousTime = currentTime;
  }

  // Activation signals for pump and step
  if (pump_signal) {
    if (loadCell_weightMeasured >= pump_endingValue_signal) {
      pump_ending(70);   // PWM when getting to set value

      if (loadCell_weightMeasured >= pump_finishValue_signal) {
        pump_off();
      }
    }
  }

  if (step_signal) {
    STEP_MOTOR_MIX.runSpeed();
  }
}