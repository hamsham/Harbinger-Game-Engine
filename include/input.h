/* 
 * File:   input.h
 * Author: hammy
 *
 * Created on December 7, 2012, 4:01 PM
 */

#ifndef __HGE_INPUT_H__
#define	__HGE_INPUT_H__

namespace n_userInput {
	
// Default user updating time interval
	const float DEFAULT_UPDATE_INTERVAL( 1.0 / 60 );
	
// Time between input updates (in seconds)
	void	setInputUpdateInterval( float );
	float	getInputUpdateInterval();
	
// Making the input sensitive to time
	void	setInputTimeSensitivity( bool );
	bool	getInputTimeSensitivity();
	
// Updating user input
	void	setMousePos( int x, int y );
	void	getMousePos( int& x, int& y );
	void	setMouseSpeed( float dx, float dy );
	void	getMouseSpeed( float& dx, float& dy );
}

#endif	/* __HGE_INPUT_H__ */

