
//msgtest2.h

#ifndef __HGE_MESSAGE_H__
#define __HGE_MESSAGE_H__

#include <queue>

typedef void ( *callBack )();
typedef void ( *callBackList[] )();

class messenger;

class message {
	friend class messenger;
	
	public:
		const messenger* sender = nullptr;
		int header = 0;
		int body = 0;
		callBack func = nullptr;
		
		message( const message& msg ) : 
			sender( msg.sender ),
			header( msg.header ),
			body( msg.body ),
			func( msg.func )
		{}
		
		message(
			const messenger* msgSender,
			int msgHeader, int msgBody,
			callBack msgCB
		) :
			sender( msgSender ),
			header( msgHeader ),
			body( msgBody ),
			func( msgCB )
		{}
		
		~message() {
			sender = nullptr;
			header = 0;
			body = 0;
			func = nullptr;
		}
		
		message& operator = ( const message& msg ) {
			sender = msg.sender;
			header = msg.header;
			body = msg.body;
			func = msg.func;
			return *this;
		}
};

class messenger {
	private:
		std::queue< const message > inbox;
		
	public:
		messenger	() {}
		messenger	( const messenger& ) {}
		~messenger	() {}
		
		bool			hasMessages		() const	{ return inbox.size() > 0; }
		unsigned int	getNumMessages	() const	{ return inbox.size(); }
		const message	getMessage		() const	{ return inbox.size() ? inbox.front() : nullptr; }
		void			removeMessage	()			{ inbox.pop(); }
		void			clearInbox		()			{ while (inbox.size()) inbox.pop(); }
		
		void			sendMessage		( messenger& recipient, int msgHeader, int msgBody, const callBack cbFunc = nullptr )
										{ recipient.inbox.push( message( this, msgHeader, msgBody, cbFunc ) ); }
};

#endif /* __HGE_MESSAGE_H__ */
