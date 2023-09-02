#include <Servo.h>
#include <math.h>

// The input pins for the potentiometers
int inputPin1 = A0;
int inputPin2 = A1;

// The output pins for the servo motors
int outputPin1 = 9;
int outputPin2 = 10;
int outputPin3 = 11;

// The setpoints for the angles
int setpoint1 = 75;
int setpoint2 = 97;

// The PID constants for the two degrees of freedom
float kp1 = 0.5;
float ki1 = 0.1;
float kd1 = 0.01;

float kp2 = 0.5;
float ki2 = 0.1;
float kd2 = 0.01;

// The variables for the PID computation
float error1, lastError1, integral1, derivative1, output1;
int coutput1;

float error2, lastError2, integral2, derivative2, output2;
int coutput2;

//CONSTANTS

const float pi = 3.14159265358979323846264338327950288;

// Initial offset distance from the ground
float h0 = 0.04; // in meter

// Gravitational acceleration
float g = 9.81; // in m/s^2

// Initial velocity
float u = 3.4; // in m/s

// Length of the cannon mouth
float l = 0.083; // in meter

// Offset distance from the vertical axis of rotation
float x0 = 0.0325; // in meter

// Servo motor constants
// Servo1 offset angle
float offset_angle1 = -12.7;

// Servo2 offset angle
float offset_angle2 = 97.0;

// Servo1 bounds
float servo1low = 19.0 + offset_angle1;
float servo1high = 150.0 + offset_angle1;

// Servo2 bound
float servo2low = -16.0 * pi / 180.0;
float servo2high = 45.0 * pi / 180.0;

// Potentiometer constants
int pot1min = 155;
int pot1max = 853;

int pot2min = 158;
int pot2max = 876;


// The servo objects
Servo servo1;
Servo servo2;
Servo trigger;

// function declaration
float calc_setpoint1(float x, float y);
float calc_range(float th);
float calc_setpoint2(float x, float y);
void pid(int setpoint1, int setpoint2);

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);

  // Attach the servo motors to their output pins
  servo1.attach(outputPin1);
  servo2.attach(outputPin2);
  trigger.attach(outputPin3);

  // Set the initial positions of the servo motors to their setpoints
  servo1.write(setpoint1);
  servo2.write(setpoint2);
  trigger.write(90);

  // Set the input pins as inputs
  pinMode(inputPin1, INPUT);
  pinMode(inputPin2, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  // // take pid parameters
  // Serial.println("K");
  // while(Serial.available() == 0) delay(10);
  // kp1 = kp2 = Serial.parseFloat();
  // ki1 = ki2 = Serial.parseFloat();
  // kd1 = kd2 = Serial.parseFloat();
  // while(Serial.available()) Serial.read();

  // take the elevation offset angle
  Serial.println("E");
  while(Serial.available() == 0) delay(10);
  offset_angle2 = Serial.parseFloat();
  while(Serial.available()) Serial.read();

  // take the speed
  Serial.println("S");
  while(Serial.available() == 0) delay(10);
  u = Serial.parseFloat();
  while(Serial.available()) Serial.read();

  // take the horizontal offset angle
  Serial.println("H");
  while(Serial.available() == 0) delay(10);
  offset_angle1 = Serial.parseFloat();
  while(Serial.available()) Serial.read();
}


void loop() {
    Serial.println("R");
    while (Serial.available() == 0) delay(10);
    float x = Serial.parseFloat();
    float y = Serial.parseFloat();
    while (Serial.available()) Serial.read();

    setpoint1 = round(calc_setpoint1(x, y));
    setpoint2 = round(calc_setpoint2(x, y));

    pid(setpoint1, setpoint2);

    trigger.write(60);
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(500);
    trigger.write(90);
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    servo1.write(75);
    servo2.write(115);
}

float calc_setpoint1(float x, float y) {
  float range = sqrt(x*x + y*y);
  float correction_angle = degrees(asin(x0/range)) + offset_angle1;
  if (x == 0) return 90.0 + correction_angle;
  float angle1 = degrees(atan(y / x));
  if (x < 0) return min(angle1 + 180.0 + correction_angle, servo1high);
  return max(angle1 + correction_angle, servo1low);
}

float calc_range(float th) {
  float sinth = sin(th), costh = cos(th);
  float range = u * costh * (u * sinth + sqrt(u * u * sinth * sinth + 2 * g * h0 + 2 * g * l * sinth)) / g + l * costh;
  return range;
}

float calc_setpoint2(float x, float y) {
  float target_range = sqrt(x*x + y*y - x0*x0);
  float l = servo2low, r = servo2high;
  while (abs(l - r) > 0.001) {
    float mid = (l + r) / 2;
    if (calc_range(mid) < target_range) {
      l = mid;
    } else {
      r = mid;
    }
  }
  return degrees(l) + offset_angle2;
}

void pid(int setpoint1, int setpoint2) {
  // Read the input values from the potentiometers
  int input1 = analogRead(inputPin1);
  int input2 = analogRead(inputPin2);

  // Convert the input values to angles
  int angle1 = map(input1, pot1min, pot1max, 0, 180);
  int angle2 = map(input2, pot2min, pot2max, 0, 180);

  // Initialize the PID variables
  lastError1 = 0;
  integral1 = 0;
  lastError2 = 0;
  integral2 = 0;
  int cnt = 0;
  while (true) {

    // Read the input values from the potentiometers
    input1 = analogRead(inputPin1);
    input2 = analogRead(inputPin2);

    // Convert the input values to angles
    angle1 = map(input1, pot1min, pot1max, 0, 180);
    angle2 = map(input2, pot2min, pot2max, 0, 180);

    if (setpoint1 != angle1 || setpoint2 != angle2){
      cnt = 0;
    }
    else{
      ++cnt;
      if(cnt == 7) break;
    }

    // Compute the errors for the angles
    error1 = setpoint1 - angle1;
    error2 = setpoint2 - angle2;


    // Compute the integral and derivative terms for the errors
    integral1 += error1;
    derivative1 = error1 - lastError1;
    integral2 += error2;
    derivative2 = error2 - lastError2;

    // Compute the control output values for the angles using PID control
    output1 = kp1 * error1 + ki1 * integral1 + kd1 * derivative1;
    output2 = kp2 * error2 + ki2 * integral2 + kd2 * derivative2;

    // Limit the output values of the controller
    coutput1 = constrain(output1, -30, 30);
    coutput2 = constrain(output2, -30, 30);

    // Update the positions of the servo motors
    servo1.write(setpoint1 + coutput1);
    servo2.write(setpoint2 + coutput2);

    // Print the input, setpoint, and output values to the serial monitor
    Serial.print("P ");
    Serial.print(setpoint1 - offset_angle1);
    Serial.print(" ");
    Serial.print(angle1 - offset_angle1);
    Serial.print(" ");
    Serial.print(setpoint2 - offset_angle2);
    Serial.print(" ");
    Serial.println(angle2 - offset_angle2);

    // Update the last error values for the angles
    lastError1 = error1;
    lastError2 = error2;

    // Delay for a short period of time to allow the servo motors to move
    delay(20);
  }
  Serial.println("D");
}