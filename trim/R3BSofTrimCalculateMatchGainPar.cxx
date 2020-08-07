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
    , fMinStatistics(0)
    , fCalData(NULL)
    , fCalPar(NULL)
    , fEpsilon(0.01)
    , fGainMin(0.8)
    , fGainMax(1.2)
    , fOutputFile(NULL)
{
    fNumPairsPerSection = fNumAnodes / 2;
    fNumHistosPerPair = (Int_t)((fGainMax - fGainMin) / fEpsilon);
}

// R3BSofTrimCalculateMatchGainPar: Standard Constructor --------------------------
R3BSofTrimCalculateMatchGainPar::R3BSofTrimCalculateMatchGainPar(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNumSections(0)
    , fNumAnodes(0)
    , fMinStatistics(0)
    , fCalData(NULL)
    , fCalPar(NULL)
    , fEpsilon(0.01)
    , fGainMin(0.8)
    , fGainMax(1.2)
    , fOutputFile(NULL)
{
    fNumPairsPerSection = fNumAnodes / 2;
    fNumHistosPerPair = (Int_t)((fGainMax - fGainMin) / fEpsilon);
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

    // --- ----------------- --- //
    // --- INPUT MAPPED DATA --- //
    // --- ------------------ --- //

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
    fh2_TrimPerPair_Esub_vs_Y = new TH2D*[fNumSections * fNumPairsPerSection * fNumHistosPerPair];
    for (Int_t sec = 0; sec < fNumSections; sec++)
    {
        for (Int_t pair = 0; pair < (fNumAnodes / 2); pair++)
        {
            for (Int_t k = 0; k < fNumHistosPerPair; k++)
            {
                sprintf(name, "Trim_Esub_vs_Y_Sec%i_Pair%i_k%i", sec + 1, pair + 1, k + 1);
                fh2_TrimPerPair_Esub_vs_Y[k + pair * fNumHistosPerPair + sec * fNumHistosPerPair * fNumAnodes / 2] =
                    new TH2D(name, name, 1600, -0.4, 0.4, 1000, 0, 20000);
                fh2_TrimPerPair_Esub_vs_Y[k + pair * fNumHistosPerPair + sec * fNumHistosPerPair * fNumAnodes / 2]
                    ->GetXaxis()
                    ->SetTitle("Ratio between EsubDiff and EsubSum <=> Y [channels]");
                fh2_TrimPerPair_Esub_vs_Y[k + pair * fNumHistosPerPair + sec * fNumHistosPerPair * fNumAnodes / 2]
                    ->GetYaxis()
                    ->SetTitle("EsubSum [channels]");
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
    Float_t EsubDown[fNumSections * fNumPairsPerSection];
    Float_t EsubUp[fNumSections * fNumPairsPerSection];
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
    } // end of loop over the cal data

    // --- --------------------------------------------- --- //
    // --- FILL THE HISTOGRAMS WITH CLEAN DATA (mult==1) --- //
    // --- --------------------------------------------- --- //
    for (Int_t section = 0; section < fNumSections; section++)
    {
        for (Int_t pair = 0; pair < fNumPairsPerSection; pair++)
        {
            if ((mult[pair * 2 + section * fNumAnodes] == mult[pair * 2 + 1 + section * fNumAnodes]) &&
                (mult[pair * 2 + section * fNumAnodes] == 1))
            {
                for (Int_t k = 0; k < fNumHistosPerPair; k++)
                {
                    gain_match_local = (Double_t)fGainMin + (Double_t)k * (Double_t)fEpsilon;
                    Esum = (Double_t)EsubUp[pair + section * fNumPairsPerSection] +
                           gain_match_local * EsubDown[pair + section * fNumPairsPerSection];
                    Ediff = (Double_t)EsubUp[pair + section * fNumPairsPerSection] -
                            gain_match_local * EsubDown[pair + section * fNumPairsPerSection];
                    if (Esum != 0)
                        fh2_TrimPerPair_Esub_vs_Y[k + pair * fNumHistosPerPair +
                                                  section * fNumHistosPerPair * fNumPairsPerSection]
                            ->Fill(Ediff / Esum, Esum);
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
    LOG(INFO) << "R3BSofTrimCalculateMatchGainPar: CalculateGainMatchingParams()";
    char name[100];
    TH1D** h1_ProjectionY = new TH1D*[fNumSections * fNumPairsPerSection * fNumHistosPerPair];
    TF1** fit_max = new TF1*[fNumSections * fNumPairsPerSection * fNumHistosPerPair];
    Double_t Bmax, Xmax, Sigma, Min;
    TGraph** gr = new TGraph*[fNumSections * fNumPairsPerSection];
    for (Int_t section = 0; section < fNumSections; section++)
    {
        for (Int_t pair = 0; pair < fNumPairsPerSection; pair++)
        {
            sprintf(name, "Sigma_S%iP%i", section + 1, pair + 1);
            gr[pair + section * fNumPairsPerSection] = new TGraph();
            gr[pair + section * fNumPairsPerSection]->SetName(name);
            gr[pair + section * fNumPairsPerSection]->SetTitle(name);
            Min = 1000.;
            for (Int_t k = 0; k < fNumHistosPerPair; k++)
            {
                if (fh2_TrimPerPair_Esub_vs_Y[k + pair * fNumHistosPerPair +
                                              section * fNumHistosPerPair * fNumPairsPerSection]
                        ->Integral() > 0)
                {

                    fh2_TrimPerPair_Esub_vs_Y[k + pair * fNumHistosPerPair +
                                              section * fNumHistosPerPair * fNumPairsPerSection]
                        ->Write();
                    sprintf(name,
                            "EvsY_S%iP%i_k%f_pjY",
                            section + 1,
                            pair + 1,
                            (Double_t)fGainMin + (Double_t)k * (Double_t)fEpsilon);
                    fh2_TrimPerPair_Esub_vs_Y[k + pair * fNumHistosPerPair +
                                              section * fNumHistosPerPair * fNumPairsPerSection]
                        ->ProjectionY(name);

                    h1_ProjectionY[k + pair * fNumHistosPerPair + section * fNumHistosPerPair * fNumPairsPerSection] =
                        (TH1D*)gDirectory->FindObject(name);
                    h1_ProjectionY[k + pair * fNumHistosPerPair + section * fNumHistosPerPair * fNumPairsPerSection]
                        ->Write();

                    Bmax =
                        h1_ProjectionY[k + pair * fNumHistosPerPair + section * fNumHistosPerPair * fNumPairsPerSection]
                            ->GetMaximumBin();
                    Xmax =
                        h1_ProjectionY[k + pair * fNumHistosPerPair + section * fNumHistosPerPair * fNumPairsPerSection]
                            ->GetBinCenter(Bmax);
                    fit_max[k + pair * fNumHistosPerPair + section * fNumHistosPerPair * fNumPairsPerSection] =
                        new TF1("fit_gaus", "gaus", Xmax - 200, Xmax + 200);
                    h1_ProjectionY[k + pair * fNumHistosPerPair + section * fNumHistosPerPair * fNumPairsPerSection]
                        ->Fit(fit_max[k + pair * fNumHistosPerPair + section * fNumHistosPerPair * fNumPairsPerSection],
                              "R");
                    fit_max[k + pair * fNumHistosPerPair + section * fNumHistosPerPair * fNumPairsPerSection]->Write();
                    Sigma = fit_max[k + pair * fNumHistosPerPair + section * fNumHistosPerPair * fNumPairsPerSection]
                                ->GetParameter(2);
                    if (Sigma < Min)
                    {
                        Min = Sigma;
                        fCalPar->SetEnergyMatchGain(
                            (Double_t)fGainMin + (Double_t)k * (Double_t)fEpsilon, section + 1, (2 * pair) + 1);
                        fCalPar->SetEnergyMatchGain(1., section + 1, (2 * pair) + 2);
                    }
                    gr[pair + section * fNumPairsPerSection]->SetPoint(
                        k, (Double_t)fGainMin + (Double_t)k * (Double_t)fEpsilon, Sigma);
                }
            }
            if (gr[pair + section * fNumPairsPerSection]->GetN() > 0)
            {
                gr[pair + section * fNumPairsPerSection]->Write();
            }
        }
    }

    fCalPar->setChanged();
    return;
}

ClassImp(R3BSofTrimCalculateMatchGainPar)
