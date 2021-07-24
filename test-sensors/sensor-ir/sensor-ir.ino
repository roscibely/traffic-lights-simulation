// Example Code
int InputPin = 0;      // analog pin 0 is the input pin
//int Buzzer = 2;   // digital pin 1 is the output pin
int val = 0;              // variable to store the value read

void setup()
{
  Serial.begin(9600);
//pinMode(Buzzer, OUTPUT);      // sets the digital pin as output
}

void loop(){
  int b=0;
  //digitalWrite(Buzzer, LOW);    // sets the output pin initially to LOW
  val = analogRead(InputPin);        // read the input pin 0 to 1023
  //Serial.println(val);
  if (val > 400){                            // if sensor value is below threshold set output HIGH
  //digitalWrite(Buzzer, HIGH);    // sets output pin HIGH to activate special effects
 // delay(1000);  // waits for a second
 b=1;
 Serial.println(b);
}
Serial.println(b);

}

