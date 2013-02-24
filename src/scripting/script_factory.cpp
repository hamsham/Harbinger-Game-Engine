
#include "scripting/script.h"
#include "scripting/script_variables.h"
#include "scripting/script_functions.h"
#include "scripting/script_factory.h"

typedef std::map< int, c_script* (*)() > scriptFactoryMap_t;

#define REGISTERED_FACTORY( num, type )	\
	{ num, []() ->c_script* { return new( std::nothrow ) type(); } }

namespace {
	/*
	 * Variable Factories
	 */
	scriptFactoryMap_t varFactory = {
		REGISTERED_FACTORY( SCRIPT_VAR_INT,		c_varInt ),
		REGISTERED_FACTORY( SCRIPT_VAR_FLOAT,	c_varFloat ),
		REGISTERED_FACTORY( SCRIPT_VAR_BOOL,	c_varBool ),
		REGISTERED_FACTORY( SCRIPT_VAR_VEC3,	c_varVec3 ),
		REGISTERED_FACTORY( SCRIPT_VAR_STRING,	c_varString )
	};

	/*
	 * Function Factories
	 */
	scriptFactoryMap_t funcFactory = {
		REGISTERED_FACTORY( SCRIPT_FUNC_INT_ADD,	c_fncIntAdd ),
		REGISTERED_FACTORY( SCRIPT_FUNC_INT_SUB,	c_fncIntSub ),
		REGISTERED_FACTORY( SCRIPT_FUNC_INT_MUL,	c_fncIntMul ),
		REGISTERED_FACTORY( SCRIPT_FUNC_INT_DIV,	c_fncIntDiv ),
		REGISTERED_FACTORY( SCRIPT_FUNC_INT_MOD,	c_fncIntMod ),
		REGISTERED_FACTORY( SCRIPT_FUNC_INT_EQL,	c_fncIntEql ),

		REGISTERED_FACTORY( SCRIPT_FUNC_FLOAT_ADD,	c_fncFloatAdd ),
		REGISTERED_FACTORY( SCRIPT_FUNC_FLOAT_SUB,	c_fncFloatSub ),
		REGISTERED_FACTORY( SCRIPT_FUNC_FLOAT_MUL,	c_fncFloatMul ),
		REGISTERED_FACTORY( SCRIPT_FUNC_FLOAT_DIV,	c_fncFloatDiv ),
		REGISTERED_FACTORY( SCRIPT_FUNC_FLOAT_MOD,	c_fncFloatMod ),
		REGISTERED_FACTORY( SCRIPT_FUNC_FLOAT_EQL,	c_fncFloatEql ),

		REGISTERED_FACTORY( SCRIPT_FUNC_INT_CAST,	c_fncIntCast ),
		REGISTERED_FACTORY( SCRIPT_FUNC_FLOAT_CAST,	c_fncFloatCast ),

		REGISTERED_FACTORY( SCRIPT_FUNC_NUM_ROUND,	c_fncNumRound )
	};

#undef REGISTERED_FACTORY
} // end anonymous namespace

c_script* n_scriptFactory::getVarInstance( long scriptType ) {
	return varFactory[ scriptType ]();
}

c_script* n_scriptFactory::getFuncInstance( long scriptType ) {
	return funcFactory[ scriptType ]();
}

void c_scriptFactorykillInstance( c_script* script ) {
	delete script;
}
