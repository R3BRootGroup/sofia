// ---------------------------------------------------------------------
// -----         R3BSofTofWTcal2HitPar source file               -----
// -----      Created 29/01/20  by J.L. Rodriguez-Sanchez          -----
// ---------------------------------------------------------------------

// ROOT headers
#include "TClonesArray.h"
#include "TF1.h"
#include "TGraph.h"
#include "TMath.h"
#include "TRandom.h"
#include "TVector3.h"

// Fair headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

// TofW headers
#include "R3BSofTofWHitPar.h"
#include "R3BSofTofWSingleTcalData.h"
#include "R3BSofTofWTcal2HitPar.h"

#include <iomanip>

// R3BSofTofWTcal2HitPar: Default Constructor --------------------------
R3BSofTofWTcal2HitPar::R3BSofTofWTcal2HitPar()
    : FairTask("R3BSof TofW Calibrator", 1)
    , fNumSci(28)
    , fMinStatistics(1000)
    , fLimit_left(0)
    , fLimit_right(24000)
    , fMaxSigma(200)
    , TofParams(NULL)
    , PosParams(NULL)
    , fHit_Par(NULL)
    , fTofCalDataCA(NULL)
{
}

// R3BSofTofWTcal2HitParPar: Standard Constructor --------------------------
R3BSofTofWTcal2HitPar::R3BSofTofWTcal2HitPar(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNumSci(28)
    , fMinStatistics(1000)
    , fLimit_left(0)
    , fLimit_right(24000)
    , fMaxSigma(200)
    , TofParams(NULL)
    , PosParams(NULL)
    , fHit_Par(NULL)
    , fTofCalDataCA(NULL)
{
}

// Virtual R3BSofTofWTcal2HitPar: Destructor
R3BSofTofWTcal2HitPar::~R3BSofTofWTcal2HitPar()
{
    LOG(INFO) << "R3BSofTofWTcal2HitPar: Delete instance";
    if (fTofCalDataCA)
        delete fTofCalDataCA;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofTofWTcal2HitPar::Init()
{
    LOG(INFO) << "R3BSofTofWTcal2HitPar: Init";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    fTofCalDataCA = (TClonesArray*)rootManager->GetObject("SofTofWSingleTcalData");
    if (!fTofCalDataCA)
    {
        LOG(ERROR) << "R3BSofTofWTcal2HitPar: SofTofWSingleTcalData not found";
        return kFATAL;
    }

    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        return kFATAL;
    }

    fHit_Par = (R3BSofTofWHitPar*)rtdb->getContainer("tofwHitPar");
    if (!fHit_Par)
    {
        LOG(ERROR) << "R3BSofTofWTcal2HitPar:: Couldn't get handle on tofwHitPar container";
        return kFATAL;
    }

    // Define histograms
    char Name1[255];
    for (Int_t i = 0; i < fNumSci; i++)
    {
    }

    return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofTofWTcal2HitPar::ReInit() { return kSUCCESS; }

// -----   Public method Execution   --------------------------------------------
void R3BSofTofWTcal2HitPar::Exec(Option_t* option)
{
    // Reading the Input -- Cal Data --
    Int_t nHits = fTofCalDataCA->GetEntries();

    if (nHits == 0)
        return;

    R3BSofTofWSingleTcalData** calData = new R3BSofTofWSingleTcalData*[nHits];
    Int_t sciId = 0;
    for (Int_t i = 0; i < nHits; i++)
    {
        calData[i] = (R3BSofTofWSingleTcalData*)(fTofCalDataCA->At(i));
    }

    if (calData)
        delete calData;
    return;
}

// -----   Protected method Finish   --------------------------------------------
void R3BSofTofWTcal2HitPar::FinishEvent() {}

// -----   Public method Reset   ------------------------------------------------
void R3BSofTofWTcal2HitPar::Reset() {}

void R3BSofTofWTcal2HitPar::FinishTask()
{
    fHit_Par->SetNumSci(fNumSci);

    fHit_Par->setChanged();
}

ClassImp(R3BSofTofWTcal2HitPar)
