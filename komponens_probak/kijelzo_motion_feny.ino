#include <Wire.h>
#include "ACROBOTIC_SSD1306.h"

int motion = 13;
int light = 15;

void setup() {
  //kijelzo kezdete
  Wire.begin();
  oled.init();                      // Initialze SSD1306 OLED display
  oled.clearDisplay();              // Clear screen
  
  //Motion
  Serial.begin(9600);
  pinMode(motion, INPUT); // declare sensor as input
  pinMode(light, INPUT);

}

void loop() {
  int ldrStatus = digitalRead(light);
  //Motion
  oled.clearDisplay(); 
  long state = digitalRead(motion);
  if(state == HIGH){
     oled.setTextXY(0,0);              
     oled.putString("Motion");
     oled.setTextXY(1,0);              
     oled.putString("detected!");
     Serial.println("Motion detected!");
     delay(3000);
   }
  else {
     oled.setTextXY(0,0);              
     oled.putString("No motion");
     oled.setTextXY(1,0);              
     oled.putString("detected!");
     Serial.println("No motion detected!");
     delay(3000);
     }
  if(ldrStatus == 1){
    Serial.println("Nincs feny!");
    oled.setTextXY(3,0);              
    oled.putString("---------");
    oled.setTextXY(4,0);              
    oled.putString("Nincs feny!");
    delay(1000);
    }
  else{
    Serial.println("Van feny!");
    oled.setTextXY(3,0);              
    oled.putString("---------");
    oled.setTextXY(4,0);              
    oled.putString("Van feny!");
    delay(1000);
    }
}
