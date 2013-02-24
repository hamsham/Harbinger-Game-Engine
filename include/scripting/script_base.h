/* 
 * File:   script_base.h
 * Author: hammy
 *
 * Created on September 25, 2012, 5:18 PM
 */

#ifndef __HGE_SCRIPT_BASE_H__
#define	__HGE_SCRIPT_BASE_H__

#include <fstream>

#include "../types.h"
#include "script.h"

///////////////////////////////////////////////////////////////////////////////
//		Base Class
///////////////////////////////////////////////////////////////////////////////
class HGE_API c_script {
#ifdef HGE_EDITOR
	private:
		vec2i pos;
		
	public:
		c_script			() : pos() {}
		c_script			( const c_script& b ) : pos( b.pos ) {}
		
		void				setPos			( const vec2i& inPos )	{ pos = inPos; }
		const vec2i			getPos			() const				{ return pos; }
#else
	public:
		c_script			() {}
		c_script			( const c_script& ) {}
#endif /* ifdef HGE_EDITOR */
		~c_script			() {}
		
		virtual long int	getScriptType	() const = 0;
		virtual long int	getScriptSubType() const = 0;
		
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
		
		long int	getScriptType			() const override { return SCRIPT_VAR; }
};

///////////////////////////////////////////////////////////////////////////////
//		Variable Template Class
///////////////////////////////////////////////////////////////////////////////
template <e_varType varType, typename type>
class HGE_API c_var final : virtual public c_varBase {
	public:
		type data;
		
		c_var		()					: c_varBase(), data() {}
		c_var		( const type& v )	: c_varBase(), data( v ) {}
		c_var		( const c_var& v )	: c_varBase( v ), data( v.data ) {}
		~c_var		()					{}
		
		c_var&		operator =			( const c_var& );
		bool		operator ==			( const c_var& );
		bool		operator !=			( const c_var& );
		
		long int	getScriptSubType	() const override { return varType; }
		
		void		read				( std::istream&, scriptMap_t& ) override;
		void		write				( std::ostream& ) const override;
};

// Overloaded Operators
template <e_varType varType, typename type>
c_var< varType, type>& c_var< varType, type>::operator = ( const c_var& var ) {
	data = var.data;
	return *this;
}

template <e_varType varType, typename type>
bool c_var< varType, type>::operator == ( const c_var& var ) {
	return data == var.data;
}

template <e_varType varType, typename type>
bool c_var< varType, type>::operator != ( const c_var& var ) {
	return var != var.data;
}

// Serialization
template <e_varType varType, typename type>
void c_var< varType, type>::read( std::istream& stin, scriptMap_t& objMap ) {
	c_script::read( stin, objMap );
	stin >> data;
}

template <e_varType varType, typename type>
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
		
		long int		getScriptType() const override		{ return SCRIPT_FUNC; }
		
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
template <e_funcType fncType, int numArgs, typename retType >
class HGE_API c_func final : virtual public c_funcBase {
	protected:
		c_varBase*	args[ numArgs ];
		
	public:
		c_func		() : c_funcBase(), args{ nullptr }	{}
		c_func		( const c_func& f );
		~c_func		() {}
		
		c_func&		operator =		( const c_func& );
		bool		operator ==		( const c_func& );
		bool		operator !=		( const c_func& );
		
		long int	getScriptSubType() const override { return fncType; }
		
		void		attachArg		( int index, c_varBase& ) override;
		void		attachRet		( c_varBase& ) override;
		void		detachArg		( int index ) override;
		void		detachRet		();
		
		void		read			( std::istream&, scriptMap_t& ) override;
		void		write			( std::ostream& ) const override;
		
		void		run				() override;
};

// Construction
template <e_funcType fncType, int numArgs, typename retType >
c_func< fncType, numArgs, retType >::c_func( const c_func& f ) :
	c_funcBase( f )
{
	for (int i(0); i<numArgs; ++i)
	args[i]=f.args[i];
}

// Overloaded Operators
template <e_funcType fncType, int numArgs, typename retType >
c_func< fncType, numArgs, retType >& c_func< fncType, numArgs, retType >::operator = ( const c_func& func ) {
	c_funcBase::operator = ( func );
	for ( int i( 0 ); i < numArgs; ++i )
		args[i] = func.args[i];
	return *this;
}

template <e_funcType fncType, int numArgs, typename retType >
bool c_func< fncType, numArgs, retType >::operator == ( const c_func& func ) {
	if ( c_funcBase::operator !=( func ) )
		return false;
	
	for ( int i( 0 ); i < numArgs; ++i ) {
		if ( args[i] != func.args[i] )
			return false;
	}
	return true;
}

template <e_funcType fncType, int numArgs, typename retType >
bool c_func< fncType, numArgs, retType >::operator != ( const c_func& func ) {
	if ( c_funcBase::operator ==( func ) )
		return false;
	
	for ( int i( 0 ); i < numArgs; ++i ) {
		if ( args[i] == func.args[i] )
			return false;
	}
	return true;
}

// Argument Management
template <e_funcType fncType, int numArgs, typename retType >
void c_func< fncType, numArgs, retType >::attachArg( int index, c_varBase& var ) {
	args[ index ] = &var;
}

template <e_funcType fncType, int numArgs, typename retType >
void c_func< fncType, numArgs, retType >::attachRet( c_varBase& var ) {
	retVal = &var;
}

template <e_funcType fncType, int numArgs, typename retType >
void c_func< fncType, numArgs, retType >::detachArg( int index ) {
	HGE_ASSERT( (index >= 0) && (index < numArgs) );
	args[ index ] = nullptr;
}

template <e_funcType fncType, int numArgs, typename retType >
void c_func< fncType, numArgs, retType >::detachRet() {
	retVal = nullptr;
}

// Serialization
template <e_funcType fncType, int numArgs, typename retType >
void c_func< fncType, numArgs, retType >::read( std::istream& stin, scriptMap_t& objMap ) {
	c_funcBase::read( stin, objMap );
	void* ptr( nullptr );
	for ( int i(0); i < numArgs; ++i ) {
		stin >> ptr;
		args[i] = dynamic_cast< c_varBase* >( objMap[ ptr ] );
	}
}

template <e_funcType fncType, int numArgs, typename retType >
void c_func< fncType, numArgs, retType >::write( std::ostream& stout ) const {
	c_funcBase::write( stout );
	// When serializing, pointer values are incremented by an offset
	// if saved as a c_baseVar* vs a c_base* data.
	// Save datas as a c_base* ptr to avoid this difference in values.
	for ( int i(0); i < numArgs; ++i )
		stout << ' ' << static_cast< c_script* >( args[i] );
}

#endif	/* __HGE_SCRIPT_BASE_H__ */