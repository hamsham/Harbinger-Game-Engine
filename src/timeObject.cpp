
#include "timeObject.h"
using namespace std::chrono;

namespace hge {

///////////////////////////////////////////////////////////////////////////////
//		TIME OBJECT BASE CLASS
///////////////////////////////////////////////////////////////////////////////
// program start time
const clock::hr_time_point clock::progEpoch( clock::hr_clock::now() );

//-----------------
// Construction
//-----------------
clock::clock( const clock& c ) :
	timeFlags( c.timeFlags ),
	currPoint( c.currPoint ),
	currTime( c.currTime ),
	tickTime( c.tickTime ),
	stopTime( c.stopTime )
{}

//-----------------
// Global Times
//-----------------
std::tm* clock::getGlobalTimeInfo() {
	time_t currTime = system_clock::to_time_t( system_clock::now() );
	return localtime( &currTime );
}

int clock::globalSecond() {
	return getGlobalTimeInfo()->tm_sec;
}

int clock::globalMinute() {
	return getGlobalTimeInfo()->tm_min;
}

int clock::globalHour( bool daylightSavings, bool tfh ) {
	std::tm* timeInfo = getGlobalTimeInfo();
	int hour = timeInfo->tm_hour;
	
	if ( tfh && (hour > 12) )
		hour -= 12;
	
	return hour + (daylightSavings ? 1 : 0 );
}

int clock::globalDay() {
	return getGlobalTimeInfo()->tm_mday;
}

int clock::globalWeekday() {
	return getGlobalTimeInfo()->tm_wday;
}

int clock::globalMonth() {
	return getGlobalTimeInfo()->tm_mon;
}

int clock::globalYear() {
	return getGlobalTimeInfo()->tm_year + 1900;
}

//-----------------
// Time Inquiry
//-----------------
void clock::start() {
	if ( timeFlags & CLOCK_PAUSED )
		timeFlags ^= CLOCK_PAUSED;
	
	tickTime = currTime = 0.0;
	currPoint = clock::hr_clock::now();
}

void clock::pause() {
	timeFlags |= CLOCK_PAUSED;
	tickTime = 0.0;
	currPoint = clock::hr_clock::now();
}

void clock::stop() {
	timeFlags |= CLOCK_PAUSED;
	tickTime = currTime = 0.0;
	//currPoint = c_timeObject::hr_duration( 0.0 );
}

//-----------------
// Get Time
//-----------------
clock::hr_prec clock::getCurrTime() const {
	return currTime;
}

void clock::setCurrTime( clock::hr_prec c ) {
	currTime = c;
}

clock::hr_prec clock::getTimeSinceUpdate() const {
	return duration_cast< hr_duration >(
		hr_clock::now() - currPoint
	).count();
}

clock::hr_prec clock::getTickDuration() const {
	return tickTime;
}

void clock::setStopTime( hr_prec st ) {
	stopTime = st;
}

clock::hr_prec clock::getStopTime() const {
	return stopTime;
}

//-----------------
// Time Flags
//-----------------
void clock::setFlags( unsigned flags ) {
	timeFlags = flags;
}

unsigned clock::getFlags() const {
	return timeFlags;
}

bool clock::isPaused() const {
	return (timeFlags & CLOCK_PAUSED) != 0;
}

bool clock::isSetToStop() const {
	return (timeFlags & CLOCK_STOP_AT_TIME_POINT) != 0;
}

//-----------------
// Time Updating
//-----------------
void clock::update() {
	if ( isPaused() )
		return;
	
	tickTime = duration_cast< hr_duration >(
		hr_clock::now() - currPoint
	).count();
	
	currTime += tickTime;
	currPoint = hr_clock::now();
	
	if ( (timeFlags & CLOCK_STOP_AT_TIME_POINT) && (currTime >= stopTime) )
		timeFlags |= CLOCK_PAUSED;
}

void clock::tick( clock::hr_prec timeElapsed ) {
	if ( isPaused() )
		return;
	
	tickTime = timeElapsed;
	currTime += tickTime;
	currPoint += hr_duration( timeElapsed );
	
	if ( (timeFlags & CLOCK_STOP_AT_TIME_POINT) && (currTime >= stopTime) )
		timeFlags |= CLOCK_PAUSED;
}

///////////////////////////////////////////////////////////////////////////////
//		TIMER CLASS
///////////////////////////////////////////////////////////////////////////////
//-----------------
// Construction
//-----------------
timer::timer() :
	clock(),
	startTime( 0.0 )
{
	timeFlags |= CLOCK_STOP_AT_TIME_POINT;
}

timer::timer( const timer& t ) :
	clock( t ),
	startTime( t.startTime )
{}

//-----------------
// Time Tracking
//-----------------
void timer::start() {
	if ( timeFlags & CLOCK_PAUSED )
		timeFlags ^= CLOCK_PAUSED;
	
	tickTime = 0.0;
	currTime = startTime;
	currPoint = clock::hr_clock::now();
}

void timer::setStartTime( clock::hr_prec st ) {
	startTime = st;
}

clock::hr_prec timer::getStartTime() const {
	return startTime;
}

bool timer::finished() const {
	if ( (timeFlags & CLOCK_STOP_AT_TIME_POINT) != 0 ) {
		if ( currTime <= stopTime )
			return true;
	}
	return false;
}

//-----------------
// Time Updating
//-----------------
void timer::update() {
	if ( isPaused() )
		return;
		
	if ( stopTime > startTime )
		return;
	
	tickTime = duration_cast< clock::hr_duration >(
		clock::hr_clock::now() - currPoint
	).count();
	
	currTime -= tickTime;
	currPoint = clock::hr_clock::now();
	
	if ( finished() )
		timeFlags |= CLOCK_PAUSED;
}

void timer::tick( clock::hr_prec timeElapsed ) {
	if ( isPaused() )
		return;
		
	if ( stopTime > startTime )
		return;
	
	tickTime = timeElapsed;
	currTime -= tickTime;
	currPoint -= clock::hr_duration( timeElapsed );
	
	if ( finished() )
		timeFlags |= CLOCK_PAUSED;
}

///////////////////////////////////////////////////////////////////////////////
//		STOPWATCH CLASS
///////////////////////////////////////////////////////////////////////////////
//-----------------
// Construction
//-----------------
stopwatch::stopwatch( const stopwatch& sw ) :
	clock( sw ),
	laps( sw.laps.begin(), sw.laps.end() )
{}

//-----------------
// Laps
//-----------------
clock::hr_prec stopwatch::getLapTime( unsigned lapIndex ) const {
	if ( lapIndex > laps.size() )
		return 0.0;
	return laps[ lapIndex ];
}

unsigned stopwatch::getNumLaps() const {
	return laps.size();
}

void stopwatch::clearLaps() {
	laps.clear();
}

//-----------------
// Time Manipulation
//-----------------
void stopwatch::lap() {
	laps.push_back( currTime );
	currTime = 0.0;
}

} // end hge namespace
