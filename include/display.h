/* 
 * File:   display.h
 * Author: hammy
 *
 * Created on December 16, 2012, 5:12 PM
 */

#ifndef __HGE_DISPLAY_H__
#define	__HGE_DISPLAY_H__

#include <vector>
#include "types.h"

struct GLFWwindow;

namespace hge {

///////////////////////////////////////////////////////////////////////////////
//		Video Modes
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//		Window/Display Object
///////////////////////////////////////////////////////////////////////////////
class HGE_API window {
    public:
        typedef GLFWwindow context;

        // Variables
        enum : int {
            DEFAULT_WINDOW_WIDTH  = 640,
            DEFAULT_WINDOW_HEIGHT = 480
        };
        
        // Another GLFW structure
        struct videoMode {
            int width       = 0;
            int height      = 0;
            int redBits     = 0;
            int blueBits    = 0;
            int greenBits   = 0;
        };
        
    private:
        bool    displayFullscreen   = false;
        context* pContext           = nullptr; // needed by input systems
        vec2i   resolution          = vec2i( DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT );
        
        static bool  displayInitialized;
        static vec2i deskResolution;
        
    public:
        // RAII
        window(                     int width       = DEFAULT_WINDOW_WIDTH,
                                    int height      = DEFAULT_WINDOW_HEIGHT,
                                    bool resizeable = true,
                                    bool fullscreen = false,
                                    bool useVsync   = false
                                );
        ~window                 ();
        
        window  () = delete;
        window  ( const window& ) = delete;
        window  ( window&& );
        
        window& operator =      ( const window& ) = delete;
        window& operator =      ( window&& );
        
        context* getContext     () { return pContext; }
        
        //Functions
        static  bool            init();
        static  void            terminate();
        static  const vec2i&    getDesktopSize();
        
        void    flip            ();
        bool    isOpen          ();

        void    resize          ( const vec2i& );
        
        const vec2i& getResolution();

        void    raise           ();
        void    minimize        ();

        void    setTitle        ( const char* );
};

} // end hge namespace

#endif	/* __HGE_DISPLAY_H__ */
