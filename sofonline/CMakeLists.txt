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
    R3BSofOnlineSpectra.cxx
    R3BSofStatusOnlineSpectra.cxx
    R3BSofMwpcvsMusicOnlineSpectra.cxx
    R3BSofAtOnlineSpectra.cxx
    R3BSofTrimOnlineSpectra.cxx
    R3BSofSciOnlineSpectra.cxx
    R3BSofSciVsMusicOnlineSpectra.cxx
    R3BSofSciVsTrimOnlineSpectra.cxx
    R3BSofSciVsMwpc0OnlineSpectra.cxx
    R3BSofTofWOnlineSpectra.cxx
    R3BSofFrsOnlineSpectra.cxx
    R3BSofTwimvsMusicOnlineSpectra.cxx
    R3BSofTwimvsTrimOnlineSpectra.cxx
    R3BSofTrimVsTofwOnlineSpectra.cxx
    R3BAmsCorrelationOnlineSpectra.cxx
    R3BSofTrackingOnlineSpectra.cxx
    R3BSofTrackingFissionOnlineSpectra.cxx
    R3BSofScalersOnlineSpectra.cxx
    R3BSofCorrOnlineSpectra.cxx
    R3BSofSciVsPspxOnlineSpectra.cxx)

set(HEADERS
    R3BSofOnlineSpectra.h
    R3BSofStatusOnlineSpectra.h
    R3BSofMwpcvsMusicOnlineSpectra.h
    R3BSofAtOnlineSpectra.h
    R3BSofTrimOnlineSpectra.h
    R3BSofSciOnlineSpectra.h
    R3BSofSciVsMusicOnlineSpectra.h
    R3BSofSciVsTrimOnlineSpectra.h
    R3BSofSciVsMwpc0OnlineSpectra.h
    R3BSofTofWOnlineSpectra.h
    R3BSofFrsOnlineSpectra.h
    R3BSofTwimvsMusicOnlineSpectra.h
    R3BSofTwimvsTrimOnlineSpectra.h
    R3BSofTrimVsTofwOnlineSpectra.h
    R3BAmsCorrelationOnlineSpectra.h
    R3BSofTrackingOnlineSpectra.h
    R3BSofTrackingFissionOnlineSpectra.h
    R3BSofScalersOnlineSpectra.h
    R3BSofCorrOnlineSpectra.h
    R3BSofSciVsPspxOnlineSpectra.h)

add_library_with_dictionary(
    LIBNAME
    R3BSofOnline
    LINKDEF
    SofOnlineLinkDef.h
    HEADERS
    ${HEADERS}
    SRCS
    ${SRCS}
    INCLUDEDIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDENCIES
    R3BCalifa
    R3BTwim
    R3BTracking
    R3BMwpc
    R3BSofData
    R3BMusic
    R3BSsd)
