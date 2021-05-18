// ----------------------------------------------------------------------
// -----                     R3BSofMwpc3Mapped2Cal                  -----
// -----             Created 11/10/19  by G. García Jiménez         -----
// -----             by modifying J.L 's class for Mwpc0            -----
// ----------------------------------------------------------------------

#include "TClonesArray.h"
#include "TMath.h"
#include "TRandom.h"

// Fair headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include <iomanip>

// MWPC headers
#include "R3BSofMwpc3CalPar.h"
#include "R3BSofMwpc3Mapped2Cal.h"
#include "R3BSofMwpcCalData.h"
#include "R3BSofMwpcMappedData.h"

/* ---- R3BSofMwpc3Mapped2Cal: Default Constructor ---- */
R3BSofMwpc3Mapped2Cal::R3BSofMwpc3Mapped2Cal()
    : FairTask("R3B MWPC3 Calibrator", 1)
    , NumPadX(0)
    , NumPadY(0)
    , NumParams(0)
    , CalParams(NULL)
    , fCal_Par(NULL)
    , fMwpcMappedDataCA(NULL)
    , fMwpcCalDataCA(NULL)
    , fOnline(kFALSE)
{
}

/* ---- R3BSofMwpc3Mapped2Cal: Standard Constructor ---- */
R3BSofMwpc3Mapped2Cal::R3BSofMwpc3Mapped2Cal(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , NumPadX(0)
    , NumPadY(0)
    , NumParams(0)
    , CalParams(NULL)
    , fCal_Par(NULL)
    , fMwpcMappedDataCA(NULL)
    , fMwpcCalDataCA(NULL)
    , fOnline(kFALSE)
{
}

/* ---- Virtual R3BSofMwpc3Mapped2Cal: Destructor ----*/
R3BSofMwpc3Mapped2Cal::~R3BSofMwpc3Mapped2Cal()
{
    LOG(INFO) << "R3BSofMwpc3Mapped2Cal: Delete instance";
    if (fMwpcMappedDataCA)
        delete fMwpcMappedDataCA;
    if (fMwpcCalDataCA)
        delete fMwpcCalDataCA;
}

void R3BSofMwpc3Mapped2Cal::SetParContainers()
{

    // Parameter Container
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        LOG(ERROR) << "FairRuntimeDb not opened!";
    }

    fCal_Par = (R3BSofMwpc3CalPar*)rtdb->getContainer("mwpc3CalPar");
    if (!fCal_Par)
    {
        LOG(ERROR) << "R3BSofMwpc3Mapped2Cal::Init() Couldn't get handle on mwpc3CalPar container";
    }
    else
    {
        LOG(INFO) << "R3BSofMwpc3Mapped2Cal:: mwpc3CalPar container open";
    }
}

void R3BSofMwpc3Mapped2Cal::SetParameter()
{

    /* ---- Parameter Container ---- */
    NumPadX = fCal_Par->GetNumPadsX();           // Number of Pads in X
    NumPadY = fCal_Par->GetNumPadsY();           // Number of Pads in Y
    NumParams = fCal_Par->GetNumParametersFit(); // Number of parameters in the Fit

    LOG(INFO) << "R3BSofMwpc3Mapped2Cal: NumPadX: " << NumPadX;
    LOG(INFO) << "R3BSofMwpc3Mapped2Cal: NumPadY: " << NumPadY;
    LOG(INFO) << "R3BSofMwpc3Mapped2Cal: Number of fit parameters: " << NumParams;

    CalParams = new TArrayF();
    Int_t array_size = (NumPadX + NumPadY) * NumParams;
    CalParams->Set(array_size);
    CalParams = fCal_Par->GetPadCalParams(); // Array with the Cal parameters
}

/* ---- Public method Init  ---- */
InitStatus R3BSofMwpc3Mapped2Cal::Init()
{
    LOG(INFO) << "R3BSofMwpc3Mapped2Cal: Init";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    fMwpcMappedDataCA = (TClonesArray*)rootManager->GetObject("Mwpc3MappedData");
    if (!fMwpcMappedDataCA)
    {
        return kFATAL;
    }

    // OUTPUT DATA
    // Calibrated data
    fMwpcCalDataCA = new TClonesArray("R3BSofMwpcCalData", 10);

    if (!fOnline)
    {
        rootManager->Register("Mwpc3CalData", "MWPC3 Cal", fMwpcCalDataCA, kTRUE);
    }
    else
    {
        rootManager->Register("Mwpc3CalData", "MWPC3 Cal", fMwpcCalDataCA, kFALSE);
    }

    SetParameter();
    return kSUCCESS;
}

/* ----   Public method ReInit ---- */
InitStatus R3BSofMwpc3Mapped2Cal::ReInit()
{
    SetParContainers();
    SetParameter();
    return kSUCCESS;
}

/* ---- Public method Execution ---- */
void R3BSofMwpc3Mapped2Cal::Exec(Option_t* option)
{
    // Reset entries in output arrays, local arrays
    Reset();

    if (!fCal_Par)
    {
        LOG(ERROR) << "NO Container Parameter!, pedestals will be set to zero";
    }

    // Reading the Input -- Mapped Data --
    Int_t nHits = fMwpcMappedDataCA->GetEntries();
    if (nHits > (NumPadX + NumPadY) && nHits > 0)
        LOG(WARNING) << "R3BSofMwpc3Mapped2Cal: nHits>(NumPadX+NumPadY)";
    if (!nHits)
        return;

    R3BSofMwpcMappedData** mappedData;
    mappedData = new R3BSofMwpcMappedData*[nHits];
    Int_t planeId;
    Int_t padId;
    Int_t charge;
    Int_t pedestal = 0;
    Int_t nbpad = 0;

    for (Int_t i = 0; i < nHits; i++)
    {
        mappedData[i] = (R3BSofMwpcMappedData*)(fMwpcMappedDataCA->At(i));
        planeId = mappedData[i]->GetPlane();
        padId = mappedData[i]->GetPad();
        if (planeId == 1)
            nbpad = padId * NumParams;
        else if (planeId == 3)
            nbpad = (padId + NumPadX) * NumParams;
        else
            LOG(ERROR) << "Plane " << planeId << " does not exist in MWPC3";

        pedestal = CalParams->GetAt(nbpad);
        charge = mappedData[i]->GetQ() - pedestal;

        // We accept the hit if the charge is larger than zero
        if (charge > 0)
        {
            AddCalData(planeId, padId, charge);
        }
    }
    if (mappedData)
        delete mappedData;
    return;
}

/* ---- Protected method Finish ---- */
void R3BSofMwpc3Mapped2Cal::Finish() {}

/* ---- Public method Reset ----*/
void R3BSofMwpc3Mapped2Cal::Reset()
{
    LOG(DEBUG) << "Clearing MwpcCalData Structure";
    if (fMwpcCalDataCA)
        fMwpcCalDataCA->Clear();
}

/* ----   Private method AddCalData  ---- */
R3BSofMwpcCalData* R3BSofMwpc3Mapped2Cal::AddCalData(Int_t plane, Int_t pad, Int_t charge)
{
    // It fills the R3BSofMwpcCalData
    TClonesArray& clref = *fMwpcCalDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofMwpcCalData(plane, pad, charge);
}

ClassImp(R3BSofMwpc3Mapped2Cal)
