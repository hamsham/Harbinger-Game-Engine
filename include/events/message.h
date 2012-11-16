/* 
 * File:   s_message.h
 * Author: hammy
 *
 * Created on September 8, 2012, 3:31 PM
 */

#ifndef	__HGE_MESSAGE_H__
#define	__HGE_MESSAGE_H__
namespace harbinger {

//-----------------------------------------------------------------------------
//		Message (Envelope) Structure
//-----------------------------------------------------------------------------
template <typename messenger_type = void* >
struct s_message {
	messenger_type& sender;
	int messageID = 0;
	
	s_message	() {}
	s_message	( const s_message& msgCopy );
	s_message	( const messenger_type& messenger, int messageEnum );
	~s_message() {}
	s_message& operator = ( const s_message& msgCopy );
};

template <typename messenger_type> HGE_INLINE
s_message< messenger_type >::s_message(const s_message& msgCopy) :
	sender	( msgCopy.sender ),
	messageID	( msgCopy.messageID )
{}

template <typename messenger_type> HGE_INLINE
s_message< messenger_type >::s_message( const messenger_type& messenger, int messageToSend ) :
	sender	( messenger ),
	messageID	( messageToSend )
{}

template <typename messenger_type> HGE_INLINE
s_message< messenger_type >& s_message< messenger_type >::operator =( const s_message& msgCopy ) {
	sender	= msgCopy.sender;
	messageID	= msgCopy.messageID;
}

//-----------------------------------------------------------------------------
//		Messenger Class
//-----------------------------------------------------------------------------
template <typename messenger_type = void* >
class c_messenger {
	protected:
		std::queue< s_message< messenger_type > > messageQueue;
	
	public:
		c_messenger			();
		c_messenger			(const c_messenger& messengerCopy);
		virtual ~c_messenger	() = 0;
		void sendMessage		( c_messenger& recipient, int messageEnum ) const;
		virtual void readMessages() = 0;
		/*	while (messageStack.front()) {
		 * 		//do something with the message
		 * 		//messageStack.pop();
		 * 	}
		 */
};

template <typename messenger_type>
c_messenger< messenger_type >::c_messenger() {}

template <typename messenger_type>
c_messenger< messenger_type >::c_messenger(const c_messenger& messengerCopy) {}

template <typename messenger_type>
c_messenger< messenger_type >::~c_messenger() {}

template <typename messenger_type>
void c_messenger< messenger_type >::sendMessage(c_messenger& recipient, int messageID) const {
	recipient.messageQueue.push(
		s_message< messenger_type >(this, messageID)
	);
}

} //end harbinger namespace
#endif	/* __HGE_MESSAGE_H__ */
