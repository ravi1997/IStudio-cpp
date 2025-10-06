#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "IStudio::IStudio" for configuration "Debug"
set_property(TARGET IStudio::IStudio APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(IStudio::IStudio PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/IStudio"
  )

list(APPEND _cmake_import_check_targets IStudio::IStudio )
list(APPEND _cmake_import_check_files_for_IStudio::IStudio "${_IMPORT_PREFIX}/bin/IStudio" )

# Import target "IStudio::ipl_compiler" for configuration "Debug"
set_property(TARGET IStudio::ipl_compiler APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(IStudio::ipl_compiler PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/ipl_compiler"
  )

list(APPEND _cmake_import_check_targets IStudio::ipl_compiler )
list(APPEND _cmake_import_check_files_for_IStudio::ipl_compiler "${_IMPORT_PREFIX}/bin/ipl_compiler" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
