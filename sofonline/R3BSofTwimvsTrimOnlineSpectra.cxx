// ------------------------------------------------------------
// -----           R3BSofTwimvsTrimOnlineSpectra          -----
// -----    Created 03/03/21  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms for Twim-Trim correlations
 */

#include "R3BSofTwimvsTrimOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BSofTrimHitData.h"
#include "R3BTwimHitData.h"
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

R3BSofTwimvsTrimOnlineSpectra::R3BSofTwimvsTrimOnlineSpectra()
    : FairTask("SofTwimvsTrimOnlineSpectra", 1)
    , fHitItemsTrim(NULL)
    , fHitItemsTwim(NULL)
    , fNEvents(0)
{
}

R3BSofTwimvsTrimOnlineSpectra::R3BSofTwimvsTrimOnlineSpectra(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fHitItemsTrim(NULL)
    , fHitItemsTwim(NULL)
    , fNEvents(0)
{
}

R3BSofTwimvsTrimOnlineSpectra::~R3BSofTwimvsTrimOnlineSpectra()
{
    LOG(INFO) << "R3BSofTwimvsTrimOnlineSpectra::Delete instance";
    if (fHitItemsTrim)
        delete fHitItemsTrim;
    if (fHitItemsTwim)
        delete fHitItemsTwim;
}

InitStatus R3BSofTwimvsTrimOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofTwimvsTrimOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTwimvsTrimOnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // get access to hit data of the Trim detector
    fHitItemsTrim = (TClonesArray*)mgr->GetObject("TrimHitData");
    if (!fHitItemsTrim)
    {
        LOG(WARNING) << "R3BSofTwimvsTrimOnlineSpectra: TrimHitData not found";
        return kERROR;
    }

    // get access to hit data of the Twim
    fHitItemsTwim = (TClonesArray*)mgr->GetObject("TwimHitData");
    if (!fHitItemsTwim)
        LOG(WARNING) << "R3BSofTwimvsTrimOnlineSpectra: TwimHitData not found";

    // Create histograms for detectors
    char Name1[255];
    char Name2[255];

    // Hit data
    TCanvas* c_Z = new TCanvas("Charge_z_correlation", "Charge Z correlation", 10, 10, 800, 700);
    fh2_hit_z = new TH2F("fh2_Twim_vs_Trim_charge_z", "Twim vs Trim: Charge Z", 1000, 6, 98, 1000, 6, 98);
    fh2_hit_z->GetXaxis()->SetTitle("Trim charge (Z)");
    fh2_hit_z->GetYaxis()->SetTitle("Twim charge (Z)");
    fh2_hit_z->GetYaxis()->SetTitleOffset(1.1);
    fh2_hit_z->GetXaxis()->SetTitleOffset(1.);
    fh2_hit_z->GetXaxis()->CenterTitle(true);
    fh2_hit_z->GetYaxis()->CenterTitle(true);
    fh2_hit_z->GetXaxis()->SetLabelSize(0.045);
    fh2_hit_z->GetXaxis()->SetTitleSize(0.045);
    fh2_hit_z->GetYaxis()->SetLabelSize(0.045);
    fh2_hit_z->GetYaxis()->SetTitleSize(0.045);
    fh2_hit_z->Draw("col");

    TCanvas* c_theta = new TCanvas("Theta_correlation", "#theta_{XZ} correlation", 10, 10, 800, 700);
    fh2_hit_theta = new TH2F("fh2_Twim_vs_Trim_theta", "Twim vs Trim: #theta_{XZ}", 900, -30, 30, 900, -30, 30);
    fh2_hit_theta->GetXaxis()->SetTitle("Trim #theta_{XZ} [mrad]");
    fh2_hit_theta->GetYaxis()->SetTitle("Twim #theta_{XZ} [mrad]");
    fh2_hit_theta->GetXaxis()->SetTitleOffset(1.1);
    fh2_hit_theta->GetYaxis()->SetTitleOffset(1.1);
    fh2_hit_theta->GetXaxis()->CenterTitle(true);
    fh2_hit_theta->GetYaxis()->CenterTitle(true);
    fh2_hit_theta->GetXaxis()->SetLabelSize(0.045);
    fh2_hit_theta->GetXaxis()->SetTitleSize(0.045);
    fh2_hit_theta->GetYaxis()->SetLabelSize(0.045);
    fh2_hit_theta->GetYaxis()->SetTitleSize(0.045);
    fh2_hit_theta->SetFillColor(29);
    fh2_hit_theta->SetLineColor(1);
    fh2_hit_theta->Draw("col");

    // MAIN FOLDER-Twim-Music
    TFolder* mainfolTwim = new TFolder("TWIM_vs_TRIM", "Twim vs Trim info");
    if (fHitItemsTwim && fHitItemsTrim)
    {
        mainfolTwim->Add(c_Z);
        // mainfolTwim->Add(c_theta);
    }
    run->AddObject(mainfolTwim);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_TWIMvsTRIM_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofTwimvsTrimOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofTwimvsTrimOnlineSpectra::Reset_Histo";

    if (fHitItemsTwim && fHitItemsTrim)
    {
        fh2_hit_z->Reset();
        fh2_hit_theta->Reset();
    }
}

void R3BSofTwimvsTrimOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTwimvsTrimOnlineSpectra::Exec FairRootManager not found";

    // Fill hit data
    if (fHitItemsTwim && fHitItemsTwim->GetEntriesFast() > 0 && fHitItemsTrim && fHitItemsTrim->GetEntriesFast() > 0)
    {
        Float_t z1 = 0., z2 = 0.; // , theta1 = 0., theta2 = 0.;
        // TRIM
        Int_t nHits1 = fHitItemsTrim->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits1; ihit++)
        {
            R3BSofTrimHitData* hit = (R3BSofTrimHitData*)fHitItemsTrim->At(ihit);
            if (!hit)
                continue;
            z1 = hit->GetZcharge();
            // theta1 = hit->GetTheta() * 1000.; // mrad
        }
        // TWIM
        Int_t nHits2 = fHitItemsTwim->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits2; ihit++)
        {
            R3BTwimHitData* hit = (R3BTwimHitData*)fHitItemsTwim->At(ihit);
            if (!hit)
                continue;
            z2 = hit->GetZcharge();
            // Fill histograms
            fh2_hit_z->Fill(z1, z2);
        }
    }
}

void R3BSofTwimvsTrimOnlineSpectra::FinishEvent()
{
    if (fHitItemsTrim)
    {
        fHitItemsTrim->Clear();
    }
    if (fHitItemsTwim)
    {
        fHitItemsTwim->Clear();
    }
}

void R3BSofTwimvsTrimOnlineSpectra::FinishTask()
{
    if (fHitItemsTwim && fHitItemsTrim)
    {
        fh2_hit_z->Write();
        fh2_hit_theta->Write();
    }
}

ClassImp(R3BSofTwimvsTrimOnlineSpectra)
