// -------------------------------------------------------------------------
// -----                        R3BSofMWPC2 source file                 -----
// -----                  Created 06/12/17  by JL Rodriguez            -----
// -------------------------------------------------------------------------
#include "R3BSofMWPC2.h"
#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoNode.h"
#include "FairGeoRootBuilder.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairVolume.h"
#include "R3BMCStack.h"
#include "R3BSofMWPCPoint2.h"
#include "TClonesArray.h"
#include "TGeoArb8.h"
#include "TGeoBBox.h"
#include "TGeoBoolNode.h"
#include "TGeoCompositeShape.h"
#include "TGeoCone.h"
#include "TGeoMCGeometry.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMatrix.h"
#include "TGeoMedium.h"
#include "TGeoPara.h"
#include "TGeoPgon.h"
#include "TGeoShapeAssembly.h"
#include "TGeoSphere.h"
#include "TGeoTube.h"
#include "TObjArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include <stdlib.h>

R3BSofMWPC2::R3BSofMWPC2()
    : R3BSofMWPC2("")
{
}

R3BSofMWPC2::R3BSofMWPC2(const TString& geoFile, const TGeoTranslation& trans, const TGeoRotation& rot)
    : R3BSofMWPC2(geoFile, { trans, rot })
{
}

R3BSofMWPC2::R3BSofMWPC2(const TString& geoFile, const TGeoCombiTrans& combi)
    : R3BDetector("R3BSofMWPC2", kSOFMWPC2, geoFile, combi)
    , fSofMWPCCollection(new TClonesArray("R3BSofMWPCPoint2"))
    , fPosIndex(0)
    , kGeoSaved(kFALSE)
    , flGeoPar(new TList())
{
    flGeoPar->SetName(GetName());
    ResetParameters();
}

R3BSofMWPC2::~R3BSofMWPC2()
{
    if (flGeoPar)
    {
        delete flGeoPar;
    }
    if (fSofMWPCCollection)
    {
        fSofMWPCCollection->Delete();
        delete fSofMWPCCollection;
    }
}

void R3BSofMWPC2::Initialize()
{
    FairDetector::Initialize();

    LOG(INFO) << "R3BSofMWPC2: initialisation";
    LOG(DEBUG) << "R3BSofMWPC2: Sens. Vol. (McId) " << gMC->VolId("MWPC21");
}

void R3BSofMWPC2::SetSpecialPhysicsCuts()
{
    LOG(INFO) << "-I- R3BSofMWPC2: Adding customized Physics cut ... ";
}

// -----   Public method ProcessHits  --------------------------------------
Bool_t R3BSofMWPC2::ProcessHits(FairVolume* vol)
{
    if (gMC->IsTrackEntering())
    {
        fELoss = 0.;
        fTime = gMC->TrackTime() * 1.0e09;
        fLength = gMC->TrackLength();
        gMC->TrackPosition(fPosIn);
        gMC->TrackMomentum(fMomIn);
    }

    // Sum energy loss for all steps in the active volume
    fELoss += gMC->Edep();

    // Set additional parameters at exit of active volume. Create R3BSofMWPC2Point.
    if (gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared())
    {
        fTrackID = gMC->GetStack()->GetCurrentTrackNumber();
        fVolumeID = vol->getMCid();
        fDetCopyID = 2;//vol->getCopyNo();
        gMC->TrackPosition(fPosOut);
        gMC->TrackMomentum(fMomOut);
        //        if (fELoss == 0.)
        //            return kFALSE;

        if (gMC->IsTrackExiting())
        {
            const Double_t* oldpos;
            const Double_t* olddirection;
            Double_t newpos[3];
            Double_t newdirection[3];
            Double_t safety;

            gGeoManager->FindNode(fPosOut.X(), fPosOut.Y(), fPosOut.Z());
            oldpos = gGeoManager->GetCurrentPoint();
            olddirection = gGeoManager->GetCurrentDirection();

            for (Int_t i = 0; i < 3; i++)
            {
                newdirection[i] = -1 * olddirection[i];
            }

            gGeoManager->SetCurrentDirection(newdirection);
            //   TGeoNode *bla = gGeoManager->FindNextBoundary(2);
            safety = gGeoManager->GetSafeDistance();

            gGeoManager->SetCurrentDirection(-newdirection[0], -newdirection[1], -newdirection[2]);

            for (Int_t i = 0; i < 3; i++)
            {
                newpos[i] = oldpos[i] - (3 * safety * olddirection[i]);
            }

            fPosOut.SetX(newpos[0]);
            fPosOut.SetY(newpos[1]);
            fPosOut.SetZ(newpos[2]);
        }

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

        // Increment number of TraPoints for this track
        R3BStack* stack = (R3BStack*)gMC->GetStack();
        stack->AddPoint(kSOFMWPC2);

        ResetParameters();
    }

    return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofMWPC2::BeginEvent() {}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofMWPC2::EndOfEvent()
{
    if (fVerboseLevel)
        Print();
    fSofMWPCCollection->Clear();

    ResetParameters();
}
// ----------------------------------------------------------------------------

// -----   Public method Register   -------------------------------------------
void R3BSofMWPC2::Register() { FairRootManager::Instance()->Register("SofMWPC2Point", 
                                           GetName(), fSofMWPCCollection, kTRUE); }
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* R3BSofMWPC2::GetCollection(Int_t iColl) const
{
    if (iColl == 0)
        return fSofMWPCCollection;
    else
        return NULL;
}
// ----------------------------------------------------------------------------

// -----   Public method Print   ----------------------------------------------
void R3BSofMWPC2::Print(Option_t* option) const
{
    Int_t nHits = fSofMWPCCollection->GetEntriesFast();
    LOG(INFO) << "R3BSofMWPC2: " << nHits << " points registered in this event";
}
// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void R3BSofMWPC2::Reset()
{
    fSofMWPCCollection->Clear();
    ResetParameters();
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void R3BSofMWPC2::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{
    Int_t nEntries = cl1->GetEntriesFast();
    LOG(INFO) << "R3BSofMWPC2: " << nEntries << " entries to add";
    TClonesArray& clref = *cl2;
    R3BSofMWPCPoint2* oldpoint = NULL;
    for (Int_t i = 0; i < nEntries; i++)
    {
        oldpoint = (R3BSofMWPCPoint2*)cl1->At(i);
        Int_t index = oldpoint->GetTrackID() + offset;
        oldpoint->SetTrackID(index);
        new (clref[fPosIndex]) R3BSofMWPCPoint2(*oldpoint);
        fPosIndex++;
    }
    LOG(INFO) << "R3BSofMWPC2: " << cl2->GetEntriesFast() << " merged entries";
}

// -----   Private method AddPoint   --------------------------------------------
R3BSofMWPCPoint2* R3BSofMWPC2::AddPoint(Int_t trackID,
                            Int_t detID,
                            Int_t detCopyID,
                            TVector3 posIn,
                            TVector3 posOut,
                            TVector3 momIn,
                            TVector3 momOut,
                            Double_t time,
                            Double_t length,
                            Double_t eLoss)
{
    TClonesArray& clref = *fSofMWPCCollection;
    Int_t size = clref.GetEntriesFast();
    if (fVerboseLevel > 1)
        LOG(INFO) << "R3BSofMWPC2: Adding Point at (" << posIn.X() << ", " << posIn.Y() << ", " << posIn.Z()
                  << ") cm,  detector " << detID << ", track " << trackID << ", energy loss " << eLoss * 1e06 << " keV";
    return new (clref[size]) R3BSofMWPCPoint2(trackID,
                                         detID,
                                         detCopyID,
                                         posIn,
                                         posOut,
                                         momIn,
                                         momOut,
                                         time,
                                         length,
                                         eLoss);
}

Bool_t R3BSofMWPC2::CheckIfSensitive(std::string name)
{
  if (TString(name).Contains("MWPC21")) {
    LOG(INFO) << "Found MWPC2 geometry from ROOT file: " << name;
    return kTRUE;
  }
    return kFALSE;
}

ClassImp(R3BSofMWPC2)
