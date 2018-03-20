#include <Adafruit_NeoPixel.h>

#define PIN      3
#define N_LEDS 16
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
 
bool isOn = false;
int buttonPin = 5;

int pressedTime = 0;
uint64_t lastPress = 0;
int buttonDelay = 1000;
int brightness = 255;
int brightnessChange = 20;
int numColors = 6;
uint32_t colors[] = { strip.Color(0xff, 0x00, 0x00), // red
                      strip.Color(0x00, 0xff, 0x00), // green
                      strip.Color(0x00, 0x00, 0xff), // blue
                      strip.Color(0xff, 0x88, 0x00), // orange
                      strip.Color(0x88, 0x00, 0xff), // violet
                      strip.Color(0xff, 0xff, 0xff), // white
                      };
int currentColor = 0;
int pressCount;

void setup() {
  strip.begin();
  isOn = false;
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  bool isCurrentlyPressed = (digitalRead(buttonPin) == HIGH);
  if (isCurrentlyPressed) {
    processButtonTouch();
  } else {
    processButtonRelease();
  }
  delay(10);
}

void processButtonTouch() {
  if (!isOn) {
    isOn = true;
    updateState();
  } else {
    pressedTime += 10;
    
    if (pressedTime > 1000 && pressedTime%50 == 0) {
      buttonHeldDown();
    }
  }
}

void processButtonRelease() {
  if (pressedTime < 1000 && pressedTime > 0) {
    buttonPressed();
  }
  pressedTime = 0;
  checkDoublePress();
  checkSinglePress();
}

void buttonHeldDown() {
  changeBrightness();
}

void buttonPressed() {
  pressCount++;
  lastPress = millis();
}

void checkDoublePress() {
  if (pressCount > 1) {
    doublePress();
    pressCount = 0;
    lastPress = 0;
  }
}

void checkSinglePress() {
  if (millis() > lastPress + 600){
    if (pressCount >= 1) {
      isOn = !isOn;
      updateState();
    }
    pressCount = 0;
    lastPress = 0;
  }
}

void doublePress() {
  rotateColor();
}

void rotateColor() {
  currentColor = (currentColor + 1) % numColors;
  Serial.print("rotate color: ");
  Serial.println(currentColor);
  updateState();
}

void changeBrightness() {
  brightness += brightnessChange;
  if (brightness >= 255){
    brightness = 255;
    brightnessChange *= -1;
  }
  if (brightness <= 1) {
    brightness = 1;
    brightnessChange *= -1;
  }
  Serial.print("set brightness: ");
  Serial.println(brightness);
  updateState();
}

void updateState(){
  uint32_t color = colors[currentColor];
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if (isOn) {
      strip.setPixelColor(i  , color);
    } else {
      strip.setPixelColor(i  , 0);
    }
    strip.setBrightness(brightness);
    strip.show();
    delay(25);
  }
}
