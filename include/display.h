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

struct GLFWmonitor;
struct GLFWwindow;

namespace hge {

///////////////////////////////////////////////////////////////////////////////
//		Window/Display Object
///////////////////////////////////////////////////////////////////////////////
class HGE_API window {
    public:
        /*
         * Public enumerations and typedefs
         */
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
        
        typedef GLFWwindow context;
        typedef GLFWmonitor display;
        typedef std::vector< videoMode > modeList_t;
        typedef std::vector< display* > displayList_t;
        
    private:
        /*
         * Private Members
         */
        bool    displayFullscreen   = false;
        context* pContext           = nullptr; // needed by input systems
        vec2i   resolution          = vec2i( DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT );
        
        /*
         * Private Static Members
         */
        static displayList_t displayList;
        static std::vector< modeList_t > modeList;
        
        /*
         * Private Static Methods
         */
        static void monitorCallback( display*, int );
        
    public:
        /*
         * Public Static Methods
         */
        static  bool    init();
        static  void    terminate();
        static  void    setActiveContext( context* );
        static  vec2i   getDesktopSize();
        static display* getMainMonitor();
        
        static const displayList_t& getMonitorList() { return displayList; }
        static const modeList_t& getDisplayModes( int monitorIndex = 0 ) { return modeList[ monitorIndex ]; }
        
        /*
         * RAII Constructor
         */
        window(
            int width           = DEFAULT_WINDOW_WIDTH,
            int height          = DEFAULT_WINDOW_HEIGHT,
            bool resizeable     = true,
            display* fullscreen = nullptr,
            bool useVsync       = false
        );
        ~window     ();
        window      () = delete;
        window      ( const window& ) = delete;
        window      ( window&& );
        window&     operator =      ( const window& ) = delete;
        window&     operator =      ( window&& );
        
        /*
         * Public Methods
         */
        context*    getContext      () { return pContext; }
        void        raise           ();
        void        minimize        ();
        void        showForeground  ();
        void        setTitle        ( const char* );
        void        flip            ();
        bool        isOpen          ();
        
        const vec2i& getResolution  ();
        void        setResolution   ( const vec2i& );
        vec2i       getPosition     ();
        void        setPosition     ( const vec2i& );
        
        void        notifyClose     ( bool );
        bool        wantsToClose    () const;
        
        void        setCloseCallback    ( void (*)( context* ) );
        void        setPositionCallback ( void (*)( context*, int, int ) );
        void        setResizeCallback   ( void (*)( context*, int, int ) ); // for when the window is obscured
        void        setRefreshCallback  ( void (*)( context* ) );
        void        setFocusCallback    ( void (*)( context*, int f ) ); // f = GL_TRUE if focused
        void        setIconifyCallback  ( void (*)( context*, int i ) ); // i = GL_TRUE if iconified
};

} // end hge namespace

#endif	/* __HGE_DISPLAY_H__ */
