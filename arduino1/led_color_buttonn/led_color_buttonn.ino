const int buttonPin = 2;
const int RledPin = 3;
const int GledPin = 4;
const int BledPin = 5;

const bool COMMON_ANODE = true;
const int NUM_COLORS = 6;

const byte colors[NUM_COLORS][3] = {
  {1, 0, 0},
  {0, 1, 0},
  {0, 0, 1},
  {1, 1, 0},
  {1, 0, 1},
  {0, 1, 1}
};

int buttonState = LOW;
int lastButtonState = LOW;
int ledcolor = 0;
bool autoMode = false;
unsigned long lastPress = 0;
unsigned long lastChange = 0;

void writeLed(int pin, byte on) {
  if (COMMON_ANODE) digitalWrite(pin, on ? LOW : HIGH);
  else digitalWrite(pin, on ? HIGH : LOW);
}

void showColor(int index) {
  writeLed(RledPin, colors[index][0]);
  writeLed(GledPin, colors[index][1]);
  writeLed(BledPin, colors[index][2]);
}

void setup() {
  writeLed(RledPin, 0);
  writeLed(GledPin, 0);
  writeLed(BledPin, 0);

  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  showColor(ledcolor);
}

void loop() {
  if (!autoMode) {
    buttonState = digitalRead(buttonPin);

    if (buttonState == HIGH && lastButtonState == LOW && millis() - lastPress > 30) {
      lastPress = millis();
      ledcolor++;

      if (ledcolor >= NUM_COLORS) {
        autoMode = true;
        ledcolor = 0;
        lastChange = millis();
      }
      showColor(ledcolor);
    }

    lastButtonState = buttonState;
  }
  else if (millis() - lastChange >= 1000) {
    lastChange += 1000;
    ledcolor = (ledcolor + 1) % NUM_COLORS;
    showColor(ledcolor);
  }
}