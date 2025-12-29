/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
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

// ----------------------------------------------------------------
// -----          R3BSofTofWDigitizer source file             -----
// -----        Created 03/11/19 by J.L. Rodriguez            -----
// ----------------------------------------------------------------

#include "R3BSofTofWDigitizer.h"
#include "R3BLogger.h"
#include "R3BMCTrack.h"
#include "R3BSofTofWPoint.h"
#include "R3BTGeoPar.h"

#include <FairLogger.h>
#include <FairRootManager.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>

#include <TClonesArray.h>
#include <TMath.h>
#include <TVector3.h>

// R3BSofTofWDigitizer: Default Constructor --------------------------
R3BSofTofWDigitizer::R3BSofTofWDigitizer()
    : R3BSofTofWDigitizer("R3BSofTofWDigitizer", 1)
{
}

// R3BSofTofWDigitizer: Standard Constructor --------------------------
R3BSofTofWDigitizer::R3BSofTofWDigitizer(const TString& name, int iVerbose)
    : FairTask(name.Data(), iVerbose)
{
    rand = new TRandom3();
}

// Virtual R3BSofTofWDigitizer: Destructor ----------------------------
R3BSofTofWDigitizer::~R3BSofTofWDigitizer()
{
    R3BLOG(debug, "");
    if (fTofHits)
    {
        delete fTofHits;
    }
}

void R3BSofTofWDigitizer::SetParContainers()
{
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    fTofWGeoPar = (R3BTGeoPar*)rtdb->getContainer("TofwGeoPar");
    if (!fTofWGeoPar)
    {
        R3BLOG(error, "Could not get access to TofwGeoPar container.");
        return;
    }
    else
    {
        R3BLOG(info, "Container TofwGeoPar found.");
    }
}

void R3BSofTofWDigitizer::SetParameter()
{
    fsigma_y = fTofWGeoPar->GetSigmaY();

    fRot.RotateX(-fTofWGeoPar->GetRotX() * TMath::DegToRad());
    fRot.RotateY(-fTofWGeoPar->GetRotY() * TMath::DegToRad());
    fRot.RotateZ(-fTofWGeoPar->GetRotZ() * TMath::DegToRad());

    fTrans.SetXYZ(fTofWGeoPar->GetPosX(), fTofWGeoPar->GetPosY(), fTofWGeoPar->GetPosZ());
}

// ----   Public method Init  -----------------------------------------
InitStatus R3BSofTofWDigitizer::Init()
{
    R3BLOG(info, "");

    // Get input array
    FairRootManager* ioman = FairRootManager::Instance();
    R3BLOG_IF(fatal, !ioman, "FairRootManager not found.");

    fMCTrack = (TClonesArray*)ioman->GetObject("MCTrack");
    fTofPoints = (TClonesArray*)ioman->GetObject("SofTofWPoint");

    // Register output array fTofHits
    fTofHits = new TClonesArray("R3BSofTofWHitData");
    ioman->Register("TofWHitData", "Digital response in TofW", fTofHits, kTRUE);

    SetParameter();
    return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofTofWDigitizer::Exec(Option_t*)
{
    Reset();
    // Reading the Input -- Point Data --
    auto nHits = fTofPoints->GetEntries();
    if (nHits == 0)
    {
        return;
    }
    // Data from Point level
    int paddle = 0;
    int TrackId = 0, PID = 0, mother = -1;
    double x = 0., y = 0., z = 0., time = 0.;
    TVector3 vpos;
    for (Int_t i = 0; i < nHits; i++)
    {
        auto* pointData = dynamic_cast<R3BSofTofWPoint*>(fTofPoints->At(i));
        TrackId = pointData->GetTrackID();

        auto* Track = dynamic_cast<R3BMCTrack*>(fMCTrack->At(TrackId));
        PID = Track->GetPdgCode();

        if (PID > 1000080160) // Z=8 and A=16
        {
            Double_t fX_in = pointData->GetXIn();
            Double_t fY_in = pointData->GetYIn();
            Double_t fZ_in = pointData->GetZIn();
            Double_t fX_out = pointData->GetXOut();
            Double_t fY_out = pointData->GetYOut();
            Double_t fZ_out = pointData->GetZOut();
            paddle = pointData->GetDetCopyID() + 1;
            // std::cout<<paddle<<std::endl;

            x = ((fX_in + fX_out) / 2.);
            y = ((fY_in + fY_out) / 2.);
            z = ((fZ_in + fZ_out) / 2.);
            vpos.SetXYZ(x, y, z);

            vpos = fRot * (vpos - fTrans);
            time = pointData->GetTime() + rand->Gaus(0., fsigma_t);

            // Add hit data
            AddHitData(paddle, vpos.X() * 10., vpos.Y() * 10. + rand->Gaus(0., fsigma_y), time);
        }
    }
    LOG(info) << "R3BSofTofWDigitizer: " << fTofHits->GetEntriesFast() << " points registered in this event";
    return;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofTofWDigitizer::ReInit()
{
    SetParContainers();
    SetParameter();
    return kSUCCESS;
}

// -----   Public method Reset   -----------------------------------------------
void R3BSofTofWDigitizer::Reset()
{
    R3BLOG(debug, "");
    if (fTofHits)
    {
        fTofHits->Clear();
    }
}

// -----   Private method AddHitData  -------------------------------------------
R3BSofTofWHitData* R3BSofTofWDigitizer::AddHitData(int paddle, double x, double y, double time)
{
    // It fills the R3BSofTofWHitData
    TClonesArray& clref = *fTofHits;
    auto size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofTofWHitData(paddle, x, y, time);
}

ClassImp(R3BSofTofWDigitizer)
