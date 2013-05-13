/* 
 * File:   object.cpp
 * Author: hammy
 * 
 * Created on December 4, 2012, 6:48 PM
 */

#include "pipeline.h"
#include "object.h"

namespace hge {

//-----------------------------------------------------------------------------
//	Object - Basic Object Abstraction
//-----------------------------------------------------------------------------
c_object::~c_object() {}

//-----------------------------------------------------------------------------
//	Drawable Object - Abstraction
//-----------------------------------------------------------------------------
c_drawableObj::~c_drawableObj() {}

void c_drawableObj::setRotation( const vec3& angles ) {
	rot = fromEuler( angles );
}

void c_drawableObj::rotate( const vec3& angles ) {
	rot *= fromEuler( angles );
}

void c_drawableObj::update() {
	
	modelMat
        = quatToMat4( rot )
        * mat4(
            scale.v[0], 0.f, 0.f, 0.f,
            0.f, scale.v[1], 0.f, 0.f,
            0.f, 0.f, scale.v[2], 0.f,
            0.f, 0.f, 0.f, 1.f
	);

	modelMat[3][0] = pos.v[0];
	modelMat[3][1] = pos.v[1];
	modelMat[3][2] = pos.v[2];
	modelMat[3][3] = 1.f;
}

} // end hge namespace
