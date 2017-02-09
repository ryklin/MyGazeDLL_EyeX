// Copyright 2017 by Ryklin Software, Inc.
// All Rights Reserved.

#include "stdafx.h"
#include "MyGaze.h" // include your custom class

MyGazeClass *myGazeDLL = 0;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        break;
	case DLL_THREAD_ATTACH:
        break;
	case DLL_THREAD_DETACH:
        break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) int addCalibrationPoint(F_POINT point)
{
    if (myGazeDLL) {
		return myGazeDLL->addCalibrationPoint(point);
    }
    return -1;
}

extern "C" __declspec(dllexport) int startCalibration()
{
    if (myGazeDLL) {
		return myGazeDLL->startCalibration();
    }
    return -1;
}

extern "C" __declspec(dllexport) int finishCalibration()
{
    if (myGazeDLL) {
		return myGazeDLL->finishCalibration();
    }
    return -1;
}

extern "C" __declspec(dllexport) int saveCalibration(char* filePath)
{
    if (myGazeDLL) {
        return myGazeDLL->saveCalibration(filePath);
    }
    return -1;
}

extern "C" __declspec(dllexport) int loadCalibration(char* filePath)
{
    if (myGazeDLL) {
        return myGazeDLL->loadCalibration(filePath);
    }
    return -1;
}

extern "C" __declspec(dllexport) int getSample_left_eye_validity()
{
    if (myGazeDLL) {
        if (myGazeDLL->getSample())
            return myGazeDLL->getSample()->left_validity;
    }
    return 0;
}

extern "C" __declspec(dllexport) int getSample_right_eye_validity()
{
    if (myGazeDLL) {
        if (myGazeDLL->getSample())
            return myGazeDLL->getSample()->right_validity;
    }
    return 0;
}

extern "C" __declspec(dllexport) float getSample_left_pupil_diameter()
{
    if (myGazeDLL) {
        if (myGazeDLL->getSample())
            return myGazeDLL->getSample()->left_pupil_diameter;
    }
    return 1;
}

extern "C" __declspec(dllexport) float getSample_right_pupil_diameter()
{
    if (myGazeDLL) {
        if (myGazeDLL->getSample())
            return myGazeDLL->getSample()->right_pupil_diameter;
    }
    return 1;
}

extern "C" __declspec(dllexport) float getSample_left_gaze_point_2d_x()
{
    if (myGazeDLL) {
        if (myGazeDLL->getSample())
			return myGazeDLL->getSample()->gaze_lx_relative;
	}
    return 0.5;
}

extern "C" __declspec(dllexport) float getSample_left_gaze_point_2d_y()
{
    if (myGazeDLL) {
        if (myGazeDLL->getSample())
			return myGazeDLL->getSample()->gaze_ly_relative;
    }
    return 0.5;
}

extern "C" __declspec(dllexport) float getSample_right_gaze_point_2d_x()
{
    if (myGazeDLL) {
        if (myGazeDLL->getSample())
			return myGazeDLL->getSample()->gaze_rx_relative;
    }
    return 0.5;
}

extern "C" __declspec(dllexport) float getSample_right_gaze_point_2d_y()
{
    if (myGazeDLL) {
        if (myGazeDLL->getSample())
			return myGazeDLL->getSample()->gaze_ry_relative;
    }
    return 0.5;
}

extern "C" __declspec(dllexport) float getSample_left_eye_position_3d_relative_x()
{
    if (myGazeDLL) {
        if (myGazeDLL->getSample())
			return myGazeDLL->getSample()->position_lx_relative;
    }
    return 0.5;
}

extern "C" __declspec(dllexport) float getSample_left_eye_position_3d_relative_y()
{
    if (myGazeDLL) {
        if (myGazeDLL->getSample())
			return myGazeDLL->getSample()->position_ly_relative;
    }
    return 0.5;
}

extern "C" __declspec(dllexport) float getSample_left_eye_position_3d_relative_z()
{
    if (myGazeDLL) {
        if (myGazeDLL->getSample())
			return myGazeDLL->getSample()->position_lz_relative;
    }
    return 0.5;
}

extern "C" __declspec(dllexport) float getSample_right_eye_position_3d_relative_x()
{
    if (myGazeDLL) {
        if (myGazeDLL->getSample())
			return myGazeDLL->getSample()->position_rx_relative;
    }
    return 0.5;
}

extern "C" __declspec(dllexport) float getSample_right_eye_position_3d_relative_y()
{
    if (myGazeDLL) {
        if (myGazeDLL->getSample())
			return myGazeDLL->getSample()->position_ry_relative;

    }
    return 0.5;
}

extern "C" __declspec(dllexport) float getSample_right_eye_position_3d_relative_z()
{
    if (myGazeDLL) {
        if (myGazeDLL->getSample())
			return myGazeDLL->getSample()->position_rz_relative;
    }
    return 0.5;
}

extern "C" __declspec(dllexport) int getInfo()
{
    if (myGazeDLL) {
		return 1;
    }       
      
    return 0;
}

extern "C" __declspec(dllexport) string getInfo_Product()
{
    if (myGazeDLL) {
		return myGazeDLL->GetProductInfo();
    }       
    return "?";
}

extern "C" __declspec(dllexport) string getInfo_GivenName()
{
    if (myGazeDLL) {
    }       
    return "?";
}

extern "C" __declspec(dllexport) string getInfo_Model()
{
    if (myGazeDLL) {
    }       
    return "?";
}

extern "C" __declspec(dllexport) string getInfo_Version()
{
    if (myGazeDLL) {
    }       
    return "?";
}

extern "C" __declspec(dllexport) string getInfo_Generation()
{
    if (myGazeDLL) {
    }       
    return "?";
}

extern "C" __declspec(dllexport) string getInfo_Status()
{
    if (myGazeDLL) {
    }       
    return "?";
}

extern "C" __declspec(dllexport) int getConnection_Status()
{
	if (myGazeDLL) {
		return myGazeDLL->getConnectionStatus();
	}       
    return 0;
}

extern "C" __declspec(dllexport) string getInfo_SDK()
{
    if (myGazeDLL) {
		return myGazeDLL->GetSDKInfo();
    }       
    return "?";
}

extern "C" __declspec(dllexport) char* getVersion_DLL()
{
    return "1.6.477";
}

extern "C" __declspec(dllexport) bool allow_saving_data()
{
    return false;
}

extern "C" __declspec(dllexport) int destroy()
{
	if (myGazeDLL) {
	    delete(myGazeDLL);
		myGazeDLL = 0;
		return 1;
	}
	return 0;
}

extern "C" __declspec(dllexport) int instantiate(HWND hWnd)
{
	destroy();

	myGazeDLL = new MyGazeClass();

	if (myGazeDLL->isConnected()) {
		return 1;
	}
	else
	{
		destroy();
	    return 0;
	}
}

