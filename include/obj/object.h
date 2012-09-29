/* 
 * File:   object.h
 * Author: hammy
 *
 * Created on September 9, 2012, 4:57 PM
 */

#ifndef	__HGE_OBJECT_H__
#define	__HGE_OBJECT_H__
namespace harbinger {

class c_entity {
	private:
		int		flags;
		
	public:
		c_entity	() : flags(0) {};
		c_entity	( const c_entity& entityCopy ) : flags( entityCopy.flags ) {};
		virtual ~c_entity () = 0;
};

class c_object : public c_entity {
	private:
		glm::vec3 pos;

	public:
		enum e_objectFlags {
			COLLIDABLE,
			VISIBLE,
			ANIMATED,
			DYNAMIC
		};
		
		c_object() : pos(0,0,0) {}
		c_object(const c_object& orig) : c_entity(orig), pos(orig.pos) {}
		~c_object() {}
};

} // end harbinger namespace
#endif	/* __HGE_OBJECT_H__ */
