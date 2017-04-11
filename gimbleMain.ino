#include <L3G4200D.h>
#include <MatrixMath.h>
#include <Wire.h>
#include <ADXL345.h>
#include <stdarg.h>
#include <VarSpeedServo.h>
ADXL345 accelerometer;
L3G4200D gyro;
VarSpeedServo servo1;
VarSpeedServo servo2;
//VarSpeedServo servo3;

/*-------gyroscope initialization------*/
float pitch = 0;
float roll = 0;
float yaw = 0;

/*-------PID initializaiton------*/

float sampleTime = 0.01;
unsigned long timer = millis(); // Time now c
unsigned long  increment = 0;
unsigned long motorWrite = 0;
//float pwmMax = 100; // PWM saturation 
//float pwmMin = 0;
//float equilibrium_x = 0.0;
//float equilibrium_theta = 0.0  ;
//float equilibrium_x_dot = 0.0;
//float equilibrium_theta_dot = 0.0;
//double Kp = 25, Ki = 0.1*1, Kd =0.01;

float dt = 0.01;

/*-------------Setup------------*/
void setup()
{

  //Serial prints for debugging and testing
  Serial.begin(9600);
/*-------Servo motor setup------*/
  // set all the motor control pins to outputs
  servo1.attach(9);
  servo2.attach(10);
//  servo3.attach(11);
  servo1.write(0,255,true);
  servo2.write(0,255,true);
//  servo3.write(0,255,true);
  

/*-------Accelerometer setup------*/
 // Initialize ADXL345
  Serial.println("Initialize ADXL345");

  if (!accelerometer.begin())
  {
    Serial.println("Could not find a valid ADXL345 sensor, check wiring!");
    delay(500);
  }
  // Set measurement range
  // +/-  2G: ADXL345_RANGE_2G
  // +/-  4G: ADXL345_RANGE_4G
  // +/-  8G: ADXL345_RANGE_8G
  // +/- 16G: ADXL345_RANGE_16G
  accelerometer.setRange(ADXL345_RANGE_16G);
  
/*-------gyroscope setup------*/
  Serial.println("Initialize L3G4200D");
    // Set scale 2000 dps and 400HZ Output data rate (cut-off 50)
  while(!gyro.begin(L3G4200D_SCALE_2000DPS, L3G4200D_DATARATE_400HZ_50))
  {
    Serial.println("Could not find a valid L3G4200D sensor, check wiring!");
    delay(500);
  }
 
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  gyro.calibrate(100);
  
}

/*-------Main loop ------*/
void loop()
{
  timer = millis();
/*-------Accelerometer Reading------*/
  // Read normalized values
  Vector norm = accelerometer.readNormalize();

  // Low Pass Filter to smooth out data. 0.1 - 0.9
  Vector filtered = accelerometer.lowPassFilter(norm, 0.5);
 
  // Calculate Pitch & Roll (Low Pass Filter)
  int fpitch = -(atan2(filtered.XAxis, sqrt(filtered.YAxis*filtered.YAxis + filtered.ZAxis*filtered.ZAxis))*180.0)/M_PI;
  int froll  = (atan2(filtered.YAxis, filtered.ZAxis)*180.0)/M_PI;
//  Serial.print("Accel pitch = ");
  Serial.println(fpitch);
//  Serial.print("Accel roll = ");
//  Serial.println(froll);

/*-------gyroscope Reading------*/
  Vector norm_gyro = gyro.readNormalize();
   pitch = pitch + norm_gyro.YAxis*sampleTime;
   roll = roll + norm_gyro.XAxis*sampleTime;
   yaw = yaw + norm_gyro.ZAxis*sampleTime;

   Serial.print(" Yaw = ");
   Serial.println(yaw);

   //Wait to full timeStep period
  delay((sampleTime*1000)-(millis()-timer));
  


/*------------Motor Control ---------------*/
    
    servo1.write(-fpitch + 90,255,false);
    servo2.write(-froll + 90,255,false);

    
/*------------PID ---------------*/



}//main loop

