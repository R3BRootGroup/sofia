// -------------------------------------------------------------------------
// -----                        R3BSofTrim source file                 -----
// -----                 Created 24/11/17 by H.Alvarez-Pol             -----
// -------------------------------------------------------------------------
#include "R3BSofTrim.h"

#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoNode.h"
#include "FairGeoRootBuilder.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairVolume.h"

#include "R3BMCStack.h"
#include "R3BSofTrimPoint.h"
#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TMCProcess.h"
#include "TObjArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TVirtualMCStack.h"
#include <iostream>
#include <stdlib.h>

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
{
    ResetParameters();
    fSofTRIMCollection = new TClonesArray("R3BSofTrimPoint");
    fPosIndex = 0;
    kGeoSaved = kFALSE;
    flGeoPar = new TList();
    flGeoPar->SetName(GetName());
}

R3BSofTrim::~R3BSofTrim()
{
    if (flGeoPar)
    {
        delete flGeoPar;
    }
    if (fSofTRIMCollection)
    {
        fSofTRIMCollection->Delete();
        delete fSofTRIMCollection;
    }
}

void R3BSofTrim::Initialize()
{
    FairDetector::Initialize();

    LOG(INFO) << "R3BSofTrim: initialisation";
    LOG(DEBUG) << "R3BSofTrim: Vol (McId) def";
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
        return kFALSE;

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
            R3BStack* stack = (R3BStack*)gMC->GetStack();
            stack->AddPoint(kSOFTRIM);

            ResetParameters();
        }
    }
    return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofTrim::BeginEvent() {}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofTrim::EndOfEvent()
{
    if (fVerboseLevel)
        Print();

    fSofTRIMCollection->Clear();

    ResetParameters();
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
        return fSofTRIMCollection;
    else
        return NULL;
}

// -----   Public method Print   ----------------------------------------------
void R3BSofTrim::Print(Option_t* option) const
{
    Int_t nHits = fSofTRIMCollection->GetEntriesFast();
    LOG(INFO) << "R3BSofTrim: " << nHits << " points registered in this event";
}

// -----   Public method Reset   ----------------------------------------------
void R3BSofTrim::Reset()
{
    fSofTRIMCollection->Clear();
    ResetParameters();
}

// -----   Public method CopyClones   -----------------------------------------
void R3BSofTrim::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{
    Int_t nEntries = cl1->GetEntriesFast();
    LOG(INFO) << "R3BSofTrim: " << nEntries << " entries to add";
    TClonesArray& clref = *cl2;
    R3BSofTrimPoint* oldpoint = NULL;
    for (Int_t i = 0; i < nEntries; i++)
    {
        oldpoint = (R3BSofTrimPoint*)cl1->At(i);
        Int_t index = oldpoint->GetTrackID() + offset;
        oldpoint->SetTrackID(index);
        new (clref[fPosIndex]) R3BSofTrimPoint(*oldpoint);
        fPosIndex++;
    }
    LOG(INFO) << "R3BSofTrim: " << cl2->GetEntriesFast() << " merged entries";
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
    if (fVerboseLevel > 1)
        LOG(INFO) << "R3BSofTrim: Adding Point at (" << posIn.X() << ", " << posIn.Y() << ", " << posIn.Z()
                  << ") cm,  detector " << detID << ", track " << trackID << ", energy loss " << eLoss * 1e06 << " keV";
    return new (clref[size])
        R3BSofTrimPoint(trackID, detID, detCopyID, zf, af, posIn, posOut, momIn, momOut, time, length, eLoss);
}

// -----  Public method CheckIfSensitive  ----------------------------------
Bool_t R3BSofTrim::CheckIfSensitive(std::string name)
{
    if (TString(name).Contains("TrimAnode"))
    { // check at the simulation
        return kTRUE;
    }
    return kFALSE;
}

ClassImp(R3BSofTrim);
