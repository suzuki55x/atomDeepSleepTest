#include <M5Atom.h>
#include <Adafruit_SHT31.h>
#include <Wire.h>

Adafruit_SHT31 sht3x = Adafruit_SHT31(&Wire);// ENV2ユニット用

const long ledColor[]={
  0x008000,// R
  0x800000,// G
  0x0000C0,// B
  0x606060 // W
};

void setup() {
  M5.begin(true, true, true);// Serial, I2C, Display
  Wire.begin(26, 32, 100000);// I2C通信初期化。pin26, pin32を使って100kHzでI2C通信
  if (!sht3x.begin(0x44)) {
    // ENV2センサが付いていない場合、シリアルにエラ〜メッセージを出す。
    Serial.println("ENV2: Could not find a valid SHT3X sensor, check wiring!");
  }
  if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
    // deepSleep後の復帰の場合実行
    M5.dis.drawpix(0, ledColor[0]);// Red
    while(1) {
      M5.update();// ボタン読み取り状態更新
      if(M5.Btn.isReleased()) break;
      delay(50);
    }
    // Ready
    M5.dis.drawpix(0, ledColor[1]);// Green
  } else {
    M5.dis.drawpix(0, ledColor[3]);// White
  }
  pinMode(GPIO_NUM_39, INPUT_PULLUP);// pin39(ボタン)をプルアップ付き入力へ
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
    esp_deep_sleep_start();// ボタンが押して離された時、ディープスリープモードへ
  } else if (M5.Btn.wasPressed()) {
    M5.dis.drawpix(0, ledColor[2]);// Blue
  }
  float tmp = sht3x.readTemperature();// 温度
  float hum = sht3x.readHumidity();// 湿度
  Serial.printf("Temp: %2.2f*C Humi: %0.2f%%\r\n", tmp, hum);// シリアル出力
  delay(100);
}
