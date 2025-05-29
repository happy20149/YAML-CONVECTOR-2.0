# IdealGasPhase - Cantera兼容的理想气体相类

## 概述

IdealGasPhase是一个C++类，提供了类似于Cantera的IdealGasPhase的功能，用于处理理想气体的热力学性质计算。该类可以读取YAML格式的化学机理文件，并提供与Cantera兼容的API接口。

## 主要特性

- **Cantera兼容的API**: 提供与Cantera相似的接口，如`setState_TPX()`, `pressure()`, `enthalpy_mole()`等
- **YAML文件支持**: 可以从YAML格式的化学机理文件中读取组分和热力学数据
- **理想气体状态方程**: 实现完整的理想气体状态方程 PV = nRT
- **热力学性质计算**: 支持焓、熵、热容、吉布斯自由能等性质的计算
- **组分管理**: 支持摩尔分数和质量分数的设置和转换
- **NASA多项式**: 支持NASA7多项式格式的热力学数据

## 类结构

### Phase基类
```cpp
class Phase {
public:
    // 基本属性
    virtual double temperature() const;
    virtual double density() const;
    virtual double pressure() const;
    
    // 组分管理
    virtual size_t nSpecies() const;
    virtual std::string speciesName(size_t k) const;
    virtual double moleFraction(size_t k) const;
    virtual double massFraction(size_t k) const;
    
    // 状态设置
    virtual void setMoleFractions(const double* x);
    virtual void setMassFractions(const double* y);
    virtual void setMoleFractionsByName(const std::string& x);
    virtual void setMassFractionsByName(const std::string& y);
};
```

### IdealGasPhase派生类
```cpp
class IdealGasPhase : public Phase {
public:
    // 构造函数
    IdealGasPhase();
    IdealGasPhase(const std::string& yamlFile, const std::string& phaseName = "");
    
    // 状态设置 (Cantera兼容)
    void setState_TPX(double T, double P, const std::string& X);
    void setState_TPX(double T, double P, const double* X);
    void setState_TPY(double T, double P, const std::string& Y);
    void setState_TPY(double T, double P, const double* Y);
    void setState_TP(double T, double P);
    
    // 热力学性质 (摩尔基)
    virtual double enthalpy_mole() const;    // J/kmol
    virtual double entropy_mole() const;     // J/(kmol·K)
    virtual double gibbs_mole() const;       // J/kmol
    virtual double cp_mole() const;          // J/(kmol·K)
    virtual double cv_mole() const;          // J/(kmol·K)
    
    // 热力学性质 (质量基)
    virtual double enthalpy_mass() const;    // J/kg
    virtual double entropy_mass() const;     // J/(kg·K)
    virtual double cp_mass() const;          // J/(kg·K)
    virtual double cv_mass() const;          // J/(kg·K)
    
    // 输出报告
    virtual std::string report() const;
};
```

## 使用方法

### 1. 基本用法

```cpp
#include "IdealGasPhase.h"

// 创建理想气体对象
IdealGasPhase gas;

// 手动添加组分
gas.addSpecies("H2", 2.016);    // 氢气，分子量 2.016 kg/kmol
gas.addSpecies("O2", 31.998);   // 氧气
gas.addSpecies("N2", 28.014);   // 氮气

// 设置状态：温度(K)，压力(Pa)，摩尔分数
gas.setState_TPX(500.0, 101325.0, "H2:1.0, O2:0.5, N2:2.0");

// 获取性质
std::cout << "Temperature: " << gas.temperature() << " K" << std::endl;
std::cout << "Pressure: " << gas.pressure() << " Pa" << std::endl;
std::cout << "Density: " << gas.density() << " kg/m³" << std::endl;
std::cout << "Cp: " << gas.cp_mole() << " J/(kmol·K)" << std::endl;
```

### 2. 从YAML文件加载

```cpp
// 从YAML文件创建 (类似Cantera的ct.Solution)
IdealGasPhase gas("mechanism.yaml", "gas");

// 设置燃烧状态
gas.setState_TPX(1000.0, 101325.0, "CH4:1.0, O2:2.0, N2:7.52");

// 生成详细报告 (类似Cantera的gas.report())
std::cout << gas.report() << std::endl;
```

### 3. 使用质量分数

```cpp
// 使用质量分数设置状态
gas.setState_TPY(600.0, 101325.0, "H2O:0.5, N2:0.3, O2:0.2");

// 获取组分信息
for (size_t i = 0; i < gas.nSpecies(); ++i) {
    std::cout << gas.speciesName(i) 
              << ": X=" << gas.moleFraction(i) 
              << ", Y=" << gas.massFraction(i) << std::endl;
}
```

### 4. Cantera风格的工作流程

```cpp
// 类似于 Cantera 的 newSolution 函数
auto gas = std::make_shared<IdealGasPhase>("h2o2.yaml");

// 类似于 Cantera 的工作流程
gas->setState_TPX(1500.0, 101325.0, "H2:2.0, O2:1.0");
double h = gas->enthalpy_mole();
double s = gas->entropy_mole();
double cp = gas->cp_mole();

std::cout << gas->report() << std::endl;
```

## 支持的功能

### 状态设置方法
- `setState_TPX(T, P, X)`: 设置温度、压力和摩尔分数
- `setState_TPY(T, P, Y)`: 设置温度、压力和质量分数  
- `setState_TP(T, P)`: 仅设置温度和压力

### 热力学性质
- 焓 (enthalpy): `enthalpy_mole()`, `enthalpy_mass()`
- 熵 (entropy): `entropy_mole()`, `entropy_mass()`
- 定压热容 (cp): `cp_mole()`, `cp_mass()`
- 定容热容 (cv): `cv_mole()`, `cv_mass()`
- 吉布斯自由能 (gibbs): `gibbs_mole()`, `gibbs_mass()`
- 内能 (internal energy): `intEnergy_mole()`, `intEnergy_mass()`

### 基本性质
- 温度: `temperature()`
- 压力: `pressure()`
- 密度: `density()`
- 平均分子量: `meanMolecularWeight()`

### 组分信息
- 组分数量: `nSpecies()`
- 组分名称: `speciesName(k)`, `speciesNames()`
- 摩尔分数: `moleFraction(k)`, `getMoleFractions()`
- 质量分数: `massFraction(k)`, `getMassFractions()`

## 编译和运行

### 依赖项
- C++14 或更高版本
- yaml-cpp 库 (用于YAML文件解析)
- ChemistryVars 和 ChemistryIO 模块

### 编译示例
```bash
# 使用Visual Studio
# 将 IdealGasPhase.h 和 IdealGasPhase.cpp 添加到项目中

# 或使用命令行 (需要适当的include路径)
g++ -std=c++14 -I. simple_test.cpp IdealGasPhase.cpp ChemistryVars.cpp ChemistryIO.cpp -lyaml-cpp -o test
```

### 运行测试
```bash
./simple_test              # 基本功能测试
./integration_test          # 集成测试
./cantera_compatibility_demo # Cantera兼容性演示
```

## 示例程序

项目包含了几个示例程序：

1. **simple_test.cpp**: 基本功能测试
2. **integration_test.cpp**: YAML集成和综合测试
3. **cantera_compatibility_demo.cpp**: Cantera兼容性演示
4. **comprehensive_demo.cpp**: 完整功能演示

## 限制和注意事项

1. **理想气体假设**: 仅适用于理想气体，不考虑分子间相互作用
2. **NASA多项式**: 目前主要支持NASA7格式，NASA9支持有限
3. **热力学数据**: 依赖于YAML文件中的热力学数据质量
4. **压力范围**: 适用于低到中等压力范围
5. **温度范围**: 受NASA多项式数据的温度范围限制

## 与Cantera的差异

虽然API尽可能与Cantera兼容，但仍有一些差异：

1. **简化实现**: 某些高级功能可能未实现
2. **数值精度**: 可能与Cantera的数值结果有小的差异
3. **错误处理**: 错误处理机制可能不同
4. **性能**: 未经过Cantera级别的优化

## 扩展和定制

该类设计为可扩展的：

1. **添加新的状态方程**: 重写相关虚函数
2. **支持更多热力学模型**: 扩展NASA多项式计算
3. **添加新的性质**: 在派生类中添加新方法
4. **优化性能**: 改进计算算法和数据结构

## 贡献

欢迎贡献代码改进和功能扩展。请确保：

1. 遵循现有的代码风格
2. 添加适当的测试
3. 更新文档
4. 确保与现有功能兼容
