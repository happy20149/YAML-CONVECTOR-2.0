cmake_minimum_required(VERSION 3.12)
project(YamlConvector2_IdealGas)

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找yaml-cpp库
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(YAML_CPP yaml-cpp)
endif()

if(NOT YAML_CPP_FOUND)
    # 尝试使用find_package
    find_package(yaml-cpp QUIET)
    if(yaml-cpp_FOUND)
        set(YAML_CPP_LIBRARIES yaml-cpp)
    else()
        # 如果找不到，设置默认路径
        set(YAML_CPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../fluid.yaml-cpp.1.0.10/build/include")
        set(YAML_CPP_LIBRARY_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../fluid.yaml-cpp.1.0.10/build/lib")
        set(YAML_CPP_LIBRARIES "yaml-cpp")
    endif()
endif()

# 包含目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
if(YAML_CPP_INCLUDE_DIRS)
    include_directories(${YAML_CPP_INCLUDE_DIRS})
endif()

# 链接目录
if(YAML_CPP_LIBRARY_DIRS)
    link_directories(${YAML_CPP_LIBRARY_DIRS})
endif()

# 源文件
set(CORE_SOURCES
    ChemistryVars.cpp
    ChemistryIO.cpp
    IdealGasPhase.cpp
)

set(CORE_HEADERS
    ChemistryVars.h
    ChemistryIO.h
    IdealGasPhase.h
)

# 创建核心库
add_library(idealgas_core STATIC ${CORE_SOURCES} ${CORE_HEADERS})
target_link_libraries(idealgas_core ${YAML_CPP_LIBRARIES})

# 简单测试程序
add_executable(simple_test simple_test.cpp)
target_link_libraries(simple_test idealgas_core)

# 集成测试程序
add_executable(integration_test integration_test.cpp)
target_link_libraries(integration_test idealgas_core)

# Cantera兼容性演示
add_executable(cantera_demo cantera_compatibility_demo.cpp)
target_link_libraries(cantera_demo idealgas_core)

# 综合演示
add_executable(comprehensive_demo comprehensive_demo.cpp)
target_link_libraries(comprehensive_demo idealgas_core)

# 原始demo
add_executable(demo_idealgas demo_idealgas.cpp)
target_link_libraries(demo_idealgas idealgas_core)

# 精确Cantera工作流程测试
add_executable(exact_cantera_test exact_cantera_test.cpp)
target_link_libraries(exact_cantera_test idealgas_core)

# 增强setState_TPX功能测试
add_executable(test_enhanced_setState_TPX test_enhanced_setState_TPX.cpp)
target_link_libraries(test_enhanced_setState_TPX idealgas_core)

# 目标密度测试
add_executable(test_target_density test_target_density.cpp)
target_link_libraries(test_target_density idealgas_core)

# 主程序
add_executable(yaml_convector yaml-convector-2.0.cpp MechanismTest.cpp)
target_link_libraries(yaml_convector idealgas_core)

# 设置输出目录
set_target_properties(simple_test integration_test cantera_demo comprehensive_demo demo_idealgas exact_cantera_test test_enhanced_setState_TPX test_target_density yaml_convector
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# 安装规则
install(TARGETS idealgas_core
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES ${CORE_HEADERS}
    DESTINATION include
)

install(TARGETS simple_test integration_test cantera_demo comprehensive_demo demo_idealgas exact_cantera_test test_enhanced_setState_TPX test_target_density yaml_convector
    RUNTIME DESTINATION bin
)

# 创建一个测试目标
enable_testing()
add_test(NAME SimpleTest COMMAND simple_test)
add_test(NAME IntegrationTest COMMAND integration_test)

# 打印配置信息
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
message(STATUS "YAML_CPP_INCLUDE_DIRS: ${YAML_CPP_INCLUDE_DIRS}")
message(STATUS "YAML_CPP_LIBRARY_DIRS: ${YAML_CPP_LIBRARY_DIRS}")
message(STATUS "YAML_CPP_LIBRARIES: ${YAML_CPP_LIBRARIES}")
