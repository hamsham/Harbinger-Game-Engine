/* 
 * File:   script_variables.h
 * Author: hammy
 *
 * Created on September 25, 2012, 5:21 PM
 */

#ifndef __HGE_SCRIPT_VARIABLES_H__
#define	__HGE_SCRIPT_VARIABLES_H__

#include "math/math.h"
using namespace hamLibs;

#include "script.h"
#include "script_base.h"

//-----------------------------------------------------------------------------
//		Integer Variable
//-----------------------------------------------------------------------------
class HGE_API c_scriptInt :	virtual public c_scriptVar<int>,
				virtual public c_scriptNum {
	friend class c_serialize;
	
	public:
		c_scriptInt();
		c_scriptInt( int integer );
		c_scriptInt( const c_scriptNum& inVar );
		~c_scriptInt();
		
		c_scriptNum operator + ( const c_scriptNum& inVar ) const;
		c_scriptNum operator - ( const c_scriptNum& inVar ) const;
		c_scriptNum operator * ( const c_scriptNum& inVar ) const;
		c_scriptNum operator / ( const c_scriptNum& inVar ) const;
		c_scriptNum operator % ( const c_scriptNum& inVar ) const;
		
		c_scriptNum& operator = ( const c_scriptNum& inVar );
		c_scriptNum& operator += ( const c_scriptNum& inVar );
		c_scriptNum& operator -= ( const c_scriptNum& inVar );
		c_scriptNum& operator *= ( const c_scriptNum& inVar );
		c_scriptNum& operator /= ( const c_scriptNum& inVar );
		c_scriptNum& operator %= ( const c_scriptNum& inVar );
		
		bool operator == ( const c_scriptNum& inVar ) const;
		bool operator != ( const c_scriptNum& inVar ) const;
		bool operator > ( const c_scriptNum& inVar ) const;
		bool operator < ( const c_scriptNum& inVar ) const;
		bool operator >= ( const c_scriptNum& inVar ) const;
		bool operator <= ( const c_scriptNum& inVar ) const;
		
		c_scriptNum& operator = ( int inVar );
		c_scriptNum& operator = ( unsigned int inVar );
		c_scriptNum& operator = ( float inVar );
		
		operator int() const;
		operator unsigned int() const;
		operator float() const;

		operator c_scriptUint() const;
		operator c_scriptFloat() const;
		
		int getScriptSubType() const {
			return SCRIPT_VAR_INT;
		}
};

//-----------------------------------------------------------------------------
//		Floating Point Variable
//-----------------------------------------------------------------------------
class HGE_API c_scriptFloat :	virtual public c_scriptVar<float>,
					virtual public c_scriptNum {
	friend class c_serialize;
	
	public:
		c_scriptFloat();
		c_scriptFloat( float fpVal );
		c_scriptFloat( const c_scriptNum& inVar );
		~c_scriptFloat();
		
		c_scriptNum operator + ( const c_scriptNum& inVar ) const;
		c_scriptNum operator - ( const c_scriptNum& inVar ) const;
		c_scriptNum operator * ( const c_scriptNum& inVar ) const;
		c_scriptNum operator / ( const c_scriptNum& inVar ) const;
		c_scriptNum operator % ( const c_scriptNum& inVar ) const;
		
		c_scriptNum& operator = ( const c_scriptNum& inVar );
		c_scriptNum& operator += ( const c_scriptNum& inVar );
		c_scriptNum& operator -= ( const c_scriptNum& inVar );
		c_scriptNum& operator *= ( const c_scriptNum& inVar );
		c_scriptNum& operator /= ( const c_scriptNum& inVar );
		c_scriptNum& operator %= ( const c_scriptNum& inVar );
		
		bool operator == ( const c_scriptNum& inVar ) const;
		bool operator != ( const c_scriptNum& inVar ) const;
		bool operator > ( const c_scriptNum& inVar ) const;
		bool operator < ( const c_scriptNum& inVar ) const;
		bool operator >= ( const c_scriptNum& inVar ) const;
		bool operator <= ( const c_scriptNum& inVar ) const;
		
		c_scriptNum& operator = ( int inVar );
		c_scriptNum& operator = ( unsigned int inVar );
		c_scriptNum& operator = ( float inVar );
		
		operator int() const;
		operator unsigned int() const;
		operator float() const;

		operator c_scriptInt() const;
		operator c_scriptUint() const;
		
		int getScriptSubType() const {
			return SCRIPT_VAR_FLOAT;
		}
};

//-----------------------------------------------------------------------------
//		Unsigned Integer Variable
//-----------------------------------------------------------------------------
class HGE_API c_scriptUint :	virtual public c_scriptVar<unsigned int>,
					virtual public c_scriptNum {
	friend class c_serialize;
	
	public:
		c_scriptUint();
		c_scriptUint( unsigned int integer );
		c_scriptUint( const c_scriptNum& inVar );
		~c_scriptUint();
		
		c_scriptNum operator + ( const c_scriptNum& inVar ) const;
		c_scriptNum operator - ( const c_scriptNum& inVar ) const;
		c_scriptNum operator * ( const c_scriptNum& inVar ) const;
		c_scriptNum operator / ( const c_scriptNum& inVar ) const;
		c_scriptNum operator % ( const c_scriptNum& inVar ) const;
		
		c_scriptNum& operator = ( const c_scriptNum& inVar );
		c_scriptNum& operator += ( const c_scriptNum& inVar );
		c_scriptNum& operator -= ( const c_scriptNum& inVar );
		c_scriptNum& operator *= ( const c_scriptNum& inVar );
		c_scriptNum& operator /= ( const c_scriptNum& inVar );
		c_scriptNum& operator %= ( const c_scriptNum& inVar );
		
		bool operator == ( const c_scriptNum& inVar ) const;
		bool operator != ( const c_scriptNum& inVar ) const;
		bool operator > ( const c_scriptNum& inVar ) const;
		bool operator < ( const c_scriptNum& inVar ) const;
		bool operator >= ( const c_scriptNum& inVar ) const;
		bool operator <= ( const c_scriptNum& inVar ) const;
		
		c_scriptNum& operator = ( int inVar );
		c_scriptNum& operator = ( unsigned int inVar );
		c_scriptNum& operator = ( float inVar );
		
		operator int() const;
		operator unsigned int() const;
		operator float() const;

		operator c_scriptInt() const;
		operator c_scriptFloat() const;
		
		int getScriptSubType() const {
			return SCRIPT_VAR_UINT;
		}
};

//-----------------------------------------------------------------------------
//		Boolean Variable
//-----------------------------------------------------------------------------
class HGE_API c_scriptBool : virtual public c_scriptVar< bool > {
	friend class c_serialize;
	
	public:
		c_scriptBool();
		c_scriptBool( bool inBool );
		c_scriptBool( const c_scriptBool& inBool );
		~c_scriptBool();
		
		int getScriptSubType() {
			return SCRIPT_VAR_BOOL;
		}
		
		void setTrue();
		void setFalse();
};

//-----------------------------------------------------------------------------
//		Character String Variable
//-----------------------------------------------------------------------------
class HGE_API c_scriptString : virtual public c_scriptVar< std::string > {
	friend class c_serialize;
	
	public:
		c_scriptString();
		c_scriptString( const char* strCopy );
		c_scriptString( const std::string& strCopy );
		c_scriptString( const c_scriptString& strCopy );
		~c_scriptString();
		
		char operator[] ( hgeSize_t index ) const;
		char& operator[] ( hgeSize_t index );
		
		int getScriptSubType() const {
			return SCRIPT_VAR_STRING;
		}
		
		void read ( std::ifstream&, scriptMap_t& );
		void write ( std::ofstream& ) const;
};

//-----------------------------------------------------------------------------
//		3D Vector Variable
//-----------------------------------------------------------------------------
class HGE_API c_scriptVec3 : virtual public c_scriptVar< math::vec3 > {
	friend class c_serialize;
	
	public:
		c_scriptVec3();
		c_scriptVec3( const c_scriptVec3& vecCopy );
		c_scriptVec3( const math::vec3& vecCopy );
		~c_scriptVec3();
		
		float operator[] ( int ) const;
		float& operator[] ( int );
		
		int getScriptSubType() const {
			return SCRIPT_VAR_VEC3;
		}
};

#endif	/* __HGE_SCRIPT_VARIABLES_H__ */
