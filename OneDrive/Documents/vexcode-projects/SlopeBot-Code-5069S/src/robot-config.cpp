#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor left_intake = motor(PORT14, ratio6_1, true);
motor right_intake = motor(PORT17, ratio18_1, false);
digital_out hood = digital_out(Brain.ThreeWirePort.C);
digital_out structure = digital_out(Brain.ThreeWirePort.A);
digital_out matchloader = digital_out(Brain.ThreeWirePort.B);
motor FL = motor(PORT16, ratio6_1, true);
motor ML = motor(PORT18, ratio6_1, true);
motor BL = motor(PORT19, ratio6_1, true);
motor FR = motor(PORT10, ratio6_1, false);
motor MR = motor(PORT13, ratio6_1, false);
motor BR = motor(PORT15, ratio6_1, false);
inertial Inertial = inertial(PORT9);
rotation Rotate = rotation(PORT11, true);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}