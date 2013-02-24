/* 
 * File:   script_manager.h
 * Author: hammy
 *
 * Created on February 16, 2013, 9:48 PM
 */

#ifndef __HGE_SCRIPT_MANAGER_H__
#define	__HGE_SCRIPT_MANAGER_H__

#include <string>
#include "script.h"

///////////////////////////////////////////////////////////////////////////////
//		Scripting Manager
///////////////////////////////////////////////////////////////////////////////
class HGE_API c_scriptManager {
	private:
		std::string			scriptFile;
		scriptMap_t			scriptMap;
		scriptList_t		scriptList;
		
	public:
		c_scriptManager		();
		c_scriptManager		( const c_scriptManager& );
		~c_scriptManager	()				{ clearEntries(); }
		
		const std::string&	getFileName		() const;
		void				setFileName		( const std::string& );
		bool				hasFile			() const;
		
		bool				save			() const;
		bool				load			();
		
		const c_script*		operator[]		( uint ) const;
		c_script*			operator[]		( uint );
		
		void				addEntry		( c_script* ); // creates a 'new' instance
		void				manageEntry		( c_script* ); // adds an instance without using 'new'
		void				popEntry		( unsigned int index );
		
		scriptListSize_t	getNumEntries	() const { return scriptList.size(); }
		void				clearEntries	();
};

#endif /* __HGE_SCRIPT_MANAGER_H__ */
