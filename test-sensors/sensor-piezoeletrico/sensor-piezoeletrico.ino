const int Piezo = 1;
int InputPin = 0;      // analog pin 0 is the input pin
const int ledPin = 13;
const int threshold = 30;
int val = 0;              // variable to store the value read
int a;
String str = "";
void setup() {
  //pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int valP = analogRead(Piezo);
  val = analogRead(InputPin);
  Serial.println(valP);
  if (val > 400) {
    a = 1;
  }
  else if(val<400){
  a=0;
  }
  if (valP >= threshold) {
    //digitalWrite(ledPin, HIGH);
    // delay(1000);

  }
 // Serial.flush();
delay(100);

 // str += a;
  //str += " ";
 // str += valP;
  //Serial.println(str);
 // delay(5);
 // Serial.flush();
 // str = "";

}
