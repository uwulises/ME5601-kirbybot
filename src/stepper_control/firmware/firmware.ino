#include <HCSR04.h>
#include <Servo.h>
#include <Stepper.h>
#include "MPU9250.h"
#include <SoftwareSerial.h>  
MPU9250 mpu;
// Definiciones para los pines de pulso y direccion
#define pinservo 3
#define pinrodillo 45
Servo servokirby; // nombre del servo es servokirby
const int dirPin_1 = 16;
const int stepPin_1 = 17;

const int dirPin_2 = 14;
const int stepPin_2 = 15;

const int maxSpeed_stepper = 300;
const int stepstorun = 200;

#define STEPS 400

Stepper stepper_1(STEPS, dirPin_1, stepPin_1);
Stepper stepper_2(STEPS, dirPin_2, stepPin_2);

HCSR04 hc1(4, 5); // initialisation class HCSR04 (trig pin , echo pin)
HCSR04 hc2(6, 7); // initialisation class HCSR04 (trig pin , echo pin)

String inputString = "";
bool stringComplete = false;

void setup()
{
  //Serial.begin(9600);
  Serial.begin(9600);
  Wire.begin();
  delay(2000);

  // if (!mpu.setup(0x68)) {  // change to your own address
  //   while (1) {
  //     Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
  //   delay(5000);
  //   }
  //  }

  servokirby.attach(pinservo);
  pinMode(pinrodillo, OUTPUT);

  inputString.reserve(200);
  pinMode(LED_BUILTIN, OUTPUT);
  stepper_1.setSpeed(maxSpeed_stepper);
  stepper_2.setSpeed(maxSpeed_stepper);
  rise_brush();
  apaga_rodillo();
  delay(200);
  Serial.println("----");
  Serial.println("READY");
  Serial.println("----");
  delay(200);
}

void BW()
{
  for(int i=0; i<stepstorun+1; i++)
{
  stepper_1.step(-1);
  stepper_2.step(-1);
}
}
void FW()
{

  for(int i=0; i<stepstorun+1; i++)
{
  stepper_1.step(1);
  stepper_2.step(1);
}
}

void TR()
{

  for(int i=0; i<stepstorun+1; i++)
{
  stepper_1.step(1);
  stepper_2.step(-1);
}
}

void TL()
{
  for(int i=0; i<stepstorun+1; i++)
{
  stepper_1.step(-1);
  stepper_2.step(1);
}
}

void rise_brush() // funcion que levanta el servo
{
  servokirby.write(50);
  delay(30);
}

void place_brush() // funcion que baja el servo
{
  servokirby.write(150);
  delay(30);
}

void prende_rodillo()
{ // funcion que prende el rodillo
  digitalWrite(pinrodillo, HIGH);
  delay(20);
}

void apaga_rodillo()
{ // funcion que apaga el rodillo
  digitalWrite(pinrodillo, LOW);
  delay(20);
}

void read_ultrasonic_1()
{
  Serial.println(hc1.dist()); // return current distance (cm) in serial
  delay(60);
}

void read_ultrasonic_2()
{

  Serial.println(hc2.dist()); // return current distance (cm) in serial
  delay(60);
}

void read_IMU()
{
}

void print_roll_pitch_yaw()
{
  Serial.print("Yaw, Pitch, Roll: ");
  Serial.print(mpu.getYaw(), 2);
  Serial.print(", ");
  Serial.print(mpu.getPitch(), 2);
  Serial.print(", ");
  Serial.println(mpu.getRoll(), 2);
}

void serialEvent()
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n')
    {
      stringComplete = true;
    }
  }
}

void loop()
{

  if (stringComplete)
  {
    Serial.println(inputString);
    if (inputString == "run_b\n")
    {
      Serial.println("runningbrush");
      prende_rodillo();
      delay(10);
    }

    if (inputString == "stop_b\n")
    {
      Serial.println("stoppingbrush");
      apaga_rodillo();
      delay(10);
    }

    if (inputString == "RB\n")
    {
      Serial.println("rising brush");
      rise_brush();
      delay(10);
    }
    if (inputString == "PB\n")
    {
      Serial.println("placing brush");
      place_brush();
      delay(10);
    }
    if (inputString == "FW\n")
    {
      FW();
    }

    if (inputString == "BW\n")
    {
      BW();
    }
    if (inputString == "TR\n")
    {
      TR();
    }

    if (inputString == "TL\n")
    {
      TL();
    }
    if (inputString == "HC1\n")
    {
      Serial.println("HC1");
      read_ultrasonic_1();
      delay(10);
    }
    if (inputString == "HC2\n")
    {
      Serial.println("HC2");
      read_ultrasonic_2();
      delay(10);
    }

    if (inputString == "r_imu\n")
    {
      read_IMU();
    }

    else
    {
      inputString = "";
    }

    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}
