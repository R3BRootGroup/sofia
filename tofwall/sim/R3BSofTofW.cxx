// ------------------------------------------------------------------------
// -----                        R3BSofTofW source file                -----
// -----                  Created 24/11/17 by H.Alvarez-Pol           -----
// ------------------------------------------------------------------------

#include "R3BSofTofW.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairVolume.h"
#include "R3BLogger.h"
#include "R3BMCStack.h"
#include "R3BSofTofWPoint.h"
#include "TGeoManager.h"
#include "TVirtualMC.h"
#include "TVirtualMCStack.h"

#include <TClonesArray.h>

R3BSofTofW::R3BSofTofW()
    : R3BSofTofW("")
{
}

R3BSofTofW::R3BSofTofW(const TString& geoFile, const TGeoTranslation& trans, const TGeoRotation& rot)
    : R3BSofTofW(geoFile, { trans, rot })
{
}

R3BSofTofW::R3BSofTofW(const TString& geoFile, const TGeoCombiTrans& combi)
    : R3BDetector("R3BSofTofW", kSOFTofWall, geoFile, combi)
    , fSofTofWallCollection(new TClonesArray("R3BSofTofWPoint"))
    , fPosIndex(0)
{
    ResetParameters();
}

R3BSofTofW::~R3BSofTofW()
{
    if (fSofTofWallCollection)
    {
        fSofTofWallCollection->Delete();
        delete fSofTofWallCollection;
    }
}

void R3BSofTofW::Initialize()
{
    FairDetector::Initialize();
    R3BLOG(info, "");
}

// -----   Public method ProcessHits  --------------------------------------
Bool_t R3BSofTofW::ProcessHits(FairVolume* vol)
{
    /** This method is called from the MC stepping */

    // Set parameters at entrance of volume. Reset ELoss.
    if (TVirtualMC::GetMC()->IsTrackEntering())
    {
        fELoss = 0.;
        fTime = TVirtualMC::GetMC()->TrackTime() * 1.0e09;
        fLength = TVirtualMC::GetMC()->TrackLength();
        TVirtualMC::GetMC()->TrackPosition(fPosIn);
        TVirtualMC::GetMC()->TrackMomentum(fMomIn);
    }

    Double_t M_in = TVirtualMC::GetMC()->TrackMass() * 1000.;
    // Charge and mass are now obtained from PDG Code
    Double_t fZ_in = int(TVirtualMC::GetMC()->TrackPid() / 10000) - 100000.;
    Double_t fA_in = 0.1 * (TVirtualMC::GetMC()->TrackPid() - (100000 + fZ_in) * 10000.);

    fELoss += TVirtualMC::GetMC()->Edep();
    if (fELoss > 0)
    {
        // Set additional parameters at exit of active volume. Create R3BSofTofWPoint.
        if (TVirtualMC::GetMC()->IsTrackExiting() || TVirtualMC::GetMC()->IsTrackStop() ||
            TVirtualMC::GetMC()->IsTrackDisappeared())
        {

            fTrackID = TVirtualMC::GetMC()->GetStack()->GetCurrentTrackNumber();
            fVolumeID = vol->getMCid();
            fDetCopyID = vol->getCopyNo();
            fTrackPID = TVirtualMC::GetMC()->TrackPid();

            gMC->TrackPosition(fPosOut);
            gMC->TrackMomentum(fMomOut);

            if (fELoss == 0.)
            {
                return kFALSE;
            }

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
            R3BStack* stack = static_cast<R3BStack*>(TVirtualMC::GetMC()->GetStack());
            stack->AddPoint(kSOFTofWall);
            ResetParameters();
        }
    }
    return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofTofW::EndOfEvent()
{
    if (fVerboseLevel)
    {
        Print();
    }
    Reset();
}

// -----   Public method Register   -------------------------------------------
void R3BSofTofW::Register()
{
    FairRootManager::Instance()->Register("SofTofWPoint", GetName(), fSofTofWallCollection, kTRUE);
}

// -----   Public method GetCollection   --------------------------------------
TClonesArray* R3BSofTofW::GetCollection(Int_t iColl) const
{
    if (iColl == 0)
    {
        return fSofTofWallCollection;
    }
    else
    {
        return nullptr;
    }
}

// -----   Public method Print   ----------------------------------------------
void R3BSofTofW::Print(Option_t* option) const
{
    Int_t nHits = fSofTofWallCollection->GetEntriesFast();
    R3BLOG(info, nHits << " points registered in this event");
}

// -----   Public method Reset   ----------------------------------------------
void R3BSofTofW::Reset()
{
    fSofTofWallCollection->Clear();
    ResetParameters();
}

// -----   Private method AddPoint   --------------------------------------------
R3BSofTofWPoint* R3BSofTofW::AddPoint(Int_t trackID,
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
    {
                 R3BLOG(info,
               "Adding Point at (" << posIn.X() << ", " << posIn.Y() << ", " << posIn.Z() << ") cm,  detector " << detID
                                   << ", track " << trackID << ", energy loss " << eLoss * 1e06 << " keV");
    }
    return new (clref[size])
        R3BSofTofWPoint(trackID, detID, volid, Z, A, posIn, posOut, momIn, momOut, time, length, eLoss);
}

// -----  Public method CheckIfSensitive  ----------------------------------
Bool_t R3BSofTofW::CheckIfSensitive(std::string name)
{
    if (TString(name).Contains("TOF_FFs"))
    {
        // LOG(debug) << "Found TOF SOFIA geometry from ROOT file: " << name;
        return kTRUE;
    }
    return kFALSE;
}

ClassImp(R3BSofTofW);
