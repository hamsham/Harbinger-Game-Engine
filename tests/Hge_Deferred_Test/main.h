/* 
 * File:   main.h
 * Author: hammy
 *
 * Created on April 16, 2013, 5:46 PM
 */

#ifndef __MAIN_H__
#define	__MAIN_H__

#include <sstream>
#include <string>
#include "harbinger.h"

/******************************************************************************
 * Namespace setup
******************************************************************************/
using namespace hamLibs;

#define HGE_MODEL_MAT hge::pipeline::HGE_MODEL_MAT
#define HGE_VP_MAT hge::pipeline::HGE_VP_MAT

/******************************************************************************
 * Forward declarations of functions
******************************************************************************/
unsigned tickI();

/******************************************************************************
 * Forward declarations of classes
******************************************************************************/
class renderApp;
class gameLogic;
class gameSystem;

class blankNormalMap;

/******************************************************************************
 * Global variables
******************************************************************************/
#define CAM_MOVE_SPEED      5.f
#define CAM_ROTATE_SPEED    (1.f / 250.f)

extern hge::dsRenderer*      pRenderer;
extern gameLogic*       pLogic;

/******************************************************************************
 * Global functions
******************************************************************************/
template <typename type>
std::string to_string(type t) {
    std::ostringstream oss;
    oss << t;
    return oss.str();
}

#endif	/* __MAIN_H__ */

