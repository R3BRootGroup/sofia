# Create a library called "libR3BSofData" which includes the source files given in
# the array. The extension is already found. Any number of sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES ${SYSTEM_INCLUDE_DIRECTORIES} ${BASE_INCLUDE_DIRECTORIES} )

set(INCLUDE_DIRECTORIES
${R3BROOT_SOURCE_DIR}/r3bdata
${R3BROOT_SOURCE_DIR}/r3bdata/califaData
${R3BROOT_SOURCE_DIR}/r3bdata/neulandData
${R3BSOF_SOURCE_DIR}/sofdata
${R3BSOF_SOURCE_DIR}/sofdata/atData
${R3BSOF_SOURCE_DIR}/sofdata/sciData
${R3BSOF_SOURCE_DIR}/sofdata/trimData
${R3BSOF_SOURCE_DIR}/sofdata/tofwData
${R3BSOF_SOURCE_DIR}/sofdata/frsData
${R3BSOF_SOURCE_DIR}/sofdata/scalersData
${R3BSOF_SOURCE_DIR}/sofdata/corrData
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES ${ROOT_LIBRARY_DIR} ${FAIRROOT_LIBRARY_DIR} )

link_directories( ${LINK_DIRECTORIES})

set(SRCS
trimData/R3BSofTrimPoint.cxx
trimData/R3BSofTrimMappedData.cxx
trimData/R3BSofTrimCalData.cxx
trimData/R3BSofTrimHitData.cxx
atData/R3BSofATPoint.cxx
atData/R3BSofAtMappedData.cxx
tofwData/R3BSofTofWPoint.cxx
sciData/R3BSofSciPoint.cxx
sciData/R3BSofSciMappedData.cxx
sciData/R3BSofSciTcalData.cxx
sciData/R3BSofSciSingleTcalData.cxx
sciData/R3BSofSciCalData.cxx
sciData/R3BSofSciHitData.cxx
tofwData/R3BSofTofWMappedData.cxx
tofwData/R3BSofTofWTcalData.cxx
tofwData/R3BSofTofWSingleTcalData.cxx
tofwData/R3BSofTofWCalData.cxx
tofwData/R3BSofTofWHitData.cxx
trackingData/R3BSofTrackingData.cxx
scalersData/R3BSofScalersMappedData.cxx
corrData/R3BSofCorrmMappedData.cxx
corrData/R3BSofCorrvMappedData.cxx
)


# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
SET(HEADERS ${HEADERS} ${R3BROOT_SOURCE_DIR}/r3bdata/R3BDetectorList.h )

set(LINKDEF  SofDataLinkDef.h)
set(LIBRARY_NAME R3BSofData)
set(DEPENDENCIES
    Base FairTools R3BBase R3BData Core m)

GENERATE_LIBRARY()
