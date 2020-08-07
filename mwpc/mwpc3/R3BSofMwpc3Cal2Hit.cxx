// ----------------------------------------------------------------------
// -----                     R3BSofMwpc3Cal2Hit                     -----
// -----             Created 14/10/19  by G. García Jiménez         -----
// -----             by modifying J.L classes for MWPC0             -----
// ----------------------------------------------------------------------

#include "TClonesArray.h"
#include "TMath.h"

// Fair headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include <iomanip>

// MWPC headers
#include "R3BSofMwpc3Cal2Hit.h"
#include "R3BSofMwpcCalData.h"
#include "R3BSofMwpcHitData.h"

/* ---- R3BSofMwpc3Cal2Hit: Default Constructor ---- */

R3BSofMwpc3Cal2Hit::R3BSofMwpc3Cal2Hit()
    : FairTask("R3B Hit-MWPC3 Task", 1)
    , fMwpcCalDataCA(NULL)
    , fMwpcHitDataCA(NULL)
    , fwy(5.000)
    , fwx(3.125)
    , fSizeX(900.0)
    , fSizeY(600.0) // in mm
    , fOnline(kFALSE)
{
}

/* ---- R3BSofMwpc3Cal2Hit: Standard Constructor ---- */
R3BSofMwpc3Cal2Hit::R3BSofMwpc3Cal2Hit(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fMwpcCalDataCA(NULL)
    , fMwpcHitDataCA(NULL)
    , fwy(5.000)
    , fwx(3.125)
    , fSizeX(900.0)
    , fSizeY(600.0) // in mm
    , fOnline(kFALSE)
{
}

/* ---- Virtual R3BSofMwpc3Cal2Hit: Destructor ---- */
R3BSofMwpc3Cal2Hit::~R3BSofMwpc3Cal2Hit()
{
    LOG(INFO) << "R3BSofMwpc3Cal2Hit: Delete instance";
    if (fMwpcCalDataCA)
        delete fMwpcCalDataCA;
    if (fMwpcHitDataCA)
        delete fMwpcHitDataCA;
}

/* ---- Public method Init   ---- */
InitStatus R3BSofMwpc3Cal2Hit::Init()
{
    LOG(INFO) << "R3BSofMwpc3Cal2Hit: Init";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    fMwpcCalDataCA = (TClonesArray*)rootManager->GetObject("Mwpc3CalData");
    if (!fMwpcCalDataCA)
    {
        return kFATAL;
    }

    // OUTPUT DATA
    // Hit data
    fMwpcHitDataCA = new TClonesArray("R3BSofMwpcHitData", 10);

    if (!fOnline)
    {
        rootManager->Register("Mwpc3HitData", "MWPC3 Hit", fMwpcHitDataCA, kTRUE);
    }
    else
    {
        rootManager->Register("Mwpc3HitData", "MWPC3 Hit", fMwpcHitDataCA, kFALSE);
    }

    return kSUCCESS;
}

/* ----   Public method ReInit   ---- */
InitStatus R3BSofMwpc3Cal2Hit::ReInit() { return kSUCCESS; }

/* ----   Public method Execution   ---- */
void R3BSofMwpc3Cal2Hit::Exec(Option_t* option)
{
    // Reset entries in output arrays, local arrays
    Reset();

    // Reading the Input -- Cal Data --
    Int_t nHits = fMwpcCalDataCA->GetEntries();
    if (!nHits)
        return;

    // Data from cal level
    R3BSofMwpcCalData** calData;
    calData = new R3BSofMwpcCalData*[nHits];
    Int_t planeId;
    Int_t padId;
    Int_t padmx = -1, padmy = -1;
    Double_t q = 0, qmx = 0, qmy = 0, qleft = 0, qright = 0, qdown = 0, qup = 0;
    Double_t x = -1000., y = -1000.;

    for (Int_t i = 0; i < Mw3PadsX; i++)
        fx[i] = 0;
    for (Int_t i = 0; i < Mw3PadsY; i++)
        fy[i] = 0;

    for (Int_t i = 0; i < nHits; i++)
    {
        calData[i] = (R3BSofMwpcCalData*)(fMwpcCalDataCA->At(i));
        planeId = calData[i]->GetPlane();
        padId = calData[i]->GetPad();
        q = calData[i]->GetQ();
        if (planeId == 1)
            fx[padId] = q;
        else
            fy[padId] = q;
        if (q > qmx && planeId == 1)
        {
            qmx = q;
            padmx = padId;
        }
        if (q > qmy && planeId == 3)
        {
            qmy = q;
            padmy = padId;
        }
    }
    // Add Hit data ----
    if (padmx > 0 && padmy > 0 && padmx + 1 < Mw3PadsX && padmy + 1 < Mw3PadsY && qmx > 0 && qmy > 0)
    {
        // Obtain position X ----
        qleft = (Double_t)fx[padmx - 1];
        qright = (Double_t)fx[padmx + 1];
        // std::cout<<qleft<<" "<<qright<<std::endl;
        if (qleft > 0 && qright > 0)
            x = GetPositionX(qmx, padmx, qleft, qright);

        // Obtain position Y ----
        qdown = fy[padmy - 1];
        qup = fy[padmy + 1];
        // if(padmy==64) std::cout << padmy << " " << qmy << " " << qdown << " " << qup << std::endl;
        // if(padmy==63) std::cout << padmy << " " << qmy << " " << qdown << " " << qup << std::endl;
        if (qdown > 0 && qup > 0)
            y = GetPositionY(qmy, padmy, qdown, qup);

        AddHitData(x, y);
    }

    if (calData)
        delete calData;
    return;
}

/* ----   Protected method to obtain the position X ---- */
Double_t R3BSofMwpc3Cal2Hit::GetPositionX(Double_t qmax, Int_t padmax, Double_t qleft, Double_t qright)
{
    Double_t a3 = TMath::Pi() * fwx / (TMath::ACosH(0.5 * (TMath::Sqrt(qmax / qleft) + TMath::Sqrt(qmax / qright))));
    // Double_t a2 = gRandom->Uniform(-fwx / 2,fwx / 2);
    Double_t a2 = (a3 / TMath::Pi()) * TMath::ATanH((TMath::Sqrt(qmax / qleft) - TMath::Sqrt(qmax / qright)) /
                                                    (2 * TMath::SinH(TMath::Pi() * fwx / a3)));

    return (-1. * padmax * fwx + (fSizeX / 2) - (fwx / 2) - a2); // Left is positive and right negative
}

/* ----   Protected method to obtain the position Y ---- */
Double_t R3BSofMwpc3Cal2Hit::GetPositionY(Double_t qmax, Int_t padmax, Double_t qdown, Double_t qup)
{
    Double_t a3 = TMath::Pi() * fwy / (TMath::ACosH(0.5 * (TMath::Sqrt(qmax / qdown) + TMath::Sqrt(qmax / qup))));
    // Double_t a2 = gRandom->Uniform(-fwy / 2, fwy / 2);
    Double_t a2 = (a3 / TMath::Pi()) * TMath::ATanH((TMath::Sqrt(qmax / qdown) - TMath::Sqrt(qmax / qup)) /
                                                    (2 * TMath::SinH(TMath::Pi() * fwy / a3)));

    return (padmax * fwy - (fSizeY / 2) + (fwy / 2) + a2);
}

/* ----   Public method Finish  ---- */
void R3BSofMwpc3Cal2Hit::Finish() {}

/* ----   Public method Reset  ---- */
void R3BSofMwpc3Cal2Hit::Reset()
{
    LOG(DEBUG) << "Clearing Mwpc3HitData Structure";
    if (fMwpcHitDataCA)
        fMwpcHitDataCA->Clear();
}

/* ----   Private method AddHitData  ---- */
R3BSofMwpcHitData* R3BSofMwpc3Cal2Hit::AddHitData(Double_t x, Double_t y)
{
    // It fills the R3BSofMwpcHitData
    TClonesArray& clref = *fMwpcHitDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofMwpcHitData(x, y);
}

ClassImp(R3BSofMwpc3Cal2Hit)
