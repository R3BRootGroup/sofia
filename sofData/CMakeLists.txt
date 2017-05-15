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
${R3BROOT_SOURCE_DIR}/r3bdata/caloData
${R3BROOT_SOURCE_DIR}/r3bdata/neulandData
${R3BSOF_SOURCE_DIR}/sofdata
${R3BSOF_SOURCE_DIR}/sofdata/atData
${R3BSOF_SOURCE_DIR}/sofdata/mwpcData
${R3BSOF_SOURCE_DIR}/sofdata/musicData
${R3BSOF_SOURCE_DIR}/sofdata/tofData
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
)

link_directories( ${LINK_DIRECTORIES})

set(SRCS
#R3BMCStack.cxx
#R3BMCTrack.cxx
#caloData/R3BCaloCrystalHit.cxx
#caloData/R3BCaloCrystalHitSim.cxx
#caloData/R3BCaloRawHit.cxx
#caloData/R3BCaloHit.cxx
#caloData/R3BCaloHitSim.cxx
#caloData/R3BCaloPoint.cxx
#caloData/R3BCaloRawHit.cxx
#neulandData/R3BNeulandMappedData.cxx
#neulandData/R3BNeulandCalData.cxx
#neulandData/R3BNeulandPoint.cxx
#neulandData/R3BNeulandDigi.cxx
#neulandData/R3BNeulandCluster.cxx
#neulandData/R3BNeulandNeutron.cxx
#neulandData/R3BNeulandPixel.cxx
#neulandData/R3BPaddleTamexMappedData.cxx
R3BSofATMadcMappedData.cxx
R3BSofSciMappedData.cxx
R3BSofComRefMappedData.cxx
R3BSofSciCalData.cxx
R3BSofToFWMappedData.cxx
R3BSofMusicMdpp16MappedData.cxx
)


# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
SET(HEADERS ${HEADERS} R3BSofDetectorList.h )

set(LINKDEF  R3BSofDataLinkDef.h)
set(LIBRARY_NAME R3BSofData)
set(DEPENDENCIES
    Base FairTools)

GENERATE_LIBRARY()
