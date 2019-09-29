// -------------------------------------------------------------------------
// -----                        R3BSofTWIM source file                 -----
// -----                  Created 06/12/17  by JL Rodriguez            -----
// -------------------------------------------------------------------------
#include "R3BSofTWIM.h"
#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoNode.h"
#include "FairGeoRootBuilder.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairVolume.h"
#include "R3BSofTWIMPoint.h"
#include "R3BMCStack.h"
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

using std::cout;
using std::cerr;
using std::endl;

#define U_MEV 931.4940954

// -----   Default constructor
R3BSofTWIM::R3BSofTWIM()
  : R3BSofTWIM("") {
}

R3BSofTWIM::R3BSofTWIM(const TString& geoFile, const TGeoTranslation& trans, const TGeoRotation& rot)
  : R3BSofTWIM(geoFile, { trans, rot }) {
}

// -----   Standard constructor
R3BSofTWIM::R3BSofTWIM(const TString& geoFile, const TGeoCombiTrans& combi)
  : R3BDetector("R3BSofTWIM", kSOFTWIM, geoFile, combi)
  , fSofTWIMCollection(new TClonesArray("R3BSofTWIMPoint"))
  , fPosIndex(0)
  , kGeoSaved(kFALSE)
  , flGeoPar(new TList())
{
  flGeoPar->SetName(GetName());
  ResetParameters();
}

R3BSofTWIM::~R3BSofTWIM() {
  if (flGeoPar) {
    delete flGeoPar;
  }
  if (fSofTWIMCollection) {
    fSofTWIMCollection->Delete();
    delete fSofTWIMCollection;
  }
}

void R3BSofTWIM::Initialize() {
  FairDetector::Initialize();

  LOG(INFO) << "R3BSofTWIM: initialisation";
  LOG(DEBUG) << "-I- R3BSofTWIM: Vol (McId) def" << gMC->VolId("TwinLog");

  //TGeoVolume* vol = gGeoManager->GetVolume("SofTRIMWorld");
  //vol->SetVisibility(kFALSE);
}

// -----   Public method ProcessHits  --------------------------------------
Bool_t R3BSofTWIM::ProcessHits(FairVolume* vol) {


  if (gMC->IsTrackEntering() ) {
    gGeoManager->cd(gMC->CurrentVolPath());
    Int_t nodeId = gGeoManager->GetNodeId();
  }
  if (gMC->IsTrackEntering()) {
    fELoss = 0.;
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
  Double_t fA_in = M_in / U_MEV;
  Double_t fZ_in = gMC->TrackCharge();

  fELoss += dE / 1000.; // back to GeV

  if (dE > 0) {

    fNSteps++;

    // Set additional parameters at exit of active volume. Create R3BSofTWIMPoint.
    if (gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared()) {

      fTrackID = gMC->GetStack()->GetCurrentTrackNumber();
      fParentTrackID = gMC->GetStack()->GetCurrentParentTrackNumber();
      fVolumeID = vol->getMCid();
      fDetCopyID = vol->getCopyNo();
      fTrackPID = gMC->TrackPid();
      fUniqueID = gMC->GetStack()->GetCurrentTrack()->GetUniqueID();

      gMC->TrackPosition(fPosOut);
      gMC->TrackMomentum(fMomOut);

      if (fELoss == 0.)
	return kFALSE;


      AddPoint(fTrackID,
               fVolumeID,
               fDetCopyID,
               fZ_in,
               fA_in,
               TVector3(fPosIn.X(), fPosIn.Y(), fPosIn.Z()),
               TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
               TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
               TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
               fTime,
               fLength,
               fELoss);

      // Increment number of SofTRIMPoints for this track
      R3BStack* stack = (R3BStack*)gMC->GetStack();
      stack->AddPoint(kSOFTWIM);

      ResetParameters();
    }
  }


  return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofTWIM::BeginEvent() {
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofTWIM::EndOfEvent() {
  if (fVerboseLevel)
    Print();

  fSofTWIMCollection->Clear();

  ResetParameters();
}

// -----   Public method Register   -------------------------------------------
void R3BSofTWIM::Register() {
  FairRootManager::Instance()->Register("SofTWIMPoint", GetName(),
					fSofTWIMCollection, kTRUE);
}

// -----   Public method GetCollection   --------------------------------------
TClonesArray* R3BSofTWIM::GetCollection(Int_t iColl) const {
    if (iColl == 0)
        return fSofTWIMCollection;
    else
        return NULL;
}

// -----   Public method Print   ----------------------------------------------
void R3BSofTWIM::Print(Option_t* option) const {
  Int_t nHits = fSofTWIMCollection->GetEntriesFast();
  LOG(INFO) << "R3BSofTWIM: " << nHits << " points registered in this event";
}

// -----   Public method Reset   ----------------------------------------------
void R3BSofTWIM::Reset() {
  fSofTWIMCollection->Clear();
  ResetParameters();
}

// -----   Public method CopyClones   -----------------------------------------
void R3BSofTWIM::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "R3BSofTWIM: " << nEntries << " entries to add";
  TClonesArray& clref = *cl2;
  R3BSofTWIMPoint* oldpoint = NULL;
  for (Int_t i = 0; i < nEntries; i++) {
    oldpoint = (R3BSofTWIMPoint*)cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[fPosIndex]) R3BSofTWIMPoint(*oldpoint);
    fPosIndex++;
  }
  LOG(INFO) << "R3BSofTWIM: " << cl2->GetEntriesFast() << " merged entries";
}

// -----   Private method AddPoint   --------------------------------------------
R3BSofTWIMPoint* R3BSofTWIM::AddPoint(Int_t trackID,
					    Int_t detID,
					    Int_t volid,
					    Double_t Z,
					    Double_t A,
					    TVector3 posIn,
					    TVector3 posOut,
					    TVector3 momIn,
					    TVector3 momOut,
					    Double_t time,
					    Double_t length,
					    Double_t eLoss) {
  TClonesArray& clref = *fSofTWIMCollection;
  Int_t size = clref.GetEntriesFast();
  if (fVerboseLevel > 1)
    LOG(INFO) << "R3BSofTWIM: Adding Point at (" << posIn.X() << ", " << posIn.Y() << ", " << posIn.Z()
	      << ") cm,  detector " << detID << ", track " << trackID << ", energy loss " << eLoss * 1e06 << " keV";
  return new (clref[size]) R3BSofTWIMPoint(trackID, detID, volid, Z, A, posIn, posOut, momIn, momOut, time, length, eLoss);
}

// -----  Public method CheckIfSensitive  ----------------------------------
Bool_t R3BSofTWIM::CheckIfSensitive(std::string name) {
  if (TString(name).Contains("TwinLog")) {
        LOG(INFO) << "Found TWIN MUSIC geometry from ROOT file: " << name;
        return kTRUE;
    }
  return kFALSE;
}

ClassImp(R3BSofTWIM)
