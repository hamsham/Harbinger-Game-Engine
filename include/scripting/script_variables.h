/* 
 * File:   script_variables.h
 * Author: hammy
 *
 * Created on September 25, 2012, 5:21 PM
 */

#ifndef SCRIPT_VARIABLES_H
#define	SCRIPT_VARIABLES_H

namespace harbinger {

//-----------------------------------------------------------------------------
//		Integer Variable
//-----------------------------------------------------------------------------
class c_scriptInt :	virtual public c_scriptVar<int>,
				virtual public c_scriptNum {
	public:
		c_scriptInt();
		c_scriptInt( int integer );
		c_scriptInt( const c_scriptNum& inVar );
		~c_scriptInt();
		
		c_scriptNum operator + ( const c_scriptNum& inVar );
		c_scriptNum operator - ( const c_scriptNum& inVar );
		c_scriptNum operator * ( const c_scriptNum& inVar );
		c_scriptNum operator / ( const c_scriptNum& inVar );
		c_scriptNum operator % ( const c_scriptNum& inVar );
		
		c_scriptNum& operator = ( const c_scriptNum& inVar );
		c_scriptNum& operator += ( const c_scriptNum& inVar );
		c_scriptNum& operator -= ( const c_scriptNum& inVar );
		c_scriptNum& operator *= ( const c_scriptNum& inVar );
		c_scriptNum& operator /= ( const c_scriptNum& inVar );
		c_scriptNum& operator %= ( const c_scriptNum& inVar );
		
		bool operator == ( const c_scriptNum& inVar );
		bool operator != ( const c_scriptNum& inVar );
		bool operator > ( const c_scriptNum& inVar );
		bool operator < ( const c_scriptNum& inVar );
		bool operator >= ( const c_scriptNum& inVar );
		bool operator <= ( const c_scriptNum& inVar );
		
		c_scriptNum& operator = ( int inVar );
		c_scriptNum& operator = ( unsigned int inVar );
		c_scriptNum& operator = ( float inVar );
		
		operator int() const;
		operator unsigned int() const;
		operator float() const;

		operator c_scriptUint() const;
		operator c_scriptFloat() const;
};

//-----------------------------------------------------------------------------
//		Floating Point Variable
//-----------------------------------------------------------------------------
class c_scriptFloat :	virtual public c_scriptVar<float>,
					virtual public c_scriptNum {
	public:
		c_scriptFloat();
		c_scriptFloat( float fpVal );
		c_scriptFloat( const c_scriptNum& inVar );
		~c_scriptFloat();
		
		c_scriptNum operator + ( const c_scriptNum& inVar );
		c_scriptNum operator - ( const c_scriptNum& inVar );
		c_scriptNum operator * ( const c_scriptNum& inVar );
		c_scriptNum operator / ( const c_scriptNum& inVar );
		c_scriptNum operator % ( const c_scriptNum& inVar );
		
		c_scriptNum& operator = ( const c_scriptNum& inVar );
		c_scriptNum& operator += ( const c_scriptNum& inVar );
		c_scriptNum& operator -= ( const c_scriptNum& inVar );
		c_scriptNum& operator *= ( const c_scriptNum& inVar );
		c_scriptNum& operator /= ( const c_scriptNum& inVar );
		c_scriptNum& operator %= ( const c_scriptNum& inVar );
		
		bool operator == ( const c_scriptNum& inVar );
		bool operator != ( const c_scriptNum& inVar );
		bool operator > ( const c_scriptNum& inVar );
		bool operator < ( const c_scriptNum& inVar );
		bool operator >= ( const c_scriptNum& inVar );
		bool operator <= ( const c_scriptNum& inVar );
		
		c_scriptNum& operator = ( int inVar );
		c_scriptNum& operator = ( unsigned int inVar );
		c_scriptNum& operator = ( float inVar );
		
		operator int() const;
		operator unsigned int() const;
		operator float() const;

		operator c_scriptInt() const;
		operator c_scriptUint() const;
};

//-----------------------------------------------------------------------------
//		Unsigned Integer Variable
//-----------------------------------------------------------------------------
class c_scriptUint :	virtual public c_scriptVar<unsigned int>,
					virtual public c_scriptNum {
	public:
		c_scriptUint();
		c_scriptUint( unsigned int integer );
		c_scriptUint( const c_scriptNum& inVar );
		~c_scriptUint();
		
		c_scriptNum operator + ( const c_scriptNum& inVar );
		c_scriptNum operator - ( const c_scriptNum& inVar );
		c_scriptNum operator * ( const c_scriptNum& inVar );
		c_scriptNum operator / ( const c_scriptNum& inVar );
		c_scriptNum operator % ( const c_scriptNum& inVar );
		
		c_scriptNum& operator = ( const c_scriptNum& inVar );
		c_scriptNum& operator += ( const c_scriptNum& inVar );
		c_scriptNum& operator -= ( const c_scriptNum& inVar );
		c_scriptNum& operator *= ( const c_scriptNum& inVar );
		c_scriptNum& operator /= ( const c_scriptNum& inVar );
		c_scriptNum& operator %= ( const c_scriptNum& inVar );
		
		bool operator == ( const c_scriptNum& inVar );
		bool operator != ( const c_scriptNum& inVar );
		bool operator > ( const c_scriptNum& inVar );
		bool operator < ( const c_scriptNum& inVar );
		bool operator >= ( const c_scriptNum& inVar );
		bool operator <= ( const c_scriptNum& inVar );
		
		c_scriptNum& operator = ( int inVar );
		c_scriptNum& operator = ( unsigned int inVar );
		c_scriptNum& operator = ( float inVar );
		
		operator int() const;
		operator unsigned int() const;
		operator float() const;

		operator c_scriptInt() const;
		operator c_scriptFloat() const;
};

} // end harbinger namespace
#endif	/* SCRIPT_VARIABLES_H */
