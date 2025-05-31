#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <Keypad.h>

// 矩陣鍵盤
const byte ROWS = 2;
const byte COLS = 2;
char keys[ROWS][COLS] = {
  {'O','X'},
  {'Y','H'}
};
byte rowPins[ROWS] = {32, 33};
byte colPins[COLS] = {27, 14}; // ⚠️ GPIO12 改成 GPIO13
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


// AP 模式
const char* ssid = "ESP32-LED-AP";
const char* password = "557799";


WebServer server(80);
const int ledPin = 2;


// 檔案上傳
void listFiles() {
  Serial.println("LittleFS 檔案列表：");
  File root = LittleFS.open("/");
  File file = root.openNextFile();
  while (file) {
    Serial.println(file.name());
    file = root.openNextFile();
  }
}

// 矩陣鍵盤
void xButton() {
  WiFi.softAPdisconnect(true);  // 關閉 AP
  Serial.println("關閉 AP");
}


void setup() {
  Serial.begin(115200);
  Serial.println("********** ESP32 Ready **********");
  pinMode(ledPin, OUTPUT);

  WiFi.softAP(ssid, password);
  Serial.println("AP Started");
  Serial.println(WiFi.softAPIP());

  if (!LittleFS.begin()) {
    Serial.println("LittleFS 初始化失敗");
    return;
  }

  listFiles();

  server.on("/", HTTP_GET, []() {
    File file = LittleFS.open("/index.html", "r");
    if (!file) {
      server.send(500, "text/plain; charset=utf-8", "Not found index.html");
      return;
    }
    server.streamFile(file, "text/html; charset=utf-8");
    file.close();
  });

  server.serveStatic("/style.css", LittleFS, "/style.css");

  server.on("/ledOn", []() {
    if (server.hasArg("duration")) {
      int duration = server.arg("duration").toInt();
      digitalWrite(ledPin, HIGH);
      Serial.println("LED ON");
      // server.send(200, "text/html", "<h3>LED Turn On</h3><a href='/'>GO BACK</a>");
      server.send_P(200, "text/html", PSTR("<h3>LED Turn On</h3><a href='/'>GO BACK</a>"));

      delay(duration * 1000);
      digitalWrite(ledPin, LOW);
      Serial.println("LED OFF");
    } else {
      server.send(400, "text/html", "<h3>Missing duration parameter</h3><a href='/'>GO BACK</a>");
    }
  });

  server.on("/ledOff", []() {
    digitalWrite(ledPin, LOW);
    server.send(200, "text/html", "<h3>LED Turn Off</h3><a href='/'>GO BACK</a>");
  });

  server.begin();
}

void loop() {
  server.handleClient();
  
  // 矩陣鍵盤
  char key = keypad.getKey();
  if (key) {
    Serial.print("Pressed: ");
    Serial.println(key);
    if (key == 'X') {
      xButton();
    }
  }
}
