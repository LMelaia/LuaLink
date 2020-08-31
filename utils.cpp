#include "utils.h"

int utils::ERROR_CODE = 0;

int utils::error(int errorCode, std::vector<std::string> texts) {
    if((ERROR_CODE = errorCode) == 0) return 0; //Success?
    
    std::cout << "ERROR " << ERROR_CODE << std::endl;
    for(const std::string &text : texts){
        std::cout << " - " << text << std::endl;
    }
    
    return ERROR_CODE;
}

std::string utils::combineArgs(int length, char **args) {
    std::string argsLine;
    for (int i = 0; i < length; ++i) {
        argsLine += "\""; argsLine += args[i]; argsLine += "\"";
    }
    
    return argsLine;
}

bool utils::stringEndsWith(const std::string &value, const std::string &ending) {
    if (ending.size() > value.size())
        return false;
    
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::string utils::getEnvVar(std::string const & key) {
    std::cout << "[[Searching -> \"" << key.c_str() << "\"]]" << std::endl;
    
    char *val = getenv(key.c_str());
    return val == nullptr ? std::string(NULL_STR) : std::string(val);
}
