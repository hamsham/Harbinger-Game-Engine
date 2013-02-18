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
#include "scripting/script_variables.h"

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

int main() {
	c_scriptFloat*			testFloatA( new c_scriptFloat );
	c_scriptFloat*			testFloatB( new c_scriptFloat );
	c_scriptFloat*			testFloatC( new c_scriptFloat );
	c_scriptString*			testString( new c_scriptString );
	c_scriptArithmetic*		testArithmetic( new c_scriptArithmetic );
	c_scriptManager			scriptManager;
	
	scriptManager.manageEntry( testFloatA );
	scriptManager.manageEntry( testFloatB );
	scriptManager.manageEntry( testFloatC );
	scriptManager.manageEntry( testString );
	scriptManager.manageEntry( testArithmetic );
	
	testFloatA->varData() = 4.2f;
	testFloatB->varData() = 7.7f;
	testFloatC->varData() = 0.f;
	testString->varData() = "Hello World!";
	
	testArithmetic->setEvalType( c_scriptArithmetic::MOD );
	testArithmetic->setVarToCompare( testFloatA );
	testArithmetic->setVarToEvaluate( testFloatB );
	testArithmetic->setReturnVal( testFloatC );
	testArithmetic->run();
	
	test1( scriptManager, "testScriptFile.hsd" );
	scriptManager.clearEntries();
	test2( scriptManager, "testScriptFile.hsd" );
	
	test1( scriptManager, "testScriptFile2.hsd" );
	scriptManager.clearEntries();
	test2( scriptManager, "testScriptFile2.hsd" );
	
	std::cin.get();
	
	return 0;
}
