/* 
 * File:   newClass.cpp
 * Author: hammy
 * 
 * Created on June 7, 2013, 5:51 PM
 */

#include "transformations.h"

namespace hge {

//-----------------------------------------------------------------------------
//	Draw Transformations
//-----------------------------------------------------------------------------
c_drawTransform::c_drawTransform() :
    pos( 0.f ),
    scaling( 1.f ),
    rot( 0.f, 0.f, 0.f, 1.f ),
    modelMat( 1.f )
{}

c_drawTransform::c_drawTransform( const c_drawTransform& obj ) :
    pos( obj.pos ),
    scaling( obj.scaling ),
    rot( obj.rot ),
    modelMat( obj.modelMat )
{}

c_drawTransform& c_drawTransform::operator = ( const c_drawTransform& i ) {
    pos = i.pos;
    scaling = i.scaling;
    rot = i.rot;
    modelMat = i.modelMat;
}

void c_drawTransform::setPos( const vec3& p ) {
    pos = p;
}

void c_drawTransform::setScale( const vec3& s ) {
    scaling = s;
}

void c_drawTransform::setRotation( const vec3& angles ) {
    rot = fromEuler( angles );
}

void c_drawTransform::move( const vec3& p ) {
    pos += p;
}

void c_drawTransform::scale( const vec3& s ) {
    scaling += s;
}

void c_drawTransform::rotate( const vec3& angles ) {
    rot *= fromEuler( angles );
}

void c_drawTransform::update() {
	modelMat
        = quatToMat4( rot )
        * mat4(
            scaling.v[0],   0.f,            0.f,            0.f,
            0.f,            scaling.v[1],   0.f,            0.f,
            0.f,            0.f,            scaling.v[2],   0.f,
            0.f,            0.f,            0.f,            1.f
	);

	modelMat[3][0] = pos.v[0];
	modelMat[3][1] = pos.v[1];
	modelMat[3][2] = pos.v[2];
	modelMat[3][3] = 1.f;
}

} // end hge namespace
