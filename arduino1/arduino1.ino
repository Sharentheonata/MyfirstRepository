const int rLEDPin = 4;
const int gLEDPin = 5;
const int bLEDPin = 6;

void setup() {
  pinMode(rLEDPin, OUTPUT);
  pinMode(gLEDPin, OUTPUT);
  pinMode(bLEDPin, OUTPUT);
}

// Common anode: LOW = nyala, HIGH = mati
void setColor(bool r, bool g, bool b) {
  digitalWrite(rLEDPin, r ? LOW : HIGH);
  digitalWrite(gLEDPin, g ? LOW : HIGH);
  digitalWrite(bLEDPin, b ? LOW : HIGH);
}

void loop() {
  setColor(true, false, false);   // merah
  delay(1000);
  setColor(false, false, false);  // mati
  delay(1000);

  setColor(false, true, false);   // hijau
  delay(1000);
  setColor(false, false, false);  // mati
  delay(1000);

  setColor(false, false, true);   // biru
  delay(1000);
  setColor(false, false, false);  // mati
  delay(1000);
}
}
