/* 
 * File:   script_base.h
 * Author: hammy
 *
 * Created on September 25, 2012, 5:18 PM
 */

#ifndef SCRIPT_BASE_H
#define	SCRIPT_BASE_H

#include "script.h"


namespace harbinger {

//-----------------------------------------------------------------------------
//		Script Base Class
//-----------------------------------------------------------------------------
class c_script {
	static const char* defaultName;
	
	protected:
		std::string name;

	public:
		c_script();
		c_script( const c_script& scriptCopy );
		~c_script();
		virtual c_script& operator= ( const c_script& scriptCopy );
		
		virtual int getScriptType() const {
			return SCRIPT_BASE;
		}
		virtual const char* getScriptTypeStr() const {
			return "SCRIPT_BASE";
		}
		std::string& getName() const;
		void setName( const std::string& inName );
		virtual std::string toString() const;
		virtual bool fromString( const std::string& inStr );
};

//-----------------------------------------------------------------------------
//		Variable Base Classes
//		All variables MUST support copy construction & assignment operators
//-----------------------------------------------------------------------------
template <typename type>
class c_scriptVar : virtual public c_script {
	friend class c_scriptNum;
	
	template <typename returnType>
	friend class c_scriptFunc;
		
	protected:
		type data;
		
	public:
		c_scriptVar();
		c_scriptVar ( const type& varCopy );
		c_scriptVar( const c_scriptVar& varCopy );
		virtual ~c_scriptVar() = 0;
		virtual c_scriptVar& operator = ( const c_scriptVar<type>& varCopy );
		virtual c_scriptVar& operator = ( const type& inVar );
		
		virtual type& getData() const;
		virtual void setData( const type& inData );
		
		virtual int getScriptType() const {
			return SCRIPT_VAR | SCRIPT_BASE;
		}
		virtual const char* getScriptTypeStr() const {
			return "SCRIPT_VAR";
		}
		virtual std::string toString() const;
		virtual bool fromString( const std::string& inStr );
};

template <typename type>
c_scriptVar<type>::c_scriptVar() :
	data()
{}

template <typename type>
c_scriptVar<type>::c_scriptVar (const type& varCopy ) :
	data( varCopy )
{}

template <typename type>
c_scriptVar<type>::c_scriptVar( const c_scriptVar& varCopy ) :
	c_script( varCopy ),
	data( varCopy.data )
{}

template <typename type>
c_scriptVar<type>::~c_scriptVar() {}

template <typename type>
c_scriptVar<type>& c_scriptVar<type>::operator= ( const c_scriptVar<type>& varCopy ) {
	name = varCopy.name;
	data = varCopy.data;
	return *this;
}

template <typename type>
c_scriptVar<type>& c_scriptVar<type>::operator= ( const type& inVar ) {
	data = inVar;
	return *this;
}

template <typename type>
type& c_scriptVar<type>::getData() const {
	return data;
}

template <typename type>
void c_scriptVar<type>::setData( const type& inData ) {
	data = inData;
}

template <typename type>
std::string c_scriptVar<type>::toString() const {
	std::string retVal( c_script::toString() );
	retVal.append( hamLibs::stringUtils::convertToString<type>( data ) );
	return retVal;
}

template <typename type>
bool c_scriptVar<type>::fromString(const std::string& inStr) {
	std::string token;
	std::istringstream parser( inStr );
	bool ableToReadData( false );
	
	while ( parser.good() ) {
		parser >> token;
		
		if ( token == "|" && ableToReadData = false ) {
			ableToReadData = true;
			continue;
		}
		else if ( token == getScriptTypeStr()  && ableToReadData = false ) {
			parser >> name;
			if ( name.size() == 0 )
				return false;
		}
		else {
			parser >> data;
		}
	}
	parser.clear();
	return true;
}

//-----------------------------------------------------------------------------
//		Numerical Variable Base Class
//		--Doesn't inherit anything
//-----------------------------------------------------------------------------
class c_scriptNum {
	protected:
	public:
		c_scriptNum();
		c_scriptNum( const c_scriptNum& numCopy );
		virtual ~c_scriptNum();
		
		virtual c_scriptNum operator + ( const c_scriptNum& inVar );
		virtual c_scriptNum operator - ( const c_scriptNum& inVar );
		virtual c_scriptNum operator * ( const c_scriptNum& inVar );
		virtual c_scriptNum operator / ( const c_scriptNum& inVar );
		virtual c_scriptNum operator % ( const c_scriptNum& inVar );
		
		virtual c_scriptNum& operator = ( const c_scriptNum& inVar );
		virtual c_scriptNum& operator += ( const c_scriptNum& inVar );
		virtual c_scriptNum& operator -= ( const c_scriptNum& inVar );
		virtual c_scriptNum& operator *= ( const c_scriptNum& inVar );
		virtual c_scriptNum& operator /= ( const c_scriptNum& inVar );
		virtual c_scriptNum& operator %= ( const c_scriptNum& inVar );
		
		virtual bool operator == ( const c_scriptNum& inVar );
		virtual bool operator != ( const c_scriptNum& inVar );
		virtual bool operator > ( const c_scriptNum& inVar );
		virtual bool operator < ( const c_scriptNum& inVar );
		virtual bool operator >= ( const c_scriptNum& inVar );
		virtual bool operator <= ( const c_scriptNum& inVar );
		
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
};

//-----------------------------------------------------------------------------
//		Function Base Class
//-----------------------------------------------------------------------------
template <typename returnType = void*>
class c_scriptFunc : virtual public c_script {
	protected:
		returnType returnVal;
		
	public:
		c_scriptFunc();
		c_scriptFunc( const c_scriptFunc& funcCopy );
		virtual ~c_scriptFunc() = 0;
		 
		virtual const returnType* returnValue();
		virtual void run() = 0;
		virtual void tick( float timeElapsed = 0 ) = 0;
		
		virtual int getScriptType() const {
			return SCRIPT_FUNC | SCRIPT_BASE;
		}
		virtual const char* getScriptTypeStr() const {
			return "SCRIPT_FUNC";
		}
		virtual std::string toString() const = 0;
		virtual bool fromString( const std::string& inStr ) = 0;
};

template <typename returnType>
c_scriptFunc<returnType>::c_scriptFunc() :
	returnVal()
{}

template <typename returnType>
c_scriptFunc<returnType>::c_scriptFunc( const c_scriptFunc& funcCopy ) :
	c_script( funcCopy ),
	returnVal( funcCopy.returnVal )
{}

template <typename returnType>
c_scriptFunc<returnType>::~c_scriptFunc() {}

template <typename returnType>
const returnType* c_scriptFunc<returnType>::returnValue() {
	return &returnVal;
}

//-----------------------------------------------------------------------------
//		Evaluation Function Base Class
//-----------------------------------------------------------------------------
class c_scriptEvaluation : public c_scriptFunc<bool> {
	public:
		c_scriptEvaluation();
		c_scriptEvaluation( const c_scriptEvaluation& evalCopy );
		c_scriptEvaluation( bool inValue );
		virtual ~c_scriptEvaluation() = 0;

		const bool& evalResult() const;
		
		virtual int getScriptType() const {
			return SCRIPT_FUNC_EVAL | SCRIPT_FUNC | SCRIPT_BASE;
		}
		virtual const char* getScriptTypeStr() const {
			return "SCRIPT_FUNC_EVAL";
		}
		virtual std::string toString() const = 0;
		virtual bool fromString( const std::string& inStr ) = 0;
};

} // end harbinger namespace
#endif	/* SCRIPT_BASE_H */
