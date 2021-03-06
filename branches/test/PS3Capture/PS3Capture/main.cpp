//! Main cpp file
/*!
// Update: 2011-05-14	by Yishi Guo
//         Add the multi-thread capture and show function
//
//         2011-05-30	by Yishi Guo
//         Add GAIN parameter setting function
// -------------------------------------------------------- */

#include "PS3.h"
#include "CamSplice.h"
#include <iostream>
#include <vector>
#include <stdio.h>

PS3** cams = {NULL};
PS3* pCam = NULL;
int camIndex;
bool hFlip = false;
bool vFlip = false;
bool bShow = false;

//! Camera count
int camCount = -1;

//! x axis
int _x = 0;
//! y axis
int _y = 0;
//! index vector
vector<int> _indexVec;
//! camera vector
vector<PS3*> _camVec;

//! Can splice the cameras
bool bSpliceMode = false;

//!
CamSplice* splice;

//! Parameter for control
int param = -1;


//! Print help information
void PrintHelp( int camCount = 1 ) {
	printf( "Help:\n" );
	//! First print "camera selected" section
	printf( "Camera must be selected\n" );
	for ( int i = 0; i < camCount; ++i ) {
		printf(
		"        %d - Select camera %d\n",
			i, i );
	}

	//! Second print "control" section
	printf(
		"    [ESC] - Exit the program\n"
		"  [SPACE] - Stop display capture image[developing]\n"
		"        h - Horizontal flip\n"
		"        v - Vertical flip\n"
		"        i - Information about camera\n"
		"        s - Camera settings\n"
		"        S - Camera Splice(setting camera first)\n"
		"        g - Set parameter to GAIN\n"
		"        - - Decrease the value of parameter\n"
		"        + - Increase the value of parameter\n"
		);
}

//! Select the camera by key pressed
bool SelectCamera( char key, int maxCamCount ) {
	int index = key - 0x30;	//! Convert the value from char to int, '0' = 0x30
	if ( index < maxCamCount ) {
		pCam = cams[index];	//! Set the current camera point
		camIndex = index;	//! Set the global value
		return true;
	}

	return false;	//! Input incorrect
}

//! Judge the correct index
bool IsCameraIndexCorrect( int index, int camCount, vector<int> vec, int x, int y ) {
	if ( index >= camCount ) {	//! return false, if the index is larger than the max camera count
		return false;
	}
	for ( int i = 0; i < vec.size(); ++i ) {
		if ( vec[i] == index ) {	//! return false, if the index already existed.
			return false;
		}
	}

	return true;
}

//! Camera settings
bool CameraSettings() {
	int x, y;
	int index;
	vector<int> indexVec;
	vector<PS3*> camVec;

	bool flag = true;

	//! Get the x, y values
	while( flag ) {
		printf( "Enter x axis camera number:\n" );
		scanf( "%d", &x );
		printf( "Enter y axis camera number:\n" );
		scanf( "%d", &y );
		if ( x == 0 || y == 0 || x * y > camCount ) {
			printf( "Wrong input, enter again!\n" );
			continue;
		} else {
			flag = false;
		}
	}

	flag = true;

	for ( int j = 0; j < y; ++j ) {
		for ( int i = 0; i < x; ++i ) {
			//! Set the flag to true
			flag = true;
			while ( flag ) {
				printf( "Enter the (%d, %d) camera index:\n", i, j );
				scanf( "%d", &index );

				if ( IsCameraIndexCorrect( index, camCount, indexVec, x, y ) ) {
					indexVec.push_back( index );
					flag = false;
				} else {
					continue;
				}
			}

			camVec.push_back( cams[index] );
		}
	}

	//! Confirm the settings
	char saveStr[16];
	printf( "Save settings?(y/n)\n" );
	scanf( "%s", &saveStr );
	printf( "%s\n", saveStr );
	

	if ( !strcmp( saveStr, "y" ) || !strcmp( saveStr, "Y" ) ) {
		_x = x;
		_y = y;
		_indexVec = indexVec;
		_camVec = camVec;

		printf( "Settings saved!\n" );
		printf( "Press S to see the result\n");
	} else {
		printf( "Settings ignored!\n" );
		return false;
	}

	//! For this moment, we can splice the cameras
	bSpliceMode = true;

	return true;
}

//! Display the spliced image
bool CameraSplice() {
	if ( !bSpliceMode ) {
		printf( "Can not splice the cameras, please set them first!\n" );
		return false;
	} else if ( _x * _y != _camVec.size() ) {
		printf( "x, y axis cameras number product is not equal to camVec.size()\n" );
		return false;
	} else if ( _x == 0 || _y == 0 || _camVec.empty() ) {
		printf( "x/y/camVec's size can not be zero\n" );
		return false;
	}

	//! Close the current windows
	for ( int i = 0; i < camCount; ++i ) {
		cams[i]->ShowWindow( false );
		//cams[i]->PrintInfo();
	}

	splice = new CamSplice();
	splice->SetCamVec( _camVec );
	splice->SetIndexVec( _indexVec );
	splice->SetWidthHeight( _camVec[0]->GetWidth(), _camVec[1]->GetHeight() );
	splice->SetWindowTitle( "Splice Windows" );
	splice->SetXY( _x, _y );

	splice->Start();

	//

	return true;
}

int main( int argc, char** ) {
	printf(
		"------------------------------------\n"
		"PS3 Multi-cam capture test project\n"
		"Creator: Yishi Guo\n"
		"------------------------------------\n"
		);

	camIndex = -1;
	camCount = PS3::GetCameraCount();

	if ( camCount > 0 ) {
		printf( "%d PS3 camera%s found!\n", camCount, camCount > 1 ? "s" : "" );
	} else {
		printf( "No PS3 cameras found!\n" );
		return -1;
	}

	//! Allocate the space
	cams = new PS3*[camCount];

	//! Create and set every PS3 cameras
	for ( int i = 0; i < camCount; i++ ) {
		GUID guid = PS3::GetGUID( i );
		std::cout << "GUID #" << i << ": "
			<< PS3::GUID2String( guid, '-', true ) << std::endl;

		cams[i] = new PS3();
		//! Set the values for camera
		cams[i]->SetCamera(
			guid,	//!< Camera with this guid you want to create
			// CLEYE_MONO_PROCESSED, //!< Gray mode
			CLEYE_MONO_PROCESSED,	//!< Color mode
			CLEYE_VGA,	//!< Resolution of camera VGA(640x480) or QVGA(320x240)
			30	//!< Framerate
			);
		//! Show window to display the capture images
		cams[i]->SetShow( true );
		cams[i]->SetWindowTitle( " ID: " + PS3::Int2String(i)
			+ " [GUID: " 
			+ PS3::GUID2String(guid, '-', true )
			+ "]");

		//cams[i]->SetDebugMode( true );
		//cams[i]->ClearFrameCount();

		//printf( "#%d frame count: %d\n", i, cams[i]->GetFrameCount() );

		if ( cams[i]->StartCamera() ) {
			printf( "#%d: started!\n", i );
		} else {
			printf( "#%d: could not started!\n", i );
		}
	}

	//! print help info
	PrintHelp( camCount );

	int key = 0;

	//! Debug
	//while ( true ) {
	//	Sleep( 1000 );
	//	printf( "%d\n", ++key );
	//	for( int i = 0; i < camCount; ++i ) {
	//		printf( "#%d frame count: %d\n", i, cams[i]->GetFrameCount() );
	//	}
	//}

	while ( ( key = cvWaitKey(0) ) != 0x1b /* ESC key: 27 or 0x1b in hex*/ )	{

		//! Print DEBUG information
		printf( "// DEBUG:\n// \tKey: %d [Hex: %x]\n", key, key );
		for( int i = 0; i < camCount; ++i ) {
			printf( "// \t#%d frame count: %d\n", i, cams[i]->GetFrameCount() );
		}

		//! Select the camera to control
		if ( key >= '0' && key <= '9' ) {
			if ( SelectCamera( key, camCount ) ) {
				printf( "Select Camera #%d\n", camIndex );
			} else {
				printf( "Wrong selecting!\n" );
			}
			continue;
		}

		if ( pCam != NULL ) {	//! camera must be selected
			switch ( key ) {
				//! Horizontal flip
				case 'h':	case 'H':
					hFlip = hFlip ? false : true;
					if ( pCam->SetHFlip( hFlip ) ) {
						printf( "Camera #%d Horizontal flip set: %s\n", camIndex, hFlip ? "true" : "false" );
					} else {
						printf( "Camera #%d could not set H Flip\n" );
					}
					break;
				//! Vertical flip
				case 'v':	case 'V':
					vFlip = vFlip ? false : true;
					if ( pCam->SetVFlip( vFlip ) ) {
						printf( "Camera #%d Vertical flip set: %s\n", camIndex, vFlip ? "true" : "flase" );
					} else {
						printf( "Camera #%d could not set V Flip\n" );
					}
					break;
				//! Show or hide the window
				case ' ':	//! Should be fixed, developing in this moment
					bShow = bShow ? false : true;
					if ( pCam->ShowWindow( bShow ) ) {
						printf( "%s camera #%d capture image\n", bShow ? "Show" : "Hide", camIndex );
					} else {
						printf( "Could not %s camera #%d capture image\n", bShow ? "show" : "hide", camIndex );
					}
					break;

				//! Print camera information
				case 'i':	case 'I':
					printf(
						"Camera #%d Information:\n"
						"\tResolution(width x height): %dx%d\n"
						"\tFPS: %d\n"
						,
						camIndex,
						pCam->GetWidth(),
						pCam->GetHeight(),
						pCam->GetFPS()
						);
					break;

				//! Camera settings
				case 's':
					CameraSettings();
					break;
				case 'S':
					CameraSplice();
					break;
				//! Used when the image is too dark
				case 'g': case 'G':
					printf( "Param gain.\n" );
					param = CLEYE_GAIN;
					break;
				//! Decrease the current parameter
				case '-':
					pCam->DecrementParam( param );
					break;
				//! Increase the current parameter
				case '+':
					pCam->IncrementParam( param );
					break;
				default:
					break;
			}
		}
	}

	if ( bSpliceMode ) {
		splice->Stop();
		splice = NULL;
	}

	//! Release the resources
	for ( int i = 0; i < camCount; ++i ) {
		printf( "Stopping capture on camera %d\n", i );
		//! Stop the camera
		cams[i]->StopCamera();
		delete cams[i];
	}



	return 0;
}