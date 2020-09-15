#include <AccelStepper.h>
#include <Bounce2.h>

const int jX = A7;
const int jY = A6;
const int stepX = 3;
const int dirX = 2;
const int rlyFwd = 6;
const int rlyRev = 7;
const int rlySldUp = 4;
const int rlySldDwn = 5;
const int btnSw = 8;
const int lzr = 9;
const int maxSpeed = 1000;
const int minSpeed = 0;
const float accelerazione = 50.0;
const int treshold = 30;
long tresholdUp, tresholdDown;
long speedX, valX, valY, mapX, mapY;

boolean abilitato, muoviX;
AccelStepper motoreX(AccelStepper::DRIVER, stepX, dirX);
Bounce debouncer = Bounce();

void setup() {
  speedX = 0;
  tresholdDown = (maxSpeed / 2) - treshold;
  tresholdUp = (maxSpeed / 2) + treshold;

  motoreX.setMaxSpeed(maxSpeed);
  motoreX.setSpeed(minSpeed);
  motoreX.setAcceleration(accelerazione);

  pinMode(rlyFwd, OUTPUT);
  pinMode(rlyRev, OUTPUT);
  pinMode(rlySldUp, OUTPUT);
  pinMode(rlySldDwn, OUTPUT);
  pinMode(lzr, OUTPUT);
  pinMode(btnSw, INPUT_PULLUP);

  debouncer.attach(btnSw);
  debouncer.interval(5);

}

void loop() {
  valX = analogRead(jX);
  valY = analogRead(jY);
  debouncer.update();

  int value = debouncer.read();

  mapX = map(valX, 0, 1023, minSpeed, maxSpeed);
  mapY = map(valY, 0, 1023, minSpeed, maxSpeed);

  pilotaMotori(mapX, mapY);

  if ( value == LOW ) {
    motoreX.stop();
    digitalWrite(lzr, LOW );
    digitalWrite(rlyFwd, HIGH);
    digitalWrite(rlyRev, HIGH);
    digitalWrite(rlySldUp, LOW);
    digitalWrite(rlySldDwn, HIGH);
  }
  else {
    digitalWrite(lzr, HIGH);
    digitalWrite(rlySldUp, HIGH);
    digitalWrite(rlySldDwn, LOW);
  }
}

void pilotaMotori(long mapX, long mapY) {

  if (mapX <= tresholdDown) {

    speedX = -map(mapX, tresholdDown, minSpeed,   minSpeed, maxSpeed);
    muoviX = true;
  } else if (mapX >= tresholdUp) {

    speedX = map(mapX,  maxSpeed, tresholdUp,  maxSpeed, minSpeed);
    muoviX = true;
  } else {

    speedX = 0;
    muoviX = false;
  }

  if (mapY <= tresholdDown) {
    digitalWrite(rlyFwd, HIGH);
    digitalWrite(rlyRev, LOW);
  }
  else if (mapY >= tresholdUp) {
    digitalWrite(rlyFwd, LOW);
    digitalWrite(rlyRev, HIGH);
  }
  else {
    digitalWrite(rlyFwd, HIGH);
    digitalWrite(rlyRev, HIGH);
  }

  if (muoviX) {
    motoreX.setSpeed(speedX);
    motoreX.run();
  } else {
    motoreX.stop();
  }

}
