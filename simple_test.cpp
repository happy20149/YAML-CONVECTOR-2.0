#include "IdealGasPhase.h"
#include <iostream>

int main() {
    std::cout << "=== Simple IdealGasPhase Test ===" << std::endl;
    
    try {
        // 创建理想气体对象
        IdealGasPhase gas;
        std::cout << "✓ Created IdealGasPhase object" << std::endl;
        
        // 添加组分
        gas.addSpecies("H2", 2.016);
        gas.addSpecies("O2", 31.998);
        gas.addSpecies("N2", 28.014);
        std::cout << "✓ Added " << gas.nSpecies() << " species" << std::endl;
        
        // 设置状态
        gas.setState_TPX(500.0, 101325.0, "H2:1.0, O2:0.5, N2:2.0");
        std::cout << "✓ Set state: T=500K, P=1atm" << std::endl;
        
        // 输出基本信息
        std::cout << "\nBasic properties:" << std::endl;
        std::cout << "Temperature: " << gas.temperature() << " K" << std::endl;
        std::cout << "Pressure: " << gas.pressure() << " Pa" << std::endl;
        std::cout << "Density: " << gas.density() << " kg/m³" << std::endl;
        std::cout << "Mean MW: " << gas.meanMolecularWeight() << " kg/kmol" << std::endl;
        
        // 输出组分信息
        std::cout << "\nComposition:" << std::endl;
        for (size_t i = 0; i < gas.nSpecies(); ++i) {
            std::cout << gas.speciesName(i) << ": X=" << gas.moleFraction(i) 
                      << ", Y=" << gas.massFraction(i) << std::endl;
        }
        
        // 输出热力学性质
        std::cout << "\nThermodynamic properties:" << std::endl;
        std::cout << "Cp (molar): " << gas.cp_mole() << " J/(kmol·K)" << std::endl;
        std::cout << "Cv (molar): " << gas.cv_mole() << " J/(kmol·K)" << std::endl;
        std::cout << "Enthalpy (molar): " << gas.enthalpy_mole() << " J/kmol" << std::endl;
        std::cout << "Entropy (molar): " << gas.entropy_mole() << " J/(kmol·K)" << std::endl;
        
        std::cout << "\n✓ All tests completed successfully!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
