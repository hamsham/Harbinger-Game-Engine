/* 
 * File:   object.h
 * Author: hammy
 *
 * Created on December 4, 2012, 6:48 PM
 */

#ifndef __HGE_OBJECT_H__
#define	__HGE_OBJECT_H__

#include "types.h"
#include "math/math.h"
using namespace hamLibs;

//-----------------------------------------------------------------------------
//	Basic Object Abstraction
//-----------------------------------------------------------------------------
class HGE_API c_object {
	public:
		static const math::vec3 WORLD_AXIS_X;
		static const math::vec3 WORLD_AXIS_Y;
		static const math::vec3 WORLD_AXIS_Z;
		static const math::mat4 WORLD_MATRIX;
		
	protected:
		math::vec3 pos;
		math::vec3 deltaPos;
		
	public:
		c_object() :
			pos( 0.f, 0.f, 0.f )
		{}
			
		c_object( const c_object& obj ) :
			pos( obj.pos )
		{}
		
		virtual ~c_object	() = 0;
		
		const math::vec3&	getPos	() const						{ return pos; }
		void				setPos	( const math::vec3& inPos )		{ pos = inPos; }
		void				move	( const math::vec3& amount )	{ deltaPos = amount; }
		
		virtual void		update	() = 0;
		virtual void		tick	( float timeElapsed ) = 0;
};

//-----------------------------------------------------------------------------
//	Drawable Object Abstraction
//-----------------------------------------------------------------------------
class HGE_API c_drawableObj : virtual public c_object {
	protected:
		math::vec3 scale;
		math::quat rot;
		math::mat4 worldMat;
		
	public:
		c_drawableObj() :
			c_object(),
			scale( 1.f, 1.f, 1.f ),
			rot( 0.f, 0.f, 0.f, 1.f ),
			worldMat( 1.f )
		{}
			
		c_drawableObj( const c_drawableObj& obj ) :
			c_object( obj ),
			scale( obj.scale ),
			rot( obj.rot ),
			worldMat( obj.worldMat )
		{}
		
		virtual ~c_drawableObj() = 0;
		
		const math::vec3&	getScale	() const				{ return scale; }
		const math::quat&	getRotation () const				{ return rot; }
		const math::mat4&	getMatrix	() const				{ return worldMat; }
		
		void				setScale	( const math::vec3& s )	{ scale = s; }
		void				setRotation	( const math::vec3& angles );
		void				rotate		( const math::vec3& angles );
		
		virtual void		update		();
		virtual void		tick		( float ) { update(); }
		virtual void		draw		() const= 0;
};

#endif	/* __HGE_OBJECT_H__ */
