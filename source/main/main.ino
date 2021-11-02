#include <iostream>
#include <Wire.h>
#include "ACROBOTIC_SSD1306.h"
#include "buttons.h"

#define light A0
#define motion 13
#define upButton 2
#define downButton 12
#define okButton 10

short lightTreshold = 1000;
int timer = 10000;
bool bulb = false;
unsigned long currentTime, lastTriggerTime = 0;
int lightValue;
bool motionState;

MenuPage mainMenuPage = MenuPage("MAIN MENU");
MenuPage settingsPage = MenuPage("SETTINGS", &mainMenuPage);
MenuPage monitoringPage = MenuPage("MONITORING", &mainMenuPage);
MenuPage logsPage = MenuPage("LOGS", &mainMenuPage);

Item settingsItem = Item("Settings", &settingsPage);
Item monitoringItem = Item("Monitoring", &monitoringPage);
Item logsItem = Item("Logs", &monitoringPage);

Screen screen;

void setup()
{
  Wire.begin();
  oled.init();
  oled.clearDisplay();
  
  pinMode(upButton, INPUT);
  pinMode(downButton, INPUT);
  pinMode(okButton, INPUT);

  pinMode(motion, INPUT);
  pinMode(light, INPUT);
  
  Serial.begin(115200);
  
  mainMenuPage.addItem(&settingsItem);
  mainMenuPage.addItem(&monitoringItem);
  mainMenuPage.addItem(&logsItem);
}

void loop()
{
  lightValue = analogRead(light);
  motionState = digitalRead(motion);
  currentTime = millis();
  
  if (lightValue <= lightTreshold && motionState)
  {
    lastTriggerTime = currentTime;
    if (!bulb) bulb = true;
  }
  else if (bulb && currentTime - lastTriggerTime > timer) bulb = false;

  std::cout << "Bulb: " << bulb
            << ", Motion: " << motionState
            << ", Time: " << (currentTime - lastTriggerTime) / 1000
            << ", Light: " << lightValue
            << std::endl;
}
