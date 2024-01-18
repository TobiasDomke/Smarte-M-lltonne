int echoPin1 = 8;
int trigPin1 = 9;
int echoPin2 = 10;
int trigPin2 = 11;
long laufzeit, entfernung;
int LED = 7;
int Benachrichtigung = 12;
int counter = 0;
int sensorwert = 0;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(Benachrichtigung, OUTPUT);
}

int func_distanz(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  laufzeit = pulseIn(echoPin, HIGH);
  entfernung = laufzeit * 0.017;
  //Serial.println(entfernung);
  //Serial.println(sensor);

  return entfernung;
}

void loop() {
  //Einlesen von Widerstand für Kallibrierung
  sensorwert = analogRead(A0);
  // Serial.print(sensorwert);
  int data1 = func_distanz(trigPin1, echoPin1);
  int data2 = func_distanz(trigPin2, echoPin2);
  // Serial.print("(");
  // Serial.print(data1);
  // Serial.print(" + ");
  // Serial.print(data2);
  // Serial.print(")/2 = ");
  // Serial.print((data1 + data2) / 2);
  // Serial.print("\n");

  if ((data1 + data2) / 2 < (sensorwert / 10)) {
    digitalWrite(LED, HIGH);
    counter = counter + 1;
  } else {
    digitalWrite(LED, LOW);
    counter = 0;
  }
  if (counter > 1000) {
    digitalWrite(Benachrichtigung, HIGH);
    Serial.print("Mülleimer voll");

    delay(1500);
  } else {
    digitalWrite(Benachrichtigung, LOW);
  }
  delayMicroseconds(50);
}