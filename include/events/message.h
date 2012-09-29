/* 
 * File:   c_message.h
 * Author: hammy
 *
 * Created on September 8, 2012, 3:31 PM
 */

#ifndef	__HGE_MESSAGE_H__
#define	__HGE_MESSAGE_H__
namespace harbinger {

class c_message {
	friend class c_messenger;
	private:
		c_messenger* sender;
		int messageID;
	public:
		c_message	();
		c_message	( const c_message& msgCopy );
		c_message	( const c_messenger* messenger, int messageToSend );
		~c_message();
		c_message& operator = ( const c_message& msgCopy );
};

class c_messenger {
	friend class c_message;
	private:
		std::queue< c_message > messageQueue;
	
	public:
		c_messenger			();
		c_messenger			(const c_messenger& messengerCopy);
		virtual ~c_messenger	() {}
		void sendMessage		(c_messenger& recipient, int messageEnum) const;
		virtual void readMessages() = 0;
		/*	while (messageStack.front()) {
		 * 		//do something with the message
		 * 		//messageStack.pop();
		 * 	}
		 */
};

} //end harbinger namespace
#endif	/* __HGE_MESSAGE_H__ */
