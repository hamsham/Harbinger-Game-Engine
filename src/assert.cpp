
#include <iostream>
#include "../include/utils/assert.h"

void hamLibs::utils::assert_runtime( bool condition, errorType type, const char* msg ) {
	if ( condition ) return;

	const char* errorString[] = { "ALERT: ", "WARNING: ", "ERROR: " };
	std::ostream& stream = ( type > ALERT ) ? std::cerr : std::cout;
	
	stream << errorString[ type ] << msg << std::endl;

	if ( type == ERROR ) throw ERROR;
}
