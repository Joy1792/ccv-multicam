//! PS3.h
/*!
*  
*
*  Created by Yishi Guo on 06/13/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/

#ifndef _PS3_H_
#define _PS3_H_

#include "CLEyeMulticam.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cv.h>

using namespace std;

class PS3 {
public:
	//! Constructor
	PS3();

	//! Destructor
	~PS3();

	//! Get the camera count
	/*!
		\return the camera count
	*/
	static int GetCameraCount();

	//! Convert GUID to string
	/*!
		\param guid a GUID structure param
		\return string of guid
	*/
	static std::string GUID2String( GUID guid, char delimiter = '-', bool uppercase = true );

	//! Convert string to GUID
	static GUID String2GUID( string str, char delimiter = '-', bool uppercase = true );

	static std::string Int2String( int val );

	//! Get camera GUID by camera Id
	static GUID GetGUID( int camId );

	//! Get GUID of the camera
	GUID GetGUID() const;

	//! Get GUID of the camera [return string]
	std::string GetGUIDStr() const;

	//! Set the GUID of camera
	void SetGUID( GUID guid );

	//! Is GUID equal?
	static bool EqualGUID( GUID guid1, GUID guid2 );

	////!
	//static DWORD WINAPI CaptureThread( LPVOID instance );

	//! Main method to running camera
	void Run();

	//! Set some values about camera
	void SetCamera( GUID camGUID, CLEyeCameraColorMode colorMode,
		CLEyeCameraResolution camRes, float frameRate);

	//! Start the camera
	bool StartCamera();

	//! Stop the camera
	void StopCamera();

	//! Start the capture
	bool StartCapture();

	//! Stop the capture
	bool StopCapture();

	//! Set the window title
	void SetWindowTitle( string title );

	//! Set Horizontal flip
	bool SetHFlip( bool flip = true );

	//! Set Vertical flip
	bool SetVFlip( bool flip = true );

	//! Set the debug mode value
	void SetDebugMode( bool debug );

	//! Set auto gain
	bool SetAutoGain( bool auto = true );

	//! Set gain value
	bool SetGainValue( int value );

	//! Set the auto exposure
	bool SetAutoExposure( bool auto = true );

	//! Set the exposure value
	bool SetExposure( int value );

	//! Set the auto white balance mode
	bool SetAutoWhiteBalance( bool autoBalance = true );

	//! Set the red value of white balance
	bool SetWhiteBalanceRed( int value );

	//! Set the green value of white balance
	bool SetWhiteBalanceGreen( int value );

	//! Set the blue value of white balance
	bool SetWhiteBalanceBlue( int value );

	//! Reset the camera parameters to default value
	bool ResetCamParam();

	//! Get the frame count
	int GetFrameCount() const;

	//! Clear frame count
	void ClearFrameCount();

	//! Get the pixels of frame
	unsigned char* GetPixels();

	//! Get the width of frame
	int GetWidth();

	//! Get the height of frame
	int GetHeight();

	//! Get the FPS
	int GetFPS();

	//! Get Horizontal flip value
	bool GetHFlip();

	//! Get Vertical flip value
	bool GetVFlip();

	//! Get auto gain value
	bool GetAutoGain() const;

	//! Get gain value
	int GetGainValue() const;

	//! Get auto exposure value
	bool GetAutoExposure() const;

	//! Get the exposure value
	int GetExposure() const;

	//! Get the auto white balance value
	bool GetAutoWhiteBalance() const;

	//! Get the red value of white balance
	int GetWhiteBalanceRed() const;

	//! Get the green value of white balance
	int GetWhiteBalanceGreen() const;

	//! Get the blue value of white balance
	int GetWhiteBalanceBlue() const;

	//! Print the camera informations
	void PrintInfo();

	//! Is the camera color mode
	bool IsColorMode();

	//! Is the camera get new frame
	bool IsFrameNew();

	//! Decrease the value of parameter
	bool DecrementParam( int param );

	//! Increase the value of parameter
	bool IncrementParam( int param );

private:
	//! Set the white balance value [0: Red, 1: Green, 2:Blue]
	bool SetWhiteBalance( int color, int value ) ;

	//! Get the value of white balance
	int GetWhiteBalance( int color ) const;

private:
	//! GUID of camera
	GUID _camGUID;

	//! Camera color mode
	CLEyeCameraColorMode _camColorMode;

	//! Camera resolution
	CLEyeCameraResolution _camResolution;

	//! Camera frame rate
	float _frameRate;

	//! PS3 camera instance
	CLEyeCameraInstance _cam;

	//! Is camera running
	bool _bRunning;

	//! Is capture
	bool _bCapture;

	//! Is show the window
	bool _bShow;

	//! The title of window
	string _windowTitle;

	//! Handle of capture thread
	HANDLE _hThread;

	//! Image capture buffer point
	unsigned char* _pCapBuffer;

	//! Is initialized
	bool _bInitialized;

	//! Horizontal flip
	bool _bHFlip;

	//! Vertical flip
	bool _bVFlip;

	//! Window name
	const char* _sWindowName;

	//! Debug Mode
	bool _bDebugMode;

	//! Frame count
	int _frameCount;

	//! Old frame count
	int _oldFrameCount;

	//! The really fps, not the framerate
	int _fps;

	//! Auto gain [true, false]
	bool _bAutoGain;

	//! Gain value [0, 79]
	int _gainValue;

	//! Auto exposure [true, false]
	bool _bAutoExposure;

	//! Exposure value [0, 511]
	int _exposureValue;

	//! Auto white balance [true, false]
	bool _bAutoWhiteBalance;

	//! White balance red value [0, 255]
	int _whiteBalanceRed;

	//! White balance green value [0, 255]
	int _whiteBalanceGreen;

	//! White balance blue value [0, 255]
	int _whiteBalanceBlue;

 };
#endif