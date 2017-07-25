int yellow = 12;
int green = 7;
int red = 13;
int blue = 8;

int buttonPin = 30;
int buttonState = 0;

void setup() {                
  pinMode(yellow, OUTPUT); 
  pinMode(green, OUTPUT);  
  pinMode(green, OUTPUT); 
  pinMode(blue, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void blinking() {
  digitalWrite(yellow, HIGH);  
  delay(2000);               
  digitalWrite(yellow, LOW); 
  digitalWrite(green, HIGH); 
  delay(2000);  
  digitalWrite(green, LOW);
  digitalWrite(red, HIGH);
  delay(2000); 
  digitalWrite(red, LOW);
  digitalWrite(blue, HIGH);
  delay(2000); 
  digitalWrite(blue, LOW);
  delay(2000);
}

void fadeUp(int Pin) {
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) { 
    analogWrite(Pin, fadeValue);            
    delay(30);                            
  }
}

void fadeDown(int Pin) {
  for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) { 
    analogWrite(Pin, fadeValue);            
    delay(30);                            
  }
}

void Fading() {
  fadeUp(yellow);
  fadeDown(yellow);
  delay(30);
  fadeUp(green);
  fadeDown(green);
  delay(30); 
  fadeUp(red);
  fadeDown(red);
  delay(30);
  fadeUp(blue);
  fadeDown(blue);
  delay(30);
}

void loop() {
  //blinking();
  //Fading();
}
