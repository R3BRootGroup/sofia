# Create a library called "libR3BSofMwpc0" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES ${SYSTEM_INCLUDE_DIRECTORIES} ${BASE_INCLUDE_DIRECTORIES})

set(INCLUDE_DIRECTORIES
#put here all directories where header files are located
${R3BROOT_SOURCE_DIR}/r3bbase
${R3BROOT_SOURCE_DIR}/r3bdata
${R3BSOF_SOURCE_DIR}/mwpc0
${R3BSOF_SOURCE_DIR}/sofdata
${R3BSOF_SOURCE_DIR}/sofdata/mwpcData
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES ${ROOT_LIBRARY_DIR} ${FAIRROOT_LIBRARY_DIR} )

link_directories( ${LINK_DIRECTORIES})

set(SRCS
#Put here your sourcefiles
R3BSofMwpc0.cxx
R3BSofMwpc0ContFact.cxx
R3BSofMwpc0CalPar.cxx
R3BSofMwpc0Mapped2Cal.cxx
R3BSofMwpc0Mapped2CalPar.cxx
R3BSofMwpc0Cal2Hit.cxx
#R3BSofMWPCHitPar.cxx
#R3BSofMWPCCalPar.cxx
#R3BSofMWPCMappedPar.cxx
#R3BSofMWPCCal2Hit.cxx
#R3BSofMWPCCal2HitPar.cxx
#R3BSofMWPCMapped2CalPar.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

set(LINKDEF SofMWPCLinkDef.h)
set(LIBRARY_NAME R3BSofMwpc0)
set(DEPENDENCIES
    R3Bbase R3BData R3BSofData)

GENERATE_LIBRARY()
