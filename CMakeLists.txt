cmake_minimum_required(VERSION 3.12)
project(YamlConvector2_IdealGas_Minimal)

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

# 核心源文件 - 仅保留必需文件
set(CORE_SOURCES
    ChemistryVars.cpp
    ChemistryIO.cpp
    IdealGasPhase.cpp
)

set(CORE_HEADERS
    ChemistryVars.h
    ChemistryIO.h
    IdealGasPhase.h
    MechanismTest.h
)

# 创建核心库
add_library(idealgas_core STATIC ${CORE_SOURCES} ${CORE_HEADERS})
target_link_libraries(idealgas_core ${YAML_CPP_LIBRARIES})

# 综合演示程序 - 主要功能
add_executable(comprehensive_demo comprehensive_demo.cpp)
target_link_libraries(comprehensive_demo idealgas_core)

# 主程序 - 保留原始功能
add_executable(yaml_convector yaml-convector-2.0.cpp MechanismTest.cpp)
target_link_libraries(yaml_convector idealgas_core)

# 设置输出目录
set_target_properties(comprehensive_demo yaml_convector
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

install(TARGETS comprehensive_demo yaml_convector
    RUNTIME DESTINATION bin
)

# 测试
enable_testing()
add_test(NAME ComprehensiveDemo COMMAND comprehensive_demo)

# 打印配置信息
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
message(STATUS "YAML_CPP_INCLUDE_DIRS: ${YAML_CPP_INCLUDE_DIRS}")
message(STATUS "YAML_CPP_LIBRARY_DIRS: ${YAML_CPP_LIBRARY_DIRS}")
message(STATUS "YAML_CPP_LIBRARIES: ${YAML_CPP_LIBRARIES}")