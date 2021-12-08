#include <Wire.h>
#include "ACROBOTIC_SSD1306.h"

int Status = 12;
int sensor = 13;

void setup() {
  //kijelzo kezdete
  Wire.begin();
  oled.init();                      // Initialze SSD1306 OLED display
  oled.clearDisplay();              // Clear screen
  
  //Motion
  Serial.begin(9600);
  pinMode(sensor, INPUT); // declare sensor as input
  pinMode(Status, OUTPUT);  // declare LED as output

  //

}

void loop() {
  //Motion
  oled.clearDisplay(); 
  long state = digitalRead(sensor);
  if(state == HIGH){
     oled.setTextXY(0,0);              
     oled.putString("Motion");
     oled.setTextXY(1,0);              
     oled.putString("detected!");
     Serial.println("Motion detected!");
     delay(4000);
   }
  else {
     oled.setTextXY(0,0);              
     oled.putString("No motion");
     oled.setTextXY(1,0);              
     oled.putString("detected!");
     Serial.println("No motion detected!");
     delay(4000);
     }
}
