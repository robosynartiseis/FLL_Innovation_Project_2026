// =================================================
// SIMULATION SYSTEM - IMPLEMENTATION FILE
// =================================================
// Υλοποίηση (implementation) του συστήματος fake sensors.
//
// Περιλαμβάνει:
// - Ορισμό όλων των global μεταβλητών (base + current)
// - Συναρτήσεις update για προσομοίωση αισθητήρων
//
// Λειτουργία:
// - Χρησιμοποιεί base + offset για έλεγχο εύρους τιμών
// - Προσθέτει μικρές τυχαίες μεταβολές (random)
// - Διατηρεί ρεαλιστική συμπεριφορά αισθητήρων
//
// Σημαντικό:
// - Το simulation.h περιέχει ΜΟΝΟ δηλώσεις (extern)
// - Εδώ βρίσκονται οι πραγματικές τιμές
// =================================================

#include "simulation.h"


// =================================================
// DHT SENSOR (Temperature & Humidity)
// =================================================

// -------- CURRENT VALUES --------
// Τρέχουσες τιμές που μεταβάλλονται δυναμικά
float fakeTemperature = baseTemperature;
float fakeHumidity = baseHumidity;

// -------- CONFIG --------
// Offset → εύρος γύρω από τη βάση
// Step   → πόσο γρήγορα αλλάζει η τιμή
float fakeTempOffset = 3.0;
float fakeTempStep = 0.1;

float fakeHumOffset = 10.0;
float fakeHumStep = 0.5;


// =================================================
// PRESSURE & ALTITUDE SENSOR
// =================================================

float fakePressure = basePressure;
float fakePressOffset = 7.0;
float fakePressStep = 0.1;

float fakeAltitude = baseAltitude;
float fakeAltOffset = 5.0;
float fakeAltStep = 0.2;


// =================================================
// RAIN SENSOR
// =================================================

int fakeRain = baseRain;
int fakeRainOffset = 20;


// =================================================
// LIGHT SENSOR (TOTAL + DIRECTIONS)
// =================================================

// -------- TOTAL LIGHT --------
int fakeLight = baseLight;
int fakeLightOffset = 40;

// -------- DIRECTIONAL LIGHT --------
// Χρησιμοποιούνται για light tracking
int baseTop = baseLight;
int fakeTop = baseTop;

int baseBottom = baseLight - 5;
int fakeBottom = baseBottom;

int baseRight = baseLight + 5;
int fakeRight = baseRight;

int baseLeft = baseLight;
int fakeLeft = baseLeft;


// =================================================
// GNSS SENSOR (Location - Latitude & Longitude)
// =================================================

// -------- CURRENT VALUES --------
float fakeLat = baseLat;
float fakeLon = baseLon;


// -------- CONFIG (RANGE) --------
// Μέγιστη απόκλιση από τη βάση (σε μοίρες)
// → το σύστημα κινείται μέσα σε μικρή περιοχή
float fakeLatOffset = 0.0005;
float fakeLonOffset = 0.0005;


// -------- CONFIG (STEP) --------
// Βήμα μεταβολής ανά update
//
// 0.00001 ≈ ~1 μέτρο
// → καθορίζει πόσο “γρήγορα” κινείται το GPS
float fakeLatStep = 0.00001;
float fakeLonStep = 0.00001;


// -------- STATUS --------
// Κατάσταση GPS signal
// Στο simulation: συνήθως true για σταθερό demo
bool fakeGnssFix = true;


// =================================================
// UPDATE FUNCTIONS
// =================================================


// =================================================
// UPDATE FAKE DHT SENSOR DATA
// =================================================
// Προσομοιώνει θερμοκρασία & υγρασία
//
// Κάνει:
// - Μικρές τυχαίες μεταβολές (random drift)
// - Περιορίζει τιμές με βάση base ± offset
// - Στρογγυλοποιεί για καθαρή εμφάνιση
// =================================================
void updateFakeDHT() {

  // -------- TEMPERATURE --------
  // Τυχαία μεταβολή
  fakeTemperature += random(-2, 3) * fakeTempStep;

  // Υπολογισμός ορίων
  float tempMin = baseTemperature - fakeTempOffset;
  float tempMax = baseTemperature + fakeTempOffset;

  // Περιορισμός τιμής
  fakeTemperature = constrain(fakeTemperature, tempMin, tempMax);

  // Στρογγυλοποίηση
  fakeTemperature = round1(fakeTemperature);

  // -------- HUMIDITY --------
  fakeHumidity += random(-2, 3) * fakeHumStep;

  float humMin = baseHumidity - fakeHumOffset;
  float humMax = baseHumidity + fakeHumOffset;

  fakeHumidity = constrain(fakeHumidity, humMin, humMax);
  fakeHumidity = round1(fakeHumidity);
}


// =================================================
// UPDATE FAKE PRESSURE & ALTITUDE
// =================================================
// Προσομοιώνει περιβαλλοντικές συνθήκες
// =================================================
void updateFakeSEN() {

  // -------- PRESSURE --------
  fakePressure += random(-5, 6) * fakePressStep;

  float pressMin = basePressure - fakePressOffset;
  float pressMax = basePressure + fakePressOffset;

  fakePressure = constrain(fakePressure, pressMin, pressMax);
  fakePressure = round1(fakePressure);

  // -------- ALTITUDE --------
  fakeAltitude += random(-3, 4) * fakeAltStep;

  float altMin = baseAltitude - fakeAltOffset;
  float altMax = baseAltitude + fakeAltOffset;

  fakeAltitude = constrain(fakeAltitude, altMin, altMax);
  fakeAltitude = round1(fakeAltitude);
}


// =================================================
// UPDATE FAKE RAIN SENSOR
// =================================================
// Προσομοιώνει ποσοστό βροχής (0–100% περίπου)
// =================================================
void updateFakeRain() {

  // Τυχαία μεταβολή
  fakeRain += random(-2, 3);

  // Υπολογισμός ορίων
  int rainMin = baseRain - fakeRainOffset;
  int rainMax = baseRain + fakeRainOffset;

  // Περιορισμός τιμής
  fakeRain = constrain(fakeRain, rainMin, rainMax);
}


// =================================================
// UPDATE FAKE LIGHT SENSOR
// =================================================
// Προσομοιώνει φως από 4 κατευθύνσεις + συνολικό
//
// Χρησιμοποιείται για:
// - light tracking
// - περιβαλλοντική ανάλυση
// =================================================
void updateFakeLight() {

  // -------- TOP --------
  fakeTop += random(-2, 3);
  fakeTop = constrain(fakeTop, baseTop - fakeLightOffset, baseTop + fakeLightOffset);

  // -------- BOTTOM --------
  fakeBottom += random(-2, 3);
  fakeBottom = constrain(fakeBottom, baseBottom - fakeLightOffset, baseBottom + fakeLightOffset);

  // -------- RIGHT --------
  fakeRight += random(-2, 3);
  fakeRight = constrain(fakeRight, baseRight - fakeLightOffset, baseRight + fakeLightOffset);

  // -------- LEFT --------
  fakeLeft += random(-2, 3);
  fakeLeft = constrain(fakeLeft, baseLeft - fakeLightOffset, baseLeft + fakeLightOffset);

  // -------- TOTAL LIGHT --------
  fakeLight += random(-3, 4);
  fakeLight = constrain(fakeLight, baseLight - fakeLightOffset, baseLight + fakeLightOffset);
}


// =================================================
// UPDATE FAKE GNSS
// =================================================
// Προσομοιώνει GPS δεδομένα (lat/lon)
//
// Λειτουργία:
// - Μικρές τυχαίες μεταβολές (drift)
// - Περιορισμός τιμών γύρω από base ± offset
// - Σταθερή συμπεριφορά για demo
// =================================================
void updateFakeGNSS() {

  // -------- RANDOM DRIFT --------
  // Μικρή μεταβολή θέσης
  fakeLat += random(-2, 3) * fakeLatStep;
  fakeLon += random(-2, 3) * fakeLonStep;

  // -------- LIMITS --------
  // Όρια γύρω από τη βάση
  float latMin = baseLat - fakeLatOffset;
  float latMax = baseLat + fakeLatOffset;

  float lonMin = baseLon - fakeLonOffset;
  float lonMax = baseLon + fakeLonOffset;

  // Περιορισμός τιμών
  fakeLat = constrain(fakeLat, latMin, latMax);
  fakeLon = constrain(fakeLon, lonMin, lonMax);

  // -------- GPS FIX --------
  // Για demo: πάντα ενεργό
  fakeGnssFix = (random(0, 10) > 1);
}


// =================================================
// SENSOR BASE VALUES (Temperature & Humidity & Pressure & Altitude & Rain & Light & GNSS)
// =================================================

// -------- BASE VALUES --------
// Βασικές τιμές (αλλάζονται στον διαγωνισμό)
// float baseTemperature = 16.5;
// float baseHumidity = 65.0;
// float basePressure = 1012.0;
// float baseAltitude = 25.0;
// int baseRain = 20;
// int baseLight = 60;
// float baseLat = 36.4340;
// float baseLon = 28.2170;

// =================================================
// SENSOR BASE VALUES
// Competition Location:
// The Ranch - Sofiko Korinthias
// Saturday 9 May
// =================================================

// -------- BASE VALUES --------

// Θερμοκρασία περιβάλλοντος (~22°C)
float baseTemperature = 22.4;

// Σχετική υγρασία (~62%)
float baseHumidity = 62.0;

// Βαρομετρική πίεση (τυπική για την περιοχή)
float basePressure = 1013.6;

// Υψόμετρο περιοχής (~270m)
float baseAltitude = 270.0;

// Μικρή πιθανότητα βροχής
int baseRain = 8;

// Υψηλή ηλιοφάνεια (ημέρα)
int baseLight = 82;

// GNSS Coordinates - The Ranch
float baseLat = 37.8025;
float baseLon = 23.0530;