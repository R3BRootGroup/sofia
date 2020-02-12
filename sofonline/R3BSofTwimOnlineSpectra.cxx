// ------------------------------------------------------------
// -----                  R3BSofTwimOnlineSpectra           -----
// -----    Created 29/09/19  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofTwimOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BSofTwimCalData.h"
#include "R3BSofTwimHitData.h"
#include "R3BSofTwimMappedData.h"
#include "THttpServer.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "TCanvas.h"
#include "TFolder.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TVector3.h"

#include "TClonesArray.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TMath.h"
#include "TRandom.h"
#include <array>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

R3BSofTwimOnlineSpectra::R3BSofTwimOnlineSpectra()
    : FairTask("SofTwimOnlineSpectra", 1)
    , fMappedItemsTwim(NULL)
    , fCalItemsTwim(NULL)
    , fHitItemsTwim(NULL)
    , fNEvents(0)
{
}

R3BSofTwimOnlineSpectra::R3BSofTwimOnlineSpectra(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fMappedItemsTwim(NULL)
    , fCalItemsTwim(NULL)
    , fHitItemsTwim(NULL)
    , fNEvents(0)
{
}

R3BSofTwimOnlineSpectra::~R3BSofTwimOnlineSpectra()
{
    LOG(INFO) << "R3BSofTwimOnlineSpectra::Delete instance";
    if (fMappedItemsTwim)
        delete fMappedItemsTwim;
    if (fCalItemsTwim)
        delete fCalItemsTwim;
    if (fHitItemsTwim)
        delete fHitItemsTwim;
}

InitStatus R3BSofTwimOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofTwimOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTwimOnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // get access to mapped data of the TWIM
    fMappedItemsTwim = (TClonesArray*)mgr->GetObject("TwimMappedData");
    if (!fMappedItemsTwim)
    {
        return kFATAL;
    }

    // get access to cal data of the TWIM
    fCalItemsTwim = (TClonesArray*)mgr->GetObject("TwimCalData");
    if (!fCalItemsTwim)
        LOG(WARNING) << "R3BSofTwimOnlineSpectra: TwimCalData not found";

    // get access to hit data of the TWIM
    fHitItemsTwim = (TClonesArray*)mgr->GetObject("TwimHitData");
    if (!fHitItemsTwim)
        LOG(WARNING) << "R3BSofTwimOnlineSpectra: TwimHitData not found";

    // Create histograms for detectors
    char Name1[255];
    char Name2[255];

    // TWIM: Map data for E
    for (Int_t i = 0; i < NbSections; i++)
    {
        sprintf(Name1, "Twim_Emap_Sec_%d", i + 1);
        sprintf(Name2, "Section %d", i + 1);
        cTwimMap_E[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);
        cTwimMap_E[i]->Divide(4, 4);
    }

    for (Int_t i = 0; i < NbSections; i++)
    {
        for (Int_t j = 0; j < NbAnodes; j++)
        {
            sprintf(Name1, "fh1_twim_Emap_sec%d_a%d", i + 1, j + 1);
            sprintf(Name2, "Sec %d:Anode %d", i + 1, j + 1);
            fh1_twimmap_E[i][j] = new TH1F(Name1, Name2, 8192, 0, 8192);
            fh1_twimmap_E[i][j]->GetXaxis()->SetTitle("Energy [channels]");
            fh1_twimmap_E[i][j]->GetYaxis()->SetTitle("Counts");
            fh1_twimmap_E[i][j]->GetYaxis()->SetTitleOffset(1.1);
            fh1_twimmap_E[i][j]->GetXaxis()->CenterTitle(true);
            fh1_twimmap_E[i][j]->GetYaxis()->CenterTitle(true);
            fh1_twimmap_E[i][j]->GetXaxis()->SetLabelSize(0.045);
            fh1_twimmap_E[i][j]->GetXaxis()->SetTitleSize(0.045);
            fh1_twimmap_E[i][j]->GetYaxis()->SetLabelSize(0.045);
            fh1_twimmap_E[i][j]->GetYaxis()->SetTitleSize(0.045);
            fh1_twimmap_E[i][j]->SetFillColor(31);
            cTwimMap_E[i]->cd(j + 1);
            fh1_twimmap_E[i][j]->Draw("");
        }
    }

    // TWIM: Map data for E vs DT
    for (Int_t i = 0; i < NbSections; i++)
    {
        sprintf(Name1, "Twim_EvsDT_Sec_%d", i + 1);
        sprintf(Name2, "Section %d", i + 1);
        cTwimMap_EvsDT[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);
        cTwimMap_EvsDT[i]->Divide(4, 4);
        sprintf(Name1, "Twim_DTvsDT_Sec_%d", i + 1);
        sprintf(Name2, "DTvsDT Section %d", i + 1);
        cTwim_DTvsDT[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);
        cTwim_DTvsDT[i]->Divide(4, 4);
    }

    char Name3[255];
    char Name4[255];
    cTwimMap_EsumvsDT = new TCanvas("Twim_EsumvsDT", "Twim_EsumvsDT", 10, 10, 800, 700);
    if (NbSections > 1)
        cTwimMap_EsumvsDT->Divide(2, 2);

    for (Int_t i = 0; i < NbSections; i++)
    {
        for (Int_t j = 0; j < NbAnodes; j++)
        {
            sprintf(Name1, "fh1_twim_EvsDT_sec%d_a%d", i + 1, j + 1);
            sprintf(Name2, "Sec %d:Anode %d", i + 1, j + 1);
            fh2_twim_EneRawVsDriftTime[i][j] = new TH2F(Name1, Name2, 500, 0, 8192, 800, 1, 40001);
            fh2_twim_EneRawVsDriftTime[i][j]->GetXaxis()->SetTitle("Energy [channels]");
            fh2_twim_EneRawVsDriftTime[i][j]->GetYaxis()->SetTitle("Drift time [channels]");
            fh2_twim_EneRawVsDriftTime[i][j]->GetYaxis()->SetTitleOffset(1.1);
            fh2_twim_EneRawVsDriftTime[i][j]->GetXaxis()->CenterTitle(true);
            fh2_twim_EneRawVsDriftTime[i][j]->GetYaxis()->CenterTitle(true);
            fh2_twim_EneRawVsDriftTime[i][j]->GetXaxis()->SetLabelSize(0.045);
            fh2_twim_EneRawVsDriftTime[i][j]->GetXaxis()->SetTitleSize(0.045);
            fh2_twim_EneRawVsDriftTime[i][j]->GetYaxis()->SetLabelSize(0.045);
            fh2_twim_EneRawVsDriftTime[i][j]->GetYaxis()->SetTitleSize(0.045);
            cTwimMap_EvsDT[i]->cd(j + 1);
            fh2_twim_EneRawVsDriftTime[i][j]->Draw("col");
        }

        for (Int_t j = 0; j < NbAnodes - 1; j++)
        {
            sprintf(Name1, "fh2_twim_DTvsDT_sec%d_a%d", i + 1, j);
            sprintf(Name2, "Sec %d: DT_%d vs DT_%d", i + 1, j + 1, j + 2);
            fh2_twim_DTvsDT[i][j] = new TH2F(Name1, Name2, 800, 1, 40001, 800, 1, 40001);
            sprintf(Name1, "Drift time %d [channels]", j + 1);
            sprintf(Name2, "Drift time %d [channels]", j + 2);
            fh2_twim_DTvsDT[i][j]->GetXaxis()->SetTitle(Name1);
            fh2_twim_DTvsDT[i][j]->GetYaxis()->SetTitle(Name2);
            fh2_twim_DTvsDT[i][j]->GetYaxis()->SetTitleOffset(1.1);
            fh2_twim_DTvsDT[i][j]->GetXaxis()->CenterTitle(true);
            fh2_twim_DTvsDT[i][j]->GetYaxis()->CenterTitle(true);
            fh2_twim_DTvsDT[i][j]->GetXaxis()->SetLabelSize(0.045);
            fh2_twim_DTvsDT[i][j]->GetXaxis()->SetTitleSize(0.045);
            fh2_twim_DTvsDT[i][j]->GetYaxis()->SetLabelSize(0.045);
            fh2_twim_DTvsDT[i][j]->GetYaxis()->SetTitleSize(0.045);
            cTwim_DTvsDT[i]->cd(j + 1);
            fh2_twim_DTvsDT[i][j]->Draw("col");
        }

        sprintf(Name3, "fh2_Twim_EsumvsDT_sec%d", i + 1);
        sprintf(Name4, "Twim: Esum vs DT for sec%d", i + 1);
        fh2_twim_EneRawSumVsDriftTime[i] = new TH2F(Name3, Name4, 1200, 1, 25000, 2192, 0, 8192);
        fh2_twim_EneRawSumVsDriftTime[i]->GetXaxis()->SetTitle("Drift time [channels]");
        fh2_twim_EneRawSumVsDriftTime[i]->GetYaxis()->SetTitle("Energy Sum [channels]");
        fh2_twim_EneRawSumVsDriftTime[i]->GetYaxis()->SetTitleOffset(1.1);
        fh2_twim_EneRawSumVsDriftTime[i]->GetXaxis()->CenterTitle(true);
        fh2_twim_EneRawSumVsDriftTime[i]->GetYaxis()->CenterTitle(true);
        fh2_twim_EneRawSumVsDriftTime[i]->GetXaxis()->SetLabelSize(0.045);
        fh2_twim_EneRawSumVsDriftTime[i]->GetXaxis()->SetTitleSize(0.045);
        fh2_twim_EneRawSumVsDriftTime[i]->GetYaxis()->SetLabelSize(0.045);
        fh2_twim_EneRawSumVsDriftTime[i]->GetYaxis()->SetTitleSize(0.045);
        if (NbSections > 1)
            cTwimMap_EsumvsDT->cd(i + 1);
        else
            cTwimMap_EsumvsDT->cd();
        fh2_twim_EneRawSumVsDriftTime[i]->Draw("col");
    }

    // TWIM: Map data for DT
    for (Int_t i = 0; i < NbSections; i++)
    {
        sprintf(Name1, "Twim_DTmap_Sec_%d", i + 1);
        sprintf(Name2, "Section %d", i + 1);
        cTwimMap_DT[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);
        cTwimMap_DT[i]->Divide(4, 4);
    }

    for (Int_t i = 0; i < NbSections; i++)
    {
        for (Int_t j = 0; j < NbAnodes; j++)
        {
            sprintf(Name1, "fh1_twim_DTmap_sec%d_a%d", i + 1, j + 1);
            sprintf(Name2, "Sec %d:Anode %d", i + 1, j + 1);
            fh1_twimmap_DT[i][j] = new TH1F(Name1, Name2, 6000, 1, 40001);
            fh1_twimmap_DT[i][j]->GetXaxis()->SetTitle("Drift time [channels]");
            fh1_twimmap_DT[i][j]->GetYaxis()->SetTitle("Counts");
            fh1_twimmap_DT[i][j]->GetYaxis()->SetTitleOffset(1.1);
            fh1_twimmap_DT[i][j]->GetXaxis()->CenterTitle(true);
            fh1_twimmap_DT[i][j]->GetYaxis()->CenterTitle(true);
            fh1_twimmap_DT[i][j]->GetXaxis()->SetLabelSize(0.045);
            fh1_twimmap_DT[i][j]->GetXaxis()->SetTitleSize(0.045);
            fh1_twimmap_DT[i][j]->GetYaxis()->SetLabelSize(0.045);
            fh1_twimmap_DT[i][j]->GetYaxis()->SetTitleSize(0.045);
            fh1_twimmap_DT[i][j]->SetFillColor(31);
            cTwimMap_DT[i]->cd(j + 1);
            fh1_twimmap_DT[i][j]->Draw("");
        }
    }

    // TWIM: Map data for multiplicities
    TCanvas* cTwim_Mult[NbSections];
    for (Int_t i = 0; i < NbSections; i++)
    {
        sprintf(Name1, "Twim_multiplicity_Sec_%d", i + 1);
        sprintf(Name2, "TWIM: Multiplicity per anode in section %d", i + 1);
        cTwim_Mult[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);
        sprintf(Name1, "fh1_Twim_mult_sec_%d", i + 1);
        sprintf(Name2, "Twim: Multiplicity section %d", i + 1);
        fh1_Twimmap_mult[i] = new TH1F(Name1, Name2, NbAnodes + 1, 0, NbAnodes + 1);
        fh1_Twimmap_mult[i]->GetXaxis()->SetTitle("Anode");
        fh1_Twimmap_mult[i]->GetYaxis()->SetTitle("Counts");
        fh1_Twimmap_mult[i]->GetYaxis()->SetTitleOffset(1.1);
        fh1_Twimmap_mult[i]->GetXaxis()->CenterTitle(true);
        fh1_Twimmap_mult[i]->GetYaxis()->CenterTitle(true);
        fh1_Twimmap_mult[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_Twimmap_mult[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_Twimmap_mult[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_Twimmap_mult[i]->GetYaxis()->SetTitleSize(0.045);
        fh1_Twimmap_mult[i]->SetFillColor(31);
        fh1_Twimmap_mult[i]->Draw("");
    }

    // TWIM: Map data for E in the first (anodes from 0 to 7) and final sections (anodes from 8 to 15)
    cTwimMap_ESum = new TCanvas("twim_ESum_1,2", "twim_ESum_1,2", 10, 10, 800, 700);
    cTwimMap_ESum->Divide(1, 2);
    cTwimMap_ESum->cd(1);
    fh1_twim_ESum[0] = new TH1F("fh1_twim_ESum1", "twim:ESum:first", 8192, 0, 8192);
    fh1_twim_ESum[0]->GetXaxis()->SetTitle("Energy [channels]");
    fh1_twim_ESum[0]->GetYaxis()->SetTitle("Counts");
    fh1_twim_ESum[0]->GetYaxis()->SetTitleOffset(1.1);
    fh1_twim_ESum[0]->GetXaxis()->CenterTitle(true);
    fh1_twim_ESum[0]->GetYaxis()->CenterTitle(true);
    fh1_twim_ESum[0]->GetXaxis()->SetLabelSize(0.045);
    fh1_twim_ESum[0]->GetXaxis()->SetTitleSize(0.045);
    fh1_twim_ESum[0]->GetYaxis()->SetLabelSize(0.045);
    fh1_twim_ESum[0]->GetYaxis()->SetTitleSize(0.045);
    fh1_twim_ESum[0]->SetFillColor(31);
    fh1_twim_ESum[0]->Draw("");
    cTwimMap_ESum->cd(2);
    fh1_twim_ESum[1] = new TH1F("fh1_twim_ESum2", "twim:ESum:Second", 8192, 0, 8192);
    fh1_twim_ESum[1]->GetXaxis()->SetTitle("Energy [channels]");
    fh1_twim_ESum[1]->GetYaxis()->SetTitle("Counts");
    fh1_twim_ESum[1]->GetYaxis()->SetTitleOffset(1.1);
    fh1_twim_ESum[1]->GetXaxis()->CenterTitle(true);
    fh1_twim_ESum[1]->GetYaxis()->CenterTitle(true);
    fh1_twim_ESum[1]->GetXaxis()->SetLabelSize(0.045);
    fh1_twim_ESum[1]->GetXaxis()->SetTitleSize(0.045);
    fh1_twim_ESum[1]->GetYaxis()->SetLabelSize(0.045);
    fh1_twim_ESum[1]->GetYaxis()->SetTitleSize(0.045);
    fh1_twim_ESum[1]->SetFillColor(31);
    fh1_twim_ESum[1]->Draw("");

    // TWIM: Map data for Esum
    cTwimMap_ESum1 = new TCanvas("twim_ESum", "twim_ESum", 10, 10, 800, 700);
    fh1_twim_ESum[2] = new TH1F("fh1_twim_ESum", "twim:ESum", 8192, 0, 8192);
    fh1_twim_ESum[2]->GetXaxis()->SetTitle("Energy [channels]");
    fh1_twim_ESum[2]->GetYaxis()->SetTitle("Counts");
    fh1_twim_ESum[2]->GetYaxis()->SetTitleOffset(1.1);
    fh1_twim_ESum[2]->GetXaxis()->CenterTitle(true);
    fh1_twim_ESum[2]->GetYaxis()->CenterTitle(true);
    fh1_twim_ESum[2]->GetXaxis()->SetLabelSize(0.045);
    fh1_twim_ESum[2]->GetXaxis()->SetTitleSize(0.045);
    fh1_twim_ESum[2]->GetYaxis()->SetLabelSize(0.045);
    fh1_twim_ESum[2]->GetYaxis()->SetTitleSize(0.045);
    fh1_twim_ESum[2]->SetFillColor(31);
    fh1_twim_ESum[2]->Draw("");

    // TWIM: Map data for Esum1 vs Esum2
    cTwimMap_ESum2 = new TCanvas("twim_E1vsE2", "twim_E1vsE2", 10, 10, 800, 700);
    fh2_twim_ESum = new TH2F("fh2_twim_ESum", "twim: ESum1 vs Esum2", 2192, 0, 8192, 2192, 0, 8192);
    fh2_twim_ESum->GetXaxis()->SetTitle("Energy1 [channels]");
    fh2_twim_ESum->GetYaxis()->SetTitle("Energy2 [channels]");
    fh2_twim_ESum->GetYaxis()->SetTitleOffset(1.1);
    fh2_twim_ESum->GetXaxis()->CenterTitle(true);
    fh2_twim_ESum->GetYaxis()->CenterTitle(true);
    fh2_twim_ESum->GetXaxis()->SetLabelSize(0.045);
    fh2_twim_ESum->GetXaxis()->SetTitleSize(0.045);
    fh2_twim_ESum->GetYaxis()->SetLabelSize(0.045);
    fh2_twim_ESum->GetYaxis()->SetTitleSize(0.045);
    fh2_twim_ESum->Draw("col");

    // TWIM: Esum versus DT16-DT1
    cTwimMap_ESum_vs_diffDT = new TCanvas("twim_ESum_vs_diffDT", "twim_ESum_vs_diffDT", 10, 10, 800, 700);
    if (NbSections > 1)
        cTwimMap_ESum_vs_diffDT->Divide(2, 2);
    for (Int_t i = 0; i < NbSections; i++)
    {
        sprintf(Name1, "fh2_twim_ESum_vs_diffDT_Sec_%d", i + 1);
        sprintf(Name2, "TWIM: ESum vs diffDT in section %d", i + 1);
        fh2_twim_ESum_vs_diffDT[i] = new TH2F(Name1, Name2, 2000, -2000, 2000, 2192, 0, 8192);
        fh2_twim_ESum_vs_diffDT[i]->GetXaxis()->SetTitle("DT difference [channels]");
        fh2_twim_ESum_vs_diffDT[i]->GetYaxis()->SetTitle("Energy Sum [channels]");
        fh2_twim_ESum_vs_diffDT[i]->GetYaxis()->SetTitleOffset(1.1);
        fh2_twim_ESum_vs_diffDT[i]->GetXaxis()->CenterTitle(true);
        fh2_twim_ESum_vs_diffDT[i]->GetYaxis()->CenterTitle(true);
        fh2_twim_ESum_vs_diffDT[i]->GetXaxis()->SetLabelSize(0.045);
        fh2_twim_ESum_vs_diffDT[i]->GetXaxis()->SetTitleSize(0.045);
        fh2_twim_ESum_vs_diffDT[i]->GetYaxis()->SetLabelSize(0.045);
        fh2_twim_ESum_vs_diffDT[i]->GetYaxis()->SetTitleSize(0.045);
        if (NbSections > 1)
            cTwimMap_ESum_vs_diffDT->cd(i + 1);
        else
            cTwimMap_ESum_vs_diffDT->cd();
        fh2_twim_ESum_vs_diffDT[i]->Draw("col");
    }

    // Cal data, position in mm for each anode
    for (Int_t i = 0; i < NbSections; i++)
    {
        sprintf(Name1, "Twim_Pos-X_Sec_%d", i + 1);
        sprintf(Name2, "Pos-X in mm section %d", i + 1);
        cTwimCal_Pos[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);
        cTwimCal_Pos[i]->Divide(4, 4);
        for (Int_t j = 0; j < NbAnodes; j++)
        {
            sprintf(Name1, "fh1_Twim_Sec%d_Pos_a%d", i + 1, j + 1);
            sprintf(Name2, "Sec %d, Anode %d", i + 1, j + 1);
            if (i == 0 || i == 3)
                fh1_Twimcal_Pos[i][j] = new TH1F(Name1, Name2, 500, -50., 90.);
            else
                fh1_Twimcal_Pos[i][j] = new TH1F(Name1, Name2, 500, -100., 0.);
            fh1_Twimcal_Pos[i][j]->GetXaxis()->SetTitle("Position-X [mm]");
            fh1_Twimcal_Pos[i][j]->GetYaxis()->SetTitle("Counts");
            fh1_Twimcal_Pos[i][j]->GetYaxis()->SetTitleOffset(1.1);
            fh1_Twimcal_Pos[i][j]->GetXaxis()->CenterTitle(true);
            fh1_Twimcal_Pos[i][j]->GetYaxis()->CenterTitle(true);
            fh1_Twimcal_Pos[i][j]->GetXaxis()->SetLabelSize(0.045);
            fh1_Twimcal_Pos[i][j]->GetXaxis()->SetTitleSize(0.045);
            fh1_Twimcal_Pos[i][j]->GetYaxis()->SetLabelSize(0.045);
            fh1_Twimcal_Pos[i][j]->GetYaxis()->SetTitleSize(0.045);
            fh1_Twimcal_Pos[i][j]->SetFillColor(31);
            fh1_Twimcal_Pos[i][j]->SetLineColor(1);
            cTwimCal_Pos[i]->cd(j + 1);
            fh1_Twimcal_Pos[i][j]->Draw("");
        }
    }

    // Hit data
    TCanvas* cTwim_Z = new TCanvas("Twim_charge_z", "Twim: Charge Z", 10, 10, 800, 700);
    fh1_Twimhit_z = new TH1F("fh1_Twim_charge_z", "Twim: Charge Z", 1200, 2, 30);
    fh1_Twimhit_z->GetXaxis()->SetTitle("Charge (Z)");
    fh1_Twimhit_z->GetYaxis()->SetTitle("Counts");
    fh1_Twimhit_z->GetYaxis()->SetTitleOffset(1.1);
    fh1_Twimhit_z->GetXaxis()->CenterTitle(true);
    fh1_Twimhit_z->GetYaxis()->CenterTitle(true);
    fh1_Twimhit_z->GetXaxis()->SetLabelSize(0.045);
    fh1_Twimhit_z->GetXaxis()->SetTitleSize(0.045);
    fh1_Twimhit_z->GetYaxis()->SetLabelSize(0.045);
    fh1_Twimhit_z->GetYaxis()->SetTitleSize(0.045);
    fh1_Twimhit_z->SetFillColor(29);
    fh1_Twimhit_z->SetLineColor(1);
    fh1_Twimhit_z->Draw("");

    TCanvas* cTwim_theta = new TCanvas("Twim_theta", "Twim: #theta_{XZ}", 10, 10, 800, 700);
    fh1_Twimhit_theta = new TH1F("fh1_Twim_theta", "Twim: #theta_{XZ}", 900, -60, 60);
    fh1_Twimhit_theta->GetXaxis()->SetTitle("#theta_{XZ} [mrad]");
    fh1_Twimhit_theta->GetYaxis()->SetTitle("Counts");
    fh1_Twimhit_theta->GetYaxis()->SetTitleOffset(1.1);
    fh1_Twimhit_theta->GetXaxis()->CenterTitle(true);
    fh1_Twimhit_theta->GetYaxis()->CenterTitle(true);
    fh1_Twimhit_theta->GetXaxis()->SetLabelSize(0.045);
    fh1_Twimhit_theta->GetXaxis()->SetTitleSize(0.045);
    fh1_Twimhit_theta->GetYaxis()->SetLabelSize(0.045);
    fh1_Twimhit_theta->GetYaxis()->SetTitleSize(0.045);
    fh1_Twimhit_theta->SetFillColor(29);
    fh1_Twimhit_theta->SetLineColor(1);
    fh1_Twimhit_theta->Draw("");

    TCanvas* cTwim_zvstheta = new TCanvas("Twim_charge_vs_theta", "Twim: Charge Z vs #theta_{XZ}", 10, 10, 800, 700);
    fh2_Twimhit_zvstheta =
        new TH2F("fh2_Twim_Charge_Z_vs_theta", "Twim: Charge Z vs #theta_{XZ}", 900, -40, 40, 900, 2, 28);
    fh2_Twimhit_zvstheta->GetXaxis()->SetTitle("#theta_{XZ} [mrad]");
    fh2_Twimhit_zvstheta->GetYaxis()->SetTitle("Charge (Z)");
    fh2_Twimhit_zvstheta->GetYaxis()->SetTitleOffset(1.1);
    fh2_Twimhit_zvstheta->GetXaxis()->CenterTitle(true);
    fh2_Twimhit_zvstheta->GetYaxis()->CenterTitle(true);
    fh2_Twimhit_zvstheta->GetXaxis()->SetLabelSize(0.045);
    fh2_Twimhit_zvstheta->GetXaxis()->SetTitleSize(0.045);
    fh2_Twimhit_zvstheta->GetYaxis()->SetLabelSize(0.045);
    fh2_Twimhit_zvstheta->GetYaxis()->SetTitleSize(0.045);
    fh2_Twimhit_zvstheta->Draw("col");

    // MAIN FOLDER-Twim
    TFolder* mainfolTwim = new TFolder("TWIM", "TWIM info");
    for (Int_t i = 0; i < NbSections; i++)
        mainfolTwim->Add(cTwimMap_E[i]);
    for (Int_t i = 0; i < NbSections; i++)
        mainfolTwim->Add(cTwimMap_DT[i]);
    for (Int_t i = 0; i < NbSections; i++)
        mainfolTwim->Add(cTwim_Mult[i]);
    mainfolTwim->Add(cTwimMap_ESum);
    mainfolTwim->Add(cTwimMap_ESum1);
    mainfolTwim->Add(cTwimMap_ESum2);
    mainfolTwim->Add(cTwimMap_ESum_vs_diffDT);
    for (Int_t i = 0; i < NbSections; i++)
    {
        mainfolTwim->Add(cTwimMap_EvsDT[i]);
        mainfolTwim->Add(cTwim_DTvsDT[i]);
    }
    mainfolTwim->Add(cTwimMap_EsumvsDT);
    if (fCalItemsTwim)
    {
        for (Int_t i = 0; i < NbSections; i++)
            mainfolTwim->Add(cTwimCal_Pos[i]);
    }
    if (fHitItemsTwim)
    {
        mainfolTwim->Add(cTwim_Z);
        mainfolTwim->Add(cTwim_theta);
        mainfolTwim->Add(cTwim_zvstheta);
    }
    run->AddObject(mainfolTwim);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_TWIM_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofTwimOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofTwimOnlineSpectra::Reset_Histo";
    // Map data
    for (Int_t i = 0; i < NbSections; i++)
    {
        fh1_Twimmap_mult[i]->Reset();
        fh2_twim_EneRawSumVsDriftTime[i]->Reset();
        fh2_twim_ESum_vs_diffDT[i]->Reset();
        for (Int_t j = 0; j < NbAnodes; j++)
        {
            fh1_twimmap_E[i][j]->Reset();
            fh1_twimmap_DT[i][j]->Reset();
            fh2_twim_EneRawVsDriftTime[i][j]->Reset();
        }
        for (Int_t j = 0; j < NbAnodes - 1; j++)
        {
            fh2_twim_DTvsDT[i][j]->Reset();
        }
    }
    fh1_twim_ESum[0]->Reset();
    fh1_twim_ESum[1]->Reset();
    fh1_twim_ESum[2]->Reset();
    fh2_twim_ESum->Reset();

    // Cal data
    if (fCalItemsTwim)
    {
        for (Int_t i = 0; i < NbSections; i++)
            for (Int_t j = 0; j < NbAnodes; j++)
                fh1_Twimcal_Pos[i][j]->Reset();
    }

    // Hit data
    if (fHitItemsTwim)
    {
        fh1_Twimhit_z->Reset();
        fh1_Twimhit_theta->Reset();
        fh2_Twimhit_zvstheta->Reset();
    }
}

void R3BSofTwimOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTwimOnlineSpectra::Exec FairRootManager not found";

    // Fill mapped data
    if (fMappedItemsTwim && fMappedItemsTwim->GetEntriesFast() > 0)
    {
        Double_t e1 = 0., e2 = 0., E[NbSections][NbAnodes], DT[NbSections][NbAnodes + NbSections];
        Double_t n1 = 0., n2 = 0.;
	UInt_t mult[NbSections][NbAnodes+NbSections];
        for (Int_t j = 0; j < NbSections; j++)
            for (Int_t i = 0; i < NbAnodes; i++)
                E[j][i] = 0.; // mult=1 !!!
        for (Int_t j = 0; j < NbSections; j++)
            for (Int_t i = 0; i < NbAnodes + NbSections; i++){
                DT[j][i] = 0.; // mult=1 !!!
		mult[j][i] = 0;
	    }
        Int_t nHits = fMappedItemsTwim->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTwimMappedData* hit = (R3BSofTwimMappedData*)fMappedItemsTwim->At(ihit);
            if (!hit)
                continue;
            fh1_Twimmap_mult[hit->GetSecID()]->Fill(hit->GetAnodeID());
            fh1_twimmap_E[hit->GetSecID()][hit->GetAnodeID()]->Fill(hit->GetEnergy());
            mult[hit->GetSecID()][hit->GetAnodeID()]++;
	    if (DT[hit->GetSecID()][hit->GetAnodeID()] == 0 && E[hit->GetSecID()][hit->GetAnodeID()] == 0 &&
                hit->GetEnergy() < 8192 && hit->GetEnergy() > 0)
            {
                E[hit->GetSecID()][hit->GetAnodeID()] = hit->GetEnergy(); // mult=1 !!!
                DT[hit->GetSecID()][hit->GetAnodeID()] = hit->GetTime();  // mult=1 !!!
            }

            if (hit->GetAnodeID() < NbAnodes / 2 && hit->GetAnodeID() != 7)
            {
                e1 = e1 + hit->GetEnergy();
                n1++;
            }
            else if (hit->GetAnodeID() >= NbAnodes / 2 && hit->GetAnodeID() < NbAnodes && hit->GetAnodeID() != 15)
            {
                e2 = e2 + hit->GetEnergy();
                n2++;
            }
        }
        for (Int_t j = 0; j < NbSections; j++)
        {
            for (Int_t i = 0; i < NbAnodes; i++)
            {
		if( (mult[j][i]==1) && (mult[j][NbAnodes]==1))
		{
			fh1_twimmap_DT[j][i]->Fill(DT[j][i] - DT[j][NbAnodes]);
			fh2_twim_EneRawVsDriftTime[j][i]->Fill(E[j][i], DT[j][i] - DT[j][NbAnodes]);
		}
	    }
	    for (Int_t i = 0; i < NbAnodes - 1; i++)
            {
		if( (mult[j][i]==1) && (mult[j][i+1]==1) && (mult[j][NbAnodes]==1) )
		{
			fh2_twim_DTvsDT[j][i]->Fill(DT[j][i] - DT[j][NbAnodes], DT[j][i + 1] - DT[j][NbAnodes]);
		}
	    }	
	    if( (mult[j][15]==1) && (mult[j][0]==1) && (mult[j][NbAnodes]==1) )
	    {
		fh2_twim_ESum_vs_diffDT[j]->Fill((DT[j][15] - DT[j][NbAnodes]) - (DT[j][0] - DT[j][NbAnodes]),
			                          (e1 + e2) / (n1 + n2));
	    }
	    if( (mult[j][5]==1) && (mult[j][NbAnodes]==1) )
	    {
		fh2_twim_EneRawSumVsDriftTime[j]->Fill(DT[j][5] - DT[j][NbAnodes], (e1 + e2) / (n1 + n2));
	    }
	}
        fh1_twim_ESum[0]->Fill(e1 / n1);
        fh1_twim_ESum[1]->Fill(e2 / n2);
        fh1_twim_ESum[2]->Fill((e1 + e2) / (n1 + n2));
        fh2_twim_ESum->Fill(e1 / n1, e2 / n2);
    }

    // Fill cal data
    if (fCalItemsTwim && fCalItemsTwim->GetEntriesFast() > 0)
    {
        Int_t nHits = fCalItemsTwim->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTwimCalData* hit = (R3BSofTwimCalData*)fCalItemsTwim->At(ihit);
            if (!hit)
                continue;
            fh1_Twimcal_Pos[hit->GetSecID()][hit->GetAnodeID()]->Fill(hit->GetDTime());
        }
    }

    // Fill hit data
    if (fHitItemsTwim && fHitItemsTwim->GetEntriesFast() > 0)
    {
        Int_t nHits = fHitItemsTwim->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTwimHitData* hit = (R3BSofTwimHitData*)fHitItemsTwim->At(ihit);
            if (!hit)
                continue;
            fh1_Twimhit_z->Fill(hit->GetZcharge());
            fh1_Twimhit_theta->Fill(hit->GetTheta() * 1000.);
            fh2_Twimhit_zvstheta->Fill(hit->GetTheta() * 1000., hit->GetZcharge());
        }
    }

    fNEvents += 1;
}

void R3BSofTwimOnlineSpectra::FinishEvent()
{
    if (fMappedItemsTwim)
    {
        fMappedItemsTwim->Clear();
    }
    if (fCalItemsTwim)
    {
        fCalItemsTwim->Clear();
    }
    if (fHitItemsTwim)
    {
        fHitItemsTwim->Clear();
    }
}

void R3BSofTwimOnlineSpectra::FinishTask()
{
    if (fMappedItemsTwim)
    {
        for (Int_t i = 0; i < NbSections; i++)
        {
            fh1_Twimmap_mult[i]->Write();
            cTwimMap_E[i]->Write();
            cTwimMap_DT[i]->Write();
            cTwimMap_EvsDT[i]->Write();
            cTwim_DTvsDT[i]->Write();
            cTwimMap_EsumvsDT->Write();
        }
        cTwimMap_ESum_vs_diffDT->Write();
        fh1_twim_ESum[0]->Write();
        fh1_twim_ESum[1]->Write();
        fh1_twim_ESum[2]->Write();
        fh2_twim_ESum->Write();
    }
    if (fCalItemsTwim)
    {
        for (Int_t i = 0; i < NbSections; i++)
            cTwimCal_Pos[i]->Write();
    }
    if (fHitItemsTwim)
    {
        fh1_Twimhit_z->Write();
        fh1_Twimhit_theta->Write();
        fh2_Twimhit_zvstheta->Write();
    }
}

ClassImp(R3BSofTwimOnlineSpectra)
