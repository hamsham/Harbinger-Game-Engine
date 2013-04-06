/* 
 * File:   object.cpp
 * Author: hammy
 * 
 * Created on December 4, 2012, 6:48 PM
 */

#include "object.h"

namespace hge {

//-----------------------------------------------------------------------------
//	Object - Basic Object Abstraction
//-----------------------------------------------------------------------------
const vec3 c_object::WORLD_AXIS_X( 1.f, 0.f, 0.f );
const vec3 c_object::WORLD_AXIS_Y( 0.f, 1.f, 0.f );
const vec3 c_object::WORLD_AXIS_Z( 0.f, 0.f, 1.f );
const mat4 c_object::WORLD_MATRIX(
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f
);

c_object::~c_object() {}

//-----------------------------------------------------------------------------
//	Drawable Object - Abstraction
//-----------------------------------------------------------------------------
c_drawableObj::~c_drawableObj() {}

void c_drawableObj::setRotation( const vec3& angles ) {
	rot = normalize (
		fromAxisAngle( WORLD_AXIS_Z, angles.v[2] ) *
		fromAxisAngle( WORLD_AXIS_Y, angles.v[1] ) *
		fromAxisAngle( WORLD_AXIS_X, angles.v[0] )
	);
}

void c_drawableObj::rotate( const vec3& angles ) {
	rot = normalize (
		fromAxisAngle( WORLD_AXIS_Z, angles.v[2] ) *
		fromAxisAngle( WORLD_AXIS_Y, angles.v[1] ) *
		fromAxisAngle( WORLD_AXIS_X, angles.v[0] ) *
		rot
	);
}

void c_drawableObj::update() {
	const mat4 scaleMat(
		scale.v[0], 0.f, 0.f, 0.f,
		0.f, scale.v[1], 0.f, 0.f,
		0.f, 0.f, scale.v[2], 0.f,
		0.f, 0.f, 0.f, 1.f
	);
	
	worldMat = quatToMat4( rot ) * scaleMat;

	worldMat[3][0] = pos.v[0];
	worldMat[3][1] = pos.v[1];
	worldMat[3][2] = pos.v[2];
	worldMat[3][3] = 1.f;
}

} // end hge namespace
