/* 
 * File:   display.h
 * Author: hammy
 *
 * Created on December 16, 2012, 5:12 PM
 */

#ifndef __HGE_DISPLAY_H__
#define	__HGE_DISPLAY_H__

#include "global.h"

///////////////////////////////////////////////////////////////////////////////
//		Window/Display Object
///////////////////////////////////////////////////////////////////////////////
namespace n_display {
	
	bool	initScreen			();
	void	setWindowTitle		( cstr );
	cstr	getWindowTitle		();
	void	setScreenSize		( int x, int y );
	int		getScreenWidth		();
	int		getScreenHeight		();
	
}

#endif	/* __HGE_DISPLAY_H__ */

