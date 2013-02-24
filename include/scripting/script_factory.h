/* 
 * File:   script_factory.h
 * Author: hammy
 *
 * Created on February 24, 2013, 2:31 PM
 */

#ifndef __HGE_SCRIPT_FACTORY_H__
#define	__HGE_SCRIPT_FACTORY_H__

#include "script.h"

///////////////////////////////////////////////////////////////////////////////
// Script Factory (singleton)
///////////////////////////////////////////////////////////////////////////////

namespace n_scriptFactory {
	
	c_script*	HGE_API getVarInstance	( long scriptType );
	c_script*	HGE_API getFuncInstance	( long scriptType );
	void		HGE_API killInstance	( c_script* );
	
};

#undef REGISTERED_FACTORY

#endif	/* __HGE_SCRIPT_FACTORY_H__ */
