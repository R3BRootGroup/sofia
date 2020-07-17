// -------------------------------------------------------------------------
// -----                        R3BSofAT source file                  -----
// -----                  Created 24/11/17  by H.Alvarez-Pol          -----
// -------------------------------------------------------------------------
#include "R3BSofAT.h"
#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoNode.h"
#include "FairGeoRootBuilder.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairVolume.h"
#include "R3BDetectorList.h"
#include "R3BMCStack.h"
#include "R3BSofATPoint.h"
#include "TClonesArray.h"
#include "TGeoCompositeShape.h"
#include "TGeoMCGeometry.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMatrix.h"
#include "TGeoMedium.h"
#include "TMCProcess.h"
#include "TObjArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TVirtualMCStack.h"
#include <iostream>
#include <stdlib.h>

#define U_MEV 931.4940954

R3BSofAT::R3BSofAT()
    : R3BSofAT("")
{
}

R3BSofAT::R3BSofAT(const TString& geoFile, const TGeoTranslation& trans, const TGeoRotation& rot)
    : R3BSofAT(geoFile, { trans, rot })
{
}

R3BSofAT::R3BSofAT(const TString& geoFile, const TGeoCombiTrans& combi)
    : R3BDetector("R3BSofAT", kSOFAT, geoFile, combi)
{
    ResetParameters();
    fSofATCollection = new TClonesArray("R3BSofATPoint");
    fPosIndex = 0;
    kGeoSaved = kFALSE;
    flGeoPar = new TList();
    flGeoPar->SetName(GetName());
}

R3BSofAT::~R3BSofAT()
{
    if (flGeoPar)
    {
        delete flGeoPar;
    }
    if (fSofATCollection)
    {
        fSofATCollection->Delete();
        delete fSofATCollection;
    }
}

void R3BSofAT::Initialize()
{
    FairDetector::Initialize();

    LOG(INFO) << "R3BSofAT: initialisation";
    LOG(DEBUG) << "-I- R3BSofAT: Vol (McId) def" << gMC->VolId("SOFATLog");
}

// -----   Public method ProcessHits  --------------------------------------
Bool_t R3BSofAT::ProcessHits(FairVolume* vol)
{
    Int_t nodeId = 0;
    if (gMC->IsTrackEntering())
    {
        gGeoManager->cd(gMC->CurrentVolPath());
        nodeId = gGeoManager->GetNodeId();
    }
    if (gMC->IsTrackEntering())
    {
        fELoss = 0.;
        fNf = 0.;
        fNs = 0.;
        fNSteps = 0; // FIXME
        fTime = gMC->TrackTime() * 1.0e09;
        fLength = gMC->TrackLength();
        gMC->TrackPosition(fPosIn);
        gMC->TrackMomentum(fMomIn);
        fEinc = gMC->Etot() - gMC->TrackMass(); // be aware!! Relativistic mass!
    }

    // Sum energy loss for all steps in the active volume
    Double_t dE = gMC->Edep() * 1000.;                          // in MeV
    Double_t post_E = (gMC->Etot() - gMC->TrackMass()) * 1000.; // in MeV
    TString ptype = gMC->GetStack()->GetCurrentTrack()->GetName();

    Double_t M_in = gMC->TrackMass() * 1000.;
    Double_t A_in = M_in / U_MEV;
    Double_t Z_in = gMC->TrackCharge();

    fELoss += dE / 1000.; // back to GeV

    if (dE > 0 /*&& dx > 0*/)
    {

        fNSteps++;

        // Set additional parameters at exit of active volume. Create R3BSofATPoint.
        if (gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared())
        {

            fTrackID = gMC->GetStack()->GetCurrentTrackNumber();
            fParentTrackID = gMC->GetStack()->GetCurrentParentTrackNumber();
            fVolumeID = vol->getMCid();
            fTrackPID = gMC->TrackPid();
            fUniqueID = gMC->GetStack()->GetCurrentTrack()->GetUniqueID();

            gMC->TrackPosition(fPosOut);
            gMC->TrackMomentum(fMomOut);

            if (fELoss == 0.)
                return kFALSE;

            AddPoint(fTrackID,
                     nodeId,
                     fVolumeID,
                     0,
                     0,
                     TVector3(fPosIn.X(), fPosIn.Y(), fPosIn.Z()),
                     TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
                     TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
                     TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
                     fTime,
                     fLength,
                     fELoss);

            // Increment number of SofATPoints for this track
            R3BStack* stack = (R3BStack*)gMC->GetStack();
            stack->AddPoint(kSOFAT);

            ResetParameters();
        }
    }
    return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofAT::BeginEvent() {}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofAT::EndOfEvent()
{
    if (fVerboseLevel)
        Print();

    fSofATCollection->Clear();

    ResetParameters();
}

// -----   Public method Register   -------------------------------------------
void R3BSofAT::Register() { FairRootManager::Instance()->Register("SofATPoint", GetName(), fSofATCollection, kTRUE); }

// -----   Public method GetCollection   --------------------------------------
TClonesArray* R3BSofAT::GetCollection(Int_t iColl) const { return fSofATCollection; }

// -----   Public method Print   ----------------------------------------------
void R3BSofAT::Print(Option_t* option) const
{
    Int_t nHits = fSofATCollection->GetEntriesFast();
    LOG(INFO) << "R3BSofAT: " << nHits << " points registered in this event";
}

// -----   Public method Reset   ----------------------------------------------
void R3BSofAT::Reset()
{
    fSofATCollection->Clear();
    ResetParameters();
}

// -----   Public method CopyClones   -----------------------------------------
void R3BSofAT::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{
    Int_t nEntries = cl1->GetEntriesFast();
    LOG(INFO) << "R3BSofAT: " << nEntries << " entries to add";
    TClonesArray& clref = *cl2;
    R3BSofATPoint* oldpoint = NULL;
    for (Int_t i = 0; i < nEntries; i++)
    {
        oldpoint = (R3BSofATPoint*)cl1->At(i);
        Int_t index = oldpoint->GetTrackID() + offset;
        oldpoint->SetTrackID(index);
        new (clref[fPosIndex]) R3BSofATPoint(*oldpoint);
        fPosIndex++;
    }
    LOG(INFO) << "R3BSofAT: " << cl2->GetEntriesFast() << " merged entries";
}

// -----   Private method AddPoint   --------------------------------------------
R3BSofATPoint* R3BSofAT::AddPoint(Int_t trackID,
                                  Int_t detID,
                                  Int_t volid,
                                  Int_t copy,
                                  Int_t ident,
                                  TVector3 posIn,
                                  TVector3 posOut,
                                  TVector3 momIn,
                                  TVector3 momOut,
                                  Double_t time,
                                  Double_t length,
                                  Double_t eLoss)
{
    TClonesArray& clref = *fSofATCollection;
    Int_t size = clref.GetEntriesFast();
    if (fVerboseLevel > 1)
        LOG(INFO) << "R3BSofAT: Adding Point at (" << posIn.X() << ", " << posIn.Y() << ", " << posIn.Z()
                  << ") cm,  detector " << detID << ", track " << trackID << ", energy loss " << eLoss * 1e06 << " keV";
    return new (clref[size])
        R3BSofATPoint(trackID, detID, volid, copy, ident, posIn, posOut, momIn, momOut, time, length, eLoss);
}

// -----  Public method CheckIfSensitive  ----------------------------------
Bool_t R3BSofAT::CheckIfSensitive(std::string name)
{
    if (TString(name).Contains("SOFATLog"))
    { // check at the simulation
        return kTRUE;
    }
    return kFALSE;
}

ClassImp(R3BSofAT)
