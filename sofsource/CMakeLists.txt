# Create a library called "libR3BSOF" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(SYSTEM_INCLUDE_DIRECTORIES
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
${ucesb_INCLUDE_DIR}
)

set(INCLUDE_DIRECTORIES
#put here all directories where header files are located
#${R3BROOT_SOURCE_DIR}/r3bsource
#${R3BROOT_SOURCE_DIR}/r3bbase
#${R3BROOT_SOURCE_DIR}/r3bdata/caloData
#${R3BROOT_SOURCE_DIR}/r3bdata/neulandData
${R3BSOF_SOURCE_DIR}/sofsource
${R3BSOF_SOURCE_DIR}/sofdata
)

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
${ucesb_LIBRARY_DIR}
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})
link_directories( ${LINK_DIRECTORIES})

set(SRCS
#R3BUcesbSource.cxx
R3BReader.cxx
#R3BUnpackReader.cxx
#R3BNeulandTamexReader.cxx
#R3BNeulandTacquilaReader.cxx
#R3BSofATReader.cxx
R3BSofSciReader.cxx
R3BSofATMadcReader.cxx
R3BSofComRefReader.cxx
)

Set(STRUCT_HEADERS
#ext_h101_unpack.h
#ext_h101_raw_nnp.h
#ext_h101_raw_nnp_tamex.h
ext_h101_sofsci.h
ext_h101_sofat_madc.h
ext_h101_softofw.h
ext_h101_sofcomref.h
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
Set(HEADERS ${STRUCT_HEADERS} ${HEADERS})

set(LINKDEF_FILE R3BLinkDef.h)
set(DEPENDENCIES
#    GeoBase ParBase MbsAPI Base FairTools
#    R3Bbase R3BData
    R3BSofData Core Geom GenVector
    Physics Matrix MathCore
    ${ucesb_LIBRARY_SHARED})
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
