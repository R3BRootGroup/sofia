// -------------------------------------------------------------------------
// -----                        R3BSofAT source file                  -----
// -----                  Created 24/11/17  by H.Alvarez-Pol          -----
// -------------------------------------------------------------------------

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairVolume.h"

#include "R3BLogger.h"
#include "R3BMCStack.h"
#include "R3BSofAT.h"
#include "R3BSofATPoint.h"

#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TParticle.h"
#include "TVirtualMC.h"

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
    R3BLOG(debug1, "");
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

    R3BLOG(info, " ");
    R3BLOG(debug, "Sens. Vol. (McId) " << TVirtualMC::GetMC()->VolId("SOFATLog"));
}

// -----   Public method ProcessHits  --------------------------------------
Bool_t R3BSofAT::ProcessHits(FairVolume* vol)
{
    Int_t nodeId = 0;
    if (TVirtualMC::GetMC()->IsTrackEntering())
    {
        gGeoManager->cd(TVirtualMC::GetMC()->CurrentVolPath());
        nodeId = gGeoManager->GetNodeId();
    }
    if (TVirtualMC::GetMC()->IsTrackEntering())
    {
        fELoss = 0.;
        fNf = 0.;
        fNs = 0.;
        fNSteps = 0; // FIXME
        fTime = TVirtualMC::GetMC()->TrackTime() * 1.0e09;
        fLength = TVirtualMC::GetMC()->TrackLength();
        TVirtualMC::GetMC()->TrackPosition(fPosIn);
        TVirtualMC::GetMC()->TrackMomentum(fMomIn);
        fEinc = TVirtualMC::GetMC()->Etot() - TVirtualMC::GetMC()->TrackMass(); // be aware!! Relativistic mass!
    }

    // Sum energy loss for all steps in the active volume
    Double_t dE = TVirtualMC::GetMC()->Edep() * 1000.;                                          // in MeV
    Double_t post_E = (TVirtualMC::GetMC()->Etot() - TVirtualMC::GetMC()->TrackMass()) * 1000.; // in MeV
    TString ptype = TVirtualMC::GetMC()->GetStack()->GetCurrentTrack()->GetName();

    Double_t M_in = TVirtualMC::GetMC()->TrackMass() * 1000.;
    Double_t A_in = M_in / U_MEV;
    Double_t Z_in = TVirtualMC::GetMC()->TrackCharge();

    fELoss += dE / 1000.; // back to GeV

    if (dE > 0 /*&& dx > 0*/)
    {

        fNSteps++;

        // Set additional parameters at exit of active volume. Create R3BSofATPoint.
        if (TVirtualMC::GetMC()->IsTrackExiting() || TVirtualMC::GetMC()->IsTrackStop() ||
            TVirtualMC::GetMC()->IsTrackDisappeared())
        {

            fTrackID = TVirtualMC::GetMC()->GetStack()->GetCurrentTrackNumber();
            fParentTrackID = TVirtualMC::GetMC()->GetStack()->GetCurrentParentTrackNumber();
            fVolumeID = vol->getMCid();
            fTrackPID = TVirtualMC::GetMC()->TrackPid();
            fUniqueID = TVirtualMC::GetMC()->GetStack()->GetCurrentTrack()->GetUniqueID();

            TVirtualMC::GetMC()->TrackPosition(fPosOut);
            TVirtualMC::GetMC()->TrackMomentum(fMomOut);

            if (fELoss == 0.)
            {
                return kFALSE;
            }

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
            R3BStack* stack = static_cast<R3BStack*>(TVirtualMC::GetMC()->GetStack());
            stack->AddPoint(kSOFAT);

            ResetParameters();
        }
    }
    return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BSofAT::EndOfEvent()
{
    if (fVerboseLevel)
    {
        Print();
    }
    Reset();
}

// -----   Public method Register   -------------------------------------------
void R3BSofAT::Register() { FairRootManager::Instance()->Register("SofATPoint", GetName(), fSofATCollection, kTRUE); }

// -----   Public method GetCollection   --------------------------------------
TClonesArray* R3BSofAT::GetCollection(Int_t iColl) const
{
    if (iColl == 0)
    {
        return fSofATCollection;
    }
    else
    {
        return nullptr;
    }
}

// -----   Public method Print   ----------------------------------------------
void R3BSofAT::Print(Option_t* option) const
{
    Int_t nHits = fSofATCollection->GetEntriesFast();
    R3BLOG(info, nHits << " points registered in this event");
}

// -----   Public method Reset   ----------------------------------------------
void R3BSofAT::Reset()
{
    fSofATCollection->Clear();
    ResetParameters();
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

ClassImp(R3BSofAT);
