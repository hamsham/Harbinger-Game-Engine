/* 
 * File:   camera.cpp
 * Author: hammy
 * 
 * Created on December 7, 2012, 6:23 PM
 */

#include "types.h"
#include "camera.h"

namespace hge {

//-----------------------------------------------------------------------------
//	Camera - Construction
//-----------------------------------------------------------------------------
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

c_camera::c_camera() :
	c_object(),
	projMat( DEFAULT_PERSPECTIVE ),
	viewMat(),
	orientation( 0.f, 0.f, 0.f, 1.f ),
	xAxis( 1.f, 0.f, 0.f ),
	yAxis( 0.f, 1.f, 0.f ),
	zAxis( 0.f, 0.f, 1.f ),
	target( 0.f, 0.f, 0.f ),
	posVel( 0.f, 0.f, 0.f ),
	angVel( 0.f, 0.f, 0.f ),
	posAccel( 0.f, 0.f, 0.f ),
	angAccel( 0.f, 0.f, 0.f ),
	pitch( 0.f ),
	yaw( 0.f ),
	roll( 0.f ),
	fov( DEFAULT_FOV ),
	aspectW( DEFAULT_ASPECT_WIDTH ),
	aspectH( DEFAULT_ASPECT_HEIGHT ),
	zNear( DEFAULT_Z_NEAR ),
	zFar( DEFAULT_Z_FAR ),
	camType( CAM_TYPE_FPS ),
	// the "rotationFunction" and "moveFunction" function pointer arrays must
	// correspond to the "cameraType" enumeration
	rotationFunction{ &c_camera::rotateFPS, &c_camera::rotateSpectator, &c_camera::rotateFlight, &c_camera::rotateOrbit },
	moveFunction{ &c_camera::moveFPS, &c_camera::moveSpectator, &c_camera::moveFlight, &c_camera::moveOrbit }
{}

c_camera::c_camera( const c_camera& camCopy ) :
	c_object( camCopy ),
	projMat( camCopy.projMat ),
	viewMat( camCopy.viewMat ),
	orientation( camCopy.orientation ),
	xAxis( camCopy.xAxis ),
	yAxis( camCopy.yAxis ),
	zAxis( camCopy.zAxis ),
	target( camCopy.target ),
	posVel( camCopy.posVel ),
	angVel( camCopy.angVel ),
	posAccel( camCopy.posAccel ),
	angAccel( camCopy.angAccel ),
	pitch( camCopy.pitch ),
	yaw( camCopy.yaw ),
	roll( camCopy.roll ),
	fov( camCopy.fov ),
	aspectW( camCopy.aspectW ),
	aspectH( camCopy.aspectH ),
	zNear( camCopy.zNear ),
	zFar( camCopy.zFar ),
	camType( camCopy.camType ),
	// the "rotationFunction" and "moveFunction" function pointer arrays must
	// correspond to the "cameraType" enumeration
	rotationFunction{ &c_camera::rotateFPS, &c_camera::rotateSpectator, &c_camera::rotateFlight, &c_camera::rotateOrbit },
	moveFunction{ &c_camera::moveFPS, &c_camera::moveSpectator, &c_camera::moveFlight, &c_camera::moveOrbit }
{}

//-----------------------------------------------------------------------------
//	Camera - Looking
//-----------------------------------------------------------------------------
void c_camera::look( const vec3& camTarget ) {
	look( pos, camTarget, yAxis );
}

void c_camera::look( const vec3& camPos, const vec3& camTarget, const vec3& camUp ) {
	pos = camPos;
	target = camTarget;
	
	zAxis = normalize( pos - target );
	xAxis = normalize( cross( camUp, zAxis ) );
	yAxis = normalize( cross( zAxis, xAxis ) );

	viewMat[0][0] = xAxis.v[0];
	viewMat[1][0] = xAxis.v[1];
	viewMat[2][0] = xAxis.v[2];
	viewMat[3][0] = -dot( xAxis, pos );
	
	viewMat[0][1] = yAxis.v[0];
	viewMat[1][1] = yAxis.v[1];
	viewMat[2][1] = yAxis.v[2];
	viewMat[3][1] = -dot( yAxis, pos );
	
	viewMat[0][2] = zAxis.v[0];
	viewMat[1][2] = zAxis.v[1];
	viewMat[2][2] = zAxis.v[2];
	viewMat[3][2] = -dot( zAxis, pos );
	
	orientation = matToQuat( viewMat );
}

//-----------------------------------------------------------------------------
//	Camera - Rotations
//-----------------------------------------------------------------------------
void c_camera::rotateFPS() {
	orientation
		= orientation
		* fromAxisAngle( c_object::WORLD_AXIS_X, pitch )
		* fromAxisAngle( c_object::WORLD_AXIS_Y, yaw )
		* fromAxisAngle( c_object::WORLD_AXIS_Z, roll );
}

void c_camera::rotateFlight() {
	orientation
		= fromAxisAngle( zAxis, roll )
		* fromAxisAngle( yAxis, yaw )
		* fromAxisAngle( xAxis, pitch )
		* orientation;
}

void c_camera::rotateOrbit() {
    rotateFlight();
/*
    // Stripped down version of the LookAt function
    viewMat[0][0] = xAxis.v[0]; viewMat[1][0] = xAxis.v[1]; viewMat[2][0] = xAxis.v[2];
    viewMat[0][1] = yAxis.v[0]; viewMat[1][1] = yAxis.v[1]; viewMat[2][1] = yAxis.v[2];
    viewMat[0][2] = zAxis.v[0]; viewMat[1][2] = zAxis.v[1]; viewMat[2][2] = zAxis.v[2];

    viewMat = quatToMat4( orientation ) * viewMat;
*/
}

void c_camera::unRoll() {
	if ( camType == CAM_TYPE_ORBIT ) {
		look( pos, target, vec3( 0.f, 1.f, 0.f ) );
	}
	else {
		look( pos, pos - zAxis, c_object::WORLD_AXIS_Y );
	}
}

//-----------------------------------------------------------------------------
//	Camera - Movement
//-----------------------------------------------------------------------------
void c_camera::moveFPS() {
	vec3 forwards( cross( c_object::WORLD_AXIS_Y, xAxis ) );
	vec3 strafe( cross( c_object::WORLD_AXIS_Y, zAxis ) );
	
	pos += strafe * deltaPos.v[0];
	pos += c_object::WORLD_AXIS_Y * deltaPos.v[1];
	pos += forwards * deltaPos.v[2];
}

void c_camera::moveSpectator() {
	vec3 strafe( cross( c_object::WORLD_AXIS_Y, zAxis ) );
	
	pos += strafe * deltaPos.v[0];
	pos += c_object::WORLD_AXIS_Y * deltaPos.v[1];
	pos -= zAxis * deltaPos.v[2];
}

void c_camera::moveFlight() {
	pos += xAxis * deltaPos.v[0];
	pos += yAxis * deltaPos.v[1];
	pos -= zAxis * deltaPos.v[2];
}

void c_camera::moveOrbit() {
	//pos -= ( target + zAxis ) * deltaPos.v[2];
}

//-----------------------------------------------------------------------------
//	Camera - Final Updates
//-----------------------------------------------------------------------------
void c_camera::tick( float timeElapsed ) {
	
	// Caution: Function pointers are used to handle movement & rotation functions
	float timeSquared( timeElapsed * timeElapsed );
	
	// Rotation
	pitch   -= ( angVel.v[0] * timeElapsed ) - ( angAccel.v[0] * timeSquared );
	yaw     -= ( angVel.v[1] * timeElapsed ) - ( angAccel.v[1] * timeSquared );
	roll    -= ( angVel.v[2] * timeElapsed ) - ( angAccel.v[2] * timeSquared );
	
	( this->*rotationFunction[ camType ] )();
	orientation = normalize( orientation );
	pitch = yaw = roll = 0.f;
	
	//Movement
	deltaPos += ( posVel * timeElapsed ) + ( posAccel * timeSquared );
	( this->*moveFunction[ camType ] )();
    
    viewMat = quatToMat4( orientation );
    xAxis = vec3( viewMat[0][0], viewMat[1][0], viewMat[2][0] );
    yAxis = vec3( viewMat[0][1], viewMat[1][1], viewMat[2][1] );
    zAxis = vec3( viewMat[0][2], viewMat[1][2], viewMat[2][2] );
	
	//update the view matrix
	if ( camType == CAM_TYPE_ORBIT )
        pos -= ( target + zAxis ) * deltaPos.v[2];
    deltaPos = 0.f;
    
    viewMat[3][0] = -dot( xAxis, pos );
    viewMat[3][1] = -dot( yAxis, pos );
    viewMat[3][2] = -dot( zAxis, pos );
}

} // end hge namespace
