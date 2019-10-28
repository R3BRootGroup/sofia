# Create a library called "libR3BSofMWPC" which includes the source files given in
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
${R3BSOF_SOURCE_DIR}/mwpc3
${R3BSOF_SOURCE_DIR}/sofdata
${R3BSOF_SOURCE_DIR}/sofdata/mwpcData
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
R3BSofMwpc3.cxx
R3BSofMwpc3CalPar.cxx
R3BSofMwpc3ContFact.cxx
R3BSofMwpc3Mapped2Cal.cxx
R3BSofMwpc3Mapped2CalPar.cxx
R3BSofMwpc3Cal2Hit.cxx


)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

set(LINKDEF SofMWPCLinkDef.h)
set(LIBRARY_NAME R3BSofMwpc3)
set(DEPENDENCIES
    R3Bbase R3BData R3BSofData)

GENERATE_LIBRARY()
