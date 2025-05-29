# 清理yaml-convector-2.0项目，只保留核心文件
# 保留文件列表：
# ChemistryIO.h/cpp, ChemistryVars.h/cpp, CMakeLists.txt, comprehensive_demo.cpp
# h2o2.yaml, IdealGasPhase.cpp/h, mechanism.yaml, MechanismTest.cpp/h
# C2H4-mechanism.yaml, yaml-convector-2.0.cpp

Write-Host "开始清理yaml-convector-2.0项目..." -ForegroundColor Green

# 保存当前目录
$originalPath = Get-Location
$projectPath = "d:\思朗-相关资料\cantera重构-5.27\yaml-convector-2.0\yaml-convector-2.0"

# 切换到项目目录
Set-Location $projectPath

# 定义需要保留的文件
$keepFiles = @(
    "ChemistryIO.h",
    "ChemistryIO.cpp", 
    "ChemistryVars.h",
    "ChemistryVars.cpp",
    "CMakeLists.txt",
    "comprehensive_demo.cpp",
    "h2o2.yaml",
    "IdealGasPhase.cpp",
    "IdealGasPhase.h", 
    "mechanism.yaml",
    "MechanismTest.cpp",
    "MechanismTest.h",
    "C2H4-mechanism.yaml",
    "yaml-convector-2.0.cpp",
    "CMakeLists_minimal.txt"  # 备份的最小化CMakeLists
)

# 定义需要保留的目录（如果为空将被删除）
$keepDirectories = @(
    ".git"  # 保留git目录
)

Write-Host "保留的文件列表：" -ForegroundColor Yellow
$keepFiles | ForEach-Object { Write-Host "  $_" -ForegroundColor Cyan }

# 备份当前的CMakeLists.txt
if (Test-Path "CMakeLists.txt") {
    Copy-Item "CMakeLists.txt" "CMakeLists_backup_$(Get-Date -Format 'yyyyMMdd_HHmmss').txt"
    Write-Host "已备份当前CMakeLists.txt" -ForegroundColor Green
}

# 获取所有文件和目录
$allItems = Get-ChildItem -Force

# 删除不在保留列表中的文件
Write-Host "`n开始删除不需要的文件..." -ForegroundColor Yellow
foreach ($item in $allItems) {
    if ($item.PSIsContainer) {
        # 处理目录
        if ($keepDirectories -notcontains $item.Name) {
            Write-Host "删除目录: $($item.Name)" -ForegroundColor Red
            Remove-Item $item.FullName -Recurse -Force -ErrorAction SilentlyContinue
        } else {
            Write-Host "保留目录: $($item.Name)" -ForegroundColor Green
        }
    } else {
        # 处理文件
        if ($keepFiles -notcontains $item.Name) {
            Write-Host "删除文件: $($item.Name)" -ForegroundColor Red
            Remove-Item $item.FullName -Force -ErrorAction SilentlyContinue
        } else {
            Write-Host "保留文件: $($item.Name)" -ForegroundColor Green
        }
    }
}

# 使用最小化的CMakeLists.txt替换原来的
if (Test-Path "CMakeLists_minimal.txt") {
    Copy-Item "CMakeLists_minimal.txt" "CMakeLists.txt" -Force
    Write-Host "`n已使用最小化的CMakeLists.txt替换原文件" -ForegroundColor Green
}

# 创建bin和build目录
if (-not (Test-Path "bin")) {
    New-Item -ItemType Directory -Name "bin" -Force | Out-Null
    Write-Host "创建bin目录" -ForegroundColor Green
}

if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Name "build" -Force | Out-Null  
    Write-Host "创建build目录" -ForegroundColor Green
}

Write-Host "`n项目清理完成！" -ForegroundColor Green
Write-Host "保留的文件：" -ForegroundColor Yellow
Get-ChildItem -File | ForEach-Object { Write-Host "  $($_.Name)" -ForegroundColor Cyan }

Write-Host "`n下一步：" -ForegroundColor Yellow
Write-Host "1. 运行 cmake . 配置项目" -ForegroundColor White  
Write-Host "2. 运行 cmake --build . --config Release 编译项目" -ForegroundColor White
Write-Host "3. 测试 .\bin\Release\comprehensive_demo.exe 和 .\bin\Release\yaml_convector.exe" -ForegroundColor White

# 恢复原目录
Set-Location $originalPath
