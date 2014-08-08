/* 
 * File:   input.h
 * Author: hammy
 *
 * Created on December 7, 2012, 4:01 PM
 */

#ifndef __INPUT_H__
#define	__INPUT_H__

#include "main.h"

/*
 * Game Systems
 */
class gameSystem {
    private:
        //Mouse
        static math::vec2d  mouseVel;
        
        static hge::window*  pWindow;
        static hge::input*   pInputs;
    
    public:
        gameSystem() {}
        virtual ~gameSystem() = 0;
        
        static bool init();
        static void terminate();

        // Updating user input
        static void                 setMousePos     ( const math::vec2d& p ) { pInputs->setMousePos( p ); }
        static void                 setMouseSpeed   ( const math::vec2d& s ) { mouseVel = s; }
        
        static const math::vec2d    getMousePos     () { return pInputs->getMousePos(); }
        static const math::vec2d&   getMouseSpeed   () { return mouseVel; }
        
        static hge::window&         getSystemWindow () { return *pWindow; }
        static hge::input&    getSystemInputs () { return *pInputs; }
};

/*
 * Game Logic
 */
class gameLogic final : public hge::application {
    using key_t     = hge::input::key_t;
    using action_t  = hge::input::action_t;
    using mouse_t   = hge::input::mouse_t;
    using mod_t     = hge::input::mod_t;

    private:
        
        static bool showVNBT[ 4 ];
        
        static void mousePosCallback    ( hge::window::context*, double, double );
        static void keyboardCallback    ( hge::window::context*, key_t, int, action_t, mod_t );
        static void mouseButtonCallback ( hge::window::context*, mouse_t, action_t, mod_t );
        
    public:
        gameLogic() {}
        ~gameLogic() {}
        
        void startRunning();
        void stopRunning();
        void tick();
};

#endif	/* __INPUT_H__ */

