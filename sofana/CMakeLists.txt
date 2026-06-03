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

set(SRCS
    #Put here your sourcefiles
    R3BSofFrsAnalysis.cxx
    R3BSofFragmentAnalysis.cxx
    R3BSofFissionAnalysis.cxx
    R3BSofFrsAnaPar.cxx
    R3BSofFragmentAnaPar.cxx
    R3BSofGladFieldPar.cxx
    R3BSofAnaContFact.cxx)

set(HEADERS
    #Put here your sourcefiles
    R3BSofFrsAnalysis.h
    R3BSofFragmentAnalysis.h
    R3BSofFissionAnalysis.h
    R3BSofFrsAnaPar.h
    R3BSofFragmentAnaPar.h
    R3BSofGladFieldPar.h
    R3BSofAnaContFact.h)

add_library_with_dictionary(
    LIBNAME
    R3BSofAna
    LINKDEF
    SofAnaLinkDef.h
    HEADERS
    ${HEADERS}
    SRCS
    ${SRCS}
    INCLUDEDIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDENCIES
    R3BSofData
    R3BMusic
    R3BTwim
    R3BTracking
    R3BNeulandShared)
