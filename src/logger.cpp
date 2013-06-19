
#include <ctime>
#include <cstdarg>

#include "../include/utils/logger.h"

namespace hamLibs {
namespace utils {

logger::logger() :
	logFile(nullptr)
{}
	
logger::logger(std::ostream* streamToLogWith) :
	logFile(streamToLogWith)
{}

void logger::setLogStream(std::ostream* streamToUse) {
	logFile = streamToUse;
}

bool logger::getLogState() const {
	return isLogging;
}

void logger::setLogState(bool state) {
	isLogging = state;
}

void logger::log(const char* info2Log, ...) {
	if (logFile == nullptr || isLogging == false) return;
	std::ostream& output = *logFile;
	
	va_list args;
	va_start(args, info2Log);
	output << "\t" << std::clock() << " -- ";
	output << info2Log << args;
	output << "\n";
	va_end(args);
	output.flush();
}

} //end utils namespace
} //end hamlibs namespace
