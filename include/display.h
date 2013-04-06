/* 
 * File:   display.h
 * Author: hammy
 *
 * Created on December 16, 2012, 5:12 PM
 */

#ifndef __HGE_DISPLAY_H__
#define	__HGE_DISPLAY_H__

#include "types.h"

namespace hge {

///////////////////////////////////////////////////////////////////////////////
//		Window/Display Object
//      It is recommended to manage the display through GLFW directly
//      These are just helper functions
///////////////////////////////////////////////////////////////////////////////
namespace n_display {
    // Variables
    const int DEFAULT_WINDOW_WIDTH  = 640;
    const int DEFAULT_WINDOW_HEIGHT = 480;
	
    //Functions
	HGE_API bool	init            (   int windowWidth = DEFAULT_WINDOW_WIDTH,
                                        int windowHeight = DEFAULT_WINDOW_HEIGHT
                                    );
    HGE_API void    terminate       ();
    
    HGE_API void    resizeWindow    ( int w, int h );
	HGE_API int		getScreenWidth	();
	HGE_API int		getScreenHeight ();
	
}

} // end hge namespace

#endif	/* __HGE_DISPLAY_H__ */
