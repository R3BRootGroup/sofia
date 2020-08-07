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
#include "R3BSofTofWHitData.h"
#include "R3BSofTofWSingleTcalData.h"
#include "R3BSofTofWTCal2Hit.h"

// FIXME after beam 15 Feb 2020
Double_t offset_tof[28];
Double_t offset_pos[28];

// R3BSofTofWTCal2Hit: Default Constructor --------------------------
R3BSofTofWTCal2Hit::R3BSofTofWTCal2Hit()
    : FairTask("R3BSof TofW tcal2hit Task", 1)
    , fTCalDataCA(NULL)
    , fHitDataCA(NULL)
    , fOnline(kFALSE)
{
    offset_pos[0] = +2.28;
    offset_pos[1] = +1.03;
    offset_pos[2] = +1.287;
    offset_pos[3] = -2.03;
    offset_pos[4] = -0.62;
    offset_pos[5] = -0.50;
    offset_pos[6] = +0.81;
    offset_pos[7] = -0.16;
    offset_pos[8] = +3.531;
    offset_pos[9] = +2.17;
    offset_pos[10] = -1.67;
    offset_pos[11] = -1.26;
    offset_pos[12] = +2.265;
    offset_pos[13] = +4.42;
    offset_pos[14] = +6.51;
    offset_pos[15] = +0.14;
    offset_pos[16] = -2.21;
    offset_pos[17] = -6.15;
    offset_pos[18] = -0.45;
    offset_pos[19] = +0.47;
    offset_pos[20] = +1.25;
    offset_pos[21] = +3.09;
    offset_pos[22] = -1.07;
    offset_pos[23] = +2.44;
    offset_pos[24] = +0.495;
    offset_pos[25] = -0.22;
    offset_pos[26] = +0.93;
    offset_pos[27] = 0.0;

    Double_t Tof_lise = 41.6; // ns

    offset_tof[0] = Tof_lise + 37.08;
    offset_tof[1] = Tof_lise + 36.43;
    offset_tof[2] = Tof_lise + 37.74;
    offset_tof[3] = Tof_lise + 36.86;
    offset_tof[4] = Tof_lise + 37.32;
    offset_tof[5] = Tof_lise + 37.63;
    offset_tof[6] = Tof_lise + 38.05;
    offset_tof[7] = Tof_lise + 37.68;
    offset_tof[8] = Tof_lise + 36.8;
    offset_tof[9] = Tof_lise + 37.59;
    offset_tof[10] = Tof_lise + 35.88;
    offset_tof[11] = Tof_lise + 37.58;
    offset_tof[12] = Tof_lise + 36.36;
    offset_tof[13] = Tof_lise + 37.14;
    offset_tof[14] = Tof_lise + 38.94;
    offset_tof[15] = Tof_lise + 38.6;
    offset_tof[16] = Tof_lise + 39.29;
    offset_tof[17] = Tof_lise + 38.14;
    offset_tof[18] = Tof_lise + 38.72;
    offset_tof[19] = Tof_lise + 38.48;
    offset_tof[20] = Tof_lise + 40.33;
    offset_tof[21] = Tof_lise + 40.53;
    offset_tof[22] = Tof_lise + 40.13;
    offset_tof[23] = Tof_lise + 40.15;
    offset_tof[24] = Tof_lise + 39.51;
    offset_tof[25] = Tof_lise + 39.3;
    offset_tof[26] = Tof_lise + 39.33;
    offset_tof[27] = Tof_lise;
}

// R3BSofTofWTCal2Hit: Standard Constructor --------------------------
R3BSofTofWTCal2Hit::R3BSofTofWTCal2Hit(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fTCalDataCA(NULL)
    , fHitDataCA(NULL)
    , fOnline(kFALSE)
{
    offset_pos[0] = +2.28;
    offset_pos[1] = +1.03;
    offset_pos[2] = +1.287;
    offset_pos[3] = -2.03;
    offset_pos[4] = -0.62;
    offset_pos[5] = -0.50;
    offset_pos[6] = +0.81;
    offset_pos[7] = -0.16;
    offset_pos[8] = +3.531;
    offset_pos[9] = +2.17;
    offset_pos[10] = -1.67;
    offset_pos[11] = -1.26;
    offset_pos[12] = +2.265;
    offset_pos[13] = +4.42;
    offset_pos[14] = +6.51;
    offset_pos[15] = +0.14;
    offset_pos[16] = -2.21;
    offset_pos[17] = -6.15;
    offset_pos[18] = -0.45;
    offset_pos[19] = +0.47;
    offset_pos[20] = +1.25;
    offset_pos[21] = +3.09;
    offset_pos[22] = -1.07;
    offset_pos[23] = +2.44;
    offset_pos[24] = +0.495;
    offset_pos[25] = -0.22;
    offset_pos[26] = +0.93;
    offset_pos[27] = 0.0;

    Double_t Tof_lise = 43.0; // ns

    offset_tof[0] = Tof_lise + 37.08;
    offset_tof[1] = Tof_lise + 36.43;
    offset_tof[2] = Tof_lise + 37.74;
    offset_tof[3] = Tof_lise + 36.86;
    offset_tof[4] = Tof_lise + 37.32;
    offset_tof[5] = Tof_lise + 37.63;
    offset_tof[6] = Tof_lise + 38.05;
    offset_tof[7] = Tof_lise + 37.68;
    offset_tof[8] = Tof_lise + 36.8;
    offset_tof[9] = Tof_lise + 37.59;
    offset_tof[10] = Tof_lise + 35.88;
    offset_tof[11] = Tof_lise + 37.58;
    offset_tof[12] = Tof_lise + 36.36;
    offset_tof[13] = Tof_lise + 37.14;
    offset_tof[14] = Tof_lise + 38.94;
    offset_tof[15] = Tof_lise + 38.6;
    offset_tof[16] = Tof_lise + 39.29;
    offset_tof[17] = Tof_lise + 38.14;
    offset_tof[18] = Tof_lise + 38.72;
    offset_tof[19] = Tof_lise + 38.48;
    offset_tof[20] = Tof_lise + 40.33;
    offset_tof[21] = Tof_lise + 40.53;
    offset_tof[22] = Tof_lise + 40.13;
    offset_tof[23] = Tof_lise + 40.15;
    offset_tof[24] = Tof_lise + 39.51;
    offset_tof[25] = Tof_lise + 39.3;
    offset_tof[26] = Tof_lise + 39.33;
    offset_tof[27] = Tof_lise;
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
    R3BSofTofWSingleTcalData** calDat;
    calDat = new R3BSofTofWSingleTcalData*[nHits];
    Int_t fPaddleId = 0; // from 1 to 28
    Double_t tofw = 0.;

    if (nHits == 1)
        for (Int_t i = 0; i < nHits; i++)
        {
            calDat[i] = (R3BSofTofWSingleTcalData*)(fTCalDataCA->At(i));
            fPaddleId = calDat[i]->GetDetector();
            tofw = calDat[i]->GetRawTofNs();
            AddHitData(fPaddleId,
                       -450. + 15. + (fPaddleId - 1) * 30.,
                       calDat[i]->GetRawPosNs() + offset_pos[fPaddleId - 1],
                       tofw + offset_tof[fPaddleId - 1]);
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
