// -----------------------------------------------------------------
// -----         R3BSofSciSingleTCal2Hit source file           -----
// -----    Created 24/02/20  by J.L. Rodriguez-Sanchez        -----
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

// SCI headers
#include "R3BSofSciHitData.h"
#include "R3BSofSciSingleTCal2Hit.h"
#include "R3BSofSciSingleTcalData.h"

// R3BSofSciSingleTCal2Hit: Default Constructor --------------------------
R3BSofSciSingleTCal2Hit::R3BSofSciSingleTCal2Hit()
    : FairTask("R3BSof-Sci-SingleTCal2Hit task", 1)
    , fTCalDataCA(NULL)
    , fHitDataCA(NULL)
    , fOffsetTof(0.)
    , fTof(0.)
    , fOnline(kFALSE)
{
}

// R3BSofSciSingleTCal2Hit: Standard Constructor --------------------------
R3BSofSciSingleTCal2Hit::R3BSofSciSingleTCal2Hit(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fTCalDataCA(NULL)
    , fHitDataCA(NULL)
    , fOffsetTof(0.)
    , fTof(0.)
    , fOnline(kFALSE)
{
}

// Virtual R3BSofSciSingleTCal2Hit: Destructor
R3BSofSciSingleTCal2Hit::~R3BSofSciSingleTCal2Hit()
{
    LOG(INFO) << "R3BSofSciSingleTCal2Hit: Delete instance";
    if (fTCalDataCA)
        delete fTCalDataCA;
    if (fHitDataCA)
        delete fHitDataCA;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofSciSingleTCal2Hit::Init()
{
    LOG(INFO) << "R3BSofSciSingleTCal2Hit: Init";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    fTCalDataCA = (TClonesArray*)rootManager->GetObject("SofSciSingleTcalData");
    if (!fTCalDataCA)
    {
        return kFATAL;
    }

    // OUTPUT DATA
    // Hit data
    fHitDataCA = new TClonesArray("R3BSofSciHitData", 10);

    if (!fOnline)
    {
        rootManager->Register("SofSciHitData", "Sci-Hit", fHitDataCA, kTRUE);
    }
    else
    {
        rootManager->Register("SofSciHitData", "Sci-Hit", fHitDataCA, kFALSE);
    }

    return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofSciSingleTCal2Hit::ReInit() { return kSUCCESS; }

// -----   Public method Execution   --------------------------------------------
void R3BSofSciSingleTCal2Hit::Exec(Option_t* option)
{
    // Reset entries in output arrays, local arrays
    Reset();

    // Reading the Input -- SingleTCal Data --
    Int_t nHits = fTCalDataCA->GetEntries();
    if (!nHits)
        return;

    // Data from SingleTCal level
    R3BSofSciSingleTcalData** calDat;
    calDat = new R3BSofSciSingleTcalData*[nHits];

    double slope_calibs2 = -50.8;    // only for the s467
    double slope_calibcave = -74.09; // only for the s467
    double offset_calibcave = 90.6;  // only for the s467

    if (nHits == 1)
        for (Int_t i = 0; i < nHits; i++)
        {
            calDat[i] = (R3BSofSciSingleTcalData*)(fTCalDataCA->At(i));
#ifdef NUMBER_OF_SOFSCI_TOF
            // AddHitData(1, calDat[i]->GetRawPosNs(1), calDat[i]->GetRawTofNs(1) );//S2-FRS
            AddHitData(2, calDat[i]->GetRawPosNs(2) * slope_calibs2, 0.); // S2-SOFIA
            // AddHitData(3, calDat[i]->GetRawPosNs(3), calDat[i]->GetRawTofNs(3) );//S8
            AddHitData(4,
                       calDat[i]->GetRawPosNs(4) * slope_calibcave + offset_calibcave,
                       calDat[i]->GetRawTofNs(4) + fTof + fOffsetTof); // Cave
#else
            AddHitData(1, calDat[i]->GetRawPosNs(1) * slope_calibcave + offset_calibcave, 0.); // Cave C -SOFIA
#endif
        }

    if (calDat)
        delete calDat;
    return;
}

// -----   Public method Finish  ------------------------------------------------
void R3BSofSciSingleTCal2Hit::Finish() {}

// -----   Public method Reset   ------------------------------------------------
void R3BSofSciSingleTCal2Hit::Reset()
{
    LOG(DEBUG) << "Clearing TofWHitData structure";
    if (fHitDataCA)
        fHitDataCA->Clear();
}

// -----   Private method AddHitData  --------------------------------------------
R3BSofSciHitData* R3BSofSciSingleTCal2Hit::AddHitData(Int_t sci, Double_t x, Double_t tof)
{
    // It fills the R3BSofSciHitData
    TClonesArray& clref = *fHitDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofSciHitData(sci, x, tof);
}

ClassImp(R3BSofSciSingleTCal2Hit)
