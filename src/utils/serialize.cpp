/* 
 * File:   serialize.cpp
 * Author: hammy
 * 
 * Created on September 9, 2012, 2:20 PM
 */

#include "../../include/harbinger.h"
using namespace hamLibs;


namespace harbinger {
	namespace serialize {

//private stuff. bwahaha
namespace {
	const char* CFG_COMMENT =
		"Configuration file created using the Harbinger Game Engine "\
		"and the Allegro 5 Multimedia library.";
	config::configLoadStatus loadVariables(
		config& cfg,
		std::vector< c_script>& scriptArray
	);
	config::configLoadStatus loadFunctions(
		config& cfg,
		std::vector< c_script>& scriptArray
	);
}
		
//-------------------------------------------------------------------
//		Saving Routines
//-------------------------------------------------------------------
config::configSaveStatus saveToConfig(
	const char* cfgFile,
	const std::vector< c_script >& scriptArray
) {
	config cfg;
	//sanity check
	cfg.addComment( NULL, CFG_COMMENT );
	if ( cfg.saveConfigFile( cfgFile, true ) != config::SAVE_SUCCESS )
		return config::SAVE_IO_ERROR;
	
	e_scriptType scrType( SCRIPT_BASE );
	const char* cfgSection( NULL );
	
	for ( size_t iter(0); iter < scriptArray.size(); ++iter ) {
		
		//acquire a pointer to the next element in the array
		scrType = scriptArray[ iter ].getScriptType();
		
		//determine the script type to save
		if ( scrType | SCRIPT_VAR ) {
			cfgSection = c_scriptVar.getScriptTypeStr();
		}
		else if ( scrType | SCRIPT_FUNC ) {
			cfgSection = c_scriptFunc.getScriptTypeStr();
		}
		else {
			//ignore unknown types (version control)
			continue;
		}
		
		//save the data
		cfg.setConfigVal(
			cfgSection,
			scriptArray[ iter ].getName.c_str(),
			scriptArray[ iter ].toString().c_str()
		);
		//loop again
	}

	return cfg.saveConfigFile();
}

//-------------------------------------------------------------------
//		General Loading Routine
//-------------------------------------------------------------------
config::configLoadStatus loadFromConfig(
	const char* cfgFile,
	std::vector< c_script >& scriptArray
) {
	config cfg;
	config::configLoadStatus loadState;
	loadState = cfg.loadConfigFile( cfgFile );
	if ( loadState != config::LOAD_SUCCESS )
		return loadState;
	
	loadState = loadVariables( scriptArray );
	if ( loadState != config::LOAD_SUCCESS )
		return loadState;
	else
		return loadFunctions( scriptArray );
}

//-------------------------------------------------------------------
//		Variable Loading Routine
//-------------------------------------------------------------------
config::configLoadStatus loadVariables(
	config& cfg,
	std::vector< c_script >& scriptArray
) {
	const char* cfgSection = c_scriptVar.getScriptTypeStr();
	const char* cfgEntry = cfg.getFirstConfigEntry( cfgSection )
	const char* cfgValue( NULL );
	std::string scriptType;
	std::string scriptData;
	
	while ( cfgEntry != NULL ) {
		
		cfgValue = cfg.getConfigVal( cfgSection, cfgEntry );
		
		//find the script type (best I could do without reference material)
		for (size_t i( 0 ); cfgValue[ i ] != NULL; ++i) {
			if ( cfgValue[ i ] == ' ' ) {
				scriptType.append( cfgValue, i );
				break;
			}
		}
		//integers
		if ( scriptType == c_scriptInt.getScriptTypeStr() )
			scriptArray.push_back( c_scriptInt() );
		
		//floats
		else if ( scriptType == c_scriptFloat.getScriptTypeStr() )
			scriptArray.push_back( c_scriptFloat() );
		
		//unsigned integers
		else if ( scriptType == c_scriptUint.getScriptTypeStr() )
			scriptArray.push_back( c_scriptUint() );
		
		//do nothing if an unknown value is found
		else
			continue;
		
		//add the data to the loaded variable
		scriptData = cfgValue;
		if ( scriptArray.back().fromString( scriptData ) == false)
			return config::LOAD_IO_ERROR;
	}
	
	return config::LOAD_SUCCESS;
}

//-------------------------------------------------------------------
//		Function Loading Routine
//-------------------------------------------------------------------
config::configLoadStatus loadFunctions(
	config& cfg,
	std::vector< c_script >& scriptArray
) {
	const char* cfgSection = c_scriptVar.getScriptTypeStr();
	const char* cfgEntry = cfg.getFirstConfigEntry( cfgSection )
	const char* cfgValue( NULL );
	std::string scriptType;
	std::string scriptData;
	
	while ( cfgEntry != NULL ) {
		
		cfgValue = cfg.getConfigVal( cfgSection, cfgEntry );
		
		//find the script type (best I could do without reference material)
		for (size_t i( 0 ); cfgValue[ i ] != NULL; ++i) {
			if ( cfgValue[ i ] == ' ' ) {
				scriptType.append( cfgValue, i );
				break;
			}
		}
		//Boolean Evaluations
		if ( scriptType == c_scriptNumEval.getScriptTypeStr() )
			scriptArray.push_back( c_scriptNumEval() );
		
		//Mathematical Operations
		else if ( scriptType == c_scriptMiscMath.getScriptTypeStr() )
			scriptArray.push_back( c_scriptMiscMath() );
		
		//Arithmetic
		else if ( scriptType == c_scriptArithmetic.getScriptTypeStr() )
			scriptArray.push_back( c_scriptArithmetic() );
		
		//Trigonometry
		else if ( scriptType == c_scriptTrigonometry.getScriptTypeStr() )
			scriptArray.push_back( c_scriptTrigonometry() );
		
		//do nothing if an unknown value is found
		else 
			continue;
		
		//add the data to the loaded variable
		scriptData = cfgValue;
		if ( scriptArray.back().fromString( scriptData ) == false)
			return config::LOAD_IO_ERROR;
	}
	
	return config::LOAD_SUCCESS;
}

	} // end serialize namespace
} // end harbinger namespace
