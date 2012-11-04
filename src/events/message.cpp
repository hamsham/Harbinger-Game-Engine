/* 
 * File:   message.cpp
 * Author: hammy
 * 
 * Created on September 8, 2012, 3:31 PM
 */

#include "../../include/harbinger.h"

namespace harbinger {

//-----------------------------------------------------------------------------
//		Message (Envelope) Structure
//-----------------------------------------------------------------------------
c_message::c_message() :
	sender	(0),
	messageID	(0)
{}

c_message::c_message(const c_message& msgCopy) :
	sender		(msgCopy.sender),
	messageID	(msgCopy.messageID)
{}

c_message::c_message( const c_messenger* messenger, int messageToSend ) :
	sender		(const_cast<c_messenger*>(messenger)),
	messageID	(messageToSend)
{}

c_message::~c_message() {
	sender = 0;
}

c_message& c_message::operator =(const c_message& msgCopy) {
	sender	= msgCopy.sender;
	messageID	= msgCopy.messageID;
}

//-----------------------------------------------------------------------------
//		Messenger Class
//-----------------------------------------------------------------------------
c_messenger::c_messenger() {}
c_messenger::c_messenger(const c_messenger& messengerCopy) {}

void c_messenger::sendMessage(c_messenger& recipient, int messageID) const {
	recipient.messageQueue.push( c_message(this, messageID) );
}

} //end harbinger namespace