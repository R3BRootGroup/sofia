# The name of our project is “R3BSOF”.  CMakeLists files in this project can
# refer to the R3BROOT root source directory of the project as ${R3BROOT_SOURCE_DIR},
# the SOFIA root source directory as ${R3BSOF_SOURCE_DIR} (for the sofia specifics) or
# as ${CMAKE_SOURCE_DIR} and to the root binary directory of the project as
# ${R3BSOF_BINARY_DIR} or ${CMAKE_BINARY_DIR}.

# Recurse into the given subdirectories.  This does not actually
# cause another cmake executable to run.  The same process will walk through
# the project's entire directory structure.

Set(R3BSOF_SOURCE_DIR ${R3BROOT_SOURCE_DIR}/sofia)

#SOFIA specific
add_subdirectory (sofdata)
if (WITH_UCESB)
add_subdirectory (sofsource)
endif (WITH_UCESB)
#add_subdirectory (sofmwpc)
#add_subdirectory (softwim)
#add_subdirectory (softrim)
#add_subdirectory (sofat)
#add_subdirectory (softof)

