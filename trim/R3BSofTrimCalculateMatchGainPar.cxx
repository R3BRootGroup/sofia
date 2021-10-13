#include "R3BSofTrimCalculateMatchGainPar.h"

#include "R3BSofTrimCalData.h"
#include "R3BSofTrimCalPar.h"

#include "R3BEventHeader.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "TGeoManager.h"

#include "TClonesArray.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TRandom.h"
#include "TVector3.h"

#include <iostream>
#include <stdlib.h>

// R3BSofTrimCalculateMatchGainPar: Default Constructor --------------------------
R3BSofTrimCalculateMatchGainPar::R3BSofTrimCalculateMatchGainPar()
    : FairTask("R3BSofTrimCalculateMatchGainPar", 1)
    , fNumSections(3)
    , fNumAnodes(6)
    , fNumPairsPerSection(3)
    , fCalData(NULL)
    , fCalPar(NULL)
    , fEpsilon(0.01)
    , fOutputFile(NULL)
{
    fGainMin = new TArrayF(9);
}

// R3BSofTrimCalculateMatchGainPar: Standard Constructor --------------------------
R3BSofTrimCalculateMatchGainPar::R3BSofTrimCalculateMatchGainPar(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNumSections(3)
    , fNumAnodes(6)
    , fNumPairsPerSection(3)
    , fCalData(NULL)
    , fCalPar(NULL)
    , fEpsilon(0.01)
    , fOutputFile(NULL)
{
    fGainMin = new TArrayF(9);
}

// R3BSofTrimCalculateMatchGainPar: Destructor ----------------------------------------
R3BSofTrimCalculateMatchGainPar::~R3BSofTrimCalculateMatchGainPar()
{
    if (fCalPar)
        delete fCalPar;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofTrimCalculateMatchGainPar::Init()
{

    LOG(INFO) << "R3BSofTrimCalculateMatchGainPar: Init";

    FairRootManager* rm = FairRootManager::Instance();
    if (!rm)
    {
        return kFATAL;
    }

    // --- -------------- --- //
    // --- INPUT CAL DATA --- //
    // --- -------------- --- //

    fCalData = (TClonesArray*)rm->GetObject("TrimCalData");
    if (!fCalData)
    {
        LOG(ERROR) << "R3BSofTrimCalculateMatchGainPar::Init() Couldn't get handle on TrimCalData";
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
        LOG(ERROR) << "R3BSofTrimCalculateMatchGainPar::Init() Couldn't get handle on trimCalPar container";
        return kFATAL;
    }
    else
    {
        fNumSections = fCalPar->GetNumSections();
        fNumAnodes = fCalPar->GetNumAnodes();
        LOG(INFO) << "R3BSofTrimCalculateMatchGainPar::Init() trimCalPar container found with fNumSections = "
                  << fNumSections;
    }

    // --- ---------------------- --- //
    // --- HISTOGRAMS DECLARATION --- //
    // --- ---------------------- --- //

    char name[100];
    fh2_TrimPerPair_Esub_vs_Y = new TH2D*[fNumSections * fNumPairsPerSection * 9];
    for (Int_t sec = 0; sec < fNumSections; sec++)
    {
        for (Int_t pair = 0; pair < 3; pair++)
        {
            for (Int_t k = 0; k < 9; k++)
            {
                sprintf(name, "Trim_Esub_vs_Y_Sec%i_Pair%i_k%i", sec + 1, pair + 1, k + 1);
                fh2_TrimPerPair_Esub_vs_Y[k + pair * 9 + sec * 9 * fNumPairsPerSection] =
                    new TH2D(name, name, 640, -0.4, 0.4, 1000, 0, 10000);
                fh2_TrimPerPair_Esub_vs_Y[k + pair * 9 + sec * 9 * fNumPairsPerSection]->GetXaxis()->SetTitle(
                    "Ratio between EsubDiff and EsubSum <=> Y [channels]");
                fh2_TrimPerPair_Esub_vs_Y[k + pair * 9 + sec * 9 * fNumPairsPerSection]->GetYaxis()->SetTitle(
                    "EsubSum [channels]");
                fh2_TrimPerPair_Esub_vs_Y[k + pair * 9 + sec * 9 * fNumPairsPerSection]->ls();
            }
        }
    }

    return kSUCCESS;
}

// -----   Public method ReInit  --------------------------------------------
InitStatus R3BSofTrimCalculateMatchGainPar::ReInit() { return kSUCCESS; }

// -----   Public method Exec   --------------------------------------------
void R3BSofTrimCalculateMatchGainPar::Exec(Option_t* opt)
{

    Int_t iSec, iAnode, iPair;
    Double_t Esub[fNumSections * fNumAnodes];
    Double_t EsubDown;
    Double_t EsubUp;
    UInt_t mult[fNumSections * fNumAnodes];
    Double_t Esum, Ediff, gain_match_local;

    for (UShort_t anode = 0; anode < fNumSections * fNumAnodes; anode++)
        mult[anode] = 0;

    // --- ------------------------------ --- //
    // --- LOOP OVER CAL DATA FOR SofTrim --- //
    // --- ------------------------------ --- //
    UInt_t nHits = fCalData->GetEntries();
    if (!nHits)
        return;
    for (Int_t ihit = 0; ihit < nHits; ihit++)
    {
        R3BSofTrimCalData* hit = (R3BSofTrimCalData*)fCalData->At(ihit);
        iSec = hit->GetSecID() - 1;
        iAnode = hit->GetAnodeID() - 1;
        mult[iAnode + iSec * fNumAnodes]++;
        Esub[iAnode + iSec * fNumAnodes] = (Double_t)hit->GetEnergySub();
    } // end of loop over the cal data

    // --- --------------------------------------------- --- //
    // --- FILL THE HISTOGRAMS WITH CLEAN DATA (mult==1) --- //
    // --- --------------------------------------------- --- //
    for (Int_t section = 0; section < fNumSections; section++)
    {
        for (Int_t pair = 0; pair < 3; pair++)
        {
            if ((mult[pair * 2 + section * fNumAnodes] == mult[pair * 2 + 1 + section * fNumAnodes]) &&
                (mult[pair * 2 + section * fNumAnodes] == 1))
            {
                for (Int_t k = 0; k < 9; k++)
                {
                    gain_match_local = GetGainMin(pair + section * 3) + (Double_t)k * (Double_t)fEpsilon;
                    EsubDown = (Double_t)Esub[pair * 2 + 1 + section * fNumAnodes];
                    EsubUp = (Double_t)Esub[pair * 2 + section * fNumAnodes];
                    Esum = EsubUp + gain_match_local * (Double_t)EsubDown;
                    Ediff = (Double_t)EsubUp - gain_match_local * (Double_t)EsubDown;
                    if (Esum != 0)
                    {
                        fh2_TrimPerPair_Esub_vs_Y[k + pair * 9 + section * 9 * 3]->Fill(Ediff / Esum, 0.1 * Esum);
                    }
                }
            } // end of check mult up = mult down
        }     // end of loop over the pairs
    }         // end of loop over the sections
}

// ---- Public method Reset   --------------------------------------------------
void R3BSofTrimCalculateMatchGainPar::Reset() {}

void R3BSofTrimCalculateMatchGainPar::FinishEvent() {}

// ---- Public method Finish   --------------------------------------------------
void R3BSofTrimCalculateMatchGainPar::FinishTask()
{
    PlotEvsY();
    fCalPar->printParams();
}

// ------------------------------
void R3BSofTrimCalculateMatchGainPar::PlotEvsY()
{
    LOG(INFO) << "R3BSofTrimCalculateMatchGainPar: PlotEvsY()";
    char name[100];

    TCanvas* can[9];
    for (int i = 0; i < 9; i++)
    {
        sprintf(name, "Pair%i", i + 1);
        can[i] = new TCanvas(name, name, 0, 0, 800, 700);
        can[i]->Divide(3, 3);
        for (int j = 0; j < 9; j++)
        {
            can[i]->cd(j + 1);
            gPad->SetGridy();
            fh2_TrimPerPair_Esub_vs_Y[j + 9 * i]->Draw("colz");
            fh2_TrimPerPair_Esub_vs_Y[j + 9 * i]->SetDirectory(0);
            fh2_TrimPerPair_Esub_vs_Y[j + 9 * i]->Write();
        }
        can[i]->Write();
    }

    return;
}

ClassImp(R3BSofTrimCalculateMatchGainPar)
