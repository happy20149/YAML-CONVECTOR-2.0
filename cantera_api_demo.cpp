// Cantera API Compatibility Demo
// This demonstrates the exact interface you requested
#include "IdealGasPhase.h"
#include <iostream>
#include <iomanip>
#include <memory>

// Forward declarations for Cantera-like API
class Solution;
class ThermoPhase;

// Solution class - mimics Cantera's Solution
class Solution {
public:
    Solution(const std::string& yamlFile, const std::string& phaseName = "") {
        m_gas = std::make_shared<IdealGasPhase>(yamlFile, phaseName);
    }
    
    std::shared_ptr<IdealGasPhase> thermo() { return m_gas; }
    
private:
    std::shared_ptr<IdealGasPhase> m_gas;
};

// Cantera-compatible newSolution function
std::shared_ptr<Solution> newSolution(const std::string& yamlFile, const std::string& phaseName = "") {
    return std::make_shared<Solution>(yamlFile, phaseName);
}

// Constants (should match Cantera)
const double OneAtm = 101325.0; // Pa

void simple_demo() {
    std::cout << "=== Cantera API Demo ===" << std::endl;
    
    try {
        // Create a new Solution object
        auto sol = newSolution("h2o2.yaml");
        auto gas = sol->thermo();

        // Set the thermodynamic state by specifying T (500 K) P (2 atm) and the mole
        // fractions. Note that the mole fractions do not need to sum to 1.0 - they will
        // be normalized internally. Also, the values for any unspecified species will be
        // set to zero.
        gas->setState_TPX(500.0, 2.0*OneAtm, "H2O:1.0, H2:8.0, AR:1.0");

        // Print a summary report of the state of the gas.
        std::cout << gas->report() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cout << "\nNote: YAML loading may not be fully implemented yet." << std::endl;
        
        // Fallback: manually create the system
        std::cout << "\n=== Manual Implementation Demo ===" << std::endl;
        
        IdealGasPhase gas;
        
        // Add species from h2o2.yaml mechanism
        gas.addSpecies("H2", 2.016);
        gas.addSpecies("H", 1.008);
        gas.addSpecies("O", 15.999);
        gas.addSpecies("O2", 31.998);
        gas.addSpecies("OH", 17.007);
        gas.addSpecies("H2O", 18.015);
        gas.addSpecies("HO2", 33.007);
        gas.addSpecies("H2O2", 34.015);
        gas.addSpecies("AR", 39.948);
        gas.addSpecies("N2", 28.014);
        
        // Set the same state as requested
        gas.setState_TPX(500.0, 2.0*OneAtm, "H2O:1.0, H2:8.0, AR:1.0");
        
        std::cout << gas.report() << std::endl;
        
        // Show detailed thermodynamic information
        std::cout << "\n=== Detailed Analysis ===" << std::endl;
        std::cout << "Number of species: " << gas.nSpecies() << std::endl;
        std::cout << "Temperature: " << gas.temperature() << " K" << std::endl;
        std::cout << "Pressure: " << gas.pressure() << " Pa" << std::endl;
        std::cout << "Density: " << gas.density() << " kg/m³" << std::endl;
        std::cout << "Mean molecular weight: " << gas.meanMolecularWeight() << " kg/kmol" << std::endl;
        
        std::cout << "\nMolar thermodynamic properties:" << std::endl;
        std::cout << "  Enthalpy: " << gas.enthalpy_mole() << " J/kmol" << std::endl;
        std::cout << "  Entropy: " << gas.entropy_mole() << " J/(kmol·K)" << std::endl;
        std::cout << "  Cp: " << gas.cp_mole() << " J/(kmol·K)" << std::endl;
        std::cout << "  Cv: " << gas.cv_mole() << " J/(kmol·K)" << std::endl;
        
        std::cout << "\nMass-based thermodynamic properties:" << std::endl;
        std::cout << "  Enthalpy: " << gas.enthalpy_mass() << " J/kg" << std::endl;
        std::cout << "  Entropy: " << gas.entropy_mass() << " J/(kg·K)" << std::endl;
        std::cout << "  Cp: " << gas.cp_mass() << " J/(kg·K)" << std::endl;
        std::cout << "  Cv: " << gas.cv_mass() << " J/(kg·K)" << std::endl;
        
        std::cout << "\nSpecies composition:" << std::endl;
        for (size_t i = 0; i < gas.nSpecies(); ++i) {
            if (gas.moleFraction(i) > 1e-10) {
                std::cout << "  " << std::setw(6) << gas.speciesName(i) 
                          << ": X = " << std::fixed << std::setprecision(3) << gas.moleFraction(i)
                          << ", Y = " << std::fixed << std::setprecision(3) << gas.massFraction(i) << std::endl;
            }
        }
    }
}

int main() {
    simple_demo();
    return 0;
}
