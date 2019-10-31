// -------------------------------------------------------------------------
// -----                        R3BSofTRIM source file                  -----
// -----                  Created 24/11/17  by H.Alvarez-Pol          -----
// -------------------------------------------------------------------------
#include "R3BSofTRIM.h"
#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoNode.h"
#include "FairGeoRootBuilder.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairVolume.h"
#include "R3BMCStack.h"
#include "R3BSofTRIMPoint.h"
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

R3BSofTRIM::R3BSofTRIM()
    : R3BSofTRIM("")
{
}

R3BSofTRIM::R3BSofTRIM(const TString& geoFile, const TGeoTranslation& trans, const TGeoRotation& rot)
    : R3BSofTRIM(geoFile, { trans, rot })
{
}

R3BSofTRIM::R3BSofTRIM(const TString& geoFile, const TGeoCombiTrans& combi)
    : R3BDetector("R3BSofTRIM", kSOFTRIM, geoFile, combi)
{
    ResetParameters();
    fSofTRIMCollection = new TClonesArray("R3BSofTRIMPoint");
    fPosIndex = 0;
    kGeoSaved = kFALSE;
    flGeoPar = new TList();
    flGeoPar->SetName(GetName());
}

R3BSofTRIM::~R3BSofTRIM()
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

void R3BSofTRIM::Initialize()
{
    FairDetector::Initialize();

    LOG(INFO) << "R3BSofTRIM: initialisation";
    LOG(DEBUG) << "-I- R3BSofTRIM: Vol (McId) def";

    TGeoVolume* vol = gGeoManager->GetVolume("SofTRIMWorld");
    vol->SetVisibility(kFALSE);
}

// -----   Public method ProcessHits  --------------------------------------
Bool_t R3BSofTRIM::ProcessHits(FairVolume* vol)
{
    if (gMC->IsTrackEntering())
    {
        gGeoManager->cd(gMC->CurrentVolPath());
        Int_t nodeId = gGeoManager->GetNodeId();
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

    if (dE > 0 && dx > 0)
    {

        fNSteps++;

        // Set additional parameters at exit of active volume. Create R3BSofTRIMPoint.
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

            // Increment number of SofTRIMPoints for this track
            R3BStack* stack = (R3BStack*)gMC->GetStack();
            stack->AddPoint(kSOFTRIM);

            ResetParameters();
        }
    }
    return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofTRIM::BeginEvent() {}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofTRIM::EndOfEvent()
{
    if (fVerboseLevel)
        Print();

    fSofTRIMCollection->Clear();

    ResetParameters();
}

// -----   Public method Register   -------------------------------------------
void R3BSofTRIM::Register()
{
    FairRootManager::Instance()->Register("SofTRIMPoint", GetName(), fSofTRIMCollection, kTRUE);
}

// -----   Public method GetCollection   --------------------------------------
TClonesArray* R3BSofTRIM::GetCollection() const { return fSofTRIMCollection; }

// -----   Public method Print   ----------------------------------------------
void R3BSofTRIM::Print(Option_t* option) const
{
    Int_t nHits = fSofTRIMCollection->GetEntriesFast();
    LOG(INFO) << "R3BSofTRIM: " << nHits << " points registered in this event";
}

// -----   Public method Reset   ----------------------------------------------
void R3BSofTRIM::Reset()
{
    fSofTRIMCollection->Clear();
    ResetParameters();
}

// -----   Public method CopyClones   -----------------------------------------
void R3BSofTRIM::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{
    Int_t nEntries = cl1->GetEntriesFast();
    LOG(INFO) << "R3BSofTRIM: " << nEntries << " entries to add";
    TClonesArray& clref = *cl2;
    R3BSofTRIMPoint* oldpoint = NULL;
    for (Int_t i = 0; i < nEntries; i++)
    {
        oldpoint = (R3BSofTRIMPoint*)cl1->At(i);
        Int_t index = oldpoint->GetTrackID() + offset;
        oldpoint->SetTrackID(index);
        new (clref[fPosIndex]) R3BSofTRIMPoint(*oldpoint);
        fPosIndex++;
    }
    LOG(INFO) << "R3BSofTRIM: " << cl2->GetEntriesFast() << " merged entries";
}

// -----   Private method AddPoint   --------------------------------------------
R3BSofTRIMPoint* R3BSofTRIM::AddPoint(Int_t trackID,
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
    TClonesArray& clref = *fSofTRIMCollection;
    Int_t size = clref.GetEntriesFast();
    if (fVerboseLevel > 1)
        LOG(INFO) << "R3BSofTRIM: Adding Point at (" << posIn.X() << ", " << posIn.Y() << ", " << posIn.Z()
                  << ") cm,  detector " << detID << ", track " << trackID << ", energy loss " << eLoss * 1e06 << " keV";
    return new (clref[size])
        R3BSofTRIMPoint(trackID, detID, volid, copy, ident, posIn, posOut, momIn, momOut, time, length, eLoss, Nf, Ns);
}

// -----  Public method CheckIfSensitive  ----------------------------------
Bool_t R3BSofTRIM::CheckIfSensitive(std::string name)
{
    if (TString(name).Contains("TRIMVolume_"))
    { // check at the simulation
        return kTRUE;
    }
    return kFALSE;
}

ClassImp(R3BSofTRIM)
