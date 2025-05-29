#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>

// Simplified test to calculate density using the same formula as yaml-convector-2.0
class SimpleDensityCalculator {
private:
    std::vector<double> moleFractions;
    std::vector<double> molecularWeights;
    std::vector<std::string> speciesNames;
    double temperature;
    double pressure;
    const double R_universal = 8314.46261815324; // J/(kmol·K)

public:
    void addSpecies(const std::string& name, double mw) {
        speciesNames.push_back(name);
        molecularWeights.push_back(mw);
        moleFractions.push_back(0.0);
    }
    
    void parseComposition(const std::string& compositionStr) {
        // Parse composition string like "H2O:0.1, H2:0.8, AR:0.1"
        std::vector<double> values(speciesNames.size(), 0.0);
        
        std::istringstream ss(compositionStr);
        std::string token;
        
        while (std::getline(ss, token, ',')) {
            // Remove leading/trailing whitespace
            size_t start = token.find_first_not_of(" \t");
            size_t end = token.find_last_not_of(" \t");
            if (start != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            size_t colonPos = token.find(':');
            if (colonPos != std::string::npos) {
                std::string species = token.substr(0, colonPos);
                double value = std::stod(token.substr(colonPos + 1));
                
                // Find species index
                for (size_t i = 0; i < speciesNames.size(); ++i) {
                    if (speciesNames[i] == species) {
                        values[i] = value;
                        break;
                    }
                }
            }
        }
        
        // Normalize to mole fractions
        double sum = 0.0;
        for (double v : values) {
            sum += v;
        }
        
        if (sum > 0) {
            for (size_t i = 0; i < values.size(); ++i) {
                moleFractions[i] = values[i] / sum;
            }
        }
    }
    
    void setState_TPX(double T, double P, const std::string& composition) {
        temperature = T;
        pressure = P;
        parseComposition(composition);
    }
    
    double meanMolecularWeight() const {
        double sum = 0.0;
        for (size_t i = 0; i < moleFractions.size(); ++i) {
            sum += moleFractions[i] * molecularWeights[i];
        }
        return sum;
    }
    
    double RT() const {
        return R_universal * temperature;
    }
    
    double density() const {
        // ρ = P * MW / (R * T)
        return pressure * meanMolecularWeight() / RT();
    }
    
    void printState() const {
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Temperature: " << temperature << " K" << std::endl;
        std::cout << "Pressure: " << pressure << " Pa" << std::endl;
        std::cout << "R_universal: " << R_universal << " J/(kmol·K)" << std::endl;
        std::cout << "RT: " << RT() << " J/kmol" << std::endl;
        
        std::cout << "\nSpecies composition:" << std::endl;
        double totalCheck = 0.0;
        for (size_t i = 0; i < speciesNames.size(); ++i) {
            std::cout << "  " << speciesNames[i] 
                      << ": X = " << moleFractions[i]
                      << ", MW = " << molecularWeights[i] << " kg/kmol"
                      << ", contribution = " << moleFractions[i] * molecularWeights[i]
                      << std::endl;
            totalCheck += moleFractions[i];
        }
        std::cout << "Total mole fraction check: " << totalCheck << std::endl;
        
        double mw = meanMolecularWeight();
        double rho = density();
        
        std::cout << "\nCalculated values:" << std::endl;
        std::cout << "  Mean molecular weight: " << mw << " kg/kmol" << std::endl;
        std::cout << "  Density: " << rho << " kg/m³" << std::endl;
        
        // Manual verification
        std::cout << "\nManual verification:" << std::endl;
        std::cout << "  P * MW / RT = " << pressure << " * " << mw << " / " << RT() 
                  << " = " << (pressure * mw / RT()) << " kg/m³" << std::endl;
    }
};

int main() {
    std::cout << "=== YAML-CONVECTOR-2.0 DENSITY CALCULATION TEST ===" << std::endl;
    std::cout << "Testing the exact conditions that show discrepancy with Cantera" << std::endl;
    std::cout << "Expected: Cantera = 0.1446 kg/m³, yaml-convector-2.0 = 0.1677 kg/m³" << std::endl;
    std::cout << std::endl;
    
    SimpleDensityCalculator calc;
    
    // Add species with the same molecular weights as in yaml-convector-2.0
    calc.addSpecies("H2O", 18.015);  // kg/kmol
    calc.addSpecies("H2", 2.016);    // kg/kmol  
    calc.addSpecies("AR", 39.948);   // kg/kmol
    
    // Set the exact test conditions
    double T = 1000.0;  // K
    double P = 101325.0; // Pa (1 atm)
    std::string composition = "H2O:0.1, H2:0.8, AR:0.1";
    
    calc.setState_TPX(T, P, composition);
    
    std::cout << "Test conditions:" << std::endl;
    std::cout << "  T = " << T << " K" << std::endl;
    std::cout << "  P = " << P << " Pa (1 atm)" << std::endl;
    std::cout << "  Composition: " << composition << std::endl;
    std::cout << std::endl;
    
    calc.printState();
    
    std::cout << "\n=== COMPARISON WITH EXPECTED VALUES ===" << std::endl;
    double our_density = calc.density();
    std::cout << "Our calculation: " << our_density << " kg/m³" << std::endl;
    std::cout << "Expected yaml-convector-2.0: 0.1677 kg/m³" << std::endl;
    std::cout << "Expected Cantera: 0.1446 kg/m³" << std::endl;
    std::cout << "Difference from yaml-convector-2.0: " << (our_density - 0.1677) << " kg/m³" << std::endl;
    std::cout << "Difference from Cantera: " << (our_density - 0.1446) << " kg/m³" << std::endl;
    
    return 0;
}
