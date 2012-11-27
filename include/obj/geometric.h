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
	float xy[ 2 ];
	float &x;
	float &y;
	HL_FASTCALL s_vertex2d() :
		xy{ 0.f, 0.f },
		x( xy[0] ), y( xy[1] )
	{}
};

struct s_vertex3d {
	float xyz[ 3 ];
	float &x;
	float &y;
	float &z;
	HL_FASTCALL s_vertex3d() :
		xyz{ 0.f, 0.f, 0.f },
		x( xyz[0] ), y( xyz[1] ), z( xyz[2] )
	{}
};

struct s_vertex4d {
	float xyzw[ 4 ];
	float &x;
	float &y;
	float &z;
	float &w;
	HL_FASTCALL s_vertex4d() :
		xyzw{ 0.f, 0.f, 0.f, 0.f },
		x( xyzw[0] ), y( xyzw[1] ), z( xyzw[2] ), w( xyzw[3] )
	{}
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
