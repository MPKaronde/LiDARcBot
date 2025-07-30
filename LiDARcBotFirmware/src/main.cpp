#include <Arduino.h>
#include <AccelStepper.h>

// Motor constants
#define HALFSTEP 4 
const int MAX_SPEED = 1000;
const int ACCELERATION = 10000;

// Machine Dimensional Constants (mm)
const int WHEEL_DIAMETER = 75;
const int WHEEL_CENTER_DISTANCE = 205;

// Directions in terms of machine POV
AccelStepper Left(HALFSTEP, 8, 9 , 10, 11);
AccelStepper Right(HALFSTEP, 2, 3, 4, 5);

void setup()
{
  // serial setup
  Serial.begin(115200);

  // motor setup
  Left.setAcceleration(ACCELERATION);
  Left.setMaxSpeed(MAX_SPEED);

  Right.setAcceleration(ACCELERATION);
  Right.setMaxSpeed(MAX_SPEED);
}

// Runs both motors simultaneously, each by their respective set amounts
void run_motors()
{
  while (Left.distanceToGo() != 0 && Right.distanceToGo() != 0)
  {
    Left.run();
    Right.run();
  }
}

// sets target positions for motors in prep for movement
void set_motor_positions(int leftSteps, int rightSteps)
{
  Left.setCurrentPosition(0);
  Right.setCurrentPosition(0);

  Left.moveTo(leftSteps);
  Right.moveTo(rightSteps);
}

// sets motors target positions and runs them simultaneously to said positions
void move_motors(int leftSteps, int rightSteps)
{
  set_motor_positions(leftSteps, rightSteps);
  run_motors();

}
