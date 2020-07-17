// ----------------------------------------------------------------
// -----          R3BSofMwpcDigitizer source file             -----
// -----         Created 03/11/19  by JL Rodriguez            -----
// ----------------------------------------------------------------

#include "R3BSofMwpcDigitizer.h"
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
#include "R3BSofMWPCPoint.h"

// R3BSofMwpcDigitizer: Default Constructor --------------------------
R3BSofMwpcDigitizer::R3BSofMwpcDigitizer()
    : FairTask("R3BSof MWPC Digitization scheme", 1)
    , fName("Mwpc")
    , fMCTrack(NULL)
    , fMwpcPoints(NULL)
    , fMwpcHits(NULL)
    , fsigma_x(0.025) // sigma=0.25mm
    , fsigma_y(0.05)  // sigma=0.5mm
    , fPosX(0.)
    , fPosZ(0.)
    , fangle(0.)
{
}

// R3BSofMwpcDigitizer: Standard Constructor --------------------------
R3BSofMwpcDigitizer::R3BSofMwpcDigitizer(const TString& name, Int_t iVerbose)
    : FairTask(name + "Digi", iVerbose)
    , fName(name)
    , fMCTrack(NULL)
    , fMwpcPoints(NULL)
    , fMwpcHits(NULL)
    , fsigma_x(0.025)
    , fsigma_y(0.05)
    , fPosX(0.)
    , fPosZ(0.)
    , fangle(0.)
{
}

// Virtual R3BSofMwpcDigitizer: Destructor ----------------------------
R3BSofMwpcDigitizer::~R3BSofMwpcDigitizer()
{
    LOG(INFO) << "R3BSof" + fName + "Digitizer: Delete instance";
    if (fMwpcPoints)
        delete fMwpcPoints;
    if (fMwpcHits)
        delete fMwpcHits;
}

// ----   Public method Init  -----------------------------------------
InitStatus R3BSofMwpcDigitizer::Init()
{
    LOG(INFO) << "R3BSof" + fName + "Digitizer: Init";

    // Get input array
    FairRootManager* ioman = FairRootManager::Instance();
    if (!ioman)
        LOG(fatal) << "Init: No FairRootManager";

    fMCTrack = (TClonesArray*)ioman->GetObject("MCTrack");
    fMwpcPoints = (TClonesArray*)ioman->GetObject("Sof" + fName + "Point");

    // Register output array fMwpcHits
    fMwpcHits = new TClonesArray("R3BSofMwpcHitData", 10);
    ioman->Register(fName + "Hit", "Digital response in " + fName, fMwpcHits, kTRUE);

    return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofMwpcDigitizer::Exec(Option_t* opt)
{
    Reset();
    // Reading the Input -- Point Data --
    Int_t nHits = fMwpcPoints->GetEntries();
    if (!nHits)
        return;
    // Data from Point level
    R3BSofMWPCPoint** pointData;
    pointData = new R3BSofMWPCPoint*[nHits];
    Int_t TrackId = 0, PID = 0;
    Double_t x = 0., y = 0., z = 0.;
    for (Int_t i = 0; i < nHits; i++)
    {
        pointData[i] = (R3BSofMWPCPoint*)(fMwpcPoints->At(i));
        TrackId = pointData[i]->GetTrackID();

        R3BMCTrack* Track = (R3BMCTrack*)fMCTrack->At(TrackId);
        PID = Track->GetPdgCode();

        if (PID > 1000080160) // Z=8 and A=16
        {
            Double_t fX_in = pointData[i]->GetXIn();
            Double_t fY_in = pointData[i]->GetYIn();
            Double_t fZ_in = pointData[i]->GetZIn();
            Double_t fX_out = pointData[i]->GetXOut();
            Double_t fY_out = pointData[i]->GetYOut();
            Double_t fZ_out = pointData[i]->GetZOut();

            x = ((fX_in + fX_out) / 2.);
            y = ((fY_in + fY_out) / 2.) + gRandom->Gaus(0., fsigma_y);
            z = ((fZ_in + fZ_out) / 2.);

            x = (((x - fPosX) * cos(fangle * TMath::DegToRad())) - ((z - fPosZ) * sin(fangle * TMath::DegToRad()))) +
                gRandom->Gaus(0., fsigma_x);

            AddHitData(x, y);
        }
    }
    if (pointData)
        delete pointData;
    return;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofMwpcDigitizer::ReInit() { return kSUCCESS; }

// -----   Public method Reset   -----------------------------------------------
void R3BSofMwpcDigitizer::Reset()
{
    LOG(DEBUG) << "Clearing R3BSof" + fName + "Digitizer Structure";
    if (fMwpcHits)
        fMwpcHits->Clear();
}

// -----   Private method AddHitData  -------------------------------------------
R3BSofMwpcHitData* R3BSofMwpcDigitizer::AddHitData(Double_t x, Double_t y)
{
    // It fills the R3BSofMwpcHitData
    TClonesArray& clref = *fMwpcHits;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofMwpcHitData(x, y);
}

// -----   Public method Finish  ------------------------------------------------
void R3BSofMwpcDigitizer::Finish() {}

ClassImp(R3BSofMwpcDigitizer)
