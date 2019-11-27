# Create a library called "libR3BSofTCal" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES ${SYSTEM_INCLUDE_DIRECTORIES} ${BASE_INCLUDE_DIRECTORIES} )

set(INCLUDE_DIRECTORIES
#put here all directories where header files are located
${R3BROOT_SOURCE_DIR}/r3bbase
${R3BROOT_SOURCE_DIR}/passive
${R3BSOF_SOURCE_DIR}/sofdata
${R3BSOF_SOURCE_DIR}/sofdata/sciData
${R3BSOF_SOURCE_DIR}/sofdata/tofwData
${R3BSOF_SOURCE_DIR}/tcal
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES ${ROOT_LIBRARY_DIR} ${FAIRROOT_LIBRARY_DIR} )

link_directories( ${LINK_DIRECTORIES})

set(SRCS
R3BSofTcalPar.cxx
R3BSofTcalContFact.cxx
R3BSofSciMapped2TcalPar.cxx
R3BSofSciMapped2Tcal.cxx
R3BSofToFWMapped2TcalPar.cxx
R3BSofToFWMapped2Tcal.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF SofTcalLinkDef.h)
Set(LIBRARY_NAME R3BSofTcal)
Set(DEPENDENCIES
    Base ParBase Core m)

if(FAIRDB_FOUND)
set(DEPENDENCIES
    ${DEPENDENCIES} FairDB)
endif(FAIRDB_FOUND)

GENERATE_LIBRARY()

