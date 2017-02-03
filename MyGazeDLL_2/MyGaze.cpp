// Copyright 2017 by Ryklin Software, Inc.
// All Rights Reserved.

#include "MyGaze.h"


TX_SIZE2 displaySize;
TX_SIZE2 screenBounds;
int MyGazeClass::connected;

int GetScreenWidth(){ if (screenBounds.Width != 0) return screenBounds.Width; else return GetSystemMetrics(SM_CXSCREEN); }
int GetScreenHeight(){ if (screenBounds.Height != 0) return screenBounds.Height; else return GetSystemMetrics(SM_CYSCREEN); }

#pragma comment (lib, "Tobii.EyeX.Client.lib")

data_sample MyGazeClass::sample;

// ID of the global interactor that provides our data stream; must be unique within the application.
static const TX_STRING InteractorId = "Twilight Sparkle";

// global variables
static TX_HANDLE g_hGlobalInteractorSnapshot = TX_EMPTY_HANDLE;
TX_CONTEXTHANDLE MyGazeClass::hContext = TX_EMPTY_HANDLE;

MyGazeClass::MyGazeClass() {

	sample.gaze_lx_relative = 0.5;
	sample.gaze_ly_relative = 0.5;
	sample.gaze_rx_relative = 0.5;
	sample.gaze_ry_relative = 0.5;
	sample.left_pupil_diameter = 3.5;
	sample.right_pupil_diameter = 3.5;
	sample.position_lx_relative = 0.6;
	sample.position_rx_relative = 0.4;
	sample.position_ly_relative = 0.5;
	sample.position_ry_relative = 0.5;
	sample.position_lz_relative = 0.0;
	sample.position_rz_relative = 0.0;

	hContext = TX_EMPTY_HANDLE;
	TX_TICKET hConnectionStateChangedTicket = TX_INVALID_TICKET;
	TX_TICKET hEventHandlerTicket = TX_INVALID_TICKET;
	BOOL success;
	connected = FALSE;

	// initialize and enable the context that is our link to the EyeX Engine.
	success = txInitializeEyeX(TX_EYEXCOMPONENTOVERRIDEFLAG_NONE, NULL, NULL, NULL, NULL) == TX_RESULT_OK;
	success &= txCreateContext(&hContext, TX_FALSE) == TX_RESULT_OK;
	success &= InitializeGlobalInteractorSnapshot(hContext);
	success &= txRegisterConnectionStateChangedHandler(hContext, &hConnectionStateChangedTicket, OnEngineConnectionStateChanged, NULL) == TX_RESULT_OK;
	success &= txRegisterEventHandler(hContext, &hEventHandlerTicket, HandleEvent, NULL) == TX_RESULT_OK;
	success &= txEnableConnection(hContext) == TX_RESULT_OK;

	if (success > 0){
		connected = TRUE;
	}
}

MyGazeClass::~MyGazeClass(){

	BOOL success;
	// disable and delete the context.
	txDisableConnection(hContext);
	txReleaseObject(&g_hGlobalInteractorSnapshot);
	success = txShutdownContext(hContext, TX_CLEANUPTIMEOUT_DEFAULT, TX_FALSE) == TX_RESULT_OK;
	success &= txReleaseContext(&hContext) == TX_RESULT_OK;
	success &= txUninitializeEyeX() == TX_RESULT_OK;
	connected = FALSE;
}

int MyGazeClass::getConnectionStatus()
{
	return connected;
}

/*
* Handles an event from the Gaze Point data stream.
*/
void MyGazeClass::OnGazeDataEvent(TX_HANDLE hGazeDataBehavior)
{
	TX_GAZEPOINTDATAEVENTPARAMS eventParams;
	if (txGetGazePointDataEventParams(hGazeDataBehavior, &eventParams) == TX_RESULT_OK) {

		sample.gaze_rx_relative = eventParams.X/GetScreenWidth();
		sample.gaze_ry_relative = eventParams.Y/GetScreenHeight();
		sample.gaze_lx_relative = eventParams.X/GetScreenWidth();
		sample.gaze_ly_relative = eventParams.Y/GetScreenHeight();
	}
}

/*
* Handles an event from the Gaze Point data stream.
*/
void MyGazeClass::OnEyePositionDataEvent(TX_HANDLE hPositionDataBehavior)
{
	TX_EYEPOSITIONDATAEVENTPARAMS eventParams;
	static TX_EYEPOSITIONDATAEVENTPARAMS eventParamsPrev;

	if (txGetEyePositionDataEventParams(hPositionDataBehavior, &eventParams) == TX_RESULT_OK) {

		sample.left_validity = eventParams.HasLeftEyePosition;
		sample.right_validity = eventParams.HasRightEyePosition;

		if ((sample.left_validity) && (sample.right_validity) ){

			eventParamsPrev = eventParams;

			sample.right_validity = sample.left_validity = 2;  // change from 1 to 2 so that it conforms with hairball's green
		}

		sample.position_lx_relative = eventParamsPrev.LeftEyeXNormalized;
		sample.position_ly_relative = eventParamsPrev.LeftEyeYNormalized;

		sample.position_rx_relative = eventParamsPrev.RightEyeXNormalized;
		sample.position_ry_relative = eventParamsPrev.RightEyeYNormalized;

		sample.position_lz_relative = eventParamsPrev.LeftEyeZNormalized;
		sample.position_rz_relative = eventParamsPrev.RightEyeZNormalized;
	}
}

/*
* Initializes g_hGlobalInteractorSnapshot with an interactor that has the Gaze Point behavior.
*/
BOOL MyGazeClass::InitializeGlobalInteractorSnapshot(TX_CONTEXTHANDLE hContext)
{
	TX_HANDLE hInteractor = TX_EMPTY_HANDLE;
	TX_HANDLE hBehavior = TX_EMPTY_HANDLE;

	TX_GAZEPOINTDATAPARAMS params = { TX_GAZEPOINTDATAMODE_LIGHTLYFILTERED }; //{ TX_GAZEPOINTDATAMODE_UNFILTERED };

	BOOL success;

	success = txCreateGlobalInteractorSnapshot(
		hContext,
		InteractorId,
		&g_hGlobalInteractorSnapshot,
		&hInteractor) == TX_RESULT_OK;

	success &= txCreateGazePointDataBehavior(hInteractor, &params) == TX_RESULT_OK;

	//success &= txCreateInteractorBehavior(hInteractor, &hBehavior, TX_BEHAVIORTYPE_GAZEPOINTDATA) == TX_RESULT_OK;
	//success &= txSetGazePointDataBehaviorParams(hBehavior, &params) == TX_RESULT_OK;

	success &= txCreateInteractorBehavior(hInteractor, &hBehavior, TX_BEHAVIORTYPE_EYEPOSITIONDATA) == TX_RESULT_OK;

	//txReleaseObject(&hBehavior);
	txReleaseObject(&hInteractor);
	return success;
}

/*
* Callback function invoked when a snapshot has been committed.
*/
void TX_CALLCONVENTION MyGazeClass::OnSnapshotCommitted(TX_CONSTHANDLE hAsyncData, TX_USERPARAM param)
{
	// check the result code using an assertion.
	// this will catch validation errors and runtime errors in debug builds. in release builds it won't do anything.

	TX_RESULT result = TX_RESULT_UNKNOWN;
	txGetAsyncDataResultCode(hAsyncData, &result);
	//assert(result == TX_RESULT_OK || result == TX_RESULT_CANCELLED);
}

/*
* Callback function invoked when the status of the connection to the EyeX Engine has changed.
*/

void MyGazeClass::OnStateReceived(TX_HANDLE hStateBag)
{
	TX_BOOL success;
	TX_INTEGER eyeTrackingState;
	TX_SIZE2 displaySize;
	TX_SIZE2 screenBounds;
	TX_SIZE stringSize = 0;
	TX_STRING currentProfileName;
	TX_INTEGER presenceData;
	TX_INTEGER gazeTracking;

	success = (txGetStateValueAsInteger(hStateBag, TX_STATEPATH_EYETRACKINGSTATE, &eyeTrackingState) == TX_RESULT_OK);
	if (success) {
		switch (eyeTrackingState) {
		case TX_EYETRACKINGDEVICESTATUS_TRACKING:
			//printf("Eye Tracking Device Status: 'TRACKING'.\n"
			//	"That means that the eye tracker is up and running and trying to track your eyes.\n");
			connected = TRUE;
			break;

		default:
			//printf("The eye tracking device is not tracking.\n"
			//	"It could be a that the eye tracker is not connected, or that a screen setup or\n"
			//	"user calibration is missing. The status code is %d.\n", eyeTrackingState);
			connected = FALSE;
		}
	}

	success = (txGetStateValueAsSize2(hStateBag, TX_STATEPATH_EYETRACKINGDISPLAYSIZE, &displaySize) == TX_RESULT_OK);
	if (success) {
		//printf("Display Size: %5.2f x %5.2f mm\n", displaySize.Width, displaySize.Height);
	}

	success = (txGetStateValueAsSize2(hStateBag, TX_STATEPATH_EYETRACKINGSCREENBOUNDS, &screenBounds) == TX_RESULT_OK);
	if (success) {
		//printf("Screen Bounds: %5.0f x %5.0f pixels\n\n", screenBounds.Width, screenBounds.Height);
	}

	success = (txGetStateValueAsInteger(hStateBag, TX_STATEPATH_USERPRESENCE, &presenceData) == TX_RESULT_OK);
	if (success) {
		if (presenceData != TX_USERPRESENCE_UNKNOWN) {
			//printf("User is %s\n", presenceData == TX_USERPRESENCE_PRESENT ? "present" : "NOT present");
		}
	}

	// The following state requires EyeX Engine 1.3.0 or later:
	success = (txGetStateValueAsString(hStateBag, TX_STATEPATH_EYETRACKINGCURRENTPROFILENAME, NULL, &stringSize) == TX_RESULT_OK);
	if (success) {
		currentProfileName = (TX_STRING)malloc(stringSize*sizeof(char));
		success = (txGetStateValueAsString(hStateBag, TX_STATEPATH_EYETRACKINGCURRENTPROFILENAME, currentProfileName, &stringSize) == TX_RESULT_OK);
		if (success) {
			//printf("Current user profile name is: %s\n", currentProfileName);
		}
		free(currentProfileName);
	}

	// The following state requires EyeX Engine 1.4.0 or later:
	success = (txGetStateValueAsInteger(hStateBag, TX_STATEPATH_GAZETRACKING, &gazeTracking) == TX_RESULT_OK);
	if (success) {
		//printf("User's eye-gaze is %s\n", gazeTracking == TX_GAZETRACKING_GAZETRACKED ? "tracked" : "NOT tracked");
	}
}

void TX_CALLCONVENTION MyGazeClass::OnEngineStateChanged(TX_CONSTHANDLE hAsyncData, TX_USERPARAM userParam)
{
	TX_RESULT result = TX_RESULT_UNKNOWN;
	TX_HANDLE hStateBag = TX_EMPTY_HANDLE;

	if (txGetAsyncDataResultCode(hAsyncData, &result) == TX_RESULT_OK && txGetAsyncDataContent(hAsyncData, &hStateBag) == TX_RESULT_OK) {
		OnStateReceived(hStateBag);
		txReleaseObject(&hStateBag);
	}
}

void TX_CALLCONVENTION MyGazeClass::OnEngineConnectionStateChanged(TX_CONNECTIONSTATE connectionState, TX_USERPARAM userParam)
{
	TX_HANDLE hStateBag = TX_EMPTY_HANDLE;
	TX_BOOL success;

	switch (connectionState) 
	{
	case TX_CONNECTIONSTATE_CONNECTED:

		txGetStateAsync(hContext, TX_STATEPATH_EYETRACKING, OnEngineStateChanged, NULL);

		//txGetState(hContext, TX_STATEPATH_EYETRACKING, &hStateBag);

		//success = (txGetStateValueAsSize2(hStateBag, TX_STATEPATH_EYETRACKINGDISPLAYSIZE, &displaySize) == TX_RESULT_OK);
		//success = (txGetStateValueAsSize2(hStateBag, TX_STATEPATH_SCREENBOUNDS, &screenBounds) == TX_RESULT_OK);

		OutputDebugString("The connection state is now CONNECTED (We are connected to the EyeX Engine)\n");
		// commit the snapshot with the global interactor as soon as the connection to the engine is established.
		// (it cannot be done earlier because committing means "send to the engine".)
		success = txCommitSnapshotAsync(g_hGlobalInteractorSnapshot, OnSnapshotCommitted, NULL) == TX_RESULT_OK;
		
		if (!success) {
			OutputDebugString("Failed to initialize the data stream.\n");
		}
		else
		{
			OutputDebugString("Waiting for gaze data to start streaming...\n");
		}
		break;	
	case TX_CONNECTIONSTATE_DISCONNECTED:
		OutputDebugString("The connection state is now DISCONNECTED (We are disconnected from the EyeX Engine)\n");
		break;

	case TX_CONNECTIONSTATE_TRYINGTOCONNECT:
		OutputDebugString("The connection state is now TRYINGTOCONNECT (We are trying to connect to the EyeX Engine)\n");
		break;

	case TX_CONNECTIONSTATE_SERVERVERSIONTOOLOW:
		OutputDebugString("The connection state is now SERVER_VERSION_TOO_LOW: this application requires a more recent version of the EyeX Engine to run.\n");
		break;

	case TX_CONNECTIONSTATE_SERVERVERSIONTOOHIGH:
		OutputDebugString("The connection state is now SERVER_VERSION_TOO_HIGH: this application requires an older version of the EyeX Engine to run.\n");
		break;
	}
}
/*
* Callback function invoked when an event has been received from the EyeX Engine.
*/
void TX_CALLCONVENTION MyGazeClass::HandleEvent(TX_CONSTHANDLE hAsyncData, TX_USERPARAM userParam)
{
	TX_HANDLE hEvent = TX_EMPTY_HANDLE;
	TX_HANDLE hBehavior = TX_EMPTY_HANDLE;

	txGetAsyncDataContent(hAsyncData, &hEvent);

	// NOTE. Uncomment the following line of code to view the event object. The same function can be used with any interaction object.
	//OutputDebugStringA(txDebugObject(hEvent));

	if (txGetEventBehavior(hEvent, &hBehavior, TX_BEHAVIORTYPE_GAZEPOINTDATA) == TX_RESULT_OK) {
		OnGazeDataEvent(hBehavior);
		txReleaseObject(&hBehavior);
	}

	if (txGetEventBehavior(hEvent, &hBehavior, TX_BEHAVIORTYPE_EYEPOSITIONDATA) == TX_RESULT_OK) {
		OnEyePositionDataEvent(hBehavior);
		txReleaseObject(&hBehavior);
	}

	txReleaseObject(&hEvent);
}

int MyGazeClass::loadCalibration(char* filePath)
{
    return 1;
}

int MyGazeClass::saveCalibration(char* filePath)
{
	return 0;
}

string MyGazeClass::GetSDKInfo() {
	return "1.6.477";
}

// Queries and prints device information
string MyGazeClass::GetProductInfo()
{
	return "1.6.477";
}