/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2019 Members of R3B Collaboration                          *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#ifndef R3BROOT_R3BSOFIAPROVIDETSTART_H
#define R3BROOT_R3BSOFIAPROVIDETSTART_H

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

#endif   // R3BROOT_R3BSOFIAPROVIDETSTART_H
