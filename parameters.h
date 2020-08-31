#ifndef LUALINK_PARAMETERS_H
#define LUALINK_PARAMETERS_H

#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <utility>
#include <cstdlib>

#include "utils.h"

class parameters {
  
  private:
    #define $SCRIPTS_PATH_ENVVAR "LLscripts"
    const char* SCRIPTS_PATH_ENVVAR = $SCRIPTS_PATH_ENVVAR;
    
    #define $SCRIPT_PATH_ENVVAR "LLscript_";
    const char* SCRIPT_PATH_ENVVAR = $SCRIPT_PATH_ENVVAR;
    
    #define $SCRIPT_RUNTIME_ENVVAR "LLruntime"
    const char* SCRIPT_RUNTIME_ENVVAR = $SCRIPT_RUNTIME_ENVVAR;
    
    std::string fullAppName;
    std::string shortAppName;
    std::string appName;
    std::string path;
    
    std::string luaScriptsPath;
    std::string luaScript;
    std::string luaRuntime;
    
    int state;
  
  public:
    parameters(int argc, char* argv[]);
    
    [[nodiscard]] int getFinalState() const {
        return this->state;
    }
  
  private:
    int setFromParameters(int argc, char* argv[]);
    int setFromFullAppName(std::string fullAppName);
    int setFromEnvironmentVar();
    int setFromDebug();
    int determine();
    int determineScript();
    int determineInterpreter();
    int tryExecute();
    
    void setFullAppName(std::string fap){
        std::cout << "SetFullAppName: '" << fap << "'" << std::endl;
        this->fullAppName = std::move(fap);
    }
    
    void setShortAppName(std::string san){
        std::cout << "SetShortAppName: '" << san << "'" << std::endl;
        this->shortAppName = std::move(san);
    }
    
    void setAppName(std::string an){
        std::cout << "SetAppName: '" << an << "'" << std::endl;
        this->appName = std::move(an);
    }
    
    void setPath(std::string p){
        std::cout << "SetPathName: '" << p << "'" << std::endl;
        this->path = std::move(p);
    }
};

#endif