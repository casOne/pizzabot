#include <AccelStepper.h>
#include <MultiStepper.h>

#define M1_ENA 10
#define M1_STP 9
#define M1_DIR 8

#define M2_ENA 7
#define M2_STP 6
#define M2_DIR 5

#define M3_ENA 2
#define M3_STP 3
#define M3_DIR 4

#define MAXSPEED 8000

int vector = 0;
int dist = 0;
int vel = 0;
int mode = 0;
volatile int speedM1 = 0, speedM2 = 0, speedM3 = 0;
int AngW = 0;
char command;
volatile float  M1, M2, M3;

// ********************************************* CONSTRUCTORS

AccelStepper stepper1(AccelStepper::DRIVER, M1_STP, M1_DIR);
AccelStepper stepper2(AccelStepper::DRIVER, M2_STP, M2_DIR);
AccelStepper stepper3(AccelStepper::DRIVER, M3_STP, M3_DIR);

// *************************************************************** SETUP

void setup() {

  pinMode(M1_ENA, OUTPUT);
  pinMode(M1_STP, OUTPUT);
  pinMode(M1_DIR, OUTPUT);

  pinMode(M2_ENA, OUTPUT);
  pinMode(M2_STP, OUTPUT);
  pinMode(M2_DIR, OUTPUT);

  pinMode(M3_ENA, OUTPUT);
  pinMode(M3_STP, OUTPUT);
  pinMode(M3_DIR, OUTPUT);

  disableAll();

  stepper1.setMaxSpeed(MAXSPEED);
  stepper1.setAcceleration(800.0);
  stepper1.moveTo(2000);

  stepper2.setMaxSpeed(MAXSPEED);
  stepper2.setAcceleration(800.0);
  stepper2.moveTo(2000);

  stepper3.setMaxSpeed(MAXSPEED);
  stepper3.setAcceleration(800.0);
  stepper3.moveTo(2000);

  //enableAll();

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Ready");
  setMotorSpeed(0, 0, 0);
}



// *********************************************************************** LOOP

void loop() {

  checkserial();

  switch (mode)
  {
    case (0):
      freerun();
      break;
    case (1):
      runTo();
      break;
  }
}




// ********************************************************************
// ********************************************************************
// ******************************* FUNCTIONS **************************
// ********************************************************************
// ********************************************************************


// ********************************************** SERIAL
void checkserial()
{
  if (Serial.available() > 0)
  {
    command = Serial.read();
    if (command == 'v')
    {
      mode = 0;
      vector = Serial.parseInt();
      vel = Serial.parseInt();
      AngW = Serial.parseInt();

      calcSpeedMs(vector, vel, AngW);
      speedM1 = (int)M1;
      speedM2 = (int)M2;
      speedM3 = (int)M3;
      setMotorSpeed(speedM1, speedM2, speedM3);


    }
    if (command == 't')
    {
      mode = 0;
      AngW = Serial.parseInt();

      speedM1 = AngW;
      speedM2 = AngW;
      speedM3 = AngW;
      setMotorSpeed(speedM1, speedM2, speedM3);

    }
    if (command == 's')
    {
      mode = 0;
      speedM1 = Serial.parseInt();
      speedM2 = Serial.parseInt();
      speedM3 = Serial.parseInt();
      setMotorSpeed(speedM1, speedM2, speedM3);
    }
    if (command == 'r')
    {
      printspeed();
      if (Serial.read() == '\n') {}
    }
    if (command == 'e')
    {
      mode = 0;
      AngW = Serial.parseInt();
      if (AngW == 0) disableAll();
      if (AngW == 1) enableAll();
      if (Serial.read() == '\n') {}
    }

    if (command == 'a')
    {
      mode = 0;
      speedM1 = 0;
      speedM2 = 0;
      speedM3 = 0;
      setMotorSpeed(speedM1, speedM2, speedM3);
      if (Serial.read() == '\n') {}
    }



  }
}

// ********************************************** CALC SPEED
void calcSpeedMs (int vect, int vel, int W)
{
  float Ax, Ay, vec;
  vec = vect * 2 * PI / 360;
  Ax = -cos(vec);
  Ay = sin(vec);
  M1 = (W / 3 - 2 * Ax / 3) * vel;
  M2 = (Ax / 3 + 250 * Ay / 433 + W / 3) * vel;
  M3 = (Ax / 3 - 250 * Ay / 433 + W / 3) * vel;
}

//******************************************* STOP THE MOTORS
void stopMotors()
{
  setMotorSpeed(0, 0, 0);
}

//******************************************* SET THE MOTOR SPEED
void setMotorSpeed(int M1, int M2, int M3)
{
  stepper1.setSpeed(M1);
  stepper2.setSpeed(M2);
  stepper3.setSpeed(M3);
}

//******************************************* RUN TO DISTANCE
void runTo()
{
  //TBD
}

//******************************************* PRINT OUT THE SPPED VALUES
void printspeed()
{

  Serial.println(speedM1);
  Serial.println(speedM2);
  Serial.println(speedM3);

}

//******************************************* FREE RUN AT GIVEN SPEED
void freerun()
{
  stepper1.runSpeed();
  stepper2.runSpeed();
  stepper3.runSpeed();

}

//******************************************* DISABLE MOTORS
void disableAll(void)
{
  digitalWrite(M1_ENA, HIGH);
  digitalWrite(M2_ENA, HIGH);
  digitalWrite(M3_ENA, HIGH);
  Serial.println ("Motors disabled");
}

//******************************************* ENABLE MOTORS
void enableAll(void)
{
  digitalWrite(M1_ENA, LOW);
  digitalWrite(M2_ENA, LOW);
  digitalWrite(M3_ENA, LOW);
  Serial.println ("Motors enabled");
}




