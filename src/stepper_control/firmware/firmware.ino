#include <AccelStepper.h>
#include <FiniteStateMachine.h>
// Definiciones para los pines de pulso y direccion
const int dirPin_1 = 4;
const int stepPin_1 = 3;

const int dirPin_2 = 6;
const int stepPin_2 = 7;
const int maxSpeed_stepper = 10000;
const int accel = 10000;
AccelStepper stepper_1(AccelStepper::DRIVER, stepPin_1, dirPin_1);
AccelStepper stepper_2(AccelStepper::DRIVER, stepPin_2, dirPin_2);

//for serial event
String input_str = "";
bool str_full = false;
bool stop_button = false;
State START = State(motor_start);
State STOP = State(motor_stop);
State FW = State(go_forward);
State BW = State(go_backward);
State TURN_R = State(turn_right);
State TURN_L = State(turn_left);
State READ_SENSORS = State(read_sensor);
//initiate state machines
FSM Motor = FSM(STOP);


void setup() {
  Serial.begin(9600);
  stepper_1.setMaxSpeed(maxSpeed_stepper);
  stepper_2.setMaxSpeed(maxSpeed_stepper);
  stepper_1.setAcceleration(accel);
  input_str.reserve(200);
  pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(20), stopp, CHANGE);
}

void motor_start()
{
  stepper_1.stop();
  stepper_2.stop();
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);

}



void motor_stop()
{
  stepper_1.stop();
  stepper_2.stop();
}

void go_forward()
{
  stepper_1.setSpeed(maxSpeed_stepper);
  stepper_2.setSpeed(maxSpeed_stepper);
  while  (!stop_button) {
  
   stepper_1.move(100);
   stepper_1.run();
    //Serial.println("RUNNING");
    //delay(10);
  }
}

void go_backward()
{
}

void turn_right()
{
}

void turn_left()
{
}

uint8_t read_sensor()
{
}

void stopp(){
  stop_button=!stop_button;
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
    Serial.println(input_str);
    //set motor state depending on serial string
    if (input_str.equals("START\n")) {
      if (Motor.isInState(STOP)) {
        Serial.println("Start!");
        Motor.transitionTo(FW);

      }
    }
    // clear the string:
    input_str = "";
    str_full = false;
    Motor.update();
  }
  else if(input_str.equals("STOP\n")){
      
        Motor.transitionTo(STOP);
        }

  else {
    ;
  }

}
