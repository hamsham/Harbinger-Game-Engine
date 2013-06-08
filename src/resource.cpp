
#include <fstream>
#include <iostream>
#include "resource.h"
using namespace std;

namespace hge {

//-----------------------------------------------------------------------------
//	Error Messaging
//-----------------------------------------------------------------------------
void printFileError( const char* fileName ) {
	std::cerr << "Error opening the file:" << endl;
	std::cerr << "\t" << fileName << endl;
	std::cerr << "Please check that the file name was input correctly." << endl;
}

//-----------------------------------------------------------------------------
//	Resource - Static Members
//-----------------------------------------------------------------------------
/*
 * Check if a file exists
 */
bool resource::fileExists(const char* fileName) {
	ifstream fin(fileName);
	
	if (!fin.good())
		return false;
	
	fin.close();
	return true;
}

/*
 * Get the size (in bytes) of the file
 */
unsigned resource::fileSize(const char* fileName) {
	unsigned size(0);
	ifstream fin(fileName);
	if (!fin.good()) {
		printFileError(fileName);
		return 0;
	}
	
	fin.seekg(0, ios::end);
	size = fin.tellg();
	fin.close();
	return size;
}

/*
 * Read the file data into a buffer
 */
void resource::readFile(const char* filename, char* buffer, unsigned length) {
	ifstream fin(filename, ios_base::in | ios_base::binary);
	if (!fin.good()) {
		printFileError(filename);
		return;
	}
	fin.read(buffer, length);
	fin.close();
}

} // end hge namespace
