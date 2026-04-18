// =================================================
// ΕΝΗΜΕΡΩΣΗ LED ΚΑΤΑΣΤΑΣΗΣ CLOUD
// =================================================
// Εμφανίζει την κατάσταση του Cloud μέσω 2 LED
//
// Κάνει:
// - πράσινο LED → κατάσταση λειτουργίας
// - κόκκινο LED → σφάλμα / offline
// - αναβόσβημα με millis (non-blocking)
//
// Χρησιμοποιείται για οπτικό feedback κατάστασης
// =================================================
void updateCloudStatusLEDs() {

  switch (cloudState) {

    // ===== ONLINE =====
    // Πράσινο ON → όλα OK
    case CLOUD_ONLINE:
      digitalWrite(cloudLedGreenPin, HIGH);
      digitalWrite(cloudLedRedPin, LOW);
      break;

    // ===== OFFLINE =====
    // Κόκκινο ON → δεν υπάρχει σύνδεση
    case CLOUD_OFFLINE:
      digitalWrite(cloudLedGreenPin, LOW);
      digitalWrite(cloudLedRedPin, HIGH);
      break;

    // ===== CONNECTING =====
    // Πράσινο blink → προσπαθεί να συνδεθεί
    case CLOUD_CONNECTING:
      digitalWrite(cloudLedGreenPin, millis() % blinkInterval < blinkOnTime);
      digitalWrite(cloudLedRedPin, LOW);
      break;

    // ===== MANUAL OFFLINE =====
    // Κόκκινο blink → Cloud disabled
    case CLOUD_MANUAL_OFFLINE:
      digitalWrite(cloudLedGreenPin, LOW);
      digitalWrite(cloudLedRedPin, millis() % blinkInterval < blinkOnTime);
      break;
 }
}

// =================================================
// ΚΙΝΗΣΗ ΜΠΡΟΣΤΑ (FORWARD)
// =================================================
// Κινεί το ρομπότ προς τα εμπρός ενεργοποιώντας
// και τα 4 μοτέρ στην ίδια κατεύθυνση.
//
// Λειτουργία:
// - Ορίζει ίδια ταχύτητα σε όλα τα μοτέρ (PWM)
// - Ρυθμίζει τα pins ώστε όλα να περιστρέφονται forward
//
// Χρησιμοποιείται για:
// - Αυτόνομη κίνηση
// - Χειροκίνητο έλεγχο από Cloud
// =================================================
void moveForward() {

  // ================= SPEED CONTROL =================
  // Σταθερή ταχύτητα σε όλους τους κινητήρες
  analogWrite(ena_1, 100);
  analogWrite(enb_1, 100);
  analogWrite(ena_2, 100);
  analogWrite(enb_2, 100);

  // ================= MOTOR DIRECTION =================
  // Όλα τα μοτέρ κινούνται προς τα εμπρός

  // Motor 1
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);

  // Motor 2
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);

  // Motor 3
  digitalWrite(motor3pin1, HIGH);
  digitalWrite(motor3pin2, LOW);

  // Motor 4
  digitalWrite(motor4pin1, HIGH);
  digitalWrite(motor4pin2, LOW);
}

// =================================================
// ΚΙΝΗΣΗ ΠΙΣΩ (BACKWARD)
// =================================================
// Κινεί το ρομπότ προς τα πίσω αντιστρέφοντας
// την κατεύθυνση όλων των μοτέρ.
//
// Λειτουργία:
// - Ορίζει ίδια ταχύτητα σε όλα τα μοτέρ (PWM)
// - Αντιστρέφει την πολικότητα για reverse κίνηση
//
// Χρησιμοποιείται για:
// - Αυτόνομη κίνηση
// - Χειροκίνητο έλεγχο από Cloud
// =================================================
void moveBackward() {

  // ================= SPEED CONTROL =================
  // Σταθερή ταχύτητα σε όλους τους κινητήρες
  analogWrite(ena_1, 100);
  analogWrite(enb_1, 100);
  analogWrite(ena_2, 100);
  analogWrite(enb_2, 100);

  // ================= MOTOR DIRECTION =================
  // Όλα τα μοτέρ κινούνται προς τα πίσω

  // Motor 1
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);

  // Motor 2
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);

  // Motor 3
  digitalWrite(motor3pin1, LOW);
  digitalWrite(motor3pin2, HIGH);

  // Motor 4
  digitalWrite(motor4pin1, LOW);
  digitalWrite(motor4pin2, HIGH);
}

// =================================================
// ΣΤΡΟΦΗ ΔΕΞΙΑ (RIGHT)
// =================================================
// Περιστρέφει το ρομπότ προς τα δεξιά
// χρησιμοποιώντας αντίθετη φορά στους τροχούς.
//
// Λειτουργία:
// - Όλα τα μοτέρ ενεργά με ίδια ταχύτητα
// - Οι δύο πλευρές περιστρέφονται αντίθετα
//
// Χρησιμοποιείται για:
// - Επιτόπια στροφή (pivot)
// - Διόρθωση κατεύθυνσης
// =================================================
void turnRight() {

  // ================= SPEED CONTROL =================
  // Σταθερή ταχύτητα σε όλους τους κινητήρες
  analogWrite(ena_1, 100);
  analogWrite(enb_1, 100);
  analogWrite(ena_2, 100);
  analogWrite(enb_2, 100);

  // ================= MOTOR DIRECTION =================
  // Αντίθετη περιστροφή για δεξιά στροφή

  // Motor 1
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);

  // Motor 2
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);

  // Motor 3
  digitalWrite(motor3pin1, HIGH);
  digitalWrite(motor3pin2, LOW);

  // Motor 4
  digitalWrite(motor4pin1, LOW);
  digitalWrite(motor4pin2, HIGH);
}

// =================================================
// ΣΤΡΟΦΗ ΑΡΙΣΤΕΡΑ (LEFT)
// =================================================
// Περιστρέφει το ρομπότ προς τα αριστερά
// χρησιμοποιώντας αντίθετη φορά στους τροχούς.
//
// Λειτουργία:
// - Όλα τα μοτέρ ενεργά με ίδια ταχύτητα
// - Οι δύο πλευρές περιστρέφονται αντίθετα
//
// Χρησιμοποιείται για:
// - Επιτόπια στροφή (pivot)
// - Διόρθωση κατεύθυνσης
// =================================================
void turnLeft() {

  // ================= SPEED CONTROL =================
  // Σταθερή ταχύτητα σε όλους τους κινητήρες
  analogWrite(ena_1, 100);
  analogWrite(enb_1, 100);
  analogWrite(ena_2, 100);
  analogWrite(enb_2, 100);

  // ================= MOTOR DIRECTION =================
  // Αντίθετη περιστροφή για αριστερή στροφή

  // Motor 1
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);

  // Motor 2
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);

  // Motor 3
  digitalWrite(motor3pin1, LOW);
  digitalWrite(motor3pin2, HIGH);

  // Motor 4
  digitalWrite(motor4pin1, HIGH);
  digitalWrite(motor4pin2, LOW);
}

// =================================================
// ΣΤΑΜΑΤΗΜΑ ΟΛΩΝ ΤΩΝ ΚΙΝΗΤΗΡΩΝ
// =================================================
// Διακόπτει πλήρως την κίνηση του ρομπότ.
//
// Λειτουργία:
// - Μηδενίζει την ταχύτητα (PWM) σε όλους τους drivers
// - Θέτει όλα τα control pins σε LOW
//
// Αποτέλεσμα:
// - Τα μοτέρ δεν τροφοδοτούνται
// - Το ρομπότ σταματά άμεσα και με ασφάλεια
//
// Χρησιμοποιείται:
// - Όταν δεν υπάρχει εντολή κίνησης
// - Σε αλλαγή κατεύθυνσης
// - Σε emergency stop / idle κατάσταση
// =================================================
void stopMotors() {

  // ================= SPEED CONTROL =================
  // Μηδενισμός ταχύτητας σε όλους τους οδηγούς (enable pins)
  analogWrite(ena_1, 0);
  analogWrite(enb_1, 0);
  analogWrite(ena_2, 0);
  analogWrite(enb_2, 0);

  // ================= MOTOR DIRECTION =================
  // Θέτουμε όλα τα pins σε LOW
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);

  digitalWrite(motor3pin1, LOW);
  digitalWrite(motor3pin2, LOW);

  digitalWrite(motor4pin1, LOW);
  digitalWrite(motor4pin2, LOW);
}

// =================================================
// ΚΙΝΗΣΗ ΡΟΜΠΟΤ ΒΑΣΕΙ ΦΩΤΟΣ
// =================================================
// Κατευθύνει το ρομπότ προς την πηγή φωτός
// χρησιμοποιώντας τις μετρήσεις από τους LDR.
//
// Λειτουργία:
// - Υπολογίζει διαφορές φωτός (κάθετα & οριζόντια)
// - Αν η διαφορά ξεπερνά το όριο (tol):
//    → κινείται προς την πλευρά με περισσότερο φως
//
// Περιλαμβάνει:
// - Κίνηση μπροστά / πίσω
// - Στροφή αριστερά / δεξιά
// - Μικρή καθυστέρηση για σταθεροποίηση
// =================================================
void controlMotorsByLight() {

  // Εκτέλεση κάθε συγκεκριμένο χρονικό διάστημα
  if (myTime - readMovingTime >= readMovingInterval) {

    // ================= ΚΑΘΕΤΗ ΚΙΝΗΣΗ =================
    // Έλεγχος διαφοράς φωτός πάνω / κάτω
    if (abs(dvert) > tol) {

      // true → περισσότερο φως μπροστά
      bool frontv = avt > avd;

      if (frontv) {
        moveForward();   // Κίνηση μπροστά
        delay(motionDelay);
        stopMotors();      // Σταμάτημα
      } else {
        moveBackward();  // Κίνηση πίσω
        delay(motionDelay);
        stopMotors();
      }
    }

    // ================= ΟΡΙΖΟΝΤΙΑ ΚΙΝΗΣΗ =================
    // Έλεγχος διαφοράς φωτός αριστερά / δεξιά
    if (abs(dhoriz) > tol) {

      // true → περισσότερο φως αριστερά
      bool leftv = avl > avr;

      if (leftv) {
        turnLeft();      // Στροφή αριστερά
        delay(motionDelay);
        stopMotors();
      } else {
        turnRight();     // Στροφή δεξιά
        delay(motionDelay);
        stopMotors();
      }
    }

    // Ενημέρωση timer
    readMovingTime = myTime;
  }
}

// =================================================
// ΑΝΟΙΓΜΑ ΜΠΡΟΣΤΙΝΗΣ ΚΟΥΡΤΙΝΑΣ
// =================================================
// Ενεργοποιεί το μοτέρ της μπροστινής κουρτίνας
// για άνοιγμα (κάθοδο).
//
// Λειτουργία:
// - Χρησιμοποιεί H-Bridge driver (2 pins)
// - LOW + HIGH → περιστροφή προς κατεύθυνση ανοίγματος
//
// Χρησιμοποιείται για:
// - Αυτόματο άνοιγμα (π.χ. βροχή)
// - Χειροκίνητο έλεγχο από Cloud
// =================================================
void openFrontCurtain() {

  // ================= MOTOR DIRECTION =================
  // Περιστροφή προς άνοιγμα κουρτίνας
  digitalWrite(frontCurtainPin1, LOW);
  digitalWrite(frontCurtainPin2, HIGH);

}

// =================================================
// ΑΝΟΙΓΜΑ ΠΙΣΩ ΚΟΥΡΤΙΝΑΣ
// =================================================
// Ενεργοποιεί το μοτέρ της πίσω κουρτίνας
// για άνοιγμα (κάθοδο).
//
// Λειτουργία:
// - Χρησιμοποιεί H-Bridge driver (2 pins)
// - LOW + HIGH → περιστροφή προς κατεύθυνση ανοίγματος
//
// Χρησιμοποιείται για:
// - Αυτόματο άνοιγμα (π.χ. βροχή)
// - Χειροκίνητο έλεγχο από Cloud
// =================================================
void openRearCurtain() {

  // ================= MOTOR DIRECTION =================
  // Περιστροφή προς άνοιγμα κουρτίνας
  digitalWrite(rearCurtainPin1, LOW);
  digitalWrite(rearCurtainPin2, HIGH);

}

// =================================================
// ΑΝΟΙΓΜΑ ΔΕΞΙΑΣ ΚΟΥΡΤΙΝΑΣ
// =================================================
// Ενεργοποιεί το μοτέρ της δεξιάς κουρτίνας
// για άνοιγμα.
//
// Λειτουργία:
// - Χρησιμοποιεί H-Bridge driver (2 pins)
// - LOW + HIGH → περιστροφή προς κατεύθυνση ανοίγματος
//
// Χρησιμοποιείται για:
// - Αυτόματο άνοιγμα
// - Χειροκίνητο έλεγχο από Cloud
// =================================================
void openRightCurtain() {

  // ================= MOTOR DIRECTION =================
  // Περιστροφή προς άνοιγμα κουρτίνας
  digitalWrite(rightCurtainPin1, LOW);
  digitalWrite(rightCurtainPin2, HIGH);

}

// =================================================
// ΑΝΟΙΓΜΑ ΑΡΙΣΤΕΡΗΣ ΚΟΥΡΤΙΝΑΣ
// =================================================
// Ενεργοποιεί το μοτέρ της αριστερής κουρτίνας
// για άνοιγμα.
//
// Λειτουργία:
// - Χρησιμοποιεί H-Bridge driver (2 pins)
// - LOW + HIGH → περιστροφή προς κατεύθυνση ανοίγματος
//
// Χρησιμοποιείται για:
// - Αυτόματο άνοιγμα
// - Χειροκίνητο έλεγχο από Cloud
// =================================================
void openLeftCurtain() {

  // ================= MOTOR DIRECTION =================
  // Περιστροφή προς άνοιγμα κουρτίνας
  digitalWrite(leftCurtainPin1, LOW);
  digitalWrite(leftCurtainPin2, HIGH);

}

// =================================================
// ΑΝΟΙΓΜΑ ΟΛΩΝ ΤΩΝ ΚΟΥΡΤΙΝΩΝ
// =================================================
// Ενεργοποιεί ταυτόχρονα όλα τα μοτέρ κουρτινών
// ώστε να ανοίξουν.
//
// Λειτουργία:
// - Καλεί τις επιμέρους συναρτήσεις για κάθε κουρτίνα
// - Εκτελεί άνοιγμα σε Front, Rear, Right, Left
//
// Χρησιμοποιείται για:
// - Αυτόματο άνοιγμα (π.χ. βροχή)
// - Global εντολή από Cloud
// =================================================
void openCurtains() {

  // Άνοιγμα όλων των κουρτινών
  openFrontCurtain();
  openRearCurtain();
  openRightCurtain();
  openLeftCurtain();

}

// =================================================
// ΚΛΕΙΣΙΜΟ ΜΠΡΟΣΤΙΝΗΣ ΚΟΥΡΤΙΝΑΣ
// =================================================
// Ενεργοποιεί το μοτέρ της μπροστινής κουρτίνας
// για κλείσιμο (άνοδο).
//
// Λειτουργία:
// - Χρησιμοποιεί H-Bridge driver (2 pins)
// - HIGH + LOW → περιστροφή προς κατεύθυνση κλεισίματος
//
// Χρησιμοποιείται για:
// - Αυτόματο κλείσιμο (π.χ. όταν σταματήσει η βροχή)
// - Χειροκίνητο έλεγχο από Cloud
// =================================================
void closeFrontCurtain() {

  // ================= MOTOR DIRECTION =================
  // Περιστροφή προς κλείσιμο κουρτίνας
  digitalWrite(frontCurtainPin1, HIGH);
  digitalWrite(frontCurtainPin2, LOW);

}

// =================================================
// ΚΛΕΙΣΙΜΟ ΠΙΣΩ ΚΟΥΡΤΙΝΑΣ
// =================================================
// Ενεργοποιεί το μοτέρ της πίσω κουρτίνας
// για κλείσιμο (άνοδο).
//
// Λειτουργία:
// - Χρησιμοποιεί H-Bridge driver (2 pins)
// - HIGH + LOW → περιστροφή προς κατεύθυνση κλεισίματος
//
// Χρησιμοποιείται για:
// - Αυτόματο κλείσιμο
// - Χειροκίνητο έλεγχο από Cloud
// =================================================
void closeRearCurtain() {

  // ================= MOTOR DIRECTION =================
  // Περιστροφή προς κλείσιμο κουρτίνας
  digitalWrite(rearCurtainPin1, HIGH);
  digitalWrite(rearCurtainPin2, LOW);

}

// =================================================
// ΚΛΕΙΣΙΜΟ ΔΕΞΙΑΣ ΚΟΥΡΤΙΝΑΣ
// =================================================
// Ενεργοποιεί το μοτέρ της δεξιάς κουρτίνας
// για κλείσιμο.
//
// Λειτουργία:
// - Χρησιμοποιεί H-Bridge driver (2 pins)
// - HIGH + LOW → περιστροφή προς κατεύθυνση κλεισίματος
//
// Χρησιμοποιείται για:
// - Αυτόματο κλείσιμο
// - Χειροκίνητο έλεγχο από Cloud
// =================================================
void closeRightCurtain() {

  // ================= MOTOR DIRECTION =================
  // Περιστροφή προς κλείσιμο κουρτίνας
  digitalWrite(rightCurtainPin1, HIGH);
  digitalWrite(rightCurtainPin2, LOW);

}

// =================================================
// ΚΛΕΙΣΙΜΟ ΑΡΙΣΤΕΡΗΣ ΚΟΥΡΤΙΝΑΣ
// =================================================
// Ενεργοποιεί το μοτέρ της αριστερής κουρτίνας
// για κλείσιμο.
//
// Λειτουργία:
// - Χρησιμοποιεί H-Bridge driver (2 pins)
// - HIGH + LOW → περιστροφή προς κατεύθυνση κλεισίματος
//
// Χρησιμοποιείται για:
// - Αυτόματο κλείσιμο
// - Χειροκίνητο έλεγχο από Cloud
// =================================================
void closeLeftCurtain() {

  // ================= MOTOR DIRECTION =================
  // Περιστροφή προς κλείσιμο κουρτίνας
  digitalWrite(leftCurtainPin1, HIGH);
  digitalWrite(leftCurtainPin2, LOW);

}

// =================================================
// ΚΛΕΙΣΙΜΟ ΟΛΩΝ ΤΩΝ ΚΟΥΡΤΙΝΩΝ
// =================================================
// Ενεργοποιεί ταυτόχρονα όλα τα μοτέρ κουρτινών
// ώστε να κλείσουν.
//
// Λειτουργία:
// - Καλεί τις επιμέρους συναρτήσεις για κάθε κουρτίνα
// - Εκτελεί κλείσιμο σε Front, Rear, Right, Left
//
// Χρησιμοποιείται για:
// - Αυτόματο κλείσιμο (π.χ. όταν σταματήσει η βροχή)
// - Global εντολή από Cloud
// =================================================
void closeCurtains() {

  // Κλείσιμο όλων των κουρτινών
  closeFrontCurtain();
  closeRearCurtain();
  closeRightCurtain();
  closeLeftCurtain();

}

// =================================================
// ΣΤΑΜΑΤΗΜΑ ΜΠΡΟΣΤΙΝΗΣ ΚΟΥΡΤΙΝΑΣ
// =================================================
// Απενεργοποιεί το μοτέρ της μπροστινής κουρτίνας
// και το φέρνει σε πλήρη ακινησία.
//
// Λειτουργία:
// - Χρησιμοποιεί H-Bridge driver (2 pins)
// - LOW + LOW → διακοπή τροφοδοσίας → stop
//
// Χρησιμοποιείται για:
// - Τερματισμό κίνησης κουρτίνας
// - Ασφαλές σταμάτημα μετά από open/close
// =================================================
void stopFrontCurtain() {

  // ================= MOTOR STOP =================
  // Διακοπή λειτουργίας κινητήρα
  digitalWrite(frontCurtainPin1, LOW);
  digitalWrite(frontCurtainPin2, LOW);

}

// =================================================
// ΣΤΑΜΑΤΗΜΑ ΠΙΣΩ ΚΟΥΡΤΙΝΑΣ
// =================================================
// Απενεργοποιεί το μοτέρ της πίσω κουρτίνας
// και το φέρνει σε πλήρη ακινησία.
//
// Λειτουργία:
// - Χρησιμοποιεί H-Bridge driver (2 pins)
// - LOW + LOW → διακοπή τροφοδοσίας → stop
//
// Χρησιμοποιείται για:
// - Τερματισμό κίνησης κουρτίνας
// - Ασφαλές σταμάτημα μετά από open/close
// =================================================
void stopRearCurtain() {

  // ================= MOTOR STOP =================
  // Διακοπή λειτουργίας κινητήρα
  digitalWrite(rearCurtainPin1, LOW);
  digitalWrite(rearCurtainPin2, LOW);

}

// =================================================
// ΣΤΑΜΑΤΗΜΑ ΔΕΞΙΑΣ ΚΟΥΡΤΙΝΑΣ
// =================================================
// Απενεργοποιεί το μοτέρ της δεξιάς κουρτίνας
// και το φέρνει σε πλήρη ακινησία.
//
// Λειτουργία:
// - Χρησιμοποιεί H-Bridge driver (2 pins)
// - LOW + LOW → διακοπή τροφοδοσίας → stop
//
// Χρησιμοποιείται για:
// - Τερματισμό κίνησης κουρτίνας
// - Ασφαλές σταμάτημα μετά από open/close
// =================================================
void stopRightCurtain() {

  // ================= MOTOR STOP =================
  // Διακοπή λειτουργίας κινητήρα
  digitalWrite(rightCurtainPin1, LOW);
  digitalWrite(rightCurtainPin2, LOW);

}

// =================================================
// ΣΤΑΜΑΤΗΜΑ ΑΡΙΣΤΕΡΗΣ ΚΟΥΡΤΙΝΑΣ
// =================================================
// Απενεργοποιεί το μοτέρ της αριστερής κουρτίνας
// και το φέρνει σε πλήρη ακινησία.
//
// Λειτουργία:
// - Χρησιμοποιεί H-Bridge driver (2 pins)
// - LOW + LOW → διακοπή τροφοδοσίας → stop
//
// Χρησιμοποιείται για:
// - Τερματισμό κίνησης κουρτίνας
// - Ασφαλές σταμάτημα μετά από open/close
// =================================================
void stopLeftCurtain() {

  // ================= MOTOR STOP =================
  // Διακοπή λειτουργίας κινητήρα
  digitalWrite(leftCurtainPin1, LOW);
  digitalWrite(leftCurtainPin2, LOW);

}

// =================================================
// ΣΤΑΜΑΤΗΜΑ ΟΛΩΝ ΤΩΝ ΚΟΥΡΤΙΝΩΝ
// =================================================
// Απενεργοποιεί όλα τα μοτέρ κουρτινών
// και τα φέρνει σε πλήρη ακινησία.
//
// Λειτουργία:
// - Καλεί τις επιμέρους συναρτήσεις stop
// - Σταματά Front, Rear, Right, Left κουρτίνες
//
// Χρησιμοποιείται για:
// - Ασφαλές σταμάτημα μετά από open/close
// - Έκτακτη διακοπή λειτουργίας
// =================================================
void stopCurtains() {

  // Σταμάτημα όλων των κουρτινών
  stopFrontCurtain();
  stopRearCurtain();
  stopRightCurtain();
  stopLeftCurtain();

}

// =================================================
// ΑΝΙΧΝΕΥΣΗ ΒΡΟΧΗΣ & ΕΛΕΓΧΟΣ ΚΟΥΡΤΙΝΩΝ
// =================================================
// Ελέγχει την ένταση της βροχής και διαχειρίζεται
// αυτόματα το άνοιγμα / κλείσιμο των κουρτινών.
//
// Λειτουργία:
// - Αν η τιμή βροχής ξεπεράσει το ανώτερο όριο → ανοίγουν
// - Αν πέσει κάτω από το κατώτερο όριο → κλείνουν
//
// Χρησιμοποιεί hysteresis:
// - rainHighThreshold → ενεργοποίηση
// - rainLowThreshold  → απενεργοποίηση
//
// rainState:
// - false → κουρτίνες κλειστές
// - true  → κουρτίνες ανοιχτές
//
// Περιλαμβάνει:
// - Μηνύματα στην LCD
// - Κίνηση μοτέρ κουρτινών
// =================================================
void controlRain() {

  // Εκτέλεση κάθε συγκεκριμένο χρονικό διάστημα
  if (myTime - readRainTime >= readRainInterval) {

    // ================= ΑΝΟΔΟΣ ΚΟΥΡΤΙΝΩΝ =================
    // Αν βρέχει πολύ και δεν έχουν ήδη ανοίξει
    if (rain > rainHighThreshold && rainState == false) {

      lcd.clear();

      // Μήνυμα στην οθόνη
      displayLine(0, "====================");
      displayLineCentered(1, "Rain Detected");
      displayLineCentered(2, "Curtains Opening");
      displayLine(3, "====================");

      // Άνοιγμα κουρτινών
      openCurtains();

      // Χρόνος για ολοκλήρωση κίνησης
      delay(curtainDelay);

      // Σταμάτημα μοτέρ
      stopCurtains();

      lcd.clear();

      // Ενημέρωση κατάστασης
      rainState = true;
    }

    // ================= ΚΑΘΟΔΟΣ ΚΟΥΡΤΙΝΩΝ =================
    // Αν δεν βρέχει και οι κουρτίνες είναι ανοιχτές
    else if (rain < rainLowThreshold && rainState == true) {

      lcd.clear();

      // Μήνυμα στην οθόνη
      displayLine(0, "====================");
      displayLineCentered(1, "Weather Clear");
      displayLineCentered(2, "Curtains Closing");
      displayLine(3, "====================");

      // Κλείσιμο κουρτινών
      closeCurtains();

      // Χρόνος για ολοκλήρωση κίνησης
      delay(curtainDelay);

      // Σταμάτημα μοτέρ
      stopCurtains();

      lcd.clear();

      // Ενημέρωση κατάστασης
      rainState = false;
    }

    // Ενημέρωση timer
    readRainTime = myTime;
  }
}

// =================================================
// ΑΥΤΟΜΑΤΟΣ ΕΛΕΓΧΟΣ ΦΩΤΙΣΜΟΥ
// =================================================
// Διαχειρίζεται αυτόματα το LIGHT φωτισμού
// με βάση την ένταση φωτός.
//
// Λειτουργία:
// - Ενεργοποιείται όταν ΔΕΝ υπάρχει manual control
//   ή όταν το σύστημα είναι offline
// - Χρησιμοποιεί thresholds για ON / OFF
// - Περιλαμβάνει hysteresis για αποφυγή flicker
//
// Ενημερώνει:
// - Κατάσταση LIGHT
// - LCD με μήνυμα
// =================================================
void controlLight() {

  // Εκτέλεση κάθε συγκεκριμένο χρονικό διάστημα
  if (myTime - readLightAVGTime >= readLightAVGInterval) {

    // ================= AUTO MODE CHECK =================
    // Αυτόματος έλεγχος μόνο όταν:
    // - ΔΕΝ είναι ενεργό manual mode
    // - Ή το σύστημα είναι offline
    if (cloudManualLight == false || offlineMode) {
        
      // ================= LIGHT ON =================
      // Αν το φως είναι χαμηλό και το LIGHT είναι σβηστό
      if (light < lightLowThreshold && lightState == false) {
    
        digitalWrite(lightPin, HIGH);
        lightState = true;
    
        // Μήνυμα στην LCD
        lcd.clear();
        displayLine(0, "====================");
        displayLineCentered(1, "Light Level Low");
        displayLineCentered(2, "Lamp ON");
        displayLine(3, "====================");
    
        // Ενεργοποίηση message timer
        modeMessageActive = true;
        modeMessageStartTime = myTime;
      }
    
      // ================= LIGHT OFF =================
      // Αν το φως επανέλθει και το LIGHT είναι αναμμένο
      else if (light > lightHighThreshold && lightState == true) {
    
        digitalWrite(lightPin, LOW);
        lightState = false;
    
        // Μήνυμα στην LCD
        lcd.clear();
        displayLine(0, "====================");
        displayLineCentered(1, "Light Restored");
        displayLineCentered(2, "Lamp OFF");
        displayLine(3, "====================");
    
        // Ενεργοποίηση message timer
        modeMessageActive = true;
        modeMessageStartTime = myTime;
      }
    }
      
    // Ενημέρωση timer
    readLightAVGTime = myTime;
  }
}

// =================================================
// ΑΥΤΟΜΑΤΟΣ ΕΛΕΓΧΟΣ ΑΝΕΜΙΣΤΗΡΑ
// =================================================
// Διαχειρίζεται τον ανεμιστήρα με βάση τη θερμοκρασία.
//
// Λειτουργία:
// - Auto mode: ενεργοποιείται όταν ΔΕΝ υπάρχει manual control
// - Manual mode: έλεγχος από Cloud
// - Χρησιμοποιεί hysteresis για σταθερότητα
//
// Ενημερώνει:
// - Κατάσταση ανεμιστήρα
// - LCD με μήνυμα
// =================================================
void controlFan() {

  // Εκτέλεση κάθε συγκεκριμένο χρονικό διάστημα
  if (myTime - readFanTime >= readFanInterval) {

    // ================= AUTO MODE CHECK =================
    // Αυτόματος έλεγχος μόνο όταν:
    // - ΔΕΝ είναι ενεργό manual mode
    // - Ή το σύστημα είναι offline
    if (cloudManualFan == false || offlineMode) {

      // ================= FAN ON =================
      // Αν η θερμοκρασία είναι θψηλή και το FAN είναι σβηστό
      if (temperature > fanHighThreshold && fanState == false) {

        digitalWrite(fanPin, HIGH);
        fanState = true;

        // Μήνυμα στην LCD
        lcd.clear();
        displayLine(0, "====================");
        displayLineCentered(1, "High Temperature");
        displayLineCentered(2, "Fan ON");
        displayLine(3, "====================");

        // Ενεργοποίηση message timer
        modeMessageActive = true;
        modeMessageStartTime = myTime;
      }

      // ================= FAN OFF =================
      // Αν η θερμοκρασίασ επανέλθει και το FAN είναι αναμμένο
      else if (temperature < fanLowThreshold && fanState == true) {

        digitalWrite(fanPin, LOW);
        fanState = false;

        // Μήνυμα στην LCD
        lcd.clear();
        displayLine(0, "====================");
        displayLineCentered(1, "Temperature OK");
        displayLineCentered(2, "Fan OFF");
        displayLine(3, "====================");

        // Ενεργοποίηση message timer
        modeMessageActive = true;
        modeMessageStartTime = myTime;
      }
    }

    // ================= TIMER UPDATE =================
    readFanTime = myTime;
  }
}

// =================================================
// BUZZER ALERT (NON-BLOCKING - INTERMITTENT)
// =================================================
// Διαχειρίζεται ηχητικές ειδοποιήσεις (alerts) μέσω buzzer.
//
// Λειτουργία:
// - Ελέγχει συνεχώς (real-time) αν ισχύει κάποιο alert condition
// - Αν ισχύει → ενεργοποιεί διακοπτόμενο ήχο (beep pattern)
// - Αν ΔΕΝ ισχύει → σταματά άμεσα τον ήχο
//
// Alert Conditions:
// - Υψηλή βροχή (rain > threshold)
// - Χαμηλός φωτισμός (light < threshold)
// - Υψηλή θερμοκρασία (temperature > threshold)
//
// Χαρακτηριστικά:
// - Non-blocking (χωρίς delay)
// - Χρησιμοποιεί millis() για timing
// - Σταθερό μοτίβο ON/OFF (ίσα διαστήματα)
//
// =================================================
void controlBuzzer() {

  // ================= CONDITION =================
  // Υπολογισμός συνθήκης alert σε πραγματικό χρόνο
  bool alertCondition = (
    rain > rainHighThreshold ||
    light < lightLowThreshold ||
    temperature > fanHighThreshold
  );

  // ================= TRIGGER (EDGE DETECTION) =================
  // Αν μόλις ξεκίνησε το alert
  if (alertCondition && !lastAlertState) {

    alertDetected = true;   // trigger ONCE
  }

  // Αν μόλις τελείωσε το alert
  if (!alertCondition && lastAlertState) {

    alertDetected = false;  // reset
  }

  // Ενημέρωση previous state
  lastAlertState = alertCondition;

  // ================= DISABLE =================
  // Αν δεν υπάρχει alert:
  // - Σταματά ο ήχος
  // - Reset κατάστασης
  if (!alertCondition) {

    noTone(buzzerPin);     // Διακοπή ήχου
    buzzerState = false;   // Reset state
    return;                // Έξοδος από τη συνάρτηση
  }

  // ================= NON-BLOCKING BEEP =================
  // Εναλλαγή ON/OFF με βάση το χρονικό διάστημα
  if (myTime - buzzerTimer >= buzzerInterval) {

    // Ενημέρωση χρονόμετρου
    buzzerTimer = myTime;

    // ================= TOGGLE =================
    // Αν παίζει ήχος → σταμάτα
    if (buzzerState) {

      noTone(buzzerPin);   // OFF phase
      buzzerState = false;

    } 
    // Αν δεν παίζει → ξεκίνα ήχο
    else {

      tone(buzzerPin, 1000); // ON phase (σταθερή συχνότητα)
      buzzerState = true;
    }
  }
}

// =================================================
// ΔΙΑΧΕΙΡΙΣΗ ΔΙΑΚΟΠΤΗ LIGHT TRACKING
// =================================================
// Ελέγχει τον φυσικό διακόπτη για ενεργοποίηση
// ή απενεργοποίηση του light tracking.
//
// Λειτουργία:
// - Ανιχνεύει αλλαγή κατάστασης (edge detection)
// - INPUT_PULLUP:
//    LOW  → ON
//    HIGH → OFF
//
// Ενημερώνει:
// - lightTrackingEnabled flag
// - LCD με μήνυμα κατάστασης
// =================================================
void readTrackingSwitch() {

  // Ανάγνωση κατάστασης διακόπτη
  bool currentSwitchState = digitalRead(switchLightPin);

  // ================= STATE CHANGE DETECTION =================
  // Εκτελείται μόνο όταν αλλάξει κατάσταση
  // και δεν εμφανίζεται ήδη μήνυμα
  if (currentSwitchState != lastSwitchState && !modeMessageActive) {

    // ================= MODE UPDATE =================
    // LOW → ενεργό tracking
    // HIGH → απενεργοποιημένο
    lightTrackingEnabled = (currentSwitchState == LOW);

    // ================= MESSAGE TIMER =================
    modeMessageActive = true;
    modeMessageStartTime = myTime;

    // ================= LCD UPDATE =================
    lcd.clear();
    displayLine(0, "====================");

    // Εμφάνιση κατάστασης
    if (lightTrackingEnabled) {
      displayLineCentered(1, "Light Tracking ON");
    } else {
      displayLineCentered(1, "Light Tracking OFF");
      stopMotors();
    }

    displayLine(2, "                    ");
    displayLine(3, "====================");
  }

  // Αποθήκευση τελευταίας κατάστασης
  lastSwitchState = currentSwitchState;
}
