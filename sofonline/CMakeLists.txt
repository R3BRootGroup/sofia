# Create a library called "libR3BSofOnline" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES ${SYSTEM_INCLUDE_DIRECTORIES} ${BASE_INCLUDE_DIRECTORIES} )

set(INCLUDE_DIRECTORIES
#put here all directories where header files are located
${R3BROOT_SOURCE_DIR}/r3bbase
${R3BROOT_SOURCE_DIR}/r3bdata
${R3BROOT_SOURCE_DIR}/r3bdata/musicData
${R3BROOT_SOURCE_DIR}/music
${R3BROOT_SOURCE_DIR}/tracker/unpackams
${R3BROOT_SOURCE_DIR}/califa/unpack
${R3BSOF_SOURCE_DIR}/sofdata
${R3BSOF_SOURCE_DIR}/sofdata/atData
${R3BSOF_SOURCE_DIR}/sofdata/mwpcData
${R3BSOF_SOURCE_DIR}/sofdata/twimData
${R3BSOF_SOURCE_DIR}/sofdata/sciData
${R3BSOF_SOURCE_DIR}/sofdata/tofwData
${R3BSOF_SOURCE_DIR}/sofonline
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES ${ROOT_LIBRARY_DIR} ${FAIRROOT_LIBRARY_DIR} ) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
R3BSofOnlineSpectra.cxx
R3BSofMwpcOnlineSpectra.cxx
R3BSofMwpcCorrelationOnlineSpectra.cxx
R3BSofAtOnlineSpectra.cxx
R3BSofTwimOnlineSpectra.cxx
R3BSofSciOnlineSpectra.cxx
R3BSofToFWOnlineSpectra.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

set(LINKDEF R3BSofOnlineLinkDef.h)
set(LIBRARY_NAME R3BSofOnline)
set(DEPENDENCIES
    Spectrum Base FairTools R3Bbase R3BData)

GENERATE_LIBRARY()
