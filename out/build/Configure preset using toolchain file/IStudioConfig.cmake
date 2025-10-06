# IStudioConfig.cmake.in
# Configuration file for finding IStudio package


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was IStudioConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

# Set the installed library directory relative to the config file
set_and_check(IStudio_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}")
set_and_check(IStudio_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}/../../../..")

# Import the targets
if(NOT TARGET IStudio::IStudio)
    include("${CMAKE_CURRENT_LIST_DIR}/IStudioTargets.cmake")
endif()

# Set the include directories
set(IStudio_INCLUDE_DIRS "${PACKAGE_PREFIX_DIR}/include")

# Set the libraries
set(IStudio_LIBRARIES IStudio)

check_required_components(IStudio)
