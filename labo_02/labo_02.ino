unsigned long currentTime = 0;
const int btnPin = 2;
const int ledPins[] = { 8, 9, 10, 11 };
const int potentiometerPin = A1;

int potentiometerValue = 0;
int ledIndex = 0;  // Index du DEL allume


unsigned long serialPrevious = 0;
int serialDelay = 1000;

int mappedIndex = 0;
int intervalle = 20;


void setup() {
  Serial.begin(9600);
  pinMode(btnPin, INPUT_PULLUP);
  for (int i = 0; i < 4; i++) {

    pinMode(ledPins[i], OUTPUT);
  }

  Serial.println("Setup completed");
}

int estClic(unsigned long ct) {
  static unsigned long lastTime = 0;
  static int lastState = HIGH;
  const int rate = 50;
  int clic = 0;

  if (ct - lastTime < rate) {
    return clic;  // Trop rapide
  }

  lastTime = ct;

  int state = digitalRead(btnPin);

  if (state == LOW) {
    if (lastState == HIGH) {
      clic = 1;
    }
  }
  lastState = state;
  return clic;
}

void affichage(int valeur) {
  int progression = map(valeur, 0, 1023, 0, 100);
  Serial.print(progression);
  Serial.print("%[");
  for (int i = 0; i < intervalle; i++) {
    Serial.print(i < progression / 5 ? "@" : ".");
  }
  Serial.println("]");
}

void loop() {
  int nbrLeds = 4;
  currentTime = millis();
  potentiometerValue = analogRead(potentiometerPin);
  mappedIndex = map(potentiometerValue, 0, 1023, 0, 100);

  for (int i = 0; i < nbrLeds; i++) {
    if (mappedIndex >= i * 25) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }

  if (currentTime - serialPrevious >= serialDelay && estClic(currentTime)) {
    serialPrevious = currentTime;
    affichage(potentiometerValue);
  }
}