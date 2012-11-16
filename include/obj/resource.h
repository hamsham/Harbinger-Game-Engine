/* 
 * File:   resource.h
 * Author: hammy
 *
 * Created on November 4, 2012, 9:21 PM
 */

#ifndef RESOURCE_H
#define	RESOURCE_H

typedef void* resourcePtr;

struct s_resource {
	resourcePtr resource;
	std::string resourceId;
	
	s_resource() :
		resource( NULL )
	{
		std::ostringstream stout;
		stout << (unsigned long)this;
		resourceId = stout.str();
		//convert ascii number into letters for use with the dictionary container
		for (int i = resourceId.size(); i >= 0; --i )
			resourceId[ i ] += ( 'a' - '0' );
		stout.flush();
	}
};

class c_resourceMgr {
	private:
		bool isLoaded;
		std::string resourceFile;
		hamLibs::containers::dictionary< s_resource* >resDict;
		
	public:
		c_resourceMgr();
		c_resourceMgr(const c_resourceMgr& orig);
		virtual ~c_resourceMgr() = 0;
		
		void setResourceFileName( const char* fileName );
		const char* getResourceFileName() const;
		
		virtual void copyResource	( const resourcePtr resourceToAdd ) = 0;
		virtual void removeResource	( const resourcePtr resourceToRemove ) = 0;
		virtual void manageResource	( resourcePtr resourceToManage ) = 0;
		virtual void unManageResource	( resourcePtr resourceToUnmanage ) = 0;
		
		virtual bool saveResourceList	( bool overwrite = true ) = 0;
		virtual bool loadResourceList	() = 0;
};

#endif	/* RESOURCE_H */

