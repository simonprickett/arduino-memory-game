#include <Bounce2.h>

#define RED_LED 0
#define GREEN_LED 1
#define BLUE_LED 2
#define YELLOW_LED 3
#define RED_BUTTON 4
#define GREEN_BUTTON 5
#define BLUE_BUTTON 6
#define YELLOW_BUTTON 7

Bounce debouncerRed = Bounce();
Bounce debouncerGreen = Bounce();
Bounce debouncerBlue = Bounce();
Bounce debouncerYellow = Bounce();

bool redOn = false;
bool greenOn = false;
bool blueOn = false;
bool yellowOn = false;

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);

  debouncerRed.attach(RED_BUTTON, INPUT_PULLUP);
  debouncerRed.interval(30);
  debouncerGreen.attach(GREEN_BUTTON, INPUT_PULLUP);
  debouncerGreen.interval(30);
  debouncerBlue.attach(BLUE_BUTTON, INPUT_PULLUP);
  debouncerBlue.interval(30);
  debouncerYellow.attach(YELLOW_BUTTON, INPUT_PULLUP);
  debouncerYellow.interval(30);
}

void loop() {
  debouncerRed.update();
  debouncerGreen.update();
  debouncerBlue.update();
  debouncerYellow.update();
  
  if (debouncerRed.fell()) {
    redOn = !redOn;
    digitalWrite(RED_LED, redOn ? HIGH : LOW);
  }

  if (debouncerGreen.fell()) {
    greenOn = !greenOn;
    digitalWrite(GREEN_LED, greenOn ? HIGH : LOW);
  }

  if (debouncerBlue.fell()) {
    blueOn = !blueOn;
    digitalWrite(BLUE_LED, blueOn ? HIGH : LOW);
  }


  if (debouncerYellow.fell()) {
    yellowOn = !yellowOn;
    digitalWrite(YELLOW_LED, yellowOn ? HIGH : LOW);
  }
}
