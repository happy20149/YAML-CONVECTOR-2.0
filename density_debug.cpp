#include "IdealGasPhase.h"
#include <iostream>
#include <iomanip>

int main() {
    try {
        // 测试条件
        double T = 500.0;  // K
        double P = 2.0 * 101325.0;  // Pa (2 atm)
        std::string composition = "H2O:1.0, H2:8.0, AR:1.0";
        
        std::cout << "=== 密度计算调试 ===" << std::endl;
        std::cout << "温度: " << T << " K" << std::endl;
        std::cout << "压力: " << P << " Pa (" << P/101325.0 << " atm)" << std::endl;
        std::cout << "组成: " << composition << std::endl << std::endl;
        
        // 创建气体对象
        std::string yamlFile = "D:\\cantera\\data\\h2o2.yaml";
        IdealGasPhase gas(yamlFile, "gas");
        
        // 设置状态
        gas.setState_TPX(T, P, composition);
        
        // 获取实际结果
        std::cout << "=== 实际结果 ===" << std::endl;
        std::cout << "温度: " << gas.temperature() << " K" << std::endl;
        std::cout << "压力: " << gas.pressure() << " Pa" << std::endl;
        std::cout << "密度: " << gas.density() << " kg/m³" << std::endl;
        std::cout << "平均分子量: " << gas.meanMolecularWeight() << " kg/kmol" << std::endl;
        
        // 手动计算预期密度
        double R = 8314.462175;  // J/(kmol·K)
        double RT = R * T;
        double MW = gas.meanMolecularWeight();
        double expectedDensity = P * MW / RT;
        
        std::cout << std::endl << "=== 手动计算 ===" << std::endl;
        std::cout << "RT = " << RT << " J/kmol" << std::endl;
        std::cout << "MW = " << MW << " kg/kmol" << std::endl;
        std::cout << "预期密度 = P * MW / RT = " << P << " * " << MW << " / " << RT << std::endl;
        std::cout << "预期密度 = " << expectedDensity << " kg/m³" << std::endl;
        
        // 比较
        std::cout << std::endl << "=== 比较 ===" << std::endl;
        std::cout << "实际密度: " << gas.density() << " kg/m³" << std::endl;
        std::cout << "预期密度: " << expectedDensity << " kg/m³" << std::endl;
        std::cout << "比率: " << expectedDensity / gas.density() << std::endl;
        
        // 检查摩尔分数和质量分数
        std::cout << std::endl << "=== 组成分析 ===" << std::endl;
        for (size_t i = 0; i < gas.nSpecies(); ++i) {
            if (gas.moleFraction(i) > 1e-10) {
                std::cout << gas.speciesName(i) << ":" << std::endl;
                std::cout << "  摩尔分数: " << gas.moleFraction(i) << std::endl;
                std::cout << "  质量分数: " << gas.massFraction(i) << std::endl;
                std::cout << "  分子量: " << gas.molecularWeights()[i] << " kg/kmol" << std::endl;
            }
        }
        
        // 检查摩尔密度
        std::cout << std::endl << "=== 摩尔密度检查 ===" << std::endl;
        std::cout << "摩尔密度: " << gas.molarDensity() << " kmol/m³" << std::endl;
        std::cout << "期望摩尔密度: " << P / RT << " kmol/m³" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
