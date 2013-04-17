/* 
 * File:   camera.h
 * Author: hammy
 *
 * Created on December 7, 2012, 6:23 PM
 */

#ifndef __HGE_CAMERA_H__
#define	__HGE_CAMERA_H__

#include "types.h"
#include "object.h"

namespace hge {

/*
 * NOTE:
 *	Much of this code is based on the camera demos found at
 *	http://www.dhpoware.com
 *	I would like to point out that while I have altered the code to fit the
 *	needs of the engine, most of the original camera control algorithms were
 *	copied from the dhpoware examples.
 */

///////////////////////////////////////////////////////////////////////////////
//		Camera & View Control
///////////////////////////////////////////////////////////////////////////////
class HGE_API c_camera : virtual public c_object {
	public:
		static const float	DEFAULT_ASPECT_WIDTH;
		static const float	DEFAULT_ASPECT_HEIGHT;
		static const float	DEFAULT_Z_NEAR;
		static const float	DEFAULT_Z_FAR;
		static const float	DEFAULT_FOV;
		static const mat4   DEFAULT_PERSPECTIVE;
		
		enum cameraType {
			CAM_TYPE_FPS,
			CAM_TYPE_SPECTATOR,
			CAM_TYPE_FLIGHT,
			CAM_TYPE_ORBIT
		};
	
	private:
		//render system output
		mat4        projMat;
		mat4        viewMat;
		//rotation & orientation
		quat        orientation;
		vec3        xAxis;
		vec3        yAxis;
		vec3        zAxis;
		//positioning
		vec3        target; // used for orbiting
		//time-based movements
		vec3        posVel;
		vec3        angVel;
		vec3        posAccel;
		vec3        angAccel;
		//euler angles
		float		pitch;
		float		yaw;
		float		roll;
		//viewport settings
		float		fov;
		float		aspectW; // for orthographic scenes
		float		aspectH;
		float		zNear;
		float		zFar;
		cameraType  camType;
		
		//function pointer arrays for movement and rotations
		void ( c_camera::*rotationFunction[ 4 ] )();
		void ( c_camera::*moveFunction[ 4 ] )();
		
		void		moveFPS			();
		void		moveSpectator	();
		void		moveFlight		();
		void		moveOrbit		();
		void		rotateFPS		();
		void		rotateSpectator	()							{ rotateFPS(); }
		void		rotateFlight	();
		void		rotateOrbit		();
		
	public:
		c_camera	();
		c_camera	( const c_camera& orig );
		~c_camera	() {}
		
		//Render System
		const mat4	getVPMat        () const					{ return viewMat * projMat; }
		const mat4&	getViewMat      () const					{ return viewMat; }
		const mat4&	getProjMat      () const					{ return projMat; }
		void		setOrtho        ();
		void		setPerspective  ();
		void		setProjection   (
						float inFov         = DEFAULT_FOV,
						float aspectWidth   = DEFAULT_ASPECT_WIDTH,
						float aspectHeight  = DEFAULT_ASPECT_HEIGHT,
						float inZNear       = DEFAULT_Z_NEAR,
						float inZFar        = DEFAULT_Z_FAR
                    );
		
		//rotation & orientation
		const quat&	getOrientation() const						{ return orientation; }
		const vec3&	getAxisX	() const						{ return xAxis; }
		const vec3&	getAxisY	() const						{ return yAxis; }
		const vec3&	getAxisZ	() const						{ return zAxis; }
		
		//Positioning
		const vec3&	getUp		() const						{ return yAxis; }
		void		setTarget	( const vec3& inTarget )        { target = inTarget; }
		const vec3&	getTarget	() const						{ return target; }
		
		//time-based movement
		void		setPosVel	( const vec3& inVel )           { posVel = inVel; }
		const vec3&	getPosVel	() const						{ return posVel; }
		void		setAngVel	( const vec3& inVel )           { angVel = inVel; }
		const vec3&	getAngVel	() const						{ return angVel; }
		void        setPosAccel	( const vec3& inAccel )         { posAccel = inAccel; }
		const vec3&	getPosAccel	() const						{ return posAccel; }
		void		setAngAccel	( const vec3& inAccel )         { angAccel = inAccel; }
		const vec3&	getAngAccel	() const						{ return angAccel; }
		
		//viewport
		void		setFOV		( float inFOV )					{ fov = inFOV; }
		float		getFOV		() const						{ return fov; }
		void		setAspect	( float width, float height )	{ aspectW = width; aspectH = height; }
		float		getAspect	() const						{ return aspectW / aspectH; }
		void		setZNear	( float inZNear )				{ zNear = inZNear; }
		float		getZNear	() const						{ return zNear; }
		void		setZFar		( float inZFar )				{ zFar = inZFar; }
		float		getZFar		() const						{ return zFar; }
		void		setCamType	( cameraType  ct )				{ camType = ct; target = -zAxis; }
		cameraType	getCamType	() const						{ return camType; }
		
		//static viewing functions
		
		//Camera Updating
		void		look		( const vec3& camTarget );
		void		look		( const vec3& camPos, const vec3& camTarget, const vec3& camUp );
		void		rotate		( float inPitch, float inYaw, float inRoll = 0.f );
		void		unRoll		();
		void		update		() { tick( 1.f ); }
		void		tick		( float timeElapsed );
};

//-----------------------------------------------------------------------------
//	Camera - Projections
//-----------------------------------------------------------------------------
HGE_INLINE void c_camera::setOrtho() {
	//projMat = ortho( -aspectW, aspectW, -aspectH, aspectH, zNear, zFar );
	projMat = ortho( -aspectW, aspectW, -aspectH, aspectH );
    update();
}

HGE_INLINE void c_camera::setPerspective() {
	projMat = infinitePerspective( fov, aspectW / aspectH, zNear );
    update();
}

HGE_INLINE void c_camera::setProjection( float inFov, float aspectWidth, float aspectHeight, float inZNear, float inZFar ) {
	fov = inFov;
	aspectW = aspectWidth;
	aspectH = aspectHeight;
	zNear = inZNear;
	zFar = inZFar;
}

//-----------------------------------------------------------------------------
//	Camera - Rotations
//-----------------------------------------------------------------------------
HGE_INLINE void c_camera::rotate( float inPitch, float inYaw, float inRoll ) {
	// Pitch, Yaw, and Roll are in Radians. They are reversed due to translation
	// from world-space into eye-space
	// No checks are performed to see if the values are less than 0 or greater
	// than 2*pi since camera orientations are based on a per-update basis
	// Rotation values are reset to 0 after "update()" is run.
	pitch = -inPitch;
	yaw = -inYaw;
	roll = -inRoll;
}

} // end hge namespace

#endif /* __HGE_CAMERA_H__ */
