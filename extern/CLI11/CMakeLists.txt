# Try to find the CLI11 package. If it’s not there,
# try downloading it and building it from source.

set(_cli11_brief "CLI11 (command line parser for C++11)")
set(_cli11_commit_hash "cbd58a3696887b34c70949aef21a71735a0c2ad5")
set(_cli11_release_tag "v2.7.2") # Aug 2, 2026

include(FetchContent)

FetchContent_Declare(
  cli11_project
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
  GIT_TAG        ${_cli11_commit_hash}
  GIT_SHALLOW    TRUE
  FIND_PACKAGE_ARGS NAMES CLI11 CONFIG GLOBAL
)

FetchContent_MakeAvailable(cli11_project)

if (cli11_project_FOUND)
  message(STATUS "${_cli11_brief}, version ${cli11_project_VERSION} found")
elseif(cli11_project_POPULATED)
  message(STATUS "${_cli11_brief}, version ${_cli11_release_tag} populated.")
else()
  message(FATAL_ERROR "${_cli11_brief} not found")
endif()