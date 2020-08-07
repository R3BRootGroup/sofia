// -------------------------------------------------------------
// -----         R3BSofTwimCal2Hit source file             -----
// -----    Created 30/11/19  by J.L. Rodriguez-Sanchez    -----
// -------------------------------------------------------------

// ROOT headers
#include "TClonesArray.h"
#include "TDecompSVD.h"
#include "TMath.h"
#include "TMatrixD.h"
#include "TRandom.h"
#include "TVectorD.h"

// Fair headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include <iomanip>

// Twim headers
#include "R3BSofTwimCal2Hit.h"
#include "R3BSofTwimCalData.h"
#include "R3BSofTwimHitData.h"
#include "R3BSofTwimHitPar.h"

// R3BSofTwimCal2Hit: Default Constructor --------------------------
R3BSofTwimCal2Hit::R3BSofTwimCal2Hit()
    : FairTask("R3BSof Twim Hit Calibrator", 1)
    , fNumSec(1)
    , fNumAnodes(16)
    , fNumAnodesAngleFit(0)
    , fNumParams(2)
    , CalZParams(NULL)
    , fCal_Par(NULL)
    , fTwimHitDataCA(NULL)
    , fTwimCalDataCA(NULL)
    , fZ0(0)
    , fZ1(0)
    , fZ2(0)
    , fOnline(kFALSE)
{
}

// R3BSofTwimCal2HitPar: Standard Constructor --------------------------
R3BSofTwimCal2Hit::R3BSofTwimCal2Hit(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNumSec(1)
    , fNumAnodes(16)
    , fNumAnodesAngleFit(0)
    , fNumParams(2)
    , CalZParams(NULL)
    , fCal_Par(NULL)
    , fTwimHitDataCA(NULL)
    , fTwimCalDataCA(NULL)
    , fZ0(0)
    , fZ1(0)
    , fZ2(0)
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
    fNumSec = fCal_Par->GetNumSec();        // Number of Sections
    fNumAnodes = fCal_Par->GetNumAnodes();  // Number of anodes
    fNumParams = fCal_Par->GetNumParZFit(); // Number of Parameters

    LOG(INFO) << "R3BSofTwimCal2Hit: Nb sections: " << fNumSec;
    LOG(INFO) << "R3BSofTwimCal2Hit: Nb anodes: " << fNumAnodes;

    // Anodes that don't work set to zero
    for (Int_t s = 0; s < fNumSec; s++)
        for (Int_t i = 0; i < fNumAnodes; i++)
        {
            StatusAnodes[s][i] = fCal_Par->GetInUse(s + 1, i + 1);
        }

    LOG(INFO) << "R3BSofTwimCal2Hit: Nb parameters for charge-Z: " << fNumParams;
    CalZParams = new TArrayF();
    Int_t array_size = fNumSec * fNumParams;
    CalZParams->Set(array_size);
    CalZParams = fCal_Par->GetZHitPar(); // Array with the Cal parameters

    // Parameters detector
    for (Int_t s = 0; s < fNumSec; s++)
        // Parameters detector
        if (fNumParams == 2)
        {
            LOG(INFO) << "R3BSofTwimCal2Hit parameters for charge-Z:" << CalZParams->GetAt(0) << " : "
                      << CalZParams->GetAt(1);
            fZ0 = CalZParams->GetAt(0);
            fZ1 = CalZParams->GetAt(1);
        }
        else if (fNumParams == 3)
        {
            LOG(INFO) << "R3BSofTwimCal2Hit parameters for charge-Z:" << CalZParams->GetAt(0) << " : "
                      << CalZParams->GetAt(1) << " : " << CalZParams->GetAt(2);
            fZ0 = CalZParams->GetAt(0);
            fZ1 = CalZParams->GetAt(1);
            fZ2 = CalZParams->GetAt(2);
        }
        else
            LOG(INFO) << "R3BSofTwimCal2Hit parameters for charge-Z cannot be used here, number of parameters: "
                      << fNumParams;
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
    Double_t dt[fNumSec][fNumAnodes];
    Double_t good_dt[fNumAnodes];
    Int_t nbdet = 0;

    for (Int_t j = 0; j < fNumAnodes; j++)
    {
        good_dt[j] = 0.;
        for (Int_t i = 0; i < fNumSec; i++)
        {
            energyperanode[i][j] = 0;
            dt[i][j] = 0.;
        }
    }

    for (Int_t i = 0; i < nHits; i++)
    {
        CalDat[i] = (R3BSofTwimCalData*)(fTwimCalDataCA->At(i));
        secId = CalDat[i]->GetSecID();
        anodeId = CalDat[i]->GetAnodeID();
        energyperanode[secId][anodeId] = CalDat[i]->GetEnergy();
        dt[secId][anodeId] = CalDat[i]->GetDTime();
    }

    Double_t nba = 0, theta = -5000., Esum = 0.;
    // calculate truncated dE from 16 anodes, Twim-MUSIC
    for (Int_t i = 0; i < fNumSec; i++)
    {
        fNumAnodesAngleFit = 0;
        for (Int_t j = 0; j < fNumAnodes; j++)
        {
            if (energyperanode[i][j] > 0 && energyperanode[i][j] < 8192 && StatusAnodes[i][j] == 1)
            {
                Esum = Esum + energyperanode[i][j];
                good_dt[fNumAnodesAngleFit] = dt[i][j];
                fPosAnodes[fNumAnodesAngleFit] = fCal_Par->GetAnodePos(j + 1);
                fNumAnodesAngleFit++;
                nba++;
            }
        }

        if (nba > 0 && (Esum / nba) > 0.)
        {
            if (fNumAnodesAngleFit > 4)
            {
                fPosZ.Use(fNumAnodesAngleFit, fPosAnodes);
                TMatrixD A(fNumAnodesAngleFit, 2);
                TMatrixDColumn(A, 0) = 1.0;
                TMatrixDColumn(A, 1) = fPosZ;
                TDecompSVD svd(A);
                Bool_t ok;
                TVectorD dt_r;
                dt_r.Use(fNumAnodesAngleFit, good_dt);
                TVectorD c_svd_r = svd.Solve(dt_r, ok);
                theta = c_svd_r[1];
            }
            Double_t zhit =
                fZ0 + fZ1 * TMath::Sqrt(Esum / nba) + fZ2 * TMath::Sqrt(Esum / nba) * TMath::Sqrt(Esum / nba);
            if (zhit > 0 && theta > -5000.)
                AddHitData(i, theta, zhit, Esum / nba);
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
// -----   For later analysis with reconstructed beta -----
R3BSofTwimHitData* R3BSofTwimCal2Hit::AddHitData(UShort_t secid, Double_t theta, Double_t charge_z, Double_t ene_ave)
{
    // It fills the R3BSofTwimHitData
    TClonesArray& clref = *fTwimHitDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofTwimHitData(secid, theta, charge_z, ene_ave);
}
