# Create a library called "libR3BSofsource" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(SYSTEM_INCLUDE_DIRECTORIES ${SYSTEM_INCLUDE_DIRECTORIES} ${BASE_INCLUDE_DIRECTORIES} ${ucesb_INCLUDE_DIR} )

set(INCLUDE_DIRECTORIES
#put here all directories where header files are located
${R3BROOT_SOURCE_DIR}/r3bsource
${R3BROOT_SOURCE_DIR}/r3bbase
${R3BROOT_SOURCE_DIR}/r3bdata/wrData
${R3BSOF_SOURCE_DIR}/sofsource
${R3BSOF_SOURCE_DIR}/sofdata
${R3BSOF_SOURCE_DIR}/sofdata/atData
${R3BSOF_SOURCE_DIR}/sofdata/sciData
${R3BSOF_SOURCE_DIR}/sofdata/trimData
${R3BSOF_SOURCE_DIR}/sofdata/twimData
${R3BSOF_SOURCE_DIR}/sofdata/mwpcData
${R3BSOF_SOURCE_DIR}/sofdata/tofwData
${R3BSOF_SOURCE_DIR}/sofdata/scalersData
${R3BSOF_SOURCE_DIR}/sofdata/corrData
)

set(LINK_DIRECTORIES ${ROOT_LIBRARY_DIR} ${FAIRROOT_LIBRARY_DIR} ${ucesb_LIBRARY_DIR} )

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})
link_directories( ${LINK_DIRECTORIES})

set(SRCS
R3BSofWhiterabbitReader.cxx
R3BSofSciReader.cxx
R3BSofTofWReader.cxx
R3BSofTwimReader.cxx
R3BSofTrimReader.cxx
R3BSofMwpcReader.cxx
R3BSofScalersReader.cxx
R3BSofAtReader.cxx
R3BSofCorrmReader.cxx
R3BSofCorrvReader.cxx
R3BVftxTrigReader.cxx
)

Set(STRUCT_HEADERS
ext_h101_wrsofia.h
ext_h101_sofsci.h
ext_h101_softofw.h
ext_h101_softwim.h
ext_h101_softrim.h
ext_h101_sofmwpc.h
ext_h101_sofscalers.h
ext_h101_sofat.h
ext_h101_sofcorrm.h
ext_h101_sofcorrv.h
ext_h101_vftx_tr.h
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
Set(HEADERS ${STRUCT_HEADERS} ${HEADERS})

set(LINKDEF_FILE R3BSofLinkDef.h)
set(DEPENDENCIES
    R3Bbase Base R3BSofData R3Bsource ${ucesb_LIBRARY_SHARED} FairTools)
set(LIBRARY_NAME R3BSofsource)

If(ROOT_FOUND_VERSION LESS 59999)
  Set(DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/G__${LIBRARY_NAME}Dict.cxx)
  ROOT_GENERATE_DICTIONARY_OLD_EXTRA("${HEADERS}" "${LINKDEF_FILE}" "${DICTIONARY}" "${INCLUDE_DIRECTORIES}${SYSTEM_INCLUDE_DIRECTORIES}")
  Set(SRCS ${SRCS} ${DICTIONARY})
  CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
  Set(HEADERS ${STRUCT_HEADERS} ${HEADERS})
  Set(LINKDEF ${LINKDEF_FILE})
  ROOT_GENERATE_ROOTMAP()
  Set(LINKDEF )
Else(ROOT_FOUND_VERSION LESS 59999)
  Set(LINKDEF ${LINKDEF_FILE})
EndIf(ROOT_FOUND_VERSION LESS 59999)

GENERATE_LIBRARY()
