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
//		Video Modes
///////////////////////////////////////////////////////////////////////////////
struct s_videoMode {
    int width;
    int height;
    int bpp;
};

///////////////////////////////////////////////////////////////////////////////
//		Window/Display Object
///////////////////////////////////////////////////////////////////////////////
namespace display {
    
    // Variables
    const int DEFAULT_WINDOW_WIDTH  = 640;
    const int DEFAULT_WINDOW_HEIGHT = 480;
	
    //Functions
	HGE_API bool	init            ();
    HGE_API void    terminate       ();
    HGE_API void    flip            ();
    
    HGE_API bool    createWindow    (   
                                        int width       = DEFAULT_WINDOW_WIDTH,
                                        int height      = DEFAULT_WINDOW_HEIGHT,
                                        bool resizeable = true,
                                        bool fullscreen = false,
                                        bool useVsync   = false
                                    );
    HGE_API void    closeWindow     ();
    HGE_API bool    isWindowOpen    ();
    
    HGE_API void    resizeWindow    ( int w, int h );
	HGE_API int		getScreenWidth	();
	HGE_API int		getScreenHeight ();
    HGE_API void    setResizeCallback( void (*callback)(int w, int h) );
    
    HGE_API void    raiseWindow     ();
    HGE_API void    lowerWindow     ();
    
    HGE_API void    setWindowTitle  ( const char* );
    
    HGE_API int     getNumVideoModes();
    HGE_API const s_videoMode* getVideoMode( int );
}

} // end hge namespace

#endif	/* __HGE_DISPLAY_H__ */
