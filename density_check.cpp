#include <iostream>
#include <iomanip>

int main() {
    double P = 202650.0;    // Pa
    double MW = 7.4091;     // kg/kmol
    double R = 8314.47;     // J/(kmol·K)
    double T = 5000.0;      // K
    
    double rho_theory = P * MW / (R * T);
    
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "理论密度计算:" << std::endl;
    std::cout << "P = " << P << " Pa" << std::endl;
    std::cout << "MW = " << MW << " kg/kmol" << std::endl;
    std::cout << "R = " << R << " J/(kmol·K)" << std::endl;
    std::cout << "T = " << T << " K" << std::endl;
    std::cout << "ρ = P*MW/(R*T) = " << rho_theory << " kg/m³" << std::endl;
    
    std::cout << "\n期望密度: 0.036118 kg/m³" << std::endl;
    std::cout << "实际密度: 0.087817 kg/m³" << std::endl;
    std::cout << "比值: " << 0.087817/0.036118 << std::endl;
    
    return 0;
}
