/* 
 * File:   script_base.h
 * Author: hammy
 *
 * Created on September 25, 2012, 5:18 PM
 */

#ifndef __HGE_SCRIPT_BASE_H__
#define	__HGE_SCRIPT_BASE_H__

#include <iostream>
#include <string>

#include "../types.h"
#include "script.h"

///////////////////////////////////////////////////////////////////////////////
//		Base Class
///////////////////////////////////////////////////////////////////////////////
class HGE_API c_script {
#ifdef HGE_EDITOR
	private:
		vec2i   editorPos;
        string  editorName;
		
	public:
		c_script			() : editorPos(), editorName() {}
		c_script			( const c_script& b ) : editorPos( b.pos ), editorName() {}
		
		void				setEditorPos	( const vec2i& inPos )	{ editorPos = inPos; }
		const vec2i&		getEditorPos	() const				{ return editorPos; }
        
        void                setEditorName   ( const std::string& n ){ editorName = n; }
        const std::string&  getEditorName   () const                { return editorName; }
        
        virtual void        drawEditor      () const = 0;
#else
	public:
		c_script			() {}
		c_script			( const c_script& ) {}
#endif /* ifdef HGE_EDITOR */
		
		~c_script			() {}
		
		virtual ulong   	getScriptType	() const = 0;
		virtual ulong       getScriptSubType() const = 0;
		
		virtual void		read			( std::istream&, scriptMap_t& );
		virtual void		write			( std::ostream& ) const;
};

///////////////////////////////////////////////////////////////////////////////
//		Variable Base Class
///////////////////////////////////////////////////////////////////////////////
class HGE_API c_varBase : virtual public c_script {
	public:
		c_varBase	()						: c_script() {}
		c_varBase	( const c_varBase& v )	: c_script( v ) {}
		~c_varBase	() {}
		
		ulong   	getScriptType			() const { return SCRIPT_VAR; }
};

///////////////////////////////////////////////////////////////////////////////
//		Variable Template Class
///////////////////////////////////////////////////////////////////////////////
template < e_varType varType, typename type>
class HGE_API c_var : virtual public c_varBase {
	public:
		type data;
		
		c_var		()					: c_varBase(), data() {}
		c_var		( const type& v )	: c_varBase(), data( v ) {}
		c_var		( const c_var& v )	: c_varBase( v ), data( v.data ) {}
		~c_var		()					{}
		
		c_var&		operator =			( const c_var& );
		bool		operator ==			( const c_var& );
		bool		operator !=			( const c_var& );
		
		ulong   	getScriptSubType	() const { return varType; }
		
		void		read				( std::istream&, scriptMap_t& );
		void		write				( std::ostream& ) const;
};

// Overloaded Operators
template < e_varType varType, typename type>
c_var< varType, type>& c_var< varType, type>::operator = ( const c_var& var ) {
	data = var.data;
	return *this;
}

template < e_varType varType, typename type>
bool c_var< varType, type>::operator == ( const c_var& var ) {
	return data == var.data;
}

template < e_varType varType, typename type>
bool c_var< varType, type>::operator != ( const c_var& var ) {
	return var != var.data;
}

// Serialization
template < e_varType varType, typename type>
void c_var< varType, type>::read( std::istream& stin, scriptMap_t& objMap ) {
	c_script::read( stin, objMap );
	stin >> data;
}

template < e_varType varType, typename type>
void c_var< varType, type>::write( std::ostream& stout ) const {
	c_script::write( stout );
	stout << ' ' << data;
}

///////////////////////////////////////////////////////////////////////////////
//		Function Base Class
///////////////////////////////////////////////////////////////////////////////
class HGE_API c_funcBase : virtual public c_script {
	protected:
		c_varBase*	retVal			= nullptr;
		c_funcBase* nextFunc		= nullptr;
		
	public:
		c_funcBase					()						: c_script() {}
		c_funcBase					( const c_funcBase& f )	: c_script( f ), retVal( f.retVal ), nextFunc( f.nextFunc )  {}
		~c_funcBase					() {}
		
		c_funcBase&		operator =	( const c_funcBase& );
		bool			operator ==	( const c_funcBase& );
		bool			operator !=	( const c_funcBase& );
		
		ulong           getScriptType() const		{ return SCRIPT_FUNC; }
		
		virtual void	attachArg	( int, c_varBase& )		= 0;
		virtual void	attachRet	( c_varBase& )			= 0;
		virtual void	detachArg	( int )					= 0;
		virtual void	detachRet	()						= 0;
		
		virtual void	read		( std::istream&, scriptMap_t& );
		virtual void	write		( std::ostream& )	const;
		
		virtual void	run			()						= 0;
};

///////////////////////////////////////////////////////////////////////////////
//		Function Template Class
///////////////////////////////////////////////////////////////////////////////
template < e_funcType fncType, int numArgs >
class HGE_API c_func : virtual public c_funcBase {
	protected:
		c_varBase*	args[ numArgs ];
		
	public:
		c_func		() : c_funcBase(), args{ nullptr }	{}
		c_func		( const c_func& f );
		~c_func		() {}
		
		c_func&		operator =		( const c_func& );
		bool		operator ==		( const c_func& );
		bool		operator !=		( const c_func& );
		
		ulong       getScriptSubType() const { return fncType; }
		
		void		attachArg		( int index, c_varBase& );
		void		attachRet		( c_varBase& );
		void		detachArg		( int index );
		void		detachRet		();
		
		void		read			( std::istream&, scriptMap_t& );
		void		write			( std::ostream& ) const;
		
		void		run				();
};

/*
 * c_func partial specialization
 */
template < e_funcType fncType >
class HGE_API c_func< fncType, 0 > : virtual public c_funcBase {
		
	public:
		c_func		() : c_funcBase() {}
		c_func		( const c_func& f ) : c_funcBase( f ) {}
		~c_func		() {}
		
		c_func&		operator =		( const c_func& f ) { c_funcBase::operator =( f ); return *this; }
		bool		operator ==		( const c_func& f ) { return c_funcBase::operator ==( f ); }
		bool		operator !=		( const c_func& f ) { return c_funcBase::operator !=( f ); }
		
		ulong       getScriptSubType() const { return fncType; }
		
		void		attachArg		( int index, c_varBase& ) { HGE_ASSERT(false); }
		void		attachRet		( c_varBase& );
		void		detachArg		( int index ) { HGE_ASSERT(false); }
		void		detachRet		();
		
		void		read			( std::istream& stin, scriptMap_t& scriptMap ) { c_funcBase::read( stin, scriptMap ); }
		void		write			( std::ostream& stout ) const  { c_funcBase::write( stout ); }
		
		void		run				();
};

// Construction
template < e_funcType fncType, int numArgs >
c_func< fncType, numArgs >::c_func( const c_func& f ) :
	c_funcBase( f )
{
	for (int i(0); i<numArgs; ++i)
	args[i]=f.args[i];
}

// Overloaded Operators
template < e_funcType fncType, int numArgs >
c_func< fncType, numArgs >& c_func< fncType, numArgs >::operator = ( const c_func& func ) {
	c_funcBase::operator = ( func );
	for ( int i( 0 ); i < numArgs; ++i )
		args[i] = func.args[i];
	return *this;
}

template < e_funcType fncType, int numArgs >
bool c_func< fncType, numArgs >::operator == ( const c_func& func ) {
	if ( c_funcBase::operator !=( func ) )
		return false;
	
	for ( int i( 0 ); i < numArgs; ++i ) {
		if ( args[i] != func.args[i] )
			return false;
	}
	return true;
}

template < e_funcType fncType, int numArgs >
bool c_func< fncType, numArgs >::operator != ( const c_func& func ) {
	if ( c_funcBase::operator !=( func ) )
        return true;
	
    for ( int i( 0 ); i < numArgs; ++i ) {
        if ( args[i] != func.args[i] )
            return true;
    }
    return false;
}

// Argument Management
template < e_funcType fncType, int numArgs >
void c_func< fncType, numArgs >::attachArg( int index, c_varBase& var ) {
	args[ index ] = &var;
}

template < e_funcType fncType, int numArgs >
void c_func< fncType, numArgs >::attachRet( c_varBase& var ) {
	retVal = &var;
}

template < e_funcType fncType, int numArgs >
void c_func< fncType, numArgs >::detachArg( int index ) {
	HGE_ASSERT( (index >= 0) && (index < numArgs) );
	args[ index ] = nullptr;
}

template < e_funcType fncType, int numArgs >
void c_func< fncType, numArgs >::detachRet() {
	retVal = nullptr;
}

// Serialization
template < e_funcType fncType, int numArgs >
void c_func< fncType, numArgs >::read( std::istream& stin, scriptMap_t& objMap ) {
	c_funcBase::read( stin, objMap );
	void* ptr( nullptr );
	for ( int i(0); i < numArgs; ++i ) {
		stin >> ptr;
		args[i] = dynamic_cast< c_varBase* >( objMap[ ptr ] );
	}
}

template < e_funcType fncType, int numArgs >
void c_func< fncType, numArgs >::write( std::ostream& stout ) const {
	c_funcBase::write( stout );
	// When serializing, pointer values are incremented by an offset
	// if saved as a c_baseVar* vs a c_base* data.
	// Save datas as a c_base* ptr to avoid this difference in values.
	for ( int i(0); i < numArgs; ++i )
		stout << ' ' << static_cast< c_script* >( args[i] );
}

#endif	/* __HGE_SCRIPT_BASE_H__ */