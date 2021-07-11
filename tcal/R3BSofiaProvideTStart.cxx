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

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRuntimeDb.h"

#include "R3BEventHeader.h"
#include "R3BSofSciRawTofPar.h"
#include "R3BSofiaProvideTStart.h"

R3BSofiaProvideTStart::R3BSofiaProvideTStart()
    : FairTask("R3BSofiaProvideTStart", 0)
    , fSofSciCalData("SofSciTcalData")
    , fRawTofPar(NULL)
    , fStartId(1)
    , fEventHeader(nullptr)
{
}

void R3BSofiaProvideTStart::SetParContainers()
{
    fRawTofPar = (R3BSofSciRawTofPar*)FairRuntimeDb::instance()->getContainer("SofSciRawTofPar");
    if (!fRawTofPar)
    {
        LOG(ERROR) << "R3BSofiaProvideTStart::SetParContainers() : SofSciRawTofPar-Container not found.";
        return;
    }
}

void R3BSofiaProvideTStart::SetParameter()
{
    if (!fRawTofPar)
        LOG(FATAL) << "R3BSofiaProvideTStart::SetParameter() : fRawTofPar not found.";
    fStartId = fRawTofPar->GetDetIdCaveC();
    return;
}

InitStatus R3BSofiaProvideTStart::Init()
{
    fSofSciCalData.Init();

    auto ioman = FairRootManager::Instance();
    if (ioman == nullptr)
    {
        throw std::runtime_error("R3BSofiaProvideTStart: No FairRootManager");
    }

    fEventHeader = (R3BEventHeader*)ioman->GetObject("R3BEventHeader");
    if (fEventHeader == nullptr)
    {
        throw std::runtime_error("R3BSofiaProvideTStart: No R3BEventHeader");
    }

    SetParameter();

    return kSUCCESS;
}

InitStatus R3BSofiaProvideTStart::ReInit()
{
    SetParContainers();
    SetParameter();
    return kSUCCESS;
}

void R3BSofiaProvideTStart::Exec(Option_t*) { fEventHeader->SetTStart(GetTStart()); }

Double_t R3BSofiaProvideTStart::GetTStart() const
{
    const auto sofsci = fSofSciCalData.Retrieve();

    if (sofsci.empty())
    {
        return std::numeric_limits<Double_t>::quiet_NaN();
    }

    Double_t ts = 0.;
    Int_t ns = 0;
    for (const auto& sof : sofsci)
    {
        if (sof->GetDetector() == fStartId)
        {
            if (sof->GetPmt() == 1 || sof->GetPmt() == 2)
            {
                ts = ts + sof->GetRawTimeNs();
                ns++;
            }
        }
    }

    if (ns == 2)
    {
        return ts / 2.;
    }
    else
    {
        return std::numeric_limits<Double_t>::quiet_NaN();
    }
}

bool R3BSofiaProvideTStart::IsBeam() const { return !std::isnan(GetTStart()); }

ClassImp(R3BSofiaProvideTStart)
