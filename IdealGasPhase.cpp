#include "IdealGasPhase.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <numeric>

// Phase 基类实现

void Phase::setMoleFractions(const double* x) {
    if (!x) return;
    m_moleFractions.assign(x, x + nSpecies());
    normalizeComposition(m_moleFractions);
    
    // 更新质量分数
    convertMoleToMass(m_moleFractions, m_massFractions);
}

void Phase::setMoleFractionsByName(const std::string& x) {
    std::vector<double> fractions(nSpecies(), 0.0);
    parseComposition(x, fractions, false);
    setMoleFractions(fractions.data());
}

void Phase::setMoleFractionsByMap(const std::map<std::string, double>& x) {
    std::vector<double> fractions(nSpecies(), 0.0);
    parseCompositionMap(x, fractions, false);
    setMoleFractions(fractions.data());
}

void Phase::getMoleFractions(double* x) const {
    if (!x) return;
    std::copy(m_moleFractions.begin(), m_moleFractions.end(), x);
}

void Phase::setMassFractions(const double* y) {
    if (!y) return;
    m_massFractions.assign(y, y + nSpecies());
    normalizeComposition(m_massFractions);
    
    // 更新摩尔分数
    convertMassToMole(m_massFractions, m_moleFractions);
}

void Phase::setMassFractionsByName(const std::string& y) {
    std::vector<double> fractions(nSpecies(), 0.0);
    parseComposition(y, fractions, true);
    setMassFractions(fractions.data());
}

void Phase::setMassFractionsByMap(const std::map<std::string, double>& y) {
    std::vector<double> fractions(nSpecies(), 0.0);
    parseCompositionMap(y, fractions, true);
    setMassFractions(fractions.data());
}

void Phase::getMassFractions(double* y) const {
    if (!y) return;
    std::copy(m_massFractions.begin(), m_massFractions.end(), y);
}

double Phase::meanMolecularWeight() const {
    if (m_moleFractions.empty() || m_molecularWeights.empty()) {
        return 28.96; // 默认空气分子量
    }
    
    double sum = 0.0;
    for (size_t i = 0; i < m_moleFractions.size(); ++i) {
        sum += m_moleFractions[i] * m_molecularWeights[i];
    }
    return sum;
}

void Phase::addSpecies(const std::string& name, double mw) {
    m_speciesNames.push_back(name);
    m_molecularWeights.push_back(mw);
    resizeArrays();
}

void Phase::resizeArrays() {
    size_t n = nSpecies();
    m_moleFractions.resize(n, 0.0);
    m_massFractions.resize(n, 0.0);
    
    // 如果只有一个组分，设为1.0
    if (n == 1) {
        m_moleFractions[0] = 1.0;
        m_massFractions[0] = 1.0;
    }
}

// IdealGasPhase 实现

IdealGasPhase::IdealGasPhase() 
    : Phase(), m_p0(OneAtm), m_pressure(OneAtm), m_tlast(-1.0) {
    setName("IdealGas");
}

IdealGasPhase::IdealGasPhase(const std::string& yamlFile, const std::string& phaseName) 
    : IdealGasPhase() {
    initFromYaml(yamlFile, phaseName);
}

void IdealGasPhase::addSpecies(const std::string& name, double mw) {
    // Call the base class method to add species and resize base arrays
    Phase::addSpecies(name, mw);
    
    // Resize thermodynamic vectors to match the new species count
    size_t n = nSpecies();
    m_h0_RT.resize(n);
    m_s0_R.resize(n);
    m_cp0_R.resize(n);
    m_g0_RT.resize(n);
    
    // Reset temperature cache to force recalculation
    m_tlast = -1.0;
}

void IdealGasPhase::initFromYaml(const std::string& yamlFile, const std::string& phaseName) {
    try {
        // 加载热力学数据
        m_thermoData = ChemistryVars::extractThermo(yamlFile);
        
        // 清除现有数据
        m_speciesNames.clear();
        m_molecularWeights.clear();
        
        // 添加组分
        for (const auto& thermo : m_thermoData) {
            // 计算分子量
            double mw = 0.0;
            for (const auto& elem : thermo.composition) {
                // 简化的原子量表
                std::map<std::string, double> atomicWeights = {
                    {"H", 1.008}, {"C", 12.011}, {"N", 14.007}, {"O", 15.999},
                    {"Ar", 39.948}, {"He", 4.003}, {"Ne", 20.180}, {"Kr", 83.798},
                    {"Xe", 131.293}, {"S", 32.06}, {"P", 30.974}, {"Cl", 35.45},
                    {"F", 18.998}, {"Br", 79.904}, {"I", 126.904}
                };
                
                auto it = atomicWeights.find(elem.first);
                if (it != atomicWeights.end()) {
                    mw += it->second * elem.second;
                }
            }
            
            addSpecies(thermo.name, mw);
        }
        
        // 初始化存储数组
        size_t n = nSpecies();
        m_h0_RT.resize(n);
        m_s0_R.resize(n);
        m_cp0_R.resize(n);
        m_g0_RT.resize(n);
        
        // 如果指定了相名，使用它
        if (!phaseName.empty()) {
            setName(phaseName);
        }
        
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to initialize from YAML: " + std::string(e.what()));
    }
}

void IdealGasPhase::setState_TPX(double T, double P, const std::string& X) {
    setTemperature(T);
    setPressure(P);
    setMoleFractionsByName(X);
}

void IdealGasPhase::setState_TPX(double T, double P, const double* X) {
    setTemperature(T);
    setPressure(P);
    setMoleFractions(X);
}

void IdealGasPhase::setState_TPX(double T, double P, const std::map<std::string, double>& X) {
    setTemperature(T);
    setPressure(P);
    setMoleFractionsByMap(X);
}

void IdealGasPhase::setState_TPY(double T, double P, const std::string& Y) {
    setTemperature(T);
    setPressure(P);
    setMassFractionsByName(Y);
}

void IdealGasPhase::setState_TPY(double T, double P, const double* Y) {
    setTemperature(T);
    setPressure(P);
    setMassFractions(Y);
}

void IdealGasPhase::setState_TPY(double T, double P, const std::map<std::string, double>& Y) {
    setTemperature(T);
    setPressure(P);
    setMassFractionsByMap(Y);
}

void IdealGasPhase::setState_TP(double T, double P) {
    setTemperature(T);
    setPressure(P);
}

double IdealGasPhase::pressure() const {
    // 直接返回存储的压力值
    return m_pressure;
}

double IdealGasPhase::density() const {
    // 使用理想气体状态方程计算密度: ρ = P*M̄/(Ru*T)
    // 其中 M̄ 是平均分子量 (kg/kmol), Ru 是通用气体常数 (J/(kmol·K))
    double meanMW = meanMolecularWeight(); // kg/kmol
    double T = temperature(); // K
    double P = pressure(); // Pa
    
    if (T > 1e-100 && meanMW > 1e-100) {
        return (P * meanMW) / (GasConstant * T); // kg/m³
    } else {
        return m_dens; // 回退到存储的值
    }
}

void IdealGasPhase::setPressure(double p) {
    // If p is less than a threshold, assume it's in atm and convert to Pa
    // (since typical atmospheric pressures are ~1e5 Pa)
    if (p < 1e4) {
        p *= 101325.0; // Convert atm to Pa
    }
    m_pressure = p;
    
    // 使用标准理想气体状态方程计算密度: ρ = P*M̄/(Ru*T)
    // 其中 M̄ 是平均分子量 (kg/kmol), Ru 是通用气体常数 (J/(kmol·K))
    double meanMW = meanMolecularWeight(); // kg/kmol
    double temperature = this->temperature(); // K
    double newDensity = (p * meanMW) / (GasConstant * temperature); // kg/m³
    
    setDensity(newDensity);
}

double IdealGasPhase::enthalpy_mole() const {
    updateThermo();
    return mean_X(m_h0_RT) * RT();
}

double IdealGasPhase::entropy_mole() const {
    updateThermo();
    double s_mix = -sum_xlogx() * GasConstant; // 混合熵
    double s_ref = mean_X(m_s0_R) * GasConstant;
    double s_pressure = -GasConstant * std::log(pressure() / m_p0);
    return s_ref + s_mix + s_pressure;
}

double IdealGasPhase::gibbs_mole() const {
    return enthalpy_mole() - temperature() * entropy_mole();
}

double IdealGasPhase::cp_mole() const {
    updateThermo();
    return mean_X(m_cp0_R) * GasConstant;
}

double IdealGasPhase::cv_mole() const {
    return cp_mole() - GasConstant; // 理想气体: Cp - Cv = R
}

double IdealGasPhase::intEnergy_mole() const {
    return enthalpy_mole() - RT();
}

std::string IdealGasPhase::report() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(4);
    
    oss << "\n";
    oss << "*******************************************************************\n";
    oss << "*                      " << name() << " Report                        *\n";
    oss << "*******************************************************************\n";
    oss << "\n";
    
    // 基本状态信息
    oss << "       temperature   " << std::setw(12) << temperature() << "  K\n";
    oss << "          pressure   " << std::setw(12) << pressure() << "  Pa\n";
    oss << "           density   " << std::setw(12) << density() << "  kg/m³\n";
    oss << "  mean mol. weight   " << std::setw(12) << meanMolecularWeight() << "  kg/kmol\n";
    oss << "\n";
    
    // 热力学性质
    oss << "                          1 kg             1 kmol\n";
    oss << "                     ---------------   ---------------\n";
    oss << "          enthalpy   " << std::setw(12) << enthalpy_mass() << "     " 
        << std::setw(12) << enthalpy_mole() << "     J\n";
    oss << "   internal energy   " << std::setw(12) << intEnergy_mass() << "     " 
        << std::setw(12) << intEnergy_mole() << "     J\n";
    oss << "           entropy   " << std::setw(12) << entropy_mass() << "     " 
        << std::setw(12) << entropy_mole() << "     J/K\n";
    oss << "    Gibbs function   " << std::setw(12) << gibbs_mass() << "     " 
        << std::setw(12) << gibbs_mole() << "     J\n";
    oss << " heat capacity c_p   " << std::setw(12) << cp_mass() << "     " 
        << std::setw(12) << cp_mole() << "     J/K\n";
    oss << " heat capacity c_v   " << std::setw(12) << cv_mass() << "     " 
        << std::setw(12) << cv_mole() << "     J/K\n";
    oss << "\n";
    
    // 组分信息
    if (nSpecies() > 0) {
        oss << "                         X             Y          Chem. Pot. / RT\n";
        oss << "                     -----------   -----------   ---------------\n";
        
        for (size_t i = 0; i < nSpecies(); ++i) {
            if (moleFraction(i) > 1e-10 || massFraction(i) > 1e-10) {
                double chemPot = 0.0; // 简化版本
                if (i < m_g0_RT.size()) {
                    chemPot = m_g0_RT[i] + std::log(std::max(moleFraction(i) * pressure() / m_p0, 1e-100));
                }
                
                oss << std::setw(16) << speciesName(i) << "   " 
                    << std::setw(12) << moleFraction(i) << "   " 
                    << std::setw(12) << massFraction(i) << "   " 
                    << std::setw(12) << chemPot << "\n";
            }
        }
    }
    
    oss << "\n";
    return oss.str();
}

void IdealGasPhase::updateThermo() const {
    double T = temperature();
    if (std::abs(T - m_tlast) < 1e-6) {
        return; // 温度没有变化，不需要更新
    }
    
    m_tlast = T;
    
    // 计算各组分的热力学性质
    for (size_t i = 0; i < nSpecies(); ++i) {
        if (i >= m_thermoData.size()) {
            // 使用默认值
            m_h0_RT[i] = 0.0;
            m_s0_R[i] = 0.0;
            m_cp0_R[i] = 3.5; // 双原子分子默认值
            continue;
        }
        
        const auto& thermo = m_thermoData[i];
        
        // 选择合适的温度范围和系数
        std::vector<double> coeffs;
        if (!thermo.coefficients.low.empty() && !thermo.coefficients.high.empty()) {
            // NASA7多项式
            if (thermo.temperatureRanges.size() >= 3) {
                double Tmid = thermo.temperatureRanges[1];
                if (T <= Tmid) {
                    coeffs = thermo.coefficients.low;
                } else {
                    coeffs = thermo.coefficients.high;
                }
            } else if (!thermo.coefficients.low.empty()) {
                coeffs = thermo.coefficients.low;
            }
        }
        
        if (coeffs.size() >= 7) {
            // NASA7多项式计算
            m_h0_RT[i] = evaluateNASA(coeffs, T, 0); // 焓
            m_s0_R[i] = evaluateNASA(coeffs, T, 1);  // 熵
            m_cp0_R[i] = evaluateNASA(coeffs, T, 2); // 热容
        } else {
            // 使用默认值
            m_h0_RT[i] = 0.0;
            m_s0_R[i] = 0.0;
            m_cp0_R[i] = 3.5;
        }
        
        // 计算吉布斯自由能
        m_g0_RT[i] = m_h0_RT[i] - m_s0_R[i];
    }
}

double IdealGasPhase::evaluateNASA(const std::vector<double>& coeffs, double T, int property) const {
    if (coeffs.size() < 7) return 0.0;
    
    double T2 = T * T;
    double T3 = T2 * T;
    double T4 = T3 * T;
    
    switch (property) {
        case 0: // H/RT
            return coeffs[0] + coeffs[1] * T / 2.0 + coeffs[2] * T2 / 3.0 + 
                   coeffs[3] * T3 / 4.0 + coeffs[4] * T4 / 5.0 + coeffs[5] / T;
        case 1: // S/R
            return coeffs[0] * std::log(T) + coeffs[1] * T + coeffs[2] * T2 / 2.0 + 
                   coeffs[3] * T3 / 3.0 + coeffs[4] * T4 / 4.0 + coeffs[6];
        case 2: // Cp/R
            return coeffs[0] + coeffs[1] * T + coeffs[2] * T2 + 
                   coeffs[3] * T3 + coeffs[4] * T4;
        default:
            return 0.0;
    }
}

double IdealGasPhase::mean_X(const std::vector<double>& values) const {
    if (values.empty() || m_moleFractions.empty()) return 0.0;
    
    double sum = 0.0;
    size_t n = std::min(values.size(), m_moleFractions.size());
    for (size_t i = 0; i < n; ++i) {
        sum += m_moleFractions[i] * values[i];
    }
    return sum;
}

double IdealGasPhase::sum_xlogx() const {
    double sum = 0.0;
    for (size_t i = 0; i < m_moleFractions.size(); ++i) {
        double x = m_moleFractions[i];
        if (x > 1e-100) {
            sum += x * std::log(x);
        }
    }
    return sum;
}

void Phase::parseComposition(const std::string& comp, std::vector<double>& fractions, bool isMass) {
    fractions.assign(nSpecies(), 0.0);
    
    if (comp.empty()) {
        return;
    }
    
    // 改进的解析器：处理 "H2O:1.0, H2:8.0, AR:1.0" 格式
    // 也支持 "H2O=1.0, H2=8.0" 和 "H2O 1.0, H2 8.0" 格式
    std::istringstream iss(comp);
    std::string token;
    
    while (std::getline(iss, token, ',')) {
        // 去除空格
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);
        
        if (token.empty()) continue;
        
        // 查找分隔符: 冒号、等号或空格
        size_t sepPos = std::string::npos;
        char separator = ':';
        
        sepPos = token.find(':');
        if (sepPos == std::string::npos) {
            sepPos = token.find('=');
            separator = '=';
        }
        if (sepPos == std::string::npos) {
            sepPos = token.find(' ');
            separator = ' ';
        }
        
        if (sepPos != std::string::npos) {
            std::string species = token.substr(0, sepPos);
            std::string value = token.substr(sepPos + 1);
            
            // 去除空格
            species.erase(0, species.find_first_not_of(" \t"));
            species.erase(species.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
              // 查找组分索引
            size_t index = speciesIndex(species);
            if (index != std::string::npos) {
                try {
                    double val = std::stod(value);
                    if (val >= 0.0) {
                        fractions[index] = val;
                        // Debug: uncomment for troubleshooting
                        // std::cout << "Set " << species << "[" << index << "] = " << val << std::endl;
                    }
                } catch (const std::exception& e) {
                    // 忽略无效值，可以选择记录警告
                    // std::cout << "Warning: Invalid value '" << value << "' for species '" << species << "'" << std::endl;
                }
            } else {
                // 可选：添加警告
                // std::cout << "Warning: Unknown species '" << species << "' not found in phase (nSpecies=" << nSpecies() << ")" << std::endl;
            }
        } else {
            // 没有找到分隔符，可能是单独的组分名（默认值为1.0）
            std::string species = token;
            size_t index = speciesIndex(species);
            if (index != std::string::npos) {
                fractions[index] = 1.0;
            }
        }
    }
      // 归一化
    normalizeComposition(fractions);
}

void Phase::parseCompositionMap(const std::map<std::string, double>& comp, std::vector<double>& fractions, bool isMass) {
    fractions.assign(nSpecies(), 0.0);
    
    // 遍历composition map
    for (const auto& pair : comp) {
        const std::string& species = pair.first;
        double value = pair.second;
        
        // 查找组分索引
        size_t index = speciesIndex(species);
        if (index != std::string::npos && value >= 0.0) {
            fractions[index] = value;
        } else if (index == std::string::npos) {
            // 可选：添加警告，当前实现忽略未知组分
            // std::cout << "Warning: Unknown species '" << species << "' ignored." << std::endl;
        }
    }
    
    // 归一化
    normalizeComposition(fractions);
}

void Phase::normalizeComposition(std::vector<double>& fractions) {
    double sum = std::accumulate(fractions.begin(), fractions.end(), 0.0);
    if (sum > 1e-100) {
        for (double& f : fractions) {
            f /= sum;
        }
    }
}

void Phase::convertMoleToMass(const std::vector<double>& X, std::vector<double>& Y) {
    Y.resize(X.size());
    double meanMW = meanMolecularWeight();
    
    for (size_t i = 0; i < X.size(); ++i) {
        if (i < m_molecularWeights.size() && meanMW > 1e-100) {
            Y[i] = X[i] * m_molecularWeights[i] / meanMW;
        } else {
            Y[i] = X[i];
        }
    }
}

void Phase::convertMassToMole(const std::vector<double>& Y, std::vector<double>& X) {
    X.resize(Y.size());
    
    // 首先计算总摩尔数
    double totalMoles = 0.0;
    for (size_t i = 0; i < Y.size(); ++i) {
        if (i < m_molecularWeights.size() && m_molecularWeights[i] > 1e-100) {
            totalMoles += Y[i] / m_molecularWeights[i];
        }
    }
    
    // 计算摩尔分数
    if (totalMoles > 1e-100) {
        for (size_t i = 0; i < Y.size(); ++i) {
            if (i < m_molecularWeights.size() && m_molecularWeights[i] > 1e-100) {
                X[i] = (Y[i] / m_molecularWeights[i]) / totalMoles;
            } else {
                X[i] = 0.0;
            }
        }
    } else {
        std::fill(X.begin(), X.end(), 0.0);
    }
}

// NASA多项式计算函数实现
void IdealGasPhase::getEnthalpy_RT_ref(double* hrt) const {
    double T = temperature();
    for (size_t k = 0; k < nSpecies(); ++k) {
        if (k < m_thermoData.size()) {
            // 选择合适的温度范围的系数
            const auto& coeffs = (T > 1000.0) ? m_thermoData[k].coefficients.high : m_thermoData[k].coefficients.low;
            if (!coeffs.empty()) {
                // 使用NASA多项式计算无量纲焓 H/(RT)
                hrt[k] = evaluateNASA(coeffs, T, 0); // 0表示焓
            } else {
                hrt[k] = 0.0;
            }
        } else {
            hrt[k] = 0.0;
        }
    }
}

void IdealGasPhase::getEntropy_R_ref(double* sr) const {
    double T = temperature();
    for (size_t k = 0; k < nSpecies(); ++k) {
        if (k < m_thermoData.size()) {
            // 选择合适的温度范围的系数
            const auto& coeffs = (T > 1000.0) ? m_thermoData[k].coefficients.high : m_thermoData[k].coefficients.low;
            if (!coeffs.empty()) {
                // 使用NASA多项式计算无量纲熵 S/R
                sr[k] = evaluateNASA(coeffs, T, 1); // 1表示熵
            } else {
                sr[k] = 0.0;
            }
        } else {
            sr[k] = 0.0;
        }
    }
}

void IdealGasPhase::getCp_R_ref(double* cpr) const {
    double T = temperature();
    for (size_t k = 0; k < nSpecies(); ++k) {
        if (k < m_thermoData.size()) {
            // 选择合适的温度范围的系数
            const auto& coeffs = (T > 1000.0) ? m_thermoData[k].coefficients.high : m_thermoData[k].coefficients.low;
            if (!coeffs.empty()) {
                // 使用NASA多项式计算无量纲热容 Cp/R
                cpr[k] = evaluateNASA(coeffs, T, 2); // 2表示热容
            } else {
                cpr[k] = 0.0;
            }
        } else {
            cpr[k] = 0.0;
        }
    }
}
