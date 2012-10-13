/* 
 * File:   resource.h
 * Author: hammy
 *
 * Created on September 9, 2012, 9:35 PM
 */

#ifndef	__HGE_RESOURCE_H__
#define	__HGE_RESOURCE_H__
namespace harbinger {

template < typename type = c_script >
class c_resourceManager {
	protected:
		std::vector< type > resourceArray;
		
	public://construction & destruction
		c_resourceManager() {}
		c_resourceManager( const c_resourceManager& mgrCopy ) {}
		virtual ~c_resourceManager() = 0;
		c_resourceManager& operator = (const c_resourceManager& mgrCopy ) {}
		
		//serialization
		virtual bool save ( const char* configFile );
		virtual bool load ( const char* configFile );
		
		//direct access
		type operator[] ( size_t index ) const;
		type& operator[] ( size_t index );
		size_t getNumObjects() const;
		
		//meat & potatoes
		virtual size_t addObject ( const type& objToAdd ); //manage an object
		virtual void removeObject ( size_t objID );
};

class c_FunctionManager : public c_resourceManager< c_script > {
	private:
		hamLibs::containers::dictionary scriptDict;
		
	public:
		c_scriptManager() {}
		c_scriptManager( const c_scriptManager& smCopy ) {}
		virtual ~c_scriptManager() {}
};

} // end harbinger namespace
#endif	/* __HGE_RESOURCE_H__ */
