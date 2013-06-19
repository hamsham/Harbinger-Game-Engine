
#include <locale>
#include "../include/utils/hash.h"

namespace hamLibs {
namespace utils {

long int runtimeHash( const std::string& s ) {
	const std::collate< char >& coll = std::use_facet< std::collate<char> >( std::locale() );
	return coll.hash( s.c_str(), s.c_str()+s.size() );
}

} // end utils namespace
} // end hamlibs namespace
