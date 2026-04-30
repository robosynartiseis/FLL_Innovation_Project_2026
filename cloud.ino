// =================================================
// ΑΡΧΙΚΟΠΟΙΗΣΗ ARDUINO CLOUD
// =================================================
// Αρχικοποιεί τη σύνδεση με το Arduino Cloud
//
// Κάνει:
// - ελέγχει αν επιτρέπεται χρήση Cloud
// - ξεκινά σύνδεση ή περνά σε offline mode
// - ορίζει αρχικό state συστήματος
//
// Χρησιμοποιείται στο setup()
// =================================================
void initCloud() {

  // ================= OFFLINE MODE =================
  // Αν το σύστημα είναι σε manual offline mode
  if (offlineMode) {
    
    Serial.println("Cloud DISABLED → MANUAL OFFLINE MODE");

    // Ενημέρωση κατάστασης
    cloudState = CLOUD_MANUAL_OFFLINE;

    return;
  }
  
  // ================= INIT CLOUD =================
  Serial.println("Cloud INIT...");

  // Θέτουμε κατάσταση CONNECTING
  cloudState = CLOUD_CONNECTING;

  // Εκκίνηση Arduino Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  // Ρύθμιση debug επιπέδου (0–4)
  setDebugMessageLevel(2);

  // Εκτύπωση πληροφοριών σύνδεσης
  ArduinoCloud.printDebugInfo();
}

// =================================================
// HANDLE CLOUD CONNECTION (SAFE + NON-BLOCKING)
// =================================================
// Διαχειρίζεται τη σύνδεση με Arduino Cloud
//
// Κάνει:
// - χειρίζεται states (CONNECTING, ONLINE, OFFLINE)
// - χρησιμοποιεί millis (όχι delay)
// - περιορίζει reconnect attempts
// - απενεργοποιεί Cloud μετά από αποτυχίες
//
// Στόχος:
// - αποφυγή lag / reset όταν δεν υπάρχει internet
// =================================================
void updateCloudConnection() {

  switch (cloudState) {

    // ================= CONNECTING =================
    // Αρχική προσπάθεια σύνδεσης
    case CLOUD_CONNECTING:

      // Καταγραφή χρόνου πρώτης προσπάθειας
      if (lastAttemptTime == 0) {
        lastAttemptTime = myTime;
      }

      // Συχνό update για γρήγορη σύνδεση
      if (myTime - lastCloudUpdateTime > fastUpdateInterval) {
        ArduinoCloud.update();
        lastCloudUpdateTime = myTime;
      }

      // Επιτυχής σύνδεση
      if (ArduinoCloud.connected()) {
        Serial.println("Cloud Connected!");
        cloudState = CLOUD_ONLINE;
        failedAttempts = 0;
      }

      // Timeout → πάμε OFFLINE
      if (myTime - lastAttemptTime > cloudConnectionTimeout) {
        Serial.println("Cloud timeout → OFFLINE");
        cloudState = CLOUD_OFFLINE;
        lastAttemptTime = myTime;
        failedAttempts++;
      }

      break;


    // ================= ONLINE =================
    // Κανονική λειτουργία
    case CLOUD_ONLINE:

      // Συνεχές update για dashboard
      if (myTime - lastCloudUpdateTime > fastUpdateInterval) {
        ArduinoCloud.update();
        lastCloudUpdateTime = myTime;
      }

      // Αν χαθεί σύνδεση
      if (!ArduinoCloud.connected()) {
        Serial.println("Cloud LOST!");
        cloudState = CLOUD_OFFLINE;
        lastAttemptTime = myTime;
      }

      break;


    // ================= OFFLINE =================
    // Προσπάθεια επανασύνδεσης με περιορισμό
    case CLOUD_OFFLINE:

      // Soft reconnect κάθε λίγα δευτερόλεπτα
      if (myTime - lastAttemptTime > slowUpdateInterval) {

        Serial.println("Soft reconnect attempt...");

        lastAttemptTime = myTime;

        // Μία μόνο προσπάθεια (non-blocking)
        ArduinoCloud.update();
      }

      // Αν επανήλθε σύνδεση
      if (ArduinoCloud.connected()) {
        Serial.println("Cloud Reconnected!");
        cloudState = CLOUD_ONLINE;
        failedAttempts = 0;
      }

      // Αν αποτύχει πολλές φορές → disable cloud
      if (myTime - lastAttemptTime > slowUpdateInterval + cloudReconnectGracePeriod) {

        Serial.print("Reconnect failed: ");
        Serial.println(failedAttempts + 1);
        
        failedAttempts++;

        if (failedAttempts >= maxFailedAttempts) {

          Serial.println("CLOUD DISABLED → MANUAL OFFLINE");

          offlineMode = true;
          cloudState = CLOUD_MANUAL_OFFLINE;
        }
      }

      break;
  }
}

// =================================================
// UPDATE CLOUD VARIABLES
// =================================================
// Ενημερώνει όλες τις μεταβλητές του Arduino Cloud
//
// Κάνει:
// - εκτελείται ανά συγκεκριμένο χρονικό διάστημα (millis)
// - διαβάζει αισθητήρες ή fake δεδομένα
// - μετατρέπει τιμές σε κατάλληλη μορφή
// - στέλνει δεδομένα στο Cloud
// - εμφανίζει debug πληροφορίες (αν ενεργό)
//
// Κεντρική συνάρτηση για συγχρονισμό δεδομένων
// =================================================
void updateCloudData() {

  // Έλεγχος χρόνου (millis-based, non-blocking)
  if (myTime - cloudVariablesTime >= cloudVariablesInterval) {

    // ===== DHT SENSOR =====
    cloudTemperature = temperature;
    cloudHumidity = humidity;

    // ===== PRESSURE SENSOR =====
    cloudPressure = pressure;
    cloudAltitude = altitude;
    
    // ===== RAIN SENSOR =====
    cloudRain = rain;

    // ===== LIGHT SENSOR =====
    cloudLight = light;
    
    // ===== GNSS SENSOR =====
    cloudLocation = String(latitude, 6) + "," + String(longitude, 6);
      
    // ===== AI OBJECT VALUES =====
    cloudAncientHelmet = objectCounters[1];
    cloudCandleHolder = objectCounters[2];
    cloudAncientStatue = objectCounters[3];
    cloudAncientVase = objectCounters[4];

    // ===== ARTIFACT DETECTION =====
    cloudArtifactDetected = artifactDetected;

    // ===== ALERT DETECTION =====
    cloudAlertDetected = alertDetected;
    
    // ===== LIGHT AVERAGES =====
    cloudAverageTop = avtPerc;
    cloudAverageBottom = avdPerc;
    cloudAverageRight = avrPerc;
    cloudAverageLeft = avlPerc;

    // ===== DEBUG OUTPUT =====
    if (debugMode) {

      Serial.println();
      Serial.println(F("=======================CLOUD======================="));
      
      Serial.print(F("System Uptime:"));
      Serial.println(cloudUptime);
      
      Serial.print(F("Temperature:"));
      Serial.print(cloudTemperature, 1);
      Serial.println(F("C"));
      
      Serial.print(F("Humidity:"));
      Serial.print(cloudHumidity, 1);
      Serial.println(F("%"));
      
      Serial.print(F("Rain:"));
      Serial.print(cloudRain);
      Serial.println(F("%"));
  
      Serial.print(F("Pressure:"));
      Serial.print(cloudPressure, 1);
      Serial.println(F("hPa"));
      
      Serial.print(F("Altitude:"));
      Serial.print(cloudAltitude, 1);
      Serial.println(F("m"));
  
      Serial.print(F("Light:"));
      Serial.print(cloudLight);
      Serial.println(F("%"));

      Serial.print(F("Location:"));
      Serial.print(cloudLocation);
      
      Serial.print(F("Ancient Helmet: "));
      Serial.println(cloudAncientHelmet);
      
      Serial.print(F("Candle Holder:  "));
      Serial.println(cloudCandleHolder);
      
      Serial.print(F("Ancient Statue: "));
      Serial.println(cloudAncientStatue);
      
      Serial.print(F("Ancient Vase:   "));
      Serial.println(cloudAncientVase);
      
      Serial.println(F("=============================================="));
      Serial.println();
    }

    // Reset timer
    cloudVariablesTime = myTime;
  }
}

// =================================================
// ΧΕΙΡΟΚΙΝΗΤΟΣ ΕΛΕΓΧΟΣ ΚΙΝΗΣΗΣ ΑΠΟ CLOUD
// =================================================
// Διαχειρίζεται την κίνηση του ρομπότ μέσω Arduino Cloud.
//
// Λειτουργία:
// - Momentary control:
//    → Όσο το κουμπί είναι πατημένο → το ρομπότ κινείται
//    → Όταν αφήνεται → το ρομπότ σταματά
//
// - Υποστηρίζει:
//    → Forward  (μπροστά)
//    → Backward (πίσω)
//    → Right    (δεξιά)
//    → Left     (αριστερά)
//
// Επιπλέον:
// - Εκτελεί ΜΟΝΟ μία εντολή κάθε φορά (priority σειρά)
// - Εμφανίζει μήνυμα στην LCD ΜΟΝΟ όταν αλλάζει κίνηση
// - Αποφεύγει flickering με χρήση lastState
//
// Χρησιμοποιεί:
// - cloudForward / cloudBackward / cloudRight / cloudLeft
// =================================================
void controlCloudMovement() {

  // Αποθήκευση προηγούμενης κατάστασης
  static MovementDirection lastDirection = DIR_STOP;
  MovementDirection currentDirection = DIR_STOP;

  // ================= MANUAL MODE =================
  // Εκτελείται μόνο όταν είναι ενεργό το manual mode
  if (cloudManualDrive) {

    // ================= DETECT COMMAND =================
    // Προτεραιότητα εντολών (μόνο μία κάθε φορά)
    if (cloudForward) {
      currentDirection = DIR_FORWARD;
    }
    else if (cloudBackward) {
      currentDirection = DIR_BACKWARD;
    }
    else if (cloudRight) {
      currentDirection = DIR_RIGHT;
    }
    else if (cloudLeft) {
      currentDirection = DIR_LEFT;
    }
    else {
      currentDirection = DIR_STOP;
    }
  
    // ================= EXECUTE MOVEMENT =================
    // Continuous (momentary) control
    switch (currentDirection) {
  
      case DIR_FORWARD:
        moveForward();
        break;
  
      case DIR_BACKWARD:
        moveBackward();
        break;
  
      case DIR_RIGHT:
        turnRight();
        break;
  
      case DIR_LEFT:
        turnLeft();
        break;
  
      case DIR_STOP:
      default:
        stopMotors();
        break;
    }
  
    // ================= MESSAGE ON CHANGE =================
    // Εμφάνιση μηνύματος μόνο όταν αλλάζει η κατεύθυνση
    if (currentDirection != lastDirection) {
  
      // Μήνυμα στην LCD
      lcd.clear();
      displayLine(0, "====================");
      displayLineCentered(1, "Manual Drive");
  
      switch (currentDirection) {
        case 1:
          displayLineCentered(2, "Moving Forward");
          break;
        case 2:
          displayLineCentered(2, "Moving Backward");
          break;
        case 3:
          displayLineCentered(2, "Turning Right");
          break;
        case 4:
          displayLineCentered(2, "Turning Left");
          break;
        default:
          displayLineCentered(2, "Stopped");
          break;
      }
  
      displayLine(3, "====================");
  
      // Ενεργοποίηση message timer
      modeMessageActive = true;
      modeMessageStartTime = myTime;
    }
  
    // ================= UPDATE STATE =================
    lastDirection = currentDirection;
  }
}

// =================================================
// ΧΕΙΡΟΚΙΝΗΤΟΣ ΕΛΕΓΧΟΣ ΚΟΥΡΤΙΝΩΝ (ΑΝΑ ΚΟΥΡΤΙΝΑ)
// =================================================
// Διαχειρίζεται ανεξάρτητα κάθε κουρτίνα μέσω Cloud.
//
// Λειτουργία:
// - Momentary control:
//    → Όσο το κουμπί είναι πατημένο → η κουρτίνα κινείται
//    → Όταν αφήνεται → σταματά
//
// - Υποστηρίζει για κάθε κουρτίνα:
//    → Open  (άνοιγμα)
//    → Close (κλείσιμο)
//
// Επιπλέον:
// - Εμφανίζει μήνυμα στην LCD ΜΟΝΟ όταν ξεκινά η κίνηση
// - Αποφεύγει flickering με χρήση lastState
//
// Χρησιμοποιεί:
// - cloudOpenX / cloudCloseX για κάθε κουρτίνα
// =================================================
void controlCloudCurtainsIndividual() {

  // ================= LAST STATES =================
  // Αποθήκευση προηγούμενων καταστάσεων για κάθε κουρτίνα
  static bool lastFrontOpen = false, lastFrontClose = false;
  static bool lastRearOpen = false, lastRearClose = false;
  static bool lastRightOpen = false, lastRightClose = false;
  static bool lastLeftOpen = false, lastLeftClose = false;

  // ================= FRONT CURTAIN =================
  if (cloudOpenFrontCurtains) {

    openFrontCurtain();

    if (!lastFrontOpen) {

      // Μήνυμα στην LCD
      lcd.clear();
      displayLine(0, "====================");
      displayLineCentered(1, "Manual Mode");
      displayLineCentered(2, "Front Opening");
      displayLine(3, "====================");

      // Ενεργοποίηση message timer
      modeMessageActive = true;
      modeMessageStartTime = myTime;
    }
  }

  else if (cloudCloseFrontCurtains) {

    closeFrontCurtain();

    if (!lastFrontClose) {

      // Μήνυμα στην LCD
      lcd.clear();
      displayLine(0, "====================");
      displayLineCentered(1, "Manual Mode");
      displayLineCentered(2, "Front Closing");
      displayLine(3, "====================");

      // Ενεργοποίηση message timer
      modeMessageActive = true;
      modeMessageStartTime = myTime;
    }
  }

  else {
    stopFrontCurtain();
  }

  // ================= REAR CURTAIN =================
  if (cloudOpenRearCurtains) {

    openRearCurtain();

    if (!lastRearOpen) {

      // Μήνυμα στην LCD
      lcd.clear();
      displayLine(0, "====================");
      displayLineCentered(1, "Manual Mode");
      displayLineCentered(2, "Rear Opening");
      displayLine(3, "====================");

      // Ενεργοποίηση message timer
      modeMessageActive = true;
      modeMessageStartTime = myTime;
    }
  }

  else if (cloudCloseRearCurtains) {

    closeRearCurtain();

    if (!lastRearClose) {

      // Μήνυμα στην LCD
      lcd.clear();
      displayLine(0, "====================");
      displayLineCentered(1, "Manual Mode");
      displayLineCentered(2, "Rear Closing");
      displayLine(3, "====================");

      // Ενεργοποίηση message timer
      modeMessageActive = true;
      modeMessageStartTime = myTime;
    }
  }

  else {
    stopRearCurtain();
  }

  // ================= RIGHT CURTAIN =================
  if (cloudOpenRightCurtains) {

    openRightCurtain();

    if (!lastRightOpen) {

      // Μήνυμα στην LCD
      lcd.clear();
      displayLine(0, "====================");
      displayLineCentered(1, "Manual Mode");
      displayLineCentered(2, "Right Opening");
      displayLine(3, "====================");

      // Ενεργοποίηση message timer
      modeMessageActive = true;
      modeMessageStartTime = myTime;
    }
  }

  else if (cloudCloseRightCurtains) {

    closeRightCurtain();

    if (!lastRightClose) {

      // Μήνυμα στην LCD
      lcd.clear();
      displayLine(0, "====================");
      displayLineCentered(1, "Manual Mode");
      displayLineCentered(2, "Right Closing");
      displayLine(3, "====================");

      // Ενεργοποίηση message timer
      modeMessageActive = true;
      modeMessageStartTime = myTime;
    }
  }

  else {
    stopRightCurtain();
  }

  // ================= LEFT CURTAIN =================
  if (cloudOpenLeftCurtains) {

    openLeftCurtain();

    if (!lastLeftOpen) {

      // Μήνυμα στην LCD
      lcd.clear();
      displayLine(0, "====================");
      displayLineCentered(1, "Manual Mode");
      displayLineCentered(2, "Left Opening");
      displayLine(3, "====================");

      // Ενεργοποίηση message timer
      modeMessageActive = true;
      modeMessageStartTime = myTime;
    }
  }

  else if (cloudCloseLeftCurtains) {

    closeLeftCurtain();

    if (!lastLeftClose) {

      // Μήνυμα στην LCD
      lcd.clear();
      displayLine(0, "====================");
      displayLineCentered(1, "Manual Mode");
      displayLineCentered(2, "Left Closing");
      displayLine(3, "====================");

      // Ενεργοποίηση message timer
      modeMessageActive = true;
      modeMessageStartTime = myTime;
    }
  }

  else {
    stopLeftCurtain();
  }

  // ================= UPDATE STATES =================
  lastFrontOpen  = cloudOpenFrontCurtains;
  lastFrontClose = cloudCloseFrontCurtains;

  lastRearOpen  = cloudOpenRearCurtains;
  lastRearClose = cloudCloseRearCurtains;

  lastRightOpen  = cloudOpenRightCurtains;
  lastRightClose = cloudCloseRightCurtains;

  lastLeftOpen  = cloudOpenLeftCurtains;
  lastLeftClose = cloudCloseLeftCurtains;
}

// =================================================
// ΧΕΙΡΟΚΙΝΗΤΟΣ ΕΛΕΓΧΟΣ ΚΟΥΡΤΙΝΩΝ (CLOUD - MOMENTARY)
// =================================================
// Διαχειρίζεται τις κουρτίνες μέσω Arduino Cloud.
//
// Λειτουργία:
// - Momentary control:
//    → Όσο το κουμπί είναι πατημένο → οι κουρτίνες κινούνται
//    → Όταν το κουμπί αφήνεται → οι κουρτίνες σταματούν
//
// - Υποστηρίζει:
//    → Global Open  (άνοιγμα όλων)
//    → Global Close (κλείσιμο όλων)
//
// Επιπλέον:
// - Εμφανίζει μήνυμα στην LCD ΜΟΝΟ όταν ξεκινά η κίνηση
// - Αποφεύγει flickering με χρήση lastState
//
// Προσοχή:
// - Δεν χρησιμοποιείται curtainState (δεν είναι auto mode)
// - Η κίνηση είναι άμεση και ελέγχεται από το χρήστη
//
// Χρησιμοποιεί:
// - cloudOpenCurtains  → κουμπί ανοίγματος
// - cloudCloseCurtains → κουμπί κλεισίματος
// =================================================
void controlCloudCurtains() {

  // Αποθήκευση προηγούμενων καταστάσεων (για αποφυγή LCD spam)
  static bool lastOpenState = false;
  static bool lastCloseState = false;

  // ================= MANUAL MODE =================
  // Εκτελείται μόνο όταν είναι ενεργό το manual mode
  if (cloudManualCurtains) {

    // ================= GLOBAL OPEN =================
    // Όσο το κουμπί είναι πατημένο → ανοίγουν
    if (cloudOpenCurtains) {
  
      openCurtains();
  
      // ================= MESSAGE ON START =================
      // Εμφάνιση μηνύματος ΜΟΝΟ όταν ξεκινά η κίνηση
      if (!lastOpenState) {
  
        // Μήνυμα στην LCD
        lcd.clear();
        displayLine(0, "====================");
        displayLineCentered(1, "Manual Mode");
        displayLineCentered(2, "Curtains Opening");
        displayLine(3, "====================");
  
        // Ενεργοποίηση message timer
        modeMessageActive = true;
        modeMessageStartTime = myTime;
      }
    }
  
    // ================= GLOBAL CLOSE =================
    // Όσο το κουμπί είναι πατημένο → κλείνουν
    else if (cloudCloseCurtains) {
  
      closeCurtains();
  
      // ================= MESSAGE ON START =================
      if (!lastCloseState) {
  
        // Μήνυμα στην LCD
        lcd.clear();
        displayLine(0, "====================");
        displayLineCentered(1, "Manual Mode");
        displayLineCentered(2, "Curtains Closing");
        displayLine(3, "====================");
  
        // Ενεργοποίηση message timer
        modeMessageActive = true;
        modeMessageStartTime = myTime;
      }
    }
  
    // ================= INDIVIDUAL CONTROL =================
    // Έλεγχος κάθε κουρτίνας ξεχωριστά
    else {
  
      controlCloudCurtainsIndividual();
    }
  
    // ================= UPDATE STATES =================
    // Αποθήκευση καταστάσεων για το επόμενο loop
    lastOpenState = cloudOpenCurtains;
    lastCloseState = cloudCloseCurtains;
  }
}

// =================================================
// ΧΕΙΡΟΚΙΝΗΤΟΣ ΕΛΕΓΧΟΣ ΦΩΤΙΣΜΟΥ (CLOUD)
// =================================================
// Διαχειρίζεται το LED φωτισμού μέσω Arduino Cloud.
//
// Λειτουργία:
// - Ενεργοποιείται μόνο όταν το manual mode είναι ON
// - Το LIGHT λειτουργεί σε momentary mode:
//    → ON όσο το κουμπί είναι πατημένο
//    → OFF όταν αφήνεται
//
// Επιπλέον:
// - Εμφανίζει μήνυμα στην LCD ΜΟΝΟ όταν αλλάζει κατάσταση
// - Αποφεύγει flickering με χρήση lastState
//
// Χρησιμοποιεί:
// - cloudManualLight → ενεργοποίηση manual control
// - cloudLightState  → κατάσταση κουμπιού (pressed / released)
// =================================================
void controlCloudLight() {

  // Αποθήκευση προηγούμενης κατάστασης (για αποφυγή LCD spam)
  static bool lastLightState = false;

  // ================= MANUAL MODE =================
  // Εκτελείται μόνο όταν είναι ενεργό το manual mode
  if (cloudManualLight) {

    // ================= CONTINUOUS CONTROL =================
    // Momentary λειτουργία:
    // - HIGH όταν πατημένο
    // - LOW όταν ελεύθερο
    digitalWrite(lightPin, cloudLightState ? HIGH : LOW);
    lightState = cloudLightState;

    // ================= MESSAGE ON CHANGE =================
    // Εμφάνιση μηνύματος μόνο όταν αλλάζει η κατάσταση

    // LIGHT ON
    if (cloudLightState && !lastLightState) {

      // Μήνυμα στην LCD
      lcd.clear();
      displayLine(0, "====================");
      displayLineCentered(1, "Manual Mode");
      displayLineCentered(2, "Light ON");
      displayLine(3, "====================");

      // Ενεργοποίηση message timer
      modeMessageActive = true;
      modeMessageStartTime = myTime;
    }

    // LIGHT OFF
    else if (!cloudLightState && lastLightState) {

      // Μήνυμα στην LCD
      lcd.clear();
      displayLine(0, "====================");
      displayLineCentered(1, "Manual Mode");
      displayLineCentered(2, "Light OFF");
      displayLine(3, "====================");

      // Ενεργοποίηση message timer
      modeMessageActive = true;
      modeMessageStartTime = myTime;
    }

    // ================= UPDATE STATE =================
    // Αποθήκευση κατάστασης για επόμενο loop
    lastLightState = cloudLightState;
  }
}

// =================================================
// ΧΕΙΡΟΚΙΝΗΤΟΣ ΕΛΕΓΧΟΣ ΑΝΕΜΙΣΤΗΡΑ (CLOUD)
// =================================================
// Διαχειρίζεται τον ανεμιστήρα μέσω Arduino Cloud.
//
// Λειτουργία:
// - Ενεργοποιείται μόνο όταν το manual mode είναι ON
// - Ο ανεμιστήρας λειτουργεί σε momentary mode:
//    → ON όσο το κουμπί είναι πατημένο
//    → OFF όταν αφήνεται
//
// Επιπλέον:
// - Εμφανίζει μήνυμα στην LCD ΜΟΝΟ όταν αλλάζει κατάσταση
// - Αποφεύγει flickering με χρήση lastState
//
// Χρησιμοποιεί:
// - cloudManualFan → ενεργοποίηση manual control
// - cloudFanState  → κατάσταση κουμπιού (pressed / released)
// =================================================
void controlCloudFan() {

  // Αποθήκευση προηγούμενης κατάστασης (για αποφυγή LCD spam)
  static bool lastFanState = false;

  // ================= MANUAL MODE =================
  // Εκτελείται μόνο όταν είναι ενεργό το manual mode
  if (cloudManualFan) {

    // ================= CONTINUOUS CONTROL =================
    // Momentary λειτουργία:
    // - HIGH όταν το κουμπί είναι πατημένο
    // - LOW όταν το κουμπί αφήνεται
    digitalWrite(fanPin, cloudFanState ? HIGH : LOW);
    fanState = cloudFanState;

    // ================= MESSAGE ON CHANGE =================
    // Εμφάνιση μηνύματος μόνο όταν αλλάζει η κατάσταση

    // FAN ON
    if (cloudFanState && !lastFanState) {

      // Μήνυμα στην LCD
      lcd.clear();
      displayLine(0, "====================");
      displayLineCentered(1, "Manual Mode");
      displayLineCentered(2, "Fan ON");
      displayLine(3, "====================");

      // Ενεργοποίηση message timer
      modeMessageActive = true;
      modeMessageStartTime = myTime;
    }

    // FAN OFF
    else if (!cloudFanState && lastFanState) {

      // Μήνυμα στην LCD
      lcd.clear();
      displayLine(0, "====================");
      displayLineCentered(1, "Manual Mode");
      displayLineCentered(2, "Fan OFF");
      displayLine(3, "====================");

      // Ενεργοποίηση message timer
      modeMessageActive = true;
      modeMessageStartTime = myTime;
    }

    // ================= UPDATE STATE =================
    // Αποθήκευση κατάστασης για το επόμενο loop
    lastFanState = cloudFanState;
  }
}

// =================================================
// ΔΙΑΧΕΙΡΙΣΗ ΔΙΑΚΟΠΤΗ CLOUD MODE
// =================================================
// Διαβάζει τον φυσικό διακόπτη και καθορίζει
// αν το σύστημα θα λειτουργεί σε Cloud mode
// ή σε Offline mode.
//
// Λειτουργία:
// - LOW  → Cloud ενεργό
// - HIGH → Offline mode
//
// Ενημερώνει:
// - LCD με την τρέχουσα κατάσταση
// - offlineMode flag
// =================================================
void readCloudSwitch() {

  // Ανάγνωση κατάστασης διακόπτη
  bool currentSwitchState = digitalRead(switchCloudPin);

  // Καθαρισμός οθόνης
  lcd.clear();
  displayLine(0, "====================");

  // ================= CLOUD MODE =================
  // Αν ο διακόπτης είναι LOW → Cloud ενεργό
  if (!currentSwitchState) {
    displayLineCentered(1, "Cloud Mode ON");
    
    // Θέτουμε κατάσταση CONNECTING
    cloudState = CLOUD_CONNECTING;
    
    // Ενημέρωση flag λειτουργίας
    offlineMode = false;
  } 
  
  // ================= OFFLINE MODE =================
  // Αν ο διακόπτης είναι HIGH → Offline mode
  else {
    displayLineCentered(1, "Cloud Mode OFF");
    
    // Θέτουμε κατάσταση CLOUD MANUAL OFFLINE
    cloudState = CLOUD_MANUAL_OFFLINE;
    
    // Ενημέρωση flag λειτουργίας
    offlineMode = true;
  }

  // Γραμμές διακόσμησης
  displayLine(2, "                    ");
  displayLine(3, "====================");
}
