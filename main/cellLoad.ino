/* ----- LOADCELL ----- */
/* Loadcell initialization */
// This should be the average of multiples scales of the same weight
void loadCell_init() {
  float loadCell_scale = (- 142.22 - 186.48 - 252.74 - 215.82) / 4;

  LOAD_CELL.set_scale(loadCell_scale);
}


/* Calibration function */
// Useful for getting calibration curve
void loadCell_calibration(const int LOADCELL_KNOWNWEIGHT, const char TARE_QTY) {
  float loadCell_scaleCal, loadCell_avgMeasures;

  Serial.println(F("No ponga ningún objeto sobre la balanza"));
  
  LOAD_CELL.set_scale();
  LOAD_CELL.tare(TARE_QTY);

  float offset = LOAD_CELL.get_offset();
  Serial.print(F("El offset es: "));
  Serial.print(offset);

  Serial.println("");
  delay(50);
  Serial.print(F("...Destarando..."));
  delay(250);
  Serial.print(F("...Destarando..."));
  delay(250);
  Serial.println(F("Coloque un peso conocido "));
  delay(1000);

  for (int i=3; i>=0; i--) {
    Serial.print(" ... ");
    Serial.print(i);

    loadCell_avgMeasures = LOAD_CELL.get_value(20);   // Obtiene el promedio de las mediciones análogas según valor ingresado (20), usted decide el valor.
  }

  Serial.println(F(" "));
  Serial.println(F(" "));
  Serial.println(F("Retire el peso "));
  delay(250);

  for (int i=3; i>=0; i--) {
    Serial.print(" ... ");
    Serial.print(i);
    delay(1000); 
  }

  loadCell_scaleCal = loadCell_avgMeasures / LOADCELL_KNOWNWEIGHT;    // Relación entre el promedio de las mediciones análogas con el peso conocido en gramos
  LOAD_CELL.set_scale(loadCell_scaleCal);    // Ajusta la escala correspondiente

  Serial.println(F(" "));
  Serial.print(F("Escala: "));
  Serial.println(loadCell_scaleCal);
  Serial.println(F(" ")); 

  delay(2000);
}


/* Tare function */
// Use when there's bad measures around zero.
void loadCell_tare(const char TARE_QTY) {
    Serial.println(F("Tarando... Tarando... Tarando..."));
    LOAD_CELL.tare(TARE_QTY);

    Serial.println(F("--------------------"));
    Serial.println(F("¡¡¡LISTO PARA PESAR!!!")); 
    Serial.println(F("--------------------"));
}


/* Weight measuring */
// Measuring code with calibration curve
float loadCell_measure() {
  float loadCell_calibratedWeight;

  if (LOAD_CELL.is_ready()) {
    float loadCell_weight = LOAD_CELL.get_units() * -1;

    // if (loadCell_weight < 100) {
    //   loadCell_calibratedWeight = (loadCell_weight / 1.0034);
    // } else if (loadCell_weight > 100 && loadCell_weight < 300) {
    //   loadCell_calibratedWeight = (loadCell_weight - 0.5133) / 0.9972;
    // } else if (loadCell_weight > 300 && loadCell_weight < 400) {
    //   loadCell_calibratedWeight = (loadCell_weight + 1.29) / 1.0033;
    // } else if (loadCell_weight > 400) {
    //   // loadCell_calibratedWeight = (loadCell_weight + 0.01);
    //   loadCell_calibratedWeight = (loadCell_weight - 25.365) / 0.9391;
    // } 
    
    // if (loadCell_weight < 0) {
    //   loadCell_weight = loadCell_weight * -1.0;
    // }

    return loadCell_weight;
  }
}
