// 自動澆水
const int soilAnalogPin = 35;    // GPIO5 接類比輸出 A0
const int relayPin = 26;      // 控制繼電器的腳位
// 閾值設定（根據實測值調整）
const int dryThreshold = 2000;  // 小於此值視為乾燥

/* ******************************** setup ******************************** */
void setup() {
  Serial.begin(115200);
  delay(2000); // 等待啟動穩定
  Serial.println("********** ESP32 Ready **********");

  // 自動澆水系統soilAnalogPin
  // pinMode(soilPin, INPUT);     // 感測器輸出為數位訊號
  pinMode(soilAnalogPin, INPUT);  // 類比
  pinMode(relayPin, OUTPUT);   // 繼電器為輸出腳
  digitalWrite(relayPin, HIGH); // 預設不啟動馬達（LOW 為關閉）根據你繼電器模組的邏輯需要反過來（部分模組 LOW 為啟動）
}

void loop() {
  // 自動澆水系統
  
  int soilValue = analogRead(soilAnalogPin);
  Serial.print("Soil value: ");
  Serial.println(soilValue);

  if (dryThreshold <  soilValue) {
    Serial.println("Soil is dry → watering...");
    digitalWrite(relayPin, LOW);   // 啟動水泵
    delay(5000);                    // 澆水 5 秒
    digitalWrite(relayPin, HIGH);    // 關閉水泵
  } else {
    Serial.println("Soil is wet → no action.");
    digitalWrite(relayPin, HIGH);    // 保持關閉
  }
}
