// ----------------------------------------------------------------
// -----         R3BSofTwimMapped2Cal source file             -----
// -----    Created 24/11/19  by J.L. Rodriguez-Sanchez       -----
// ----------------------------------------------------------------

// ROOT headers
#include "TClonesArray.h"
#include "TMath.h"
#include "TRandom.h"

// Fair headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include <iomanip>

// TWIM headers
#include "R3BSofTwimCalData.h"
#include "R3BSofTwimCalPar.h"
#include "R3BSofTwimMapped2Cal.h"
#include "R3BSofTwimMappedData.h"

// R3BSofTwimMapped2Cal: Default Constructor --------------------------
R3BSofTwimMapped2Cal::R3BSofTwimMapped2Cal()
    : FairTask("R3BSof Twim Cal Calibrator", 1)
    , NumSec(1)
    , NumAnodes(16)
    , NumParams(0)
    , CalParams(NULL)
    , fCal_Par(NULL)
    , fTwimMappedDataCA(NULL)
    , fTwimCalDataCA(NULL)
    , fOnline(kFALSE)
{
}

// R3BSofTwimMapped2CalPar: Standard Constructor --------------------------
R3BSofTwimMapped2Cal::R3BSofTwimMapped2Cal(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , NumSec(1)
    , NumAnodes(16)
    , NumParams(0)
    , CalParams(NULL)
    , fCal_Par(NULL)
    , fTwimMappedDataCA(NULL)
    , fTwimCalDataCA(NULL)
    , fOnline(kFALSE)
{
}

// Virtual R3BSofTwimMapped2Cal: Destructor
R3BSofTwimMapped2Cal::~R3BSofTwimMapped2Cal()
{
    LOG(INFO) << "R3BSofTwimMapped2Cal: Delete instance";
    if (fTwimMappedDataCA)
        delete fTwimMappedDataCA;
    if (fTwimCalDataCA)
        delete fTwimCalDataCA;
}

void R3BSofTwimMapped2Cal::SetParContainers()
{
    // Parameter Container
    // Reading amsStripCalPar from FairRuntimeDb
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        LOG(ERROR) << "FairRuntimeDb not opened!";
    }

    fCal_Par = (R3BSofTwimCalPar*)rtdb->getContainer("twimCalPar");
    if (!fCal_Par)
    {
        LOG(ERROR) << "R3BSofTwimMapped2CalPar::Init() Couldn't get handle on twimCalPar container";
    }
    else
    {
        LOG(INFO) << "R3BSofTwimMapped2CalPar:: twimCalPar container open";
    }
}

void R3BSofTwimMapped2Cal::SetParameter()
{

    //--- Parameter Container ---
    NumSec = fCal_Par->GetNumSec();              // Number of sections
    NumAnodes = fCal_Par->GetNumAnodes();        // Number of anodes per section
    NumParams = fCal_Par->GetNumParametersFit(); // Number of Parameters

    LOG(INFO) << "R3BSofTwimMapped2Cal: NumSec: " << NumSec;
    LOG(INFO) << "R3BSofTwimMapped2Cal: NumAnodes: " << NumAnodes;
    LOG(INFO) << "R3BSofTwimMapped2Cal: NumParams: " << NumParams;

    CalParams = new TArrayF();
    Int_t array_size = (NumSec * NumAnodes) * NumParams;
    CalParams->Set(array_size);
    CalParams = fCal_Par->GetTwimCalParams(); // Array with the Cal parameters
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofTwimMapped2Cal::Init()
{
    LOG(INFO) << "R3BSofTwimMapped2Cal: Init";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    fTwimMappedDataCA = (TClonesArray*)rootManager->GetObject("TwimMappedData");
    if (!fTwimMappedDataCA)
    {
        return kFATAL;
    }

    // OUTPUT DATA
    // Calibrated data
    fTwimCalDataCA = new TClonesArray("R3BSofTwimCalData", 10);

    if (!fOnline)
    {
        rootManager->Register("TwimCalData", "TWIM Cal", fTwimCalDataCA, kTRUE);
    }
    else
    {
        rootManager->Register("TwimCalData", "TWIM Cal", fTwimCalDataCA, kFALSE);
    }

    SetParameter();
    return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofTwimMapped2Cal::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofTwimMapped2Cal::Exec(Option_t* option)
{
    // Reset entries in output arrays, local arrays
    Reset();

    if (!fCal_Par)
    {
        LOG(ERROR) << "NO Container Parameter!!";
    }

    // Reading the Input -- Mapped Data --
    Int_t nHits = fTwimMappedDataCA->GetEntries();
    // if (nHits != (NumSec * NumAnodes) && nHits > 0)
    //     LOG(WARNING) << "R3BSofTwimMapped2Cal: nHits!=NumSec*NumAnodes"; // FIXME: what happens with multihit ??
    if (!nHits)
        return;

    R3BSofTwimMappedData** mappedData;
    mappedData = new R3BSofTwimMappedData*[nHits];
    UShort_t secId;
    UShort_t anodeId;
    Double_t energy;
    Double_t pedestal = 0.;
    Double_t dtime = 0.;

    for (Int_t i = 0; i < NumAnodes + NumSec; i++)
    {
        fE[i] = 0.;
        fDT[i] = 0.;
    }

    for (Int_t i = 0; i < nHits; i++)
    {
        mappedData[i] = (R3BSofTwimMappedData*)(fTwimMappedDataCA->At(i));
        secId = mappedData[i]->GetSecID();
        anodeId = mappedData[i]->GetAnodeID();
        energy = mappedData[i]->GetEnergy() - pedestal; // FIXME
        dtime = mappedData[i]->GetTime();

        if (fDT[anodeId] == 0)
            fDT[anodeId] = dtime; // mult=1
        if (fE[anodeId] == 0)
            fE[anodeId] = energy; // mult=1
    }

    for (Int_t i = 0; i < NumAnodes; i++)
    {
        // We accept the hit if the energy is larger than zero
        if (fE[i] > 0)
        {
            AddCalData(0, i, fDT[i] - fDT[NumAnodes], fE[i]); // Only one section
        }
    }

    if (mappedData)
        delete mappedData;
    return;
}

// -----   Protected method Finish   --------------------------------------------
void R3BSofTwimMapped2Cal::Finish() {}

// -----   Public method Reset   ------------------------------------------------
void R3BSofTwimMapped2Cal::Reset()
{
    LOG(DEBUG) << "Clearing TwimCalData Structure";
    if (fTwimCalDataCA)
        fTwimCalDataCA->Clear();
}

// -----   Private method AddCalData  --------------------------------------------
R3BSofTwimCalData* R3BSofTwimMapped2Cal::AddCalData(UShort_t secID, UShort_t anodeID, Double_t dtime, Double_t energy)
{
    // It fills the R3BSofTwimCalData
    TClonesArray& clref = *fTwimCalDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofTwimCalData(secID, anodeID, dtime, energy);
}

ClassImp(R3BSofTwimMapped2Cal)
