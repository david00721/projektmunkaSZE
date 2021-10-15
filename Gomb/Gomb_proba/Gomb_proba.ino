
int button = D6; // push button is connected d6

void setup() {
  Serial.begin(115200);
  pinMode(button, INPUT); // declare push button as input
}

void loop() {
     int kapcs=digitalRead(button);
     Serial.println(kapcs);
     delay(150);   
}
