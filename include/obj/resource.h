/* 
 * File:   resource.h
 * Author: hammy
 *
 * Created on September 9, 2012, 9:35 PM
 */

#ifndef	__HGE_RESOURCE_H__
#define	__HGE_RESOURCE_H__
namespace harbinger {

//-----------------------------------------------------------------------------
//		Resource class
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//		Resource Manager base class
//-----------------------------------------------------------------------------
template < typename type >
class c_resourceManager {
	private:
		//resource structure
		struct s_resource {
			type* resource;
			std::string resourceId;
			
			s_resource() : resource( NULL ) { setIdStr(); }
			
			inline void setIdStr() {
				std::ostringstream stout;
				//use the address of the object as its ID
				stout << (unsigned long int)&resource;
				resourceId = stout.str();
				//convert the pointer/ID of the object into an alphabetical string
				for ( int i( 0 ); i < resourceId.size(); ++i )
					resourceId[ i ] += ('a' - '0');
			}
		}; // end resource structure
		
	protected:
		hamLibs::containers::dictionary< type* > referenceDict;
		std::vector< s_resource > resourceArray;
		std::string resourceFile;
		
	public:
		//construction & destruction
		c_resourceManager();
		c_resourceManager( const c_resourceManager& mgrCopy );
		virtual ~c_resourceManager() = 0;
		virtual c_resourceManager& operator = (const c_resourceManager& mgrCopy ) = 0;
		
		//object access
		type* getResource( const char* resourceName );
		type& operator[] ( size_t index );
		type operator[] ( size_t index ) const;
		const char* getResourceId( size_t index ) const;
		
		//utilities
		size_t numResources() const;
		size_t maxResources() const;
		
		//meat & potatoes
		virtual const char* addResource ( const type& objToAdd ); // makes a copy
		virtual void removeObject ( const char* objID );
		virtual void clear();
		
		//serialization
		void setResourceFile( const char* file );
		const char* getResourceFile() const;
		virtual serialization::e_fileStatus save() const = 0;
		virtual serialization::e_fileStatus load() = 0;
};

//construction & destruction
template <typename type>
c_resourceManager<type>::c_resourceManager() {}

template <typename type>
c_resourceManager<type>::c_resourceManager( const c_resourceManager& rmCopy ) {
	*this = rmCopy;
}

template <typename type>
c_resourceManager<type>::~c_resourceManager() {}

//element access
template <typename type>
type c_resourceManager<type>::operator [] ( size_t index ) const {
	assert( index < resourceArray.size() );
	return *resourceArray[ index ]->resource;
}

template <typename type>
type& c_resourceManager<type>::operator [] ( size_t index ) {
	assert( index < resourceArray.size() );
	return *resourceArray[ index ]->resource;
}

template <typename type>
type* c_resourceManager<type>::getResource ( const char* resourceName ) {
	//CAUTION: returns NULL if "resourceName" doesn't exist within the dictionary
	type** resRef = referenceDict.getDefinition( resourceName );
	return ( resRef != NULL ) ? *resRef : NULL;
}

template <typename type>
size_t c_resourceManager<type>::numResources() const {
	return resourceArray.size();
}

template <typename type>
size_t c_resourceManager<type>::maxResources() const {
	return resourceArray.max_size();
}

//object addition & removal
template <typename type>
const char* c_resourceManager<type>::addResource( const type& objToAdd ) {
	try {
		resourceArray.reserve( resourceArray.size()+1 );
	}
	catch ( std::length_error& vecMemErr ) {
		std::cerr
			<< "ERROR: Maximum resource limit reached for " << vecMemErr.what()
			<< " in Resource Manager Object " << this << std::endl;
		return NULL;
	}
	catch ( std::bad_alloc& vecMemErr ) {
		std::cerr
			<< "ERROR: Unable to reallocate memory for " << vecMemErr.what()
			<< " in Resource Manager Object " << this << std::endl;
		return NULL;
	}
	
	s_resource& resRef = resourceArray.back();
	resRef.resource = new type( objToAdd );
	referenceDict.addWord( resRef.resourceId.c_str() );
	referenceDict.setDefinition( resRef.resourceId.c_str(), resRef.resource );
	return resRef.resourceId.c_str();
}

template <typename type>
void c_resourceManager<type>::removeObject( const char* objID ) {
	type** objToRemove = referenceDict.getDefinition( objID );
	if ( objToRemove == NULL || *objToRemove == NULL )
		return;
	
	typename std::vector< s_resource >::iterator iter;
	iter = resourceArray.begin();
	while ( iter != resourceArray.end() ) {
		if ( iter->resource == *objToRemove ) {
			delete *objToRemove;
			*objToRemove = NULL;
			resourceArray.erase( iter );
		}
		++iter;
	}
	
	referenceDict.deleteWord( objID );
}

template <typename type>
void c_resourceManager<type>::clear() {
	for (size_t i( 0 ); i < resourceArray.size(); ++i)
		delete resourceArray[ i ].resource;
	referenceDict.clear();
	resourceArray.clear();
}

//serializations
template <typename type>
void c_resourceManager<type>::setResourceFile( const char* file ) {
	resourceFile = file;
}

template <typename type>
const char* c_resourceManager<type>::getResourceFile() const {
	return resourceFile.c_str();
}

//-----------------------------------------------------------------------------
//		Scripting Manager
//-----------------------------------------------------------------------------
class c_scriptManager : public c_resourceManager< c_script > {
	//This class uses dynamic memory. Ensure that all objects are removed from
	// the array using the "delete" operator in order to avoid memory leaks.
	//dynamic memory was chosen so that the manager could hold pointers to
	//non-base-class script types and make better use of virtual/derived types
	static const char* scriptVarCfgSection;
	static const char* scriptFuncCfgSection;
	
	public:
		c_scriptManager() {}
		c_scriptManager( const c_scriptManager& smCopy );
		~c_scriptManager() { clear(); }
		
		c_scriptManager& operator = ( const c_scriptManager& smCopy );
		void clear();
		
		//serialization
		serialization::e_fileStatus save() const;
		serialization::e_fileStatus load();
};

} // end harbinger namespace
#endif	/* __HGE_RESOURCE_H__ */
