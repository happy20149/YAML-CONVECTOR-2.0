#include <iostream>
#include <iomanip>
#include "IdealGasPhase.h"

int main() {
    std::cout << "=== TARGET DENSITY TEST ===" << std::endl;
    std::cout << "Testing: H2O:0.1, H2:0.8, AR:0.1 at T=1000K, P=101325Pa" << std::endl;
    
    try {
        IdealGasPhase gas;
        
        // 手动添加所需的组分及其准确分子量 (kg/kmol)
        gas.addSpecies("H2O", 18.01534);   // 水
        gas.addSpecies("H2", 2.01588);     // 氢气  
        gas.addSpecies("AR", 39.948);      // 氩气
        
        // 设置目标条件
        double T = 1000.0;    // K
        double P = 101325.0;  // Pa
        
        std::cout << "Setting state: T=" << T << "K, P=" << P << "Pa" << std::endl;
        
        // 设置组分：H2O:0.1, H2:0.8, AR:0.1
        gas.setState_TPX(T, P, "H2O:0.1, H2:0.8, AR:0.1");
        
        // 获取系统值
        double actualDensity = gas.density();
        double actualMW = gas.meanMolecularWeight();
        double actualT = gas.temperature();
        double actualP = gas.pressure();
        
        std::cout << "\n系统计算结果:" << std::endl;
        std::cout << "  温度: " << std::fixed << std::setprecision(2) << actualT << " K" << std::endl;
        std::cout << "  压力: " << std::fixed << std::setprecision(1) << actualP << " Pa" << std::endl;
        std::cout << "  密度: " << std::fixed << std::setprecision(8) << actualDensity << " kg/m³" << std::endl;
        std::cout << "  平均分子量: " << std::fixed << std::setprecision(6) << actualMW << " kg/kmol" << std::endl;
        
        // 与标准Cantera预期值比较
        double cantera_expected = 0.090292;  // 从之前分析得出的标准值
        
        std::cout << "\n=== 与标准Cantera比较 ===" << std::endl;
        std::cout << "yaml-convector-2.0 密度: " << std::fixed << std::setprecision(8) << actualDensity << " kg/m³" << std::endl;
        std::cout << "标准Cantera预期密度:    " << std::fixed << std::setprecision(6) << cantera_expected << " kg/m³" << std::endl;
        
        double ratio_actual = actualDensity / cantera_expected;
        std::cout << "密度比值: " << std::fixed << std::setprecision(6) << ratio_actual << std::endl;
        
        // 判断修复效果
        std::cout << "\n修复效果评估:" << std::endl;
        if (std::abs(ratio_actual - 1.0) < 0.01) {
            std::cout << "✓ 系统密度与Cantera差异在1%以内，修复成功!" << std::endl;
        } else {
            std::cout << "✗ 密度差异仍然很大，需要进一步调查" << std::endl;
            std::cout << "  系统差异: " << std::fixed << std::setprecision(2) << std::abs(ratio_actual - 1.0) * 100 << "%" << std::endl;
            
            // 手动计算验证
            const double Ru = 8314.462618;  // J/(kmol·K) - 标准值
            double manual_density = (actualP * actualMW) / (Ru * actualT);
            std::cout << "  手动计算密度: " << std::fixed << std::setprecision(8) << manual_density << " kg/m³" << std::endl;
            double manual_ratio = manual_density / cantera_expected;
            std::cout << "  手动计算比值: " << std::fixed << std::setprecision(6) << manual_ratio << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n=== Test completed ===" << std::endl;
    return 0;
}
