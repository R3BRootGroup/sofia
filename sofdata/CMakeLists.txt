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
    corrData/R3BSofCorrvMappedData.cxx)

set(HEADERS
    trimData/R3BSofTrimPoint.h
    trimData/R3BSofTrimMappedData.h
    trimData/R3BSofTrimCalData.h
    trimData/R3BSofTrimHitData.h
    atData/R3BSofATPoint.h
    atData/R3BSofAtMappedData.h
    tofwData/R3BSofTofWPoint.h
    sciData/R3BSofSciPoint.h
    sciData/R3BSofSciMappedData.h
    sciData/R3BSofSciTcalData.h
    sciData/R3BSofSciSingleTcalData.h
    sciData/R3BSofSciCalData.h
    sciData/R3BSofSciHitData.h
    tofwData/R3BSofTofWMappedData.h
    tofwData/R3BSofTofWTcalData.h
    tofwData/R3BSofTofWSingleTcalData.h
    tofwData/R3BSofTofWCalData.h
    tofwData/R3BSofTofWHitData.h
    trackingData/R3BSofTrackingData.h
    scalersData/R3BSofScalersMappedData.h
    corrData/R3BSofCorrmMappedData.h
    corrData/R3BSofCorrvMappedData.h)

add_library_with_dictionary(
    LIBNAME
    R3BSofData
    LINKDEF
    SofDataLinkDef.h
    HEADERS
    ${HEADERS}
    SRCS
    ${SRCS}
    INCLUDEDIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    trimData
    atData
    tofwData
    sciData
    tofwData
    trackingData
    scalersData
    corrData
    DEPENDENCIES
    R3BData)
