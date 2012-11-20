/* 
 * File:   message.cpp
 * Author: hammy
 *
 * Created on November 17, 2012, 6:12 PM
 */
#include "../../include/harbinger.h"
namespace harbinger {

//-----------------------------------------------------------------------------
//		Log Handler Class
//-----------------------------------------------------------------------------
c_logHandler::c_logHandler() :
	logFlags(( e_logLogFlags )0)
{}
	
c_logHandler::~c_logHandler() {
	if (logStream.is_open())
		logStream.close();
}

void c_logHandler::setLogFlags( c_logHandler::e_logLogFlags inFlags ) {
	HGE_ASSERT( (inFlags >= 0) && (inFlags <= HGE_LOG_TO_FILE) );
	logFlags = inFlags;
	
	if ( (inFlags | HGE_LOG_TO_FILE) == 0 && logStream.is_open() ) {
		logStream.close();
	}
	else if ( (inFlags | HGE_LOG_TO_FILE) != 0 && logStream.is_open() == false ) {
		if ( logFile.size() > 0 ) //open a log file only if it was defined
			logStream.open( logFile.c_str(), std::ios_base::trunc );
	}
	//don't do anything if file logging file wasn't requested and the log stream is closed
	//don't do anything if the log stream is open and the inFlags requires file logging
}

void c_logHandler::setLogFile( std::string& logFileName ) {
	logFile = logFileName;
}

const std::string& c_logHandler::getLogFile() const {
	return logFile;
}

void c_logHandler::readMessages() {
	if ( logFlags == 0 ) {
		clearInbox();
		return;
	}
	std::ostringstream output;
	output.setf( std::ios::fixed, std::ios::floatfield );
	s_logMessage* currMessage( NULL );
	
	while ( !messageQueue.empty() ) {
		currMessage = messageQueue.front();
		
		switch( currMessage->messageID ) {
			case HGE_LOG_REGULAR:
				output << "LOG";
				break;
			case HGE_LOG_ALERT:
				output << "ALERT";
				break;
			case HGE_LOG_WARNING:
				output << "WARNING";
				break;
			case HGE_LOG_ERROR:
				output << "ERROR";
				break;
			default:
				output << "UNKNOWN";
				break;
		}
		
		output
			<< " - "
			<< system::timeSinceStart()
			<< " "
			<< currMessage->sender
			<< "\n\t" << currMessage->logStr.c_str() << std::endl;
		
		//send the current output to whichever buffers are activated
		if ( logFlags | HGE_LOG_TO_CONSOLE )
			logToStream( std::cout, output.str() );
		
		if ( (logFlags | HGE_LOG_TO_FILE) && logStream.good() ) {
			logToStream( logStream, output.str() );
		}
		output.flush();
		messageQueue.pop();
	}
}

void c_logHandler::logToStream( std::ostream& outStream, const std::string& str ) {
	outStream << str.c_str() << std::endl;
}

//-----------------------------------------------------------------------------
//		Resource Handler
//-----------------------------------------------------------------------------
void c_resourceHandler::readMessages() {
	s_resourceMessage* message( NULL );
	
	while ( !messageQueue.empty() ) {
		message = messageQueue.front();
		if ( message == NULL ) {
			messageQueue.pop();
			continue;
		}
		
		if (message->messageID == RES_LOAD)
			message->sender->load();
		
		else if (message->messageID == RES_UNLOAD)
			message->sender->unload();
		
		messageQueue.pop();
	}
}

}
