/******************************************************************************
 *   Copyright (C) 2017 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2017-2025 Members of R3B Collaboration                     *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU Lesser General Public Licence (LGPL) version 3,        *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#pragma once

#include "FairTask.h"
#include "R3BSofSciTcalData.h"
#include "TCAConnector.h"

class R3BEventHeader;
class R3BSofSciRawTofPar;

class R3BSofiaProvideTStart : public FairTask
{
  public:
    R3BSofiaProvideTStart();

    InitStatus Init() override;
    InitStatus ReInit() override;
    void Exec(Option_t*) override;
    void SetParContainers() override;

  private:
    void SetParameter();
    TCAInputConnector<R3BSofSciTcalData> fSofSciCalData;
    R3BEventHeader* fEventHeader;
    R3BSofSciRawTofPar* fRawTofPar;
    Int_t fStartId;

    bool IsBeam() const;
    Double_t GetTStart() const;

    ClassDefOverride(R3BSofiaProvideTStart, 0)
};
