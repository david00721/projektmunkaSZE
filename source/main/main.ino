#include <iostream>
#include <Wire.h>
#include "ACROBOTIC_SSD1306.h"
#include "buttons.h"

///GPIO names
#define light A0
#define motion 10
#define upButton 12
#define downButton 13
#define okButton 14

///algorythm variables
short screenTimer = 10;
bool bulb = false, powerSaveMode = true;
unsigned long lastTriggerTime = 0, buttonPressTime = 0;
int lightValue;
bool motionState;

///menu pages
MenuPage mainMenuPage = MenuPage("MAIN MENU");
MenuPage settingsPage = MenuPage("SETTINGS", &mainMenuPage);
MenuPage monitoringPage = MenuPage("MONITORING", &mainMenuPage);
MenuPage logsPage = MenuPage("LOGS", &mainMenuPage);

///main menu items
Item settingsItem = Item("Settings", &settingsPage);
Item monitoringItem = Item("Monitoring", &monitoringPage);
Item logsItem = Item("Logs", &monitoringPage);

///settings menu pages
SliderPage bulbTimerPage = SliderPage("BULB TIMER", &settingsPage, Spectrum(5, 100, 5));
TextOptsPage lightTresholdPage = TextOptsPage("LIGHT TRESHOLD", &settingsPage);

///settings menu items
ValueItem bulbTimerItem = ValueItem("Bulb timer", &bulbTimerPage, 10, " sec");
ValueItem lightTresholdItem = ValueItem("Light treshold", &lightTresholdPage, 256);

///page pointers
Page* landingPage = &mainMenuPage;
Page* currentPage = landingPage;

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

  ///setting baud rate
  Serial.begin(115200);

  ///main menu fill
  mainMenuPage.addItem(&settingsItem);
  mainMenuPage.addItem(&monitoringItem);
  mainMenuPage.addItem(&logsItem);

  ///settings menu fill
  settingsPage.addItem(&bulbTimerItem);
  settingsPage.addItem(&lightTresholdItem);

  ///light treshold options fill
  lightTresholdPage.addOpt("Low light", 256);
  lightTresholdPage.addOpt("Medium light", 512);
  lightTresholdPage.addOpt("High light", 768);
}

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
    upButtonAction(currentPage);
    powerSaveMode = false;
  }
  if (digitalRead(downButton) && !powerSaveMode) //<if down button is pressed and screen is on
  {
    buttonPressTime = millis(); //<store up button press time
    while (digitalRead(downButton) && (unsigned long)(millis() - buttonPressTime) < 1000) {} //<wait until releasing button or one sec has elapsed
    downButtonAction(currentPage);
    powerSaveMode = false;
  }
  if (digitalRead(okButton)) //<if ok button is pressed
  {
    buttonPressTime = millis(); //<store up button press time
    while (digitalRead(okButton) && (unsigned long)(millis() - buttonPressTime) < 1000) {} //<wait until releasing button or one sec has elapsed
    if (powerSaveMode) //<if screen is off
    {
      landingPage->loadPage(); //<load landing page
      currentPage = landingPage; //<set landing page as current page
    }
    else currentPage = okButtonAction(currentPage); //<else (if screen is on), execute normal 'ok' button action
    powerSaveMode = false;
  }

  ///powerSaveMode activation
  if (!powerSaveMode && (unsigned long)(millis() - buttonPressTime) > screenTimer * 1000) //<if screen is on and enough seconds have elapsed from the last button press
  {
    powerSaveMode = true;
    Screen::clear();
  }

  ///trigger bulb
  if (motionState) //<if motion has just been detected
  {
    lastTriggerTime = millis(); //<store motion detection time
    if (lastTriggerTime >= 1043) lastTriggerTime -= 1043; //<compensation for continuous sensor signal
    if (lightValue <= lightTresholdItem.getValue()) //<if dark (bulb is off, otherwise should not be dark)
    {
      bulb = true;
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  ///turn off bulb
  else if (bulb && (unsigned long)(millis() - lastTriggerTime) > bulbTimerItem.getValue() * 1000) //<else if bulb is on and enough seconds have elapsed from the last trigger
  {
    bulb = false;
    digitalWrite(LED_BUILTIN, HIGH);
  }

  ///serial monitoring values
  std::cout << "Bulb: " << (bulb ? "ON" : "OFF")
            << ", Motion: " << (motionState ? "YES" : "NO")
            << ", Time: " << (millis() - lastTriggerTime) / 1000 << " seconds"
            << ", Light: " << lightValue
            << std::endl;
}
