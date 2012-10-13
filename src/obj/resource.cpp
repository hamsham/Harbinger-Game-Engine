/* 
 * File:   resource.cpp
 * Author: hammy
 * 
 * Created on September 9, 2012, 9:35 PM
 */

#include "../../include/harbinger.h"
namespace harbinger {

//-----------------------------------------------------------------------------
//			Serialization
//-----------------------------------------------------------------------------
/*
int c_resourceManager::loadFromConfig( hamLibs::config& cfg ) {
	if ( cfg.checkIfOpen() == false )
		return RESOURCE_LOAD_INVALID_CFG;
	
	const char* cfgSection = cfg.getFirstConfigSection();
	const char* cfgEntry = cfg.getFirstConfigEntry( cfgSection );
	const char* cfgValue = NULL;
	c_script* temp = NULL;
	
	//when loading an object fron a config, use the object's section as the
	//dictionary name, then push back the object itself into the dictionary
	//word's section list (a.k.a "definition")
	while (cfgSection != NULL) {
		if (cfgEntry == NULL) {
			cfgSection = cfg.getNextConfigSection();
			if (cfgSection != NULL) {
				//push back an empty vector if a new section is discovered in the cfg file
				resources.addWord( cfgSection );
				if (resources.knowDefinition( cfgSection ) == false) {
					resources.setDefinition( cfgSection, std::vector< c_script >() );
				}
			}
			else {
				break;
			}
			cfgEntry = cfg.getFirstConfigEntry( cfgSection );
		}
		//if-statement fall-through
		if (cfgEntry != NULL ) {
			cfgValue = cfg.getConfigVal( cfgSection, cfgEntry);
			if ( cfgValue != NULL ) {
				resources.getDefinition( cfgSection )->push_back(
					getResourceFromString( cfgValue )
				);
				temp = &( resources.getDefinition( cfgSection )->back() );
				//im a terrible programmer
				temp->scriptName() = cfgEntry;
			}
			cfgEntry = cfg.getNextConfigEntry( cfgSection );
		}
	}
	return RESOURCE_LOAD_SUCCESS;
}
*/
} // end harbinger namespace
