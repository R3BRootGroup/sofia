// clang-format off
/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2019 University of Santiago de Compostela                  *
 *   Copyright (C) 2019 CEA-DAM-DIF, Université Paris-Saclay                  *
 *   Copyright (C) 2023 University of Coruña                                  *
 *   Copyright (C) 2019-2026 Members of R3B Collaboration                     *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// Point classes for simulation
#pragma link C++ class R3BSofSciPoint+;
#pragma link C++ class R3BSofTrimPoint+;
#pragma link C++ class R3BSofATPoint+;
#pragma link C++ class R3BSofTofWPoint+;

// General
#pragma link C++ class R3BSofAtMappedData+;

#pragma link C++ class R3BSofSciMappedData+;
#pragma link C++ class R3BSofSciTcalData+;
#pragma link C++ class R3BSofSciSingleTcalData+;
#pragma link C++ class R3BSofSciCalData+;
#pragma link C++ class R3BSofSciHitData+;

#pragma link C++ class R3BSofTofWMappedData+;
#pragma link C++ class R3BSofTofWTcalData+;
#pragma link C++ class R3BSofTofWSingleTcalData+;
#pragma link C++ class R3BSofTofWCalData+;
#pragma link C++ class R3BSofTofWHitData+;

#pragma link C++ class R3BSofTrimMappedData+;
#pragma link C++ class R3BSofTrimCalData+;
#pragma link C++ class R3BSofTrimHitData+;

#pragma link C++ class R3BSofTrackingData+;

#pragma link C++ class R3BSofScalersMappedData+;

#pragma link C++ class R3BSofCorrmMappedData+;
#pragma link C++ class R3BSofCorrvMappedData+;

#endif
