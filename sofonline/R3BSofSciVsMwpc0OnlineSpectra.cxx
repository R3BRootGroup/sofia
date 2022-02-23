// ------------------------------------------------------------
// -----                  R3BSofSciVsMwpc0OnlineSpectra           -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofSciVsMwpc0OnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BMwpcHitData.h"
#include "R3BSofSciCalData.h"
#include "R3BSofSciSingleTcalData.h"
#include "THttpServer.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "TCanvas.h"
#include "TFolder.h"
#include "TH1.h"
#include "TH2.h"
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

R3BSofSciVsMwpc0OnlineSpectra::R3BSofSciVsMwpc0OnlineSpectra()
    : FairTask("SofSciOnlineSpectra", 1)
    , fSTcalSci(NULL)
    , fCalSci(NULL)
    , fHitMwpc0(NULL)
    , fNEvents(0)
    , fNbDetectors(2)
{
}

R3BSofSciVsMwpc0OnlineSpectra::R3BSofSciVsMwpc0OnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fSTcalSci(NULL)
    , fCalSci(NULL)
    , fHitMwpc0(NULL)
    , fNEvents(0)
    , fNbDetectors(2)
{
}

R3BSofSciVsMwpc0OnlineSpectra::~R3BSofSciVsMwpc0OnlineSpectra()
{
    LOG(INFO) << "R3BSofSciVsMwpc0OnlineSpectra::Delete instance";
    if (fSTcalSci)
        delete fSTcalSci;
    if (fCalSci)
        delete fCalSci;
    if (fHitMwpc0)
        delete fHitMwpc0;
}

InitStatus R3BSofSciVsMwpc0OnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofSciVsMwpc0OnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofSciVsMwpc0OnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // --- -------------------------------------------- --- //
    // --- get access to single tcal data of the SofSci --- //
    // --- -------------------------------------------- --- //
    fSTcalSci = (TClonesArray*)mgr->GetObject("SofSciSingleTcalData");
    if (!fSTcalSci)
    {
        return kFATAL;
    }

    // --- ------------------------------------ --- //
    // --- get access to cal data of the SofSci --- //
    // --- ------------------------------------ --- //
    fCalSci = (TClonesArray*)mgr->GetObject("SofSciCalData");
    if (!fCalSci)
    {
        return kFATAL;
    }

    // --- ----------------------------------- --- //
    // --- get access to hit data of the MWPC0 --- //
    // --- ----------------------------------- --- //
    fHitMwpc0 = (TClonesArray*)mgr->GetObject("Mwpc0HitData");
    if (!fHitMwpc0)
        LOG(WARNING) << "R3BSofSciVsMwpc0OnlineSpectra: Mwpc0HitData not found";

    // --- ------------------------------- --- //
    // --- Create histograms for detectors --- //
    // --- ------------------------------- --- //
    char Name1[255];
    char Name2[255];

    // === Mwpc0 cal data vs CalPos of SofSci at Cave C === //
    cMwpc0vsRawPos = new TCanvas("Mwpc0_vs_RawPosSciCaveC", "Mwpc0 vs SofSciCaveC-RawPos", 10, 10, 800, 700);
    cMwpc0vsRawPos->Divide(2, 1);

    cMwpc0vsRawPos->cd(1);
    fh2_Mwpc0XvsRawPos =
        new TH2F("fh2_Mwpc0X_vs_RawPosSciCaveC", "Mwpc0-X vs SciCaveC-RawPos", 1400, -7, 7, 2000, -50, 50);
    fh2_Mwpc0XvsRawPos->GetXaxis()->SetTitle(
        "(RIGHT,Wix. side) -->  SofSci-RawPos Cave C [ns] --> (LEFT,Mes. side) -->");
    fh2_Mwpc0XvsRawPos->GetYaxis()->SetTitle("Mwpc0-X in the det frame [mm]");
    fh2_Mwpc0XvsRawPos->GetYaxis()->SetTitleOffset(1.1);
    fh2_Mwpc0XvsRawPos->GetXaxis()->CenterTitle(true);
    fh2_Mwpc0XvsRawPos->GetYaxis()->CenterTitle(true);
    fh2_Mwpc0XvsRawPos->GetXaxis()->SetLabelSize(0.045);
    fh2_Mwpc0XvsRawPos->GetXaxis()->SetTitleSize(0.045);
    fh2_Mwpc0XvsRawPos->GetYaxis()->SetLabelSize(0.045);
    fh2_Mwpc0XvsRawPos->GetYaxis()->SetTitleSize(0.045);
    fh2_Mwpc0XvsRawPos->Draw("col");

    cMwpc0vsRawPos->cd(2);
    fh2_Mwpc0YvsRawPos =
        new TH2F("fh2_Mwpc0Y_vs_RawPosSciCaveC", "Mwpc0-Y vs SciCaveC-RawPos", 1400, -7, 7, 2000, -50, 50);
    fh2_Mwpc0YvsRawPos->GetXaxis()->SetTitle(
        "(RIGHT,Wix. side) -->  SofSci-RawPos Cave C [ns] --> (LEFT,Mes. side) -->");
    fh2_Mwpc0YvsRawPos->GetYaxis()->SetTitle("Mwpc0-Y in the det frame [mm]");
    fh2_Mwpc0YvsRawPos->GetYaxis()->SetTitleOffset(1.1);
    fh2_Mwpc0YvsRawPos->GetXaxis()->CenterTitle(true);
    fh2_Mwpc0YvsRawPos->GetYaxis()->CenterTitle(true);
    fh2_Mwpc0YvsRawPos->GetXaxis()->SetLabelSize(0.045);
    fh2_Mwpc0YvsRawPos->GetXaxis()->SetTitleSize(0.045);
    fh2_Mwpc0YvsRawPos->GetYaxis()->SetLabelSize(0.045);
    fh2_Mwpc0YvsRawPos->GetYaxis()->SetTitleSize(0.045);
    fh2_Mwpc0YvsRawPos->Draw("col");

    // === Mwpc0 cal data vs CalPos of SofSci at Cave C === //
    cMwpc0vsCalPos = new TCanvas("Mwpc0_vs_CalPosSciCaveC", "Mwpc0 vs SofSciCaveC-X", 10, 10, 800, 700);
    cMwpc0vsCalPos->Divide(2, 1);

    cMwpc0vsCalPos->cd(1);
    fh2_Mwpc0XvsCalPos =
        new TH2F("fh2_Mwpc0X_vs_CalPosSciCaveC", "Mwpc0-X vs SciCaveC-CalPos", 1400, -7, 7, 2000, -50, 50);
    fh2_Mwpc0XvsCalPos->GetXaxis()->SetTitle("(RIGHT,Wix. side) -->  SofSci-X Cave C [mm] --> (LEFT,Mes. side) -->");
    fh2_Mwpc0XvsCalPos->GetYaxis()->SetTitle("Mwpc0-X [mm]");
    fh2_Mwpc0XvsCalPos->GetYaxis()->SetTitleOffset(1.1);
    fh2_Mwpc0XvsCalPos->GetXaxis()->CenterTitle(true);
    fh2_Mwpc0XvsCalPos->GetYaxis()->CenterTitle(true);
    fh2_Mwpc0XvsCalPos->GetXaxis()->SetLabelSize(0.045);
    fh2_Mwpc0XvsCalPos->GetXaxis()->SetTitleSize(0.045);
    fh2_Mwpc0XvsCalPos->GetYaxis()->SetLabelSize(0.045);
    fh2_Mwpc0XvsCalPos->GetYaxis()->SetTitleSize(0.045);
    fh2_Mwpc0XvsCalPos->Draw("col");

    cMwpc0vsCalPos->cd(2);
    fh2_Mwpc0YvsCalPos =
        new TH2F("fh2_Mwpc0Y_vs_CalPosSciCaveC", "Mwpc0-Y vs SciCaveC-CalPos", 1400, -7, 7, 2000, -50, 50);
    fh2_Mwpc0YvsCalPos->GetXaxis()->SetTitle("(RIGHT,Wix. side) -->  SofSci-X Cave C [mm] --> (LEFT,Mes. side) -->");
    fh2_Mwpc0YvsCalPos->GetYaxis()->SetTitle("Mwpc0-Y [mm]");
    fh2_Mwpc0YvsCalPos->GetYaxis()->SetTitleOffset(1.1);
    fh2_Mwpc0YvsCalPos->GetXaxis()->CenterTitle(true);
    fh2_Mwpc0YvsCalPos->GetYaxis()->CenterTitle(true);
    fh2_Mwpc0YvsCalPos->GetXaxis()->SetLabelSize(0.045);
    fh2_Mwpc0YvsCalPos->GetXaxis()->SetTitleSize(0.045);
    fh2_Mwpc0YvsCalPos->GetYaxis()->SetLabelSize(0.045);
    fh2_Mwpc0YvsCalPos->GetYaxis()->SetTitleSize(0.045);
    fh2_Mwpc0YvsCalPos->Draw("col");

    // --- --------------- --- //
    // --- MAIN FOLDER-Sci --- //
    // --- --------------- --- //
    TFolder* mainfol = new TFolder("SofSci-Mwpc0", "Mwpc0 vs SofSci info");
    mainfol->Add(cMwpc0vsRawPos);
    mainfol->Add(cMwpc0vsCalPos);
    run->AddObject(mainfol);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_SOFSCI_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofSciVsMwpc0OnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofSciVsMwpc0OnlineSpectra::Reset_Histo";
    fh2_Mwpc0XvsRawPos->Reset();
    fh2_Mwpc0YvsRawPos->Reset();
    fh2_Mwpc0XvsCalPos->Reset();
    fh2_Mwpc0YvsCalPos->Reset();
}

void R3BSofSciVsMwpc0OnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofSciVsMwpc0OnlineSpectra::Exec FairRootManager not found";

    Int_t nHits;

    if (fHitMwpc0 && fHitMwpc0->GetEntriesFast() == 1)
    {
        // --- -------------------------- --- //
        // --- read the hit data of Mwpc0 --- //
        // --- -------------------------- --- //
        R3BMwpcHitData* hitMwpc0 = (R3BMwpcHitData*)fHitMwpc0->At(0);

        // --- ------------------------------ --- //
        // --- loop over sci single tcal data --- //
        // --- ------------------------------ --- //
        if (fSTcalSci && fSTcalSci->GetEntriesFast())
        {
            nHits = fSTcalSci->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BSofSciSingleTcalData* hitstcal = (R3BSofSciSingleTcalData*)fSTcalSci->At(ihit);
                if (!hitstcal)
                    continue;
                if (hitstcal->GetDetector() == fNbDetectors && hitMwpc0)
                {
                    fh2_Mwpc0XvsRawPos->Fill(hitstcal->GetRawPosNs(), hitMwpc0->GetX());
                    fh2_Mwpc0YvsRawPos->Fill(hitstcal->GetRawPosNs(), hitMwpc0->GetY());
                }
            }
        }
        // --- ---------------------- --- //
        // --- loop over sci cal data --- //
        // --- ---------------------- --- //
        if (fCalSci && fCalSci->GetEntriesFast())
        {
            nHits = fCalSci->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BSofSciCalData* hitcal = (R3BSofSciCalData*)fCalSci->At(ihit);
                if (!hitcal)
                    continue;
                if (hitcal->GetDetector() == fNbDetectors && hitMwpc0)
                {
                    fh2_Mwpc0XvsCalPos->Fill(hitcal->GetPosMm(), hitMwpc0->GetX());
                    fh2_Mwpc0YvsCalPos->Fill(hitcal->GetPosMm(), hitMwpc0->GetY());
                }
            }
        }
    } // end of HitMwpc0

    fNEvents += 1;
}

// -----   Public method Reset   ------------------------------------------------
void R3BSofSciVsMwpc0OnlineSpectra::Reset() {}

// -----   Public method Finish   -----------------------------------------------
void R3BSofSciVsMwpc0OnlineSpectra::FinishEvent()
{
    if (fSTcalSci)
    {
        fSTcalSci->Clear();
    }
    if (fCalSci)
    {
        fCalSci->Clear();
    }
    if (fHitMwpc0)
    {
        fHitMwpc0->Clear();
    }
}

void R3BSofSciVsMwpc0OnlineSpectra::FinishTask()
{
    if (fSTcalSci && fHitMwpc0)
    {
        fh2_Mwpc0XvsRawPos->Write();
        fh2_Mwpc0YvsRawPos->Write();
    }
    if (fCalSci && fHitMwpc0)
    {
        fh2_Mwpc0XvsCalPos->Write();
        fh2_Mwpc0YvsCalPos->Write();
    }
}
