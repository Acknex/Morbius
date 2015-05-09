#ifndef DIALOGS_H
#define DIALOGS_H

#define CAMERA_TYPE_FIXED_FOLLOW 0
#define CAMERA_TYPE_SPLINE 1
#define CAMERA_TYPE_AXIS 2

action actSplineCamMin();
action actSplineCamMax();

void cameraInit();
void cameraLoop();

void moveCameraFixedFollow();

void moveCameraSpline();

void moveCameraAxis(int axis);

// Specified the min / max position of a level (For moveCameraSpline)
VECTOR vecSplineCamMin;
VECTOR vecSplineCamMax;

// Temporary camera variables
VECTOR vecCameraTemp;
var cameraPathLength = 0;
var cameraPathPlayerPos = 0;
ENTITY* entCameraPathEntity = NULL;

// Active camera type
int activeCameraType = CAMERA_TYPE_FIXED_FOLLOW;

#endif