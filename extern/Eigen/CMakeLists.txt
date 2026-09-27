# Try to find the Eigen package. If it’s not there,
# try downloading it and building it from source.

set(_eigen_brief "Eigen (C++ template library for linear algebra)")
set(_eigen_commit_hash "bc3b39870ecb690a623a3f49149a358b95c5781d")
set(_eigen_release_tag "5.0.1") # Nov 8, 2025

include(FetchContent)

FetchContent_Declare(
  eigen_project
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG        ${_eigen_commit_hash}
  GIT_SHALLOW    TRUE
  FIND_PACKAGE_ARGS NAMES Eigen3 CONFIG GLOBAL
)

FetchContent_MakeAvailable(eigen_project)

if (eigen_project_FOUND)
  message(STATUS "${_eigen_brief}, version ${eigen_project_VERSION} found")
elseif(eigen_project_POPULATED)
  message(STATUS "${_eigen_brief}, version ${_eigen_release_tag} populated.")
else()
  message(FATAL_ERROR "${_eigen_brief} not found")
endif()
