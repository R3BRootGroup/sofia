# Create a library called "libR3BSofAT" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES ${SYSTEM_INCLUDE_DIRECTORIES} ${BASE_INCLUDE_DIRECTORIES} )

set(INCLUDE_DIRECTORIES
#put here all directories where header files are located
${R3BROOT_SOURCE_DIR}/r3bbase
${R3BROOT_SOURCE_DIR}/r3bdata
${R3BROOT_SOURCE_DIR}/passive
${R3BSOF_SOURCE_DIR}/at
${R3BSOF_SOURCE_DIR}/sofdata
${R3BSOF_SOURCE_DIR}/sofdata/atData
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES ${ROOT_LIBRARY_DIR} ${FAIRROOT_LIBRARY_DIR} )

link_directories( ${LINK_DIRECTORIES})

set(SRCS
#Put here your sourcefiles
R3BSofAT.cxx
#R3BSofATHitPar.cxx
#R3BSofATCalPar.cxx
#R3BSofATMappedPar.cxx
#R3BSofATCal2Hit.cxx
#R3BSofATMapped2Cal.cxx
#R3BSofATCal2HitPar.cxx
#R3BSofATMapped2CalPar.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

set(LINKDEF SofATLinkDef.h)
set(LIBRARY_NAME R3BSofAT)
set(DEPENDENCIES
    R3Bbase R3BSofData)

GENERATE_LIBRARY()
