/* 
 * File:   input.h
 * Author: hammy
 *
 * Created on April 13, 2013, 1:29 PM
 */

#ifndef __HGE_INPUT_H__
#define	__HGE_INPUT_H__

#include "types.h"
#include "display.h"

/*/
 * Keyboard and mouse inputs.
 * These functions have a 1:1 correlation to GLFW.
/*/

namespace hge {
namespace input {

enum action_t   : int;
enum key_t      : int;
enum mod_t      : int;
enum mouse_t    : int;
enum joy_t      : int;

// Button Callbacks
HGE_API void setKeyCallback         ( display::hgeWindow*, void (*func)( display::hgeWindow*, key_t, int, action_t,mod_t ) );
HGE_API void setTextCallback        ( display::hgeWindow*, void (*func)( display::hgeWindow*, int utf8_char ) );
HGE_API void setMouseCallback       ( display::hgeWindow*, void (*func)( display::hgeWindow*, mouse_t, action_t, mod_t ) );
HGE_API void setMousePosCallback    ( display::hgeWindow*, void (*func)( display::hgeWindow*, double x, double y ) );
HGE_API void setMouseWheelCallback  ( display::hgeWindow*, void (*func)( display::hgeWindow*, double x, double y ) );

// Button Pressing
HGE_API action_t getKeyButtonState  ( display::hgeWindow*, key_t );
HGE_API action_t getMouseButtonState( display::hgeWindow*, mouse_t );

// Mouse Positioning
HGE_API void setMousePos            ( display::hgeWindow*, const vec2d& );
HGE_API vec2d getMousePos           ( display::hgeWindow* );

/*/
 * These key codes have been copied and pasted directly from the GLFW Header.
 * I have done this to provide an HGE-only interface. This way, it is no longer
 * required to keep switching back and forth between Harbinger and GLFW. Nor
 * is it required to link GLFW when including harbinger in an external project.
 * Most of the input calls have a direct 1:1 correlation with GLFW
/*/

// Button Pressing/Releasing
enum action_t: int {
    HGE_RELEASED            = 0,
    HGE_PRESSED             = 1,
    HGE_REPEATED            = 2
};

enum key_t : int {
    HGE_KEY_UNKNOWN         = -1,

    HGE_KEY_SPACE           = 32,
    HGE_KEY_APOSTROPHE      = 39,   /* ' */
    HGE_KEY_COMMA           = 44,   /* , */
    HGE_KEY_MINUS           = 45,   /* - */
    HGE_KEY_PERIOD          = 46,   /* . */
    HGE_KEY_SLASH           = 47,   /* / */
    HGE_KEY_0               = 48,
    HGE_KEY_1               = 49,
    HGE_KEY_2               = 50,
    HGE_KEY_3               = 51,
    HGE_KEY_4               = 52,
    HGE_KEY_5               = 53,
    HGE_KEY_6               = 54,
    HGE_KEY_7               = 55,
    HGE_KEY_8               = 56,
    HGE_KEY_9               = 57,
    HGE_KEY_SEMICOLON       = 59,   /* ; */
    HGE_KEY_EQUAL           = 61,   /* = */
    HGE_KEY_A               = 65,
    HGE_KEY_B               = 66,
    HGE_KEY_C               = 67,
    HGE_KEY_D               = 68,
    HGE_KEY_E               = 69,
    HGE_KEY_F               = 70,
    HGE_KEY_G               = 71,
    HGE_KEY_H               = 72,
    HGE_KEY_I               = 73,
    HGE_KEY_J               = 74,
    HGE_KEY_K               = 75,
    HGE_KEY_L               = 76,
    HGE_KEY_M               = 77,
    HGE_KEY_N               = 78,
    HGE_KEY_O               = 79,
    HGE_KEY_P               = 80,
    HGE_KEY_Q               = 81,
    HGE_KEY_R               = 82,
    HGE_KEY_S               = 83,
    HGE_KEY_T               = 84,
    HGE_KEY_U               = 85,
    HGE_KEY_V               = 86,
    HGE_KEY_W               = 87,
    HGE_KEY_X               = 88,
    HGE_KEY_Y               = 89,
    HGE_KEY_Z               = 90,
    HGE_KEY_LEFT_BRACKET    = 91,   /* [ */
    HGE_KEY_BACKSLASH       = 92,   /* \ */
    HGE_KEY_RIGHT_BRACKET   = 93,   /* ] */
    HGE_KEY_GRAVE_ACCENT    = 96,   /* ` */
    HGE_KEY_WORLD_1         = 161,  /* non-US #1 */
    HGE_KEY_WORLD_2         = 162,  /* non-US #2 */

    /* Function keys */
    HGE_KEY_ESCAPE          = 256,
    HGE_KEY_ENTER           = 257,
    HGE_KEY_TAB             = 258,
    HGE_KEY_BACKSPACE       = 259,
    HGE_KEY_INSERT          = 260,
    HGE_KEY_DELETE          = 261,
    HGE_KEY_RIGHT           = 262,
    HGE_KEY_LEFT            = 263,
    HGE_KEY_DOWN            = 264,
    HGE_KEY_UP              = 265,
    HGE_KEY_PAGE_UP         = 266,
    HGE_KEY_PAGE_DOWN       = 267,
    HGE_KEY_HOME            = 268,
    HGE_KEY_END             = 269,
    HGE_KEY_CAPS_LOCK       = 280,
    HGE_KEY_SCROLL_LOCK     = 281,
    HGE_KEY_NUM_LOCK        = 282,
    HGE_KEY_PRINT_SCREEN    = 283,
    HGE_KEY_PAUSE           = 284,
    HGE_KEY_F1              = 290,
    HGE_KEY_F2              = 291,
    HGE_KEY_F3              = 292,
    HGE_KEY_F4              = 293,
    HGE_KEY_F5              = 294,
    HGE_KEY_F6              = 295,
    HGE_KEY_F7              = 296,
    HGE_KEY_F8              = 297,
    HGE_KEY_F9              = 298,
    HGE_KEY_F10             = 299,
    HGE_KEY_F11             = 300,
    HGE_KEY_F12             = 301,
    HGE_KEY_F13             = 302,
    HGE_KEY_F14             = 303,
    HGE_KEY_F15             = 304,
    HGE_KEY_F16             = 305,
    HGE_KEY_F17             = 306,
    HGE_KEY_F18             = 307,
    HGE_KEY_F19             = 308,
    HGE_KEY_F20             = 309,
    HGE_KEY_F21             = 310,
    HGE_KEY_F22             = 311,
    HGE_KEY_F23             = 312,
    HGE_KEY_F24             = 313,
    HGE_KEY_F25             = 314,
    HGE_KEY_KP_0            = 320,
    HGE_KEY_KP_1            = 321,
    HGE_KEY_KP_2            = 322,
    HGE_KEY_KP_3            = 323,
    HGE_KEY_KP_4            = 324,
    HGE_KEY_KP_5            = 325,
    HGE_KEY_KP_6            = 326,
    HGE_KEY_KP_7            = 327,
    HGE_KEY_KP_8            = 328,
    HGE_KEY_KP_9            = 329,
    HGE_KEY_KP_DECIMAL      = 330,
    HGE_KEY_KP_DIVIDE       = 331,
    HGE_KEY_KP_MULTIPLY     = 332,
    HGE_KEY_KP_SUBTRACT     = 333,
    HGE_KEY_KP_ADD          = 334,
    HGE_KEY_KP_ENTER        = 335,
    HGE_KEY_KP_EQUAL        = 336,
    HGE_KEY_LEFT_SHIFT      = 340,
    HGE_KEY_LEFT_CONTROL    = 341,
    HGE_KEY_LEFT_ALT        = 342,
    HGE_KEY_LEFT_SUPER      = 343,
    HGE_KEY_RIGHT_SHIFT     = 344,
    HGE_KEY_RIGHT_CONTROL   = 345,
    HGE_KEY_RIGHT_ALT       = 346,
    HGE_KEY_RIGHT_SUPER     = 347,
    HGE_KEY_MENU            = 348,
    HGE_KEY_LAST            = HGE_KEY_MENU
}; /* key_t */

enum mod_t : int {
    HGE_MOD_SHIFT           = 0x0001,
    HGE_MOD_CONTROL         = 0x0002,
    HGE_MOD_ALT             = 0x0004,
    HGE_MOD_SUPER           = 0x0008
}; /* mod_t */

enum mouse_t : int {
    HGE_MB_1                = 0,
    HGE_MB_2                = 1,
    HGE_MB_3                = 2,
    HGE_MB_4                = 3,
    HGE_MB_5                = 4,
    HGE_MB_6                = 5,
    HGE_MB_7                = 6,
    HGE_MB_8                = 7,
    HGE_MB_LAST             = HGE_MB_8,
    HGE_MB_LEFT             = HGE_MB_1,
    HGE_MB_RIGHT            = HGE_MB_2,
    HGE_MB_MIDDLE           = HGE_MB_3
}; /* mouse_t */

enum joy_t : int {
    HGE_JOY_1               = 0,
    HGE_JOY_2               = 1,
    HGE_JOY_3               = 2,
    HGE_JOY_4               = 3,
    HGE_JOY_5               = 4,
    HGE_JOY_6               = 5,
    HGE_JOY_7               = 6,
    HGE_JOY_8               = 7,
    HGE_JOY_9               = 8,
    HGE_JOY_10              = 9,
    HGE_JOY_11              = 10,
    HGE_JOY_12              = 11,
    HGE_JOY_13              = 12,
    HGE_JOY_14              = 13,
    HGE_JOY_15              = 14,
    HGE_JOY_16              = 15,
    HGE_JOY_LAST            = HGE_JOY_16
}; /* joy_t */
        
} /* End input namespace */
} /* End harbinger namespace */

#endif	/* __HGE_INPUT_H__ */

