/* 
 * File:   geometric.h
 * Author: hammy
 *
 * Created on November 13, 2012, 9:59 PM
 */

#ifndef __HGE_GEOMETRIC_H__
#define	__HGE_GEOMETRIC_H__
namespace harbinger {

struct s_vertex2d {
	union {
		struct {
			float x;
			float y;
		};
		float xy[ 2 ];
	};
	HL_FASTCALL s_vertex2d() : x( 0.f ), y( 0.f ) {}
};

struct s_vertex3d {
	union {
		struct {
			float x;
			float y;
			float z;
		};
		float xyz[ 3 ];
	};
	HL_FASTCALL s_vertex3d() : x( 0.f ), y( 0.f ), z( 0.f ) {}
};

struct s_vertex4d {
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		float xyzw[ 4 ];
	};
	HL_FASTCALL s_vertex4d() : x( 0.f ), y( 0.f ), z( 0.f ), w( 1.f ) {}
};

template < unsigned int numVerts >
struct s_polygon {
	s_vertex3d vert[ numVerts ];
};

class c_mesh {
	private:
		
	protected:
		s_tri* polys = NULL;
		
	public:
		c_mesh();
		c_mesh( const c_mesh& meshCopy );
		~c_mesh();
		
		bool loadMeshResource( const char* fileName );
		void unloadMeshResource();
};

} // end harbinger namespace
#endif	/* __HGE_GEOMETRIC_H__ */
