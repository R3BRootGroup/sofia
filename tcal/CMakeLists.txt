##############################################################################
#   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    #
#   Copyright (C) 2019 University of Santiago de Compostela                  #
#   Copyright (C) 2019 CEA-DAM-DIF, Université Paris-Saclay                  #
#   Copyright (C) 2023 University of Coruña                                  #
#   Copyright (C) 2019-2025 Members of R3B Collaboration                     #
#                                                                            #
#             This software is distributed under the terms of the            #
#                 GNU General Public Licence (GPL) version 3,                #
#                    copied verbatim in the file "LICENSE".                  #
#                                                                            #
# In applying this license GSI does not waive the privileges and immunities  #
# granted to it by virtue of its status as an Intergovernmental Organization #
# or submit itself to any jurisdiction.                                      #
##############################################################################

set(SRCS R3BSofSciRawTofPar.cxx R3BSofTcalContFact.cxx R3BSofTcalPar.cxx R3BSofiaProvideTStart.cxx)

set(HEADERS R3BSofTcalContFact.h R3BSofSciRawTofPar.h R3BSofTcalPar.h R3BSofiaProvideTStart.h)

add_library_with_dictionary(
    LIBNAME
    R3BSofTcal
    LINKDEF
    SofTcalLinkDef.h
    HEADERS
    ${HEADERS}
    SRCS
    ${SRCS}
    INCLUDEDIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDENCIES
    R3BSofData
    R3BNeulandShared)
