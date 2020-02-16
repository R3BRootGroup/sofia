// -----------------------------------------------------------------
// -----         R3BSofTofWTCal2Hit source file                -----
// -----    Created 15/02/20  by J.L. Rodriguez-Sanchez        -----
// -----------------------------------------------------------------

// ROOT headers
#include "TClonesArray.h"
#include "TMath.h"

// Fair headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include <iomanip>

// TofW headers
#include "R3BSofToFWSingleTcalData.h"
#include "R3BSofTofWHitData.h"
#include "R3BSofTofWTCal2Hit.h"

// FIXME after beam 15 Feb 2020
Double_t offset_tof[28];

// R3BSofTofWTCal2Hit: Default Constructor --------------------------
R3BSofTofWTCal2Hit::R3BSofTofWTCal2Hit()
    : FairTask("R3BSof TofW tcal2hit Task", 1)
    , fTCalDataCA(NULL)
    , fHitDataCA(NULL)
    , fOnline(kFALSE)
{
    offset_tof[0] = 46.3+2.28-4.567;
    offset_tof[1] = 46.3+1.03-4.567;
    offset_tof[2] = 46.3+1.287-4.567;
    offset_tof[3] = 46.3-2.03-4.567;
    offset_tof[4] = 46.3-0.62-4.567;
    offset_tof[5] = 46.3-0.50-4.567;
    offset_tof[6] = 46.3+0.81-4.567;
    offset_tof[7] = 46.3-0.16-4.567;
    offset_tof[8] = 46.3+3.531-4.567;
    offset_tof[9] = 46.3+2.17-4.567;
    offset_tof[10] = 46.3-1.67-4.567;
    offset_tof[11] = 46.3-1.26-4.567;
    offset_tof[12] = 46.3+2.265-4.567;
    offset_tof[13] = 46.3+4.42-4.567;
    offset_tof[14] = 46.3+6.51-4.567;
    offset_tof[15] = 46.3+0.14-4.567;
    offset_tof[16] = 46.3-2.21-4.567;
    offset_tof[17] = 46.3-6.15-4.567;
    offset_tof[18] = 46.3-0.45-4.567;
    offset_tof[19] = 46.3+0.47-4.567;
    offset_tof[20] = 46.3+1.25-4.567;
    offset_tof[21] = 46.3+3.09-4.567;
    offset_tof[22] = 46.3-1.07-4.567;
    offset_tof[23] = 46.3+2.44-4.567;
    offset_tof[24] = 46.3+0.495-4.567;
    offset_tof[25] = 46.3-0.22-4.567;
    offset_tof[26] = 46.3+0.93-4.567;
    offset_tof[27] = 46.3-4.567;
}

// R3BSofTofWTCal2Hit: Standard Constructor --------------------------
R3BSofTofWTCal2Hit::R3BSofTofWTCal2Hit(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fTCalDataCA(NULL)
    , fHitDataCA(NULL)
    , fOnline(kFALSE)
{
    offset_tof[0] = 46.3+2.28-4.567;
    offset_tof[1] = 46.3+1.03-4.567;
    offset_tof[2] = 46.3+1.287-4.567;
    offset_tof[3] = 46.3-2.03-4.567;
    offset_tof[4] = 46.3-0.62-4.567;
    offset_tof[5] = 46.3-0.50-4.567;
    offset_tof[6] = 46.3+0.81-4.567;
    offset_tof[7] = 46.3-0.16-4.567;
    offset_tof[8] = 46.3+3.531-4.567;
    offset_tof[9] = 46.3+2.17-4.567;
    offset_tof[10] = 46.3-1.67-4.567;
    offset_tof[11] = 46.3-1.26-4.567;
    offset_tof[12] = 46.3+2.265-4.567;
    offset_tof[13] = 46.3+4.42-4.567;
    offset_tof[14] = 46.3+6.51-4.567;
    offset_tof[15] = 46.3+0.14-4.567;
    offset_tof[16] = 46.3-2.21-4.567;
    offset_tof[17] = 46.3-6.15-4.567;
    offset_tof[18] = 46.3-0.45-4.567;
    offset_tof[19] = 46.3+0.47-4.567;
    offset_tof[20] = 46.3+1.25-4.567;
    offset_tof[21] = 46.3+3.09-4.567;
    offset_tof[22] = 46.3-1.07-4.567;
    offset_tof[23] = 46.3+2.44-4.567;
    offset_tof[24] = 46.3+0.495-4.567;
    offset_tof[25] = 46.3-0.22-4.567;
    offset_tof[26] = 46.3+0.93-4.567;
    offset_tof[27] = 46.3-4.567;
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

    fTCalDataCA = (TClonesArray*)rootManager->GetObject("SofToFWSingleTcalData");
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
    if (!nHits)
        return;

    // Data from cal level
    R3BSofToFWSingleTcalData** calDat;
    calDat = new R3BSofToFWSingleTcalData*[nHits];
    Int_t fPaddleId = 0; // from 1 to 28
    Double_t tofw = 0.;

    if(nHits==1)
    for (Int_t i = 0; i < nHits; i++)
    {
        calDat[i] = (R3BSofToFWSingleTcalData*)(fTCalDataCA->At(i));
        fPaddleId= calDat[i]->GetDetector();
        tofw=calDat[i]->GetRawTofNs();
        AddHitData(fPaddleId, -450.+15.+(fPaddleId-1)*30., calDat[i]->GetRawPosNs(), tofw+offset_tof[fPaddleId-1]);
    }
    
    if (calDat)
        delete calDat;
    return;
}

// -----   Public method Finish  ------------------------------------------------
void R3BSofTofWTCal2Hit::Finish() {}

// -----   Public method Reset   ------------------------------------------------
void R3BSofTofWTCal2Hit::Reset()
{
    LOG(DEBUG) << "Clearing TofWHitData structure";
    if (fHitDataCA)
        fHitDataCA->Clear();
}

// -----   Private method AddHitData  --------------------------------------------
R3BSofTofWHitData* R3BSofTofWTCal2Hit::AddHitData(Int_t paddle, Double_t x, Double_t y, Double_t tof)
{
    // It fills the R3BSofTofWHitData
    TClonesArray& clref = *fHitDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofTofWHitData(paddle, x, y, tof);
}

ClassImp(R3BSofTofWTCal2Hit)
