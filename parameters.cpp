#include "parameters.h"

using std::string, std::cout, std::endl;

static inline bool isFailed(int &state){
    return state != 0;
}

parameters::parameters(int argc, char **argv) {
    this->state = setFromParameters(argc, argv);
    if(!isFailed(state)) this->setFromDebug();
    if(!isFailed(state)) this->state = determine();
    if(!isFailed(state)) this->state = tryExecute();
}

int parameters::setFromParameters(int argc, char **argv) {
    if(argc < 1) return utils::error(utils::errorCodes::INVALID_ARGS, {
            "The program started with an invalid number of arguments."
    });
    
    this->setFullAppName(argv[0]);
    if(this->setFromFullAppName(this->fullAppName) != 0)
        return this->state;
    return setFromEnvironmentVar();
}

int parameters::setFromFullAppName(std::string fap) {
    std::string cPath;
    std::string cElement = "\t";
    
    std::string aName = std::move(fap);
    std::string delimiter = "\\";
    
    size_t pos;
    std::string token;
    while ((pos = aName.find(delimiter)) != std::string::npos) {
        if(cElement != "\t"){
            cPath += cElement + "\\";
        }
        token = aName.substr(0, pos);
        cElement = token;
        aName.erase(0, pos + delimiter.length());
    }
    cPath += cElement + "\\";
    cElement = aName;
    
    this->setPath(cPath);
    this->setShortAppName(cElement);
    this->setAppName(std::regex_replace(cElement, std::regex(".exe"), ""));
    
    this->opensWithCap = utils::startsWithCapital(this->shortAppName);
    this->endsWithCap = utils::endsWithCapital(this->shortAppName);
    cout << "Opening capital: '" << opensWithCap << "'\tClosing capital: '" << endsWithCap << "'" << endl;
    
    return 0;
}

int parameters::setFromEnvironmentVar() {
    std::cout << "Looking for scripts folder from Environment Variable '" << SCRIPTS_PATH_ENVVAR
              << "'..." << std::endl;
    std::string scriptsFolder = utils::getEnvVar(SCRIPTS_PATH_ENVVAR) ;
    std::cout << "Scripts Folder (" << SCRIPTS_PATH_ENVVAR << "): "
              << (scriptsFolder == NULL_STR ? "\\0" : scriptsFolder) << std::endl;
    std::cout << "Looking for script from Environment Variable '"
              << SCRIPT_PATH_ENVVAR << this->appName << "'..." << std::endl;
    std::string script = utils::getEnvVar(SCRIPT_PATH_ENVVAR + this->appName);
    std::cout << "Script (" << SCRIPT_PATH_ENVVAR + this->appName << "): "
              << (script == NULL_STR ? "\\0" : script) << std::endl;
    std::cout << "Looking for runtime from Environment Variable '"
              << SCRIPT_RUNTIME_ENVVAR << "'..." << std::endl;
    std::string runtime = utils::getEnvVar(SCRIPT_RUNTIME_ENVVAR);
    std::cout << "Script (" << SCRIPT_RUNTIME_ENVVAR << "): "
              << (runtime == NULL_STR ? "\\0" : runtime) << std::endl;
    
    if(runtime != NULL_STR) this->luaRuntime = runtime;
    if(scriptsFolder != NULL_STR) this->luaScriptsPath = scriptsFolder;
    if(script != NULL_STR) this->luaScript = script;
    
    return 0;
}

int parameters::determine() {
    int scriptResult;
    
    if((scriptResult = determineScript()) == 0)
        return determineInterpreter();
    else return scriptResult;
}

int parameters::determineScript() {
    cout << "Looking for script..." << endl;
    
    int code = 0;
    string &script = this->luaScript;
    string &scripts = this->luaScriptsPath;
    
    //Direct
    if(script != NULL_STR){
        cout << "Found script '" << script << "' in environment var." << endl;
        if(!utils::stringEndsWith(script, ".lua")){
            cout << "WARN: '" << script << "' does not end in Lua (.lua) extension." << endl;
        }
        
        if(!utils::doesFileExist(script)){
            code = utils::errorCodes::MISSING_SCRIPT;
        } else {
            cout << "Proceeding with '" << script << "' as Lua Script" << endl;
            this->luaScript = script;
            return code;
        }
    } else cout << "No directly specified scripts..." << endl;
    
    //In given folder
    if(scripts != NULL_STR){
        string expectedScript = scripts + "\\" + this->appName + ".lua";
        cout << "Found script directory '" << scripts << "' in environment var." << endl;
        cout << "Looking in script directory for '" << expectedScript << "'..." << endl;
    
        if(!utils::doesFileExist(expectedScript))
            code = utils::errorCodes::MISSING_SCRIPT;
        else {
            cout << "Proceeding with '" << expectedScript << "' as Lua Script" << endl;
            this->luaScript = expectedScript;
            return code;
        }
    } else cout << "No specified scripts directory..." << endl;
    
    //In working directory
    string lastScript = this->path + this->appName + ".lua";
    cout << "Last try! Looking for `" << lastScript << endl;
    
    if(utils::doesFileExist(lastScript)) {
        cout << "Proceeding with '" << lastScript << "' as Lua Script" << endl;
        this->luaScript = lastScript;
        return code;
    }
    
    //Fail
    if(code == utils::errorCodes::MISSING_SCRIPT){
        return utils::error(utils::errorCodes::MISSING_SCRIPT, {
            "No Lua script could be found on disk."
        });
    }
    
    return utils::error(utils::errorCodes::NO_SCRIPT, {
        "Could not find a script nor was one specified."
    });
}

int parameters::determineInterpreter(){
    cout << "Looking for a Lua interpreter..." << endl;
    
    if(this->luaRuntime == NULL_STR){
        return utils::error(utils::errorCodes::NO_RUNTIME, {
            "No Lua interpreter was specified or none could be found."
        });
    }
    
    if(!utils::stringEndsWith(this->luaRuntime, ".exe")){
        return utils::error(utils::errorCodes::INVALID_RUNTIME, {
            "Lua interpreter path doesn't point to an executable (.exe)."
        });
    }
    
    if(!utils::doesFileExist(this->luaRuntime)){
        return utils::error(utils::errorCodes::INVALID_RUNTIME, {
            "Lua interpreter path doesn't point to an existing file on disk.",
            "File '" + luaRuntime + "' does not exist."
        });
    }
    
    cout << "Proceeding with '" << this->luaRuntime << "' as the interpreter..." << endl;
    return 0;
}

int parameters::tryExecute() {
    cout << endl << "Attempting to run script '" << this->luaScript << "'" << endl;
    cout << "Using the interpreter '" << this->luaRuntime << "'" << endl;
    cout << endl << endl;
    
    
    auto run = "\"\"" + this->luaRuntime + "\" \"" + this->luaScript + "\"";
    int retCode = system(run.c_str());
    
    cout << endl << endl << "Execution of '" << run << "' has returned with code '" << retCode << "'" << endl;
    
    return retCode;
}

int parameters::setFromDebug() {
    $SET(this->luaRuntime, "D:\\Projects\\LuaLink\\test-files\\lua.exe");
    $SET(this->luaScript, "D:\\Projects\\LuaLink\\test-files\\LuaLink.lua");
    return 0;
}
