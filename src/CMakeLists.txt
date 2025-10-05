# src/CMakeLists.txt
# Source subdirectory management

# Add subdirectories if they exist
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ipl_compiler)
    # ipl_compiler sources are handled directly in the main CMakeLists.txt
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/istudio)
    # istudio sources are handled directly in the main CMakeLists.txt
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/semantic)
    # semantic sources are handled directly in the main CMakeLists.txt
endif()

# This subdirectory just serves to organize the build structure
# Actual source grouping is done in the main CMakeLists.txt