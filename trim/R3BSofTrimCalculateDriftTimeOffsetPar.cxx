#include "R3BSofTrimCalculateDriftTimeOffsetPar.h"

#include "R3BSofMwpcHitData.h"
#include "R3BSofTrimCalData.h"
#include "R3BSofTrimCalPar.h"

#include "R3BEventHeader.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "TGeoManager.h"

#include "TClonesArray.h"
#include "TF1.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TRandom.h"
#include "TVector3.h"

#include <iostream>
#include <stdlib.h>

// R3BSofTrimCalculateDriftTimeOffsetPar: Default Constructor --------------------------
R3BSofTrimCalculateDriftTimeOffsetPar::R3BSofTrimCalculateDriftTimeOffsetPar()
    : FairTask("R3BSofTrimCalculateDriftTimeOffsetPar", 1)
    , fNumSections(3)
    , fNumAnodes(6)
    , fMinStatistics(0)
    , fTrimCalData(NULL)
    , fMwpc0HitData(NULL)
    , fMwpc1HitData(NULL)
    , fCalPar(NULL)
    , fMwpc0OffsetX(0)
    , fMwpc1OffsetX(0)
    , fDistMwpc0Anode1(0)
    , fDistMwpc0Mwpc1(0)
    , fWidthAnode(25)       // mm
    , fDistInterSection(50) // mm FIX ME: exact value ? (2 edge anodes + stripper + field cage)
    , fDriftVelocity(45)    // mm/micros
    , fOutputFile(NULL)
{
}

// R3BSofTrimCalculateDriftTimeOffsetPar: Standard Constructor --------------------------
R3BSofTrimCalculateDriftTimeOffsetPar::R3BSofTrimCalculateDriftTimeOffsetPar(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNumSections(0)
    , fNumAnodes(0)
    , fMinStatistics(0)
    , fTrimCalData(NULL)
    , fMwpc0HitData(NULL)
    , fMwpc1HitData(NULL)
    , fCalPar(NULL)
    , fMwpc0OffsetX(0)
    , fMwpc1OffsetX(0)
    , fDistMwpc0Anode1(0)
    , fDistMwpc0Mwpc1(0)
    , fWidthAnode(25)       // mm
    , fDistInterSection(50) // mm FIX ME: exact value ? (2 edge anodes + stripper + field cage)
    , fDriftVelocity(45)    // mm/micros
    , fOutputFile(NULL)
{
}

// R3BSofTrimCalculateDriftTimeOffsetPar: Destructor ----------------------------------------
R3BSofTrimCalculateDriftTimeOffsetPar::~R3BSofTrimCalculateDriftTimeOffsetPar()
{
    if (fCalPar)
        delete fCalPar;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofTrimCalculateDriftTimeOffsetPar::Init()
{

    LOG(INFO) << "R3BSofTrimCalculateDriftTimeOffsetPar: Init";

    FairRootManager* rm = FairRootManager::Instance();
    if (!rm)
    {
        return kFATAL;
    }

    // --- ------------------- --- //
    // --- INPUT TRIM CAL DATA --- //
    // --- ------------------- --- //

    fTrimCalData = (TClonesArray*)rm->GetObject("TrimCalData");
    if (!fTrimCalData)
    {
        LOG(ERROR) << "R3BSofTrimCalculateDriftTimeOffsetPar::Init() Couldn't get handle on TrimCalData";
        return kFATAL;
    }

    // --- -------------------- --- //
    // --- INPUT MWPC0 HIT DATA --- //
    // --- -------------------- --- //

    fMwpc0HitData = (TClonesArray*)rm->GetObject("Mwpc0HitData");
    if (!fMwpc0HitData)
    {
        LOG(ERROR) << "R3BSofTrimCalculateDriftTimeOffsetPar::Init() Couldn't get handle on Mwpc0HitData";
        return kFATAL;
    }

    // --- -------------------- --- //
    // --- INPUT MWPC1 HIT DATA --- //
    // --- -------------------- --- //

    fMwpc1HitData = (TClonesArray*)rm->GetObject("Mwpc1HitData");
    if (!fMwpc1HitData)
    {
        LOG(ERROR) << "R3BSofTrimCalculateDriftTimeOffsetPar::Init() Couldn't get handle on Mwpc1HitData";
        return kFATAL;
    }

    // --- ------------------------------------------ --- //
    // --- SOF TRIM TRIANGLE CAL PARAMETERS CONTAINER --- //
    // --- ------------------------------------------ --- //

    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        return kFATAL;
    }

    fCalPar = (R3BSofTrimCalPar*)rtdb->getContainer("trimCalPar");
    if (!fCalPar)
    {
        LOG(ERROR) << "R3BSofTrimCalculateDriftTimeOffsetPar::Init() Couldn't get handle on trimCalPar container";
        return kFATAL;
    }
    else
    {
        fNumSections = fCalPar->GetNumSections();
        fNumAnodes = fCalPar->GetNumAnodes();
        LOG(INFO) << "R3BSofTrimCalculateDriftTimeOffsetPar::Init() trimCalPar container found with fNumSections = "
                  << fNumSections;
    }

    // --- ---------------------- --- //
    // --- HISTOGRAMS DECLARATION --- //
    // --- ---------------------- --- //

    char name[100];
    fh1_DeltaDT = new TH1D*[fNumSections * fNumAnodes];
    for (Int_t section = 0; section < fNumSections; section++)
    {
        for (Int_t anode = 0; anode < fNumAnodes; anode++)
        {
            sprintf(name, "DeltaDT_S%iA%i", section + 1, anode + 1);
            fh1_DeltaDT[anode + section * fNumAnodes] = new TH1D(name, name, 5000, -2500, 2500);
            fh1_DeltaDT[anode + section * fNumAnodes]->GetXaxis()->SetTitle(
                "Delta Drift Time [channels, 100 ps TDC resolution]");
        }
    }

    return kSUCCESS;
}

// -----   Public method ReInit  --------------------------------------------
InitStatus R3BSofTrimCalculateDriftTimeOffsetPar::ReInit() { return kSUCCESS; }

// -----   Public method Exec   --------------------------------------------
void R3BSofTrimCalculateDriftTimeOffsetPar::Exec(Option_t* opt)
{

    Int_t iSec, iAnode;
    Double_t X0, X1, DTraw, Xanode, Zanode;
    UInt_t nHits;

    // --- -------------- --- //
    // --- MWPC0 HIT DATA --- //
    // --- -------------- --- //
    // only event with one single hit are considered
    nHits = fMwpc0HitData->GetEntries();
    if (nHits != 1)
        return;
    R3BSofMwpcHitData* hitMwpc0 = (R3BSofMwpcHitData*)fMwpc0HitData->At(0);
    X0 = hitMwpc0->GetX() + fMwpc0OffsetX;

    // --- -------------- --- //
    // --- MWPC1 HIT DATA --- //
    // --- -------------- --- //
    // only event with empty target (beam data with minimum straggling) and one single event are considered
    nHits = fMwpc1HitData->GetEntries();
    if (nHits != 1)
        return;
    R3BSofMwpcHitData* hitMwpc1 = (R3BSofMwpcHitData*)fMwpc1HitData->At(0);
    X1 = hitMwpc1->GetX() + fMwpc1OffsetX;

    // --- --------------------------------------- --- //
    // --- LOOP OVER TRIANGLE CAL HITS FOR SofTrim --- //
    // --- --------------------------------------- --- //
    nHits = fTrimCalData->GetEntries();
    if (!nHits)
        return;
    for (Int_t ihit = 0; ihit < nHits; ihit++)
    {
        R3BSofTrimCalData* hit = (R3BSofTrimCalData*)fTrimCalData->At(ihit);
        iSec = hit->GetSecID() - 1;
        iAnode = hit->GetAnodeID() - 1;
        DTraw = hit->GetDriftTimeRaw();
        Zanode = fDistMwpc0Anode1 + (iAnode + iSec * fNumAnodes) * fWidthAnode + iSec * fDistInterSection;
        Xanode = Zanode * (X1 - X0) / fDistMwpc0Mwpc1;
        fh1_DeltaDT[iAnode + iSec * fNumAnodes]->Fill(10000. * Xanode / fDriftVelocity -
                                                      DTraw); // 10000 : mm/micros -> mm/100ps
    }                                                         // end of loop over the mapped data
}

// ---- Public method Reset   --------------------------------------------------
void R3BSofTrimCalculateDriftTimeOffsetPar::Reset() {}

void R3BSofTrimCalculateDriftTimeOffsetPar::FinishEvent() {}

// ---- Public method Finish   --------------------------------------------------
void R3BSofTrimCalculateDriftTimeOffsetPar::FinishTask()
{
    CalculateOffsets();
    fCalPar->printParams();
}

// ------------------------------
void R3BSofTrimCalculateDriftTimeOffsetPar::CalculateOffsets()
{
    LOG(INFO) << "R3BSofTrimCalculateDriftTimeOffsetPar: CalculateOffsets()";

    Double_t BinMax, X_BinMax;
    TF1** fit_max = new TF1*[fNumSections * fNumAnodes];

    for (Int_t section = 0; section < fNumSections; section++)
    {
        for (Int_t anode = 0; anode < fNumAnodes; anode++)
        {
            if (fh1_DeltaDT[anode + section * fNumAnodes]->Integral() > fMinStatistics)
            {
                BinMax = fh1_DeltaDT[anode + section * fNumAnodes]->GetMaximumBin();
                X_BinMax = fh1_DeltaDT[anode + section * fNumAnodes]->GetBinCenter(BinMax);
                fit_max[anode + section * fNumAnodes] = new TF1("fit_gaus", "gaus", X_BinMax - 500, X_BinMax + 500);
                fh1_DeltaDT[anode + section * fNumAnodes]->Fit(fit_max[anode + section * fNumAnodes], "R");
                fCalPar->SetDriftTimeOffset(
                    section + 1, anode + 1, fit_max[anode + section * fNumAnodes]->GetParameter(1));
            }
        }
    }

    fCalPar->setChanged();
    return;
}

ClassImp(R3BSofTrimCalculateDriftTimeOffsetPar)
