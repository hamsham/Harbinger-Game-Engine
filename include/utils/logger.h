
#ifndef __HL_LOGGER_H__
#define __HL_LOGGER_H__

#include <ostream>

namespace hamLibs {
namespace utils {

class logger {
    private:
        std::ostream* logFile;
        bool isLogging;

    public:
        logger      ();
        logger      (std::ostream* streamToLogWith);
        ~logger     () {}
        
        void        setLogStream    (std::ostream* streamToUse);
        bool        getLogState     () const;
        void        setLogState     (bool state);
        void        log             (const char* info2Log, ...);
};

} // end utils namespace
} //end hamlibs namespace

#endif /*__LOGGER_H__*/
