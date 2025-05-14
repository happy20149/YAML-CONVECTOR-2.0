#include "ChemistryVars.h"
#include "ChemistryIO.h"
#include <iostream>
#include <limits>
#include "MechanismTest.h"


int main() {
    
    std::string yamlFile = "D:\\mechanism.yaml";
    ChemistryVars::extractThermo(yamlFile, true);
    //ChemistryVars::extractKinetics(yamlFile,true);
	//testSpecificC2H4Data(yamlFile);
	//runMechanismTest(yamlFile);
    return 1;
    
}