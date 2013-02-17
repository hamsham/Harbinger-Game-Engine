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
#include "script_serializer.h"

///////////////////////////////////////////////////////////////////////////////
//		Scripting Manager
///////////////////////////////////////////////////////////////////////////////
class HGE_API c_scriptManager {
	private:
		std::string			scriptFile;
		scriptList_t		scriptList;
		
	public:
		c_scriptManager		() {}
		c_scriptManager		( const c_scriptManager& );
		~c_scriptManager	()				{ clearEntries(); }
		
		static c_script*	getVarInstance	( int scriptVarType );
		static c_script*	getFuncInstance	( int scriptFuncType );
		static void			killInstance	( c_script* );
		
		const std::string&	getFileName		() const;
		void				setFileName		( const std::string& );
		bool				hasFile			();
		
		bool				save			( const char* filename ) const;
		bool				load			( const char* filename );
		
		const c_script*		operator[]		( unsigned int ) const;
		c_script*			operator[]		( unsigned int );
		
		void				addEntry		( c_script* ); // creates a 'new' instance
		void				manageEntry		( c_script* ); // adds an instance without using 'new'
		void				popEntry		( unsigned int index );
		
		scriptListSize_t	getNumEntries	() const { return scriptList.size(); }
		void				clearEntries	();
};

#endif /* __HGE_SCRIPT_MANAGER_H__ */
