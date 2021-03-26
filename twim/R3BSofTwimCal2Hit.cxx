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
#include "R3BSofTofWHitData.h"
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
    , CalZTofParams(NULL)
    , fCal_Par(NULL)
    , fTwimHitDataCA(NULL)
    , fTwimCalDataCA(NULL)
    , fHitItemsTofW(NULL)
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
    , CalZTofParams(NULL)
    , fCal_Par(NULL)
    , fTwimHitDataCA(NULL)
    , fTwimCalDataCA(NULL)
    , fHitItemsTofW(NULL)
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
    if (fHitItemsTofW)
        delete fHitItemsTofW;
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

    if (fHitItemsTofW)
    {
        LOG(INFO) << "R3BSofTwimCal2Hit: Nb parameters for charge-Z vs Tof correction: " << fNumParams;
        CalZTofParams = new TArrayF();
        Int_t size_tof = fNumSec * fNumParams;
        CalZTofParams->Set(size_tof);
        CalZTofParams = fCal_Par->GetZTofHitPar(); // Array with the Cal parameters
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
            LOG(INFO) << "R3BSofTwimCal2Hit parameters for charge-Z:" << CalZParams->GetAt(s * fNumParams) << " : "
                      << CalZParams->GetAt(s * fNumParams + 1);
            fZ0[s] = CalZParams->GetAt(s * fNumParams);
            fZ1[s] = CalZParams->GetAt(s * fNumParams + 1);
        }
        else if (fNumParams == 3)
        {
            if (fHitItemsTofW)
                LOG(INFO) << "R3BSofTwimCal2Hit parameters for charge-Z vs tof:" << CalZTofParams->GetAt(s * fNumParams)
                          << " : " << CalZTofParams->GetAt(s * fNumParams + 1) << " : "
                          << CalZTofParams->GetAt(s * fNumParams + 2);
            LOG(INFO) << "R3BSofTwimCal2Hit parameters for charge-Z:" << CalZParams->GetAt(s * fNumParams) << " : "
                      << CalZParams->GetAt(s * fNumParams + 1) << " : " << CalZParams->GetAt(s * fNumParams + 2);
            fZ0[s] = CalZParams->GetAt(s * fNumParams);
            fZ1[s] = CalZParams->GetAt(s * fNumParams + 1);
            fZ2[s] = CalZParams->GetAt(s * fNumParams + 2);
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

    // get access to cal data of the Twim
    fTwimCalDataCA = (TClonesArray*)rootManager->GetObject("TwimCalData");
    if (!fTwimCalDataCA)
    {
        LOG(ERROR) << "R3BSofTwimCal2Hit::TwimCalData not found";
        return kFATAL;
    }

    // get access to hit data of the Tof-Wall
    fHitItemsTofW = (TClonesArray*)rootManager->GetObject("TofWHitData");
    if (!fHitItemsTofW)
        LOG(WARNING) << "R3BSofTwimCal2Hit::TofWHitData not found";

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
    SetParameter();
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
    Double_t energyperanode[4][fNumAnodes];
    Double_t dt[4][fNumAnodes];
    Double_t good_dt[fNumAnodes];
    Int_t nbdet = 0;

    for (Int_t j = 0; j < fNumAnodes; j++)
    {
        good_dt[j] = 0.;
        for (Int_t i = 0; i < 4; i++)
        {
            energyperanode[i][j] = 0.;
            dt[i][j] = 0.;
        }
    }

    // std::cout<<"Event " <<std::endl;
    for (Int_t i = 0; i < nHits; i++)
    {
        CalDat[i] = (R3BSofTwimCalData*)(fTwimCalDataCA->At(i));
        secId = CalDat[i]->GetSecID();
        anodeId = CalDat[i]->GetAnodeID();
        if (energyperanode[secId][anodeId] == 0)
        {
            energyperanode[secId][anodeId] = CalDat[i]->GetEnergy();
            dt[secId][anodeId] = CalDat[i]->GetDTime();
            // std::cout<< secId <<" "<<anodeId <<" "<< CalDat[i]->GetEnergy() <<std::endl;
        }
    }

    // Fill TofW Hit data
    Double_t tof[2] = { 0., 0. };
    Int_t padid[2] = { 0, 0 };
    if (fHitItemsTofW && fHitItemsTofW->GetEntriesFast() > 0)
    {
        nHits = fHitItemsTofW->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTofWHitData* hit = (R3BSofTofWHitData*)fHitItemsTofW->At(ihit);
            if (!hit)
                continue;
            if (padid[0] == 0)
            {
                padid[0] = hit->GetPaddle();
                tof[0] = hit->GetTof();
            }
            else
            {
                if (hit->GetPaddle() > padid[0] && hit->GetPaddle() - padid[0] > 1)
                {
                    padid[1] = hit->GetPaddle();
                    tof[1] = hit->GetTof(); // right
                }
                else if (hit->GetPaddle() - padid[0] < -1)
                {
                    tof[1] = tof[0]; // right
                    padid[1] = padid[0];
                    tof[0] = hit->GetTof(); // new left
                    padid[0] = hit->GetPaddle();
                }
            }
        }
    }

    // calculate truncated dE from 16 anodes, Twim-MUSIC
    for (Int_t i = 0; i < fNumSec; i++)
    {
        Double_t nba = 0, theta = -5000., Esum = 0.;
        fNumAnodesAngleFit = 0;
        for (Int_t j = 0; j < fNumAnodes; j++)
        {
            // if(i==0){
            // if (((energyperanode[i][j] > 0 && energyperanode[i+1][j] == 0) || (energyperanode[i][j] == 0 &&
            // energyperanode[i+1][j] > 0)) && energyperanode[i][j] < 65535 && StatusAnodes[i][j] == 1 &&
            // energyperanode[i+1][j] < 65535 && StatusAnodes[i+1][j] == 1)

            if (energyperanode[i][j] > 0 && energyperanode[i][j] < 65535 && StatusAnodes[i][j] == 1)
            {
                Esum += energyperanode[i][j];
                if (dt[i][j] > 0.)
                    good_dt[fNumAnodesAngleFit] = dt[i][j];
                else if (dt[i + 1][j] > 0.)
                    good_dt[fNumAnodesAngleFit] = dt[i + 1][j];
                fPosAnodes[fNumAnodesAngleFit] = fCal_Par->GetAnodePos(j + 1);
                fNumAnodesAngleFit++;
                nba++;
            }
            /*}else
            if ( energyperanode[i][j] > 0 && energyperanode[i][j] < 65535 && StatusAnodes[i][j] == 1 &&
            energyperanode[i+1][j] < 65535 && StatusAnodes[i+1][j] == 1)
            {
                Esum = Esum + energyperanode[i][j];// + energyperanode[i + 1][j];
                if (dt[i][j] > 0.)
                    good_dt[fNumAnodesAngleFit] = dt[i][j];
                else if (dt[i + 1][j] > 0.)
                    good_dt[fNumAnodesAngleFit] = dt[i + 1][j];
                fPosAnodes[fNumAnodesAngleFit] = fCal_Par->GetAnodePos(j + 1);
                fNumAnodesAngleFit++;
                nba++;
            }
            */
        }

        if (nba > 8 && (Esum / nba) > 0.)
        {
            if (fNumAnodesAngleFit > 8)
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
            if (CalZTofParams)
            {
                Double_t Esum_mean = Esum / nba;
                if (i < 2)
                    Esum_mean = Esum_mean - (CalZTofParams->GetAt(i * fNumParams) +
                                             CalZTofParams->GetAt(i * fNumParams + 1) * tof[0] +
                                             CalZTofParams->GetAt(i * fNumParams + 2) * tof[0] * tof[0]);
                else
                    Esum_mean = Esum_mean - (CalZTofParams->GetAt(i * fNumParams) +
                                             CalZTofParams->GetAt(i * fNumParams + 1) * tof[1] +
                                             CalZTofParams->GetAt(i * fNumParams + 2) * tof[1] * tof[1]);

                Double_t zhit = fZ0[i] + fZ1[i] * TMath::Sqrt(Esum_mean) + fZ2[i] * Esum_mean;
                // if (zhit > 0 && theta > -5000.)
                if (zhit > 0)
                    AddHitData(i, theta, zhit, Esum_mean);
            }
            else
            {
                Double_t Esum_mean = Esum / nba;
                Double_t zhit = fZ0[i] + fZ1[i] * TMath::Sqrt(Esum_mean) + fZ2[i] * Esum_mean;
                // if (zhit > 0 && theta > -5000.)
                if (zhit > 0)
                    AddHitData(i, theta, zhit, Esum_mean);
            }
        }
        // i = i + 2;
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
