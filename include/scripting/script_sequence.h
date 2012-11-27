/* 
 * File:   script_sequence.h
 * Author: hammy
 *
 * Created on November 4, 2012, 6:59 PM
 */

#ifndef __HGE_SCRIPT_SEQUENCE_H__
#define	__HGE_SCRIPT_SEQUENCE_H__
namespace harbinger {

class c_scriptSequence {
	private:
		//functions are run once per "run" command
		bool runOnce;
		std::list< c_scriptFuncBase* > scriptSeq;
		std::list< c_scriptFuncBase* >::iterator funcToRun;

	public:
		c_scriptSequence();
		c_scriptSequence(const c_scriptSequence& orig);
		virtual ~c_scriptSequence();
		
		void addToSequence( const c_scriptFuncBase& funcToAdd );
		void removeFromSequence( const c_scriptFuncBase& funcToRemove );
		
		void run();
		void tick();
};

} // end harbinger namespace
#endif	/* __HGE_SCRIPT_SEQUENCE_H__ */
