// -------------------------------------------------------------------------
// -----                        R3BSofTofWall source file                  -----
// -----                  Created 24/11/17  by H.Alvarez-Pol          -----
// -------------------------------------------------------------------------
#include "R3BSofTofWall.h"
#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoNode.h"
#include "FairGeoRootBuilder.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairVolume.h"
#include "R3BMCStack.h"
#include "R3BSofToFWCalData.h"
#include "R3BSofToFWPoint.h"
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

R3BSofTofWall::R3BSofTofWall()
    : R3BSofTofWall("")
{
}

R3BSofTofWall::R3BSofTofWall(const TString& geoFile, const TGeoTranslation& trans, const TGeoRotation& rot)
    : R3BSofTofWall(geoFile, { trans, rot })
{
}

R3BSofTofWall::R3BSofTofWall(const TString& geoFile, const TGeoCombiTrans& combi)
    : R3BDetector("R3BSofTofWall", kSOFTofWall, geoFile, combi)
    , fSofTofWallCollection(new TClonesArray("R3BSofToFWPoint"))
    , fSofTofWallCalCollection(new TClonesArray("R3BSofToFWCalData"))
    , fPosIndex(0)
    , kGeoSaved(kFALSE)
    , flGeoPar(new TList())
{
    flGeoPar->SetName(GetName());
    ResetParameters();
}

R3BSofTofWall::~R3BSofTofWall()
{
    if (flGeoPar)
    {
        delete flGeoPar;
    }
    if (fSofTofWallCollection)
    {
        fSofTofWallCollection->Delete();
        delete fSofTofWallCollection;
    }
    if (fSofTofWallCalCollection)
    {
        fSofTofWallCalCollection->Delete();
        delete fSofTofWallCalCollection;
    }
}

void R3BSofTofWall::Initialize()
{
    FairDetector::Initialize();

    LOG(INFO) << "R3BSofTofWall: initialisation";
    LOG(DEBUG) << "-I- R3BSofTofWall: Vol (McId) def";

    // TGeoVolume* vol = gGeoManager->GetVolume("SofTofWallWorld");
    // vol->SetVisibility(kFALSE);
}

// -----   Public method ProcessHits  --------------------------------------
Bool_t R3BSofTofWall::ProcessHits(FairVolume* vol)
{
    if (gMC->IsTrackEntering())
    {
        gGeoManager->cd(gMC->CurrentVolPath());
        Int_t nodeId = gGeoManager->GetNodeId();
    }
    if (gMC->IsTrackEntering())
    {
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

    if (dE > 0)
    {

        fNSteps++;

        // Set additional parameters at exit of active volume. Create R3BSofToFWPoint.
        if (gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared())
        {

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

            // Increment number of SofTofWallPoints for this track
            R3BStack* stack = (R3BStack*)gMC->GetStack();
            stack->AddPoint(kSOFTofWall);

            // Cal Data for each plastic
            // Adding a CalHit support
            Int_t nPlasticHits = fSofTofWallCalCollection->GetEntriesFast();
            Bool_t existHit = 0;

            if (nPlasticHits == 0)
                AddCalHit(vol->getCopyNo(), 0, fTime, fELoss);
            else
            {
                for (Int_t i = 0; i < nPlasticHits; i++)
                {
                    if (((R3BSofToFWCalData*)(fSofTofWallCalCollection->At(i)))->GetDetector() == vol->getCopyNo())
                    {
                        ((R3BSofToFWCalData*)(fSofTofWallCalCollection->At(i)))
                            ->SetEnergy(fELoss + ((R3BSofToFWCalData*)(fSofTofWallCalCollection->At(i)))->GetEnergy());
                        if (((R3BSofToFWCalData*)(fSofTofWallCalCollection->At(i)))->GetTime() > fTime &&
                            fZ_in > 6) // Above Z=6
                        {
                            ((R3BSofToFWCalData*)(fSofTofWallCalCollection->At(i)))->SetTime(fTime);
                        }
                        existHit = 1; // to avoid the creation of a new CalHit
                        break;
                    }
                }
                if (!existHit)
                    AddCalHit(vol->getCopyNo(), 0, fTime, fELoss);
            }

            existHit = 0;

            ResetParameters();
        }
    }
    return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofTofWall::BeginEvent() {}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofTofWall::EndOfEvent()
{
    if (fVerboseLevel)
        Print();

    fSofTofWallCollection->Clear();
    fSofTofWallCalCollection->Clear();

    ResetParameters();
}

// -----   Public method Register   -------------------------------------------
void R3BSofTofWall::Register()
{
    FairRootManager::Instance()->Register("SofTofWallPoint", GetName(), fSofTofWallCollection, kTRUE);
    FairRootManager::Instance()->Register("TofWCal", GetName(), fSofTofWallCalCollection, kTRUE);
}

// -----   Public method GetCollection   --------------------------------------
TClonesArray* R3BSofTofWall::GetCollection(Int_t iColl) const
{
    if (iColl == 0)
        return fSofTofWallCollection;
    else
        return NULL;
}

// -----   Public method Print   ----------------------------------------------
void R3BSofTofWall::Print(Option_t* option) const
{
    Int_t nHits = fSofTofWallCollection->GetEntriesFast();
    LOG(INFO) << "R3BSofTofWall: " << nHits << " points registered in this event";
    Int_t nCalHits = fSofTofWallCalCollection->GetEntriesFast();
    LOG(INFO) << "R3BSofTofWallCalData: " << nCalHits << " points registered in this event";
}

// -----   Public method Reset   ----------------------------------------------
void R3BSofTofWall::Reset()
{
    fSofTofWallCollection->Clear();
    fSofTofWallCalCollection->Clear();
    ResetParameters();
}

// -----   Public method CopyClones   -----------------------------------------
void R3BSofTofWall::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{
    Int_t nEntries = cl1->GetEntriesFast();
    LOG(INFO) << "R3BSofTofWall: " << nEntries << " entries to add";
    TClonesArray& clref = *cl2;
    R3BSofToFWPoint* oldpoint = NULL;
    for (Int_t i = 0; i < nEntries; i++)
    {
        oldpoint = (R3BSofToFWPoint*)cl1->At(i);
        Int_t index = oldpoint->GetTrackID() + offset;
        oldpoint->SetTrackID(index);
        new (clref[fPosIndex]) R3BSofToFWPoint(*oldpoint);
        fPosIndex++;
    }
    LOG(INFO) << "R3BSofTofWall: " << cl2->GetEntriesFast() << " merged entries";
}

// -----   Private method AddCalPoint   --------------------------------------------
R3BSofToFWCalData* R3BSofTofWall::AddCalHit(UShort_t ident, UShort_t pmt, Double_t time, Float_t eLoss)
{
    TClonesArray& clref = *fSofTofWallCalCollection;
    Int_t size = clref.GetEntriesFast();
    if (fVerboseLevel > 1)
        LOG(INFO) << "TofWCalData: Adding Point at detector " << ident << ", pmt " << pmt << ", time " << time << " ps"
                  << ", energy loss " << eLoss << " MeV";
    return new (clref[size]) R3BSofToFWCalData(ident, pmt, time, eLoss);
}

// -----   Private method AddPoint   --------------------------------------------
R3BSofToFWPoint* R3BSofTofWall::AddPoint(Int_t trackID,
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
                                         Double_t eLoss)
{
    TClonesArray& clref = *fSofTofWallCollection;
    Int_t size = clref.GetEntriesFast();
    if (fVerboseLevel > 1)
        LOG(INFO) << "R3BSofTofWall: Adding Point at (" << posIn.X() << ", " << posIn.Y() << ", " << posIn.Z()
                  << ") cm,  detector " << detID << ", track " << trackID << ", energy loss " << eLoss * 1e06 << " keV";
    return new (clref[size])
        R3BSofToFWPoint(trackID, detID, volid, Z, A, posIn, posOut, momIn, momOut, time, length, eLoss);
}

// -----  Public method CheckIfSensitive  ----------------------------------
Bool_t R3BSofTofWall::CheckIfSensitive(std::string name)
{
    if (TString(name).Contains("TOF_FFs"))
    {
        LOG(INFO) << "Found TOF SOFIA geometry from ROOT file: " << name;
        return kTRUE;
    }
    return kFALSE;
}

ClassImp(R3BSofTofWall)
