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

  //test test
  int R_percentage = 0;
  int L_percentage = 0;
  int axis_R = 0;
  int axis_L = 0;
  int R_speed = 0;
  int L_speed = 0;
  int R_run = 0;
  int L_run = 0;
  float maxStep = 5; // %   NEED TO TUNE

  int drivetrain_controls()
  {
    //Axis 3 is up/down
    //Axis 1 is left/right
    while(true){

    //old is 0.7 for axis 1
    // determines the value of the joystick
    axis_R = (Controller1.Axis3.position(pct) + 0.5*Controller1.Axis1.position(pct));
    axis_L = (Controller1.Axis3.position(pct) - 0.5*Controller1.Axis1.position(pct));

      //rejects low values to stop static jitter
      if (abs(axis_R) < 5)
      {
        axis_R = 0;
      }
      if (abs(axis_L) < 5)
      {
        axis_L = 0;
      }

      //the rpm of both motor groups (used for dampening)
      R_speed = (R.velocity(rpm) / 6);
      L_speed = (L.velocity(rpm) / 6);

    //R_percentage = ((0.665*pow(axis_R, 3))/(7000))+0.05*(axis_R);
    //L_percentage = ((0.665*pow(axis_L, 3))/(7000))+0.05*(axis_L);

    //new exponential function
    R_percentage = (0.00001*pow(axis_R, 3))+(0.000000009*pow(axis_R, 5));
    L_percentage = (0.00001*pow(axis_L, 3))+(0.000000009*pow(axis_L, 5));

    //OR

    //R_percentage = (0.00007*pow(axis_R, 3))+(0.000000003*pow(axis_R, 5));
    //L_percentage = (0.00007*pow(axis_L, 3))+(0.000000003*pow(axis_L, 5));


    //acceleration dampening
    //check that rate at which the joystick is moved
    double rate_R = axis_R - R_speed;
    double rate_L = axis_R - R_speed;

    //rate of change modifier
    float s = 0.1;                  //NEED TO TUNE
      R_percentage -= rate_R * s;
      L_percentage -= rate_L * s;

    //right side dampening
    if (R_percentage > R_run + maxStep)
    {
      R_run += maxStep; 
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
    if (L_percentage > L_run + maxStep)
    {
      L_run += maxStep; 
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

    //runnning the motors
    R.spin(fwd, -R_run, pct);
    L.spin(fwd, -L_run, pct);

    wait(10, msec);


    /* BAD
    // only runs the drivetrain normally if a value is present (used to avoid conflict with the dampening)
    if (Controller1.Axis3.position(pct) != 0 && Controller1.Axis1.position(pct) != 0)
    {
    R.spin(fwd, -R_percentage, pct);
    L.spin(fwd, -L_percentage, pct);
    }

    //if the joystick is moved to zero the deceleration is damped
    if (Controller1.Axis3.position(pct) == 0 && Controller1.Axis1.position(pct) == 0)
    {
      R_percentage = 0.75 * R_speed;
      L_percentage = 0.75 * L_speed;

      R.spin(fwd, -R_percentage, pct);
      L.spin(fwd, -L_percentage, pct);
      
      }
    */
  }
}


int intake_controls()
  {
    while(true)
    {
      if (Controller1.ButtonR2.pressing())
      {
        right_intake.spin(fwd, 600, rpm);
        left_intake.spin(fwd, 600, rpm);
      }

      else if (Controller1.ButtonL2.pressing())
      {
        right_intake.spin(reverse, 600, rpm);
      }
      
      else if (Controller1.ButtonL1.pressing())
      {
        right_intake.spin(reverse, 600, rpm);
        left_intake.spin(reverse, 600, rpm);
      }

      else if (Controller1.ButtonR1.pressing())
      {
        left_intake.spin(fwd, 600, rpm);
      }

      else
      {
        right_intake.stop(coast);
        left_intake.stop(coast);
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

float Kp = .009;
float Kd = -1000;
float kI = .003;

float error;
float derivative;
float integral;
float previouserror;
float currentvalue;
float output;

float speed_;


void drivePD(float targetvalue){

Rotate.resetPosition ();
error = 0;
speed_ = 1;

while(true)
{
  // radius = 2.75/2 = 1.375in
  //360 deg = 1 rotation, 1 rot = 2pi*r = 8.64in for circumference
  //360/8.64 = 41.67
/*
  if (fabs(targetvalue) < 10)
  {
    speed_ = 0.5;
    break;
  }
*/
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
    R.spin(reverse, 0.05*output*speed_, rpm);
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
  structure.set(true);
  drivePD(15);
  wait(.1, sec);
  R.spin(fwd, -25, rpm);
  L.spin(fwd, -25, rpm);
  wait(2.5, sec);
  R.stop(brake);
  L.stop(brake);

  wait(.8, sec);
  drivePD(1.5);

  //drive to the matchloader
  turnPD(-110);
  right_intake.stop(coast);

  wait(.1, sec);
  drivePD(12);
  drivePD(12);
  drivePD(14);
  wait(.05, sec);
  turnPD(-62);
  matchloader.set(true);
  wait(.8, sec);
  right_intake.spin(reverse, 100, pct);
  
  R.spin(fwd, -125, rpm);
  L.spin(fwd, -125, rpm);
  wait(1.4, sec);
  R.stop(coast);
  L.stop(coast);

  R.spin(fwd, -10, rpm);
  L.spin(fwd, -10, rpm);
  wait(.4, sec);
  R.stop(coast);
  L.stop(coast);

  wait(.3, sec);

  R.spin(fwd, -10, rpm);
  L.spin(fwd, -10, rpm);
  wait(.4, sec);
  R.stop(coast);
  L.stop(coast);

  //drive to the long goal
  wait(1, sec);
  structure.set(false);
  drivePD(-10);
  drivePD(-11);
  drivePD(-10.8);
  matchloader.set(false);
  drivePD(-8.5);
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
/*
  matchloader.set(false);
  right_intake.spin(reverse, 100, pct);
  drivePD(10);
  structure.set(true);
  drivePD(15);
  wait(.1, sec);
  R.spin(fwd, -25, rpm);
  L.spin(fwd, -25, rpm);
  wait(2.5, sec);
  R.stop(brake);
  L.stop(brake);

  wait(.8, sec);
  drivePD(1.5);

  //drive to the matchloader
  turnPD(110);
  right_intake.stop(coast);

  wait(.1, sec);
  drivePD(12);
  drivePD(12);
  drivePD(14);
  wait(.05, sec);
  turnPD(62);
  matchloader.set(true);
  wait(.8, sec);
  right_intake.spin(reverse, 100, pct);

  //TOO LONG

  R.spin(fwd, -125, rpm);
  L.spin(fwd, -125, rpm);
  wait(1.4, sec);
  R.stop(coast);
  L.stop(coast);

  R.spin(fwd, -10, rpm);
  L.spin(fwd, -10, rpm);
  wait(.4, sec);
  R.stop(coast);
  L.stop(coast);

  wait(.3, sec);

  R.spin(fwd, -10, rpm);
  L.spin(fwd, -10, rpm);
  wait(.4, sec);
  R.stop(coast);
  L.stop(coast);
//////////////////////////////////////////////////////////////////////////////////


  //NEW NEED TO TEST
  /*
  R.spin(fwd, -600, rpm);
  L.spin(fwd, -600, rpm);
  wait(.4, sec);
  R.stop(coast);
  L.stop(coast);

  R.spin(fwd, -125, rpm);
  L.spin(fwd, -125, rpm);
  wait(1, sec);
  R.stop(coast);
  L.stop(coast);
  */

  //drive to the long goal
  /*
  wait(1, sec);
  structure.set(false);
  drivePD(-10);
  drivePD(-11);
  drivePD(-10.8);
  matchloader.set(false);
  drivePD(-8.5);
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

//drive to the blocks
  matchloader.set(false);
  right_intake.spin(reverse, 100, pct);
  drivePD(10);
  structure.set(true);
  drivePD(17.5);
  wait(.1, sec);
  R.spin(fwd, -25, rpm);
  L.spin(fwd, -25, rpm);
  wait(2.2, sec); //old 1.9
  R.stop(brake);
  L.stop(brake);

  //turnPD(-15);
  wait(.8, sec);
  drivePD(1.5);

  //drive to the matchloader
  turnPD(-110);
  right_intake.stop(coast);

  wait(.1, sec);
  drivePD(12);
  drivePD(12);
  drivePD(14);
  wait(.05, sec);
  turnPD(-58);
  matchloader.set(true);
  wait(.8, sec);
  right_intake.spin(reverse, 100, pct);
  //drivePD(7);
//TOO LONG
/*
  R.spin(fwd, -125, rpm);
  L.spin(fwd, -125, rpm);
  wait(1.4, sec);
  R.stop(coast);
  L.stop(coast);

  R.spin(fwd, -10, rpm);
  L.spin(fwd, -10, rpm);
  wait(.4, sec);
  R.stop(coast);
  L.stop(coast);

  wait(.3, sec);

  R.spin(fwd, -10, rpm);
  L.spin(fwd, -10, rpm);
  wait(.4, sec);
  R.stop(coast);
  L.stop(coast);
*/

//NEW
  R.spin(fwd, -600, rpm);
  L.spin(fwd, -600, rpm);
  wait(.4, sec);
  R.stop(coast);
  L.stop(coast);

  R.spin(fwd, -125, rpm);
  L.spin(fwd, -125, rpm);
  wait(.5, sec);
  R.stop(coast);
  L.stop(coast);

  //drive to the long goal and score
  wait(1, sec);
  structure.set(false);
  drivePD(-10);
  drivePD(-11);
  drivePD(-10.8);
  matchloader.set(false);
  drivePD(-8.5);
  left_intake.spin(reverse, 600, rpm);
  right_intake.stop(coast);
  right_intake.spin(reverse, 600, rpm);

  wait(.5, sec);
  drivePD(10);
  turnPD(135);
  drivePD(-35);
  turnPD(-45);
  drivePD(-25);

/*
//drive to second matchloader
drivePD(15);
structure.set(true);
wait(.1,sec);
turnPD(90);
drivePD(-32);
drivePD(-25);
drivePD(-25);
drivePD(-25);
turnPD(-90);
matchloader.set(true);
right_intake.spin(reverse, 100, pct);
wait(.1, sec);
drivePD(10);
wait(2,sec);
//drivePD(-20);
//drivePD(-16);
wait(1, sec);
structure.set(false);
drivePD(-10);
drivePD(-11);
drivePD(-10.8);
matchloader.set(false);
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



/*
█▀▀ █▀▀█ █▀▀ █▀▀ 　 █▀▀█ █░░█ ▀▀█▀▀ █▀▀█ █▀▀▄ 
█▀▀ █▄▄▀ █▀▀ █▀▀ 　 █▄▄█ █░░█ ░░█░░ █░░█ █░░█ 
▀░░ ▀░▀▀ ▀▀▀ ▀▀▀ 　 ▀░░▀ ░▀▀▀ ░░▀░░ ▀▀▀▀ ▀░░▀
*/

  //drivePD(10);

  /*
  R.spin(fwd, 200, rpm);
  L.spin(fwd, 200, rpm);
  wait(1.1, sec);
  R.stop(brake);
  L.stop(brake);
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
