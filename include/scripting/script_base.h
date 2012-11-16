/* 
 * File:   script_base.h
 * Author: hammy
 *
 * Created on September 25, 2012, 5:18 PM
 */

#ifndef SCRIPT_BASE_H
#define	SCRIPT_BASE_H
namespace harbinger {

//-----------------------------------------------------------------------------
//		Script Base Class
//-----------------------------------------------------------------------------
class c_script {
	friend class c_scriptManager;
	friend class c_serialize;

	public:
		virtual ~c_script	() = 0;
		
		virtual c_script&	operator=			( const c_script& scriptCopy );
		virtual bool		operator==		( const c_script& scriptCopy ) const;
		virtual bool		operator!=		( const c_script& scriptCopy ) const;
		
		virtual int		getScriptType		() const { return SCRIPT_BASE; }
		virtual int		getScriptSubType	() const { return SCRIPT_INVALID; }
};

//-----------------------------------------------------------------------------
//		Variable Base Class
//		All variables MUST support copy construction & assignment operators
//-----------------------------------------------------------------------------
//ADT used for separation of variables and functions
class c_scriptVarBase : virtual public c_script {
	friend class c_scriptManager;
	friend class c_serialize;
	friend class c_scriptFuncBase;
		
	public:
		virtual ~c_scriptVarBase	() = 0;
		
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
class c_scriptVar : virtual public c_scriptVarBase {
	friend class c_scriptManager;
	friend class c_serialize;
	
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

//-----------------------------------------------------------------------------
//		Numerical Variable Base Class
//-----------------------------------------------------------------------------
class c_scriptNum : virtual public c_scriptVarBase {
	friend class c_scriptManager;
	friend class c_serialize;
	
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
//		Function Base Classes
//		Abstract
//-----------------------------------------------------------------------------
class c_scriptFuncBase : virtual public c_script {
	friend class c_scriptManager;
	friend class c_serialize;
	
	protected:
		const c_scriptFuncBase* nextFunc;
	
	public:
		c_scriptFuncBase();
		c_scriptFuncBase( const c_scriptFuncBase& funcCopy );
		virtual ~c_scriptFuncBase() = 0;
		 
		virtual void run() = 0;
		virtual void tick( float timeElapsed = 0 ) {
			assert( timeElapsed == 0);
			run();
		}
		
		int getScriptType() const { return SCRIPT_FUNC; }
		virtual int getScriptSubType() const { return SCRIPT_INVALID; }
		
		const c_scriptFuncBase* getNextFunction() const;
		void setNextFunction( const c_scriptFuncBase& next );
};

//-----------------------------------------------------------------------------
//		Function Class Interface
//		Abstract
//-----------------------------------------------------------------------------
template <typename returnType>
class c_scriptFunc : virtual public c_scriptFuncBase {
	friend class c_scriptManager;
	friend class c_serialize;
	
	protected:
		returnType returnVal;
		
	public:
		c_scriptFunc();
		c_scriptFunc( const c_scriptFunc& funcCopy );
		c_scriptFunc( const returnType& retVal );
		virtual ~c_scriptFunc() = 0;
		
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

} // end harbinger namespace
#endif	/* SCRIPT_BASE_H */
