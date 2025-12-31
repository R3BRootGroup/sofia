/******************************************************************************
 *   Copyright (C) 2017 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2017-2026 Members of R3B Collaboration                     *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

// -------------------------------------------------------------------------
// -----                        R3BSofTrim source file                 -----
// -----                 Created 24/11/17 by H.Alvarez-Pol             -----
// -------------------------------------------------------------------------

#include "R3BSofTrim.h"
#include "R3BLogger.h"
#include "R3BMCStack.h"
#include "R3BSofTrimPoint.h"

#include <FairLogger.h>
#include <FairRootManager.h>
#include <FairVolume.h>

#include <TClonesArray.h>
#include <TGeoManager.h>
#include <TParticle.h>
#include <TVirtualMC.h>

#define U_MEV 931.4940954

R3BSofTrim::R3BSofTrim()
    : R3BSofTrim("")
{
}

R3BSofTrim::R3BSofTrim(const TString& geoFile, const TGeoTranslation& trans, const TGeoRotation& rot)
    : R3BSofTrim(geoFile, { trans, rot })
{
}

R3BSofTrim::R3BSofTrim(const TString& geoFile, const TGeoCombiTrans& combi)
    : R3BDetector("R3BSofTrim", kSOFTRIM, geoFile, combi)
    , fSofTRIMCollection(new TClonesArray("R3BSofTrimPoint"))
{
    ResetParameters();
}

R3BSofTrim::~R3BSofTrim()
{
    R3BLOG(debug1, "");
    if (fSofTRIMCollection)
    {
        fSofTRIMCollection->Delete();
        delete fSofTRIMCollection;
    }
}

void R3BSofTrim::Initialize()
{
    FairDetector::Initialize();

    R3BLOG(info, " ");
    R3BLOG(debug, "Sens. Vol. (McId) " << TVirtualMC::GetMC()->VolId("TrimAnode"));
}

// -----   Public method ProcessHits  --------------------------------------
bool R3BSofTrim::ProcessHits(FairVolume* vol)
{
    if (gMC->IsTrackEntering())
    {
        gGeoManager->cd(gMC->CurrentVolPath());
        int nodeId = gGeoManager->GetNodeId();
        fELoss = 0.;
        fNSteps = 0; // FIXME
        fTime = gMC->TrackTime() * 1.0e09;
        fLength = gMC->TrackLength();
        gMC->TrackPosition(fPosIn);
        gMC->TrackMomentum(fMomIn);
    }

    // Sum energy loss for all steps in the active volume
    fELoss += gMC->Edep();

    if (fELoss == 0.)
    {
        return kFALSE;
    }

    if (fELoss > 0)
    {
        fNSteps++;

        // Set additional parameters at exit of active volume. Create R3BSofTrimPoint.
        if (gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared())
        {

            fTrackID = gMC->GetStack()->GetCurrentTrackNumber();
            fVolumeID = vol->getMCid();
            fDetCopyID = vol->getCopyNo();
            fUniqueID = gMC->GetStack()->GetCurrentTrack()->GetUniqueID();

            gMC->TrackPosition(fPosOut);
            gMC->TrackMomentum(fMomOut);

            Double_t M_in = gMC->TrackMass() * 1000.;
            fA = M_in / U_MEV;
            fZ = gMC->TrackCharge();

            AddPoint(fTrackID,
                     fVolumeID,
                     fDetCopyID,
                     fZ,
                     fA,
                     TVector3(fPosIn.X(), fPosIn.Y(), fPosIn.Z()),
                     TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
                     TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
                     TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
                     fTime,
                     fLength,
                     fELoss);

            // Increment number of SofTRIMPoints for this track
            auto* stack = static_cast<R3BStack*>(TVirtualMC::GetMC()->GetStack());
            stack->AddPoint(kSOFTRIM);
            ResetParameters();
        }
    }
    return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofTrim::EndOfEvent()
{
    if (fVerboseLevel)
    {
        Print();
    }
    Reset();
}

// -----   Public method Register   -------------------------------------------
void R3BSofTrim::Register()
{
    FairRootManager::Instance()->Register("SofTrimPoint", GetName(), fSofTRIMCollection, true);
}

// -----   Public method GetCollection   --------------------------------------
TClonesArray* R3BSofTrim::GetCollection(int iColl) const
{
    if (iColl == 0)
    {
        return fSofTRIMCollection;
    }
    else
    {
        return nullptr;
    }
}

// -----   Public method Print   ----------------------------------------------
void R3BSofTrim::Print(Option_t*) const
{
    auto nHits = fSofTRIMCollection->GetEntriesFast();
    LOG(info) << "R3BSofTrim: " << nHits << " points registered in this event";
}

// -----   Public method Reset   ----------------------------------------------
void R3BSofTrim::Reset()
{
    fSofTRIMCollection->Clear();
    ResetParameters();
}

// -----   Private method AddPoint   --------------------------------------------
R3BSofTrimPoint* R3BSofTrim::AddPoint(int trackID,
                                      int detID,
                                      int detCopyID,
                                      double zf,
                                      double af,
                                      TVector3 posIn,
                                      TVector3 posOut,
                                      TVector3 momIn,
                                      TVector3 momOut,
                                      double time,
                                      double length,
                                      double eLoss)
{
    TClonesArray& clref = *fSofTRIMCollection;
    auto size = clref.GetEntriesFast();
    return new (clref[size])
        R3BSofTrimPoint(trackID, detID, detCopyID, zf, af, posIn, posOut, momIn, momOut, time, length, eLoss);
}

// -----  Public method CheckIfSensitive  ----------------------------------
bool R3BSofTrim::CheckIfSensitive(std::string name)
{
    if (TString(name).Contains("TrimAnode"))
    {
        return kTRUE;
    }
    return kFALSE;
}

ClassImp(R3BSofTrim)
