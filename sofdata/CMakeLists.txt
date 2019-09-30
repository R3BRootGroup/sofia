# Create a library called "libR3BSOFDATA" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual")

Set(SYSTEM_INCLUDE_DIRECTORIES
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
)

set(INCLUDE_DIRECTORIES
${R3BROOT_SOURCE_DIR}/r3bdata
${R3BROOT_SOURCE_DIR}/r3bdata/califaData
${R3BROOT_SOURCE_DIR}/r3bdata/neulandData
${R3BSOF_SOURCE_DIR}/sofdata
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
)

link_directories( ${LINK_DIRECTORIES})

set(SRCS
R3BSofATPoint.cxx
R3BSofComRefMappedData.cxx
R3BSofSciMappedData.cxx
R3BSofSciTcalData.cxx
R3BSofSciCalData.cxx
R3BSofToFWMappedData.cxx
R3BSofToFWCalData.cxx
R3BSofMWPCPoint.cxx
R3BSofTWIMPoint.cxx
R3BSofTwimMappedData.cxx
R3BSofTwimCalData.cxx
R3BSofToFWPoint.cxx
R3BSofMWPCPoint2.cxx
)


# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
SET(HEADERS ${HEADERS} ${R3BROOT_SOURCE_DIR}/r3bdata/R3BDetectorList.h )

set(LINKDEF  R3BSofDataLinkDef.h)
set(LIBRARY_NAME R3BSofData)
set(DEPENDENCIES
    Base FairTools Core m)

GENERATE_LIBRARY()
