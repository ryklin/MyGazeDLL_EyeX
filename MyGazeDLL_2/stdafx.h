#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
// TODO: reference additional headers your program requires here
#include <string>
#include <iostream>
#include "eyex\EyeX.h"

using namespace std;

struct F_POINT
{
	float x;
	float y;
	float z;
};

struct data_sample
{
	float gaze_lx_relative;
    float gaze_ly_relative;
    float gaze_rx_relative;
    float gaze_ry_relative;
    float position_lx_relative;
    float position_ly_relative;
    float position_lz_relative;
    float position_rx_relative;
    float position_ry_relative;
    float position_rz_relative;
	int left_validity;
	int right_validity;
	float left_pupil_diameter;
	float right_pupil_diameter;
};
