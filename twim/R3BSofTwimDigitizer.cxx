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

#include "TMath.h"
#include "TRandom.h"
#include "TVector3.h"
#include <iostream>
#include <string>

#include "R3BMCTrack.h"
#include "R3BSofTwimHitData.h"
#include "R3BSofTwimPoint.h"

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
    LOG(INFO) << "R3BSof" + fName + "Digitizer::Init()";

    // Get input array
    FairRootManager* ioman = FairRootManager::Instance();
    if (!ioman)
        LOG(fatal) << "Init: No FairRootManager";

    fMCTrack = (TClonesArray*)ioman->GetObject("MCTrack");
    fTwimPoints = (TClonesArray*)ioman->GetObject("Sof" + fName + "Point");

    // Register output array fTwimHits
    // fTwimHits = new TClonesArray("R3BHit", 10);
    fTwimHits = new TClonesArray("R3BSofTwimHitData", 10);
    ioman->Register(fName + "HitData", "Digital response in " + fName, fTwimHits, kTRUE);

    return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofTwimDigitizer::Exec(Option_t* opt)
{
    // The idea of this digitizer is to provide the charge of the fragments
    // from the sum of energy loss in the anodes.
    // At the moment, we take the Z from the point level and store it in the
    // R3BSofTwimHitData class. This should be changed in the future ...

    Reset();
    // Reading the Input -- Point Data --
    Int_t nHits = fTwimPoints->GetEntries();
    if (!nHits)
        return;
    // Data from Point level
    R3BSofTwimPoint** pointData;
    pointData = new R3BSofTwimPoint*[nHits];
    Int_t TrackId = 0, PID = 0, anodeId = 0;
    Double_t x[4], zf[4], y = 0., z = 0.;
    for (Int_t i = 0; i < 4; i++)
    {
        x[i] = 0.;
        zf[i] = 0.;
    }
    // Double_t eloss[64];
    // for (Int_t i = 0; i < 64; i++)
    //  eloss[i] = 0.;

    for (Int_t i = 0; i < nHits; i++)
    {
        pointData[i] = (R3BSofTwimPoint*)(fTwimPoints->At(i));
        TrackId = pointData[i]->GetTrackID();

        R3BMCTrack* Track = (R3BMCTrack*)fMCTrack->At(TrackId);
        PID = Track->GetPdgCode();
        anodeId = pointData[i]->GetDetCopyID();
        // eloss[anodeId] = eloss[anodeId] + pointData[i]->GetEnergyLoss() * 1000.;

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
                x[0] = (fX_out + fX_in) / 2.;
                zf[0] = pointData[i]->GetZFF();
            }
            else if (anodeId == 16)
            {
                x[1] = (fX_out + fX_in) / 2.;
                zf[1] = pointData[i]->GetZFF();
            }
            else if (anodeId == 32)
            {
                x[2] = (fX_out + fX_in) / 2.;
                zf[2] = pointData[i]->GetZFF();
            }
            else if (anodeId == 48)
            {
                x[3] = (fX_out + fX_in) / 2.;
                zf[3] = pointData[i]->GetZFF();
            }
        }
    }

    for (Int_t i = 0; i < 4; i++)
    {
        if (zf[i] > 1)
            AddR3BHitData(i + 1, x[i], zf[i]);
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
R3BSofTwimHitData* R3BSofTwimDigitizer::AddR3BHitData(Int_t secId, Double_t theta, Double_t z)
{
    // It fills the R3BHit
    TClonesArray& clref = *fTwimHits;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofTwimHitData(secId, theta, z);
}

ClassImp(R3BSofTwimDigitizer);
