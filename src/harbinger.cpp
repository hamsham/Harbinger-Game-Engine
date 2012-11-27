

#include "../include/harbinger.h"
#define hr_clock std::chrono::high_resolution_clock
#define hr_duration std::chrono::duration< double, std::nano >

namespace harbinger {
namespace system {

//-----------------------------------------------------------------------------
//	Establish private variables
//-----------------------------------------------------------------------------
namespace {
	static std::chrono::time_point< hr_clock, hr_duration > startTime;
	static bool engineInitialized( false );
}

//-----------------------------------------------------------------------------
//	Engine Initialization Check
//-----------------------------------------------------------------------------
bool isInitialized() {
	return engineInitialized;
}

//-----------------------------------------------------------------------------
//	Establish Initializations
//-----------------------------------------------------------------------------
bool initialize() {
	if ( engineInitialized == true )
		return true;
	
	startTime = hr_clock::now();
	engineInitialized = true;
	return true;
}

//-----------------------------------------------------------------------------
//	Establish Shut Down function
//-----------------------------------------------------------------------------
void shutdown() {
	engineInitialized = false;
}

//-----------------------------------------------------------------------------
//	Time Tracking using nanosecond resolution
//-----------------------------------------------------------------------------
double timeSinceStart() {
	if ( engineInitialized == false )
		return 0.0;

	return std::chrono::duration_cast< hr_duration >(
		hr_clock::now() - startTime
	).count();
}
	
} // end system namespace
} // end harbinger namespace

#undef hr_clock
#undef hr_duration
