// -----------------------------------------------------------------
// -----         R3BSofTofWTCal2Hit source file                -----
// -----    Created 15/02/20  by J.L. Rodriguez-Sanchez        -----
// -----------------------------------------------------------------
#include "R3BSofTofWTCal2Hit.h"

// R3BSofTofWTCal2Hit: Default Constructor --------------------------
R3BSofTofWTCal2Hit::R3BSofTofWTCal2Hit()
    : FairTask("R3BSof TofW tcal2hit Task", 1)
    , fTCalDataCA(NULL)
    , fHitDataCA(NULL)
    , fOnline(kFALSE)
    , Tof_lise(43.)
    , TofWPosition(560.)
{
}

// R3BSofTofWTCal2Hit: Standard Constructor --------------------------
R3BSofTofWTCal2Hit::R3BSofTofWTCal2Hit(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fTCalDataCA(NULL)
    , fHitDataCA(NULL)
    , fOnline(kFALSE)
    , Tof_lise(43.)
    , TofWPosition(560.)
{
}

// Virtual R3BSofTofWTCal2Hit: Destructor
R3BSofTofWTCal2Hit::~R3BSofTofWTCal2Hit()
{
    LOG(INFO) << "R3BSofTofWTCal2Hit: Delete instance";
    if (fTCalDataCA)
        delete fTCalDataCA;
    if (fHitDataCA)
        delete fHitDataCA;
}

void R3BSofTofWTCal2Hit::SetParContainers()
{
    fTofWHitPar = (R3BSofTofWHitPar*)FairRuntimeDb::instance()->getContainer("tofwHitPar");
    if (!fTofWHitPar)
    {
        LOG(ERROR) << "R3BSofTofWTCal2Hit::SetParContainers() : Could not get access to tofwHitPar-Container.";
        return;
    }
    else
        LOG(INFO) << "R3BSofTofWTCal2Hit::SetParContainers() : tofwHitPar-Container found with "
                  << fTofWHitPar->GetNumSci() << " paddles.";
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofTofWTCal2Hit::Init()
{
    LOG(INFO) << "R3BSofTofWTCal2Hit: Init";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    fTCalDataCA = (TClonesArray*)rootManager->GetObject("SofTofWSingleTcalData");
    if (!fTCalDataCA)
    {
        return kFATAL;
    }

    // OUTPUT DATA
    // Hit data
    fHitDataCA = new TClonesArray("R3BSofTofWHitData", 10);

    if (!fOnline)
    {
        rootManager->Register("TofWHitData", "TofW-Hit", fHitDataCA, kTRUE);
    }
    else
    {
        rootManager->Register("TofWHitData", "TofW-Hit", fHitDataCA, kFALSE);
    }
    // fTofWHitPar->printParams();
    return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofTofWTCal2Hit::ReInit() { return kSUCCESS; }

// -----   Public method Execution   --------------------------------------------
void R3BSofTofWTCal2Hit::Exec(Option_t* option)
{
    // Reset entries in output arrays, local arrays
    Reset();

    // Reading the Input -- Cal Data --
    Int_t nHits = fTCalDataCA->GetEntries();
    if (nHits < 1)
        return;

    // Data from cal level
    calDat = new R3BSofTofWSingleTcalData*[nHits];
    Int_t fPaddleId = 0; // from 1 to 28
    Double_t tofw = 0., posx = 0., posy = 0., vel = 0.;
    Int_t mult = 0;

    for (Int_t i = 0; i < nHits; i++)
    {
        calDat[i] = (R3BSofTofWSingleTcalData*)(fTCalDataCA->At(i));
        fPaddleId = calDat[i]->GetDetector();
        if (fTofWHitPar->GetInUse(fPaddleId) != 1)
            continue;
        mult++;
        tofw = calDat[i]->GetRawTofNs();
        posy = calDat[i]->GetRawPosNs();
    }

    if (mult == 1)
    {
        posx = (TofWPosition - 525.) - 14. * 30. + (Double_t)(fPaddleId - 1) * 30.; // x=0 at the gap of bars 14 and 15
        posy = posy - fTofWHitPar->GetPosPar(fPaddleId);
        tofw = tofw - fTofWHitPar->GetTofPar(fPaddleId) - fTofWHitPar->GetVelPar0(fPaddleId) + Tof_lise;
        // TofPar is adjusted to align the time 0 with motor sweep runs,
        // Tof_lise + VelPar0 is to adjust the velocity correlation with FRS with a certain setting,
        // And the Tof_lise is to adjust the difference of the flight path from sofsci to target setting-by-setting.
        vel = fTofWHitPar->GetVelPar1(fPaddleId) / tofw;
        AddHitData(fPaddleId, posx, posy, tofw, 0., vel);
    }
    if (calDat)
        delete calDat;

    return;
}

// -----   Public method Finish  ------------------------------------------------
void R3BSofTofWTCal2Hit::Finish() {}

void R3BSofTofWTCal2Hit::FinishEvent() {}

// -----   Public method Reset   ------------------------------------------------
void R3BSofTofWTCal2Hit::Reset()
{
    LOG(DEBUG) << "Clearing TofWHitData structure";
    if (fHitDataCA)
        fHitDataCA->Clear();
}

// -----   Private method AddHitData  --------------------------------------------
R3BSofTofWHitData* R3BSofTofWTCal2Hit::AddHitData(Int_t paddle,
                                                  Double_t x,
                                                  Double_t y,
                                                  Double_t tof,
                                                  Double_t brho,
                                                  Double_t vel)
{
    // It fills the R3BSofTofWHitData
    TClonesArray& clref = *fHitDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofTofWHitData(paddle, x, y, tof, brho, vel);
}

ClassImp(R3BSofTofWTCal2Hit)
