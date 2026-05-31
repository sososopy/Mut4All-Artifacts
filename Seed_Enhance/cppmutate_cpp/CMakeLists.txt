cmake_minimum_required(VERSION 3.5)
project(cppmutate VERSION 0.1.0)
find_package(PkgConfig REQUIRED)
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)
pkg_check_modules(JSONCPP jsoncpp)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Using ClangConfig.cmake in: ${Clang_DIR}")


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")

include_directories(
  ${LLVM_INCLUDE_DIRS}
  ${CLANG_INCLUDE_DIRS}
  progress_bar_cpp/include
)

add_definitions(${LLVM_DEFINITIONS})

add_executable(cppmutate
    progress_bar_cpp/src/ProgressBar.cpp
    main.cpp
    do_visit.cpp
    mutator.cpp
    conf.cpp
)

target_link_libraries(cppmutate   PRIVATE
    clang-cpp
    LLVM
    ${JSONCPP_LIBRARIES}
)
target_link_options(cppmutate PRIVATE
    "-Wl,-z,stack-size=2147483648"
)
set_target_properties(cppmutate PROPERTIES CXX_STANDARD 17 OUTPUT_NAME cppast)

install(TARGETS cppmutate)