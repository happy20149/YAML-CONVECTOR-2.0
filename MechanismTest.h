#pragma once
#include <string>
#include "ChemistryVars.h"
#include "ChemistryIO.h"

// �������Ժ�����������MechanismTest.cpp�У�
bool runMechanismTest(const std::string& yamlFile);//����ch4����
bool testSpecificC2H4Data(const std::string& yamlFile); // ����c2h4����

bool chemkinIOTest(const std::string& yamlFile);//������Խӿ�