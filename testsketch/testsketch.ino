#include <Tone.h>
#include <Bounce2.h>


#define RED_LED 0
#define GREEN_LED 1
#define BLUE_LED 2
#define YELLOW_LED 3
#define RED_BUTTON 4
#define GREEN_BUTTON 5
#define BLUE_BUTTON 6
#define YELLOW_BUTTON 7
#define BUZZER_PIN 8

Bounce debouncerRed = Bounce();
Bounce debouncerGreen = Bounce();
Bounce debouncerBlue = Bounce();
Bounce debouncerYellow = Bounce();

Tone buzzer;

unsigned short currentDelay;

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

  buzzer.begin(BUZZER_PIN);

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
}

void loop() {  
  debouncerGreen.update();
  debouncerRed.update();
  debouncerBlue.update();
  debouncerYellow.update();

  if (debouncerGreen.fell() || debouncerRed.fell() || debouncerBlue.fell() || debouncerYellow.fell()) {
    buzzer.play(NOTE_F3, 300);
  }
}
