#pragma once
#include "ChemistryVars.h"
#include "ChemistryIO.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>

// 常量定义
const double GasConstant = 8314.462175;  // J/(kmol·K) - 通用气体常数
const double OneAtm = 101325.0;          // Pa - 标准大气压

// 简化的Phase基类
class Phase {
public:
    Phase() : m_temp(298.15), m_dens(1.0), m_name("gas") {}
    virtual ~Phase() = default;

    // 基本属性访问
    virtual double temperature() const { return m_temp; }
    virtual void setTemperature(double t) { m_temp = t; }
      virtual double density() const { return m_dens; }
    virtual void setDensity(double d) { m_dens = d; }
    
    virtual double molarDensity() const { return m_dens / meanMolecularWeight(); }
    
    virtual double pressure() const { return OneAtm; } // 默认实现
    
    virtual std::string name() const { return m_name; }
    virtual void setName(const std::string& name) { m_name = name; }
    
    // 组分相关
    virtual size_t nSpecies() const { return m_speciesNames.size(); }
    virtual const std::vector<std::string>& speciesNames() const { return m_speciesNames; }
    virtual std::string speciesName(size_t k) const { 
        if (k >= m_speciesNames.size()) throw std::out_of_range("Species index out of range");
        return m_speciesNames[k]; 
    }
    virtual size_t speciesIndex(const std::string& name) const {
        auto it = std::find(m_speciesNames.begin(), m_speciesNames.end(), name);
        if (it != m_speciesNames.end()) {
            return std::distance(m_speciesNames.begin(), it);
        }
        return std::string::npos;
    }
    
    // 摩尔分数和质量分数
    virtual void setMoleFractions(const double* x);
    virtual void setMoleFractionsByName(const std::string& x);
    virtual void getMoleFractions(double* x) const;
    virtual double moleFraction(size_t k) const { 
        if (k >= m_moleFractions.size()) return 0.0;
        return m_moleFractions[k]; 
    }
    
    virtual void setMassFractions(const double* y);
    virtual void setMassFractionsByName(const std::string& y);
    virtual void getMassFractions(double* y) const;
    virtual double massFraction(size_t k) const { 
        if (k >= m_massFractions.size()) return 0.0;
        return m_massFractions[k];    }
    
    // 分子量相关
    virtual double meanMolecularWeight() const;
    virtual const std::vector<double>& molecularWeights() const { return m_molecularWeights; }
      // 初始化函数（公共接口）
    virtual void addSpecies(const std::string& name, double mw);
    
protected:
    virtual void resizeArrays();
    
    // 辅助函数声明
    void parseComposition(const std::string& comp, std::vector<double>& fractions, bool isMass = false);
    void normalizeComposition(std::vector<double>& fractions);
    void convertMoleToMass(const std::vector<double>& X, std::vector<double>& Y);
    void convertMassToMole(const std::vector<double>& Y, std::vector<double>& X);
    
    // 成员变量
    double m_temp;                              // 温度 K
    double m_dens;                              // 密度 kg/m³
    std::string m_name;                         // 相名称
    std::vector<std::string> m_speciesNames;    // 组分名称
    std::vector<double> m_moleFractions;        // 摩尔分数
    std::vector<double> m_massFractions;        // 质量分数
    std::vector<double> m_molecularWeights;     // 分子量 kg/kmol
};

// 理想气体相类
class IdealGasPhase : public Phase {
public:
    IdealGasPhase();
    IdealGasPhase(const std::string& yamlFile, const std::string& phaseName = "");
    virtual ~IdealGasPhase() = default;    // 从YAML文件初始化
    void initFromYaml(const std::string& yamlFile, const std::string& phaseName = "");

    // Override addSpecies to resize thermodynamic vectors
    virtual void addSpecies(const std::string& name, double mw) override;

    // 状态设置函数
    void setState_TPX(double T, double P, const std::string& X);
    void setState_TPX(double T, double P, const double* X);
    void setState_TPY(double T, double P, const std::string& Y);
    void setState_TPY(double T, double P, const double* Y);
    void setState_TP(double T, double P);

    // 压力相关
    virtual double pressure() const override;
    virtual void setPressure(double p);

    // 基本热力学性质
    virtual double enthalpy_mole() const;      // 摩尔焓 J/kmol
    virtual double entropy_mole() const;       // 摩尔熵 J/(kmol·K)
    virtual double gibbs_mole() const;         // 摩尔吉布斯自由能 J/kmol
    virtual double cp_mole() const;            // 定压摩尔热容 J/(kmol·K)
    virtual double cv_mole() const;            // 定容摩尔热容 J/(kmol·K)
    virtual double intEnergy_mole() const;     // 摩尔内能 J/kmol

    // 比热力学性质 (per unit mass)
    virtual double enthalpy_mass() const { return enthalpy_mole() / meanMolecularWeight(); }
    virtual double entropy_mass() const { return entropy_mole() / meanMolecularWeight(); }
    virtual double gibbs_mass() const { return gibbs_mole() / meanMolecularWeight(); }
    virtual double cp_mass() const { return cp_mole() / meanMolecularWeight(); }
    virtual double cv_mass() const { return cv_mole() / meanMolecularWeight(); }
    virtual double intEnergy_mass() const { return intEnergy_mole() / meanMolecularWeight(); }

    // 输出报告
    virtual std::string report() const;

    // 获取参考状态压力
    double refPressure() const { return m_p0; }

    // 工具函数
    double RT() const { return GasConstant * temperature(); }
    
protected:
    // 更新热力学数据
    virtual void updateThermo() const;
    
    // 计算参考状态热力学性质
    virtual void getEnthalpy_RT_ref(double* hrt) const;
    virtual void getEntropy_R_ref(double* sr) const;
    virtual void getCp_R_ref(double* cpr) const;
    
    // NASA多项式计算
    virtual double evaluateNASA(const std::vector<double>& coeffs, double T, int property) const;
    
    // 辅助函数
    virtual double mean_X(const std::vector<double>& values) const;
    virtual double sum_xlogx() const;
    
private:
    // 参考状态压力
    double m_p0;                                // Pa
    double m_pressure;                          // 当前压力 Pa
    
    // 热力学数据存储
    std::vector<ChemistryVars::ThermoData> m_thermoData;
    
    // 临时存储数组 (mutable for const functions)
    mutable std::vector<double> m_h0_RT;        // 无量纲参考焓
    mutable std::vector<double> m_s0_R;         // 无量纲参考熵
    mutable std::vector<double> m_cp0_R;        // 无量纲参考热容
    mutable std::vector<double> m_g0_RT;        // 无量纲参考吉布斯能
    mutable double m_tlast;                     // 上次计算的温度
    
    // 内部辅助函数
    void parseComposition(const std::string& comp, std::vector<double>& fractions, bool isMass = false);
    void normalizeComposition(std::vector<double>& fractions);
    void convertMoleToMass(const std::vector<double>& X, std::vector<double>& Y);
    void convertMassToMole(const std::vector<double>& Y, std::vector<double>& X);
};
