#include <HCSR04.h>
#include <Servo.h>
#include <Stepper.h>
#include "MPU9250.h"
MPU9250 mpu;
// Definiciones para los pines de pulso y direccion
#define pinservo 3
#define pinrodillo 45
Servo servokirby;   // nombre del servo es servokirby
const int dirPin_1 = 16;
const int stepPin_1 = 17;

const int dirPin_2 = 14;
const int stepPin_2 = 15;


const int maxSpeed_stepper = 50;


#define STEPS 400

Stepper stepper_1(STEPS, dirPin_1, stepPin_1 );
Stepper stepper_2(STEPS, dirPin_2, stepPin_2 );



HCSR04 hc1(4, 5); //initialisation class HCSR04 (trig pin , echo pin)
HCSR04 hc2(6, 7); //initialisation class HCSR04 (trig pin , echo pin)

String inputString = "";
bool stringComplete = false;

void setup() {
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

  Serial.println("----");
  Serial.println("READY");
  Serial.println("----");

}



void BW() {

  stepper_1.step(-10);
  stepper_2.step(-10);

}

void FW() {

  stepper_1.step(10);
  stepper_2.step(10);

}

void TR() {

  stepper_1.step(-10);
  stepper_2.step(10);

}

void TL() {

  stepper_1.step(10);
  stepper_2.step(-10);

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
  delay(20);
}

void apaga_rodillo() {  // funcion que apaga el rodillo
  digitalWrite(pinrodillo, LOW);
  delay(20);
}


void read_ultrasonic_1() {
  Serial.println( hc1.dist() ); //return current distance (cm) in serial
  delay(60);

}


void read_ultrasonic_2() {

  Serial.println( hc2.dist() ); //return current distance (cm) in serial
  delay(60);
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
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void loop() {

  if (stringComplete) {
    Serial.println(inputString);

    if (inputString == "run_brush\n") {
      Serial.println("runningbrush");
      prende_rodillo();
      delay(10);
      stringComplete = false;

    }
    if (inputString == "stop_brush\n") {
      Serial.println("stoppingbrush");
      apaga_rodillo();
      delay(10);
      stringComplete = false;

    }
    if (inputString == "FW\n") {
      Serial.println("FORWARD");
      FW();

    }

    if (inputString == "BW\n") {
      Serial.println("BACKWARD");
      BW();


    }
    if (inputString == "TR\n") {
      Serial.println("TURNING RIGHT");
      TR();


    }

    if (inputString == "TL\n") {
      Serial.println("TURNING LEFT");
      TL();


    }
    if (inputString == "HC1\n") {
      Serial.println("HC1");
      read_ultrasonic_1();
      delay(10);

    }



    else {
      inputString = "";
    }


    // clear the string:
    inputString = "";
    stringComplete = false;
  }





}
