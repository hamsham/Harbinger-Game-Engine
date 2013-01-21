/* 
 * File:   resource.h
 * Author: hammy
 *
 * Created on November 4, 2012, 9:21 PM
 */

#ifndef __HGE_SCRIPT_MGR_H__
#define	__HGE_SCRIPT_MGR_H__

#include "script.h"
#include <string>
#include <fstream>

//-----------------------------------------------------------------------------
//		Script Resource Variable Base Class
//-----------------------------------------------------------------------------
class c_resource : virtual public c_scriptVarBase {
	protected:
		std::string resFile;
		
	public:
		enum e_resourceType : signed int {
			RES_INVALID = SCRIPT_INVALID,
			RES_GENERAL = 0,
			RES_BITMAP,
			RES_SPRITE,
			RES_AUDIO,
			RES_AUDIO_CLIP,
			RES_VIDEO,
			RES_VIDEO_CLIP,
			RES_MESH,
			RES_SKELETON,
			RES_ANIMATION,
			RES_SHADER,

			RES_MAX
		};
	
		c_resource() {}
		c_resource( const std::string& );
		c_resource( const c_resource& );
		~c_resource() {}
		virtual c_resource& operator = ( const c_resource& ) = 0;
		
		virtual e_resourceType getResourceType() const {
			return RES_INVALID;
		}
		void read		( std::ifstream&, scriptMap_t& );
		void write	( std::ofstream& ) const;
		
		// Regarding "setFile(...)", the resource will be reloaded if:
		// A: The new filename is different  than the current one
		// B: The current resource is loaded as per the "isLoaded" function
		// Reloading 
		bool				setFile ( const std::string& );
		const std::string&	getFile () const;
		
		virtual bool	load		() = 0;
		virtual void	unload	() = 0;
		bool			reload	() { unload(); return load(); }
		virtual bool	isLoaded	() const;
};

//-----------------------------------------------------------------------------
//		Script Management
//-----------------------------------------------------------------------------
class c_scriptManager {
	enum e_managementTasks {
		MGR_MANAGE,
		MGR_UNMANAGE,
		MGR_RESOURCE_LOAD,
		MGR_RESOURCE_UNLOAD
	};
	
	private:
		bool isLoaded = false;
		std::string scriptFileName;
		scriptList_t resourceList;
		
	public:
		c_scriptManager() {}
		c_scriptManager( const c_scriptManager& ) = delete;
		~c_scriptManager();
		
		void setFilename( const std::string& fileName );
		const std::string& getFileName() const;
		
		bool loadScripts();
		void unloadScripts();
		bool saveScripts();
		
		bool loadResources( c_resource::e_resourceType = c_resource::RES_MAX );
		void unloadResources( c_resource::e_resourceType = c_resource::RES_MAX );
};

#endif	/* __HGE_SCRIPT_MGR_H__ */
