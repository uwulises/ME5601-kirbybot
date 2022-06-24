#include <Servo.h>
#include <Stepper.h>
#include "MPU9250.h"

MPU9250 mpu;
// Definiciones para los pines de pulso y direccion
#define pinservo 3
#define pinrodillo 45
Servo servokirby;   // nombre del servo es servokirby
const int dirPin_1 = 4;
const int stepPin_1 = 3;

const int dirPin_2 = 15;
const int stepPin_2 = 14;


const int maxSpeed_stepper = 10000;
const int accel = 10000;

#define STEPS 400

Stepper stepper_1(STEPS, dirPin_1, stepPin_1 );
Stepper stepper_2(STEPS, dirPin_2, stepPin_2 );


//for serial event
String input_str = "";
bool str_full = false;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2000);

  if (!mpu.setup(0x68)) {  // change to your own address
    while (1) {
      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }

  //Serial.begin(9600);
  servokirby.attach(pinservo);
  pinMode(pinrodillo, OUTPUT);
  input_str.reserve(200);
  pinMode(LED_BUILTIN, OUTPUT);
  stepper_1.setSpeed(maxSpeed_stepper);
  stepper_2.setSpeed(maxSpeed_stepper);
}



void FW() {

  stepper_1.step(10);
  stepper_2.step(10);

}

void BW() {

  stepper_1.step(-10);
  stepper_2.step(-10);

}

void FW_to_pos(int distance) {

  int pasos = distance * STEPS;
  stepper_1.step(pasos);
  stepper_2.step(pasos);
  delay(10);
}



void levanta_servo()  // funcion que levanta el servo
{ servokirby.write(180);
  delay(30);
}

void baja_servo()  // funcion que baja el servo
{ servokirby.write(0);
  delay(30);
}

void prende_rodillo() {  // funcion que prende el rodillo
  digitalWrite(pinrodillo, HIGH);
}

void apaga_rodillo() {  // funcion que apaga el rodillo
  digitalWrite(pinrodillo, LOW);
}


void read_ultrasonic_1() {


}


void read_ultrasonic_2() {


}

void read_IMU() {

}


void print_roll_pitch_yaw() {
  Serial.print("Yaw, Pitch, Roll: ");
  Serial.print(mpu.getYaw(), 2);
  Serial.print(", ");
  Serial.print(mpu.getPitch(), 2);
  Serial.print(", ");
  Serial.println(mpu.getRoll(), 2);
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    input_str += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      str_full = true;
    }
  }
}

void loop() {

  if (str_full) {

  }
}
