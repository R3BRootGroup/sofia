// -------------------------------------------------------------
// -----         R3BSofTwimCal2Hit source file             -----
// -----    Created 30/11/19  by J.L. Rodriguez-Sanchez    -----
// -------------------------------------------------------------

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

// Music headers
#include "R3BSofTwimCal2Hit.h"
#include "R3BSofTwimCalData.h"
#include "R3BSofTwimHitData.h"
#include "R3BSofTwimHitPar.h"

// R3BSofTwimCal2Hit: Default Constructor --------------------------
R3BSofTwimCal2Hit::R3BSofTwimCal2Hit()
    : FairTask("R3BSof Twim Hit Calibrator", 1)
    , fNumSec(1)
    , fNumAnodes(16)
    , fNumParams(2)
    , MaxSigma(200)
    , CalParams(NULL)
    , fCal_Par(NULL)
    , fTwimHitDataCA(NULL)
    , fTwimCalDataCA(NULL)
    , fOnline(kFALSE)
{
}

// R3BSofTwimCal2HitPar: Standard Constructor --------------------------
R3BSofTwimCal2Hit::R3BSofTwimCal2Hit(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNumSec(1)
    , fNumAnodes(16)
    , fNumParams(2)
    , MaxSigma(200)
    , CalParams(NULL)
    , fCal_Par(NULL)
    , fTwimHitDataCA(NULL)
    , fTwimCalDataCA(NULL)
    , fOnline(kFALSE)
{
}

// Virtual R3BSofTwimCal2Hit: Destructor
R3BSofTwimCal2Hit::~R3BSofTwimCal2Hit()
{
    LOG(INFO) << "R3BSofTwimCal2Hit: Delete instance";
    if (fTwimHitDataCA)
        delete fTwimHitDataCA;
    if (fTwimCalDataCA)
        delete fTwimCalDataCA;
}

void R3BSofTwimCal2Hit::SetParContainers()
{

    // Parameter Container
    // Reading TwimCalPar from FairRuntimeDb
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        LOG(ERROR) << "FairRuntimeDb not opened!";
    }

    fCal_Par = (R3BSofTwimHitPar*)rtdb->getContainer("twimHitPar");
    if (!fCal_Par)
    {
        LOG(ERROR) << "R3BSofTwimCal2HitPar::Init() Couldn't get handle on twimHitPar container";
    }
    else
    {
        LOG(INFO) << "R3BSofTwimCal2HitPar:: twimHitPar container open";
    }
}

void R3BSofTwimCal2Hit::SetParameter()
{

    //--- Parameter Container ---
    fNumSec = fCal_Par->GetNumSec();              // Number of Sections
    fNumParams = fCal_Par->GetNumParametersFit(); // Number of Parameters

    LOG(INFO) << "R3BSofTwimCal2Hit: Nb sections: " << fNumSec;
    LOG(INFO) << "R3BSofTwimCal2Hit: Nb parameters from pedestal fit: " << fNumParams;

    CalParams = new TArrayF();
    Int_t array_size = fNumSec * fNumParams;
    CalParams->Set(array_size);
    CalParams = fCal_Par->GetDetectorHitParams(); // Array with the Cal parameters

    // Parameters detector
    for (Int_t s = 0; s < fNumSec; s++)
    {
        LOG(INFO) << "R3BSofTwimCal2Hit Nb section: " << s + 1 << " Par:" << CalParams->GetAt(s * fNumParams) << " : "
                  << CalParams->GetAt(s * fNumParams + 1);
    }
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofTwimCal2Hit::Init()
{
    LOG(INFO) << "R3BSofTwimCal2Hit: Init";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    fTwimCalDataCA = (TClonesArray*)rootManager->GetObject("TwimCalData");
    if (!fTwimCalDataCA)
    {
        return kFATAL;
    }

    // OUTPUT DATA
    fTwimHitDataCA = new TClonesArray("R3BSofTwimHitData", 10);
    if (!fOnline)
    {
        rootManager->Register("TwimHitData", "TWIM Hit", fTwimHitDataCA, kTRUE);
    }
    else
    {
        rootManager->Register("TwimHitData", "TWIM Hit", fTwimHitDataCA, kFALSE);
    }

    SetParameter();
    return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofTwimCal2Hit::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofTwimCal2Hit::Exec(Option_t* option)
{
    // Reset entries in output arrays, local arrays
    Reset();

    if (!fCal_Par)
    {
        LOG(ERROR) << "NO Container Parameter!!";
    }

    Int_t nHits = fTwimCalDataCA->GetEntries();
    if (!nHits)
        return;

    // R3BSofTwimCalData* CalDat;
    R3BSofTwimCalData** CalDat = new R3BSofTwimCalData*[nHits];

    Int_t secId, anodeId;
    Double_t energyperanode[fNumSec][fNumAnodes];
    Int_t nbdet = 0;

    for (Int_t i = 0; i < fNumSec; i++)
        for (Int_t j = 0; j < fNumAnodes; j++)
            energyperanode[i][j] = 0;

    for (Int_t i = 0; i < nHits; i++)
    {
        CalDat[i] = (R3BSofTwimCalData*)(fTwimCalDataCA->At(i));
        secId = CalDat[i]->GetSecID();
        anodeId = CalDat[i]->GetAnodeID();
        energyperanode[secId][anodeId] = CalDat[i]->GetEnergy();
    }

    Double_t nba = 0, a0 = 0., a1 = 0., theta = 0., charge = 0.;

    // calculate truncated dE from 16 anodes, Twim-MUSIC
    for (Int_t i = 0; i < fNumSec; i++)
    {
        for (Int_t j = 0; j < fNumAnodes; j++)
        {
            if (energyperanode[i][j] > 0.)
            {
                charge = charge + energyperanode[i][j];
                nba++;
            }
        }

        if (nba > 0 && charge / nba > 0.)
        {
            a0 = CalParams->GetAt(i * fNumParams);
            a1 = CalParams->GetAt(i * fNumParams + 1);
            if ((a0 + a1 * charge / nba) > 0)
                AddHitData(i, theta, a0 + a1 * charge / nba);
        }
    }

    if (CalDat)
        delete CalDat;
    return;
}

// -----   Protected method Finish   --------------------------------------------
void R3BSofTwimCal2Hit::Finish() {}

// -----   Public method Reset   ------------------------------------------------
void R3BSofTwimCal2Hit::Reset()
{
    LOG(DEBUG) << "Clearing TwimHitData Structure";
    if (fTwimHitDataCA)
        fTwimHitDataCA->Clear();
}

// -----   Private method AddHitData  --------------------------------------------
R3BSofTwimHitData* R3BSofTwimCal2Hit::AddHitData(UShort_t secid, Double_t theta, Double_t charge_z)
{
    // It fills the R3BSofTwimHitData
    TClonesArray& clref = *fTwimHitDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofTwimHitData(secid, theta, charge_z);
}
