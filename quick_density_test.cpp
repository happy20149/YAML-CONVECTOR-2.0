#include <iostream>
#include <iomanip>

int main() {
    std::cout << "=== QUICK DENSITY VERIFICATION ===" << std::endl;
    
    // Constants
    const double Ru = 8314.462618;  // J/(kmol·K) - standard value
    
    // Target conditions
    double T = 1000.0;    // K
    double P = 101325.0;  // Pa
    
    // Composition mole fractions: H2O:0.1, H2:0.8, AR:0.1
    double x_H2O = 0.1;
    double x_H2 = 0.8;
    double x_AR = 0.1;
    
    // Molecular weights (kg/kmol)
    double MW_H2O = 18.01534;
    double MW_H2 = 2.01588;
    double MW_AR = 39.948;
    
    // Calculate average molecular weight
    double meanMW = x_H2O * MW_H2O + x_H2 * MW_H2 + x_AR * MW_AR;
    
    // Calculate density using ideal gas law: ρ = p*M̄/(Ru*T)
    double density = (P * meanMW) / (Ru * T);
    
    std::cout << "Input conditions:" << std::endl;
    std::cout << "  Temperature: " << T << " K" << std::endl;
    std::cout << "  Pressure: " << P << " Pa" << std::endl;
    std::cout << "  Composition: H2O=" << x_H2O << ", H2=" << x_H2 << ", AR=" << x_AR << std::endl;
    
    std::cout << "\nMolecular weights (kg/kmol):" << std::endl;
    std::cout << "  H2O: " << std::fixed << std::setprecision(5) << MW_H2O << std::endl;
    std::cout << "  H2: " << std::fixed << std::setprecision(5) << MW_H2 << std::endl;
    std::cout << "  AR: " << std::fixed << std::setprecision(3) << MW_AR << std::endl;
    std::cout << "  Average: " << std::fixed << std::setprecision(6) << meanMW << std::endl;
    
    std::cout << "\nCalculation:" << std::endl;
    std::cout << "  Gas constant (Ru): " << std::fixed << std::setprecision(9) << Ru << " J/(kmol·K)" << std::endl;
    std::cout << "  Formula: ρ = (P × M̄) / (Ru × T)" << std::endl;
    std::cout << "  ρ = (" << P << " × " << meanMW << ") / (" << Ru << " × " << T << ")" << std::endl;
    std::cout << "  ρ = " << std::fixed << std::setprecision(8) << density << " kg/m³" << std::endl;
    
    // Compare with expected Cantera value
    double cantera_expected = 0.090292;  // From previous analysis
    double ratio = density / cantera_expected;
    
    std::cout << "\nComparison with Cantera:" << std::endl;
    std::cout << "  Calculated density: " << std::fixed << std::setprecision(8) << density << " kg/m³" << std::endl;
    std::cout << "  Expected Cantera:   " << std::fixed << std::setprecision(6) << cantera_expected << " kg/m³" << std::endl;
    std::cout << "  Ratio: " << std::fixed << std::setprecision(6) << ratio << std::endl;
    std::cout << "  Difference: " << std::fixed << std::setprecision(2) << (ratio - 1.0) * 100 << "%" << std::endl;
    
    if (std::abs(ratio - 1.0) < 0.01) {
        std::cout << "\n✓ SUCCESS: Density calculation matches Cantera within 1%" << std::endl;
    } else {
        std::cout << "\n✗ WARNING: Significant density discrepancy still exists" << std::endl;
    }
    
    return 0;
}
