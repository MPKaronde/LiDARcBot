#include <Arduino.h>
#include <AccelStepper.h>

// Motor constants
#define HALFSTEP 4 
const int MAX_SPEED = 1000;
const int ACCELERATION = 10000;

// Machine Dimensional Constants (mm)
const int WHEEL_DIAMETER = 75;
const int WHEEL_CENTER_DISTANCE = 205;
const int STEPS_PER_REVOLUTION = 200;
double CIRCUM = 0; // calculate in setup

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

  // calculate constants
  CIRCUM = WHEEL_DIAMETER * PI;
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
void set_motor_positions(int rightSteps, int leftSteps)
{
  Left.setCurrentPosition(0);
  Right.setCurrentPosition(0);

  Left.moveTo(leftSteps);
  Right.moveTo(rightSteps);
}

// sets motors target positions and runs them simultaneously to said positions
void move_motors(int rightSteps, int leftSteps)
{
  set_motor_positions(rightSteps, leftSteps);
  run_motors();
}

// calculate number of steps to drive a given number of whole cm
// returns value rounded to nearest whole number of steps
int steps_for_cm(int cm)
{
  // solve with (Steps per Rev) * (Rev per Cm) == (Steps per Rev) / circumference
  double circum = WHEEL_DIAMETER * PI;
  double real_steps = (double)((double)cm * (double)STEPS_PER_REVOLUTION) / circum;
  return (int)real_steps;
}

// drive straight given num cm (foward = +, backward = -)
// return true if all precon met
bool drive_straight(int cm)
{
  int steps = steps_for_cm(cm);
  move_motors(steps, steps);
  return true;
}

// calculate num steps to counterrotate wheels for rotation by given num degrees
int calculate_in_place_rotation_steps(int degrees)
{
  double rads = (double)degrees * PI / 180.0; // convert degrees to radians
  double arc_length = ((double)WHEEL_CENTER_DISTANCE / 2.0) * rads;   // find arclength covered by each wheel
  double rotations = arc_length / CIRCUM; // number of rotations the wheel needs to take
  double steps = rotations * STEPS_PER_REVOLUTION;  // number of steps to complete required num revolutions
  return (int)steps;  // cast to int --> make it a whole number
}

// causes motor to turn in place by given num degrees
// + = right, - = left, robot frame of reference
// return true if all precon met
bool turn_in_place(int degrees)
{
  int steps = calculate_in_place_rotation_steps(degrees);

  // reverse direction if rotating left
  if(degrees < 0)
  {
    steps = -steps;
  }
  move_motors(steps, -steps);
  return true;
}


