/* 
 * File:   display.h
 * Author: hammy
 *
 * Created on December 16, 2012, 5:12 PM
 */

#ifndef __HGE_DISPLAY_H__
#define	__HGE_DISPLAY_H__

#include "types.h"

struct GLFWwindow;

namespace hge {

///////////////////////////////////////////////////////////////////////////////
//		Video Modes
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//		Window/Display Object
///////////////////////////////////////////////////////////////////////////////
namespace display {
    
    typedef GLFWwindow hgeWindow;
    HGE_API hgeWindow* getCurrentWindow();
    
    // Variables
    const int DEFAULT_WINDOW_WIDTH  = 640;
    const int DEFAULT_WINDOW_HEIGHT = 480;
	
    struct videoMode {
        int width;
        int height;
        int bpp;
    };

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
	HGE_API int		getWindowWidth	();
	HGE_API int		getWindowHeight ();
    HGE_API int     getDesktopWidth ();
    HGE_API int     getDesktopHeight();
    
    HGE_API void    raiseWindow     ();
    HGE_API void    lowerWindow     ();
    
    HGE_API void    setWindowTitle  ( const char* );
}

} // end hge namespace

#endif	/* __HGE_DISPLAY_H__ */
