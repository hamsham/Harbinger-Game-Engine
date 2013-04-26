
#include "timeObject.h"
using namespace std::chrono;

namespace hge {

///////////////////////////////////////////////////////////////////////////////
//		TIME OBJECT BASE CLASS
///////////////////////////////////////////////////////////////////////////////
// program start time
const c_timeObject::hr_time_point c_timeObject::progEpoch( c_timeObject::hr_clock::now() );

//-----------------
// Construction
//-----------------
c_timeObject::c_timeObject() :
	timeFlags( TIMEOBJ_PAUSED ),
	currPoint(),
	currTime( 0.0 ),
	tickTime( 0.0 ),
	stopTime( 0.0 )
{}

c_timeObject::c_timeObject( const c_timeObject& c ) :
	timeFlags( c.timeFlags ),
	currPoint( c.currPoint ),
	currTime( c.currTime ),
	tickTime( c.tickTime ),
	stopTime( c.stopTime )
{}

c_timeObject::~c_timeObject() {}

//-----------------
// Global Times
//-----------------

std::tm* c_timeObject::getGlobalTimeInfo() {
	time_t currTime = system_clock::to_time_t( system_clock::now() );
	return localtime( &currTime );
}

int c_timeObject::globalSecond() {
	return getGlobalTimeInfo()->tm_sec;
}

int c_timeObject::globalMinute() {
	return getGlobalTimeInfo()->tm_min;
}

int c_timeObject::globalHour( bool daylightSavings, bool tfh ) {
	std::tm* timeInfo = getGlobalTimeInfo();
	int hour = timeInfo->tm_hour;
	
	if ( tfh && (hour > 12) )
		hour -= 12;
	
	return hour + (daylightSavings ? 1 : 0 );
}

int c_timeObject::globalDay() {
	return getGlobalTimeInfo()->tm_mday;
}

int c_timeObject::globalWeekday() {
	return getGlobalTimeInfo()->tm_wday;
}

int c_timeObject::globalMonth() {
	return getGlobalTimeInfo()->tm_mon;
}

int c_timeObject::globalYear() {
	return getGlobalTimeInfo()->tm_year + 1900;
}

//-----------------
// Time Inquiry
//-----------------
void c_timeObject::start() {
	if ( timeFlags & TIMEOBJ_PAUSED )
		timeFlags ^= TIMEOBJ_PAUSED;
	
	tickTime = currTime = 0.0;
	currPoint = c_timeObject::hr_clock::now();
}

void c_timeObject::pause() {
	timeFlags |= TIMEOBJ_PAUSED;
	tickTime = 0.0;
	currPoint = c_timeObject::hr_clock::now();
}

void c_timeObject::stop() {
	timeFlags |= TIMEOBJ_PAUSED;
	tickTime = currTime = 0.0;
	//currPoint = c_timeObject::hr_duration( 0.0 );
}

//-----------------
// Get Time
//-----------------
c_timeObject::hr_prec c_timeObject::getCurrTime() const {
	return currTime;
}

void c_timeObject::setCurrTime( c_timeObject::hr_prec c ) {
	currTime = c;
}

c_timeObject::hr_prec c_timeObject::getTimeSinceUpdate() const {
	return duration_cast< hr_duration >(
		hr_clock::now() - currPoint
	).count();
}

c_timeObject::hr_prec c_timeObject::getTickDuration() const {
	return tickTime;
}

void c_timeObject::setStopTime( hr_prec st ) {
	stopTime = st;
}

c_timeObject::hr_prec c_timeObject::getStopTime() const {
	return stopTime;
}

//-----------------
// Time Flags
//-----------------
void c_timeObject::setFlags( unsigned int flags ) {
	timeFlags = flags;
}

unsigned int c_timeObject::getFlags() const {
	return timeFlags;
}

bool c_timeObject::isPaused() const {
	return (timeFlags & TIMEOBJ_PAUSED) != 0;
}

bool c_timeObject::isSetToStop() const {
	return (timeFlags & TIMEOBJ_STOP_AT_TIME_POINT) != 0;
}

//-----------------
// Time Updating
//-----------------
void c_timeObject::update() {
	if ( isPaused() )
		return;
	
	tickTime = duration_cast< hr_duration >(
		hr_clock::now() - currPoint
	).count();
	
	currTime += tickTime;
	currPoint = hr_clock::now();
	
	if ( (timeFlags & TIMEOBJ_STOP_AT_TIME_POINT) && (currTime >= stopTime) )
		timeFlags |= TIMEOBJ_PAUSED;
}

void c_timeObject::tick( c_timeObject::hr_prec timeElapsed ) {
	if ( isPaused() )
		return;
	
	tickTime = timeElapsed;
	currTime += tickTime;
	currPoint += hr_duration( timeElapsed );
	
	if ( (timeFlags & TIMEOBJ_STOP_AT_TIME_POINT) && (currTime >= stopTime) )
		timeFlags |= TIMEOBJ_PAUSED;
}

///////////////////////////////////////////////////////////////////////////////
//		CLOCK CLASS
///////////////////////////////////////////////////////////////////////////////
//-----------------
// Construction
//-----------------
c_clock::c_clock() :
	c_timeObject()
{}

c_clock::c_clock( const c_clock& c ) :
	c_timeObject( c )
{}

///////////////////////////////////////////////////////////////////////////////
//		TIMER CLASS
///////////////////////////////////////////////////////////////////////////////
//-----------------
// Construction
//-----------------
c_timer::c_timer() :
	c_timeObject(),
	startTime( 0.0 )
{
	timeFlags |= TIMEOBJ_STOP_AT_TIME_POINT;
}

c_timer::c_timer( const c_timer& t ) :
	c_timeObject( t ),
	startTime( t.startTime )
{}

//-----------------
// Time Tracking
//-----------------
void c_timer::start() {
	if ( timeFlags & TIMEOBJ_PAUSED )
		timeFlags ^= TIMEOBJ_PAUSED;
	
	tickTime = 0.0;
	currTime = startTime;
	currPoint = c_timeObject::hr_clock::now();
}

void c_timer::setStartTime( c_timeObject::hr_prec st ) {
	startTime = st;
}

c_timeObject::hr_prec c_timer::getStartTime() const {
	return startTime;
}

bool c_timer::finished() const {
	if ( (timeFlags & TIMEOBJ_STOP_AT_TIME_POINT) != 0 ) {
		if ( currTime <= stopTime )
			return true;
	}
	return false;
}

//-----------------
// Time Updating
//-----------------
void c_timer::update() {
	if ( isPaused() )
		return;
		
	if ( stopTime > startTime )
		return;
	
	tickTime = duration_cast< c_timeObject::hr_duration >(
		c_timeObject::hr_clock::now() - currPoint
	).count();
	
	currTime -= tickTime;
	currPoint = c_timeObject::hr_clock::now();
	
	if ( finished() )
		timeFlags |= TIMEOBJ_PAUSED;
}

void c_timer::tick( c_timeObject::hr_prec timeElapsed ) {
	if ( isPaused() )
		return;
		
	if ( stopTime > startTime )
		return;
	
	tickTime = timeElapsed;
	currTime -= tickTime;
	currPoint -= c_timeObject::hr_duration( timeElapsed );
	
	if ( finished() )
		timeFlags |= TIMEOBJ_PAUSED;
}

///////////////////////////////////////////////////////////////////////////////
//		STOPWATCH CLASS
///////////////////////////////////////////////////////////////////////////////
//-----------------
// Construction
//-----------------
c_stopwatch::c_stopwatch() :
	c_timeObject(),
	laps()
{}

c_stopwatch::c_stopwatch( const c_stopwatch& sw ) :
	c_timeObject( sw ),
	laps( sw.laps.begin(), sw.laps.end() )
{}

//-----------------
// Laps
//-----------------
c_timeObject::hr_prec c_stopwatch::getLapTime( unsigned int lapIndex ) const {
	if ( lapIndex > laps.size() )
		return 0.0;
	return laps[ lapIndex ];
}

unsigned int c_stopwatch::getNumLaps() const {
	return laps.size();
}

void c_stopwatch::clearLaps() {
	laps.clear();
}

//-----------------
// Time Manipulation
//-----------------
void c_stopwatch::lap() {
	laps.push_back( currTime );
	currTime = 0.0;
}

} // end hge namespace
