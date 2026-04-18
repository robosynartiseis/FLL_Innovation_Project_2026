// =================================================
// ΠΑΡΑΚΟΛΟΥΘΗΣΗ ΦΩΤΟΣ & ΕΛΕΓΧΟΣ SERVO
// =================================================
// Διαβάζει 4 LDR και κινεί τα servo προς το φως
//
// Κάνει:
// - ανάγνωση αισθητήρων φωτός
// - υπολογισμό μέσων όρων (πάνω/κάτω, αριστερά/δεξιά)
// - βρίσκει διαφορά φωτός
// - κινεί servo προς την πιο φωτεινή πλευρά
//
// Λειτουργεί σαν "sun tracking system"
// =================================================
void trackLightSensors() {

  // Έλεγχος χρόνου (millis-based)
  if (myTime - lightReadTime >= lightReadInterval) {
    
    // ===== ΑΝΑΓΝΩΣΗ LDR =====
    lt = analogRead(ldrltPin);
    rt = analogRead(ldrrtPin);
    ld = analogRead(ldrldPin);
    rd = analogRead(ldrrdPin);

    // ===== ΥΠΟΛΟΓΙΣΜΟΣ ΜΕΣΩΝ ΟΡΩΝ =====
    avt = (lt + rt) / 2;   // πάνω
    avd = (ld + rd) / 2;   // κάτω
    avl = (lt + ld) / 2;   // αριστερά
    avr = (rt + rd) / 2;   // δεξιά

    // ===== ΣΥΝΟΛΙΚΟ ΦΩΣ =====
    avgLight = (avt + avd + avl + avr) / 4;

    light = map(avgLight, 0, 1023, 0, 100);

    // ===== ΔΙΑΦΟΡΕΣ ΦΩΤΟΣ =====
    dvert = avt - avd;     // κάθετη διαφορά
    dhoriz = avl - avr;    // οριζόντια διαφορά

    // ===== ΚΑΘΕΤΗ ΚΙΝΗΣΗ =====
    if (abs(dvert) > tol) {

      // Αν πάνω έχει περισσότερο φως → ανεβαίνει
      if (avt > avd) servoVertPos--;
      else servoVertPos++;

      // Περιορισμός ορίων
      servoVertPos = constrain(servoVertPos, servoVertLimitLow, servoVertLimitHigh);
      
      // Κίνηση servo
      controlServoPWM(servoVertPin, servoVertPos);
    }
  
    // ===== ΟΡΙΖΟΝΤΙΑ ΚΙΝΗΣΗ =====
    if (abs(dhoriz) > tol) {

      // Αν αριστερά έχει περισσότερο φως → στρίβει
      if (avl > avr) servoHoriPos--;
      else servoHoriPos++;

      // Περιορισμός ορίων
      servoHoriPos = constrain(servoHoriPos, servoHoriLimitLow, servoHoriLimitHigh);

      // Κίνηση servo
      controlServoPWM(servoHoriPin, servoHoriPos);
    }

    // Reset timer
    lightReadTime = myTime;
  }
}

// =================================================
// ΑΝΑΓΝΩΣΗ ΑΙΣΘΗΤΗΡΩΝ & ΕΝΗΜΕΡΩΣΗ ΤΙΜΩΝ
// =================================================
// Διαβάζει όλους τους αισθητήρες του συστήματος:
// - Θερμοκρασία & Υγρασία (DHT20)
// - Πίεση & Υψόμετρο (SEN0516)
// - Βροχή
// - Φωτεινότητα
//
// Υποστηρίζει:
// - Πραγματικά δεδομένα
// - Fake δεδομένα (simulation)
//
// Εκτελείται με millis (non-blocking)
// και ενημερώνει τις βασικές μεταβλητές συστήματος
// =================================================
void readAllSensors() {
  
  // Εκτέλεση κάθε συγκεκριμένο χρονικό διάστημα
  if (myTime - sensorsReadTime >= sensorsReadInterval) {

    // ===== DHT SENSOR =====
    // Θερμοκρασία & Υγρασία
    if (useFakeDataDHT) {
      updateFakeDHT();

      temperature = fakeTemperature;
      humidity = fakeHumidity;

    } else {
      
      temperature = round1(dht20.getTemperature());
      humidity = round1(dht20.getHumidity() * 100);
    }

    // ===== PRESSURE SENSOR =====
    // Πίεση & Υψόμετρο
    if (useFakeDataSEN) {
      updateFakeSEN();

      pressure = fakePressure;
      altitude = fakeAltitude;
      
    } else {
      
      pressure = round1(abs(icp.getAirPressure()) / 100);
      altitude = round1(abs(icp.getElevation()));
    }

    // ===== RAIN SENSOR =====
    // Ανάγνωση βροχής
    if (useFakeDataRain) {
      updateFakeRain();

      rain = fakeRain;

    } else {
      
      rain = map(analogRead(rainDropPin), 0, 1023, 0, 100);
    }

    // ===== LIGHT SENSOR =====
    // Φωτεινότητα
    if (useFakeDataLight) {
      updateFakeLight();

      light = fakeLight;

    } else {
      
      light = map(avgLight, 0, 1023, 0, 100);
    }

    // ===== GNSS SENSOR =====
    // Θέση GPS (Latitude / Longitude)
    if (useFakeDataGNSS) {

      updateFakeGNSS();

      latitude = fakeLat;
      longitude = fakeLon;
      gnssFix = fakeGnssFix;

    } else {

      updateFakeGNSS();

      latitude = fakeLat;
      longitude = fakeLon;
      gnssFix = fakeGnssFix;
    }

    // ===== DEBUG OUTPUT =====
    // Εκτύπωση όλων των δεδομένων στη Serial
    if (debugMode) {

      Serial.println();
      Serial.println(F("=======================SENSORS======================="));
      
      Serial.print(F("System Uptime:"));
      Serial.println(uptime);
      
      Serial.print(F("Temperature:"));
      Serial.print(temperature, 1);
      Serial.println(F("C"));
      
      Serial.print(F("Humidity:"));
      Serial.print(humidity, 1);
      Serial.println(F("%"));
      
      Serial.print(F("Rain:"));
      Serial.print(rain);
      Serial.println(F("%"));
  
      Serial.print(F("Pressure:"));
      Serial.print(pressure, 1);
      Serial.println(F("hPa"));
      
      Serial.print(F("Altitude:"));
      Serial.print(altitude, 1);
      Serial.println(F("m"));
  
      Serial.print(F("Light:"));
      Serial.print(light);
      Serial.println(F("%"));
      
      Serial.print(F("Latitude: "));
      Serial.println(latitude, 6);

      Serial.print(F("Longitude: "));
      Serial.println(longitude, 6);

      Serial.print(F("GNSS Fix: "));
      Serial.println(gnssFix ? "YES" : "NO");

      Serial.print(F("Ancient Helmet: "));
      Serial.println(objectCounters[1]);
      
      Serial.print(F("Candle Holder:  "));
      Serial.println(objectCounters[2]);
      
      Serial.print(F("Ancient Statue: "));
      Serial.println(objectCounters[3]);
      
      Serial.print(F("Ancient Vase:   "));
      Serial.println(objectCounters[4]);
      
      Serial.println(F("=============================================="));
      Serial.println();
    }

    // Ενημέρωση timer
    sensorsReadTime = myTime;
  }
}
