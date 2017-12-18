# Create a library called "libR3BSofTofWall" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
)

set(INCLUDE_DIRECTORIES
#put here all directories where header files are located
${R3BROOT_SOURCE_DIR}/r3bbase
${R3BROOT_SOURCE_DIR}/r3bdata
${R3BROOT_SOURCE_DIR}/passive
${R3BROOT_SOURCE_DIR}/sofia/tofwall
${R3BROOT_SOURCE_DIR}/sofia/sofdata
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
)

link_directories( ${LINK_DIRECTORIES})

set(SRCS
#Put here your sourcefiles
R3BSofTofWall.cxx
#R3BSofTofWallHitPar.cxx
#R3BSofTofWallCalPar.cxx
#R3BSofTofWallMappedPar.cxx
#R3BSofTofWallCal2Hit.cxx
#R3BSofTofWallMapped2Cal.cxx
#R3BSofTofWallCal2HitPar.cxx
#R3BSofTofWallMapped2CalPar.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

set(LINKDEF SofTofWallLinkDef.h)
set(LIBRARY_NAME R3BSofTofWall)
set(DEPENDENCIES
    R3Bbase R3BPassive R3BSofData)

if(FAIRDB_FOUND)
set(DEPENDENCIES
    ${DEPENDENCIES} FairDB)
endif(FAIRDB_FOUND)

GENERATE_LIBRARY()
