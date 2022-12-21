# Create a library called "libR3BSofsource" which includes the source files given in
# the array. The extension is already found. Any number of sources could be listed here.

set(SYSTEM_INCLUDE_DIRECTORIES ${SYSTEM_INCLUDE_DIRECTORIES} ${BASE_INCLUDE_DIRECTORIES} ${ucesb_INCLUDE_DIR} )

set(INCLUDE_DIRECTORIES
#put here all directories where header files are located
${R3BROOT_SOURCE_DIR}/r3bsource
${R3BROOT_SOURCE_DIR}/r3bsource/base
${R3BROOT_SOURCE_DIR}/r3bbase
${R3BROOT_SOURCE_DIR}/r3bdata/wrData
${R3BSOF_SOURCE_DIR}/sofsource
${R3BSOF_SOURCE_DIR}/sofsource/ext
${R3BSOF_SOURCE_DIR}/sofdata
${R3BSOF_SOURCE_DIR}/sofdata/atData
${R3BSOF_SOURCE_DIR}/sofdata/sciData
${R3BSOF_SOURCE_DIR}/sofdata/trimData
${R3BSOF_SOURCE_DIR}/sofdata/twimData
${R3BSOF_SOURCE_DIR}/sofdata/mwpcData
${R3BSOF_SOURCE_DIR}/sofdata/tofwData
${R3BSOF_SOURCE_DIR}/sofdata/scalersData
${R3BSOF_SOURCE_DIR}/sofdata/corrData
)

set(LINK_DIRECTORIES ${ROOT_LIBRARY_DIR} ${FAIRROOT_LIBRARY_DIR} ${ucesb_LIBRARY_DIR} )

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})
link_directories( ${LINK_DIRECTORIES})

file(GLOB SRCS *.cxx)

Set(STRUCT_HEADERS
ext/ext_h101_wrsofia.h
ext/ext_h101_sofsci.h
ext/ext_h101_softofw.h
ext/ext_h101_softrim.h
ext/ext_h101_sofscalers.h
ext/ext_h101_sofat.h
ext/ext_h101_sofcorrm.h
ext/ext_h101_sofcorrv.h
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
Set(HEADERS ${STRUCT_HEADERS} ${HEADERS})

set(LINKDEF SofSourceLinkDef.h)
set(DEPENDENCIES
    R3BBase Base R3BSofData R3Bsource ${ucesb_LIBRARY_SHARED} FairTools)
set(LIBRARY_NAME R3BSofsource)

GENERATE_LIBRARY()
