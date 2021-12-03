#include <iostream>
#include <Wire.h>
#include "ACROBOTIC_SSD1306.h"
#include "screen.h"
#include "page.h"

///GPIO names
#define light A0
#define motion 10
#define upButton 12
#define downButton 13
#define okButton 14

///constant algorythm variables
constexpr short monitoringRefreshRate = 2;

///algorythm variables
short screenTimer, bulbTimer, lightTreshold;
bool bulb = false, powerSaveMode = true;
unsigned long lastTriggerTime = 0, buttonPressTime = 0, monitoringRefreshTime = 0;
short lightValue;
bool motionState;

//creating pages
MenuPage mainMenuPage = MenuPage("MAIN MENU");
MenuPage lightTresholdPage = MenuPage("LIGHT TRSH");
MenuPage bulbTimerPage = MenuPage("BULB TIMER");
MenuPage screenTimerPage = MenuPage("SCREEN TIMER");
MonitoringPage monitoringPage = monitoringPage.getInstance();

///setup
void setup()
{
  ///screen initialization
  Wire.begin();
  oled.init();
  oled.clearDisplay();

  ///accessing pins
  pinMode(upButton, INPUT);
  pinMode(downButton, INPUT);
  pinMode(okButton, INPUT);
  pinMode(motion, INPUT);
  pinMode(light, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  ///filling pages
  mainMenuPage.addOpt("Bulb timer");
  mainMenuPage.addOpt("Light trsh");
  mainMenuPage.addOpt("Screen timer");
  mainMenuPage.addOpt("Monitoring");
  
  lightTresholdPage.addOpt("Very low light", 1000);
  lightTresholdPage.addOpt("Low light", 800);
  lightTresholdPage.addOpt("Medium light", 500);
  lightTresholdPage.addOpt("High light", 200);
  lightTresholdPage.addOpt("Very high light", 10);
  
  bulbTimerPage.addOpt("5 seconds", 5);
  bulbTimerPage.addOpt("10 seconds", 10);
  bulbTimerPage.addOpt("30 seconds", 30);
  bulbTimerPage.addOpt("1 minute", 60);
  
  screenTimerPage.addOpt("5 seconds", 5);
  screenTimerPage.addOpt("10 seconds", 10);
  screenTimerPage.addOpt("30 seconds", 30);
  screenTimerPage.addOpt("1 minute", 60);

  monitoringPage.setScreenTimerPointer(&lastTriggerTime);
  monitoringPage.setLightValuePointer(&lightValue);
  monitoringPage.setMotionStatePointer(&motionState);

  ///assigning values from page options to optional values
  lightTresholdPage.setArrowPos(2);
  lightTreshold = lightTresholdPage.getOptValue(lightTresholdPage.getArrowPos());

  bulbTimerPage.setArrowPos(1);
  bulbTimer = bulbTimerPage.getOptValue(bulbTimerPage.getArrowPos());

  screenTimerPage.setArrowPos(2);
  screenTimer = screenTimerPage.getOptValue(screenTimerPage.getArrowPos());

  ///setting baud rate
  Serial.begin(115200);
}

Page* landingPage = &mainMenuPage;
Page* currentPage = landingPage;

void loop()
{
  ///sensor read
  lightValue = analogRead(light); //<store light sensor value
  motionState = digitalRead(motion); //<store motion sensor state

  ///button actions
  if (digitalRead(upButton) && !powerSaveMode) //<if up button is pressed and screen is on
  {
    buttonPressTime = millis(); //<store up button press time
    while (digitalRead(upButton) && (unsigned long)(millis() - buttonPressTime) < 1000) {} //<wait until releasing button or one sec has elapsed
    currentPage->moveArrow(true);
    powerSaveMode = false;
  }
  if (digitalRead(downButton) && !powerSaveMode) //<if down button is pressed and screen is on
  {
    buttonPressTime = millis(); //<store down button press time
    while (digitalRead(downButton) && (unsigned long)(millis() - buttonPressTime) < 1000) {} //<wait until releasing button or one sec has elapsed
    currentPage->moveArrow(false);
    powerSaveMode = false;
  }
  if (digitalRead(okButton)) //<if ok button is pressed
  {
    buttonPressTime = millis(); //<store ok button press time
    while (digitalRead(okButton) && (unsigned long)(millis() - buttonPressTime) < 1000) {} //<wait until releasing button or one sec has elapsed
    if (powerSaveMode) powerSaveMode = false; //<if screen is off, turn it on
    else
    {
      if (currentPage == &mainMenuPage)
      {
        switch (currentPage->getArrowPos())
        {
          case 0: currentPage = &bulbTimerPage; break;
          case 1: currentPage = &lightTresholdPage; break;
          case 2: currentPage = &screenTimerPage; break;
          case 3: currentPage = &monitoringPage; break;
        }        
      }
      else if (currentPage == &bulbTimerPage)
      {
        bulbTimer = currentPage->getOptValue(currentPage->getArrowPos());
        currentPage = &mainMenuPage;
      }
      else if (currentPage == &lightTresholdPage)
      {
        lightTreshold = currentPage->getOptValue(currentPage->getArrowPos());
        currentPage == &mainMenuPage;
      }
      else if (currentPage == &screenTimerPage)
      {
        screenTimer = currentPage->getOptValue(currentPage->getArrowPos());
        currentPage = &mainMenuPage;
      }
      else if (currentPage == &monitoringPage)
      {
        currentPage = &mainMenuPage;
      }
    }
    currentPage->printPage();
  }

  ///refresh if monitoring page
  if (currentPage == &monitoringPage && (unsigned long)(millis() - monitoringRefreshTime) > monitoringRefreshRate * 1000)
  {
    currentPage->refreshPage();
    monitoringRefreshTime = millis();
  }

  ///powerSaveMode activation
  if (!powerSaveMode && (unsigned long)(millis() - buttonPressTime) > screenTimer * 1000) //<if screen is on and enough seconds have elapsed from the last button press
  {
    powerSaveMode = true;
    Screen::clear();
    currentPage = landingPage;
    currentPage->setArrowPos(0);
  }

  ///trigger bulb
  if (motionState) //<if motion has just been detected
  {
    lastTriggerTime = millis(); //<store motion detection time
    if (lastTriggerTime >= 1043) lastTriggerTime -= 1043; //<compensation for continuous sensor signal
    if (lightValue >= lightTreshold) //<if dark (bulb is off, otherwise should not be dark)
    {
      bulb = true;
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  ///turn off bulb
  else if (bulb && (unsigned long)(millis() - lastTriggerTime) > bulbTimer * 1000) //<else if bulb is on and enough seconds have elapsed from the last trigger
  {
    bulb = false;
    digitalWrite(LED_BUILTIN, HIGH);
  }

  ///serial monitoring values
  std::cout << "Bulb: " << (bulb ? "ON" : "OFF")
            << ", Motion: " << (motionState ? "YES" : "NO")
            << ", Time: " << (unsigned long)(millis() - lastTriggerTime) / 1000 << " seconds"
            << ", Light: " << lightValue
            << std::endl;
}
