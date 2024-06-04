#include <Adafruit_Fingerprint.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the fingerprint sensor and keypad
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LCD setup (optional)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.print("Initializing...");

  // Set up fingerprint sensor
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    lcd.setCursor(0, 1);
    lcd.print("Sensor ready");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    lcd.setCursor(0, 1);
    lcd.print("Sensor error");
    while (1);
  }
  delay(2000);
  lcd.clear();
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Scan Finger:");
  
  if (getFingerprintID() == 1) { // Assuming ID 1 is the authorized user
    lcd.clear();
    lcd.print("Enter PIN:");
    String pin = "";
    while (pin.length() < 4) {
      char key = keypad.getKey();
      if (key) {
        pin += key;
        lcd.print("*");
      }
    }
    
    if (pin == "1234") { // Replace with actual PIN
      lcd.clear();
      lcd.print("Access Granted");
      Serial.println("Access Granted");
      delay(2000);
    } else {
      lcd.clear();
      lcd.print("Wrong PIN");
      Serial.println("Wrong PIN");
      delay(2000);
    }
  } else {
    lcd.clear();
    lcd.print("Try Again");
    Serial.println("Try Again");
    delay(2000);
  }
  lcd.clear();
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;

  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}