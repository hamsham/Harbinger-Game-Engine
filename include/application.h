/* 
 * File:   application.h
 * Author: hammy
 *
 * Created on August 10, 2013, 7:07 PM
 */

#ifndef __HGE_APPLICATION_H__
#define	__HGE_APPLICATION_H__

#include "types.h"
#include "utils/timeObject.h"

namespace hge {

/******************************************************************************
//	Application Base Class
******************************************************************************/
class HGE_API application {
    protected:
        bool appIsRunning = false;
        hamLibs::utils::clock appTimer;
        
    public:
        application     () = default;
        application     ( const application& ) = delete;
        application     ( application&& ) = default;
        
        virtual ~application() = 0;
        
        application&    operator =      ( const application& ) = delete;
        application&    operator =      ( application&& ) = default;
        
        virtual void    tick            () { appTimer.update(); }
        
        virtual void    startRunning    () { appIsRunning = true; appTimer.start(); }
        virtual void    stopRunning     () { appIsRunning = false; appTimer.stop(); }
        virtual bool    isRunning       () { return appIsRunning; }
};

} // end hge namespace

#endif	/* __HGE_APPLICATION_H__ */

