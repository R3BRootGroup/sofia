# Create a library called "libR3BSofMwpc" which includes the source files given in
# the array. The extension is already found. Any number of sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES ${SYSTEM_INCLUDE_DIRECTORIES} ${BASE_INCLUDE_DIRECTORIES})

set(INCLUDE_DIRECTORIES
#put here all directories where header files are located
${R3BROOT_SOURCE_DIR}/r3bbase
${R3BROOT_SOURCE_DIR}/r3bdata
${R3BROOT_SOURCE_DIR}/tracking
${R3BSOF_SOURCE_DIR}/mwpc
${R3BSOF_SOURCE_DIR}/mwpc/mwpc0
${R3BSOF_SOURCE_DIR}/mwpc/mwpc1
${R3BSOF_SOURCE_DIR}/mwpc/mwpc2
${R3BSOF_SOURCE_DIR}/mwpc/mwpc3
${R3BSOF_SOURCE_DIR}/sofdata
${R3BSOF_SOURCE_DIR}/sofdata/mwpcData
${R3BSOF_SOURCE_DIR}/sofdata/tofwData
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES ${ROOT_LIBRARY_DIR} ${FAIRROOT_LIBRARY_DIR} )

link_directories( ${LINK_DIRECTORIES})

#set(SRCS
#Put here your sourcefiles
#R3BSofMwpcDigitizer.cxx
#mwpc0/R3BSofMwpc0.cxx
#mwpc0/R3BSofMwpc0ContFact.cxx
#mwpc0/R3BSofMwpc0CalPar.cxx
#mwpc0/R3BSofMwpc0Mapped2Cal.cxx
#mwpc0/R3BSofMwpc0Mapped2CalPar.cxx
#mwpc0/R3BSofMwpc0Cal2Hit.cxx
#mwpc1/R3BSofMwpc1.cxx
#mwpc1/R3BSofMwpc1Cal2Hit.cxx
#mwpc1/R3BSofMwpc1Mapped2CalPar.cxx
#mwpc1/R3BSofMwpc1Mapped2Cal.cxx
#mwpc1/R3BSofMwpc1ContFact.cxx
#mwpc1/R3BSofMwpc1CalPar.cxx
#mwpc2/R3BSofMwpc2.cxx
#mwpc2/R3BSofMwpc2Cal2Hit.cxx
#mwpc2/R3BSofMwpc2Mapped2CalPar.cxx
#mwpc2/R3BSofMwpc2Mapped2Cal.cxx
#mwpc2/R3BSofMwpc2ContFact.cxx
#mwpc2/R3BSofMwpc2CalPar.cxx
#mwpc3/R3BSofMwpc3.cxx
#mwpc3/R3BSofMwpc3Cal2Hit.cxx
#mwpc3/R3BSofMwpc3Mapped2CalPar.cxx
#mwpc3/R3BSofMwpc3Mapped2Cal.cxx
#mwpc3/R3BSofMwpc3ContFact.cxx
#mwpc3/R3BSofMwpc3CalPar.cxx
#)

file(GLOB SRCS *.cxx mwpc0/*.cxx mwpc1/*.cxx mwpc2/*.cxx mwpc3/*.cxx)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

set(LINKDEF SofMwpcLinkDef.h)
set(LIBRARY_NAME R3BSofMwpc)
set(DEPENDENCIES
  R3Bbase R3BData R3BSofData R3BTracking)

GENERATE_LIBRARY()
