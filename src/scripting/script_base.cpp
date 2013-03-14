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
#ifdef HGE_EDITOR
	std::string::size_type strSize( 0 );
	fin >> strSize;
    
	if ( strSize != 0 ) {
        fin.get(); // discard the next whitespace before reading in the string
        data.resize( strSize );
        
        for ( std::string::size_type iter( 0 ); iter < strSize; ++iter )
            data[ iter ] = fin.get();
    }
    
    stin >> editorPos[0] >> editorPos[1];
#endif
	void* ptr( nullptr );
	stin >> ptr;
	scriptMap[ ptr ] = this;
}

void c_script::write( std::ostream& stout ) const {
#ifdef HGE_EDITOR
    stout << editorName.size() << ' ' << editorName;
    stout << editorPos[0] << ' ' << editorPos[1] << ' ';
#endif
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
	return (retVal == func.retVal ) && (nextFunc == func.nextFunc);
}

bool c_funcBase::operator != ( const c_funcBase& func ) {
	return (retVal != func.retVal ) && (nextFunc != func.nextFunc);
}
