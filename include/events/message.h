/* 
 * File:   message.h
 * Author: hammy
 *
 * Created on September 8, 2012, 3:31 PM
 */

#ifndef	__HGE_MESSAGE_H__
#define	__HGE_MESSAGE_H__
namespace harbinger {

//-----------------------------------------------------------------------------
//		Message (Envelope) Structures
//-----------------------------------------------------------------------------
template <typename message_t >
struct s_message {
	const message_t sender;
	int messageID;
	
	s_message() :
		sender(),
		messageID( 0 )
	{}
};

struct s_logMessage : s_message< void* > {
	std::string logStr;
};

typedef s_message< c_resource* > s_resourceMessage;

struct s_scriptMessage : s_message< c_scriptFuncBase* > {
	float timeInterval;
};

//-----------------------------------------------------------------------------
//		Messenger Class
//-----------------------------------------------------------------------------
template < typename message_t = void >
class c_messenger {
	protected:
		hamLibs::containers::queue< message_t > messageQueue;
	
	public:
		c_messenger			() {}
		c_messenger			( const c_messenger& ) = delete;
		virtual ~c_messenger	() = 0;
		
		c_messenger&	operator =	( const c_messenger& ) = delete;
		
		void			notify		( const message_t& );
		virtual void	clearInbox	();
		size_t		numMessages	() const;
		bool			hasMessages	() const;
		virtual void	readMessages	() = 0;
		/*
		 *	while (!messageQueue.empty()) {
		 *		//do something with the message
		 *		//messageQueue.pop();
		 *	}
		 */
};

template < typename message_t >
c_messenger< message_t >::~c_messenger() {}

template < typename message_t >
void c_messenger< message_t >::notify( const message_t& msgToSend ) {
	messageQueue.push( msgToSend );
}
template < typename message_t >
void c_messenger< message_t >::clearInbox() {
	messageQueue.clear();
}
template < typename message_t >
size_t c_messenger< message_t >::numMessages() const {
	return messageQueue.size();
}

template < typename message_t >
bool c_messenger< message_t >::hasMessages() const {
	return !messageQueue.empty();
}

//-----------------------------------------------------------------------------
//		Log Messenger Class
//-----------------------------------------------------------------------------
class c_logHandler : public c_messenger< s_logMessage > {
	enum e_logLogFlags {
		HGE_LOG_TO_CONSOLE	= 0x1,
		HGE_LOG_TO_FILE	= 0x2
	};
	
	enum e_logType {
		HGE_LOG_REGULAR,
		HGE_LOG_ALERT,
		HGE_LOG_WARNING,
		HGE_LOG_ERROR,
		
		HGE_LOG_MAX
	};
	
	private:
		e_logLogFlags logFlags;
		std::ofstream logStream;
		std::string logFile;
		
	public:
		c_logHandler		();
		~c_logHandler		();
		
		void setLogFlags	( e_logLogFlags );
		void setLogFile	( std::string& logFileName );
		const std::string&	getLogFile() const;
		
		void readMessages	();
		void logToStream	( std::ostream&, const std::string& );
};

//-----------------------------------------------------------------------------
//		Resource Handler, not a manager
//-----------------------------------------------------------------------------
class c_resourceHandler : public c_messenger< s_resourceMessage > {
	enum e_resourceTags {
		RES_LOAD,
		RES_UNLOAD
	};
	
	public:
		c_resourceHandler	() {}
		c_resourceHandler	( const c_resourceHandler& ) = delete;
		~c_resourceHandler	() { readMessages(); }
		
		c_resourceHandler& operator = ( const c_resourceHandler& ) = delete;
		
		void		clearInbox	() { readMessages(); }
		void		readMessages	();
};

} //end harbinger namespace
#endif	/* __HGE_MESSAGE_H__ */
