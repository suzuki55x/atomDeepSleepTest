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
    while(1) {
      M5.update();
      if(M5.Btn.isReleased()) break;
      delay(50);// ボタン離さないとまたdeepSleepに入る可能性があるので。
    }
    // Ready
    M5.dis.drawpix(0, ledColor[1]);// G
  } else {
    M5.dis.drawpix(0, ledColor[3]);// W
  }
  pinMode(GPIO_NUM_39, INPUT_PULLUP);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_39, LOW);// BTN(GPIO39)がLOWで復帰するように設定

  delay(50);

  M5.update();
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.update();
  if(M5.Btn.wasReleased()) {
    M5.dis.clear();// Off
    delay(50);
    esp_deep_sleep_start();
  } else if (M5.Btn.wasPressed()) {
    M5.dis.drawpix(0, ledColor[2]);// B
  }
  delay(10);
}
