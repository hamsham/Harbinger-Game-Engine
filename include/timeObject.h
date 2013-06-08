
#ifndef __HGE_TIME_OBJECT_H__
#define __HGE_TIME_OBJECT_H__

#include <vector>
#include <chrono>
#include <ctime>
#include "types.h"

namespace hge {

///////////////////////////////////////////////////////////////////////////////
//		TIME OBJECT BASE CLASS
///////////////////////////////////////////////////////////////////////////////
class HGE_API c_timeObject {
    public:
        enum : unsigned {
            TIMEOBJ_STOP_AT_TIME_POINT    = 0x01,
            TIMEOBJ_PAUSED                = 0x02
        };

        typedef double                          hr_prec;
        typedef std::chrono::steady_clock       hr_clock;
        typedef std::ratio< 1, 1 >              hr_ratio; // seconds
        typedef std::chrono::duration < hr_prec, hr_ratio >             hr_duration;
        typedef std::chrono::time_point < hr_clock, hr_duration >       hr_time_point;

    private:
        static const hr_time_point progEpoch;
        static std::tm* getGlobalTimeInfo(); // NOT thread safe at the moment

    protected:
        unsigned timeFlags;
        hr_time_point currPoint;
        hr_prec currTime;
        hr_prec tickTime;
        hr_prec stopTime;

    public:
        c_timeObject();
        c_timeObject(const c_timeObject&);
        virtual ~c_timeObject() = 0;

        template < typename precision_t = hr_prec, typename ratio_t = hr_ratio >
        static hr_prec  progUpTime(); // seconds

        static int      globalSecond();
        static int      globalMinute();
        static int      globalHour(bool daylightSavings = false, bool tfh = true); // 24-hour format enabled by default
        static int      globalDay();
        static int      globalWeekday();
        static int      globalMonth();
        static int      globalYear();

        virtual void    start();
        virtual void    pause();
        virtual void    stop();

        hr_prec         getCurrTime() const;
        void            setCurrTime(hr_prec = 0.0);
        hr_prec         getTimeSinceUpdate() const;
        hr_prec         getTickDuration() const;
        void            setStopTime(hr_prec = 0.0);
        hr_prec         getStopTime() const;

        void            setFlags(unsigned);
        unsigned            getFlags() const;
        bool            isPaused() const;
        bool            isSetToStop() const;

        // possible use for time dilation
        virtual void    update();
        virtual void    tick(hr_prec timeElapsed);
};

// default templat args:
// precision_t: hr_precision
// ratio_t:     hr_ratio

template < typename precision_t, typename ratio_t >
c_timeObject::hr_prec c_timeObject::progUpTime() {
    return
    std::chrono::duration_cast< std::chrono::duration< precision_t, ratio_t > >(
            hr_clock::now() - progEpoch
            ).count();
}

///////////////////////////////////////////////////////////////////////////////
//		CLOCK CLASS
///////////////////////////////////////////////////////////////////////////////

class HGE_API c_clock : virtual public c_timeObject {
    public:
        c_clock();
        c_clock(const c_clock&);

        ~c_clock() {}
};

///////////////////////////////////////////////////////////////////////////////
//		TIMER
///////////////////////////////////////////////////////////////////////////////

class HGE_API c_timer : virtual public c_timeObject {
    private:
        c_timeObject::hr_prec startTime;

    public:
        c_timer();
        c_timer(const c_timer&);

        ~c_timer() {}

        void    start();
        void    setStartTime(hr_prec);
        hr_prec getStartTime() const;
        bool    finished() const;

        void    update();
        void    tick(hr_prec timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////
//		STOPWATCH
///////////////////////////////////////////////////////////////////////////////

class HGE_API c_stopwatch : virtual public c_timeObject {
    private:
        std::vector< c_timeObject::hr_prec >laps;

    public:
        c_stopwatch();
        c_stopwatch(const c_stopwatch&);

        ~c_stopwatch() {}

        hr_prec getLapTime(unsigned lapIndex) const;
        unsigned    getNumLaps() const;
        void    clearLaps();

        void    lap();
};

} // end hge namespace

#endif /* __HGE_TIME_OBJECT_H__ */
