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

R3BSofTwimOnlineSpectra::R3BSofTwimOnlineSpectra(const char* name, Int_t iVerbose)
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

    // TWIM: Map data
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
            cTwimMap_E[i]->cd(j + 1);
            fh1_twimmap_E[i][j]->Draw("");
        }
    }

    // TWIM: Map data
    for (Int_t i = 0; i < NbSections; i++)
    {
        sprintf(Name1, "Twim_EvsDT_Sec_%d", i + 1);
        sprintf(Name2, "Section %d", i + 1);
        cTwimMap_EvsDT[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);
        cTwimMap_EvsDT[i]->Divide(4, 4);
    }

    for (Int_t i = 0; i < NbSections; i++)
    {
        for (Int_t j = 0; j < NbAnodes; j++)
        {
            sprintf(Name1, "fh1_twim_EvsDT_sec%d_a%d", i + 1, j + 1);
            sprintf(Name2, "Sec %d:Anode %d", i + 1, j + 1);
            fh2_twim_EneRawVsDriftTime[i][j] = new TH2F(Name1, Name2, 500, 0, 8192, 800, -20000, 20000);
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
    }

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
            fh1_twimmap_DT[i][j] = new TH1F(Name1, Name2, 6000, -20000, 20000);
            fh1_twimmap_DT[i][j]->GetXaxis()->SetTitle("Drift time [channels]");
            fh1_twimmap_DT[i][j]->GetYaxis()->SetTitle("Counts");
            fh1_twimmap_DT[i][j]->GetYaxis()->SetTitleOffset(1.1);
            fh1_twimmap_DT[i][j]->GetXaxis()->CenterTitle(true);
            fh1_twimmap_DT[i][j]->GetYaxis()->CenterTitle(true);
            fh1_twimmap_DT[i][j]->GetXaxis()->SetLabelSize(0.045);
            fh1_twimmap_DT[i][j]->GetXaxis()->SetTitleSize(0.045);
            fh1_twimmap_DT[i][j]->GetYaxis()->SetLabelSize(0.045);
            fh1_twimmap_DT[i][j]->GetYaxis()->SetTitleSize(0.045);
            cTwimMap_DT[i]->cd(j + 1);
            fh1_twimmap_DT[i][j]->Draw("");
        }
    }

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
        fh1_Twimmap_mult[i]->Draw("");
    }

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
    fh1_twim_ESum[1]->Draw("");

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
    fh1_twim_ESum[2]->Draw("");

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

    // Hit data

    TCanvas* cTwim_Z = new TCanvas("Twim_charge_z", "Twim: Charge Z", 10, 10, 800, 700);
    fh1_Twimhit_z = new TH1F("fh1_Twim_charge_z", "Twim: Charge Z", 240, 0, 40);
    fh1_Twimhit_z->GetXaxis()->SetTitle("Charge (Z)");
    fh1_Twimhit_z->GetYaxis()->SetTitle("Counts");
    fh1_Twimhit_z->GetYaxis()->SetTitleOffset(1.1);
    fh1_Twimhit_z->GetXaxis()->CenterTitle(true);
    fh1_Twimhit_z->GetYaxis()->CenterTitle(true);
    fh1_Twimhit_z->GetXaxis()->SetLabelSize(0.045);
    fh1_Twimhit_z->GetXaxis()->SetTitleSize(0.045);
    fh1_Twimhit_z->GetYaxis()->SetLabelSize(0.045);
    fh1_Twimhit_z->GetYaxis()->SetTitleSize(0.045);
    fh1_Twimhit_z->Draw("");

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
    for (Int_t i = 0; i < NbSections; i++)
        mainfolTwim->Add(cTwimMap_EvsDT[i]);
    mainfolTwim->Add(cTwim_Z);
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
        for (Int_t j = 0; j < NbAnodes; j++)
        {
            fh1_twimmap_E[i][j]->Reset();
            fh1_twimmap_DT[i][j]->Reset();
            fh2_twim_EneRawVsDriftTime[i][j]->Reset();
        }
    }
    fh1_twim_ESum[0]->Reset();
    fh1_twim_ESum[1]->Reset();
    fh1_twim_ESum[2]->Reset();
    fh2_twim_ESum->Reset();
    fh1_Twimhit_z->Reset();
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
        for (Int_t j = 0; j < NbSections; j++)
            for (Int_t i = 0; i < NbAnodes; i++)
                E[j][i] = 0.; // mult=1 !!!
        for (Int_t j = 0; j < NbSections; j++)
            for (Int_t i = 0; i < NbAnodes + NbSections; i++)
                DT[j][i] = 0.; // mult=1 !!!
        Int_t nHits = fMappedItemsTwim->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTwimMappedData* hit = (R3BSofTwimMappedData*)fMappedItemsTwim->At(ihit);
            if (!hit)
                continue;
            fh1_Twimmap_mult[hit->GetSecID()]->Fill(hit->GetAnodeID());
            fh1_twimmap_E[hit->GetSecID()][hit->GetAnodeID()]->Fill(hit->GetEnergy());
            if (DT[hit->GetSecID()][hit->GetAnodeID()] == 0 && E[hit->GetSecID()][hit->GetAnodeID()] == 0 &&
                hit->GetEnergy() < 8192 && hit->GetEnergy() > 0)
            {
                E[hit->GetSecID()][hit->GetAnodeID()] = hit->GetEnergy(); // mult=1 !!!
                DT[hit->GetSecID()][hit->GetAnodeID()] = hit->GetTime();  // mult=1 !!!
            }

            if (hit->GetAnodeID() < NbAnodes / 2)
            {
                e1 = e1 + hit->GetEnergy();
                n1++;
            }
            else if (hit->GetAnodeID() < NbAnodes)
            {
                e2 = e2 + hit->GetEnergy();
                n2++;
            }
        }
        for (Int_t j = 0; j < NbSections; j++)
            for (Int_t i = 0; i < NbAnodes; i++)
            {
                fh1_twimmap_DT[j][i]->Fill(DT[j][i] - DT[j][NbAnodes]);
                fh2_twim_EneRawVsDriftTime[j][i]->Fill(E[j][i], DT[j][i] - DT[j][NbAnodes]);
            }
        fh1_twim_ESum[0]->Fill(e1 / n1);
        fh1_twim_ESum[1]->Fill(e2 / n2);
        fh1_twim_ESum[2]->Fill((e1 + e2) / (n1 + n2));
        fh2_twim_ESum->Fill(e1 / n1, e2 / n2);
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
        }
        fh1_twim_ESum[0]->Write();
        fh1_twim_ESum[1]->Write();
        fh1_twim_ESum[2]->Write();
        fh2_twim_ESum->Write();
    }
    if (fHitItemsTwim)
    {
        fh1_Twimhit_z->Write();
    }
}

ClassImp(R3BSofTwimOnlineSpectra)
