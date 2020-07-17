// ----------------------------------------------------------------
// -----          R3BSofTwimDigitizer source file             -----
// -----         Created 06/11/19  by JL Rodriguez            -----
// ----------------------------------------------------------------

#include "R3BSofTwimDigitizer.h"
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "TClonesArray.h"

// includes for modeling
#include "TGeoManager.h"
#include "TParticle.h"
#include "TVirtualMC.h"

#include "TMath.h"
#include "TRandom.h"
#include "TVector3.h"
#include <iostream>
#include <string>

#include "R3BMCTrack.h"
#include "R3BSofTWIMPoint.h"

// R3BSofTwimDigitizer: Default Constructor --------------------------
R3BSofTwimDigitizer::R3BSofTwimDigitizer()
    : FairTask("R3BSof Twim Digitization scheme", 1)
    , fName("Twim")
    , fDetId(0)
    , fMCTrack(NULL)
    , fTwimPoints(NULL)
    , fTwimHits(NULL)
    , fsigma_x(0.003) // sigma=0.03mm
    , fPosX(0.)
    , fPosZ(0.)
    , fangle(0.)
{
}

// R3BSofTwimDigitizer: Standard Constructor --------------------------
R3BSofTwimDigitizer::R3BSofTwimDigitizer(const TString& name, Int_t iVerbose)
    : FairTask(name + "Digi", iVerbose)
    , fName(name)
    , fDetId(0)
    , fMCTrack(NULL)
    , fTwimPoints(NULL)
    , fTwimHits(NULL)
    , fsigma_x(0.003)
    , fPosX(0.)
    , fPosZ(0.)
    , fangle(0.)
{
}

// Virtual R3BSofTwimDigitizer: Destructor ----------------------------
R3BSofTwimDigitizer::~R3BSofTwimDigitizer()
{
    LOG(INFO) << "R3BSof" + fName + "Digitizer: Delete instance";
    if (fTwimPoints)
        delete fTwimPoints;
    if (fTwimHits)
        delete fTwimHits;
}

// ----   Public method Init  -----------------------------------------
InitStatus R3BSofTwimDigitizer::Init()
{
    LOG(INFO) << "R3BSof" + fName + "Digitizer: Init";

    // Get input array
    FairRootManager* ioman = FairRootManager::Instance();
    if (!ioman)
        LOG(fatal) << "Init: No FairRootManager";

    fMCTrack = (TClonesArray*)ioman->GetObject("MCTrack");
    fTwimPoints = (TClonesArray*)ioman->GetObject("Sof" + fName + "Point");

    // Register output array fTwimHits
    fTwimHits = new TClonesArray("R3BHit", 10);
    ioman->Register(fName + "Hit", "Digital response in " + fName, fTwimHits, kTRUE);

    fDetId = 3;

    return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofTwimDigitizer::Exec(Option_t* opt)
{
    // The idea of this digitizer is to provide the charge of the fragments
    // from the sum of energy loss in the anodes.
    // At the moment, we take the Z from the point level and store it in the
    // R3BHit class as the time data member. This should be changed in the
    // future ...

    Reset();
    // Reading the Input -- Point Data --
    Int_t nHits = fTwimPoints->GetEntries();
    if (!nHits)
        return;
    // Data from Point level
    R3BSofTWIMPoint** pointData;
    pointData = new R3BSofTWIMPoint*[nHits];
    Int_t TrackId = 0, PID = 0, anodeId = 0;
    Double_t x[4], zf[4], y = 0., z = 0.;
    for (Int_t i = 0; i < 4; i++)
    {
        x[i] = 0.;
        zf[i] = 0.;
    }
    Double_t eloss[64];
    for (Int_t i = 0; i < 64; i++)
        eloss[i] = 0.;

    for (Int_t i = 0; i < nHits; i++)
    {
        pointData[i] = (R3BSofTWIMPoint*)(fTwimPoints->At(i));
        TrackId = pointData[i]->GetTrackID();

        R3BMCTrack* Track = (R3BMCTrack*)fMCTrack->At(TrackId);
        PID = Track->GetPdgCode();
        anodeId = pointData[i]->GetDetCopyID();
        eloss[anodeId] = eloss[anodeId] + pointData[i]->GetEnergyLoss() * 1000.;

        if (PID > 1000080160 && (anodeId == 0 || anodeId == 16 || anodeId == 32 || anodeId == 48)) // Z=8 and A=16
        {

            Double_t fX_in = pointData[i]->GetXIn();
            // Double_t fY_in = pointData[i]->GetYIn();
            // Double_t fZ_in = pointData[i]->GetZIn();
            Double_t fX_out = pointData[i]->GetXOut();
            // Double_t fY_out = pointData[i]->GetYOut();
            // Double_t fZ_out = pointData[i]->GetZOut();

            if (anodeId == 0)
            {
                x[0] = ((fX_in + fX_out) / 2.) + gRandom->Gaus(0., fsigma_x);
                zf[0] = pointData[i]->GetZFF();
            }
            else if (anodeId == 16)
            {
                x[1] = ((fX_in + fX_out) / 2.) + gRandom->Gaus(0., fsigma_x);
                zf[1] = pointData[i]->GetZFF();
            }
            else if (anodeId == 32)
            {
                x[2] = ((fX_in + fX_out) / 2.) + gRandom->Gaus(0., fsigma_x);
                zf[2] = pointData[i]->GetZFF();
            }
            else if (anodeId == 48)
            {
                x[3] = ((fX_in + fX_out) / 2.) + gRandom->Gaus(0., fsigma_x);
                zf[3] = pointData[i]->GetZFF();
            }

            // z = ((fZ_in + fZ_out) / 2.);

            // x = (((x - fPosX) * cos(fangle * TMath::DegToRad())) - ((z - fPosZ) * sin(fangle * TMath::DegToRad()))) +
            //  gRandom->Gaus(0., fsigma_x);
        }
    }

    for (Int_t i = 0; i < 4; i++)
    {
        double el = 0.;
        for (Int_t j = 16 * i; j < 16 * (1 + i); j++)
            if (eloss[j] > 0.001)
                el = el + eloss[j];
        AddR3BHitData(fDetId, x[i], 0., el, zf[i]);
    }

    if (pointData)
        delete pointData;
    return;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofTwimDigitizer::ReInit() { return kSUCCESS; }

// -----   Public method Reset   -----------------------------------------------
void R3BSofTwimDigitizer::Reset()
{
    LOG(DEBUG) << "Clearing R3BSof" + fName + "Digitizer Structure";
    if (fTwimHits)
        fTwimHits->Clear();
}

// -----   Private method AddR3BHitData  -------------------------------------------
R3BHit* R3BSofTwimDigitizer::AddR3BHitData(Int_t detId, Double_t x, Double_t y, Double_t eloss, Double_t time)
{
    // It fills the R3BHit
    TClonesArray& clref = *fTwimHits;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BHit(detId, x, y, eloss, time);
}

// -----   Public method Finish  ------------------------------------------------
void R3BSofTwimDigitizer::Finish() {}

ClassImp(R3BSofTwimDigitizer)
