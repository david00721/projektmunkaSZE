#include <iostream>
#include <Wire.h>
#include "ACROBOTIC_SSD1306.h"
#include "screen.h"

///GPIO names
#define light A0
#define motion 10
#define upButton 12
#define downButton 13
#define okButton 14

///algorythm variables
short screenTimer = 10, bulbTimer = 10, lightTreshold = 512;
bool bulb = false, powerSaveMode = true;
unsigned long lastTriggerTime = 0, buttonPressTime = 0;
short lightValue;
bool motionState;

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
    //DO DOWN BUTTON STUFF
    powerSaveMode = false;
  }
  if (digitalRead(downButton) && !powerSaveMode) //<if down button is pressed and screen is on
  {
    buttonPressTime = millis(); //<store up button press time
    while (digitalRead(downButton) && (unsigned long)(millis() - buttonPressTime) < 1000) {} //<wait until releasing button or one sec has elapsed
    //DO OK BUTTON STUFF
    powerSaveMode = false;
  }
  if (digitalRead(okButton)) //<if ok button is pressed
  {
    buttonPressTime = millis(); //<store up button press time
    while (digitalRead(okButton) && (unsigned long)(millis() - buttonPressTime) < 1000) {} //<wait until releasing button or one sec has elapsed
    if (powerSaveMode) //<if screen is off
    {
      //load MAIN MENU
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
    if (lightValue >= lightTreshold) //<if dark (bulb is off, otherwise should not be dark)
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
