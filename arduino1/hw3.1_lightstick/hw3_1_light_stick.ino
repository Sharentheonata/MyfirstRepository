const int buttonPin = 2;
const int RLedPin = 9;
const int GLedPin = 10;
const int BLedPin = 11;

int buttonState = 0;
bool buttonPressed = false;
unsigned long pressingTime = 0;
const int longPressInterval = 1000;

int currentMode = 0;
const int modeCount = 5;

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
  buttonState = digitalRead(buttonPin);

  if(buttonState == HIGH && !buttonPressed){
    pressingTime = millis();
    buttonPressed = true;
  }
  if(buttonState == LOW && buttonPressed){
    unsigned long currentTime = millis();
    if(currentTime - pressingTime < longPressInterval){
      Serial.println("short click");
      if(currentMode == 3){
        changeRainbowSpeed();
      }
      else{
        changeLEDColor();
      }
    }
    else{
      Serial.println("long press");
      changeMode();
    }
    buttonPressed = false;
    delay(20);
  }
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
    if(millis() - rainbowTimer > rainbowSpeeds[rainbowSpeedLevel]){
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
