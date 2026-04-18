// =================================================
// FAKE SENSOR SYSTEM - HEADER FILE
// =================================================
// Δηλώνει (declare) όλες τις μεταβλητές και συναρτήσεις
// για το σύστημα προσομοίωσης αισθητήρων.
//
// Λειτουργία:
// - Επιτρέπει τη χρήση fake δεδομένων αντί για πραγματικούς αισθητήρες
// - Χρησιμοποιεί base + offset design για εύκολο tuning
// - Οι πραγματικές τιμές ορίζονται στο .cpp αρχείο
//
// Σημαντικό:
// - Χρησιμοποιείται "extern" για να αποφευχθεί διπλή δήλωση
// - Το αρχείο αυτό περιέχει ΜΟΝΟ δηλώσεις (όχι υλοποίηση)
//
// Χρήση:
// - #include "fakeSensors.h" σε όποιο αρχείο χρειάζεται πρόσβαση
// =================================================

#ifndef FAKE_SENSORS_H
#define FAKE_SENSORS_H

// =================================================
// DHT SENSOR (Temperature & Humidity)
// =================================================

// -------- BASE VALUES --------
// Βασικές τιμές (ρυθμίζονται στον διαγωνισμό)
extern float baseTemperature;
extern float baseHumidity;

// -------- CURRENT VALUES --------
// Τρέχουσες μεταβαλλόμενες τιμές
extern float fakeTemperature;
extern float fakeHumidity;

// -------- CONFIG --------
// Offset = εύρος μεταβολής γύρω από τη βάση
// Step   = ταχύτητα αλλαγής
extern float fakeTempOffset;
extern float fakeTempStep;

extern float fakeHumOffset;
extern float fakeHumStep;


// =================================================
// PRESSURE & ALTITUDE SENSOR
// =================================================

extern float basePressure;
extern float fakePressure;
extern float fakePressOffset;
extern float fakePressStep;

extern float baseAltitude;
extern float fakeAltitude;
extern float fakeAltOffset;
extern float fakeAltStep;


// =================================================
// RAIN SENSOR
// =================================================

extern int baseRain;
extern int fakeRain;
extern int fakeRainOffset;


// =================================================
// LIGHT SENSOR (TOTAL + DIRECTIONS)
// =================================================

// -------- TOTAL LIGHT --------
extern int baseLight;
extern int fakeLight;
extern int fakeLightOffset;

// -------- DIRECTIONAL LIGHT --------
// Αισθητήρες για light tracking
extern int baseTop;
extern int fakeTop;

extern int baseBottom;
extern int fakeBottom;

extern int baseRight;
extern int fakeRight;

extern int baseLeft;
extern int fakeLeft;


// =================================================
// GNSS SENSOR (Location - Latitude & Longitude)
// =================================================

// -------- BASE VALUES --------
// Σταθερή τοποθεσία (ρυθμίζεται για demo)
extern float baseLat;
extern float baseLon;

// -------- CURRENT VALUES --------
// Τρέχουσες μεταβαλλόμενες τιμές (simulation)
extern float fakeLat;
extern float fakeLon;

// -------- STATUS --------
// Κατάσταση GPS σήματος
extern bool fakeGnssFix;

// -------- CONFIG --------
// Offset = εύρος μεταβολής γύρω από τη βάση
// Step   = ταχύτητα αλλαγής
extern float fakeLatOffset;
extern float fakeLonOffset;

extern float fakeLatStep;
extern float fakeLonStep;


// =================================================
// FUNCTION DECLARATIONS
// =================================================
// Δηλώσεις συναρτήσεων update (υλοποίηση στο .cpp)

void updateFakeDHT();
void updateFakeSEN();
void updateFakeRain();
void updateFakeLight();
void updateFakeGNSS();

#endif