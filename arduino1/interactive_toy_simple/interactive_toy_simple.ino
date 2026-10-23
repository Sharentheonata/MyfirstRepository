int redPin = 9;
int greenPin = 10;
int bluePin = 11;
int buttonPin = 2;
int lightPin = A0;

bool useLight = true;     
int darkValue = 600;      
int veryDarkValue = 850;   

int mood = 10;
bool lastButton = false;
unsigned long lastPress = 0;
unsigned long lastDecay = 0;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  lastPress = millis();
  lastDecay = millis();
}

void setColor(int r, int g, int b) {
  analogWrite(redPin, 255 - r);
  analogWrite(greenPin, 255 - g);
  analogWrite(bluePin, 255 - b);
}

void showMood() {
  int r, g, b;
  if (mood >= 10) {
    r = 0;
    g = map(mood, 10, 20, 255, 0);
    b = map(mood, 10, 20, 0, 255);
  } else {
    r = map(mood, 0, 10, 255, 0);
    g = map(mood, 0, 10, 0, 255);
    b = 0;
  }
  setColor(r, g, b);
}

void loop() {
  bool pressed = (digitalRead(buttonPin) == HIGH);

  if (pressed && !lastButton) {
    if (mood < 20) {
      mood = mood + 1;
    }
    lastPress = millis();
    lastDecay = millis();
    Serial.println(mood);
  }
  lastButton = pressed;

  int decay = 0;
  if (millis() - lastPress > 5000) {
    decay = 1;
  }

  if (useLight) {
    int light = analogRead(lightPin);
    if (light > veryDarkValue) {
      decay = 2;
    } else if (light > darkValue) {
      decay = 1;
    }
  }

  if (decay > 0 && millis() - lastDecay >= 1000) {
    lastDecay = millis();
    mood = mood - decay;
    if (mood < 0) {
      mood = 0;
    }
    Serial.println(mood);
  }

  showMood();
}
