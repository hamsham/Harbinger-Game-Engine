/* 
 * File:   camera.cpp
 * Author: hammy
 * 
 * Created on December 7, 2012, 6:23 PM
 */

#include "hamLibs.h"

#include "types.h"
#include "camera.h"

using namespace hamLibs;

//-----------------------------------------------------------------------------
//	Camera - Construction
//-----------------------------------------------------------------------------
const float c_camera::DEFAULT_ASPECT_WIDTH( 4.f );
const float c_camera::DEFAULT_ASPECT_HEIGHT( 3.f );
const float c_camera::DEFAULT_Z_NEAR( 0.1f );
const float c_camera::DEFAULT_Z_FAR( 100.f );
const float c_camera::DEFAULT_FOV( 45.f );

const math::mat4 c_camera::DEFAULT_PERSPECTIVE(
	math::perspective(
		DEFAULT_FOV,
		DEFAULT_ASPECT_WIDTH / DEFAULT_ASPECT_HEIGHT,
		DEFAULT_Z_NEAR,
		DEFAULT_Z_FAR
	)
);

c_camera::c_camera() :
	c_object(),
	vpMat(),
	projMat( DEFAULT_PERSPECTIVE ),
	viewMat(),
	orientation( 0.f, 0.f, 0.f, 1.f ),
	xAxis( 1.f, 0.f, 0.f ),
	yAxis( 0.f, 1.f, 0.f ),
	zAxis( 0.f, 0.f, 1.f ),
	deltaPos( 0.f, 0.f, 0.f ),
	target( 0.f, 0.f, 0.f ),
	orbitDistance( 1.f ),
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
	vpMat( camCopy.vpMat ),
	projMat( camCopy.projMat ),
	viewMat( camCopy.viewMat ),
	orientation( camCopy.orientation ),
	xAxis( camCopy.xAxis ),
	yAxis( camCopy.yAxis ),
	zAxis( camCopy.zAxis ),
	deltaPos( camCopy.deltaPos ),
	target( camCopy.target ),
	orbitDistance( camCopy.orbitDistance ),
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
void c_camera::look( const math::vec3& camTarget ) {
	look( pos, camTarget, yAxis );
}

void c_camera::look( const math::vec3& camPos, const math::vec3& camTarget, const math::vec3& camUp ) {
	pos = camPos;
	target = camTarget;
	
	zAxis = math::normalize( pos - target );
	xAxis = math::normalize( math::cross( camUp, zAxis ) );
	yAxis = math::normalize( math::cross( zAxis, xAxis ) );

	viewMat[0][0] = xAxis.v[0];
	viewMat[1][0] = xAxis.v[1];
	viewMat[2][0] = xAxis.v[2];
	viewMat[3][0] = -math::dot( xAxis, pos );
	
	viewMat[0][1] = yAxis.v[0];
	viewMat[1][1] = yAxis.v[1];
	viewMat[2][1] = yAxis.v[2];
	viewMat[3][1] = -math::dot( yAxis, pos );
	
	viewMat[0][2] = zAxis.v[0];
	viewMat[1][2] = zAxis.v[1];
	viewMat[2][2] = zAxis.v[2];
	viewMat[3][2] = -math::dot( zAxis, pos );
	
	orientation = math::matToQuat( viewMat );
}

//-----------------------------------------------------------------------------
//	Camera - Rotations
//-----------------------------------------------------------------------------
void c_camera::rotateFPS() {
	orientation
		= orientation
		* math::fromAxisAngle( c_object::WORLD_AXIS_X, pitch )
		* math::fromAxisAngle( c_object::WORLD_AXIS_Y, yaw )
		* math::fromAxisAngle( c_object::WORLD_AXIS_Z, roll );
}

void c_camera::rotateFlight() {
	orientation
		= math::fromAxisAngle( zAxis, roll )
		* math::fromAxisAngle( yAxis, yaw )
		* math::fromAxisAngle( xAxis, pitch )
		* orientation;
	
}

void c_camera::rotateOrbit() {
	orientation
		= orientation
		* math::fromAxisAngle( zAxis, roll )
		* math::fromAxisAngle( yAxis, yaw )
		* math::fromAxisAngle( xAxis, pitch );
	
	// Stripped down version of the LookAt function
	viewMat[0][0] = xAxis.v[0]; viewMat[1][0] = xAxis.v[1]; viewMat[2][0] = xAxis.v[2];
	viewMat[0][1] = yAxis.v[0]; viewMat[1][1] = yAxis.v[1]; viewMat[2][1] = yAxis.v[2];
	viewMat[0][2] = zAxis.v[0]; viewMat[1][2] = zAxis.v[1]; viewMat[2][2] = zAxis.v[2];
	
	viewMat = math::quatToMat4( orientation ) * viewMat;
}

void c_camera::unRoll() {
	if ( camType == CAM_TYPE_ORBIT ) {
		look( pos, target + zAxis * orbitDistance, math::vec3( 0.f, 1.f, 0.f ) );
	}
	else {
		look( pos, zAxis, math::vec3( 0.f, 1.f, 0.f ) );
	}
}

//-----------------------------------------------------------------------------
//	Camera - Movement
//-----------------------------------------------------------------------------
void c_camera::moveFPS( float timeElapsed ) {
	math::vec3 forwards(
		math::normalize(
			math::cross( c_object::WORLD_AXIS_Y, xAxis )
		)
	);
	deltaPos += ( posVel * timeElapsed ) + ( posAccel * timeElapsed * timeElapsed );
	pos += xAxis * deltaPos.v[0];
	pos += c_object::WORLD_AXIS_Y * deltaPos.v[1];
	pos += forwards * deltaPos.v[2];
}

void c_camera::moveSpectator( float timeElapsed ) {
	math::vec3 forwards( -zAxis );
	deltaPos += ( posVel * timeElapsed ) + ( posAccel * timeElapsed * timeElapsed );
	pos += c_object::WORLD_AXIS_X * deltaPos.v[0];
	pos += c_object::WORLD_AXIS_Y * deltaPos.v[1];
	pos += forwards * deltaPos.v[2];
}

void c_camera::moveFlight( float timeElapsed ) {
	math::vec3 forwards( -zAxis );
	deltaPos += ( posVel * timeElapsed ) + ( posAccel * timeElapsed * timeElapsed );
	pos += xAxis * deltaPos.v[0];
	pos += yAxis * deltaPos.v[1];
	pos += forwards * deltaPos.v[2];
}

void c_camera::moveOrbit( float timeElapsed ) {
	orbitDistance
	= deltaPos.v[2]
	+ ( posVel.v[2] * timeElapsed )
	+ ( posAccel.v[2] * timeElapsed * timeElapsed );
	
	pos -= ( target + zAxis ) * orbitDistance;
}

//-----------------------------------------------------------------------------
//	Camera - Final Updates
//-----------------------------------------------------------------------------
void c_camera::tick( float timeElapsed ) {
	
	// Caution: Function pointers are used to handle movement & rotation functions
	
	// Rotation
	pitch -= ( angVel.v[0] * timeElapsed ) - ( angAccel.v[0] * timeElapsed * timeElapsed );
	yaw -= ( angVel.v[1] * timeElapsed ) - ( angAccel.v[1] * timeElapsed * timeElapsed );
	roll -= ( angVel.v[2] * timeElapsed ) - ( angAccel.v[2] * timeElapsed * timeElapsed );
	
	( this->*rotationFunction[ camType ] )();
	orientation = math::normalize( orientation );
	pitch = yaw = roll = 0.f;
	
	//Movement
	( this->*moveFunction[ camType ] )( timeElapsed );
	deltaPos.v[0] = deltaPos.v[1] = deltaPos.v[2] = 0.f;
	
	//update the view matrix
	if ( camType != CAM_TYPE_ORBIT ) {
		viewMat = math::quatToMat4( orientation );
		xAxis = math::vec3( viewMat[0][0], viewMat[1][0], viewMat[2][0] );
		yAxis = math::vec3( viewMat[0][1], viewMat[1][1], viewMat[2][1] );
		zAxis = math::vec3( viewMat[0][2], viewMat[1][2], viewMat[2][2] );
	}
	
	viewMat[3][0] = -math::dot( xAxis, pos );
	viewMat[3][1] = -math::dot( yAxis, pos );
	viewMat[3][2] = -math::dot( zAxis, pos );
	
	vpMat = viewMat * projMat;
}
