cmake_minimum_required(VERSION 3.10)

project(cmake_project_name_original_case CXX)
set(CMAKE_BUILD_TYPE Release)

# include eigen
find_package(Eigen3 REQUIRED)


# python binding
option(BUILD_PYTHON "Build the Python binding (requires Python 3 and pybind11)" OFF)


# call the CMakeLists.txt to make the documentation (Doxygen)
# > 'make html' to generate the documentation
find_package(Doxygen OPTIONAL_COMPONENTS QUIET)
if(DOXYGEN_FOUND)
    add_subdirectory(doc)
else()
    message(WARNING "Doxygen not found, skip dococumentation")
endif()


# files to compile
set(source_files src/cmake_project_name_original_case/Mvec.cpp)
file(GLOB_RECURSE header_files src/cmake_project_name_original_case/*.hpp)

# display info
message(STATUS "  sources")
foreach(src_file ${source_files})
    message("          " ${src_file})
endforeach()
message(STATUS "  headers")
foreach(src_file ${header_files})
    message("          " ${src_file})
endforeach()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# generate a library
if (MSVC)
	add_library(cmake_project_name_original_case STATIC ${source_files} ${header_files})
else()
	add_library(cmake_project_name_original_case SHARED ${source_files} ${header_files})
endif()

if(TARGET Eigen3::Eigen)
    message(STATUS "lib EIGEN3 ${Eigen3_VERSION} found")
    target_link_libraries(cmake_project_name_original_case PUBLIC Eigen3::Eigen)
else()
    include_directories(${EIGEN3_INCLUDE_DIR})
endif()

if (BUILD_PYTHON)
    if (CMAKE_VERSION VERSION_LESS 3.18)
        message(FATAL_ERROR "BUILD_PYTHON requires CMake 3.18 or more")
    endif()

    # the Python used to build the module is the one that will import it (an active venv is honoured)
    find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)

    # locate pybind11 installed with 'pip install pybind11'
    execute_process(COMMAND "${Python_EXECUTABLE}" -m pybind11 --cmakedir
                    OUTPUT_VARIABLE pybind11_cmake_dir
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
    set(PYBIND11_FINDPYTHON ON)
    find_package(pybind11 CONFIG REQUIRED HINTS "${pybind11_cmake_dir}")

    # standalone module (the library is header-only in practice): no runtime dependency on the shared library
    pybind11_add_module(cmake_project_name_original_case_py
        src/cmake_project_name_original_case/PythonBindings.cpp
        src/cmake_project_name_original_case/Mvec.cpp
    )
    target_include_directories(cmake_project_name_original_case_py PRIVATE src)
    target_compile_features(cmake_project_name_original_case_py PRIVATE cxx_std_14)
    if(TARGET Eigen3::Eigen)
        target_link_libraries(cmake_project_name_original_case_py PRIVATE Eigen3::Eigen)
    else()
        target_include_directories(cmake_project_name_original_case_py PRIVATE ${EIGEN3_INCLUDE_DIR})
    endif()

    # > 'cmake --install build --component python' to install the module into the site-packages
    set(PYTHON_INSTALL_DIR "${Python_SITEARCH}" CACHE PATH "Installation directory of the Python module")
    install(TARGETS cmake_project_name_original_case_py
            LIBRARY DESTINATION "${PYTHON_INSTALL_DIR}" COMPONENT python
            RUNTIME DESTINATION "${PYTHON_INSTALL_DIR}" COMPONENT python)

    # > 'cmake --build build --target python_sample' to run sample/sample.py with the just-built module
    add_custom_target(python_sample
        COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=$<TARGET_FILE_DIR:cmake_project_name_original_case_py>"
                "${Python_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/sample/sample.py"
        DEPENDS cmake_project_name_original_case_py
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/sample"
        VERBATIM)
endif()

# compilation flags
if (MSVC)   
    target_compile_features(cmake_project_name_original_case PRIVATE cxx_std_14) 
    target_compile_options(cmake_project_name_original_case PRIVATE -Wall -O2) 
else()
    target_compile_features(cmake_project_name_original_case PRIVATE cxx_std_14) 
    target_compile_options(cmake_project_name_original_case PRIVATE -Wall -O2 -Wno-return-local-addr)
endif()
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftime-report ") # comilation profiling:  -Q -ftime-report
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-elide-constructors") # force the compilor to use our move 'copy constructor'

# include directory path
include_directories(src)

# install lib
install(FILES ${header_files} ${source_files} DESTINATION /usr/local/include/cmake_project_name_original_case)
install(TARGETS cmake_project_name_original_case
        RUNTIME DESTINATION /usr/local/bin
        LIBRARY DESTINATION /usr/local/lib
        ARCHIVE DESTINATION /usr/local/lib)

