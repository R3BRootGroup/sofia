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
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

R3BSofTwimOnlineSpectra::R3BSofTwimOnlineSpectra()
    : FairTask("SofTwimOnlineSpectra", 1)
    , fMappedItemsTwim(NULL)
    , fNEvents(0)
{
}

R3BSofTwimOnlineSpectra::R3BSofTwimOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fMappedItemsTwim(NULL)
    , fNEvents(0)
{
}

R3BSofTwimOnlineSpectra::~R3BSofTwimOnlineSpectra()
{
    LOG(INFO) << "R3BSofTwimOnlineSpectra::Delete instance";
    if (fMappedItemsTwim)
        delete fMappedItemsTwim;
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

    // MAIN FOLDER-Twim
    TFolder* mainfolTwim = new TFolder("TWIM", "TWIM info");
    for (Int_t i = 0; i < NbSections; i++)
        mainfolTwim->Add(cTwimMap_E[i]);
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
        for (Int_t j = 0; j < NbAnodes; j++)
            fh1_twimmap_E[i][j]->Reset();
}

void R3BSofTwimOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTwimOnlineSpectra::Exec FairRootManager not found";

    // Fill mapped data
    if (fMappedItemsTwim && fMappedItemsTwim->GetEntriesFast())
    {
        Int_t nHits = fMappedItemsTwim->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTwimMappedData* hit = (R3BSofTwimMappedData*)fMappedItemsTwim->At(ihit);
            if (!hit)
                continue;
            fh1_twimmap_E[hit->GetSecID()][hit->GetAnodeID()]->Fill(hit->GetEnergy());
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
}

void R3BSofTwimOnlineSpectra::FinishTask()
{

    if (fMappedItemsTwim)
    {
        for (Int_t i = 0; i < NbSections; i++)
            cTwimMap_E[i]->Write();
    }
}

ClassImp(R3BSofTwimOnlineSpectra)
