/* 
 * File:   object.cpp
 * Author: hammy
 * 
 * Created on December 4, 2012, 6:48 PM
 */

#include "object.h"

//-----------------------------------------------------------------------------
//	Object - Basic Object Abstraction
//-----------------------------------------------------------------------------
const math::vec3 c_object::WORLD_AXIS_X( 1.f, 0.f, 0.f );
const math::vec3 c_object::WORLD_AXIS_Y( 0.f, 1.f, 0.f );
const math::vec3 c_object::WORLD_AXIS_Z( 0.f, 0.f, 1.f );
const math::mat4 c_object::WORLD_MATRIX(
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

void c_drawableObj::setRotation( const math::vec3& angles ) {
	rot = math::normalize (
		math::fromAxisAngle( WORLD_AXIS_Z, angles.v[2] ) *
		math::fromAxisAngle( WORLD_AXIS_Y, angles.v[1] ) *
		math::fromAxisAngle( WORLD_AXIS_X, angles.v[0] )
	);
}

void c_drawableObj::rotate( const math::vec3& angles ) {
	rot = math::normalize (
		math::fromAxisAngle( WORLD_AXIS_Z, angles.v[2] ) *
		math::fromAxisAngle( WORLD_AXIS_Y, angles.v[1] ) *
		math::fromAxisAngle( WORLD_AXIS_X, angles.v[0] ) *
		rot
	);
}

void c_drawableObj::update() {
	const math::mat4 scaleMat(
		scale.v[0], 0.f, 0.f, 0.f,
		0.f, scale.v[1], 0.f, 0.f,
		0.f, 0.f, scale.v[2], 0.f,
		0.f, 0.f, 0.f, 1.f
	);
	
	worldMat = math::quatToMat4( rot ) * scaleMat;

	worldMat[3][0] = pos.v[0];
	worldMat[3][1] = pos.v[1];
	worldMat[3][2] = pos.v[2];
	worldMat[3][3] = 1.f;
}
