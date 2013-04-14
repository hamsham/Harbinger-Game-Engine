/* 
 * File:   input.h
 * Author: hammy
 *
 * Created on April 13, 2013, 1:29 PM
 */

#ifndef __HGE_INPUT_H__
#define	__HGE_INPUT_H__

#include "types.h"

/*/
 * Keyboard and mouse inputs.
 * These functions have a 1:1 correlation to GLFW.
/*/

namespace hge {
namespace input {

// Button Callbacks
HGE_API void setKeyCallback         ( void (*func)( int keycode, int isPressed ) );
HGE_API void setTextCallback        ( void (*func)( int keycode, int isPressed ) );
HGE_API void setMouseCallback       ( void (*func)( int button, int isPressed ) );
HGE_API void setMousePosCallback    ( void (*func)( int x, int y ) );
HGE_API void setMouseWheelCallback  ( void (*func)( int pos ) );

// Button Pressing
HGE_API bool isKeyPressed           ( int keycode );
HGE_API bool isMouseButtonPressed   ( int button );

// Mouse Positioning
HGE_API void setMousePos            ( int x, int y );
HGE_API void getMousePos            ( int* x, int* y );
HGE_API void setMouseWheelPos       ( int );
HGE_API void getMouseWheelPos       ( int* );

/*/
 * These key codes have been copied and pasted directly from the GLFW Header.
 * I have done this to provide an HGE-only interface. This way, it is no longer
 * required to keep switching back and forth between Harbinger and GLFW.
 * Most of the input calls have a direct 1:1 correlation with GLFW
/*/

// Button Pressing/Releasing
enum : signed int {
    HGE_RELEASED = 0,
    HGE_PRESSED = 1
};

enum : signed int {
    HGE_KEY_UNKNOWN = -1,
    HGE_KEY_SPACE = 32,
    HGE_KEY_SPECIAL = 256,
    HGE_KEY_ESC,
    HGE_KEY_F1,
    HGE_KEY_F2,
    HGE_KEY_F3,
    HGE_KEY_F4,
    HGE_KEY_F5,
    HGE_KEY_F6,
    HGE_KEY_F7,
    HGE_KEY_F8,
    HGE_KEY_F9,
    HGE_KEY_F10,
    HGE_KEY_F11,
    HGE_KEY_F12,
    HGE_KEY_F13,
    HGE_KEY_F14,
    HGE_KEY_F15,
    HGE_KEY_F16,
    HGE_KEY_F17,
    HGE_KEY_F18,
    HGE_KEY_F19,
    HGE_KEY_F20,
    HGE_KEY_F21,
    HGE_KEY_F22,
    HGE_KEY_F23,
    HGE_KEY_F24,
    HGE_KEY_F25,
    HGE_KEY_UP,
    HGE_KEY_DOWN,
    HGE_KEY_LEFT,
    HGE_KEY_RIGHT,
    HGE_KEY_LSHIFT,
    HGE_KEY_RSHIFT,
    HGE_KEY_LCTRL,
    HGE_KEY_RCTRL,
    HGE_KEY_LALT,
    HGE_KEY_RALT,
    HGE_KEY_TAB,
    HGE_KEY_ENTER,
    HGE_KEY_BACKSPACE,
    HGE_KEY_INSERT,
    HGE_KEY_DEL,
    HGE_KEY_PAGEUP,
    HGE_KEY_PAGEDOWN,
    HGE_KEY_HOME,
    HGE_KEY_END,
    HGE_KEY_KP_0,
    HGE_KEY_KP_1,
    HGE_KEY_KP_2,
    HGE_KEY_KP_3,
    HGE_KEY_KP_4,
    HGE_KEY_KP_5,
    HGE_KEY_KP_6,
    HGE_KEY_KP_7,
    HGE_KEY_KP_8,
    HGE_KEY_KP_9,
    HGE_KEY_KP_DIVIDE,
    HGE_KEY_KP_MULTIPLY,
    HGE_KEY_KP_SUBTRACT,
    HGE_KEY_KP_ADD,
    HGE_KEY_KP_DECIMAL,
    HGE_KEY_KP_EQUAL,
    HGE_KEY_KP_ENTER,
    HGE_KEY_KP_NUM_LOCK,
    HGE_KEY_CAPS_LOCK,
    HGE_KEY_SCROLL_LOCK,
    HGE_KEY_PAUSE,
    HGE_KEY_LSUPER,
    HGE_KEY_RSUPER,
    HGE_KEY_MENU,
    HGE_KEY_LAST = HGE_KEY_MENU,
    
    /* Mouse button definitions */
    HGE_MOUSE_BUTTON_1 = 0,
    HGE_MOUSE_BUTTON_2,
    HGE_MOUSE_BUTTON_3,
    HGE_MOUSE_BUTTON_4,
    HGE_MOUSE_BUTTON_5,
    HGE_MOUSE_BUTTON_6,
    HGE_MOUSE_BUTTON_7,
    HGE_MOUSE_BUTTON_8,
    HGE_MOUSE_BUTTON_LAST = HGE_MOUSE_BUTTON_8,

    /* Mouse button aliases */
    HGE_MOUSE_BUTTON_LEFT = HGE_MOUSE_BUTTON_1,
    HGE_MOUSE_BUTTON_RIGHT = HGE_MOUSE_BUTTON_2,
    HGE_MOUSE_BUTTON_MIDDLE = HGE_MOUSE_BUTTON_3
}; /* e_key */
        
} /* End input namespace */
} /* End harbinger namespace */

#endif	/* __HGE_INPUT_H__ */

