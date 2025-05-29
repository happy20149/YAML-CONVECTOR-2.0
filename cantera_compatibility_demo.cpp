#include "IdealGasPhase.h"
#include <iostream>
#include <iomanip>

// 创建类似Cantera的newSolution函数
std::shared_ptr<IdealGasPhase> newSolution(const std::string& yamlFile, const std::string& phaseName = "") {
    return std::make_shared<IdealGasPhase>(yamlFile, phaseName);
}

// 模拟Cantera工作流程的示例
void canteraLikeWorkflow() {
    std::cout << "=== Cantera-like Workflow Demo ===" << std::endl;
    
    try {
        // 步骤1: 创建解决方案对象 (类似 ct.Solution)
        auto gas = newSolution("mechanism.yaml", "gas");
        std::cout << "✓ Created solution from mechanism.yaml" << std::endl;
        std::cout << "  Number of species: " << gas->nSpecies() << std::endl;
        
        // 显示一些组分名称
        if (gas->nSpecies() > 0) {
            std::cout << "  Available species: ";
            size_t showCount = std::min(static_cast<size_t>(5), gas->nSpecies());
            for (size_t i = 0; i < showCount; ++i) {
                std::cout << gas->speciesName(i);
                if (i < showCount - 1) std::cout << ", ";
            }
            if (gas->nSpecies() > 5) std::cout << ", ...";
            std::cout << std::endl;
        }
        
        // 步骤2: 设置初始状态 (类似 gas.setState_TPX)
        std::cout << "\n✓ Setting initial state..." << std::endl;
        gas->setState_TPX(300.0, 101325.0, "CH4:1.0, O2:2.0, N2:7.52");
        std::cout << "  Initial conditions: 300K, 1atm, stoichiometric CH4/air" << std::endl;
        
        // 步骤3: 显示初始状态 (类似 gas.report())
        std::cout << "\n✓ Initial state report:" << std::endl;
        std::cout << gas->report() << std::endl;
        
        // 步骤4: 改变状态并观察性质变化
        std::cout << "✓ Testing property calculations at different conditions..." << std::endl;
        
        std::cout << "\nTemperature effects (at 1 atm, same composition):" << std::endl;
        std::cout << std::setw(8) << "T (K)" << std::setw(12) << "ρ (kg/m³)" 
                  << std::setw(12) << "Cp (J/kg·K)" << std::setw(15) << "H (kJ/kg)" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        for (double T = 300; T <= 2000; T += 200) {
            gas->setState_TP(T, 101325.0);
            std::cout << std::setw(8) << std::fixed << std::setprecision(0) << T
                      << std::setw(12) << std::fixed << std::setprecision(3) << gas->density()
                      << std::setw(12) << std::fixed << std::setprecision(1) << gas->cp_mass()
                      << std::setw(15) << std::fixed << std::setprecision(1) << gas->enthalpy_mass() / 1000.0 << std::endl;
        }
        
        // 步骤5: 测试不同组成
        std::cout << "\n✓ Testing different compositions..." << std::endl;
        
        // 纯甲烷
        gas->setState_TPX(500.0, 101325.0, "CH4:1.0");
        std::cout << "\nPure CH4 at 500K:" << std::endl;
        std::cout << "  MW = " << gas->meanMolecularWeight() << " kg/kmol" << std::endl;
        std::cout << "  Cp = " << gas->cp_mass() << " J/(kg·K)" << std::endl;
        
        // 燃烧产物 (假设)
        gas->setState_TPX(1500.0, 101325.0, "CO2:1.0, H2O:2.0, N2:7.52");
        std::cout << "\nCombustion products at 1500K:" << std::endl;
        std::cout << "  MW = " << gas->meanMolecularWeight() << " kg/kmol" << std::endl;
        std::cout << "  Cp = " << gas->cp_mass() << " J/(kg·K)" << std::endl;
        std::cout << "  H = " << gas->enthalpy_mass() / 1000.0 << " kJ/kg" << std::endl;
        
        // 步骤6: 测试压力效应
        std::cout << "\n✓ Testing pressure effects..." << std::endl;
        std::cout << "\nDensity vs pressure (at 400K, fixed composition):" << std::endl;
        std::cout << std::setw(12) << "P (atm)" << std::setw(15) << "ρ (kg/m³)" 
                  << std::setw(15) << "ρ_ideal" << std::setw(12) << "Error %" << std::endl;
        std::cout << std::string(55, '-') << std::endl;
        
        for (double P_atm = 0.1; P_atm <= 10.0; P_atm *= 2) {
            double P = P_atm * 101325.0;
            gas->setState_TP(400.0, P);
            double rho_actual = gas->density();
            double rho_ideal = P * gas->meanMolecularWeight() / (8314.462 * 400.0);
            double error = std::abs(rho_actual - rho_ideal) / rho_ideal * 100.0;
            
            std::cout << std::setw(12) << std::fixed << std::setprecision(1) << P_atm
                      << std::setw(15) << std::fixed << std::setprecision(4) << rho_actual
                      << std::setw(15) << std::fixed << std::setprecision(4) << rho_ideal
                      << std::setw(12) << std::fixed << std::setprecision(2) << error << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error in Cantera-like workflow: " << e.what() << std::endl;
        std::cout << "Note: This may be expected if YAML loading is not fully implemented" << std::endl;
        
        // 退回到手动创建
        std::cout << "\nFalling back to manual species creation..." << std::endl;
        
        auto gas = std::make_shared<IdealGasPhase>();
        gas->addSpecies("CH4", 16.042);
        gas->addSpecies("O2", 31.998);
        gas->addSpecies("N2", 28.014);
        gas->addSpecies("CO2", 44.010);
        gas->addSpecies("H2O", 18.015);
        
        gas->setState_TPX(1000.0, 101325.0, "CH4:0.1, O2:0.2, N2:0.6, CO2:0.05, H2O:0.05");
        
        std::cout << "\nManually created gas mixture at 1000K:" << std::endl;
        std::cout << "Temperature: " << gas->temperature() << " K" << std::endl;
        std::cout << "Pressure: " << gas->pressure() << " Pa" << std::endl;
        std::cout << "Density: " << gas->density() << " kg/m³" << std::endl;
        std::cout << "Mean MW: " << gas->meanMolecularWeight() << " kg/kmol" << std::endl;
        
        std::cout << "\nSpecies composition:" << std::endl;
        for (size_t i = 0; i < gas->nSpecies(); ++i) {
            std::cout << "  " << gas->speciesName(i) 
                      << ": X = " << std::fixed << std::setprecision(4) << gas->moleFraction(i) << std::endl;
        }
    }
}

void demonstrateCanteraAPI() {
    std::cout << "\n=== Demonstrating Cantera-compatible API ===" << std::endl;
    
    // 创建理想气体对象
    IdealGasPhase gas;
    
    // 添加常见气体组分
    gas.addSpecies("H2", 2.016);
    gas.addSpecies("O2", 31.998);
    gas.addSpecies("N2", 28.014);
    gas.addSpecies("H2O", 18.015);
    gas.addSpecies("CO2", 44.010);
    gas.addSpecies("CO", 28.010);
    
    std::cout << "Created gas with " << gas.nSpecies() << " species" << std::endl;
    
    // 展示Cantera风格的API调用
    std::cout << "\nCantera-style API calls:" << std::endl;
    
    // gas.setState_TPX(T, P, X)
    gas.setState_TPX(800.0, 101325.0, "H2:1.0, O2:0.5");
    std::cout << "gas.setState_TPX(800, 101325, 'H2:1.0, O2:0.5')" << std::endl;
    
    // gas.temperature()
    std::cout << "gas.temperature() = " << gas.temperature() << " K" << std::endl;
    
    // gas.pressure()
    std::cout << "gas.pressure() = " << gas.pressure() << " Pa" << std::endl;
    
    // gas.density()
    std::cout << "gas.density() = " << gas.density() << " kg/m³" << std::endl;
    
    // gas.enthalpy_mole()
    std::cout << "gas.enthalpy_mole() = " << gas.enthalpy_mole() << " J/kmol" << std::endl;
    
    // gas.cp_mole()
    std::cout << "gas.cp_mole() = " << gas.cp_mole() << " J/(kmol·K)" << std::endl;
    
    // gas.entropy_mole()
    std::cout << "gas.entropy_mole() = " << gas.entropy_mole() << " J/(kmol·K)" << std::endl;
    
    // 显示组分信息
    std::cout << "\nSpecies information:" << std::endl;
    for (size_t i = 0; i < gas.nSpecies(); ++i) {
        if (gas.moleFraction(i) > 1e-10) {
            std::cout << "  " << gas.speciesName(i) 
                      << ": X = " << gas.moleFraction(i) 
                      << ", MW = " << gas.molecularWeights()[i] << " kg/kmol" << std::endl;
        }
    }
    
    // gas.report() - 生成详细报告
    std::cout << "\ngas.report():" << std::endl;
    std::cout << gas.report() << std::endl;
}

int main() {
    std::cout << "Cantera Compatibility Demo" << std::endl;
    std::cout << "===========================" << std::endl;
    
    // 演示Cantera工作流程
    canteraLikeWorkflow();
    
    // 演示API兼容性
    demonstrateCanteraAPI();
    
    std::cout << "\n=== Cantera compatibility demo completed ===" << std::endl;
    
    return 0;
}
