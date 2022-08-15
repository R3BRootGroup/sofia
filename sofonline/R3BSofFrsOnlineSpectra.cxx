// ------------------------------------------------------------
// -----                  R3BSofFrsOnlineSpectra          -----
// -----    Created 28/01/20  by J.L. Rodriguez-Sanchez   -----
// -----           Fill FRS online histograms             -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with FRS online data
 */

#include "R3BSofFrsOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BFrsData.h"
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
#include <stdlib.h>

using namespace std;

R3BSofFrsOnlineSpectra::R3BSofFrsOnlineSpectra()
    : FairTask("SofFrsOnlineSpectra", 1)
    , fHitItemsFrs(NULL)
    , fNEvents(0)
{
}

R3BSofFrsOnlineSpectra::R3BSofFrsOnlineSpectra(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fHitItemsFrs(NULL)
    , fNEvents(0)
{
}

R3BSofFrsOnlineSpectra::~R3BSofFrsOnlineSpectra()
{
    LOG(INFO) << "R3BSofFrsOnlineSpectra::Delete instance";
    if (fHitItemsFrs)
        delete fHitItemsFrs;
}

InitStatus R3BSofFrsOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofFrsOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofFrsOnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // get access to mapped data of FRS
    fHitItemsFrs = (TClonesArray*)mgr->GetObject("FrsData");
    if (!fHitItemsFrs)
    {
        return kFATAL;
    }

    // Create histograms for detectors
    TString Name1;
    TString Name2;

    cBeta = new TCanvas("Beta_frs", "Beta info", 10, 10, 800, 700);

    // Hit data, beta
    Name1 = "fh1_beta_frs";
    Name2 = "FRS: Beta";
    fh1_beta = new TH1F(Name1, Name2, 800, 0.45, 0.9);
    fh1_beta->GetXaxis()->SetTitle("Beta");
    fh1_beta->GetYaxis()->SetTitle("Counts");
    fh1_beta->GetYaxis()->SetTitleOffset(1.15);
    fh1_beta->GetXaxis()->CenterTitle(true);
    fh1_beta->GetYaxis()->CenterTitle(true);
    fh1_beta->GetXaxis()->SetLabelSize(0.045);
    fh1_beta->GetXaxis()->SetTitleSize(0.045);
    fh1_beta->GetYaxis()->SetLabelSize(0.045);
    fh1_beta->GetYaxis()->SetTitleSize(0.045);
    fh1_beta->SetFillColor(2);
    fh1_beta->SetLineColor(1);
    fh1_beta->Draw("");

    // Hit data, brho
    cBrho = new TCanvas("Brho_frs", "Brho info", 10, 10, 800, 700);

    Name1 = "fh1_brho_frs";
    Name2 = "FRS: Brho S2-Cave";
    fh1_brho = new TH1F(Name1, Name2, 1000, 7., 11.);
    fh1_brho->GetXaxis()->SetTitle("Brho [Tm]");
    fh1_brho->GetYaxis()->SetTitle("Counts");
    fh1_brho->GetYaxis()->SetTitleOffset(1.15);
    fh1_brho->GetXaxis()->CenterTitle(true);
    fh1_brho->GetYaxis()->CenterTitle(true);
    fh1_brho->GetXaxis()->SetLabelSize(0.045);
    fh1_brho->GetXaxis()->SetTitleSize(0.045);
    fh1_brho->GetYaxis()->SetLabelSize(0.045);
    fh1_brho->GetYaxis()->SetTitleSize(0.045);
    fh1_brho->SetFillColor(2);
    fh1_brho->SetLineColor(1);
    fh1_brho->Draw("");

    // Hit data, Xs2_vs_Tof
    cXs2vsBeta = new TCanvas("Xs2_vs_beta_frs", "Xs2_vs_Beta 2D info", 10, 10, 800, 700);

    Name1 = "fh2_Xs2_vs_beta_frs";
    Name2 = "FRS: Xs2 vs #beta";
    fh2_Xs2vsbeta = new TH2F(Name1, Name2, 800, -100., 100., 400, 0.45, 0.9);
    fh2_Xs2vsbeta->GetXaxis()->SetTitle("X at S2 [mm]");
    fh2_Xs2vsbeta->GetYaxis()->SetTitle("FRS-#beta");
    fh2_Xs2vsbeta->GetYaxis()->SetTitleOffset(1.1);
    fh2_Xs2vsbeta->GetXaxis()->CenterTitle(true);
    fh2_Xs2vsbeta->GetYaxis()->CenterTitle(true);
    fh2_Xs2vsbeta->GetXaxis()->SetLabelSize(0.045);
    fh2_Xs2vsbeta->GetXaxis()->SetTitleSize(0.045);
    fh2_Xs2vsbeta->GetYaxis()->SetLabelSize(0.045);
    fh2_Xs2vsbeta->GetYaxis()->SetTitleSize(0.045);
    fh2_Xs2vsbeta->Draw("col");

    // Hit data, Aq_vs_q
    cAqvsq = new TCanvas("Aq_vs_q_frs", "A/q_vs_q 2D info", 10, 10, 800, 700);

    Name1 = "fh2_Aq_vs_q_frs";
    Name2 = "FRS: A/q vs q";
    fh2_Aqvsq = new TH2F(Name1, Name2, 900, 1.6, 2.7, 900, 10, 30.5);
    fh2_Aqvsq->GetXaxis()->SetTitle("A/q");
    fh2_Aqvsq->GetYaxis()->SetTitle("Z [Charge units]");
    fh2_Aqvsq->GetYaxis()->SetTitleOffset(1.1);
    fh2_Aqvsq->GetXaxis()->CenterTitle(true);
    fh2_Aqvsq->GetYaxis()->CenterTitle(true);
    fh2_Aqvsq->GetXaxis()->SetLabelSize(0.045);
    fh2_Aqvsq->GetXaxis()->SetTitleSize(0.045);
    fh2_Aqvsq->GetYaxis()->SetLabelSize(0.045);
    fh2_Aqvsq->GetYaxis()->SetTitleSize(0.045);
    fh2_Aqvsq->Draw("colz");

    // MAIN FOLDER-FRS
    TFolder* mainfol = new TFolder("FRS-IncomingID", "FRS incomingID info");
    mainfol->Add(cBeta);
    mainfol->Add(cBrho);
    mainfol->Add(cXs2vsBeta);
    mainfol->Add(cAqvsq);
    run->AddObject(mainfol);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_FRS_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofFrsOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofFrsOnlineSpectra::Reset_Histo";

    fh1_beta->Reset();
    fh1_brho->Reset();
    fh2_Aqvsq->Reset();
    fh2_Xs2vsbeta->Reset();
}

void R3BSofFrsOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofFRSOnlineSpectra::Exec FairRootManager not found";

    // Fill Hit data
    if (fHitItemsFrs && fHitItemsFrs->GetEntriesFast() > 0)
    {
        Int_t nHits = fHitItemsFrs->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BFrsData* hit = (R3BFrsData*)fHitItemsFrs->At(ihit);
            if (!hit)
                continue;
            fh1_beta->Fill(hit->GetBeta());
            fh1_brho->Fill(hit->GetBrho());
            fh2_Aqvsq->Fill(hit->GetAq(), hit->GetZ());
            fh2_Xs2vsbeta->Fill(hit->GetXS2(), hit->GetBeta());
        }
    }

    fNEvents += 1;
}

void R3BSofFrsOnlineSpectra::FinishEvent()
{
    if (fHitItemsFrs)
    {
        fHitItemsFrs->Clear();
    }
}

void R3BSofFrsOnlineSpectra::FinishTask()
{
    if (fHitItemsFrs)
    {
        cBeta->Write();
        cBrho->Write();
        cXs2vsBeta->Write();
        cAqvsq->Write();
    }
}

ClassImp(R3BSofFrsOnlineSpectra)
