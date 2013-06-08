/* 
 * File:   newClass.h
 * Author: hammy
 *
 * Created on June 7, 2013, 5:51 PM
 */

#ifndef __HGE_TRANSFORMATIONS_H__
#define	__HGE_TRANSFORMATIONS_H__

#include "types.h"

namespace hge {

//-----------------------------------------------------------------------------
//	Draw Transformations
//-----------------------------------------------------------------------------
class HGE_API c_drawTransform {
    
	protected:
		vec3    pos;
		vec3    scaling;
		quat    rot;
		mat4    modelMat;
		
	public:
		c_drawTransform     ();
		c_drawTransform     ( const c_drawTransform& obj );
		~c_drawTransform    () {}
        
        c_drawTransform&    operator =      ( const c_drawTransform& );
		
		const vec3&         getPos          () const				{ return pos; }
		const vec3&         getScale        () const				{ return scaling; }
		const quat&         getRotation     () const				{ return rot; }
		const mat4&         getModelMatrix	() const                { return modelMat; }
		
		void                setPos          ( const vec3& p );
		void                setScale        ( const vec3& s );
		void                setRotation     ( const vec3& angles );
        
        void                move            ( const vec3& p );
        void                scale           ( const vec3& s );
		void                rotate          ( const vec3& angles );
		
		void                update          ();
};

} // hge namespace

#endif	/* __HGE_TRANSFORMATIONS_H__ */
