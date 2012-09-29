/* 
 * File:   clock.cpp
 * Author: hammy
 * 
 * Created on September 7, 2012, 7:22 PM
 */

#include <ctime>
#include "../../include/harbinger.h"

namespace {
	const static double SECS_PER_CLOCK(1.0/CLOCKS_PER_SEC); //hurr hurr
}

namespace harbinger {

//-----------------------------------------------------------------------------
//		c_clock Construction & Destruction
//-----------------------------------------------------------------------------
c_clock::c_clock() :
	currentTime	( ((double)clock()) * SECS_PER_CLOCK ),
	previousTime	( 0 )
{}

c_clock::c_clock(const c_clock& c_clockCopy) :
	currentTime	( clock() ),
	previousTime	( c_clockCopy.previousTime )
{}

c_clock::~c_clock() {}

//-----------------------------------------------------------------------------
//		c_clock Construction & Destruction
//-----------------------------------------------------------------------------
double c_clock::timeSinceLastUpdate() const {
	return currentTime - previousTime;
}

double c_clock::getTime() const {
	return ((double)clock()) * SECS_PER_CLOCK;
}

void c_clock::update() {
	previousTime = currentTime;
	currentTime = ((double)clock()) * SECS_PER_CLOCK;
}

} //end harbinger namespace
