// -------------------------------------------------------------------------
// -----                        R3BSofTrim source file                 -----
// -----                 Created 24/11/17 by H.Alvarez-Pol             -----
// -------------------------------------------------------------------------

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairVolume.h"

#include "R3BLogger.h"
#include "R3BMCStack.h"
#include "R3BSofTrim.h"
#include "R3BSofTrimPoint.h"

#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TParticle.h"
#include "TVirtualMC.h"

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
Bool_t R3BSofTrim::ProcessHits(FairVolume* vol)
{
    if (gMC->IsTrackEntering())
    {
        gGeoManager->cd(gMC->CurrentVolPath());
        Int_t nodeId = gGeoManager->GetNodeId();
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
            R3BStack* stack = static_cast<R3BStack*>(TVirtualMC::GetMC()->GetStack());
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
    FairRootManager::Instance()->Register("SofTrimPoint", GetName(), fSofTRIMCollection, kTRUE);
}

// -----   Public method GetCollection   --------------------------------------
TClonesArray* R3BSofTrim::GetCollection(Int_t iColl) const
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
void R3BSofTrim::Print(Option_t* option) const
{
    Int_t nHits = fSofTRIMCollection->GetEntriesFast();
    R3BLOG(info, nHits << " points registered in this event");
}

// -----   Public method Reset   ----------------------------------------------
void R3BSofTrim::Reset()
{
    fSofTRIMCollection->Clear();
    ResetParameters();
}

// -----   Private method AddPoint   --------------------------------------------
R3BSofTrimPoint* R3BSofTrim::AddPoint(Int_t trackID,
                                      Int_t detID,
                                      Int_t detCopyID,
                                      Double_t zf,
                                      Double_t af,
                                      TVector3 posIn,
                                      TVector3 posOut,
                                      TVector3 momIn,
                                      TVector3 momOut,
                                      Double_t time,
                                      Double_t length,
                                      Double_t eLoss)
{
    TClonesArray& clref = *fSofTRIMCollection;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size])
        R3BSofTrimPoint(trackID, detID, detCopyID, zf, af, posIn, posOut, momIn, momOut, time, length, eLoss);
}

// -----  Public method CheckIfSensitive  ----------------------------------
Bool_t R3BSofTrim::CheckIfSensitive(std::string name)
{
    if (TString(name).Contains("TrimAnode"))
    {
        return kTRUE;
    }
    return kFALSE;
}

ClassImp(R3BSofTrim);
