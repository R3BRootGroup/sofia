# Create a library called "libR3BSofTwim" which includes the source files given in
# the array. The extension is already found. Any number of sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES ${SYSTEM_INCLUDE_DIRECTORIES} ${BASE_INCLUDE_DIRECTORIES})

set(INCLUDE_DIRECTORIES
#put here all directories where header files are located
${R3BROOT_SOURCE_DIR}/r3bbase
${R3BROOT_SOURCE_DIR}/r3bdata
${R3BROOT_SOURCE_DIR}/tracking
${R3BSOF_SOURCE_DIR}/sofdata
${R3BSOF_SOURCE_DIR}/sofdata/twimData
${R3BSOF_SOURCE_DIR}/sofdata/tofwData
${R3BSOF_SOURCE_DIR}/sofdata/mwpcData
${R3BSOF_SOURCE_DIR}/tcal
${R3BSOF_SOURCE_DIR}/twim
${R3BSOF_SOURCE_DIR}/twim/sim
${R3BSOF_SOURCE_DIR}/twim/par
${R3BSOF_SOURCE_DIR}/twim/calibration
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES ${ROOT_LIBRARY_DIR} ${FAIRROOT_LIBRARY_DIR} )

link_directories( ${LINK_DIRECTORIES})

#set(SRCS
#Put here your sourcefiles
#*.cxx
#./sim/R3BSofTwim.cxx
#./sim/R3BSofTwimDigitizer.cxx
#./par/R3BSofTwimContFact.cxx
#./par/R3BSofTwimCalPar.cxx
#./par/R3BSofTwimHitPar.cxx
#./calibration/R3BSofTwimMapped2Cal.cxx
#./calibration/R3BSofTwimCal2Hit.cxx
#./calibration/R3BSofTwimMapped2CalPar.cxx
#)

file(GLOB SRCS ./sim/*.cxx ./par/*.cxx ./calibration/*.cxx)


# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

set(LINKDEF SofTwimLinkDef.h)
set(LIBRARY_NAME R3BSofTwim)
set(DEPENDENCIES
    R3Bbase R3BData R3BSofData R3BSofTcal)

GENERATE_LIBRARY()
