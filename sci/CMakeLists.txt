##############################################################################
#   Copyright (C) 2018 GSI Helmholtzzentrum für Schwerionenforschung GmbH    #
#   Copyright (C) 2018 University of Santiago de Compostela                  #
#   Copyright (C) 2018-2026 CEA-DAM-DIF, Université Paris-Saclay             #
#   Copyright (C) 2023-2026 University of Coruña                             #
#   Copyright (C) 2018-2026 Members of R3B Collaboration                     #
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
    ./sim/R3BSofSci.cxx
    ./pars/R3BSofSciContFact.cxx
    ./pars/R3BSofSciRawPosPar.cxx
    ./pars/R3BSofSciMapped2TcalPar.cxx
    ./pars/R3BSofSciTcal2RawPosPar.cxx
    ./pars/R3BSofSciTcal2RawTofPar.cxx
    ./pars/R3BSofSciCalPosPar.cxx
    ./pars/R3BSofSciCalTofPar.cxx
    ./pars/R3BSofSciSingleTcal2CalPosCaveCPar.cxx
    ./calibration/R3BSofSciMapped2Tcal.cxx
    ./calibration/R3BSofSciTcal2SingleTcal.cxx
    ./calibration/R3BSofSciSingleTcal2Cal.cxx
    ./calibration/R3BSofSciSingleTcal2Hit.cxx)

set(HEADERS
    ./sim/R3BSofSci.h
    ./pars/R3BSofSciContFact.h
    ./pars/R3BSofSciRawPosPar.h
    ./pars/R3BSofSciMapped2TcalPar.h
    ./pars/R3BSofSciTcal2RawPosPar.h
    ./pars/R3BSofSciTcal2RawTofPar.h
    ./pars/R3BSofSciCalPosPar.h
    ./pars/R3BSofSciCalTofPar.h
    ./pars/R3BSofSciSingleTcal2CalPosCaveCPar.h
    ./calibration/R3BSofSciMapped2Tcal.h
    ./calibration/R3BSofSciTcal2SingleTcal.h
    ./calibration/R3BSofSciSingleTcal2Cal.h
    ./calibration/R3BSofSciSingleTcal2Hit.h)

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
    sim
    pars
    calibration
    DEPENDENCIES
    R3BSofTcal
    R3BSofData
    R3BTracking)
