/* 
 * File:   script_base.cpp
 * Author: hammy
 * 
 * Created on September 25, 2012, 5:18 PM
 */

#include "../../include/harbinger.h"

namespace harbinger {

//-----------------------------------------------------------------------------
//		Script Base Class
//-----------------------------------------------------------------------------
const char* c_script::defaultName = "???";

c_script::c_script() :
	name( defaultName )
{}

c_script::c_script( const c_script& scriptCopy ) :
	name (scriptCopy.name)
{}

c_script::~c_script() {}

c_script& c_script::operator= ( const c_script& scriptCopy ) {
	name = scriptCopy.name;
	return *this;
}

std::string& c_script::getName() const {
	return name;
}

void c_script::setName( const std::string& inName ) {
	name = inName;
}

std::string c_script::toString() const {
	//the format is "TYPE NAME | data..."
	std::string outStr ( getScriptTypeStr() );
	outStr.push_back(' '); //always need whitespace
	outStr.append( name );
	outStr.append(" | "); //separator for use with derived classes
	return outStr;
}

bool c_script::fromString(const std::string& inStr) {
	std::string token;
	std::istringstream parser( inStr );
	
	while ( parser.good() ) {
		parser >> token;
		
		if ( token == "|") {
			continue;
		}
		else if ( token == getScriptTypeStr() ) {
			parser >> name;
			if ( name.size() == 0 )
				return false;
		}
	}
	parser.clear();
	return true;
}

//-----------------------------------------------------------------------------
//		Number Variable Base Class
//-----------------------------------------------------------------------------
c_scriptNum::c_scriptNum() {}
c_scriptNum::c_scriptNum( const c_scriptNum& numCopy ) {}
c_scriptNum::~c_scriptNum() {}

c_scriptNum c_scriptNum::operator + ( const c_scriptNum& inVar ) {
	return inVar;
}
c_scriptNum c_scriptNum::operator - ( const c_scriptNum& inVar ) {
	return inVar;
}
c_scriptNum c_scriptNum::operator * ( const c_scriptNum& inVar ) {
	return inVar;
}
c_scriptNum c_scriptNum::operator / ( const c_scriptNum& inVar ) {
	return inVar;
}
c_scriptNum c_scriptNum::operator % ( const c_scriptNum& inVar ) {
	return inVar;
}

c_scriptNum& c_scriptNum::operator = ( const c_scriptNum& inVar ) {
	return *this;
}
c_scriptNum& c_scriptNum::operator += ( const c_scriptNum& inVar ) {
	return *this;
}
c_scriptNum& c_scriptNum::operator -= ( const c_scriptNum& inVar ) {
	return *this;
}
c_scriptNum& c_scriptNum::operator *= ( const c_scriptNum& inVar ) {
	return *this;
}
c_scriptNum& c_scriptNum::operator /= ( const c_scriptNum& inVar ) {
	return *this;
}
c_scriptNum& c_scriptNum::operator %= ( const c_scriptNum& inVar ) {
	return *this;
}

bool c_scriptNum::operator == ( const c_scriptNum& inVar ) {
	return false;
}

bool c_scriptNum::operator != ( const c_scriptNum& inVar ) {
	return false;
}

bool c_scriptNum::operator > ( const c_scriptNum& inVar ) {
	return false;
}

bool c_scriptNum::operator < ( const c_scriptNum& inVar ) {
	return false;
}

bool c_scriptNum::operator >= ( const c_scriptNum& inVar ) {
	return false;
}

bool c_scriptNum::operator <= ( const c_scriptNum& inVar ) {
	return false;
}

c_scriptNum& c_scriptNum::operator= ( int inVar ) {
	return *this;
}

c_scriptNum& c_scriptNum::operator= ( unsigned int inVar ) {
	return *this;
}

c_scriptNum& c_scriptNum::operator= ( float inVar ) {
	return *this;
}

c_scriptNum::operator int() const {
	return 0;
}

c_scriptNum::operator unsigned int() const {
	return 0;
}

c_scriptNum::operator float() const {
	return 0.0f;
}

//-----------------------------------------------------------------------------
//		Evaluation Function Base Class
//-----------------------------------------------------------------------------
c_scriptEvaluation::c_scriptEvaluation() {
	returnVal = false;
}

c_scriptEvaluation::c_scriptEvaluation( const c_scriptEvaluation& evalCopy ) :
	c_script( evalCopy )
{}

c_scriptEvaluation::c_scriptEvaluation( bool inValue ) {
	
	returnVal = inValue;
}

c_scriptEvaluation::~c_scriptEvaluation() {}

const bool& c_scriptEvaluation::evalResult() const {
	return returnVal;
}

} // end harbinger namespace
