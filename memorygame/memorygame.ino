#include <Bounce2.h>
#include <Tone.h>

#define RED_LED 0
#define GREEN_LED 1
#define BLUE_LED 2
#define YELLOW_LED 3
#define RED_BUTTON 4
#define GREEN_BUTTON 5
#define BLUE_BUTTON 6
#define YELLOW_BUTTON 7
#define BUZZER_PIN 8
#define MAX_GAME_SEQUENCE 800

Bounce debouncerRed = Bounce();
Bounce debouncerGreen = Bounce();
Bounce debouncerBlue = Bounce();
Bounce debouncerYellow = Bounce();

Tone buzzer;

unsigned short gameSequence[MAX_GAME_SEQUENCE];

bool gameInProgress = false;
bool attractLEDOn = false;
bool showingSequenceToUser;
unsigned short currentDelay;
unsigned short currentSequenceLength;
unsigned short userPositionInSequence;
unsigned short n;
unsigned long count = 0;


// Create a new game sequence.
// Prevent generating sequence elements repeating more than twice in a row.
void generate_sequence(unsigned short *sequence, unsigned long max_game_sequence){
  randomSeed(analogRead(0));
  unsigned short second_prev;
  unsigned short prev;

  sequence[0] = random(0, 3);
  second_prev = sequence[0];
  sequence[1] = random(0, 3);
  prev = sequence[1];

  for (int i = 2; i < max_game_sequence; i++) {
    sequence[i] = random(0, 3);
    if(sequence[i] == second_prev || sequence[i] == prev){
      // prevent 3 matching colors in a row (2 in row is ok)
      sequence[i] = (sequence[i]+1)%4;
    }
    second_prev = prev;
    prev = sequence[i];
  }
}

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
}

void loop() {  
  if (! gameInProgress) {
    // Waiting for someone to press the green button...
    debouncerGreen.update();

    if (debouncerGreen.fell()) {
      digitalWrite(GREEN_LED, LOW);
      
      generate_sequence(gameSequence, MAX_GAME_SEQUENCE);

      currentSequenceLength = 1;
      currentDelay = 500;

      gameInProgress = true;
      count = 0;
      showingSequenceToUser = true;

      // Little delay before the game starts.
      delay(1000);
    } else {
      // Attract mode - flash the green LED.
      if (count == 50000) {
        attractLEDOn = ! attractLEDOn;
        digitalWrite(GREEN_LED, attractLEDOn ? HIGH : LOW);    
        count = 0;    
      } 
      
      count++;
    }
  } else {
    // Game is in progress...
    if (showingSequenceToUser) {
      // Play the pattern out to the user
      for (n = 0; n < currentSequenceLength; n++) {
        digitalWrite(gameSequence[n] - 4, HIGH);
        delay(currentDelay);
        digitalWrite(gameSequence[n] - 4, LOW);
        delay(currentDelay);
      }

      showingSequenceToUser = false;
      userPositionInSequence = 0;
    } else {
      // Waiting for the user to repeat the sequence back
      debouncerGreen.update();
      debouncerRed.update();
      debouncerBlue.update();
      debouncerYellow.update();

      unsigned short userPressed = 0;

      if (debouncerGreen.fell()) {
        digitalWrite(GREEN_LED, HIGH);
        delay(currentDelay);
        digitalWrite(GREEN_LED, LOW);
        userPressed = GREEN_BUTTON;
      } else if (debouncerRed.fell()) {
        digitalWrite(RED_LED, HIGH);
        delay(currentDelay);
        digitalWrite(RED_LED, LOW);
        userPressed = RED_BUTTON;
      } else if (debouncerBlue.fell()) {
        digitalWrite(BLUE_LED, HIGH);
        delay(currentDelay);
        digitalWrite(BLUE_LED, LOW);
        userPressed = BLUE_BUTTON;
      } else if (debouncerYellow.fell()) {
        digitalWrite(YELLOW_LED, HIGH);
        delay(currentDelay);
        digitalWrite(YELLOW_LED, LOW);
        userPressed = YELLOW_BUTTON;
      }

      if (userPressed > 0) {
        // A button was pressed, check it against current sequence...
        if (userPressed != gameSequence[userPositionInSequence]) {
          // Failed...
          buzzer.play(NOTE_F3, 300);
          delay(300);
          buzzer.play(NOTE_G3, 500);
          delay(2500);
          gameInProgress = false;
        } else {
          userPositionInSequence++;
          if (userPositionInSequence == currentSequenceLength) {
            // User has successfully repeated back the sequence so make it one longer...
            currentSequenceLength++;

            // There's no win scenario here, so just reset...
            if (currentSequenceLength >= MAX_GAME_SEQUENCE) {
              gameInProgress = false;
            }

            // Play a tone...
            buzzer.play(NOTE_A3, 300);
            delay(2000);

            showingSequenceToUser = true;
          }
        }
      }
    }
  }
}
