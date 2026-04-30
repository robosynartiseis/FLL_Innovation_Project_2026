// =================================================
// PRINT VALUE WITH UNIT (LCD HELPER)
// =================================================
// Εκτυπώνει μία ακέραια τιμή μαζί με τη μονάδα μέτρησης
// στην LCD, χωρίς κενό μεταξύ τιμής και unit.
//
// Λειτουργία:
// - Συνδυάζει value + unit σε ένα string (π.χ. "25C")
// - Εκτυπώνει το αποτέλεσμα στην LCD
// - Συμπληρώνει κενά (spaces) δεξιά ώστε να καλυφθεί
//   πλήρως το διαθέσιμο πλάτος (totalWidth)
//
// Χρήσιμο για:
// - Σταθερό formatting στην οθόνη
// - Αποφυγή "υπολειμμάτων" από προηγούμενες τιμές
//
// Παράμετροι:
// - value      → αριθμητική τιμή (int)
// - unit       → μονάδα (π.χ. "C", "%", "hPa")
// - totalWidth → συνολικό πλάτος πεδίου στην LCD
// =================================================
void printValueWithUnit(int value, const char* unit, int totalWidth) {

  // Buffer για αποθήκευση formatted string
  char buffer[16];

  // Δημιουργία string: value + unit (χωρίς κενό)
  sprintf(buffer, "%d%s", value, unit);

  // Εκτύπωση στην LCD
  lcd.print(buffer);

  // ================= RIGHT CLEAN =================
  // Συμπλήρωση με κενά ώστε να "σβηστούν"
  // παλιά δεδομένα από μεγαλύτερες τιμές
  int len = strlen(buffer);

  for (int i = len; i < totalWidth; i++) {
    lcd.print(" ");
  }
}

// =================================================
// PRINT FLOAT WITH UNIT (LCD HELPER)
// =================================================
// Εκτυπώνει μία δεκαδική τιμή (float) μαζί με τη μονάδα
// μέτρησης στην LCD, χωρίς κενό μεταξύ τιμής και unit.
//
// Λειτουργία:
// - Δημιουργεί δυναμικά format string (π.χ. "%.1f")
// - Συνδυάζει value + unit (π.χ. "25.3C")
// - Εκτυπώνει το αποτέλεσμα στην LCD
// - Συμπληρώνει κενά (spaces) δεξιά ώστε να καλυφθεί
//   πλήρως το διαθέσιμο πλάτος (totalWidth)
//
// Χρήσιμο για:
// - Σταθερό formatting δεκαδικών τιμών
// - Αποφυγή "υπολειμμάτων" στην οθόνη
//
// Παράμετροι:
// - value      → αριθμητική τιμή (float)
// - unit       → μονάδα (π.χ. "C", "%", "hPa")
// - totalWidth → συνολικό πλάτος πεδίου στην LCD
// - decimals   → αριθμός δεκαδικών ψηφίων
// =================================================
void printFloatWithUnit(float value, const char* unit, int totalWidth, int decimals) {

  // Buffer για το τελικό string (value + unit)
  char buffer[32];

  // ================= FORMAT STRING =================
  // Δημιουργία format π.χ. "%.1f%s"
  char format[10];
  sprintf(format, "%%.%df%%s", decimals);

  // ================= FORMAT VALUE =================
  // Συνδυασμός float + unit (χωρίς κενό)
  sprintf(buffer, format, value, unit);

  // Εκτύπωση στην LCD
  lcd.print(buffer);

  // ================= RIGHT CLEAN =================
  // Συμπλήρωση με κενά για καθαρισμό παλιών χαρακτήρων
  int len = strlen(buffer);

  for (int i = len; i < totalWidth; i++) {
    lcd.print(" ");
  }
}

// =================================================
// ΕΜΦΑΝΙΣΗ ΚΕΙΜΕΝΟΥ ΣΕ ΣΥΓΚΕΚΡΙΜΕΝΗ ΓΡΑΜΜΗ LCD
// =================================================
// Εμφανίζει κείμενο σε συγκεκριμένη γραμμή της LCD
//
// Παράμετροι:
// - row  : γραμμή (0–3 για LCD 20x4)
// - text : το κείμενο που θα εμφανιστεί
//
// Κάνει:
// - καθαρισμό της γραμμής
// - εμφάνιση νέου κειμένου από την αρχή
//
// Χρησιμοποιείται για καθαρή και σωστή εμφάνιση δεδομένων
// =================================================
void displayLine(int row, String text) {

  // Πηγαίνουμε στην αρχή της γραμμής
  lcd.setCursor(0, row);

  // Καθαρίζουμε όλη τη γραμμή (20 χαρακτήρες)
  lcd.print("                    ");

  // Ξαναπάμε στην αρχή
  lcd.setCursor(0, row);

  // Εμφανίζουμε το νέο κείμενο
  lcd.print(text);
}

// =================================================
// DISPLAY LINE CENTERED (LCD HELPER)
// =================================================
// Εκτυπώνει ένα κείμενο κεντραρισμένο σε συγκεκριμένη
// γραμμή της LCD.
//
// Λειτουργία:
// - Αν το κείμενο είναι μεγαλύτερο από το πλάτος της LCD,
//   κόβεται ώστε να χωρέσει
// - Υπολογίζει τη θέση έναρξης για κεντράρισμα
// - Καθαρίζει πρώτα όλη τη γραμμή
// - Εκτυπώνει το κείμενο στη σωστή θέση
//
// Χρήσιμο για:
// - Τίτλους
// - Μηνύματα συστήματος
// - Alerts (π.χ. "Fan ON", "Manual Mode")
//
// Παράμετροι:
// - row  → γραμμή LCD (0–3 για 20x4)
// - text → κείμενο προς εμφάνιση
// =================================================
void displayLineCentered(int row, String text) {

  // Πλάτος LCD (20 χαρακτήρες)
  const int lcdWidth = 20;

  // ================= TRIM TEXT =================
  // Αν το κείμενο είναι μεγαλύτερο από τη γραμμή,
  // κόβεται για αποφυγή overflow
  if (text.length() > lcdWidth) {
    text = text.substring(0, lcdWidth);
  }

  // ================= CALCULATE POSITION =================
  // Υπολογισμός θέσης έναρξης για κεντράρισμα
  int startPos = (lcdWidth - text.length()) / 2;

  // ================= CLEAR LINE =================
  // Καθαρισμός ολόκληρης γραμμής
  lcd.setCursor(0, row);
  lcd.print("                    "); // 20 spaces

  // ================= PRINT TEXT =================
  // Εκτύπωση κεντραρισμένου κειμένου
  lcd.setCursor(startPos, row);
  lcd.print(text);
}

// =================================================
// ΟΘΟΝΗ ΕΚΚΙΝΗΣΗΣ
// =================================================
// Εμφανίζει βασικές πληροφορίες στην LCD:
// - Όνομα ομάδας
// - Διαγωνισμό
// - Όνομα ρομπότ
// - Υπογραφή
//
// Χρησιμοποιείται στο setup() για αρχική εικόνα
// =================================================
void displayIntro() {

  // Όνομα ομάδας
  displayLineCentered(0, "Robo+art(iseis)");

  // Διαγωνισμός
  displayLineCentered(1, "FLL UNEARTHED 2026");

  // Όνομα ρομπότ
  displayLineCentered(2, "KALIPSO");

  // Υπογραφή
  displayLineCentered(3, "Power By Anosis!");
}

// =================================================
// ΕΜΦΑΝΙΣΗ ΤΙΜΩΝ ΦΩΤΟΣ ΣΤΗΝ LCD
// =================================================
// Εμφανίζει ποσοστά φωτός από 4 κατευθύνσεις
//
// Κάνει:
// - διαβάζει fake ή πραγματικά δεδομένα
// - μετατρέπει τιμές σε %
// - εμφανίζει σε μορφή T B R L στην LCD
//
// Δεν εμφανίζει αν υπάρχει άλλο ενεργό μήνυμα
// =================================================
void displayLightData() {

  // Έλεγχος χρόνου (millis-based)
  if (myTime - displayLightSensorsTime >= displayLightSensorsInterval) {

    // ===== ΕΠΙΛΟΓΗ DATA MODE =====
    if (useFakeDataLight) {

      // Fake δεδομένα
      updateFakeLight();

      avtPerc = fakeTop;
      avdPerc = fakeBottom;
      avrPerc = fakeRight;
      avlPerc = fakeLeft;

    } else {

      // Μετατροπή σε ποσοστό (0–100%)
      avtPerc = map(avt, 0, 1023, 0, 100);
      avdPerc = map(avd, 0, 1023, 0, 100);
      avrPerc = map(avr, 0, 1023, 0, 100);
      avlPerc = map(avl, 0, 1023, 0, 100);
    }

    // ===== ΕΜΦΑΝΙΣΗ LCD =====
    // Εμφανίζει μόνο αν δεν υπάρχει άλλο active display
    if (!aiDisplayActive && !modeMessageActive) {

      lcd.setCursor(0, 0);
      
      lcd.print("T:");
      lcd.print(formatInt(avtPerc, 3));

      lcd.print("B:");
      lcd.print(formatInt(avdPerc, 3));

      lcd.print("R:");
      lcd.print(formatInt(avrPerc, 3));

      lcd.print("L:");
      lcd.print(formatInt(avlPerc, 3));
    }

    // Reset timer
    displayLightSensorsTime = myTime;
  }
}

// =================================================
// ΕΜΦΑΝΙΣΗ ΠΕΡΙΒΑΛΛΟΝΤΙΚΩΝ ΔΕΔΟΜΕΝΩΝ (LCD)
// =================================================
// Εμφανίζει θερμοκρασία, πίεση, υγρασία,
// υψόμετρο, βροχή και φως σε split layout
//
// Κάνει:
// - ενημέρωση ανά χρονικό διάστημα (millis)
// - εμφανίζει δεδομένα με μονάδες
// - χωρίζει την οθόνη σε 2 στήλες με "|"
//
// Δεν εμφανίζει αν υπάρχει άλλο ενεργό μήνυμα
// =================================================
void displayEnvironmentData() {

  // Έλεγχος χρόνου (millis-based)
  if (myTime - displaySensorsTime >= displaySensorsInterval) {

    // Εμφάνιση μόνο αν δεν υπάρχει άλλο active display
    if (!aiDisplayActive && !modeMessageActive) {

      // ================= LINE 1 =================
      // Θερμοκρασία | Πίεση
      lcd.setCursor(0, 1);
      lcd.print("T:");
      printFloatWithUnit(temperature, "C", 6, 1);

      lcd.setCursor(8, 1);
      lcd.print("|");

      lcd.setCursor(9, 1);
      lcd.print("P:");
      printFloatWithUnit(pressure, "hPa", 9, 1);

      // ================= LINE 2 =================
      // Υγρασία | Υψόμετρο
      lcd.setCursor(0, 2);
      lcd.print("H:");
      printFloatWithUnit(humidity, "%", 5, 1);

      lcd.setCursor(8, 2);
      lcd.print("|");

      lcd.setCursor(9, 2);
      lcd.print("A:");
      printFloatWithUnit(altitude, "m", 6, 1);

      // ================= LINE 3 =================
      // Βροχή | Φως
      lcd.setCursor(0, 3);
      lcd.print("R:");
      printValueWithUnit(rain, "%", 4);

      lcd.setCursor(8, 3);
      lcd.print("|");

      lcd.setCursor(9, 3);
      lcd.print("L:");
      printValueWithUnit(light, "%", 4);
    }

    // Reset timer
    displaySensorsTime = myTime;
  }
}

// =================================================
// MESSAGE TIMER CONTROL
// =================================================
// Διαχειρίζεται τη διάρκεια εμφάνισης μηνυμάτων στην LCD
//
// Κάνει:
// - ελέγχει αν υπάρχει ενεργό μήνυμα
// - μετά από συγκεκριμένο χρόνο το κλείνει
// - καθαρίζει την οθόνη
//
// Χρησιμοποιείται για προσωρινά μηνύματα
// =================================================
void updateDisplayMessages() {

  // Αν υπάρχει ενεργό μήνυμα
  if (modeMessageActive) {

    // Έλεγχος αν πέρασε ο χρόνος εμφάνισης
    if (myTime - modeMessageStartTime >= modeMessageStartInterval) {

      // Απενεργοποίηση μηνύματος
      modeMessageActive = false;

      // Καθαρισμός οθόνης
      lcd.clear();
    }
  }
}