#include <iostream>
#include <string>

#include "parameters.h"

const parameters *PARAMETERS;

int main(int length, char **args) {
    std::string arguments = utils::combineArgs(length, args);
    std::cout << "Begin with: [length: '" << length << "', args: '" << arguments << "']" << std::endl;
    
    PARAMETERS = new parameters(length, args);
    int code;
    
    if(PARAMETERS->getFinalState() != 0)
        code = PARAMETERS->getFinalState();
    else code = (PARAMETERS->getFinalState() != 0 ? PARAMETERS->getFinalState() : utils::getError());
    
    system("pause");
    
    return code;
}
