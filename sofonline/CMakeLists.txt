# Create a library called "libR3BSofOnline" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES ${SYSTEM_INCLUDE_DIRECTORIES} ${BASE_INCLUDE_DIRECTORIES} )

set(INCLUDE_DIRECTORIES
#put here all directories where header files are located
${R3BROOT_SOURCE_DIR}/r3bbase
${R3BROOT_SOURCE_DIR}/r3bdata
${R3BROOT_SOURCE_DIR}/r3bdata/frsData
${R3BROOT_SOURCE_DIR}/music/online
${R3BROOT_SOURCE_DIR}/music/pars
${R3BROOT_SOURCE_DIR}/twim/online
${R3BROOT_SOURCE_DIR}/mwpc/online
${R3BROOT_SOURCE_DIR}/passive
${R3BROOT_SOURCE_DIR}/r3bdata/musicData
${R3BROOT_SOURCE_DIR}/r3bdata/pspData
${R3BROOT_SOURCE_DIR}/r3bdata/amsData
${R3BROOT_SOURCE_DIR}/r3bdata/califaData
${R3BROOT_SOURCE_DIR}/r3bdata/mwpcData
${R3BROOT_SOURCE_DIR}/r3bdata/twimData
${R3BROOT_SOURCE_DIR}/r3bdata/wrData
${R3BROOT_SOURCE_DIR}/r3bdata/trloiiData
${R3BROOT_SOURCE_DIR}/ssd/online
${R3BROOT_SOURCE_DIR}/califa/online
${R3BROOT_SOURCE_DIR}/trim
${R3BROOT_SOURCE_DIR}/twim/pars
${R3BROOT_SOURCE_DIR}/mwpc/mwpc0
${R3BROOT_SOURCE_DIR}/mwpc/mwpc1
${R3BROOT_SOURCE_DIR}/mwpc/mwpc2
${R3BROOT_SOURCE_DIR}/mwpc/mwpc3
${R3BROOT_SOURCE_DIR}/tofw
${R3BROOT_SOURCE_DIR}/tracking
${R3BSOF_SOURCE_DIR}/sofdata
${R3BSOF_SOURCE_DIR}/sofdata/atData
${R3BSOF_SOURCE_DIR}/sofdata/trimData
${R3BSOF_SOURCE_DIR}/sofdata/sciData
${R3BSOF_SOURCE_DIR}/sofdata/tofwData
${R3BSOF_SOURCE_DIR}/sofdata/trackingData
${R3BSOF_SOURCE_DIR}/sofdata/scalersData
${R3BSOF_SOURCE_DIR}/sofdata/corrData
${R3BSOF_SOURCE_DIR}/sofonline
${R3BSOF_SOURCE_DIR}/sofana
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES ${ROOT_LIBRARY_DIR} ${FAIRROOT_LIBRARY_DIR} ) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
R3BSofOnlineSpectra.cxx
R3BSofStatusOnlineSpectra.cxx
R3BSofMwpcvsMusicOnlineSpectra.cxx
R3BSofAtOnlineSpectra.cxx
R3BSofTrimOnlineSpectra.cxx
R3BSofSciOnlineSpectra.cxx
R3BSofSciVsMusicOnlineSpectra.cxx
R3BSofSciVsTrimOnlineSpectra.cxx
R3BSofSciVsMwpc0OnlineSpectra.cxx
R3BSofTofWOnlineSpectra.cxx
R3BSofFrsOnlineSpectra.cxx
R3BSofTwimvsMusicOnlineSpectra.cxx
R3BSofTwimvsTrimOnlineSpectra.cxx
R3BSofTrimVsTofwOnlineSpectra.cxx
R3BAmsCorrelationOnlineSpectra.cxx
R3BSofTrackingOnlineSpectra.cxx
R3BSofTrackingFissionOnlineSpectra.cxx
R3BSofScalersOnlineSpectra.cxx
R3BSofCorrOnlineSpectra.cxx
R3BSofSciVsPspxOnlineSpectra.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

set(LINKDEF SofOnlineLinkDef.h)
set(LIBRARY_NAME R3BSofOnline)
set(DEPENDENCIES
    Spectrum Base FairTools R3BBase R3BData R3BTracking R3BSsd R3BCalifa)

GENERATE_LIBRARY()
