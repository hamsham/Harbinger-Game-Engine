
//input.cpp

#include "hamLibs.h"

#include "display.h"
#include "input.h"
#include "camera.h"
#include "mesh.h"

using namespace hamLibs;

extern c_camera testCam; // declared in main.cpp
extern c_mesh testMesh; // declared in main.cpp

namespace {
	/*
	 * Private Variables
	 */
	//Timers
	bool	inputTimeSensitivity = true;
	float	inputUpdateInterval = n_userInput::DEFAULT_UPDATE_INTERVAL;
	//Mouse
	int mouseX = 0;
	int mouseY = 0;
	float mouseDX = 1.f;
	float mouseDY = 1.f;
	
} // end anonymous namespace

//-----------------------------------------------------------------------------
//	Time between input updates (in seconds)
//-----------------------------------------------------------------------------
void n_userInput::setInputUpdateInterval( float dt ) {
	inputUpdateInterval = dt;
}

float n_userInput::getInputUpdateInterval() {
	return inputUpdateInterval;
}

//-----------------------------------------------------------------------------
//	Making the input sensitive to time
//-----------------------------------------------------------------------------
void n_userInput::setInputTimeSensitivity( bool b ) {
	inputTimeSensitivity = b;
}

bool n_userInput::getInputTimeSensitivity() {
	return inputTimeSensitivity;
}

//-----------------------------------------------------------------------------
//	Mouse Input
//-----------------------------------------------------------------------------
void n_userInput::setMousePos( int x, int y ) {
	//mouse x-position
	if ( x < 0 )
		mouseX = 0;
	else if ( x > n_display::getScreenWidth() )
		mouseX = n_display::getScreenWidth();
	else
		mouseX = x;
	
	//mouse y-position
	if ( y < 0 )
		mouseY = 0;
	else if ( y > n_display::getScreenHeight() )
		mouseY = n_display::getScreenHeight();
	else
		mouseY = y;
	
	glfwSetMousePos( mouseX, mouseY );
}

void n_userInput::getMousePos( int& x, int& y ) {
	glfwGetMousePos( &x, &y );
}

void n_userInput::setMouseSpeed( float dx, float dy ) {
	mouseDX = dx;
	mouseDY = dy;
}

void n_userInput::getMouseSpeed( float& dx, float& dy ) {
	dx = mouseDX;
	dy = mouseDY;
}
