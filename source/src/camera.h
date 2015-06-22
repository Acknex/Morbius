#ifndef CAMERA_H
#define CAMERA_H

#define CAMERA_TYPE_FIXED_FOLLOW 0
#define CAMERA_TYPE_SPLINE 1
#define CAMERA_TYPE_AXIS 2
#define CAMERA_TYPE_MULTIPLE_FOLLOW 3

action actSplineCamMin();
action actSplineCamMax();

action actDynamicCamera();

action actSetCameraType();

void cameraInit();
void cameraLoop();

void moveCameraFixedFollow();

void moveCameraSpline();

void moveCameraAxis(int axis);

void cameraPause();
void cameraResume();

// Specified the min / max position of a level (For moveCameraSpline)
VECTOR vecSplineCamMin;
VECTOR vecSplineCamMax;

// Temporary camera variables
VECTOR vecCameraTemp;
var cameraPathLength = 0;
var cameraPathPlayerPos = 0;
var cameraPathPlayerPosOld = 0;
ENTITY* entCameraPathEntity = NULL;

// Active camera type
int activeCameraType = CAMERA_TYPE_FIXED_FOLLOW;

var camera_initialized = 0;
var isCameraInitialized();

#include "camera.c"

#endif