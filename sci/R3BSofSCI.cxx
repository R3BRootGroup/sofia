// -------------------------------------------------------------------------
// -----                        R3BSofSCI source file                  -----
// -----                  Created 24/11/17  by H.Alvarez-Pol          -----
// -------------------------------------------------------------------------
#include "R3BSofSCI.h"
#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoNode.h"
#include "FairGeoRootBuilder.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairVolume.h"
#include "R3BSofSCIPoint.h"
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

R3BSofSCI::R3BSofSCI()
  : R3BSofSCI("") {
}

R3BSofSCI::R3BSofSCI(const TString& geoFile, const TGeoTranslation& trans, const TGeoRotation& rot)
  : R3BSofSCI(geoFile, { trans, rot }) {
}

R3BSofSCI::R3BSofSCI(const TString& geoFile, const TGeoCombiTrans& combi)
  : R3BDetector("R3BSofSCI", kSOFSCI, geoFile, combi) {
  ResetParameters();
  fSofSCICollection = new TClonesArray("R3BSofSCIPoint");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  flGeoPar = new TList();
  flGeoPar->SetName(GetName());
}

R3BSofSCI::~R3BSofSCI() {
  if (flGeoPar) {
    delete flGeoPar;
  }
  if (fSofSCICollection) {
    fSofSCICollection->Delete();
    delete fSofSCICollection;
  }
}

void R3BSofSCI::Initialize() {
  FairDetector::Initialize();
  
  LOG(INFO) << "R3BSofSCI: initialisation";
  LOG(DEBUG) << "-I- R3BSofSCI: Vol (McId) def";
  
  TGeoVolume* vol = gGeoManager->GetVolume("SofSCIWorld");
  vol->SetVisibility(kFALSE);
}

// -----   Public method ProcessHits  --------------------------------------
Bool_t R3BSofSCI::ProcessHits(FairVolume* vol) {
  if (gMC->IsTrackEntering() ) {
    gGeoManager->cd(gMC->CurrentVolPath());
    Int_t nodeId = gGeoManager->GetNodeId();
  }
  if (gMC->IsTrackEntering()) {
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
  
  if (dE > 0 && dx > 0) {
    
    fNSteps++;
    
    // Set additional parameters at exit of active volume. Create R3BSofSCIPoint.
    if (gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared()) {
      
      fTrackID = gMC->GetStack()->GetCurrentTrackNumber();
      fParentTrackID = gMC->GetStack()->GetCurrentParentTrackNumber();
      fVolumeID = vol->getMCid();
      fTrackPID = gMC->TrackPid();
      fUniqueID = gMC->GetStack()->GetCurrentTrack()->GetUniqueID();
      
      gMC->TrackPosition(fPosOut);
      gMC->TrackMomentum(fMomOut);
      
      if (fELoss == 0.)
	return kFALSE;
      
      // Increment number of SofSCIPoints for this track
      R3BStack* stack = (R3BStack*)gMC->GetStack();
      stack->AddPoint(kSOFSCI);
      
      ResetParameters();
    }
  }
  return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofSCI::BeginEvent() {
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofSCI::EndOfEvent() {
  if (fVerboseLevel)
    Print();
  
  fSofSCICollection->Clear();
  
  ResetParameters();
}

// -----   Public method Register   -------------------------------------------
void R3BSofSCI::Register() {
  FairRootManager::Instance()->Register("SofSCIPoint", GetName(),
					fSofSCICollection, kTRUE);
}

// -----   Public method GetCollection   --------------------------------------
TClonesArray* R3BSofSCI::GetCollection() const {
  return fSofSCICollection;
}

// -----   Public method Print   ----------------------------------------------
void R3BSofSCI::Print(Option_t* option) const {
  Int_t nHits = fSofSCICollection->GetEntriesFast();
  LOG(INFO) << "R3BSofSCI: " << nHits << " points registered in this event";
}

// -----   Public method Reset   ----------------------------------------------
void R3BSofSCI::Reset() {
  fSofSCICollection->Clear();
  ResetParameters();
}

// -----   Public method CopyClones   -----------------------------------------
void R3BSofSCI::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "R3BSofSCI: " << nEntries << " entries to add";
  TClonesArray& clref = *cl2;
  R3BSofSCIPoint* oldpoint = NULL;
  for (Int_t i = 0; i < nEntries; i++) {
    oldpoint = (R3BSofSCIPoint*)cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[fPosIndex]) R3BSofSCIPoint(*oldpoint);
    fPosIndex++;
  }
  LOG(INFO) << "R3BSofSCI: " << cl2->GetEntriesFast() << " merged entries";
}

// -----   Private method AddPoint   --------------------------------------------
R3BSofSCIPoint* R3BSofSCI::AddPoint(Int_t trackID,
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
				      Double_t eLoss) {
  TClonesArray& clref = *fSofSCICollection;
  Int_t size = clref.GetEntriesFast();
  if (fVerboseLevel > 1)
    LOG(INFO) << "R3BSofSCI: Adding Point at (" << posIn.X() << ", " << posIn.Y() << ", " << posIn.Z()
	      << ") cm,  detector " << detID << ", track " << trackID << ", energy loss " << eLoss * 1e06 << " keV";
  return new (clref[size]) R3BSofSCIPoint(trackID, detID, volid, copy, ident, posIn, posOut, momIn, momOut, time, length, eLoss, Nf, Ns);
}

// -----  Public method CheckIfSensitive  ----------------------------------
Bool_t R3BSofSCI::CheckIfSensitive(std::string name) {
  if (TString(name).Contains("SCIVolume_")) {//check at the simulation
    return kTRUE;
  }
  return kFALSE;
}

ClassImp(R3BSofSCI)
