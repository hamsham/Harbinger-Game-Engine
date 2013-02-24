/* 
 * File:   script_base.cpp
 * Author: hammy
 * 
 * Created on September 25, 2012, 5:18 PM
 */

#include "scripting/script.h"
#include "scripting/script_base.h"

//-----------------------------------------------------------------------------
//		Script Base Class
//-----------------------------------------------------------------------------
void c_script::read( std::istream& stin, scriptMap_t& scriptMap ) {
	void* ptr( nullptr );
	stin >> ptr;
	scriptMap[ ptr ] = this;
}

void c_script::write( std::ostream& stout ) const {
	stout << this;
}

//-----------------------------------------------------------------------------
//		Function Base Class
//-----------------------------------------------------------------------------
void c_funcBase::read( std::istream& stin, scriptMap_t& scriptMap ) {
	void* nextPtr( nullptr );
	void* retPtr( nullptr );
	
	c_script::read( stin, scriptMap );
	stin >> nextPtr >> retPtr;
	
	nextFunc = dynamic_cast< c_funcBase* >( scriptMap[ nextPtr ] ); 
	retVal = dynamic_cast< c_varBase* >( scriptMap[ retPtr ] ); 
}

void c_funcBase::write( std::ostream& stout ) const {
	c_script::write( stout );
	stout
		<< ' ' << static_cast< c_script* >( nextFunc )
		<< ' ' << static_cast< c_script* >( retVal );
}
		
c_funcBase& c_funcBase::operator = ( const c_funcBase& func ) {
	retVal = func.retVal;
	nextFunc = func.nextFunc;
	return *this;
}

bool c_funcBase::operator == ( const c_funcBase& func ) {
	if ( (retVal != func.retVal ) || (nextFunc != func.nextFunc) )
		return false;
	return true;
}

bool c_funcBase::operator != ( const c_funcBase& func ) {
	if ( (retVal == func.retVal ) && (nextFunc == func.nextFunc) )
		return false;
	return true;
}
