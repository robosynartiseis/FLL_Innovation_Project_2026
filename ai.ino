// =================================================
// CLEAR HUSKYLENS SERIAL BUFFER
// =================================================
// Καθαρίζει το buffer της Serial2 (HuskyLens)
//
// Κάνει:
// - διαβάζει και απορρίπτει όλα τα διαθέσιμα bytes
//
// Χρησιμοποιείται:
// - πριν από νέο request για αποφυγή "σκουπιδιών"
// =================================================
void clearHuskyLensSerial() {

  // Άδειασμα buffer
  while (Serial2.available()) {
    Serial2.read();
  }
}

// =================================================
// PRINT HUSKYLENS RESULT (DEBUG)
// =================================================
// Εμφανίζει δεδομένα αντικειμένου από HuskyLens στο Serial
//
// Κάνει:
// - εκτυπώνει ID, θέση, μέγεθος και metadata
// - μορφοποιεί output για ευκολότερο debugging
//
// Εκτελείται μόνο αν debugMode = true
// =================================================
void debugHuskyLensResult(Result *result) {

  // Εκτύπωση μόνο σε debug mode
  if (debugMode) {
  
    Serial.println();
    Serial.println(F("=======================HUSKYLENS======================="));
       
    // ID αντικειμένου
    Serial.print(F("result->ID="));
    Serial.println(result->ID, HEX);
  
    // Κέντρο αντικειμένου (x, y)
    Serial.print(F("result->Center=("));
    Serial.print(result->xCenter, HEX);
    Serial.print(F(","));
    Serial.print(result->yCenter, HEX);
    Serial.println(F(")"));
  
    // Πλάτος αντικειμένου
    Serial.print(F("Width="));
    Serial.println(result->width, HEX);
  
    // Ύψος αντικειμένου
    Serial.print(F("Height="));
    Serial.println(result->height, HEX);
  
    // Όνομα αντικειμένου
    Serial.print(F("Name="));
    Serial.println(result->name);
  
    // Περιεχόμενο / επιπλέον δεδομένα
    Serial.print(F("Content="));
    Serial.println(result->content);
  
    Serial.println(F("=============================================="));
    Serial.println();
  }
}

// =================================================
// HANDLE HUSKYLENS DATA
// =================================================
// Διαχειρίζεται ανάγνωση δεδομένων από HuskyLens
//
// Κάνει:
// - ελέγχει αν είναι ενεργό (όχι fake mode)
// - διαβάζει δεδομένα ανά χρονικό διάστημα
// - καθαρίζει buffer πριν την ανάγνωση
// - επεξεργάζεται όλα τα detected objects
//
// Χρησιμοποιείται στο loop() για συνεχή ανάγνωση
// =================================================
void updateHuskyLens() {

  // Αν είμαστε σε fake mode → δεν κάνουμε τίποτα
  if (useFakeDataHL) return;

  // ================= DISPLAY TIMER CONTROL =================
  if (aiDisplayActive) {

    // Αν πέρασε ο χρόνος εμφάνισης
    if (myTime - aiDisplayStartTime >= aiDisplayStartInterval) {
      aiDisplayActive = false;
      lcd.clear();
    }

    return;
  }

  // Έλεγχος χρόνου (millis-based)
  if (myTime - readhuskylensTime >= readhuskylensInterval) {

    // Καθαρισμός buffer πριν το request
    clearHuskyLensSerial();

    // Ζητάμε δεδομένα από HuskyLens
    if (huskylens.getResult(ALGORITHM_ANY)) {

      // Αν δεν υπάρχει τίποτα → reset ID
      if (!huskylens.available(ALGORITHM_ANY)) {
        lastDetectedID = 0;
      }

      // Επεξεργασία αποτελεσμάτων
      while (huskylens.available(ALGORITHM_ANY)) {

        // Ανάκτηση αποτελέσματος (V2 pointer)
        Result *result = static_cast<Result *>(huskylens.popCachedResult(ALGORITHM_ANY));

        // Debug
        debugHuskyLensResult(result);

        // ================= ANTI-DUPLICATE =================
        if (result->ID != lastDetectedID) {

          // Αν το ID είναι valid (1–4)
          if (result->ID >= 1 && result->ID <= 4) {

            // Ενημέρωση counters (variables)
            if (result->ID == 1) objectCounters[1]++;
            if (result->ID == 2) objectCounters[2]++;
            if (result->ID == 3) objectCounters[3]++;
            if (result->ID == 4) objectCounters[4]++;
          }

          // Αποθήκευση τελευταίου ID
          lastDetectedID = result->ID;

          // Εντοπισμός αντικειμένου
          artifactDetected = true;
          
          // ================= DISPLAY RESULTS =================
          lcd.clear();

          lcd.setCursor(0, 0);
          lcd.print(F("Ancient Helmet: "));
          lcd.print(objectCounters[1]);

          lcd.setCursor(0, 1);
          lcd.print(F("Candle Holder:  "));
          lcd.print(objectCounters[2]);

          lcd.setCursor(0, 2);
          lcd.print(F("Ancient Statue: "));
          lcd.print(objectCounters[3]);

          lcd.setCursor(0, 3);
          lcd.print(F("Ancient Vase:   "));
          lcd.print(objectCounters[4]);

          // Ενεργοποίηση display mode
          aiDisplayActive = true;
          aiDisplayStartTime = myTime;

          break;
        } else {
          // Εντοπισμός αντικειμένου
          artifactDetected = false;
        }
      }
    }

    // Reset timer
    readhuskylensTime = myTime;
  }
}
