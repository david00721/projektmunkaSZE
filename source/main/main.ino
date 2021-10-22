#include <Wire.h>
#include "ACROBOTIC_SSD1306.h"
#include "buttons.h"
#define light A0
#define motion 13

int upButton = 2;
int downButton = 12;
int okButton = 10;

int lsth=600;
bool lampa=false;
unsigned long currentTime, prevTime = 0;
int timer = 10000;


MenuPage mainMenuPage = MenuPage("MAIN MENU");
MenuPage settingsPage = MenuPage("SETTINGS", &mainMenuPage);
MenuPage monitoringPage = MenuPage("MONITORING", &mainMenuPage);
MenuPage logsPage = MenuPage("LOGS", &mainMenuPage);

Item settingsItem = Item("Settings", &settingsPage);
Item monitoringItem = Item("Monitoring", &monitoringPage);
Item logsItem = Item("Logs", &monitoringPage);

//mainMenuPage.addItem(&settingsItem);
//mainMenuPage.addItem(&monitoringItem);
//mainMenuPage.addItem(&logsItem);

Screen screen;
bool firstrun=true;
void setup()
{
  Wire.begin();
  oled.init();
  oled.clearDisplay();
  
  pinMode(upButton, INPUT);
  pinMode(downButton, INPUT);
  pinMode(okButton, INPUT);

    pinMode(motion, INPUT); // declare sensor as input
    //pinMode(light, INPUT);
  
  Serial.begin(115200);
}

void loop()
{
  if(firstrun)
  {
    delay(5000);
    firstrun=false;
  }
   int light_value = analogRead(light);
   int state = digitalRead(motion);

  /*
  int okButtonState = digitalRead(okButton);
  if (okButtonState == 1)
  {
    mainMenuPage.loadPage(&screen);
  }
  */
  currentTime = millis();
  if(light_value>=lsth && state==1){
    prevTime = currentTime;
    if(!lampa)lampa=true;
  }
  else if(lampa && currentTime - prevTime > timer) lampa=false;

  
  if(lampa)Serial.println("on");
  else Serial.println("off");
  

  Serial.println(light_value);
  Serial.println(state);
  delay(2000);
}
