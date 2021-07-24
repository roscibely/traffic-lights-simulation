#define PIEZO 1 //Pino Analogico 1
#define SENSOR_CAR 2 // Pino Analogico 2
//#define LED_PIEZO 1 //Pino Digital 1
#define THRESHOLD 30 //Threshold do Piezoeletrico
#define FOTOTRANSISTOR 0 //Pino Analogico 0      
#define BUZZER 2  //Pino Digital 2
/* LEDS para simular o semáforo*/
#define RED1  13 //Pino Digital 13
#define YELLOW1 12 //Pino Digital 12
#define GREEN1 11 //Pino Digital 11

#define PEDESTRIAN_GREEN1 10 //Pino Digital 10
#define PEDESTRIAN_RED1 9 //Pino Digital 9

#define RED2  7 //Pino Digital 7
#define YELLOW2 6 //Pino Digital 6
#define GREEN2 5 //Pino Digital 5

#define RED3  4 //Pino Digital 4
#define YELLOW3 3 //Pino Digital 3
#define GREEN3 8 //Pino Digital 8


int piezo_value = 0;
int value = 0;
int start = 1;
/*--------- TIMERS -------------- */
int GREEN2time;
int RED2time = 3000;
int YELLOW2time = 3000; //3s
/*-------VARIAVEIS DE CONTROLE------- */
int inicio = 1;
int count_v = 0; //Contador de veículos
int S_via_second = 3150; //Fluxo de Saturação veq/htv da via secundaria
int S_via_prim = 4725; //Fluxo de Saturação veq/htv da via primaria
int q = 200; //Demanda da aproximação
int y; //Taxa de ocupação
int Sum_Y = 0; //Somatorio das taxas de ocupação
unsigned long Co; //Tempo de ciclo ótimo;
unsigned long Tp = 3000;
unsigned long I = 3000; //Tempo perdido inicial
unsigned long gef = 3000; //Tempo de Verde efetivo 8s inciais;
unsigned long g = 3000;
unsigned long t1, t2, tempo; //Variaveis para calculo de velocidade do veiculo;
unsigned long timer = 0;

void pedestre() {
  value = analogRead(FOTOTRANSISTOR);
  piezo_value = analogRead(PIEZO); //Sensor piezo para detectar o peso do pedestre
  Serial.print(value);
  Serial.println("PEDESTRE");
  if (value >= 300 || piezo_value >= THRESHOLD) {
    digitalWrite(GREEN1, LOW);
    digitalWrite(GREEN2, HIGH);
    digitalWrite(GREEN3, LOW);
    digitalWrite(PEDESTRIAN_GREEN1, HIGH);
    digitalWrite(PEDESTRIAN_RED1, LOW);
    digitalWrite(YELLOW1, LOW);
    digitalWrite(YELLOW2, LOW);
    digitalWrite(YELLOW3, LOW);
    digitalWrite(RED2, LOW);
    digitalWrite(RED3, HIGH);
    digitalWrite(RED1, HIGH);
    //  Serial.println("INRERRUPCAO");
    RED2time = RED2time + 200; //7s é tempo dado para o intervalo de caminhada para o pedestre
    //Serial.print("RED"); Serial.println(RED2time);
    YELLOW2time = YELLOW2time + 100; //É adcionado um tempo de segurança a mais no amarelo
    g = g - 100;
  }
}

void setup() {
  attachInterrupt(digitalPinToInterrupt(2), pedestre, HIGH);
  pinMode(BUZZER, INPUT);
  pinMode(GREEN1, OUTPUT);
  pinMode(RED1, OUTPUT);
  pinMode(YELLOW1, OUTPUT);
  pinMode(GREEN2, OUTPUT);
  pinMode(RED2, OUTPUT);
  pinMode(YELLOW2, OUTPUT);
  pinMode(GREEN3, OUTPUT);
  pinMode(RED3, OUTPUT);
  pinMode(YELLOW3, OUTPUT);
  pinMode(PEDESTRIAN_GREEN1, OUTPUT);
  pinMode(PEDESTRIAN_RED1, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  /*Loop inicial*/
  while (start) {
    Serial.println("INICIO");
    digitalWrite(GREEN1, LOW);
    digitalWrite(GREEN2, HIGH);
    digitalWrite(GREEN3, LOW);
    digitalWrite(PEDESTRIAN_GREEN1, LOW);
    digitalWrite(PEDESTRIAN_RED1, HIGH);
    digitalWrite(YELLOW1, HIGH);
    digitalWrite(YELLOW2, HIGH);
    digitalWrite(YELLOW3, HIGH);
    delay(1000);
    digitalWrite(YELLOW1, LOW);
    digitalWrite(YELLOW2, LOW);
    digitalWrite(YELLOW3, LOW);
    delay(1000);
    digitalWrite(YELLOW1, HIGH);
    digitalWrite(YELLOW2, LOW);
    digitalWrite(YELLOW3, HIGH);
    delay(1000);
    digitalWrite(YELLOW1, LOW);
    digitalWrite(GREEN2, LOW);
    digitalWrite(YELLOW2, HIGH);
    digitalWrite(YELLOW3, LOW);
    digitalWrite(RED1, HIGH);
    digitalWrite(RED2, LOW);
    digitalWrite(RED3, HIGH);
    digitalWrite(PEDESTRIAN_GREEN1, HIGH);
    digitalWrite(PEDESTRIAN_RED1, LOW);
    delay(3000);
    digitalWrite(RED1, LOW);
    digitalWrite(RED2, HIGH);
    digitalWrite(RED3, LOW);
    digitalWrite(PEDESTRIAN_GREEN1, LOW);
    digitalWrite(PEDESTRIAN_RED1, HIGH);
    digitalWrite(GREEN1, HIGH);
    digitalWrite(GREEN3, LOW);
    delay(1000); //10s
    start = 0;
  }

  int sensor =  analogRead(SENSOR_CAR);
  value = analogRead(FOTOTRANSISTOR);
  if (sensor > THRESHOLD) { //Passou um veiculo
    g = g + 3000;
    count_v = count_v + 20; //q: demanda ou quantidade de veiculos
    //q = count_v;
    Serial.println("Demanda" + count_v);
  }
  /*Calculo dos parametros do plano de tráfego*/
  g = gef;
  gef = g + YELLOW2time - I;
  Serial.print("Verde Efetivo:"); Serial.println(gef);
  if (gef > 9000) {
    gef = 6000;
  }
  I = (g + YELLOW2time) - gef;
  Serial.print("Tempo Perdido I :"); Serial.println(I);
  Tp = Tp + (I - YELLOW2time);
  Serial.print("Tp:"); Serial.println(Tp);
  y = q * S_via_second; //q é damanda
  Serial.print("y:"); Serial.println(y);
  Sum_Y = Sum_Y + y;
  Co = (1.5 * Tp + 5) / (1 - Sum_Y);
  Serial.print("Ciclo otimo :"); Serial.println(Co);
  RED2time = 3000;
  YELLOW2time = 3000;

  if (gef * S_via_second >= q * Co || gef >= y * Co) {
    unsigned long t = millis();
    while (timer < t + 3000) { //Enquanto não tiver passado os 30s
      Serial.print("Verde: "); Serial.println(g);
      Serial.print("Vermelho: "); Serial.println(RED2time);
      Serial.print("Amarelo: "); Serial.println(YELLOW2time);
      Serial.println("Semaforo 1");
      digitalWrite(GREEN1, HIGH);
      digitalWrite(GREEN2, LOW);
      digitalWrite(GREEN3, LOW);
      digitalWrite(PEDESTRIAN_GREEN1, LOW);
      digitalWrite(PEDESTRIAN_RED1, HIGH);
      digitalWrite(YELLOW1, LOW);
      digitalWrite(YELLOW2, LOW);
      digitalWrite(YELLOW3, LOW);
      digitalWrite(RED2, HIGH);
      digitalWrite(RED3, HIGH);
      digitalWrite(RED1, LOW);
      delay(g);

      digitalWrite(GREEN1, LOW);
      digitalWrite(GREEN2, LOW);
      digitalWrite(GREEN3, LOW);
      digitalWrite(PEDESTRIAN_GREEN1, LOW);
      digitalWrite(PEDESTRIAN_RED1, HIGH);
      digitalWrite(YELLOW1, HIGH);
      digitalWrite(YELLOW2, LOW);
      digitalWrite(YELLOW3, LOW);
      digitalWrite(RED2, HIGH);
      digitalWrite(RED3, HIGH);
      digitalWrite(RED1, LOW);
      delay(YELLOW2time);

      Serial.println("Semaforo 2");
      digitalWrite(GREEN1, LOW);
      digitalWrite(GREEN2, HIGH);
      digitalWrite(GREEN3, LOW);
      digitalWrite(PEDESTRIAN_GREEN1, HIGH);
      digitalWrite(PEDESTRIAN_RED1, LOW);
      digitalWrite(YELLOW1, LOW);
      digitalWrite(YELLOW2, LOW);
      digitalWrite(YELLOW3, LOW);
      digitalWrite(RED2, LOW);
      digitalWrite(RED3, HIGH);
      digitalWrite(RED1, HIGH);
      delay(RED2time);

      digitalWrite(GREEN1, LOW);
      digitalWrite(GREEN2, LOW);
      digitalWrite(GREEN3, LOW);
      digitalWrite(PEDESTRIAN_GREEN1, HIGH);
      digitalWrite(PEDESTRIAN_RED1, LOW);
      digitalWrite(YELLOW1, LOW);
      digitalWrite(YELLOW2, HIGH);
      digitalWrite(YELLOW3, LOW);
      digitalWrite(RED2, LOW);
      digitalWrite(RED3, HIGH);
      digitalWrite(RED1, HIGH);
      delay(YELLOW2time);

      Serial.println("Semaforo 3");
      digitalWrite(GREEN1, LOW);
      digitalWrite(GREEN2, LOW);
      digitalWrite(GREEN3, HIGH);
      digitalWrite(PEDESTRIAN_GREEN1, HIGH);
      digitalWrite(PEDESTRIAN_RED1, LOW);
      digitalWrite(YELLOW1, LOW);
      digitalWrite(YELLOW2, LOW);
      digitalWrite(YELLOW3, LOW);
      digitalWrite(RED2, HIGH);
      digitalWrite(RED3, LOW);
      digitalWrite(RED1, HIGH);
      delay(RED2time);

      digitalWrite(GREEN1, LOW);
      digitalWrite(GREEN2, LOW);
      digitalWrite(GREEN3, LOW);
      digitalWrite(PEDESTRIAN_GREEN1, HIGH);
      digitalWrite(PEDESTRIAN_RED1, LOW);
      digitalWrite(YELLOW1, LOW);
      digitalWrite(YELLOW2, LOW);
      digitalWrite(YELLOW3, HIGH);
      digitalWrite(RED2, HIGH);
      digitalWrite(RED3, LOW);
      digitalWrite(RED1, HIGH);
      delay(YELLOW2time);

      timer = millis();
      Serial.print("Time: "); Serial.println(timer);
    }
  }
}





