import subprocess
import sys

# Test the specific composition: H2O:0.1, H2:0.8, AR:0.1 at T=1000K, P=101325Pa
print("=== 测试密度计算修复 ===")
print("条件: H2O:0.1, H2:0.8, AR:0.1 at T=1000K, P=101325Pa")
print()

# Manual calculation using the correct formula
# ρ = P × M̄ / (Ru × T)

# Molecular weights (kg/kmol)
MW_H2O = 18.01534
MW_H2 = 2.01588  
MW_AR = 39.948

# Mole fractions
x_H2O = 0.1
x_H2 = 0.8
x_AR = 0.1

# Calculate mean molecular weight: M̄ = Σ(xi × Mi)
mean_MW = x_H2O * MW_H2O + x_H2 * MW_H2 + x_AR * MW_AR
print(f"平均分子量计算:")
print(f"  M̄ = {x_H2O} × {MW_H2O} + {x_H2} × {MW_H2} + {x_AR} × {MW_AR}")
print(f"  M̄ = {x_H2O * MW_H2O:.6f} + {x_H2 * MW_H2:.6f} + {x_AR * MW_AR:.6f}")
print(f"  M̄ = {mean_MW:.6f} kg/kmol")
print()

# Conditions
T = 1000.0  # K
P = 101325.0  # Pa
Ru = 8314.462618  # J/(kmol·K) - updated standard value

# Calculate density using ideal gas law
density_new = (P * mean_MW) / (Ru * T)
print(f"使用修复后的公式计算密度:")
print(f"  ρ = P × M̄ / (Ru × T)")
print(f"  ρ = {P} × {mean_MW:.6f} / ({Ru} × {T})")
print(f"  ρ = {density_new:.8f} kg/m³")
print()

# Compare with old gas constant
Ru_old = 8314.462175  # Old value
density_old = (P * mean_MW) / (Ru_old * T)
print(f"使用旧气体常数的密度:")
print(f"  Ru_old = {Ru_old} J/(kmol·K)")
print(f"  ρ_old = {density_old:.8f} kg/m³")
print()

# Expected Cantera value (from previous analysis)
cantera_expected = 0.090292
print(f"=== 与标准Cantera比较 ===")
print(f"yaml-convector-2.0 (修复后): {density_new:.8f} kg/m³")
print(f"标准Cantera预期值:          {cantera_expected:.6f} kg/m³")

ratio_new = density_new / cantera_expected
ratio_old = density_old / cantera_expected

print(f"修复后密度比值: {ratio_new:.6f}")
print(f"修复前密度比值: {ratio_old:.6f}")
print()

if abs(ratio_new - 1.0) < 0.01:
    print("✓ 密度差异在1%以内，修复成功!")
else:
    print("✗ 密度差异仍然存在")
    print(f"  差异: {abs(ratio_new - 1.0) * 100:.2f}%")

print()
print("=== 分析总结 ===")
print(f"气体常数改变导致的密度变化: {(density_old/density_new):.8f}")
print(f"气体常数差异: {((Ru - Ru_old)/Ru_old * 100):.8f}%")
