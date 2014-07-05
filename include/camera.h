/* 
 * File:   camera.h
 * Author: hammy
 *
 * Created on December 7, 2012, 6:23 PM
 */

#ifndef __HGE_CAMERA_H__
#define	__HGE_CAMERA_H__

#include "types.h"

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

class HGE_API camera {
    
    public:
        enum e_viewMode : unsigned {
            VIEW_NORMAL = 0,
            VIEW_ORBIT  = 1
        };
        
		static const float	DEFAULT_ASPECT_WIDTH;
		static const float	DEFAULT_ASPECT_HEIGHT;
		static const float	DEFAULT_Z_NEAR;
		static const float	DEFAULT_Z_FAR;
		static const float	DEFAULT_FOV;
		static const mat4   DEFAULT_PERSPECTIVE;
    
    private:
		//function pointer arrays for movement and rotations
		void ( camera::*rotateFunction[ 2 ] )( const vec3& );
		void ( camera::*updateFunction[ 2 ] )();
        
        e_viewMode viewMode = VIEW_NORMAL;
        float fov       = 45.f;
        float aspectW   = 4.f;
        float aspectH   = 3.f;
        float zNear     = 0.1f;
        float zFar      = 100.f;
        
        float orbitDist = 1.f;
        
        vec3 pos;
        vec3 target;
        vec3 xAxis;
        vec3 yAxis;
        vec3 zAxis;
        vec3 angles;
        
        quat orientation;
        mat4 viewMatrix;
        mat4 projMatrix;
        
        void        rotateUnlockedY( const vec3& amount );
        void        rotateLockedY( const vec3& amount );
        void        rotateOrbitUnlockedY( const vec3& amount );
        void        rotateOrbitLockedY( const vec3& amount );
        void        updateNormal();
        void        updateOrbit();
        
    public:
        camera      ();
        camera      ( const camera& );
        camera      ( camera&& ) = default;
        ~camera     () {}
        
        camera&     operator =      ( const camera& );
        camera&     operator =      ( camera&& )            = default;
        
        void        makeOrtho       ();
        void        makePerspective ();
        void        setProjectionParams(
                                    float inFov             = DEFAULT_FOV,
                                    float aspectWidth       = DEFAULT_ASPECT_WIDTH,
                                    float aspectHeight      = DEFAULT_ASPECT_HEIGHT,
                                    float near              = DEFAULT_Z_NEAR,
                                    float far               = DEFAULT_Z_FAR
                    );
        
        float       getOrbitDist    () const                { return orbitDist; }
        void        setOrbitDist    ( float d )             { orbitDist = std::fabs(d); }
        
		const vec3& getPos          () const				{ return pos; }
		void        setPos          ( const vec3& p )       { pos = p; }
        const vec3& getDir          () const                { return zAxis; }
        void        setDir          ( const vec3& d )       { look( d ); }
        const vec3& getTarget       () const                { return target; }
        void        setTarget       ( const vec3& t )       { target = t; }
        
        const vec3& getUp           () const                { return yAxis; }
        void        setUp           ( const vec3& up )      { yAxis = up; }
        const mat4& getViewMatrix   () const                { return viewMatrix; }
        const mat4& getProjMatrix   () const                { return projMatrix; }
        mat4        getVPMatrix     () const                { return projMatrix*viewMatrix; }
        
        const quat& getOrientation  () const                { return orientation; }
        void        setOrientation  ( const quat& o )       { orientation = o; }
        
        void        setAspect       ( float w, float h )    { aspectW = w; aspectH = h; }
        float       getAspect       () const                { return aspectW / aspectH; }
        void        setZNear        ( float inZNear )       { zNear = inZNear; }
        float       getZNear        () const                { return zNear; }
        void        setZFar         ( float inZFar )        { zFar = inZFar; }
        float       getZFar         () const                { return zFar; }
        
        void        setViewMode     ( e_viewMode m )        { viewMode = m; }
        e_viewMode  getViewMode     () const                { return viewMode; }
        
        void        lockYAxis       ( bool );
        
        void        look            ( const vec3& eye, const vec3& point, const vec3& up );
        void        look            ( const vec3& point );
        void        move            ( const vec3& amount );
        void        rotate          ( const vec3& amount );
        void        unroll          ();
        
        void        update          ();
};

} // end hge namespace

#endif /* __HGE_CAMERA_H__ */
