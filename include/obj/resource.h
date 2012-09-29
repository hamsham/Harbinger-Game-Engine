/* 
 * File:   resource.h
 * Author: hammy
 *
 * Created on September 9, 2012, 9:35 PM
 */

#ifndef	__HGE_RESOURCE_H__
#define	__HGE_RESOURCE_H__
namespace harbinger {

template <typename type>
class c_resourceManager {
	private:
		struct s_resource {
			std::string	fileName;
			std::string	filePath;
			std::string	fileType;
			type			data;
		};
		std::list<s_resource>	resourceList;
		std::list<s_resource>	resourceCopies;
		
	public:
		c_resourceManager() {};
		c_resourceManager( const c_resourceManager& mgrCopy ) = 0;
		virtual ~c_resourceManager() {};
		virtual c_resourceManager& operator = (const c_resourceManager& mgrCopy ) = 0;
		
		//Loading Data
		virtual bool		loadResource	( const char* fileName, const char* name = NULL ) = 0; // the name can be inferred by the filename
		virtual void		unloadResource	( const char* name ) = 0;
		
		//Resouce Acquisition
		type*			getResource	( const char* name ) const;
		type*			getNewInstance	( const char* name ) const;	//return a copy of a resource
		virtual void		killInstance	( const type* instance );	//destroy a copy of a resource
		virtual void		clearResources	();
};

//-----------------------------------------------------------------------------
//			Resource Acquisition
//-----------------------------------------------------------------------------
template <typename type>
type* c_resourceManager<type>::getResource(const char* name) const {
	typename std::list<s_resource>::iterator iter = resourceList.begin();
	while (iter != resourceList.end()) {
		if (iter->fileName == name)
			return iter->data;
		++iter;
	}
	return NULL;
}

template <typename type>
type* c_resourceManager<type>::getNewInstance(const char* name) const {
	typename std::list<s_resource>::iterator iter = resourceList.begin();
	while (iter != resourceList.end()) {
		if (iter->fileName == name) {
			resourceCopies.push_back( type() );
			return resourceCopies.back();
		}
		++iter;
	}
	return NULL;
}

template <typename type>
void c_resourceManager<type>::killInstance(const type* instance) {
	typename std::list<s_resource>::iterator iter = resourceCopies.begin();
	while (iter != resourceCopies.end()) {
		if (&(iter->data) == instance) {
			resourceCopies.erase( iter );
			return;
		}
	}
}

template <typename type>
void c_resourceManager<type>::clearResources() {
	resourceList.clear();
	resourceCopies.clear();
}

} // end harbinger namespace
#endif	/* __HGE_RESOURCE_H__ */
