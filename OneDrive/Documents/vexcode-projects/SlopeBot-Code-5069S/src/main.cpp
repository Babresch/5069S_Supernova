/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// left_intake          motor         14              
// right_intake         motor         17              
// hood                 digital_out   C               
// structure            digital_out   A               
// matchloader          digital_out   B               
// FL                   motor         16              
// ML                   motor         18              
// BL                   motor         19              
// FR                   motor         10              
// MR                   motor         13              
// BR                   motor         15              
// Inertial             inertial      9               
// Rotate               rotation      11              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

motor_group R= motor_group(FR, MR, BR);
motor_group L= motor_group(FL, ML, BL);

/*
██████╗░░█████╗░██████╗░░█████╗░████████╗  ░█████╗░░█████╗░███╗░░██╗████████╗██████╗░░█████╗░██╗░░░░░░██████╗
██╔══██╗██╔══██╗██╔══██╗██╔══██╗╚══██╔══╝  ██╔══██╗██╔══██╗████╗░██║╚══██╔══╝██╔══██╗██╔══██╗██║░░░░░██╔════╝
██████╔╝██║░░██║██████╦╝██║░░██║░░░██║░░░  ██║░░╚═╝██║░░██║██╔██╗██║░░░██║░░░██████╔╝██║░░██║██║░░░░░╚█████╗░
██╔══██╗██║░░██║██╔══██╗██║░░██║░░░██║░░░  ██║░░██╗██║░░██║██║╚████║░░░██║░░░██╔══██╗██║░░██║██║░░░░░░╚═══██╗
██║░░██║╚█████╔╝██████╦╝╚█████╔╝░░░██║░░░  ╚█████╔╝╚█████╔╝██║░╚███║░░░██║░░░██║░░██║╚█████╔╝███████╗██████╔╝
╚═╝░░╚═╝░╚════╝░╚═════╝░░╚════╝░░░░╚═╝░░░  ░╚════╝░░╚════╝░╚═╝░░╚══╝░░░╚═╝░░░╚═╝░░╚═╝░╚════╝░╚══════╝╚═════╝░
*/

  int R_percentage = 0;
  int L_percentage = 0;
  int axis_R_value = 0;
  int axis_L_value = 0;
  int axis_R = 0;
  int axis_L = 0;
  int R_speed = 0;
  int L_speed = 0;
  int R_run = 0;
  int L_run = 0;
  int t_value;
  int d_value;
  float maxStep = 6; // %   NEED TO TUNE old 2
  float ImaxStep = 8; // increase maxstep, less since we have acceleration dampening

  //percentage to voltage function
  float setVolt (float percentage)
      {
        // 0.12 = 12/100
        return (percentage * 0.12);
      }


  int drivetrain_controls()
  {

    while(true){

    d_value = Controller1.Axis3.position(pct);
    t_value = 0.85*Controller1.Axis1.position(pct);

    axis_R_value = (d_value + t_value);
    axis_L_value = (d_value - t_value);

  
      float maxValue = axis_R_value;
      if (maxValue < axis_L_value) {
      maxValue = axis_L_value;
    }

      if (maxValue > 100) {
        axis_R_value = (axis_R_value/maxValue) * 100;
        axis_L_value = (axis_L_value/maxValue) * 100;
      }
    

      if (abs(d_value) < 2 && abs(t_value) < 2)
      {
        R.stop(coast);
        L.stop(coast);
      }

    //fast
    //R_percentage = ((0.665*pow(axis_R_value, 3))/(7000))+0.05*(axis_R_value);
    //L_percentage = ((0.665*pow(axis_L_value, 3))/(7000))+0.05*(axis_L_value);

    //medium
    //R_percentage = (0.00001*pow(axis_R_value, 3))+(0.000000009*pow(axis_R_value, 5));
    //L_percentage = (0.00001*pow(axis_L_value, 3))+(0.000000009*pow(axis_L_value, 5));

    //slow BAD
    //R_percentage = (0.00007*pow(axis_R_value, 3))+(0.000000003*pow(axis_R_value, 5));
    //L_percentage = (0.00007*pow(axis_L_value, 3))+(0.000000003*pow(axis_L_value, 5));

    //NONE
    R_percentage =axis_R_value;
    L_percentage =axis_L_value;

    //acceleration dampening

    //right side dampening

    if (R_percentage > R_run + ImaxStep)
    {
      R_run += ImaxStep; 
    }
    else if (R_percentage < R_run - maxStep)
    {
      R_run -= maxStep;
    }
    else 
    {
      R_run = R_percentage;
    }

    //left side dampening
    if (L_percentage > L_run + ImaxStep)
    {
      L_run += ImaxStep; 
    }
    else if (L_percentage < L_run - maxStep)
    {
      L_run -= maxStep;
    }
    else 
    {
      L_run = L_percentage;
    }


    //keeps the values between (100, -100)
    //R side
    if (R_run > 100)
    {
      R_run = 100;
    }
    if (R_run < -100)
    {
      R_run = -100;
    }
    //L side
    if (L_run > 100)
    {
      L_run = 100;
    }
    if (L_run < -100)
    {
      L_run = -100;
    }

    int right_volt = setVolt(-R_run);
    int left_volt = setVolt(-L_run);

    //int right_volt = setVolt(-R_percentage);
    //int left_volt = setVolt(-L_percentage);

    //runnning the motors
    R.spin(fwd, right_volt, volt);
    L.spin(fwd, left_volt, volt);

    wait(10, msec);
    }
  }

int intake_controls()
  {
    while(true)
    {
      if (Controller1.ButtonR2.pressing())
      {
        right_intake.spin(fwd, 12, volt);
        left_intake.spin(fwd, 12, volt);
      }

      else if (Controller1.ButtonL2.pressing())
      {
        right_intake.spin(reverse, 12, volt);
      }
      
      else if (Controller1.ButtonL1.pressing())
      {
        right_intake.spin(reverse, 12, volt);
        left_intake.spin(reverse, 12, volt);
      }

      else if (Controller1.ButtonR1.pressing())
      {
        left_intake.spin(fwd, 12, volt);
      }

      else
      {
        right_intake.stop(brake);
        left_intake.stop(brake);
      }


    }
  }


int pneumatic_controls()
{
  int m_value = 0;
  int h_value = 0;
  int p_value = 0;

  while (1)
  {

  if(Controller1.ButtonX.pressing())
    {
      m_value = m_value + 1;
      if (Controller1.ButtonX.pressing() && m_value == 1)
      {
        structure.set(true);
      }
      else if (Controller1.ButtonX.pressing() && m_value == 2)
      {
        structure.set(false);
      }
      else
      {
        m_value = 1;
        structure.set(true);
      }
      wait(0.2, sec);
    }

    if(Controller1.ButtonA.pressing())
    {
      h_value = h_value + 1;
      if (Controller1.ButtonA.pressing() && h_value == 1)
      {
        hood.set(true);
      }
      else if (Controller1.ButtonA.pressing() && h_value == 2)
      {
        hood.set(false);
      }
      else
      {
        h_value = 1;
        hood.set(true);
      }
      wait(0.2, sec);
    }

    if(Controller1.ButtonUp.pressing())
    {
      p_value = p_value + 1;
      if (Controller1.ButtonUp.pressing() && p_value == 1)
      {
        matchloader.set(true);
      }
      else if (Controller1.ButtonUp.pressing() && p_value == 2)
      {
        matchloader.set(false);
      }
      else
      {
        p_value = 1;
        matchloader.set(true);
      }
      wait(0.2, sec);
    }
  }
}


void usercontrol(void) {
    task a (intake_controls);
    task b (pneumatic_controls);
    task c (drivetrain_controls);

    wait(20, msec);
}


void pre_auton(void) {
  {
  Inertial.calibrate(3);
  }
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

}

/*
░█▀▀▄ ░█▀▀█ ▀█▀ ░█──░█ ░█▀▀▀ 　 ░█▀▀█ ▀█▀ ░█▀▀▄ 
░█─░█ ░█▄▄▀ ░█─ ─░█░█─ ░█▀▀▀ 　 ░█▄▄█ ░█─ ░█─░█ 
░█▄▄▀ ░█─░█ ▄█▄ ──▀▄▀─ ░█▄▄▄ 　 ░█─── ▄█▄ ░█▄▄▀
*/

float Kp = 0.009; //oldish 10
float Kd = -1000; //old -1000 oldish 20
float kI = 0.003; //old .003

float error;
float derivative;
float integral;
float previouserror;
float currentvalue;
float output;

float speed_;
float dSpeed = 0;


void drivePD(float targetvalue){

Rotate.resetPosition ();
speed_ = 1;

while(true)
{

   if (fabs(targetvalue) < 2.5)
  {
    speed_ = 0.2;
    break;
  }

  currentvalue = Rotate.position(degrees) / 41.67;
    error = (targetvalue) - currentvalue;
    derivative = error - previouserror;
    integral += error;
    output = (error*Kp) + (derivative*Kd) + (integral*kI);
    previouserror = error;
    R.spin(reverse, 0.05*output*speed_, rpm); //old 0.05
    L.spin(reverse, 0.05*output*speed_, rpm); 
    if((fabs(error) < 1.0 && fabs(error - previouserror) < 0.5)){
      L.stop(brake);
      R.stop(brake);
    break;

          }         
     } 
}

/*
▀▀█▀▀ ░█─░█ ░█▀▀█ ░█▄─░█ 　 ░█▀▀█ ▀█▀ ░█▀▀▄ 
─░█── ░█─░█ ░█▄▄▀ ░█░█░█ 　 ░█▄▄█ ░█─ ░█─░█ 
─░█── ─▀▄▄▀ ░█─░█ ░█──▀█ 　 ░█─── ▄█▄ ░█▄▄▀
*/

float turnKp = 3.5;
float tKd = 10;
float tkI = 0;

float terror;
float tderivative;
float tintegral;
float tpreviouserror;
float tcurrentvalue;
float toutput;

void turnPD(float turntargetvalue){

Inertial.resetHeading();
wait(10, msec);

while(1)
{
    tcurrentvalue = Inertial.heading();
    terror = turntargetvalue - tcurrentvalue;
    if (terror>180){
      terror-=360;
    }
    if (terror<-180){
      terror+=360;
    }
    
    tderivative = terror - tpreviouserror;
    tintegral += terror;
    toutput = (terror*turnKp ) + (tderivative*tKd) + (tintegral*tkI);
    tpreviouserror = terror;
    R.spin(fwd, -toutput, rpm);
    L.spin(fwd, toutput, rpm);
    
    if((fabs(terror) < 1.0 && fabs(terror - tpreviouserror) < 0.5)){
      L.stop(brake);
      R.stop(brake);
      wait(.1, sec);
      break;
    }    
   }
}

motor_group A= motor_group(FR, MR, BR, FL, ML, BL);

void autonomous(void) {
  
  //TEST AUTON
  //drivePD(24);
  //turnPD(180);


/*
██╗░░░░░███████╗███████╗████████╗  ░█████╗░██╗░░░██╗████████╗░█████╗░███╗░░██╗
██║░░░░░██╔════╝██╔════╝╚══██╔══╝  ██╔══██╗██║░░░██║╚══██╔══╝██╔══██╗████╗░██║
██║░░░░░█████╗░░█████╗░░░░░██║░░░  ███████║██║░░░██║░░░██║░░░██║░░██║██╔██╗██║
██║░░░░░██╔══╝░░██╔══╝░░░░░██║░░░  ██╔══██║██║░░░██║░░░██║░░░██║░░██║██║╚████║
███████╗███████╗██║░░░░░░░░██║░░░  ██║░░██║╚██████╔╝░░░██║░░░╚█████╔╝██║░╚███║
╚══════╝╚══════╝╚═╝░░░░░░░░╚═╝░░░  ╚═╝░░╚═╝░╚═════╝░░░░╚═╝░░░░╚════╝░╚═╝░░╚══╝
*/

/*
//drive to the blocks
  matchloader.set(false);
  right_intake.spin(reverse, 100, pct);
  drivePD(10);
  drivePD(15);
  wait(.1, sec);
  R.spin(fwd, -50, rpm);
  L.spin(fwd, -50, rpm);
  wait(1.3, sec); //old 1.25
  R.stop(brake);
  L.stop(brake);

  wait(.2, sec);
  drivePD(1.5);

  //drive to the matchloader
  turnPD(-110);
  right_intake.stop(coast);

  wait(.1, sec);
  drivePD(12);
  drivePD(12);
  drivePD(15); //12
  wait(.2, sec);
  turnPD(-62);
  matchloader.set(true);
  wait(.2, sec);
  right_intake.spin(reverse, 100, pct);
  
  R.spin(fwd, -125, rpm);
  L.spin(fwd, -125, rpm);
  wait(1.25, sec); //1.4
  R.stop(coast);
  L.stop(coast);
  /*
  R.spin(fwd, -10, rpm);
  L.spin(fwd, -10, rpm);
  wait(.001, sec);
  R.stop(coast);
  L.stop(coast);

  wait(.05, sec);

  R.spin(fwd, 10, rpm);
  L.spin(fwd, 10, rpm);
  wait(.001, sec);
  R.stop(coast);
  L.stop(coast);
*/

  //drive to the long goal
  /*
  wait(0.4, sec);
  drivePD(-10);
  drivePD(-11);
  drivePD(-9);
  matchloader.set(false);
  drivePD(-8.5);
  right_intake.spin(fwd, 12, volt);
  left_intake.spin(fwd, 12, volt);
  wait(0.1, sec);
  left_intake.spin(reverse, 600, rpm);
  right_intake.stop(coast);
  right_intake.spin(reverse, 600, rpm);
  wait(2, sec);
  drivePD(10);

/*
██████╗░██╗░██████╗░██╗░░██╗████████╗  ░█████╗░██╗░░░██╗████████╗░█████╗░███╗░░██╗
██╔══██╗██║██╔════╝░██║░░██║╚══██╔══╝  ██╔══██╗██║░░░██║╚══██╔══╝██╔══██╗████╗░██║
██████╔╝██║██║░░██╗░███████║░░░██║░░░  ███████║██║░░░██║░░░██║░░░██║░░██║██╔██╗██║
██╔══██╗██║██║░░╚██╗██╔══██║░░░██║░░░  ██╔══██║██║░░░██║░░░██║░░░██║░░██║██║╚████║
██║░░██║██║╚██████╔╝██║░░██║░░░██║░░░  ██║░░██║╚██████╔╝░░░██║░░░╚█████╔╝██║░╚███║
╚═╝░░╚═╝╚═╝░╚═════╝░╚═╝░░╚═╝░░░╚═╝░░░  ╚═╝░░╚═╝░╚═════╝░░░░╚═╝░░░░╚════╝░╚═╝░░╚══╝
*/

//drive to the blocks
  matchloader.set(false);
  right_intake.spin(reverse, 100, pct);
  drivePD(10);
  drivePD(15);
  wait(.1, sec);
  R.spin(fwd, -50, rpm);
  L.spin(fwd, -50, rpm);
  wait(1.3, sec); //old 1.25
  R.stop(brake);
  L.stop(brake);

  wait(.2, sec);
  drivePD(1.5);

  //drive to the matchloader
  turnPD(110);
  right_intake.stop(coast);

  wait(.1, sec);
  drivePD(12);
  drivePD(12);
  drivePD(15); //12
  wait(.2, sec);
  turnPD(62);
  matchloader.set(true);
  wait(.2, sec);
  right_intake.spin(reverse, 100, pct);
  
  R.spin(fwd, -125, rpm);
  L.spin(fwd, -125, rpm);
  wait(1.25, sec); //1.4
  R.stop(coast);
  L.stop(coast);
  /*
  R.spin(fwd, -10, rpm);
  L.spin(fwd, -10, rpm);
  wait(.001, sec);
  R.stop(coast);
  L.stop(coast);

  wait(.05, sec);

  R.spin(fwd, 10, rpm);
  L.spin(fwd, 10, rpm);
  wait(.001, sec);
  R.stop(coast);
  L.stop(coast);
*/

  //drive to the long goal
  wait(0.4, sec);
  drivePD(-10);
  drivePD(-11);
  drivePD(-9);
  matchloader.set(false);
  drivePD(-8.5);
  right_intake.spin(fwd, 12, volt);
  left_intake.spin(fwd, 12, volt);
  wait(0.1, sec);
  left_intake.spin(reverse, 600, rpm);
  right_intake.stop(coast);
  right_intake.spin(reverse, 600, rpm);
  wait(2, sec);
  drivePD(10);

/*
░██████╗██╗░░██╗██╗██╗░░░░░██╗░░░░░░██████╗  ░█████╗░██╗░░░██╗████████╗░█████╗░███╗░░██╗
██╔════╝██║░██╔╝██║██║░░░░░██║░░░░░██╔════╝  ██╔══██╗██║░░░██║╚══██╔══╝██╔══██╗████╗░██║
╚█████╗░█████═╝░██║██║░░░░░██║░░░░░╚█████╗░  ███████║██║░░░██║░░░██║░░░██║░░██║██╔██╗██║
░╚═══██╗██╔═██╗░██║██║░░░░░██║░░░░░░╚═══██╗  ██╔══██║██║░░░██║░░░██║░░░██║░░██║██║╚████║
██████╔╝██║░╚██╗██║███████╗███████╗██████╔╝  ██║░░██║╚██████╔╝░░░██║░░░╚█████╔╝██║░╚███║
╚═════╝░╚═╝░░╚═╝╚═╝╚══════╝╚══════╝╚═════╝░  ╚═╝░░╚═╝░╚═════╝░░░░╚═╝░░░░╚════╝░╚═╝░░╚══╝
*/


/*
//drive to the blocks
  matchloader.set(false);
  right_intake.spin(reverse, 100, pct);
  drivePD(10);
  drivePD(15);
  wait(.1, sec);
  R.spin(fwd, -50, rpm);
  L.spin(fwd, -50, rpm);
  wait(1.3, sec); //old 1.25
  R.stop(brake);
  L.stop(brake);

  wait(.2, sec);
  drivePD(1.5);

  //drive to the matchloader
  turnPD(-110);
  right_intake.stop(coast);

  wait(.1, sec);
  drivePD(12);
  drivePD(12);
  drivePD(15); //12
  wait(.2, sec);
  turnPD(-62);
  matchloader.set(true);
  wait(.2, sec);
  right_intake.spin(reverse, 100, pct);
  
  R.spin(fwd, -125, rpm);
  L.spin(fwd, -125, rpm);
  wait(1.25, sec); //1.4
  R.stop(coast);
  L.stop(coast);
  /*
  R.spin(fwd, -10, rpm);
  L.spin(fwd, -10, rpm);
  wait(.001, sec);
  R.stop(coast);
  L.stop(coast);

  wait(.05, sec);

  R.spin(fwd, 10, rpm);
  L.spin(fwd, 10, rpm);
  wait(.001, sec);
  R.stop(coast);
  L.stop(coast);
*/

  //drive to the long goal
  /*
  wait(0.4, sec);
  drivePD(-10);
  drivePD(-11);
  drivePD(-9);
  matchloader.set(false);
  drivePD(-8.5);
  right_intake.spin(fwd, 12, volt);
  left_intake.spin(fwd, 12, volt);
  wait(0.1, sec);
  left_intake.spin(reverse, 600, rpm);
  right_intake.stop(coast);
  right_intake.spin(reverse, 600, rpm);
  wait(2, sec);
  drivePD(10);

  /*
  wait(.5, sec);
  drivePD(10);
  turnPD(135);
  drivePD(-35);
  turnPD(-45);
  drivePD(-25);
  */


//drive to second matchloader
/*
drivePD(15);
wait(.1,sec);
turnPD(90);
drivePD(-32);
drivePD(-25);
drivePD(-25);
drivePD(-25);
turnPD(-90);
matchloader.set(false);
right_intake.spin(reverse, 100, pct);
wait(.1, sec);
drivePD(10);
wait(2,sec);
//drivePD(-20);
//drivePD(-16);
wait(1, sec);
drivePD(-10);
drivePD(-11);
drivePD(-10.8);
matchloader.set(true);
drivePD(-8.5);
left_intake.spin(reverse, 600, rpm);
right_intake.stop(coast);
right_intake.spin(reverse, 600, rpm);

//skills park
drivePD(10);
turnPD(-135);
drivePD(60);
turnPD(45);
drivePD(40);

*/
//NEW AUTON
//drivePD(24); 
//wait(1, sec);
//drivePD(12);
//drivePD(12);
/*
turnPD(-90);
drivePD(4);
right_intake.spin(reverse, 12, volt);
drivePD(1);
wait(4, sec);





/*
█▀▀ █▀▀█ █▀▀ █▀▀ 　 █▀▀█ █░░█ ▀▀█▀▀ █▀▀█ █▀▀▄ 
█▀▀ █▄▄▀ █▀▀ █▀▀ 　 █▄▄█ █░░█ ░░█░░ █░░█ █░░█ 
▀░░ ▀░▀▀ ▀▀▀ ▀▀▀ 　 ▀░░▀ ░▀▀▀ ░░▀░░ ▀▀▀▀ ▀░░▀
*/

  //drivePD(8);
  /*
  right_intake.spin(fwd, 12, volt);
  matchloader.set(false);
  wait(0.8, sec);
  R.spin(reverse, 200, rpm);
  L.spin(reverse, 200, rpm);
  wait(1.64, sec);
  R.stop(brake);
  L.stop(brake);
  right_intake.stop(brake);
*/
}


int main() {

  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    wait(100, msec);
  }
}
