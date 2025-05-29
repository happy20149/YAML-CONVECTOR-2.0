#include "IdealGasPhase.h"
#include "ChemistryVars.h"
#include <iostream>
#include <iomanip>

void testYamlIntegration() {
    std::cout << "=== Testing YAML Integration ===" << std::endl;
    
    try {
        // 测试YAML读取功能
        std::cout << "Testing ChemistryVars::extractThermo..." << std::endl;
        
        std::string yamlFile = "../mechanism.yaml";
        auto thermoData = ChemistryVars::extractThermo(yamlFile, true);
        
        std::cout << "✓ Successfully extracted " << thermoData.size() << " species thermo data" << std::endl;
        
        // 显示前几个组分的信息
        size_t maxShow = std::min(static_cast<size_t>(5), thermoData.size());
        std::cout << "\nFirst " << maxShow << " species:" << std::endl;
        
        for (size_t i = 0; i < maxShow; ++i) {
            const auto& thermo = thermoData[i];
            std::cout << "  " << i+1 << ". " << thermo.name << std::endl;
            std::cout << "     Model: " << thermo.model << std::endl;
            std::cout << "     Composition: ";
            for (const auto& elem : thermo.composition) {
                std::cout << elem.first << ":" << elem.second << " ";
            }
            std::cout << std::endl;
            
            if (!thermo.temperatureRanges.empty()) {
                std::cout << "     Temperature ranges: ";
                for (const auto& temp : thermo.temperatureRanges) {
                    std::cout << temp << " ";
                }
                std::cout << "K" << std::endl;
            }
            
            if (!thermo.coefficients.low.empty()) {
                std::cout << "     Has NASA7 coefficients (low: " 
                          << thermo.coefficients.low.size() << ", high: " 
                          << thermo.coefficients.high.size() << ")" << std::endl;
            }
            std::cout << std::endl;
        }
        
        // 测试IdealGasPhase与YAML的集成
        std::cout << "\n=== Testing IdealGasPhase with YAML ===" << std::endl;
        
        IdealGasPhase gas(yamlFile, "gas");
        std::cout << "✓ Successfully created IdealGasPhase from YAML" << std::endl;
        std::cout << "  Number of species: " << gas.nSpecies() << std::endl;
        
        if (gas.nSpecies() > 0) {
            // 设置一个测试状态
            std::cout << "\nSetting test state..." << std::endl;
            
            // 尝试使用YAML文件中的一些组分
            std::string composition = "H2:1.0, O2:0.5, N2:3.76";
            gas.setState_TPX(800.0, 101325.0, composition);
            
            std::cout << "✓ Set state: T=800K, P=1atm, X=" << composition << std::endl;
            
            // 显示结果
            std::cout << "\nResulting state:" << std::endl;
            std::cout << "  Temperature: " << gas.temperature() << " K" << std::endl;
            std::cout << "  Pressure: " << gas.pressure() << " Pa" << std::endl;
            std::cout << "  Density: " << gas.density() << " kg/m³" << std::endl;
            std::cout << "  Mean MW: " << gas.meanMolecularWeight() << " kg/kmol" << std::endl;
            
            // 显示主要组分
            std::cout << "\nMajor species:" << std::endl;
            for (size_t i = 0; i < gas.nSpecies(); ++i) {
                if (gas.moleFraction(i) > 0.01) {
                    std::cout << "  " << gas.speciesName(i) 
                              << ": X = " << std::fixed << std::setprecision(4) 
                              << gas.moleFraction(i) << std::endl;
                }
            }
            
            // 显示热力学性质
            std::cout << "\nThermodynamic properties:" << std::endl;
            std::cout << "  Cp (molar): " << std::scientific << std::setprecision(3) 
                      << gas.cp_mole() << " J/(kmol·K)" << std::endl;
            std::cout << "  Enthalpy (molar): " << std::scientific << std::setprecision(3) 
                      << gas.enthalpy_mole() << " J/kmol" << std::endl;
            std::cout << "  Entropy (molar): " << std::scientific << std::setprecision(3) 
                      << gas.entropy_mole() << " J/(kmol·K)" << std::endl;
        }
          } catch (const std::exception& e) {
        std::cerr << "Error during YAML integration test: " << e.what() << std::endl;
        
        // 退回到手动测试
        std::cout << "\nFalling back to manual testing..." << std::endl;
        // testManualCreation(); // 移除这个调用，因为它在后面会被调用
    }
}

void testManualCreation() {
    std::cout << "=== Manual Creation Test ===" << std::endl;
    
    try {
        IdealGasPhase gas;
        
        // 手动添加一些常见组分
        gas.addSpecies("H2", 2.016);
        gas.addSpecies("O2", 31.998);
        gas.addSpecies("N2", 28.014);
        gas.addSpecies("CO2", 44.010);
        gas.addSpecies("H2O", 18.015);
        gas.addSpecies("CO", 28.010);
        gas.addSpecies("CH4", 16.042);
        
        std::cout << "✓ Added " << gas.nSpecies() << " species manually" << std::endl;
        
        // 测试不同的组成
        struct TestCase {
            std::string name;
            double T, P;
            std::string composition;
        };
        
        std::vector<TestCase> testCases = {
            {"Air", 298.15, 101325.0, "N2:0.79, O2:0.21"},
            {"Combustion air", 600.0, 101325.0, "N2:3.76, O2:1.0"},
            {"Flue gas", 1200.0, 101325.0, "CO2:1.0, H2O:2.0, N2:10.0"},
            {"Natural gas", 300.0, 200000.0, "CH4:0.95, CO2:0.03, N2:0.02"},
            {"Syngas", 800.0, 2000000.0, "CO:1.0, H2:2.0, CO2:0.1"}
        };
        
        std::cout << "\nTesting different compositions:" << std::endl;
        std::cout << std::setw(15) << "Case" << std::setw(8) << "T(K)" << std::setw(10) << "P(Pa)" 
                  << std::setw(12) << "ρ(kg/m³)" << std::setw(10) << "MW" << std::setw(12) << "Cp(J/kg·K)" << std::endl;
        std::cout << std::string(75, '-') << std::endl;
        
        for (const auto& testCase : testCases) {
            try {
                gas.setState_TPX(testCase.T, testCase.P, testCase.composition);
                
                std::cout << std::setw(15) << testCase.name
                          << std::setw(8) << std::fixed << std::setprecision(0) << testCase.T
                          << std::setw(10) << std::scientific << std::setprecision(1) << testCase.P
                          << std::setw(12) << std::fixed << std::setprecision(3) << gas.density()
                          << std::setw(10) << std::fixed << std::setprecision(2) << gas.meanMolecularWeight()
                          << std::setw(12) << std::fixed << std::setprecision(1) << gas.cp_mass() << std::endl;
                          
            } catch (const std::exception& e) {
                std::cout << std::setw(15) << testCase.name << " - Error: " << e.what() << std::endl;
            }
        }
        
        // 详细报告一个案例
        std::cout << "\nDetailed report for air at 500K:" << std::endl;
        gas.setState_TPX(500.0, 101325.0, "N2:0.79, O2:0.21");
        std::cout << gas.report() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in manual creation test: " << e.what() << std::endl;
    }
}

void testPropertyCalculations() {
    std::cout << "\n=== Testing Property Calculations ===" << std::endl;
    
    try {
        IdealGasPhase gas;
        gas.addSpecies("N2", 28.014);
        gas.addSpecies("O2", 31.998);
        
        // 测试理想气体定律
        std::cout << "Testing ideal gas law (PV = nRT):" << std::endl;
        std::cout << std::setw(8) << "T(K)" << std::setw(10) << "P(Pa)" << std::setw(12) << "ρ(kg/m³)" 
                  << std::setw(12) << "P_calc(Pa)" << std::setw(10) << "Error%" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        for (double T = 300; T <= 1000; T += 100) {
            for (double P = 50000; P <= 200000; P += 50000) {
                gas.setState_TPX(T, P, "N2:0.79, O2:0.21");
                double P_calc = gas.pressure();
                double error = std::abs(P_calc - P) / P * 100.0;
                
                std::cout << std::setw(8) << std::fixed << std::setprecision(0) << T
                          << std::setw(10) << std::scientific << std::setprecision(1) << P
                          << std::setw(12) << std::fixed << std::setprecision(4) << gas.density()
                          << std::setw(12) << std::scientific << std::setprecision(1) << P_calc
                          << std::setw(10) << std::fixed << std::setprecision(2) << error << std::endl;
                          
                if (T == 300 && P == 50000) break; // 只显示几行作为示例
            }
            if (T == 400) break; // 限制输出
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error in property calculations test: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "IdealGasPhase Integration Test" << std::endl;
    std::cout << "==============================" << std::endl;
    
    // 测试YAML集成
    testYamlIntegration();
    
    // 测试手动创建
    testManualCreation();
    
    // 测试性质计算
    testPropertyCalculations();
    
    std::cout << "\n=== Integration test completed ===" << std::endl;
    return 0;
}
