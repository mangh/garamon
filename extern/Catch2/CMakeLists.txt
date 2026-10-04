# Try to find the Catch2 package. If it’s not there,
# try downloading it and building it from source.

set(_catch2_brief "Catch2 (unit testing framework for C++)")
set(_catch2_commit_hash "317ac1ed4c0bb6e6b91eafc817e05c488feffcb3")
set(_catch2_release_tag "v3.16.0")  # August 2026

include(FetchContent)

FetchContent_Declare(
  catch2_project
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        ${_catch2_commit_hash}
  GIT_SHALLOW    TRUE
  FIND_PACKAGE_ARGS NAMES Catch2 CONFIG GLOBAL
)

FetchContent_MakeAvailable(catch2_project)

if (catch2_project_FOUND)
  message(STATUS "${_catch2_brief}, version ${catch2_project_VERSION} found")
elseif(catch2_project_POPULATED)
  message(STATUS "${_catch2_brief}, version ${_catch2_release_tag} populated.")
else()
  message(FATAL_ERROR "${_catch2_brief} not found")
endif()