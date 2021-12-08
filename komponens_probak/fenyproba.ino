void setup(){
  Serial.begin(9600);
  pinMode(15, INPUT);
  
  }

void loop(){
  int ldrStatus = digitalRead(15);
  Serial.println(ldrStatus);
  delay(200);
  }
