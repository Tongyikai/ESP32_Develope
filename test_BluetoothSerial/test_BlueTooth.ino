#include <BTAddress.h>
#include <BTAdvertisedDevice.h>
#include <BTScan.h>
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial _blueTooth;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  _blueTooth.begin("BT:SiteB"); //Bluetooth device name
  _blueTooth.println("The device started, now you can pair it with bluetooth!");

}

void loop() {
  // put your main code here, to run repeatedly:
  if (_blueTooth.available()) {
    _blueTooth.write(_blueTooth.read());
  }
  delay(20);
}
