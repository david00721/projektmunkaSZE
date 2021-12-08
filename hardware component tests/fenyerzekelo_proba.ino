#define LDR A0

void setup() 
{
  Serial.begin(115200);
  delay(100);
 
  
}

void loop() 
{
  int light = analogRead(A0);
  Serial.println(light);
  delay(1000);
} 

