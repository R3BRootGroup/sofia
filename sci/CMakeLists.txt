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

set(SRCS
    R3BSofSci.cxx
    R3BSofSciContFact.cxx
    R3BSofSciRawPosPar.cxx
    R3BSofSciMapped2TcalPar.cxx
    R3BSofSciTcal2RawPosPar.cxx
    R3BSofSciTcal2RawTofPar.cxx
    R3BSofSciCalPosPar.cxx
    R3BSofSciCalTofPar.cxx
    R3BSofSciSingleTcal2CalPosCaveCPar.cxx
    R3BSofSciMapped2Tcal.cxx
    R3BSofSciTcal2SingleTcal.cxx
    R3BSofSciSingleTcal2Cal.cxx
    R3BSofSciSingleTcal2Hit.cxx)

set(HEADERS
    R3BSofSci.h
    R3BSofSciContFact.h
    R3BSofSciRawPosPar.h
    R3BSofSciMapped2TcalPar.h
    R3BSofSciTcal2RawPosPar.h
    R3BSofSciTcal2RawTofPar.h
    R3BSofSciCalPosPar.h
    R3BSofSciCalTofPar.h
    R3BSofSciSingleTcal2CalPosCaveCPar.h
    R3BSofSciMapped2Tcal.h
    R3BSofSciTcal2SingleTcal.h
    R3BSofSciSingleTcal2Cal.h
    R3BSofSciSingleTcal2Hit.h)

add_library_with_dictionary(
    LIBNAME
    R3BSofSci
    LINKDEF
    SofSciLinkDef.h
    HEADERS
    ${HEADERS}
    SRCS
    ${SRCS}
    INCLUDEDIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDENCIES
    R3BSofTcal
    R3BSofData
    R3BTracking)
