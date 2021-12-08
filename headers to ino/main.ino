#include "page.h"
#include <Wire.h>
#include "ACROBOTIC_SSD1306.h"

Page* testPage = new Page("page name");
Item* testItem = new Item("item name", testPage);
  

void setup() {
  Serial.begin(115200);
  Wire.begin();
  oled.init();
  oled.clearDisplay();
}

void kijelzo(){
  oled.clearDisplay();
  oled.setTextXY(0,0);
  oled.putString((testItem->getLink()->getTitle()).c_str());
  delay(2000);
  oled.setTextXY(0,0);
  oled.putString("                ");
  oled.setTextXY(0,0);
  oled.putString("aaa");
  delay(3000);
  }

void loop() {
  kijelzo();
  
}
