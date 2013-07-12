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
drawTransform::drawTransform() :
    pos( 0.f ),
    scaling( 1.f ),
    rot( 0.f, 0.f, 0.f, 1.f ),
    modelMat( 1.f )
{}

drawTransform::drawTransform( const drawTransform& obj ) :
    pos( obj.pos ),
    scaling( obj.scaling ),
    rot( obj.rot ),
    modelMat( obj.modelMat )
{}

drawTransform& drawTransform::operator = ( const drawTransform& i ) {
    pos = i.pos;
    scaling = i.scaling;
    rot = i.rot;
    modelMat = i.modelMat;
}

void drawTransform::setPos( const vec3& p ) {
    pos = p;
}

void drawTransform::setScale( const vec3& s ) {
    scaling = s;
}

void drawTransform::setOrientation( const quat& o ) {
    rot = o;
}

void drawTransform::move( const vec3& p ) {
    pos += p;
}

void drawTransform::scale( const vec3& s ) {
    scaling += s;
}

void drawTransform::rotate( const vec3& angles ) {
    rot *= fromEuler( angles );
}

void drawTransform::update() {
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
