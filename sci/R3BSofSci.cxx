// -------------------------------------------------------------------------
// -----                        R3BSofSci source file                  -----
// -----                  Created 24/11/17  by H.Alvarez-Pol           -----
// -------------------------------------------------------------------------
#include "R3BSofSci.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairVolume.h"
#include "R3BMCStack.h"
#include "R3BSofSciPoint.h"
#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TMCProcess.h"
#include "TObjArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TVirtualMCStack.h"

R3BSofSci::R3BSofSci()
    : R3BSofSci("")
{
}

R3BSofSci::R3BSofSci(const TString& geoFile, const TGeoTranslation& trans, const TGeoRotation& rot)
    : R3BSofSci(geoFile, { trans, rot })
{
}

R3BSofSci::R3BSofSci(const TString& geoFile, const TGeoCombiTrans& combi)
    : R3BDetector("R3BSofSci", kSOFSCI, geoFile, combi)
{
    ResetParameters();
    fSofSCICollection = new TClonesArray("R3BSofSciPoint");
    fPosIndex = 0;
    kGeoSaved = kFALSE;
    flGeoPar = new TList();
    flGeoPar->SetName(GetName());
}

R3BSofSci::~R3BSofSci()
{
    if (flGeoPar)
    {
        delete flGeoPar;
    }
    if (fSofSCICollection)
    {
        fSofSCICollection->Delete();
        delete fSofSCICollection;
    }
}

void R3BSofSci::Initialize()
{
    FairDetector::Initialize();

    LOG(info) << "R3BSofSci: initialisation";
    LOG(debug) << "R3BSofSci: Vol (McId) def";
}

// -----   Public method ProcessHits  --------------------------------------
Bool_t R3BSofSci::ProcessHits(FairVolume* vol)
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

    if (gMC->Edep() > 0)
    {

        fNSteps++;

        // Set additional parameters at exit of active volume. Create R3BSofSciPoint.
        if (gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared())
        {

            fTrackID = gMC->GetStack()->GetCurrentTrackNumber();
            fVolumeID = vol->getMCid();
            fDetCopyID = vol->getCopyNo();
            fUniqueID = gMC->GetStack()->GetCurrentTrack()->GetUniqueID();

            gMC->TrackPosition(fPosOut);
            gMC->TrackMomentum(fMomOut);

            if (fELoss == 0.)
                return kFALSE;

            AddPoint(fTrackID,
                     fVolumeID,
                     fDetCopyID,
                     TVector3(fPosIn.X(), fPosIn.Y(), fPosIn.Z()),
                     TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
                     TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
                     TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
                     fTime,
                     fLength,
                     fELoss);

            // Increment number of SofSCIPoints for this track
            R3BStack* stack = (R3BStack*)gMC->GetStack();
            stack->AddPoint(kSOFSCI);

            ResetParameters();
        }
    }
    return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofSci::BeginEvent() {}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofSci::EndOfEvent()
{
    if (fVerboseLevel)
        Print();

    fSofSCICollection->Clear();
    ResetParameters();
}

// -----   Public method Register   -------------------------------------------
void R3BSofSci::Register()
{
    FairRootManager::Instance()->Register("SofSciPoint", GetName(), fSofSCICollection, kTRUE);
}

// -----   Public method GetCollection   --------------------------------------
TClonesArray* R3BSofSci::GetCollection(Int_t iColl) const
{
    if (iColl == 0)
        return fSofSCICollection;
    else
        return NULL;
}

// -----   Public method Print   ----------------------------------------------
void R3BSofSci::Print(Option_t* option) const
{
    Int_t nHits = fSofSCICollection->GetEntriesFast();
    LOG(info) << "R3BSofSci: " << nHits << " points registered in this event";
}

// -----   Public method Reset   ----------------------------------------------
void R3BSofSci::Reset()
{
    fSofSCICollection->Clear();
    ResetParameters();
}

// -----   Public method CopyClones   -----------------------------------------
void R3BSofSci::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{
    Int_t nEntries = cl1->GetEntriesFast();
    LOG(info) << "R3BSofSci: " << nEntries << " entries to add";
    TClonesArray& clref = *cl2;
    R3BSofSciPoint* oldpoint = NULL;
    for (Int_t i = 0; i < nEntries; i++)
    {
        oldpoint = (R3BSofSciPoint*)cl1->At(i);
        Int_t index = oldpoint->GetTrackID() + offset;
        oldpoint->SetTrackID(index);
        new (clref[fPosIndex]) R3BSofSciPoint(*oldpoint);
        fPosIndex++;
    }
    LOG(info) << "R3BSofSci: " << cl2->GetEntriesFast() << " merged entries";
}

// -----   Private method AddPoint   --------------------------------------------
R3BSofSciPoint* R3BSofSci::AddPoint(Int_t trackID,
                                    Int_t detID,
                                    Int_t copyID,
                                    TVector3 posIn,
                                    TVector3 posOut,
                                    TVector3 momIn,
                                    TVector3 momOut,
                                    Double_t time,
                                    Double_t length,
                                    Double_t eLoss)
{
    TClonesArray& clref = *fSofSCICollection;
    Int_t size = clref.GetEntriesFast();
    if (fVerboseLevel > 1)
        LOG(info) << "R3BSofSci: Adding Point at (" << posIn.X() << ", " << posIn.Y() << ", " << posIn.Z()
                  << ") cm,  detector " << detID << ", track " << trackID << ", energy loss " << eLoss * 1e06 << " keV";
    return new (clref[size]) R3BSofSciPoint(trackID, detID, copyID, posIn, posOut, momIn, momOut, time, length, eLoss);
}

// -----  Public method CheckIfSensitive  ----------------------------------
Bool_t R3BSofSci::CheckIfSensitive(std::string name)
{
    if (TString(name).Contains("Sci_"))
    { // check at the simulation
        return kTRUE;
    }
    return kFALSE;
}

ClassImp(R3BSofSci);
