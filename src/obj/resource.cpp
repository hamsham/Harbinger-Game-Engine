/* 
 * File:   resource.cpp
 * Author: hammy
 * 
 * Created on November 4, 2012, 9:21 PM
 */

#include "../../include/harbinger.h"

c_resourceMgr::c_resourceMgr() :
	isLoaded( false )
{}

c_resourceMgr::c_resourceMgr(const c_resourceMgr& orig) :
	isLoaded( NULL ),
	resourceFile( orig.resourceFile ),
	resDict( orig.resDict )
{}

c_resourceMgr::~c_resourceMgr() {}

