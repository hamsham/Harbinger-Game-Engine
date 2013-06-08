/* 
 * File:   camera.cpp
 * Author: hammy
 * 
 * Created on December 7, 2012, 6:23 PM
 */

#include "pipeline.h"
#include "camera.h"

namespace hge {

const float c_camera::DEFAULT_ASPECT_WIDTH( 4.f );
const float c_camera::DEFAULT_ASPECT_HEIGHT( 3.f );
const float c_camera::DEFAULT_Z_NEAR( 0.1f );
const float c_camera::DEFAULT_Z_FAR( 100.f );
const float c_camera::DEFAULT_FOV( 45.f );

const mat4 c_camera::DEFAULT_PERSPECTIVE(
	perspective(
		DEFAULT_FOV,
		DEFAULT_ASPECT_WIDTH / DEFAULT_ASPECT_HEIGHT,
		DEFAULT_Z_NEAR,
		DEFAULT_Z_FAR
	)
);

/******************************************************************************
 * CAMERA -- Construction & Operators
******************************************************************************/
c_camera::c_camera() :
    rotateFunction{ &c_camera::rotateLockedY, &c_camera::rotateOrbitLockedY },
    updateFunction{ &c_camera::updateNormal, &c_camera::updateOrbit },
    viewMode( VIEW_NORMAL ),
    fov( DEFAULT_FOV ),
    aspectW( DEFAULT_ASPECT_WIDTH ),
    aspectH( DEFAULT_ASPECT_HEIGHT ),
    zNear( DEFAULT_Z_NEAR ),
    zFar( DEFAULT_Z_FAR ),
    orbitDist( 1.f ),
    pos( 0.f ),
    target( 0.f ),
    xAxis( pipeline::getWorldAxisX() ),
    yAxis( pipeline::getWorldAxisY() ),
    zAxis( pipeline::getWorldAxisZ() ),
    angles( vec3( 0.f, 0.f, 0.f ) ),
    orientation( quat( 0.f, 0.f, 0.f, 1.f ) ),
    viewMatrix( mat4( 1.f ) ),
    projMatrix(
        infinitePerspective( DEFAULT_FOV, DEFAULT_ASPECT_WIDTH/DEFAULT_ASPECT_HEIGHT, DEFAULT_Z_NEAR )
    )
{}

c_camera::c_camera( const c_camera& c ) :
    rotateFunction{ &c_camera::rotateLockedY, &c_camera::rotateOrbitLockedY },
    updateFunction{ &c_camera::updateNormal, &c_camera::updateOrbit },
    viewMode( c.viewMode ),
    fov( c.fov ),
    aspectW( c.aspectW ),
    aspectH( c.aspectH ),
    zNear( c.zNear ),
    zFar( c.zFar ),
    orbitDist( c.orbitDist ),
    pos( c.pos ),
    target( c.target ),
    xAxis( c.xAxis ),
    yAxis( c.yAxis ),
    zAxis( c.zAxis ),
    angles( c.angles ),
    orientation( c.orientation ),
    viewMatrix( c.viewMatrix ),
    projMatrix( c.projMatrix )
{}

c_camera& c_camera::operator = ( const c_camera& c ) {
    rotateFunction[ 0 ] = &c_camera::rotateLockedY;
    rotateFunction[ 1 ] = &c_camera::rotateOrbitLockedY;
    updateFunction[ 0 ] = &c_camera::updateNormal;
    updateFunction[ 1 ] = &c_camera::updateOrbit;
    viewMode = c.viewMode;
    fov = c.fov;
    aspectW = c.aspectW;
    aspectH = c.aspectH;
    zNear = c.zNear;
    zFar = c.zFar;
    orbitDist = c.orbitDist;
    pos = c.pos;
    target = c.target;
    xAxis = c.xAxis;
    yAxis = c.yAxis;
    zAxis = c.zAxis;
    angles = c.angles;
    orientation = c.orientation;
    viewMatrix = c.viewMatrix;
    projMatrix = c.projMatrix;
}

/******************************************************************************
 * CAMERA -- Functions
******************************************************************************/
void c_camera::makeOrtho() {
    //projMatrix = ortho( -aspectW, aspectW, -aspectH, aspectH, zNear, zFar );
    projMatrix = ortho( -aspectW, aspectW, -aspectH, aspectH );
}

void c_camera::makePerspective() {
    projMatrix = infinitePerspective(
        fov, aspectW/aspectH, zNear
    );
}

void c_camera::setProjectionParams(
    float inFov, float aspectWidth, float aspectHeight, float near, float far
) {
    fov = inFov;
    aspectW = aspectWidth;
    aspectH = aspectHeight;
    zNear = near;
    zFar = far;
}

void c_camera::lockYAxis( bool isLocked ) {
    rotateFunction[ VIEW_NORMAL ] = ( isLocked )
        ? &c_camera::rotateLockedY
        : &c_camera::rotateUnlockedY;
    
    rotateFunction[ VIEW_ORBIT ] = ( isLocked )
        ? &c_camera::rotateOrbitLockedY
        : &c_camera::rotateOrbitUnlockedY;
    
}

void c_camera::look( const vec3& eye, const vec3& point, const vec3& up ) {
    pos = eye;
    target = point;
    
	zAxis = normalize( pos - target );
	xAxis = normalize( cross( up, zAxis ) );
	yAxis = normalize( cross( zAxis, xAxis ) );
	
    viewMatrix[0][0] = xAxis.v[0];
    viewMatrix[1][0] = xAxis.v[1];
    viewMatrix[2][0] = xAxis.v[2];
    
    viewMatrix[0][1] = yAxis.v[0];
    viewMatrix[1][1] = yAxis.v[1];
    viewMatrix[2][1] = yAxis.v[2];
    
    viewMatrix[0][2] = zAxis.v[0];
    viewMatrix[1][2] = zAxis.v[1];
    viewMatrix[2][2] = zAxis.v[2];
    
    orientation = matToQuat( viewMatrix );
}
        
void c_camera::look( const vec3& point ) {
    look( pos, point, yAxis );
}

void c_camera::move( const vec3& amount ) {
    pos -= xAxis * amount[0];
    pos -= yAxis * amount[1];
    pos -= zAxis * amount[2];
}

void c_camera::rotate( const vec3& amount ) {
    ( this->*rotateFunction[ viewMode ] )( amount );
}

void c_camera::rotateUnlockedY( const vec3& amount ) {
    orientation *= fromEuler( -amount );
}

void c_camera::rotateLockedY( const vec3& amount ) {
    orientation
        = fromAxisAngle( zAxis, -amount[2] )
        * fromAxisAngle( vec3( 0.f, 1.f, 0.f), -amount[1] )
        * fromAxisAngle( xAxis, -amount[0] )
        * orientation;
}

void c_camera::rotateOrbitUnlockedY( const vec3& amount ) {
    pos -= xAxis * amount[0];
    pos -= yAxis * amount[1];
    pos -= zAxis * amount[2];
}

void c_camera::rotateOrbitLockedY( const vec3& amount ) {
    rotateOrbitUnlockedY( amount );
    look( pos, target, vec3( 0.f, 1.f, 0.f ) );
}

void c_camera::unroll() {
	if ( viewMode == VIEW_ORBIT )   look( pos, target, vec3( 0.f, 1.f, 0.f ) );
	else                            look( pos, pos - zAxis, vec3( 0.f, 1.f, 0.f ) );
}

void c_camera::updateNormal() {
    viewMatrix = quatToMat4( orientation );
    
    xAxis = vec3( viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0] );
    yAxis = vec3( viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1] );
    zAxis = vec3( viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2] );
}

void c_camera::updateOrbit() {
    pos = normalize( pos - target ) * orbitDist;
    look( target );
}

void c_camera::update() {
    ( this->*updateFunction[ viewMode ] )();
    viewMatrix[3][0] = -dot( xAxis, pos );
    viewMatrix[3][1] = -dot( yAxis, pos );
    viewMatrix[3][2] = -dot( zAxis, pos );
}

} // end hge namespace
