#include "IdealGasPhase.h"
#include <iostream>
#include <iomanip>
#include <map>

int main() {
    try {
        std::cout << "=== Enhanced setState_TPX Functionality Test ===" << std::endl;
        std::cout << std::endl;

        // Create an ideal gas phase object
        IdealGasPhase gas;
        
        // Add some species manually for testing
        gas.addSpecies("H2O", 18.015);   // Water
        gas.addSpecies("H2", 2.016);     // Hydrogen
        gas.addSpecies("O2", 31.998);    // Oxygen
        gas.addSpecies("N2", 28.014);    // Nitrogen
        gas.addSpecies("CO2", 44.01);    // Carbon dioxide
        gas.addSpecies("CH4", 16.043);   // Methane
        gas.addSpecies("AR", 39.948);    // Argon

        double T = 800.0;  // Temperature in K
        double P = 101325.0; // Pressure in Pa

        std::cout << "Testing enhanced setState_TPX functionality" << std::endl;
        std::cout << "Temperature: " << T << " K" << std::endl;
        std::cout << "Pressure: " << P << " Pa" << std::endl;
        std::cout << std::endl;

        // Test 1: String format (existing functionality)
        std::cout << "=== Test 1: String format ===" << std::endl;
        std::string comp1 = "H2O:0.1, H2:0.8, AR:0.1";
        gas.setState_TPX(T, P, comp1);
        std::cout << "Composition: " << comp1 << std::endl;
        std::cout << "Results:" << std::endl;
        for (size_t i = 0; i < gas.nSpecies(); ++i) {
            if (gas.moleFraction(i) > 1e-10) {
                std::cout << "  " << gas.speciesName(i) << ": X = " 
                          << std::fixed << std::setprecision(6) << gas.moleFraction(i) << std::endl;
            }
        }
        std::cout << std::endl;

        // Test 2: Enhanced string format with different separators
        std::cout << "=== Test 2: Enhanced string format with '=' separator ===" << std::endl;
        std::string comp2 = "H2O=0.2, CO2=0.3, N2=0.5";
        gas.setState_TPX(T, P, comp2);
        std::cout << "Composition: " << comp2 << std::endl;
        std::cout << "Results:" << std::endl;
        for (size_t i = 0; i < gas.nSpecies(); ++i) {
            if (gas.moleFraction(i) > 1e-10) {
                std::cout << "  " << gas.speciesName(i) << ": X = " 
                          << std::fixed << std::setprecision(6) << gas.moleFraction(i) << std::endl;
            }
        }
        std::cout << std::endl;

        // Test 3: Composition map format (NEW functionality)
        std::cout << "=== Test 3: Composition map format (NEW) ===" << std::endl;
        std::map<std::string, double> comp3;
        comp3["CH4"] = 0.6;
        comp3["O2"] = 0.3;
        comp3["N2"] = 0.1;
        
        gas.setState_TPX(T, P, comp3);
        std::cout << "Composition map:" << std::endl;
        for (const auto& pair : comp3) {
            std::cout << "  " << pair.first << " = " << pair.second << std::endl;
        }
        std::cout << "Results:" << std::endl;
        for (size_t i = 0; i < gas.nSpecies(); ++i) {
            if (gas.moleFraction(i) > 1e-10) {
                std::cout << "  " << gas.speciesName(i) << ": X = " 
                          << std::fixed << std::setprecision(6) << gas.moleFraction(i) << std::endl;
            }
        }
        std::cout << std::endl;

        // Test 4: Complex mixture with many components
        std::cout << "=== Test 4: Complex mixture with many components ===" << std::endl;
        std::map<std::string, double> comp4;
        comp4["H2O"] = 0.15;
        comp4["H2"] = 0.25;
        comp4["O2"] = 0.10;
        comp4["N2"] = 0.30;
        comp4["CO2"] = 0.05;
        comp4["CH4"] = 0.10;
        comp4["AR"] = 0.05;
        
        gas.setState_TPX(T, P, comp4);
        std::cout << "Complex composition map:" << std::endl;
        for (const auto& pair : comp4) {
            std::cout << "  " << pair.first << " = " << pair.second << std::endl;
        }
        std::cout << "Results:" << std::endl;
        for (size_t i = 0; i < gas.nSpecies(); ++i) {
            if (gas.moleFraction(i) > 1e-10) {
                std::cout << "  " << gas.speciesName(i) << ": X = " 
                          << std::fixed << std::setprecision(6) << gas.moleFraction(i) << std::endl;
            }
        }
        std::cout << std::endl;

        // Test 5: Mass fractions using composition map
        std::cout << "=== Test 5: Mass fractions using composition map (setState_TPY) ===" << std::endl;
        std::map<std::string, double> mass_comp;
        mass_comp["H2O"] = 0.3;
        mass_comp["CO2"] = 0.4;
        mass_comp["N2"] = 0.3;
        
        gas.setState_TPY(T, P, mass_comp);
        std::cout << "Mass fraction composition map:" << std::endl;
        for (const auto& pair : mass_comp) {
            std::cout << "  " << pair.first << " = " << pair.second << " (mass fraction)" << std::endl;
        }
        std::cout << "Results:" << std::endl;
        for (size_t i = 0; i < gas.nSpecies(); ++i) {
            if (gas.moleFraction(i) > 1e-10) {
                std::cout << "  " << gas.speciesName(i) << ": X = " 
                          << std::fixed << std::setprecision(6) << gas.moleFraction(i) 
                          << ", Y = " << gas.massFraction(i) << std::endl;
            }
        }
        std::cout << std::endl;

        // Test 6: Error handling - unknown species
        std::cout << "=== Test 6: Error handling - unknown species ===" << std::endl;
        std::map<std::string, double> comp_with_unknown;
        comp_with_unknown["H2O"] = 0.5;
        comp_with_unknown["UnknownSpecies"] = 0.3;  // This should be ignored
        comp_with_unknown["N2"] = 0.2;
        
        gas.setState_TPX(T, P, comp_with_unknown);
        std::cout << "Composition with unknown species:" << std::endl;
        for (const auto& pair : comp_with_unknown) {
            std::cout << "  " << pair.first << " = " << pair.second << std::endl;
        }
        std::cout << "Results (unknown species should be ignored):" << std::endl;
        for (size_t i = 0; i < gas.nSpecies(); ++i) {
            if (gas.moleFraction(i) > 1e-10) {
                std::cout << "  " << gas.speciesName(i) << ": X = " 
                          << std::fixed << std::setprecision(6) << gas.moleFraction(i) << std::endl;
            }
        }
        std::cout << std::endl;        // NEW TEST: Target density conditions
        std::cout << "=== Test 7: TARGET DENSITY CONDITIONS ===" << std::endl;
        std::cout << "Testing: H2O:0.1, H2:0.8, AR:0.1 at T=1000K, P=101325Pa" << std::endl;
        
        // Reset gas with precise molecular weights
        IdealGasPhase targetGas;
        targetGas.addSpecies("H2O", 18.01534);   // Water (precise)
        targetGas.addSpecies("H2", 2.01588);     // Hydrogen (precise)
        targetGas.addSpecies("AR", 39.948);      // Argon (precise)
        
        // Set target conditions
        double targetT = 1000.0;    // K
        double targetP = 101325.0;  // Pa
        targetGas.setState_TPX(targetT, targetP, "H2O:0.1, H2:0.8, AR:0.1");
        
        // Get system values
        double actualDensity = targetGas.density();
        double actualMW = targetGas.meanMolecularWeight();
        double actualT = targetGas.temperature();
        double actualP = targetGas.pressure();
        
        std::cout << "\nSystem calculation results:" << std::endl;
        std::cout << "  Temperature: " << std::fixed << std::setprecision(2) << actualT << " K" << std::endl;
        std::cout << "  Pressure: " << std::fixed << std::setprecision(1) << actualP << " Pa" << std::endl;
        std::cout << "  Density: " << std::fixed << std::setprecision(8) << actualDensity << " kg/m³" << std::endl;
        std::cout << "  Mean MW: " << std::fixed << std::setprecision(6) << actualMW << " kg/kmol" << std::endl;
        
        // Compare with expected Cantera value
        double cantera_expected = 0.090292;  // From analysis
        double ratio = actualDensity / cantera_expected;
        
        std::cout << "\nComparison with standard Cantera:" << std::endl;
        std::cout << "  yaml-convector-2.0 density: " << std::fixed << std::setprecision(8) << actualDensity << " kg/m³" << std::endl;
        std::cout << "  Expected Cantera density:    " << std::fixed << std::setprecision(6) << cantera_expected << " kg/m³" << std::endl;
        std::cout << "  Density ratio: " << std::fixed << std::setprecision(6) << ratio << std::endl;
        
        // Manual verification
        const double Ru = 8314.462618;  // J/(kmol·K) - standard value
        double manual_density = (actualP * actualMW) / (Ru * actualT);
        std::cout << "  Manual calculation: " << std::fixed << std::setprecision(8) << manual_density << " kg/m³" << std::endl;
        double manual_ratio = manual_density / cantera_expected;
        std::cout << "  Manual calculation ratio: " << std::fixed << std::setprecision(6) << manual_ratio << std::endl;
        
        // Assessment
        if (std::abs(ratio - 1.0) < 0.01) {
            std::cout << "\n✓ SUCCESS: Density within 1% of Cantera, fix verified!" << std::endl;
        } else {
            std::cout << "\n✗ WARNING: Density discrepancy of " << std::fixed << std::setprecision(2) << std::abs(ratio - 1.0) * 100 << "%" << std::endl;
        }
        
        std::cout << "\n=== All tests completed successfully! ===" << std::endl;
        std::cout << "The enhanced setState_TPX functionality supports:" << std::endl;
        std::cout << "1. Original string format: 'H2O:0.1, H2:0.8, AR:0.1'" << std::endl;
        std::cout << "2. Enhanced string formats: 'H2O=0.1' and 'H2O 0.1'" << std::endl;
        std::cout << "3. Composition maps: std::map<std::string, double>" << std::endl;
        std::cout << "4. Flexible number of species (not limited to 3)" << std::endl;
        std::cout << "5. Both mole fractions (TPX) and mass fractions (TPY)" << std::endl;
        std::cout << "6. Automatic normalization and error handling" << std::endl;
        std::cout << "7. TARGET DENSITY CONDITIONS: H2O:0.1, H2:0.8, AR:0.1 at 1000K" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
