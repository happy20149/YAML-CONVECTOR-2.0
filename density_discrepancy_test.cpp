#include "IdealGasPhase.h"
#include <iostream>
#include <iomanip>

void testDensityDiscrepancy() {
    std::cout << "=== DENSITY DISCREPANCY TEST ===" << std::endl;
    std::cout << "Testing case: H2O:0.1, H2:0.8, AR:0.1 at T=1000K, P=101325Pa" << std::endl;
    
    try {
        IdealGasPhase gas;
        
        // Add species with molecular weights as they appear in the system
        gas.addSpecies("H2O", 18.015);  // Water
        gas.addSpecies("H2", 2.016);    // Hydrogen  
        gas.addSpecies("AR", 39.948);   // Argon
        
        std::cout << "Added species:" << std::endl;
        for (size_t i = 0; i < gas.nSpecies(); ++i) {
            std::cout << "  " << gas.speciesName(i) 
                      << ": MW = " << gas.molecularWeights()[i] << " kg/kmol" << std::endl;
        }
        
        // Set the exact conditions mentioned in the task
        double T = 1000.0;  // K
        double P = 101325.0; // Pa
        std::string composition = "H2O:0.1, H2:0.8, AR:0.1";
        
        std::cout << "\nSetting state:" << std::endl;
        std::cout << "  Temperature: " << T << " K" << std::endl;
        std::cout << "  Pressure: " << P << " Pa" << std::endl;
        std::cout << "  Composition: " << composition << std::endl;
        
        gas.setState_TPX(T, P, composition);
        
        // Get the calculated values
        double actualDensity = gas.density();
        double actualMW = gas.meanMolecularWeight();
        double actualT = gas.temperature();
        double actualP = gas.pressure();
        double RT_value = gas.RT();
        
        std::cout << "\n=== CALCULATED VALUES ===" << std::endl;
        std::cout << "  Actual Temperature: " << actualT << " K" << std::endl;
        std::cout << "  Actual Pressure: " << actualP << " Pa" << std::endl;
        std::cout << "  Actual Density: " << actualDensity << " kg/m³" << std::endl;
        std::cout << "  Actual Mean MW: " << actualMW << " kg/kmol" << std::endl;
        std::cout << "  RT value: " << RT_value << " J/kmol" << std::endl;
        
        // Manual calculation of mean molecular weight
        std::cout << "\n=== MANUAL CALCULATION ANALYSIS ===" << std::endl;
        
        double sum_X = 0.0;
        double manual_meanMW = 0.0;
        
        std::cout << "Composition breakdown:" << std::endl;
        for (size_t i = 0; i < gas.nSpecies(); ++i) {
            double Xi = gas.moleFraction(i);
            double MWi = gas.molecularWeights()[i];
            if (Xi > 1e-10) {
                std::cout << "  " << gas.speciesName(i) 
                          << ": X = " << std::setprecision(6) << Xi 
                          << ", MW = " << MWi 
                          << " kg/kmol, X*MW = " << (Xi * MWi) << std::endl;
                sum_X += Xi;
                manual_meanMW += Xi * MWi;
            }
        }
        
        std::cout << "Sum of mole fractions: " << sum_X << std::endl;
        std::cout << "Manual mean MW: " << manual_meanMW << " kg/kmol" << std::endl;
        std::cout << "System mean MW: " << actualMW << " kg/kmol" << std::endl;
        std::cout << "Difference: " << (manual_meanMW - actualMW) << " kg/kmol" << std::endl;
        
        // Expected composition analysis
        std::cout << "\n=== EXPECTED COMPOSITION ANALYSIS ===" << std::endl;
        // Total moles: 0.1 + 0.8 + 0.1 = 1.0
        double expected_X_H2O = 0.1 / 1.0;
        double expected_X_H2 = 0.8 / 1.0;
        double expected_X_AR = 0.1 / 1.0;
        
        double expected_meanMW = expected_X_H2O * 18.015 + expected_X_H2 * 2.016 + expected_X_AR * 39.948;
        
        std::cout << "Expected mole fractions:" << std::endl;
        std::cout << "  H2O: " << expected_X_H2O << std::endl;
        std::cout << "  H2: " << expected_X_H2 << std::endl;
        std::cout << "  AR: " << expected_X_AR << std::endl;
        std::cout << "Expected mean MW: " << expected_meanMW << " kg/kmol" << std::endl;
        
        // Density calculation
        std::cout << "\n=== DENSITY CALCULATION ===" << std::endl;
        const double R = 8314.462618; // J/(kmol*K) - universal gas constant
        
        // Manual calculation using expected values
        double expected_density = (P * expected_meanMW) / (R * T);
        std::cout << "Expected density (using expected MW): " << expected_density << " kg/m³" << std::endl;
        
        // Manual calculation using actual MW from system
        double manual_density = (actualP * actualMW) / (R * actualT);
        std::cout << "Manual density (using actual MW): " << manual_density << " kg/m³" << std::endl;
        
        std::cout << "System density: " << actualDensity << " kg/m³" << std::endl;
        
        // Compare with reported values
        std::cout << "\n=== COMPARISON WITH REPORTED VALUES ===" << std::endl;
        double cantera_density = 0.1446; // Reported Cantera value
        double yaml_convector_density = 0.1677; // Reported yaml-convector-2.0 value
        
        std::cout << "Cantera reported density: " << cantera_density << " kg/m³" << std::endl;
        std::cout << "yaml-convector-2.0 reported density: " << yaml_convector_density << " kg/m³" << std::endl;
        std::cout << "Our calculation: " << actualDensity << " kg/m³" << std::endl;
        
        // Error analysis
        std::cout << "\nError analysis:" << std::endl;
        std::cout << "  vs Cantera: " << ((actualDensity - cantera_density)/cantera_density * 100) << "%" << std::endl;
        std::cout << "  vs expected yaml-convector: " << ((actualDensity - yaml_convector_density)/yaml_convector_density * 100) << "%" << std::endl;
        
        // Check if our calculation matches the expected yaml-convector result
        if (std::abs(actualDensity - yaml_convector_density) < 0.0001) {
            std::cout << "\n✓ Our calculation matches yaml-convector-2.0 expected result!" << std::endl;
        } else {
            std::cout << "\n✗ Our calculation does NOT match yaml-convector-2.0 expected result." << std::endl;
        }
        
        // Additional debugging
        std::cout << "\n=== STEP-BY-STEP VERIFICATION ===" << std::endl;
        std::cout << "Using ideal gas law: ρ = P*MW/(R*T)" << std::endl;
        std::cout << "P = " << actualP << " Pa" << std::endl;
        std::cout << "MW = " << actualMW << " kg/kmol" << std::endl;
        std::cout << "R = " << R << " J/(kmol*K)" << std::endl;
        std::cout << "T = " << actualT << " K" << std::endl;
        std::cout << "RT = " << (R * actualT) << " J/kmol" << std::endl;
        std::cout << "P*MW = " << (actualP * actualMW) << " Pa*kg/kmol" << std::endl;
        std::cout << "ρ = " << (actualP * actualMW)/(R * actualT) << " kg/m³" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in density discrepancy test: " << e.what() << std::endl;
    }
}

int main() {
    testDensityDiscrepancy();
    return 0;
}
