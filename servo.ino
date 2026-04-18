// =================================================
// ΕΛΕΓΧΟΣ SERVO ΧΩΡΙΣ ΒΙΒΛΙΟΘΗΚΗ (PWM MANUAL)
// =================================================
// Στέλνει PWM σήμα σε servo χωρίς χρήση Servo library
//
// Παράμετροι:
// - pin   : pin σύνδεσης servo
// - angle : γωνία (0°–180°)
//
// Πώς δουλεύει:
// - μετατρέπει τη γωνία σε pulse width (500–2500 μs)
// - στέλνει HIGH για τη διάρκεια του pulse
// - μετά LOW για να συμπληρωθεί περίοδος 20ms (50Hz)
//
// Σημαντικό:
// - Μπλοκάρει με delayMicroseconds()
// - Κατάλληλο για απλό έλεγχο (όχι multitasking)
// =================================================
void controlServoPWM(int pin, int angle) {

  // Μετατροπή γωνίας σε pulse width (μs)
  int pulse = map(angle, 0, 180, 500, 2500);

  // Έναρξη παλμού (HIGH)
  digitalWrite(pin, HIGH);

  // Διάρκεια παλμού
  delayMicroseconds(pulse);

  // Τέλος παλμού (LOW)
  digitalWrite(pin, LOW);

  // Αναμονή μέχρι να συμπληρωθεί περίοδος 20ms
  delayMicroseconds(servoPeriodDelay - pulse);   // ~20ms
}

// =================================================
// SERVO TEST – ΚΙΝΗΣΗ ΑΠΟ ΟΡΙΟ ΣΕ ΟΡΙΟ
// =================================================
// Ελέγχει αν τα servo λειτουργούν σωστά
//
// Κάνει:
// - κινεί κάθε servo από min → max
// - μετά από max → min
// - στο τέλος επιστρέφει στη μέση θέση
//
// Χρησιμοποιείται στο setup() για έλεγχο hardware
// =================================================
void testServoSweep() {

  // ===== ΟΡΙΖΟΝΤΙΟ SERVO =====
  // Κίνηση από ελάχιστο → μέγιστο
  for (int pos = servoHoriLimitLow; pos <= servoHoriLimitHigh; pos++) {

    controlServoPWM(servoHoriPin, pos);
    delay(servoTestStepDelay); // ομαλή κίνηση
  }

  delay(servoTestDirectionDelay);

  // Κίνηση από μέγιστο → ελάχιστο
  for (int pos = servoHoriLimitHigh; pos >= servoHoriLimitLow; pos--) {

    controlServoPWM(servoHoriPin, pos);
    delay(servoTestStepDelay); // ομαλή κίνηση
  }

  // Επιστροφή στη μέση θέση
  delay(servoTestResetDelay);
  controlServoPWM(servoHoriPin, servoHoriPos);

  // ===== ΚΑΘΕΤΟ SERVO =====
  // Κίνηση από ελάχιστο → μέγιστο
  for (int pos = servoVertLimitLow; pos <= servoVertLimitHigh; pos++) {

    controlServoPWM(servoVertPin, pos);
    delay(servoTestStepDelay); // ομαλή κίνηση
  }

  delay(servoTestDirectionDelay);

  // Κίνηση από μέγιστο → ελάχιστο
  for (int pos = servoVertLimitHigh; pos >= servoVertLimitLow; pos--) {

    controlServoPWM(servoVertPin, pos);
    delay(servoTestStepDelay); // ομαλή κίνηση
  }

  // Επιστροφή στη μέση θέση
  delay(servoTestResetDelay);
  controlServoPWM(servoVertPin, servoVertPos);
}