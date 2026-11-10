const int buttonPin = 2;
const int PRESSED = HIGH;
const int RLedPin = 9;
const int GLedPin = 10;
const int BLedPin = 11;

int buttonState = !PRESSED;
bool buttonPressed = false;
unsigned long pressingTime = 0;
const int longPressInterval = 1500;
const int debounceTime = 30;
int lastReading = !PRESSED;
unsigned long lastChangeTime = 0;
bool longPressDone = false;

int currentMode = 0;
const int modeCount = 5;
const int secretMode = 5;

int lightNum = 0;
const int colorCount = 7;
int RLightColor = 0;
int GLightColor = 0;
int BLightColor = 0;
int RCurrentColor = 0;
int GCurrentColor = 0;
int BCurrentColor = 0;

unsigned long blinkTimer = 0;
const int blinkInterval = 500;
bool blinkOn = true;

const int fadeAmount = 2;
int fadeDirection = 1;

int rainbowHue = 0;
unsigned long rainbowTimer = 0;
int rainbowSpeedLevel = 0;
const int rainbowSpeeds[3] = {30, 12, 4};

unsigned long heartbeatStart = 0;
const int heartbeatPeriod = 1000;

int clickCount = 0;
unsigned long lastClickTime = 0;
const int secretClicks = 5;
const unsigned long clickGap = 500;

void setup()
{
  pinMode(buttonPin, INPUT);
  pinMode(RLedPin, OUTPUT);
  pinMode(GLedPin, OUTPUT);
  pinMode(BLedPin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  checkButton();
  updateLEDColor();
  setRGBLEDColor(RCurrentColor, GCurrentColor, BCurrentColor);
}

void checkButton(){
  int reading = digitalRead(buttonPin);

  if(reading != lastReading){
    lastChangeTime = millis();
    lastReading = reading;
  }
  if(millis() - lastChangeTime > debounceTime){
    buttonState = reading;
  }
  else{
    return;
  }

  if(buttonState == PRESSED && !buttonPressed){
    pressingTime = millis();
    buttonPressed = true;
    longPressDone = false;
  }

  if(buttonState == PRESSED && buttonPressed && !longPressDone){
    if(millis() - pressingTime >= longPressInterval){
      Serial.println("long press");
      longPressFeedback();
      changeMode();
      longPressDone = true;
      clickCount = 0;
    }
  }

  if(buttonState != PRESSED && buttonPressed){
    if(!longPressDone){
      Serial.println("short click");
      shortClick();
    }
    buttonPressed = false;
  }
}

void shortClick(){
  if(millis() - lastClickTime < clickGap){
    clickCount = clickCount + 1;
  }
  else{
    clickCount = 1;
  }
  lastClickTime = millis();

  if(clickCount >= secretClicks && currentMode != secretMode){
    enterSecretMode();
    clickCount = 0;
  }
  else if(currentMode == 3){
    changeRainbowSpeed();
  }
  else if(currentMode != secretMode){
    changeLEDColor();
  }
}

void enterSecretMode(){
  currentMode = secretMode;
  Serial.println("SECRET MODE: fireworks");
}

void longPressFeedback(){
  for(int i = 0; i < 2; i++){
    setRGBLEDColor(255, 255, 255);
    delay(80);
    setRGBLEDColor(0, 0, 0);
    delay(80);
  }
  setRGBLEDColor(255, 255, 255);
  delay(80);
}

void changeLEDColor(){
  lightNum = lightNum + 1;
  if(lightNum >= colorCount)
    lightNum = 0;

  if(lightNum == 0){
    RLightColor = 0;   GLightColor = 0;   BLightColor = 0;
  }
  if(lightNum == 1){
    RLightColor = 0;   GLightColor = 255; BLightColor = 255;
  }
  if(lightNum == 2){
    RLightColor = 255; GLightColor = 0;   BLightColor = 255;
  }
  if(lightNum == 3){
    RLightColor = 255; GLightColor = 255; BLightColor = 0;
  }
  if(lightNum == 4){
    RLightColor = 0;   GLightColor = 0;   BLightColor = 255;
  }
  if(lightNum == 5){
    RLightColor = 0;   GLightColor = 255; BLightColor = 0;
  }
  if(lightNum == 6){
    RLightColor = 255; GLightColor = 0;   BLightColor = 0;
  }

  RCurrentColor = RLightColor;
  GCurrentColor = GLightColor;
  BCurrentColor = BLightColor;
}

void changeMode(){
  currentMode = currentMode + 1;
  if(currentMode >= modeCount){
    currentMode = 0;
  }
  Serial.print("mode: ");
  Serial.println(currentMode);

  if(currentMode == 1){
    blinkTimer = 0;
    blinkOn = true;
  }
  if(currentMode == 2){
    RCurrentColor = RLightColor;
    GCurrentColor = GLightColor;
    BCurrentColor = BLightColor;
    fadeDirection = 1;
  }
  if(currentMode == 3){
    rainbowHue = 0;
    rainbowTimer = millis();
  }
  if(currentMode == 4){
    heartbeatStart = millis();
  }
}

void changeRainbowSpeed(){
  rainbowSpeedLevel = rainbowSpeedLevel + 1;
  if(rainbowSpeedLevel >= 3)
    rainbowSpeedLevel = 0;
  Serial.print("rainbow speed: ");
  Serial.println(rainbowSpeedLevel);
}

void updateLEDColor(){
  if(currentMode == 0){
    RCurrentColor = RLightColor;
    GCurrentColor = GLightColor;
    BCurrentColor = BLightColor;
  }
  else if(currentMode == 1){
    unsigned long currentTime = millis();
    if(currentTime - blinkTimer > blinkInterval){
      blinkOn = !blinkOn;
      blinkTimer = currentTime;
    }

    if(blinkOn){
      RCurrentColor = RLightColor;
      GCurrentColor = GLightColor;
      BCurrentColor = BLightColor;
    }
    else{
      RCurrentColor = 255;
      GCurrentColor = 255;
      BCurrentColor = 255;
    }
  }
  else if(currentMode == 2){
    bool negativeDir = false;
    if(RLightColor == 0){
      RCurrentColor = RCurrentColor + fadeDirection * fadeAmount;
      if(RCurrentColor > (255 - RLightColor) || (RCurrentColor < 0)){
        negativeDir = true;
      }
      if(RCurrentColor < 0) RCurrentColor = 0;
      if(RCurrentColor > 255) RCurrentColor = 255;
    }
    if(GLightColor == 0){
      GCurrentColor = GCurrentColor + fadeDirection * fadeAmount;
      if(GCurrentColor > (255 - GLightColor) || (GCurrentColor < 0)){
        negativeDir = true;
      }
      if(GCurrentColor < 0) GCurrentColor = 0;
      if(GCurrentColor > 255) GCurrentColor = 255;
    }
    if(BLightColor == 0){
      BCurrentColor = BCurrentColor + fadeDirection * fadeAmount;
      if(BCurrentColor > (255 - BLightColor) || (BCurrentColor < 0)){
        negativeDir = true;
      }
      if(BCurrentColor < 0) BCurrentColor = 0;
      if(BCurrentColor > 255) BCurrentColor = 255;
    }
    if(negativeDir)
      fadeDirection = -fadeDirection;

    delay(10);
  }
  else if(currentMode == 3){
    if(millis() - rainbowTimer > (unsigned long)rainbowSpeeds[rainbowSpeedLevel]){
      rainbowHue = (rainbowHue + 1) % 360;
      rainbowTimer = millis();
    }
    setRainbowColor(rainbowHue);
  }
  else if(currentMode == 4){
    unsigned long t = (millis() - heartbeatStart) % heartbeatPeriod;
    float level = 0;
    if(t < 400)              level = exp(-(float)t / 70.0);
    if(t >= 250 && t < 650)  level = max(level, (float)(0.6 * exp(-(float)(t - 250) / 70.0)));

    RCurrentColor = 255 - (int)((255 - RLightColor) * level);
    GCurrentColor = 255 - (int)((255 - GLightColor) * level);
    BCurrentColor = 255 - (int)((255 - BLightColor) * level);
  }
  else if(currentMode == secretMode){
    static unsigned long burstStart = 0;
    static unsigned long sparkleTimer = 0;
    static bool sparkle = false;
    static int fr = 255, fg = 0, fb = 0;
    static int lastColor = -1;
    unsigned long t = millis() - burstStart;

    if(t >= 1200){
      burstStart = millis();
      t = 0;
      int c = random(0, 7);
      while(c == lastColor){
        c = random(0, 7);
      }
      lastColor = c;
      if(c == 0){ fr = 255; fg = 0;   fb = 0;   }
      if(c == 1){ fr = 0;   fg = 255; fb = 0;   }
      if(c == 2){ fr = 0;   fg = 0;   fb = 255; }
      if(c == 3){ fr = 255; fg = 180; fb = 0;   }
      if(c == 4){ fr = 180; fg = 0;   fb = 255; }
      if(c == 5){ fr = 0;   fg = 255; fb = 255; }
      if(c == 6){ fr = 255; fg = 0;   fb = 120; }
    }

    if(millis() - sparkleTimer > 40){
      sparkleTimer = millis();
      sparkle = (random(0, 10) < 3);
    }

    if(t < 60){
      RCurrentColor = 0; GCurrentColor = 0; BCurrentColor = 0;
    }
    else if(t < 1000){
      float level = exp(-(float)(t - 60) / 250.0);
      if(sparkle && t > 200){
        level = level * 0.3;
      }
      RCurrentColor = 255 - (int)(fr * level);
      GCurrentColor = 255 - (int)(fg * level);
      BCurrentColor = 255 - (int)(fb * level);
    }
    else{
      RCurrentColor = 255; GCurrentColor = 255; BCurrentColor = 255;
    }
  }
}

void setRainbowColor(int hue){
  int r, g, b;
  int x = (hue % 60) * 255 / 60;
  if(hue < 60)       { r = 255;     g = x;       b = 0; }
  else if(hue < 120) { r = 255 - x; g = 255;     b = 0; }
  else if(hue < 180) { r = 0;       g = 255;     b = x; }
  else if(hue < 240) { r = 0;       g = 255 - x; b = 255; }
  else if(hue < 300) { r = x;       g = 0;       b = 255; }
  else               { r = 255;     g = 0;       b = 255 - x; }

  RCurrentColor = 255 - r;
  GCurrentColor = 255 - g;
  BCurrentColor = 255 - b;
}

void setRGBLEDColor(int r, int g, int b){
  analogWrite(RLedPin, r);
  analogWrite(GLedPin, g);
  analogWrite(BLedPin, b);
}