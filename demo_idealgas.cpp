#include "IdealGasPhase.h"
#include <iostream>
#include <iomanip>

int main() {
    try {
        std::cout << "=== IdealGasPhase Demo ===" << std::endl;
        
        // 创建理想气体对象
        IdealGasPhase gas;
        
        // 手动添加一些组分进行测试
        gas.addSpecies("H2", 2.016);    // 氢气
        gas.addSpecies("O2", 31.998);   // 氧气
        gas.addSpecies("H2O", 18.015);  // 水蒸气
        gas.addSpecies("N2", 28.014);   // 氮气
        
        std::cout << "Added " << gas.nSpecies() << " species:" << std::endl;
        for (size_t i = 0; i < gas.nSpecies(); ++i) {
            std::cout << "  " << i << ": " << gas.speciesName(i) 
                      << " (MW = " << gas.molecularWeights()[i] << " kg/kmol)" << std::endl;
        }
        std::cout << std::endl;
        
        // 设置状态 - 温度、压力和摩尔分数
        double T = 1000.0;  // K
        double P = 101325.0; // Pa
        
        // 测试字符串解析
        std::cout << "Setting state: T = " << T << " K, P = " << P << " Pa" << std::endl;
        std::cout << "Composition: H2:2.0, O2:1.0, H2O:2.0, N2:1.0" << std::endl;
        
        gas.setState_TPX(T, P, "H2:2.0, O2:1.0, H2O:2.0, N2:1.0");
        
        // 打印详细报告
        std::cout << gas.report() << std::endl;
        
        // 测试不同的状态设置方法
        std::cout << "\n=== Testing different state setting methods ===" << std::endl;
        
        // 使用数组设置摩尔分数
        double X[] = {0.4, 0.2, 0.3, 0.1}; // H2, O2, H2O, N2
        gas.setState_TPX(800.0, 2.0 * OneAtm, X);
        
        std::cout << "\nAfter setState_TPX with array (T=800K, P=2atm):" << std::endl;
        std::cout << "Temperature: " << gas.temperature() << " K" << std::endl;
        std::cout << "Pressure: " << gas.pressure() << " Pa" << std::endl;
        std::cout << "Density: " << gas.density() << " kg/m³" << std::endl;
        std::cout << "Mean molecular weight: " << gas.meanMolecularWeight() << " kg/kmol" << std::endl;
        
        // 显示热力学性质
        std::cout << "\nThermodynamic properties:" << std::endl;
        std::cout << "Enthalpy (molar): " << gas.enthalpy_mole() << " J/kmol" << std::endl;
        std::cout << "Entropy (molar): " << gas.entropy_mole() << " J/(kmol·K)" << std::endl;
        std::cout << "Cp (molar): " << gas.cp_mole() << " J/(kmol·K)" << std::endl;
        std::cout << "Cv (molar): " << gas.cv_mole() << " J/(kmol·K)" << std::endl;
        std::cout << "Gibbs energy (molar): " << gas.gibbs_mole() << " J/kmol" << std::endl;
        
        std::cout << "\nMass-based properties:" << std::endl;
        std::cout << "Enthalpy (mass): " << gas.enthalpy_mass() << " J/kg" << std::endl;
        std::cout << "Entropy (mass): " << gas.entropy_mass() << " J/(kg·K)" << std::endl;
        std::cout << "Cp (mass): " << gas.cp_mass() << " J/(kg·K)" << std::endl;
        std::cout << "Cv (mass): " << gas.cv_mass() << " J/(kg·K)" << std::endl;
        
        // 显示组分信息
        std::cout << "\nSpecies composition:" << std::endl;
        std::cout << std::setw(10) << "Species" << std::setw(15) << "Mole Fraction" 
                  << std::setw(15) << "Mass Fraction" << std::endl;
        std::cout << std::string(40, '-') << std::endl;
        
        for (size_t i = 0; i < gas.nSpecies(); ++i) {
            std::cout << std::setw(10) << gas.speciesName(i) 
                      << std::setw(15) << std::fixed << std::setprecision(4) << gas.moleFraction(i)
                      << std::setw(15) << std::fixed << std::setprecision(4) << gas.massFraction(i) 
                      << std::endl;
        }
        
        // 测试质量分数设置
        std::cout << "\n=== Testing mass fraction setting ===" << std::endl;
        gas.setState_TPY(600.0, OneAtm, "H2O:0.5, N2:0.3, O2:0.2");
        
        std::cout << "\nAfter setState_TPY (T=600K, P=1atm, mass fractions):" << std::endl;
        std::cout << "Temperature: " << gas.temperature() << " K" << std::endl;
        std::cout << "Pressure: " << gas.pressure() << " Pa" << std::endl;
        std::cout << "Mean molecular weight: " << gas.meanMolecularWeight() << " kg/kmol" << std::endl;
        
        for (size_t i = 0; i < gas.nSpecies(); ++i) {
            if (gas.moleFraction(i) > 1e-10) {
                std::cout << gas.speciesName(i) << ": X=" << gas.moleFraction(i) 
                          << ", Y=" << gas.massFraction(i) << std::endl;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n=== Demo completed successfully ===" << std::endl;
    return 0;
}
