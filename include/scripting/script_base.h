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
	friend class c_scriptManager;
	friend class c_serializer;
	
	/* A note about the stream operators:
	 * The script type and sub-type will be printed when being sent to an ostream
	 * but they are not read back in by an istream.
	 * This is because the script type and sub-type must be determined before
	 * reading in any object data. This makes it much easier to determine what
	 * type of polymorphic object should be loaded when saving to/loading from files
	 */
	friend std::ostream& operator << ( std::ostream&, const c_script& );
	friend std::istream& operator >> ( std::istream&, c_script& );
	
	static const char* defaultName;
	static const char* invalidName;
	
	protected:
		std::string name;

	public:
		c_script			();
		c_script			( const c_script& scriptCopy );
		~c_script			();
		
		virtual c_script&	operator=			( const c_script& scriptCopy );
		virtual bool		operator==		( const c_script& scriptCopy ) const;
		virtual bool		operator!=		( const c_script& scriptCopy ) const;
		
		virtual int		getScriptType		() const { return SCRIPT_BASE; }
		virtual int		getScriptSubType	() const { return SCRIPT_INVALID; }
		
		const std::string&	getName			() const;
		void				setName			( const std::string& inName );
};

//-----------------------------------------------------------------------------
//		Variable Base Classe
//		All variables MUST support copy construction & assignment operators
//-----------------------------------------------------------------------------
//ADT used for separation of variables and functions
class c_scriptVarBase : virtual public c_script {
	friend class c_scriptManager;
	friend class c_scriptFuncBase;
		
	public:
		c_scriptVarBase	() {}
		c_scriptVarBase	( const c_scriptVarBase& varCopy ) :
			c_script( varCopy )
		{}
		~c_scriptVarBase	() {}
		
		int			getScriptType		() const { return SCRIPT_VAR; }
		virtual int	getScriptSubType	() const { return SCRIPT_INVALID; }
		
		virtual c_scriptVarBase& operator =	( const c_scriptVarBase& varCopy );
		virtual bool	operator==		( const c_scriptVarBase& scriptCopy ) const;
		virtual bool	operator!=		( const c_scriptVarBase& scriptCopy ) const;
};

//-----------------------------------------------------------------------------
//		Variable Interface Classe
//		All variables MUST support copy construction & assignment operators
//-----------------------------------------------------------------------------
template <typename type>
class c_scriptVar : virtual public c_scriptVarBase {
	friend class c_scriptManager;
	
	/* Serialization Spec:
	 * name
	 * variable
	 */
	template <typename T>
	friend std::ostream& operator << ( std::ostream&, const c_scriptVar<T>& );
	template <typename T>
	friend std::istream& operator >> ( std::istream&, c_scriptVar<T>& );
	
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
	c_scriptVarBase( varCopy ),
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
	name = varCopy.name;
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
	public:
		c_scriptNum();
		c_scriptNum( const c_scriptNum& numCopy );
		virtual ~c_scriptNum();
		
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
};

//-----------------------------------------------------------------------------
//		Function Base Classes
//-----------------------------------------------------------------------------
class c_scriptFuncBase : virtual public c_script {
	friend class c_scriptManager;
	public:
		c_scriptFuncBase();
		c_scriptFuncBase( const c_scriptFuncBase& funcCopy );
		virtual ~c_scriptFuncBase() = 0;
		 
		virtual void run() = 0;
		virtual void tick( float timeElapsed = 0 ) = 0;
		
		int getScriptType() const { return SCRIPT_FUNC; }
		virtual int getScriptSubType() const { return SCRIPT_INVALID; }
};

//-----------------------------------------------------------------------------
//		Function Class Interface (abstract due to c_scriptFuncBase)
//-----------------------------------------------------------------------------
template <typename returnType>
class c_scriptFunc : virtual public c_scriptFuncBase {
	friend class c_scriptManager;
	protected:
		returnType returnVal;
		
	public:
		c_scriptFunc();
		c_scriptFunc( const c_scriptFunc& funcCopy );
		~c_scriptFunc();
		
		returnType getReturnVal() const;
		returnType& getReturnVal();
};

template <typename returnType>
c_scriptFunc< returnType >::c_scriptFunc() :
	returnVal()
{}

template <typename returnType>
c_scriptFunc< returnType >::c_scriptFunc( const c_scriptFunc& funcCopy ) :
	c_scriptFuncBase( funcCopy ),
	returnVal()
{}

template <typename returnType>
c_scriptFunc< returnType >::~c_scriptFunc() {}

template <typename returnType>
returnType c_scriptFunc< returnType >::getReturnVal() const {
	return returnVal;
}

template <typename returnType>
returnType& c_scriptFunc< returnType >::getReturnVal() {
	return returnVal;
}

//-----------------------------------------------------------------------------
//		Evaluation Function Base Class
//-----------------------------------------------------------------------------
class c_scriptEvaluation : virtual public c_scriptFunc< bool > {
	friend class c_scriptManager;
	
	friend std::ostream& operator << ( std::ostream&, const c_scriptEvaluation& );
	friend std::istream& operator >> ( std::istream&, c_scriptEvaluation& );
	
	protected:
		int evalType;
	
	public:
		c_scriptEvaluation();
		c_scriptEvaluation( const c_scriptEvaluation& evalCopy );
		virtual ~c_scriptEvaluation();
			
	virtual int getScriptSubType() const {
			return SCRIPT_FUNC_EVAL;
		}
		
		virtual void setEvalType( int eval = 0 );
		int getEvalType() const;
		
		virtual const c_scriptVarBase* getVarToEvaluate() const;
		virtual void attachVarToEvaluate( const c_scriptVarBase* inVar ) = 0;
		virtual void detachVarToEvaluate() = 0;
		
		virtual const c_scriptVarBase* getVarToReference() const;
		virtual void attachVarToReference( const c_scriptVarBase* inVar ) = 0;
		virtual void detachVarToReference() = 0;
};

} // end harbinger namespace
#endif	/* SCRIPT_BASE_H */
