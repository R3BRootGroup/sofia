# Create a library called "libR3BSofTofW" which includes the source files given in
# the array. The extension is already found. Any number of sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES ${SYSTEM_INCLUDE_DIRECTORIES} ${BASE_INCLUDE_DIRECTORIES} )

set(INCLUDE_DIRECTORIES
#put here all directories where header files are located
${R3BROOT_SOURCE_DIR}/r3bbase
${R3BROOT_SOURCE_DIR}/r3bdata
${R3BROOT_SOURCE_DIR}/tracking
${R3BSOF_SOURCE_DIR}/sofdata
${R3BSOF_SOURCE_DIR}/sofdata/tofwData
${R3BSOF_SOURCE_DIR}/tcal
${R3BSOF_SOURCE_DIR}/sofdata/sciData
${R3BSOF_SOURCE_DIR}/sci
${R3BSOF_SOURCE_DIR}/tofwall
${R3BSOF_SOURCE_DIR}/tofwall/sim
${R3BSOF_SOURCE_DIR}/tofwall/digi
${R3BSOF_SOURCE_DIR}/tofwall/pars
${R3BSOF_SOURCE_DIR}/tofwall/calibration
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES ${ROOT_LIBRARY_DIR} ${FAIRROOT_LIBRARY_DIR} )

link_directories( ${LINK_DIRECTORIES})

file(GLOB SRCS ./sim/*.cxx ./digi/*.cxx ./pars/*.cxx ./calibration/*.cxx)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

set(LINKDEF SofTofWLinkDef.h)
set(LIBRARY_NAME R3BSofTofW)
set(DEPENDENCIES
    R3BData R3BSofTcal R3BSofData R3BTracking)

GENERATE_LIBRARY()

if(BUILD_GEOMETRY)
  add_subdirectory(geobase)
endif()
add_subdirectory(test)
