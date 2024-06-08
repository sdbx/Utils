cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("hhss_cpp")

add_executable (hhss_cpp "hhss_cpp.cpp" "hhss_cpp.h" "generator.cpp" "generator.h"     "random_formatting_map.cpp" "random_formatting_map.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET hhss_cpp PROPERTY CXX_STANDARD 20)
endif()
