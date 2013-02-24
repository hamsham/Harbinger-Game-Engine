/* 
 * File:   script_variables.h
 * Author: hammy
 *
 * Created on September 25, 2012, 5:21 PM
 */

#ifndef __HGE_SCRIPT_VARIABLES_H__
#define	__HGE_SCRIPT_VARIABLES_H__

#include <string>
#include "script.h"
#include "script_base.h"

///////////////////////////////////////////////////////////////////////////////
//		Typedefs
///////////////////////////////////////////////////////////////////////////////
// Object Variables
typedef c_var< SCRIPT_VAR_INT, int >			c_varInt;
typedef c_var< SCRIPT_VAR_FLOAT, float >		c_varFloat;
typedef c_var< SCRIPT_VAR_BOOL, bool >			c_varBool;
typedef c_var< SCRIPT_VAR_BOOL, vec3 >			c_varVec3;
typedef c_var< SCRIPT_VAR_BOOL, std::string >	c_varString;

///////////////////////////////////////////////////////////////////////////////
//		Specializations
///////////////////////////////////////////////////////////////////////////////
template <>	void HGE_API c_varBool::read	( std::istream&, scriptMap_t& );
template <>	void HGE_API c_varBool::write	( std::ostream& ) const;

template <> void HGE_API c_varVec3::read	( std::istream&, scriptMap_t& );
template <> void HGE_API c_varVec3::write	( std::ostream& ) const;

template <> void HGE_API c_varString::read	( std::istream&, scriptMap_t& );
template <> void HGE_API c_varString::write	( std::ostream& ) const;

#endif	/* __HGE_SCRIPT_VARIABLES_H__ */
