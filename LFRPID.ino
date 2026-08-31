

#include <QTRSensors.h>
#include <SparkFun_TB6612.h>


#define AIN1 5
#define BIN1 7
#define AIN2 4
#define BIN2 8
#define PWMA 3
#define PWMB 9
#define STBY 6

const int offsetA = 1;
const int offsetB = 1;

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

QTRSensors qtr;


const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
int max_speed =  110 ;// Change this var to change bot max speed
int turn = 30;//45 works
int L = 0;
int R = 0;
int error = 0;
int adj = 0;

float Kp = 0.08;
float Ki = 0;  //0.0090
float Kd = 4;//1

int P;
int I;
int D;
int lastError = 0;

uint16_t position;
void setup() {
  brake(motor1, motor2);

  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){
                      A7, A6, A5, A4, A3, A2, A1, A0 },
                    SensorCount);
  for (uint16_t i = 0; i < 200; i++) {
    qtr.calibrate();//Auto Threshold Calibration
  }
  Serial.println("Done");
  Serial.begin(9600);
}

void loop() {
  // forward(225,225);
  PID_control();
  // checksensor();
}
void PID_control() { // PID algo
  position = qtr.readLineBlack(sensorValues);
  //int mappedvalue = map(position, 45, 994, 0, 1023);
  //Serial.print("Actual Position:");
  //Serial.println(position);
  //Serial.println("Mapped Value:");
  //Serial.println(position);
  error = 3500 - position;
//  if (position > 1000 && position < 6000) {
    P = error;
    //I = I + error;
    D = error - lastError;
    lastError = error;

    adj = P * Kp + D * Kd;

    L = max_speed + adj;
    R = max_speed - adj;

    if (L > max_speed) {
      L = max_speed;
    }
    if (R > max_speed) {
      R = max_speed;
    }
    if (L < 0) {
      L = 0;
    }
    if (R < 0) {
      R = 0;
    }
    forward(R, L);
  }
  //  else{
  //   sharp_left();
  // }

  //  else if (position >=6000 && position <= 7000) {
  //   sharp_right();
  // }
  // //else{
  //   //sharp_right();
  // //}
  




void forward(int L, int R) {
  motor1.drive(L);
  motor2.drive(R);
}
void sharp_right() {
  motor1.drive(-255);
  motor2.drive(255);
}
void sharp_left() {
  motor1.drive(255);
  motor2.drive(-255);
}

