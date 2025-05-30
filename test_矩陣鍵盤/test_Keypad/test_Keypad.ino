#include <Keypad.h>

// const byte ROWS = 4;
// const byte COLS = 3;

const byte ROWS = 2;
const byte COLS = 2;

// char keys[ROWS][COLS] = {
//   {'1','2','3'},
//   {'4','5','6'},
//   {'7','8','9'},
//   {'*','0','#'}
// };

char keys[ROWS][COLS] = {
  {'O','X'},
  {'Y','H'}
};

// byte rowPins[ROWS] = {32, 33, 25, 26};
// byte colPins[COLS] = {27, 14, 13}; // ⚠️ GPIO12 改成 GPIO13

byte rowPins[ROWS] = {32, 33};
byte colPins[COLS] = {27, 14}; // ⚠️ GPIO12 改成 GPIO13

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(115200);
  Serial.println("Ready");
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    Serial.print("Pressed: ");
    Serial.println(key);
  }
}
