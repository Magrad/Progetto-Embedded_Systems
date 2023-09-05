#include "in-game-constants.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include <stdio.h>
#include <EnableInterrupt.h>

#define EI_ARDUINO_INTERRUPTED_PIN // to enable pin states functionality 
#define LED_S_PIN 9 // red led 'Ls'
#define POT_PIN A0 // potentiometer 'Pot'
#define LED1_PIN 10 // green led 'L1'
#define LED2_PIN 11 // green led 'L2'
#define LED3_PIN 12 // green led 'L3'
#define LED4_PIN 13 // green led 'L4'
#define BUTTON1_PIN 2 // button 'T1'
#define BUTTON2_PIN 3 // button 'T2'
#define BUTTON3_PIN 4 // button 'T3'
#define BUTTON4_PIN 5 // button 'T4'

#define SECOND 1000 // 1 second

// Game variables
int state = 0; // 0: idle, 1: in-game, 2: game-over
int tts = 10; // time to sleep variable, after 10 second in idle the system is put to sleep
int score = 0; // increments every time the user scores a point
int bestScore[c_difficulty_modes] = {0,0,0,0,0,0,0,0};
int difficulty = 0; // 0: super easy, 1: easy, 2: normal, 3: medium, 4: hard, 5: nightmare, 6: hell, 7: are you even human?
long ttgo = 0; // time to game over
int over = 0; // game over
int timer1; // time to wait for the light to stop on a led
int ranTimer1;
float timer2; // time remaining to click the correct button
float stepVelocity; // time required for the light to jump to the next led
float reducingFactor; // quantity of time reducing the timers

// Hardware variables
long startTime;
long lastTime = 0;
long awokenTime = 0;
int awoken = 0;
int ledState = 1; // 0: low, 1: high
int btnTime = 0;
int msgOut = 1;
int currentLit = LED1_PIN - 1;
int movingBall = 1;
int tBlink;

void setup() {
  Serial.begin(9600);

  // Game variables
  timer1 = c_timer1;
  timer2 = c_timer2;
  stepVelocity = c_speed;

  // Hardware variables
  startTime = 0;
  tBlink = c_tBlink;  
}

void loop() {
  long currentTime = millis();

  // Idle state
  if(state == 0) {
    idle(currentTime);
  } 

  // In game state
  if(state == 1) {
    inGame(currentTime);
  }

  // Game over state
  if(state == 2){
    gameOver(currentTime);
  }
}

// Generates a random integer from two given numbers
int random(int lower, int upper) {
  return rand() % (upper - lower + 1) + lower;
}

// Idle game state
void idle(long currentTime) {
  if(msgOut) {
    Serial.println(c_welcome); // welcome text
    Serial.println(c_difficulty + c_difficulties[difficulty]);
    if(bestScore[difficulty] != 0) {
      Serial.println(c_best_score + c_difficulties[difficulty] + ": " + bestScore[difficulty]);
    }
    msgOut = 0;
  }
  
  // Red blinking light that changes state every seconds
  if(currentTime - lastTime > SECOND) {
    digitalWrite(LED_S_PIN, ledState); 
    lastTime = currentTime; // Updates the last updated time
    ledState = ledState == 0 ? 1 : 0; // Changes state (LOW, HIGH)
  }

  if(awoken && currentTime - awokenTime > SECOND) {
    awoken = 0;
    awokenTime = 0;
  }

  // Waits for user input
  if(digitalRead(BUTTON1_PIN) == HIGH && !awoken) {
    state = 1; // Changes game state from idle to Game state
    msgOut = 1;
    ranTimer1 = random(c_timer1_min, timer1);
    return;
  }

  // Difficulty selector
  int potNewValue = map(analogRead(POT_PIN), 0, 1023, 0, c_difficulty_modes);
  if(potNewValue != difficulty) {
    // Maps the analog value to a 1 to 8 difficulty setting
    difficulty = potNewValue;
    reducingFactor = float(difficulty+1) / 10;
    // Prints the new difficulty setting
    Serial.println(c_difficulty + c_difficulties[difficulty]);
    // Updates the idle timer (user is not idle)
    startTime = currentTime;
    if(bestScore[difficulty] != 0) {
      Serial.println(c_best_score + c_difficulties[difficulty] + ": " + bestScore[difficulty]);
    }
  }

  // If the user has been idle for more than 10 sec arduino goes into deep sleep
  if(currentTime - startTime > (tts * SECOND)) {
    startSleep();
    lastTime = currentTime;
    startTime = currentTime;
  }
}

// Real game state
void inGame(long currentTime) {
  digitalWrite(LED_S_PIN, LOW);

  // Execute this code only the first time each game cicle
  if(msgOut) {
    Serial.println(c_game_starts);
    // Initialize Game variables
    startTime = currentTime;
    msgOut = 0;
    score = 0;
    ttgo = 0;
    over = 0;
    currentLit = LED1_PIN - 1;
  }

  // If no input has been detected and the time elapsed is greater than 'timer2', the game is over
  if(over && currentTime - ttgo > (timer2 * SECOND)) {
    state = 2;
    msgOut = 1;
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    digitalWrite(LED4_PIN, LOW);
    return;
  }

  // Keeps the light cicle when elapsed time in lower than the 'timer1'
  if(currentTime - startTime <= (ranTimer1 * SECOND)) {
    // Changes lit light every time time elapsed in greater than the game 'speed'
    if(currentTime - lastTime > (stepVelocity * SECOND)) {
      /* If the position of the current lit led does not coincide with the last green led
         the ball keeps moving right (1) otherwise it will move left (-1)

         if (currentLit == LED4_PIN) {
           movingBall = -1;
         }
         else if (currentLit == LED1_PIN) {
           movingBall = 1;
         } else {
           // movingBall remains unchanged
         }
      */
      movingBall = currentLit == LED4_PIN ? -1 : currentLit == LED1_PIN ? 1 : movingBall;
      
      currentLit += movingBall;
      digitalWrite(currentLit, HIGH);
      digitalWrite(currentLit-movingBall, LOW);
      lastTime = currentTime;
    }
  } else {
    if(over == 0) {
      over = 1;
      ttgo = currentTime;
    }
    // Checks if the correct button is clicked corresponding to the lit light
    if(digitalRead(currentLit - LED1_PIN + BUTTON1_PIN) == HIGH && currentTime - btnTime > 20) {
      correctButton(currentLit);
      btnTime = currentTime;
      score++;
      Serial.println(c_score + score);
      // Reduce all the game timers by some factor based on difficulty level
      //timer1 = timer1 - reducingFactor < c_timer1_min ? c_timer1_min : timer1 - reducingFactor;
      ranTimer1 = random(c_timer1_min, timer1);
      stepVelocity = stepVelocity - reducingFactor < c_speed_min ? c_speed_min : stepVelocity - reducingFactor;
      timer2 = timer2 - reducingFactor < c_timer2_min ? c_timer2_min : timer2 - reducingFactor;
      over = 0;
      startTime = currentTime;
    } else if(digitalRead(BUTTON1_PIN) == HIGH ||
              digitalRead(BUTTON2_PIN) == HIGH ||
              digitalRead(BUTTON3_PIN) == HIGH || 
              digitalRead(BUTTON4_PIN) == HIGH) {
                wrongButton();
                int test = digitalRead(BUTTON1_PIN) == HIGH ? 1 : digitalRead(BUTTON2_PIN) == HIGH ? 2 : digitalRead(BUTTON3_PIN) == HIGH ? 3 : digitalRead(BUTTON4_PIN) == HIGH ? 4 : -1;
                ranTimer1 = random(c_timer1_min, timer1);
                over = 0;
                startTime = currentTime;
    }
  }
}

// Blinking all lights, wrong input
void wrongButton() {
  for(int i=0; i<tBlink;i++) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED3_PIN, HIGH);
    digitalWrite(LED4_PIN, HIGH);
    delay(500);
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    digitalWrite(LED4_PIN, LOW);
    delay(500);
  }
}

// Blinking correct light
void correctButton(int lit) {
  for(int i=0; i<tBlink;i++) {
    digitalWrite(lit, LOW);
    delay(500);
    digitalWrite(lit, HIGH);
    delay(500);
  }
}

// Game over state
void gameOver(long currentTime) {
  // Prints the user score
  if(msgOut) {
    bestScore[difficulty] = bestScore[difficulty] < score ? score : bestScore[difficulty];
    Serial.println(c_game_over + score);
    startTime = currentTime;
    msgOut = 0;
  }
  
  // After 10 sec have passed the game goes back to idle
  if(currentTime - startTime > 10 * SECOND) {
    state = 0;
    startTime = currentTime;
    lastTime = currentTime;
    msgOut = 1;
  }
}

// Set the Arduino to deep sleep
void startSleep() {
  Serial.println("Asleep");
  // Once the user clicks the 'Button 1' Arduino will wake up
  enableInterrupt(BUTTON1_PIN, wakeUpNow, RISING);
  enableInterrupt(BUTTON2_PIN, wakeUpNow, RISING);
  enableInterrupt(BUTTON3_PIN, wakeUpNow, RISING);
  enableInterrupt(BUTTON4_PIN, wakeUpNow, RISING);
  delay(100);
  digitalWrite(LED_S_PIN, LOW);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  sleep_enable();
  sleep_mode();

  //...

  sleep_disable();
  power_all_enable();
  Serial.println("Awaken");
  disableInterrupt(BUTTON1_PIN);
  disableInterrupt(BUTTON2_PIN);
  disableInterrupt(BUTTON3_PIN);
  disableInterrupt(BUTTON4_PIN);
}

// Wake up function
void wakeUpNow() {
  msgOut = 1;
  awoken = 1;
  awokenTime = millis();
}
