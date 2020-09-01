#ifndef LUALINK_UTILS_H
#define LUALINK_UTILS_H

#include <string>
#include <vector>
#include <iostream>
#include <iterator>

//DEBUG Flag
#define $DEBUG false

class utils {
  private:
    static int ERROR_CODE;
  
  public:
    struct errorCodes {
        #define ECODE static const int
    
        ECODE INVALID_ARGS    =     -210;   //Invalid arguments
        ECODE NO_RUNTIME      =     -220;   //No Lua runtime could be found
        ECODE INVALID_RUNTIME =     -230;   //The given lua interpreter path is missing, invalid, or unusable
        ECODE NO_SCRIPT       =     -240;   //No script to execute could be found
        ECODE MISSING_SCRIPT  =     -250;   //script.lua not on disk, but was specified.
    };
    
    static int error(int errorCode, std::vector<std::string> texts);
    static std::string combineArgs(int length, char** args);
    static bool stringEndsWith(std::string const & value, std::string const & ending);
    static std::string getEnvVar(std::string const & key);
    static bool startsWithCapital(const std::string&);
    static bool endsWithCapital(const std::string&);
    static bool isCapital(const char &);
    
    static inline int getError() {
        return ERROR_CODE;
    }
    
    static inline bool doesFileExist (const std::string& fileAsPath) {
        struct stat buffer;
        return (stat (fileAsPath.c_str(), &buffer) == 0);
    }
    
    #ifndef NULL_STR
        #define NULL_STR "\0"
    #endif
    
    #if $DEBUG
        #define $TEST \
            std::cout << "{ WARN! DEVELOPER DEBUG ENABLED! }" << std::endl;
        
        #define $SET(var, val) \
            std::cout << "{{" << "Debug-Set '" << val << "'}}" << std::endl; var = val;
    #else
        #define $TEST
        #define $SET(var, val)
    #endif
};

#endif //LUALINK_UTILS_H
