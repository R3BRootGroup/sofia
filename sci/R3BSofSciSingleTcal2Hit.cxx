// -----------------------------------------------------------------
// -----         R3BSofSciSingleTcal2Hit source file           -----
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
#include "R3BSofSciSingleTcal2Hit.h"
#include "R3BSofSciSingleTcalData.h"

// R3BSofSciSingleTcal2Hit: Default Constructor --------------------------
R3BSofSciSingleTcal2Hit::R3BSofSciSingleTcal2Hit()
    : FairTask("R3BSof-Sci-SingleTcal2Hit task", 1)
    , fSingleTcalDataCA(NULL)
    , fHitDataCA(NULL)
    , fOffsetTof(0.)
    , fRawTofPar(NULL)
    , fTof(0.)
    , fOnline(kFALSE)
{
}

// R3BSofSciSingleTcal2Hit: Standard Constructor --------------------------
R3BSofSciSingleTcal2Hit::R3BSofSciSingleTcal2Hit(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fSingleTcalDataCA(NULL)
    , fHitDataCA(NULL)
    , fRawTofPar(NULL)
    , fOffsetTof(0.)
    , fTof(0.)
    , fOnline(kFALSE)
{
}

// Virtual R3BSofSciSingleTcal2Hit: Destructor
R3BSofSciSingleTcal2Hit::~R3BSofSciSingleTcal2Hit()
{
    LOG(INFO) << "R3BSofSciSingleTcal2Hit: Delete instance";
    if (fSingleTcalDataCA)
        delete fSingleTcalDataCA;
    if (fHitDataCA)
        delete fHitDataCA;
}

void R3BSofSciSingleTcal2Hit::SetParContainers()
{
    fRawTofPar = (R3BSofSciRawTofPar*)FairRuntimeDb::instance()->getContainer("SofSciRawTofPar");
    if (!fRawTofPar)
    {
        LOG(ERROR)
            << "R3BSofSciTcal2SingleTcal::SetParContainers() : Could not get access to SofSciRawTofPar-Container.";
        return;
    }
    else
        LOG(INFO) << "R3BSofSciTcal2SingleTcal::SetParContainers() : SofSciRawTofPar-Container found with "
                  << fRawTofPar->GetNumDets() << " detector declare and " << fRawTofPar->GetNumSignals() << " signals";
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofSciSingleTcal2Hit::Init()
{
    LOG(INFO) << "R3BSofSciSingleTcal2Hit: Init";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    fSingleTcalDataCA = (TClonesArray*)rootManager->GetObject("SofSciSingleTcalData");
    if (!fSingleTcalDataCA)
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

    // --- ---------------------------- --- //
    // --- CHECK THE RAWTOFPAR VALIDITY --- //
    // --- ---------------------------- --- //
    if (fRawTofPar->GetNumDets() == 0)
    { // DO NOT TEST fRawTofPar->GetNumSignals(), IT CAN BE 0 IN CASE OF PRIMARY BEAM EXPERIMENT
        LOG(ERROR)
            << " R3BSofSciSingleTcal2Hit::Init() : There are no SofSci detectors declared, should be at least 1 ";
        return kFATAL;
    }
    else
    {
        LOG(INFO) << "  R3BSofSciSingleTcal2Hit::Init() : fRawPosPar: fNumSignals=" << fRawTofPar->GetNumSignals()
                  << "                                                fNumDets=" << fRawTofPar->GetNumDets();
    }

    return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofSciSingleTcal2Hit::ReInit() { return kSUCCESS; }

// -----   Public method Execution   --------------------------------------------
void R3BSofSciSingleTcal2Hit::Exec(Option_t* option)
{
    // Reset entries in output arrays, local arrays
    Reset();

    // Reading the Input -- SingleTcal Data --
    Int_t nHits = fSingleTcalDataCA->GetEntries();
    if (!nHits)
        return;

    double slope_calibs2 = -50.8;    // only for the s467 in position
    double slope_calibcave = -74.09; // only for the s467 in position
    double offset_calibcave = 90.6;  // only for the s467 in position

    for (Int_t i = 0; i < nHits; i++)
    {

        // Data from SingleTcal level
        R3BSofSciSingleTcalData* hit = (R3BSofSciSingleTcalData*)fSingleTcalDataCA->At(i);

        // if detector at S2
        if (hit->GetDetector() == fRawTofPar->GetDetIdS2())
            AddHitData(hit->GetDetector(), hit->GetRawPosNs() * slope_calibs2, hit->GetRawTofNs_FromS2());
        if (hit->GetDetector() == fRawTofPar->GetDetIdCaveC())
            AddHitData(hit->GetDetector(),
                       hit->GetRawPosNs() * slope_calibcave + offset_calibcave,
                       hit->GetRawTofNs_FromS2() + fTof + fOffsetTof);
    }

    return;
}

// -----   Public method Finish  ------------------------------------------------
void R3BSofSciSingleTcal2Hit::Finish() {}

// -----   Public method Reset   ------------------------------------------------
void R3BSofSciSingleTcal2Hit::Reset()
{
    LOG(DEBUG) << "Clearing TofWHitData structure";
    if (fHitDataCA)
        fHitDataCA->Clear();
}

// -----   Private method AddHitData  --------------------------------------------
R3BSofSciHitData* R3BSofSciSingleTcal2Hit::AddHitData(Int_t sci, Double_t x, Double_t tof)
{
    // It fills the R3BSofSciHitData
    TClonesArray& clref = *fHitDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofSciHitData(sci, x, tof);
}

ClassImp(R3BSofSciSingleTcal2Hit)
