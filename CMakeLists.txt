cmake_minimum_required(VERSION 3.10)

project(garamon_generator)

# To avoid portability issues, language extensions (enabled by default)
# are disabled before any target is created. This allows to stick to the
# language standard such as "-std=c++14" instead of "-std=gnu++14" (which
# is c++14 with GNU extensions):
set(CMAKE_CXX_EXTENSIONS OFF)

# include Eigen (C++ template library for linear algebra)
add_subdirectory( "extern/Eigen" )

# include CLI11 (command line parser for C++11)
add_subdirectory( "extern/CLI11" )

# add a "output" directory in the project binary directory
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/output)

# files to compile
file(GLOB_RECURSE source_files src/*.cpp src/*.hpp)
message(STATUS "${CMAKE_PROJECT_NAME} source files:")
foreach(src_file ${source_files})
  message(STATUS "  ${src_file}")
endforeach()

# compile
add_executable(garamon_generator ${source_files})

# link libs
target_link_libraries(garamon_generator PRIVATE
  Eigen3::Eigen
  CLI11::CLI11
)

# compilation flags
target_compile_features(garamon_generator PRIVATE cxx_std_14)
target_compile_options(garamon_generator PRIVATE $<IF:$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},MSVC>,/W3,-Wall>)
