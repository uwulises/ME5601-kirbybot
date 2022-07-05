#include <HCSR04.h>
#include <Servo.h>
#include <Stepper.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include <SoftwareSerial.h>

// Definiciones para los pines de pulso y direccion
#define pinservo 3
#define pinrodillo 45

const int dirPin_1 = 16;
const int stepPin_1 = 17;

const int dirPin_2 = 14;
const int stepPin_2 = 15;

const int maxSpeed_stepper = 300;
const int stepstorun = 200;

const int STEPS = 400;

// La dirección del MPU6050 puede ser 0x68 o 0x69, dependiendo
// del estado de AD0. Si no se especifica, 0x68 estará implicito
MPU6050 sensor;

// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int ax, ay, az;
int gx, gy, gz;

long tiempo_prev;
float dt;
float ang_x, ang_y;
float ang_x_prev, ang_y_prev;

///

Servo servokirby; // nombre del servo es servokirby
Stepper stepper_1(STEPS, dirPin_1, stepPin_1);
Stepper stepper_2(STEPS, dirPin_2, stepPin_2);

HCSR04 hc1(4, 5); // initialisation class HCSR04 (trig pin , echo pin)
HCSR04 hc2(6, 7); // initialisation class HCSR04 (trig pin , echo pin)

String inputString = "";
bool stringComplete = false;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  sensor.initialize(); // Iniciando el sensor
  delay(2000);

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
  for (int i = 0; i < stepstorun + 1; i++)
  {
    stepper_1.step(-1);
    stepper_2.step(-1);
  }
}
void FW()
{
  // digitalWrite(LED_BUILTIN,HIGH);
  for (int i = 0; i < stepstorun + 1; i++)
  {
    stepper_1.step(1);
    stepper_2.step(1);
  }
  // digitalWrite(LED_BUILTIN,LOW);
}

void TR()
{

  for (int i = 0; i < stepstorun + 1; i++)
  {
    stepper_1.step(1);
    stepper_2.step(-1);
  }
}

void TL()
{
  for (int i = 0; i < stepstorun + 1; i++)
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
  servokirby.write(180);
  delay(30);
}

void prende_rodillo()
{ // funcion que prende el rodillo
  digitalWrite(pinrodillo, LOW);
  delay(20);
}

void apaga_rodillo()
{ // funcion que apaga el rodillo
  digitalWrite(pinrodillo, HIGH);
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

void read_MPU6050()
{
  // Leer las aceleraciones y velocidades angulares
  sensor.getAcceleration(ax, ay, az);
  sensor.getRotation(gx, gy, gz);

  dt = (millis() - tiempo_prev) / 1000.0;
  tiempo_prev = millis();

  // Calcular los ángulos con acelerometro
  float accel_ang_x = atan(ay / sqrt(pow(ax, 2) + pow(az, 2))) * (180.0 / 3.14);
  float accel_ang_y = atan(-ax / sqrt(pow(ay, 2) + pow(az, 2))) * (180.0 / 3.14);

  // Calcular angulo de rotación con giroscopio y filtro complemento
  ang_x = 0.98 * (ang_x_prev + (gx / 131) * dt) + 0.02 * accel_ang_x;
  ang_y = 0.98 * (ang_y_prev + (gy / 131) * dt) + 0.02 * accel_ang_y;

  ang_x_prev = ang_x;
  ang_y_prev = ang_y;

  // Mostrar los angulos separadas por un [tab]

  // Serial.print("Rotacion en X:  ");
  // Serial.print(ang_x);
  // Serial.print("tRotacion en Y: ");
  // Serial.println(ang_y);

  delay(10);
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
    // Serial.println(inputString);
    if (inputString == "run_b\n")
    {
      // Serial.println("runningbrush");
      prende_rodillo();
      delay(10);
    }

    if (inputString == "stop_b\n")
    {
      // Serial.println("stoppingbrush");
      apaga_rodillo();
      delay(10);
    }

    if (inputString == "RB\n")
    {
      // Serial.println("rising brush");
      rise_brush();
      delay(10);
    }
    if (inputString == "PB\n")
    {
      // Serial.println("placing brush");
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
      // Serial.println("HC1");
      read_ultrasonic_1();
      delay(10);
    }
    if (inputString == "HC2\n")
    {
      // Serial.println("HC2");
      read_ultrasonic_2();
      delay(10);
    }

    if (inputString == "r_imu\n")
    {
      read_MPU6050();
    }

    else
    {
      inputString = "";
    }

    // clear the string:
    inputString = "";
    stringComplete = false;
    // Serial.println(17);
  }
}
