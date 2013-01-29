/* 
 * File:   display.h
 * Author: hammy
 *
 * Created on December 16, 2012, 5:12 PM
 */

#ifndef __HGE_DISPLAY_H__
#define	__HGE_DISPLAY_H__

#include "types.h"

///////////////////////////////////////////////////////////////////////////////
//		Window/Display Object
///////////////////////////////////////////////////////////////////////////////
namespace n_display {
	
	HGE_API bool	initScreen			();
	HGE_API void	setWindowTitle		( cstr );
	HGE_API cstr	getWindowTitle		();
	HGE_API void	setScreenSize		( int x, int y );
	HGE_API int		getScreenWidth		();
	HGE_API int		getScreenHeight		();
	
}

#endif	/* __HGE_DISPLAY_H__ */

