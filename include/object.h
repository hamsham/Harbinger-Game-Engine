/* 
 * File:   object.h
 * Author: hammy
 *
 * Created on December 4, 2012, 6:48 PM
 */

#ifndef __HGE_OBJECT_H__
#define	__HGE_OBJECT_H__

#include "types.h"

namespace hge {

//-----------------------------------------------------------------------------
//	Basic Object Abstraction
//-----------------------------------------------------------------------------
class HGE_API c_object {
	protected:
		vec3 pos;
		
	public:
		c_object() :
			pos( 0.f, 0.f, 0.f )
		{}
			
		c_object( const c_object& obj ) :
			pos( obj.pos )
		{}
		
		virtual ~c_object	() = 0;
		
		const vec3&		getPos	() const				{ return pos; }
		void			setPos	( const vec3& inPos )	{ pos = inPos; }
		virtual void	move	( const vec3& amount )	{ pos += amount; }
		
		virtual void	update	() = 0;
		virtual void	tick	( float timeElapsed ) = 0;
};

//-----------------------------------------------------------------------------
//	Drawable Object Abstraction
//-----------------------------------------------------------------------------
class HGE_API c_drawableObj : virtual public c_object {
	protected:
		vec3 scale;
		quat rot;
		mat4 modelMat;
		
	public:
		c_drawableObj() :
			c_object(),
			scale( 1.f, 1.f, 1.f ),
			rot( 0.f, 0.f, 0.f, 1.f ),
			modelMat( 1.f )
		{}
			
		c_drawableObj( const c_drawableObj& obj ) :
			c_object( obj ),
			scale( obj.scale ),
			rot( obj.rot ),
			modelMat( obj.modelMat )
		{}
		
		virtual ~c_drawableObj() = 0;
		
		const vec3&		getScale	() const				{ return scale; }
		const quat&		getRotation () const				{ return rot; }
		const mat4&		getModelMatrix	() const			{ return modelMat; }
		
		void			setScale	( const vec3& s )		{ scale = s; }
		void			setRotation	( const vec3& angles );
		void			rotate		( const vec3& angles );
		
		virtual void	update		();
		virtual void	tick		( float )				{ update(); }
		virtual void	draw		() const= 0;
};

} // end hge namespace

#endif	/* __HGE_OBJECT_H__ */
