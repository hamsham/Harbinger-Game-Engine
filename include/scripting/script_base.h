/* 
 * File:   script_base.h
 * Author: hammy
 *
 * Created on September 25, 2012, 5:18 PM
 */

#ifndef __HGE_SCRIPT_BASE_H__
#define	__HGE_SCRIPT_BASE_H__

#include <fstream>

#include "math/math.h"
using namespace hamLibs;

#include "../types.h"

//-----------------------------------------------------------------------------
//		Script Base Class
//-----------------------------------------------------------------------------
class HGE_API c_script {
	friend class c_serializer;

	public:
		virtual ~c_script	() = 0;

		virtual c_script&	operator=			( const c_script& scriptCopy );
		virtual bool		operator==			( const c_script& scriptCopy ) const;
		virtual bool		operator!=			( const c_script& scriptCopy ) const;

		virtual int			getScriptType		() const { return SCRIPT_BASE; }
		virtual int			getScriptSubType	() const { return SCRIPT_INVALID; }

		virtual void		read				( std::ifstream&, scriptMap_t& );
		virtual void		write				( std::ofstream& ) const;
};

//-----------------------------------------------------------------------------
//		Variable Base Class
//		All variables MUST support copy construction & assignment operators
//-----------------------------------------------------------------------------
//ADT used for separation of variables and functions
class HGE_API c_scriptVarBase : virtual public c_script {
	friend class c_serializer;
	friend class c_scriptFuncBase;

	public:
		virtual ~c_scriptVarBase		() = 0;

		int			getScriptType		() const { return SCRIPT_VAR; }
		virtual int	getScriptSubType	() const { return SCRIPT_INVALID; }

		virtual c_scriptVarBase& operator =	( const c_scriptVarBase& varCopy );
		virtual bool	operator==		( const c_scriptVarBase& scriptCopy ) const;
		virtual bool	operator!=		( const c_scriptVarBase& scriptCopy ) const;
};

//-----------------------------------------------------------------------------
//		Variable Interface Classe
//		Abstract
//		All variables MUST support copy construction & assignment operators
//-----------------------------------------------------------------------------
template <typename type>
class HGE_API c_scriptVar : virtual public c_scriptVarBase {
	friend class c_serializer;

	protected:
		type variable;

	public:
		c_scriptVar();
		c_scriptVar( const type& varCopy);
		c_scriptVar( const c_scriptVar& varCopy);
		virtual ~c_scriptVar() = 0;

		virtual type& varData();
		virtual type varData() const;

		virtual operator type() const;
		c_scriptVar& operator = ( const type& varCopy );
		virtual c_scriptVar& operator = ( const c_scriptVar& varCopy );
		virtual bool operator== ( const c_scriptVar& scriptCopy ) const;
		virtual bool operator!= ( const c_scriptVar& scriptCopy ) const;

		virtual void read ( std::ifstream&, scriptMap_t& );
		virtual void write ( std::ofstream& ) const;
};

template <typename type>
c_scriptVar<type>::c_scriptVar() {}

template <typename type>
c_scriptVar<type>::c_scriptVar( const type& varCopy ) :
	variable( varCopy )
{}

template <typename type>
c_scriptVar<type>::c_scriptVar( const c_scriptVar& varCopy ) :
	variable( varCopy.variable )
{}

template <typename type>
c_scriptVar<type>::~c_scriptVar() {}

template <typename type>
type c_scriptVar<type>::varData() const {
	return variable;
}

template <typename type>
type& c_scriptVar<type>::varData() {
	return variable;
}

template <typename type>
c_scriptVar<type>::operator type() const {
	return variable;
}

template <typename type>
c_scriptVar<type>& c_scriptVar<type>::operator= ( const type& inVar ) {
	variable = inVar;
	return *this;
}

template <typename type>
c_scriptVar<type>& c_scriptVar<type>::operator= ( const c_scriptVar& varCopy ) {
	variable = varCopy.variable;
	return *this;
}

template <typename type>
bool c_scriptVar<type>::operator== ( const c_scriptVar& varCopy ) const {
	return ( variable == varCopy.variable );
}

template <typename type>
bool c_scriptVar<type>::operator!= ( const c_scriptVar& varCopy ) const {
	return ( variable != varCopy.variable );
}

template <typename type>
void c_scriptVar<type>::read( std::ifstream& fin, scriptMap_t& scrMap ) {
	c_script::read( fin, scrMap );
	fin >> variable;
}

template <>
void c_scriptVar< math::vec3 >::read( std::ifstream&, scriptMap_t& );

template <typename type>
void c_scriptVar<type>::write( std::ofstream& fout ) const {
	c_script::write( fout );
	fout << " " << variable;
}

template <>
void c_scriptVar< math::vec3 >::write( std::ofstream& ) const;

//-----------------------------------------------------------------------------
//		Numerical Variable Base Class
//-----------------------------------------------------------------------------
class HGE_API c_scriptNum : virtual public c_scriptVarBase {
	friend class c_serializer;

	public:
		~c_scriptNum() {}

		virtual c_scriptNum operator + ( const c_scriptNum& inVar ) const;
		virtual c_scriptNum operator - ( const c_scriptNum& inVar ) const;
		virtual c_scriptNum operator * ( const c_scriptNum& inVar ) const;
		virtual c_scriptNum operator / ( const c_scriptNum& inVar ) const;
		virtual c_scriptNum operator % ( const c_scriptNum& inVar ) const;

		virtual c_scriptNum& operator = ( const c_scriptNum& inVar );
		virtual c_scriptNum& operator += ( const c_scriptNum& inVar );
		virtual c_scriptNum& operator -= ( const c_scriptNum& inVar );
		virtual c_scriptNum& operator *= ( const c_scriptNum& inVar );
		virtual c_scriptNum& operator /= ( const c_scriptNum& inVar );
		virtual c_scriptNum& operator %= ( const c_scriptNum& inVar );

		virtual bool operator == ( const c_scriptNum& inVar ) const;
		virtual bool operator != ( const c_scriptNum& inVar ) const;
		virtual bool operator > ( const c_scriptNum& inVar ) const;
		virtual bool operator < ( const c_scriptNum& inVar ) const;
		virtual bool operator >= ( const c_scriptNum& inVar ) const;
		virtual bool operator <= ( const c_scriptNum& inVar ) const;

		virtual c_scriptNum& operator = ( int inVar );
		virtual c_scriptNum& operator = ( unsigned int inVar );
		virtual c_scriptNum& operator = ( float inVar );

		//these will be used in derived classes, in conjunction with the
		//c_scriptNum assignment operator, in order to allow for siblings
		//to equal each other at run-time (see script_variables.h & 
		//c_script_variables.cpp for an example)
		virtual operator int() const;
		virtual operator unsigned int() const;
		virtual operator float() const;

		virtual e_scriptVarType getScriptSubType() {
			return SCRIPT_VAR_NUM;
		}
};

//-----------------------------------------------------------------------------
//		Script Resource Variables
//	Any type of object which must be saved or loaded from a file
//-----------------------------------------------------------------------------
template <typename type>
class HGE_API c_scriptResource : virtual public c_scriptVar< type > {
	friend class c_serializer;

	enum e_resourceFlags : signed int {
		RESOURCE_LOADED = 0x1,
		RESOURCE_MUTABLE = 0x2, // the resource can be edited
		RESOURCE_COPYABLE = 0x4
	};

	private:
		std::string resourceFile;
		unsigned int numReferences = 0;

	protected:
		e_resourceFlags resourceFlags;

	public:
		c_scriptResource() {}
		c_scriptResource( const c_scriptResource< type >& resCopy );
		virtual ~c_scriptResource() = 0;

		virtual e_scriptVarType getScriptSubType() {
			return SCRIPT_VAR_RESOURCE;
		}

		//file handling
		const char* getResourceFile() const;
		void setResourceFile( const char* resFile );
		virtual bool saveResource( const char* fileName ) = 0;
		virtual bool loadResource( const char* fileName ) = 0;
		virtual void unloadResource() = 0;

		// resource management
		virtual c_scriptResource& operator = ( const c_scriptResource< type >& resCopy ) = 0;
		virtual const c_scriptResource* getCopy() = 0; // much different than the above
		const type& getReference() const;
		void detachReference( type&  );

		e_resourceFlags getResourceFlags() const;
		void setResourceFlags( e_resourceFlags );
};

template <typename type>
c_scriptResource< type >::c_scriptResource( const c_scriptResource< type >& resCopy ) :
	c_script( resCopy ),
	c_scriptVarBase( resCopy ),
	c_scriptVar< type >( resCopy )
{
	*this = this->operator=( resCopy );
}

template <typename type>
c_scriptResource< type >::~c_scriptResource() {
	unloadResource();
}

template <typename type>
const type& c_scriptResource< type >::getReference() const {
	return this->variable;
}

//-----------------------------------------------------------------------------
//		Function Base Classes
//		Abstract
//-----------------------------------------------------------------------------
class HGE_API c_scriptFuncBase : virtual public c_script {
	friend class c_serializer;

	protected:
		const c_scriptFuncBase* nextFunc;

	public:
		c_scriptFuncBase();
		c_scriptFuncBase( const c_scriptFuncBase& funcCopy );
		virtual ~c_scriptFuncBase() = 0;

		virtual void run() = 0;
		virtual void tick( float timeElapsed = 0 ) {
			HGE_ASSERT( timeElapsed == 0);
			run();
		}

		int getScriptType() const { return SCRIPT_FUNC; }
		virtual int getScriptSubType() const { return SCRIPT_INVALID; }

		virtual void read ( std::ifstream&, scriptMap_t& );
		virtual void write ( std::ofstream& ) const;

		const c_scriptFuncBase* getNextFunction() const;
		void setNextFunction( const c_scriptFuncBase& next );
};

//-----------------------------------------------------------------------------
//		Function Class Interface
//		Abstract
//-----------------------------------------------------------------------------
template <typename returnType>
class HGE_API c_scriptFunc : virtual public c_scriptFuncBase {
	friend class c_serializer;

	protected:
		returnType returnVal;

	public:
		c_scriptFunc();
		c_scriptFunc( const c_scriptFunc& funcCopy );
		c_scriptFunc( const returnType& retVal );
		virtual ~c_scriptFunc() = 0;

		virtual void read ( std::ifstream&, scriptMap_t& ) = 0;
		virtual void write ( std::ofstream& ) const = 0;

		const returnType& getReturnVal() const;
		void setReturnVal( const returnType& retVal );
};

template <typename returnType>
c_scriptFunc< returnType >::c_scriptFunc() :
	returnVal()
{}

template <typename returnType>
c_scriptFunc< returnType >::c_scriptFunc( const c_scriptFunc& funcCopy ) :
	c_scriptFuncBase( funcCopy ),
	returnVal( funcCopy.returnVal)
{}

template <typename returnType>
c_scriptFunc< returnType >::~c_scriptFunc() {}

template <typename returnType>
c_scriptFunc< returnType >::c_scriptFunc( const returnType& retVal ) :
	returnVal( retVal )
{}

template <typename returnType>
const returnType& c_scriptFunc< returnType >::getReturnVal() const {
	return returnVal;
}

template <typename returnType>
void c_scriptFunc< returnType >::setReturnVal( const returnType& retVal ) {
	returnVal = retVal;
}

template <typename returnType>
void c_scriptFunc< returnType >::read( std::ifstream& fin, scriptMap_t& scrMap ) {
	c_scriptFuncBase::read( fin, scrMap );
	fin >> returnVal;
}

template <typename returnType>
void c_scriptFunc< returnType >::write( std::ofstream& fout ) const {
	c_scriptFuncBase::write( fout );
	fout << " " << returnVal;
}

/* NOTE:
 * The s_scriptEval and c_scriptNumeric functions have been moved to the
 * "script_functions.h" header due to compiler errors, even though they are
 * abstract base classes.
 * The error generated was due to a combination of template definitions and
 * virtual inheritance.
 * 
 * G++ generated the error:
 *	"harbinger::c_scriptFunc<returnType>::returnVal' has incomplete type"
 */

#endif	/* __HGE_SCRIPT_BASE_H__ */