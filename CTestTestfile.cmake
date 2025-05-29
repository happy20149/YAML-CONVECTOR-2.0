# CMake generated Testfile for 
# Source directory: D:/思朗-相关资料/cantera重构-5.27/yaml-convector-2.0/yaml-convector-2.0
# Build directory: D:/思朗-相关资料/cantera重构-5.27/yaml-convector-2.0/yaml-convector-2.0
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(ComprehensiveDemo "D:/思朗-相关资料/cantera重构-5.27/yaml-convector-2.0/yaml-convector-2.0/bin/Debug/comprehensive_demo.exe")
  set_tests_properties(ComprehensiveDemo PROPERTIES  _BACKTRACE_TRIPLES "D:/思朗-相关资料/cantera重构-5.27/yaml-convector-2.0/yaml-convector-2.0/CMakeLists.txt;86;add_test;D:/思朗-相关资料/cantera重构-5.27/yaml-convector-2.0/yaml-convector-2.0/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(ComprehensiveDemo "D:/思朗-相关资料/cantera重构-5.27/yaml-convector-2.0/yaml-convector-2.0/bin/Release/comprehensive_demo.exe")
  set_tests_properties(ComprehensiveDemo PROPERTIES  _BACKTRACE_TRIPLES "D:/思朗-相关资料/cantera重构-5.27/yaml-convector-2.0/yaml-convector-2.0/CMakeLists.txt;86;add_test;D:/思朗-相关资料/cantera重构-5.27/yaml-convector-2.0/yaml-convector-2.0/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(ComprehensiveDemo "D:/思朗-相关资料/cantera重构-5.27/yaml-convector-2.0/yaml-convector-2.0/bin/MinSizeRel/comprehensive_demo.exe")
  set_tests_properties(ComprehensiveDemo PROPERTIES  _BACKTRACE_TRIPLES "D:/思朗-相关资料/cantera重构-5.27/yaml-convector-2.0/yaml-convector-2.0/CMakeLists.txt;86;add_test;D:/思朗-相关资料/cantera重构-5.27/yaml-convector-2.0/yaml-convector-2.0/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(ComprehensiveDemo "D:/思朗-相关资料/cantera重构-5.27/yaml-convector-2.0/yaml-convector-2.0/bin/RelWithDebInfo/comprehensive_demo.exe")
  set_tests_properties(ComprehensiveDemo PROPERTIES  _BACKTRACE_TRIPLES "D:/思朗-相关资料/cantera重构-5.27/yaml-convector-2.0/yaml-convector-2.0/CMakeLists.txt;86;add_test;D:/思朗-相关资料/cantera重构-5.27/yaml-convector-2.0/yaml-convector-2.0/CMakeLists.txt;0;")
else()
  add_test(ComprehensiveDemo NOT_AVAILABLE)
endif()
