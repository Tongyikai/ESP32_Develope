

const int relayPin = 26;

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);  // 預設關閉
  delay(3000); // 等你觀察一下 LED 有沒有熄滅
  digitalWrite(relayPin, HIGH);   // 啟動繼電器
  delay(5000);
  digitalWrite(relayPin, LOW);  // 再關閉
}

void loop() {
  // 空的 loop
}
