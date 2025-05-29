#include "IdealGasPhase.h"
#include <iostream>
#include <iomanip>
#include <map>

int main() {
    try {
        // 创建理想气体相
        IdealGasPhase gas;
        
        // 添加组分及其分子量 (kg/kmol)
        gas.addSpecies("H2O", 18.01534);   // 水
        gas.addSpecies("H2", 2.01588);     // 氢气
        gas.addSpecies("AR", 39.948);      // 氩气
        
        // 设置状态：T=1000K, P=101325Pa
        double T = 1000.0;  // K
        double P = 101325.0; // Pa
        
        // 设置组分摩尔分数: H2O:0.1, H2:0.8, AR:0.1
        std::map<std::string, double> composition = {
            {"H2O", 0.1},
            {"H2", 0.8}, 
            {"AR", 0.1}
        };
        
        gas.setState_TPX(T, P, composition);
        
        // 输出结果
        std::cout << std::fixed << std::setprecision(8);
        std::cout << "=== 密度计算验证 ===" << std::endl;
        std::cout << "温度: " << gas.temperature() << " K" << std::endl;
        std::cout << "压力: " << gas.pressure() << " Pa" << std::endl;
        std::cout << std::endl;
        
        // 显示组分信息
        std::cout << "组分摩尔分数:" << std::endl;
        for (size_t i = 0; i < gas.nSpecies(); ++i) {
            std::cout << "  " << gas.speciesName(i) << ": " 
                      << gas.moleFraction(i) << std::endl;
        }
        std::cout << std::endl;
        
        // 显示分子量信息
        std::cout << "组分分子量 (kg/kmol):" << std::endl;
        const auto& molWeights = gas.molecularWeights();
        for (size_t i = 0; i < gas.nSpecies(); ++i) {
            std::cout << "  " << gas.speciesName(i) << ": " 
                      << molWeights[i] << std::endl;
        }
        std::cout << std::endl;
        
        // 计算平均分子量
        double meanMW = gas.meanMolecularWeight();
        std::cout << "平均分子量计算:" << std::endl;
        std::cout << "  M̄ = Σ(xi * Mi)" << std::endl;
        double calculated_meanMW = 0.0;
        for (size_t i = 0; i < gas.nSpecies(); ++i) {
            double contribution = gas.moleFraction(i) * molWeights[i];
            calculated_meanMW += contribution;
            std::cout << "  " << gas.speciesName(i) << ": " 
                      << gas.moleFraction(i) << " × " << molWeights[i] 
                      << " = " << contribution << std::endl;
        }
        std::cout << "  M̄ = " << calculated_meanMW << " kg/kmol" << std::endl;
        std::cout << "  实际返回值: " << meanMW << " kg/kmol" << std::endl;
        std::cout << std::endl;
        
        // 密度计算
        double density = gas.density();
        std::cout << "密度计算:" << std::endl;
        std::cout << "  公式: ρ = P × M̄ / (Ru × T)" << std::endl;
        std::cout << "  其中:" << std::endl;
        std::cout << "    P = " << P << " Pa" << std::endl;
        std::cout << "    M̄ = " << meanMW << " kg/kmol" << std::endl;
        std::cout << "    Ru = 8314.462618 J/(kmol·K)" << std::endl;
        std::cout << "    T = " << T << " K" << std::endl;
        std::cout << std::endl;
        
        double manual_density = (P * meanMW) / (8314.462618 * T);
        std::cout << "  手动计算: ρ = (" << P << " × " << meanMW << ") / (8314.462618 × " << T << ")" << std::endl;
        std::cout << "           = " << manual_density << " kg/m³" << std::endl;
        std::cout << "  实际返回值: " << density << " kg/m³" << std::endl;
        std::cout << std::endl;
        
        // 与标准Cantera预期值比较 
        double cantera_expected = 0.090292; // 从之前的分析得出
        std::cout << "=== 与标准Cantera比较 ===" << std::endl;
        std::cout << "yaml-convector-2.0 密度: " << density << " kg/m³" << std::endl;
        std::cout << "标准Cantera预期密度:   " << cantera_expected << " kg/m³" << std::endl;
        
        double ratio = density / cantera_expected;
        std::cout << "密度比值: " << ratio << std::endl;
        
        if (std::abs(ratio - 1.0) < 0.01) {
            std::cout << "✓ 密度差异在1%以内，修复成功!" << std::endl;
        } else {
            std::cout << "✗ 密度差异仍然很大，需要进一步调查" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}