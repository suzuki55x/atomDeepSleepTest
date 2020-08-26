#include <M5Atom.h>

const long ledColor[]={
  0x008000,// R
  0x800000,// G
  0x0000C0,// B
  0x606060 // W
};

void setup() {
  // put your setup code here, to run once:
  M5.begin(false, false, true);// Serial, I2C, Display

  if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {// deepSleep後の復帰かどうか
    M5.dis.drawpix(0, ledColor[0]);// R
  } else {
    M5.dis.drawpix(0, ledColor[3]);// W   
  }
  pinMode(GPIO_NUM_39, INPUT_PULLUP);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_39, LOW);

  delay(1000);// スリープと復帰でボタン同じなので…

  M5.dis.drawpix(0, ledColor[1]);// G   
}

void loop() {
  // put your main code here, to run repeatedly:
  if(M5.Btn.wasReleased()) {
    esp_deep_sleep_start();
  } else if (M5.Btn.wasPressed()) {
      M5.dis.drawpix(0, ledColor[2]);// B
  }
  M5.update();
  delay(10);
}
