// Copyright 2017 by Ryklin Software, Inc.
// All Rights Reserved.

#ifndef _MYGAZE_DLL_2_H_
#define _MYGAZE_DLL_2_H_
#include "stdafx.h"

class MyGazeClass
{
public:

	MyGazeClass();
	~MyGazeClass();
	int isConnected(){ return connected; }
    int loadCalibration(char* filePath);
    int saveCalibration(char* filepath);

	string GetSDKInfo();
	string GetProductInfo();
	int getConnectionStatus();

	data_sample* getSample(){return &sample;}
	int	addCalibrationPoint(F_POINT){ return 0; }
	int	finishCalibration(){ return 0; }
	int startCalibration(){ return 0; }

private:
	BOOL InitializeGlobalInteractorSnapshot(TX_CONTEXTHANDLE hContext);
	static void TX_CALLCONVENTION OnSnapshotCommitted(TX_CONSTHANDLE hAsyncData, TX_USERPARAM param);
	static void TX_CALLCONVENTION OnEngineConnectionStateChanged(TX_CONNECTIONSTATE connectionState, TX_USERPARAM userParam);
	static void OnGazeDataEvent(TX_HANDLE hGazeDataBehavior);
	static void OnEyePositionDataEvent(TX_HANDLE hPositionDataBehavior);
	static void TX_CALLCONVENTION HandleEvent(TX_CONSTHANDLE hAsyncData, TX_USERPARAM userParam);

	static void OnStateReceived(TX_HANDLE hStateBag);
	static void TX_CALLCONVENTION OnEngineStateChanged(TX_CONSTHANDLE hAsyncData, TX_USERPARAM userParam);

	static TX_CONTEXTHANDLE hContext;

	static data_sample sample;
	string sdk_info;
	static int connected;

};

#endif // _MYGAZE_DLL_2_H_
