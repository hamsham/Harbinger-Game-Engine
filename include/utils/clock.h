/* 
 * File:   clock.h
 * Author: hammy
 *
 * Created on September 7, 2012, 7:22 PM
 */

#ifndef	__HGE_CLOCK_H__
#define	__HGE_CLOCK_H__
namespace harbinger {

class c_clock {
	private:
		double currentTime;
		double previousTime;
		
	public:
		//construction & destruction
		c_clock();
		c_clock(const c_clock& orig);
		virtual ~c_clock();
		
		//utilities
		double timeSinceLastUpdate() const;
		double getTime() const;			//returns the time, in seconds, since the program started
		void update();
};

} //end harbinger namespace

#endif	/* __HGE_CLOCK_H__ */
