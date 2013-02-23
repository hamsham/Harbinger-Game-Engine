/* 
 * File:   test_serialize.cpp
 * Author: hammy
 *
 * Created on Feb 17, 2013, 7:12:45 PM
 */

#include <iostream>
#include <string>

#include "scripting/script.h"
#include "scripting/script_base.h"
#include "scripting/script_serializer.h"
#include "scripting/script_manager.h"
#include "scripting/script_functions.h"

void test1( c_scriptManager& sm, const std::string& file ) {
	sm.setFileName( file );
	std::cout << "Serialization Test #1\n\tSaving Data...";
	if ( !sm.save() ) {
		std::cout << "Failed" << std::endl;
		return;
	}
	std::cout << "Success" << std::endl;
}

void test2( c_scriptManager& sm, const std::string& file  ) {
	sm.setFileName( file );
	std::cout << "Serialization Test #2\n\tLoading Data...";
	if ( !sm.load() ) {
		std::cout << "Failed" << std::endl;
		return;
	}
	std::cout << "Success" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
//		Main
///////////////////////////////////////////////////////////////////////////////
int main() {
	
	std::cout << "Creating Objects...";
	c_scriptManager scriptManager;
	c_varFloat* testNumA = new c_varFloat;
	c_varFloat* testNumB = new c_varFloat;
	c_varFloat* testNumC = new c_varFloat;
	c_fncFloatMod* mathFunc = new c_fncFloatMod;
	std::cout << "Done\n";
	
	std::cout << "Adding Objects to the manager...";
	scriptManager.manageEntry( testNumA );
	scriptManager.manageEntry( testNumB );
	scriptManager.manageEntry( testNumC );
	scriptManager.manageEntry( mathFunc );
	std::cout << "Done\n";
	
	std::cout << "Setting Object values...";
	testNumA->data = 42.f;
	testNumB->data = 7.7f;
	testNumC->data = 0.f;
	std::cout << "Done\n";
	
	std::cout << "Saving to the first file...";
	test1( scriptManager, "testScriptFile.hsd");
	std::cout << "\n";
	
	std::cout << "Setting up the test function...";
	mathFunc->attachArg( 0, *testNumA );
	mathFunc->attachArg( 1, *testNumB );
	mathFunc->attachRet( *testNumC );
	mathFunc->run();
	std::cout << "Done\n";
	
	std::cout << "Saving to the second file...";
	test1( scriptManager, "testScriptFile2.hsd");
	std::cout << "Done\n";
	
	std::cout << "Unloading data from the manager...";
	scriptManager.clearEntries();
	std::cout << "Done\n";
	
	std::cout << "Loading from the second file...";
	test2( scriptManager, "testScriptFile2.hsd");
	std::cout << "Done.";
	
	return 0;
}
