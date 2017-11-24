// -------------------------------------------------------------------------
// -----                        R3BSofMWPC source file                  -----
// -----                  Created 24/11/17  by H.Alvarez-Pol          -----
// -------------------------------------------------------------------------
#include "R3BSofMWPC.h"
#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoNode.h"
#include "FairGeoRootBuilder.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairVolume.h"
#include "R3BSofMWPCPoint.h"
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

R3BSofMWPC::R3BSofMWPC()
  : R3BSofMWPC("") {
}

R3BSofMWPC::R3BSofMWPC(const TString& geoFile, const TGeoTranslation& trans, const TGeoRotation& rot)
  : R3BSofMWPC(geoFile, { trans, rot }) {
}

R3BSofMWPC::R3BSofMWPC(const TString& geoFile, const TGeoCombiTrans& combi)
  : R3BDetector("R3BSofMWPC", kSOFMWPC, geoFile, combi) {
  ResetParameters();
  fSofMWPCCollection = new TClonesArray("R3BSofMWPCPoint");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  flGeoPar = new TList();
  flGeoPar->SetName(GetName());
}

R3BSofMWPC::~R3BSofMWPC() {
  if (flGeoPar) {
    delete flGeoPar;
  }
  if (fSofMWPCCollection) {
    fSofMWPCCollection->Delete();
    delete fSofMWPCCollection;
  }
}

void R3BSofMWPC::Initialize() {
  FairDetector::Initialize();
  
  LOG(INFO) << "R3BSofMWPC: initialisation" << FairLogger::endl;
  LOG(DEBUG) << "-I- R3BSofMWPC: Vol (McId) def" << FairLogger::endl;
  
  TGeoVolume* vol = gGeoManager->GetVolume("SofMWPCWorld");
  vol->SetVisibility(kFALSE);
}

// -----   Public method ProcessHits  --------------------------------------
Bool_t R3BSofMWPC::ProcessHits(FairVolume* vol) {
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
    
    // Set additional parameters at exit of active volume. Create R3BSofMWPCPoint.
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
      
      // Increment number of SofMWPCPoints for this track
      R3BStack* stack = (R3BStack*)gMC->GetStack();
      stack->AddPoint(kSOFMWPC);
      
      ResetParameters();
    }
  }
  return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofMWPC::BeginEvent() {
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofMWPC::EndOfEvent() {
  if (fVerboseLevel)
    Print();
  
  fSofMWPCCollection->Clear();
  
  ResetParameters();
}

// -----   Public method Register   -------------------------------------------
void R3BSofMWPC::Register() {
  FairRootManager::Instance()->Register("SofMWPCPoint", GetName(),
					fSofMWPCCollection, kTRUE);
}

// -----   Public method GetCollection   --------------------------------------
TClonesArray* R3BSofMWPC::GetCollection() const {
  return fSofMWPCCollection;
}

// -----   Public method Print   ----------------------------------------------
void R3BSofMWPC::Print(Option_t* option) const {
  Int_t nHits = fSofMWPCCollection->GetEntriesFast();
  LOG(INFO) << "R3BSofMWPC: " << nHits << " points registered in this event" << FairLogger::endl;
}

// -----   Public method Reset   ----------------------------------------------
void R3BSofMWPC::Reset() {
  fSofMWPCCollection->Clear();
  ResetParameters();
}

// -----   Public method CopyClones   -----------------------------------------
void R3BSofMWPC::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "R3BSofMWPC: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  R3BSofMWPCPoint* oldpoint = NULL;
  for (Int_t i = 0; i < nEntries; i++) {
    oldpoint = (R3BSofMWPCPoint*)cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[fPosIndex]) R3BSofMWPCPoint(*oldpoint);
    fPosIndex++;
  }
  LOG(INFO) << "R3BSofMWPC: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
}

// -----   Private method AddPoint   --------------------------------------------
R3BSofMWPCPoint* R3BSofMWPC::AddPoint(Int_t trackID,
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
  TClonesArray& clref = *fSofMWPCCollection;
  Int_t size = clref.GetEntriesFast();
  if (fVerboseLevel > 1)
    LOG(INFO) << "R3BSofMWPC: Adding Point at (" << posIn.X() << ", " << posIn.Y() << ", " << posIn.Z()
	      << ") cm,  detector " << detID << ", track " << trackID << ", energy loss " << eLoss * 1e06 << " keV"
              << FairLogger::endl;
  return new (clref[size]) R3BSofMWPCPoint(trackID, detID, volid, copy, ident, posIn, posOut, momIn, momOut, time, length, eLoss, Nf, Ns);
}

// -----  Public method CheckIfSensitive  ----------------------------------
Bool_t R3BSofMWPC::CheckIfSensitive(std::string name) {
  if (TString(name).Contains("MWPCVolume_")) {//check at the simulation
    return kTRUE;
  }
  return kFALSE;
}

ClassImp(R3BSofMWPC)
