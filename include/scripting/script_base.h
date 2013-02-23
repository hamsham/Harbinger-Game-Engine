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
	private:
		int xPos = 0;
		int yPos = 0;
		
	public:
		c_script				() {}
		c_script				( const c_script& b ) : xPos( b.xPos ), yPos( b.yPos ) {}
		virtual ~c_script		() = 0;
		
		virtual long int	getScriptType	() const = 0;
		virtual long int	getScriptSubType() const = 0;
		
		virtual void		read			( std::istream&, scriptMap_t& );
		virtual void		write			( std::ostream& ) const;
		
		void				setPos			( int x, int y )	{ xPos = x; yPos = y; }
		int					getXPos			() const			{ return xPos; }
		int					getYPos			() const			{ return yPos; }
};

///////////////////////////////////////////////////////////////////////////////
//		Variable Base Class
///////////////////////////////////////////////////////////////////////////////
class HGE_API c_varBase : virtual public c_script {
	public:
		c_varBase				()						: c_varBase() {}
		c_varBase				( const c_varBase& v )	: c_varBase( v ) {}
		virtual ~c_varBase		() = 0;
		
		long int	getScriptType	() const				{ return SCRIPT_VAR; }
};

///////////////////////////////////////////////////////////////////////////////
//		Variable Template Class
///////////////////////////////////////////////////////////////////////////////
template <e_varType varType, typename type>
class HGE_API c_var : virtual public c_varBase {
	public:
		type data;
		
		c_var						()						: c_varBase(), data() {}
		c_var						( const type& v )		: c_varBase(), data( v ) {}
		c_var						( const c_var& v )		: c_varBase( v ), data( v.data ) {}
		~c_var						()						{}
		
		long int		getScriptSubType() const			{ return varType; }
		
		virtual void	read		( std::istream&, scriptMap_t& );
		virtual void	write		( std::ostream& ) const;
};

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

//Specializations
template <>	void HGE_API c_varBool::read	( std::istream&, scriptMap_t& );
template <>	void HGE_API c_varBool::write	( std::ostream& ) const;

template <> void HGE_API c_varVec3::read	( std::istream&, scriptMap_t& );
template <> void HGE_API c_varVec3::write	( std::ostream& ) const;

template <> void HGE_API c_varString::read	( std::istream&, scriptMap_t& );
template <> void HGE_API c_varString::write	( std::ostream& ) const;

///////////////////////////////////////////////////////////////////////////////
//		Function Base Class
///////////////////////////////////////////////////////////////////////////////
class HGE_API c_funcBase : virtual public c_script {
	protected:
		c_varBase*	retVal			= nullptr;
		c_funcBase* nextFunc			= nullptr;
		
	public:
		c_funcBase					()						: c_script() {}
		c_funcBase					( const c_funcBase& f )	: c_script( f ), retVal( f.retVal ), nextFunc( f.nextFunc )  {}
		virtual ~c_funcBase			() = 0;
		
		long int		getScriptType() const				{ return SCRIPT_FUNC; }
		
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
class HGE_API c_func : virtual public c_funcBase {
	protected:
		c_varBase*	args[ numArgs ];
		
	public:
		c_func		() : c_funcBase(), args{ nullptr }	{}
		c_func		( const c_func& f );
		~c_func		() {}
		
		long int	getScriptSubType() const					{ return fncType; }
		
		void		attachArg	( int index, c_varBase& );
		void		attachRet	( c_varBase& );
		void		detachArg	( int index );
		void		detachRet	();
		
		void		read		( std::istream&, scriptMap_t& );
		void		write		( std::ostream& )	const;
		
		void		run			();
};

template <e_funcType fncType, int numArgs, typename retType >
c_func< fncType, numArgs, retType >::c_func( const c_func& f ) :
	c_funcBase( f )
{
	for (int i(0); i<numArgs; ++i)
	args[i]=f.args[i];
}

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