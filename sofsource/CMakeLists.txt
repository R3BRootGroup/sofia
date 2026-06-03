##############################################################################
#   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    #
#   Copyright (C) 2019 University of Santiago de Compostela                  #
#   Copyright (C) 2019 CEA-DAM-DIF, Université Paris-Saclay                  #
#   Copyright (C) 2023 University of Coruña                                  #
#   Copyright (C) 2019-2026 Members of R3B Collaboration                     #
#                                                                            #
#             This software is distributed under the terms of the            #
#                 GNU General Public Licence (GPL) version 3,                #
#                    copied verbatim in the file "LICENSE".                  #
#                                                                            #
# In applying this license GSI does not waive the privileges and immunities  #
# granted to it by virtue of its status as an Intergovernmental Organization #
# or submit itself to any jurisdiction.                                      #
##############################################################################

file(GLOB SRCS *.cxx)

set(STRUCT_HEADERS
    ext/ext_h101_wrsofia.h
    ext/ext_h101_sofsci.h
    ext/ext_h101_softofw.h
    ext/ext_h101_softrim.h
    ext/ext_h101_sofscalers.h
    ext/ext_h101_sofat.h
    ext/ext_h101_sofcorrm.h
    ext/ext_h101_sofcorrv.h)

# fill list of header files from list of source files
# by exchanging the file extension
change_file_extension(*.cxx *.h HEADERS "${SRCS}")

add_library_with_dictionary(
    LIBNAME
    R3BSofsource
    LINKDEF
    SofSourceLinkDef.h
    HEADERS
    ${HEADERS}
    ${STRUCT_HEADERS}
    SRCS
    ${SRCS}
    INCLUDEDIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ext
    DEPENDENCIES
    R3BSource
    R3BSofData)
