using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor left_intake;
extern motor right_intake;
extern digital_out hood;
extern digital_out structure;
extern digital_out matchloader;
extern motor FL;
extern motor ML;
extern motor BL;
extern motor FR;
extern motor MR;
extern motor BR;
extern inertial Inertial;
extern rotation Rotate;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );