#include <iostream>
using namespace std;

int button1 = 2; // push button is connected d6
int button2 = 12;

void setup() {
  Serial.begin(115200);
  pinMode(button1, INPUT); // declare push button as input
  pinMode(button2, INPUT);
}

void loop() {
     int kapcs1=digitalRead(button1);
     int kapcs2=digitalRead(button2);
     cout << "Elso szar: " << kapcs1 << " Masodik szar: " << kapcs2 <<endl;
     delay(150);   
}
