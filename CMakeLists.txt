cmake_minimum_required(VERSION 3.10)

project(garamon_generator)

# add a "output" directory in the project binary directory
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/output)

# include eigen
find_package(Eigen3 REQUIRED)

# files to compile
file(GLOB_RECURSE source_files src/*.cpp src/*.hpp)
message(STATUS "  files " ${source_files})

# compile
add_executable(garamon_generator ${source_files})

# link libs
if(TARGET Eigen3::Eigen)
    message(STATUS "lib EIGEN3 ${Eigen3_VERSION} found")
    target_link_libraries(garamon_generator PRIVATE Eigen3::Eigen)
else()
    message(STATUS "lib EIGEN3 found")
    include_directories(${EIGEN3_INCLUDE_DIR})
endif()

# compilation flags
target_compile_features(garamon_generator PRIVATE cxx_std_14) 
target_compile_options(garamon_generator PRIVATE -Wall -O2) 
