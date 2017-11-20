#include <stdio.h>
#include <stdlib.h>
#include <Wire.h>

const int SIGNAL_MAX = 255;//working PWM range: 100-255
const int SIGNAL_MIN = 100;
const int INITIAL_VALUE = 0;

// PID parameters
double Kp = 2.5;   // 2.5 //testing required to fine tune PID constants
double Ki = 0.2;   // 1.0
double Kd = 8.0;   // 8.0
double K  = 1.9 * 1.12;


// Complimentary Filter parameters
double K0 = (double) 0.40;//percentage of gyroscopic value influence on final motor output 
double K1 = (double) 0.60;//percentage of accelerometer/ rotation value's influence on final motor output

int fd;
int16_t acclX, acclY, acclZ;
int16_t gyroX, gyroY, gyroZ;
double accl_scaled_x, accl_scaled_y, accl_scaled_z;
double gyro_scaled_x, gyro_scaled_y, gyro_scaled_z;


double gyro_offset_x, gyro_offset_y;
double gyro_total_x, gyro_total_y;
double gyro_x_delta, gyro_y_delta;
double rotation_x, rotation_y;
double last_x, last_y;

double GUARD_GAIN = 255;
double error, last_error, integrated_error;
double pTerm, iTerm, dTerm;
int const memorySize = 5;
double dMem[memorySize];
double dAvg;
double angle;
double angle_offset = -5;  //1.5

double speed;
double pid_out;
double dir;

unsigned long timer, t, deltaT;

void pid(){
  error = 10;//instead of using last_x- angle offset, we use a manual value
  integrated_error=30;//completely arbitrary value for testing PID output
  last_value=8;//arbitrary value for testing
  pTerm = Kp * error;
  
  integrated_error = 0.95 * integrated_error + error;
  iTerm = Ki * integrated_error;

  dTerm = Kd * (error - last_error);
  dAvg = dTerm;
  //Serial.write("Values: "); Serial.print(dAvg);
  for(int i = 0; i < memorySize - 1; i ++)
  {
    //Serial.write(" "); Serial.print(dMem[i]);
    dAvg += dMem[i];
    dMem[i] = dMem[i+1];
  }
  
  dAvg += dMem[memorySize - 1];
  dMem[memorySize - 1] = dTerm;

  dAvg /= memorySize;
  //Serial.write("Average: "); Serial.print(dAvg); Serial.write("\n");
  last_error = error;
  
  pid_out = K * (pTerm + iTerm + dTerm);
  //dir = pid_out / abs(pid_out);//direction dependent on sign of PID value

  /*if(abs(pid_out) < 30)
  {
    speed = 0; //if the PID value is smaller than 30 it is negligible enough to not require rebalancing and thus a zero speed value is asserted
  }else if(pid_out > 0) 
  {
    speed = constrain(pid_out, SIGNAL_MIN, SIGNAL_MAX);//makes sure the output does not go beyond the allowable PWM range
  }else if(pid_out < 0)
  {
    speed = constrain(pid_out, -SIGNAL_MAX, -SIGNAL_MIN);//makes sure the output does not go beyond the allowable PWM range
  }*/
}
void test(){
  if(pid_out==208.544){
    System.out.println("Test Passed");
    }
  else{
  System.out.println("Test Failed");
  }
}
