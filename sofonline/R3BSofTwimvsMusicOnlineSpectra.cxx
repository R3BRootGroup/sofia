// ------------------------------------------------------------
// -----           R3BSofTwimvsMusicOnlineSpectra         -----
// -----    Created 29/09/19  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms for Music-Twim correlations
 */

#include "R3BSofTwimvsMusicOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BMusicHitData.h"
#include "R3BSofTwimHitData.h"
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

R3BSofTwimvsMusicOnlineSpectra::R3BSofTwimvsMusicOnlineSpectra()
    : FairTask("SofTwimvsMusicOnlineSpectra", 1)
    , fHitItemsMusic(NULL)
    , fHitItemsTwim(NULL)
    , fNEvents(0)
{
}

R3BSofTwimvsMusicOnlineSpectra::R3BSofTwimvsMusicOnlineSpectra(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fHitItemsMusic(NULL)
    , fHitItemsTwim(NULL)
    , fNEvents(0)
{
}

R3BSofTwimvsMusicOnlineSpectra::~R3BSofTwimvsMusicOnlineSpectra()
{
    LOG(INFO) << "R3BSofTwimvsMusicOnlineSpectra::Delete instance";
    if (fHitItemsMusic)
        delete fHitItemsMusic;
    if (fHitItemsTwim)
        delete fHitItemsTwim;
}

InitStatus R3BSofTwimvsMusicOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofTwimvsMusicOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTwimvsMusicOnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // get access to hit data of the MUSIC detector
    fHitItemsMusic = (TClonesArray*)mgr->GetObject("MusicHitData");
    if (!fHitItemsMusic)
    {
        LOG(WARNING) << "R3BSofTwimvsMusicOnlineSpectra: MusicHitData not found";
        return kERROR;
    }

    // get access to hit data of the TWIM
    fHitItemsTwim = (TClonesArray*)mgr->GetObject("TwimHitData");
    if (!fHitItemsTwim)
        LOG(WARNING) << "R3BSofTwimvsMusicOnlineSpectra: TwimHitData not found";

    // Create histograms for detectors
    char Name1[255];
    char Name2[255];

    // Hit data
    TCanvas* c_Z = new TCanvas("Charge_z_correlation", "Charge Z correlation", 10, 10, 800, 700);
    fh2_hit_z = new TH2F("fh2_Twim_vs_Music_charge_z", "Twim vs Music: Charge Z", 1000, 6, 38, 1000, 6, 38);
    fh2_hit_z->GetXaxis()->SetTitle("Music charge (Z)");
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
    fh2_hit_theta = new TH2F("fh2_Twim_vs_Music_theta", "Twim vs Music: #theta_{XZ}", 900, -30, 30, 900, -30, 30);
    fh2_hit_theta->GetXaxis()->SetTitle("Music #theta_{XZ} [mrad]");
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
    TFolder* mainfolTwim = new TFolder("TWIM_vs_MUSIC", "TWIM vs MUSIC info");
    if (fHitItemsTwim && fHitItemsMusic)
    {
        mainfolTwim->Add(c_Z);
        mainfolTwim->Add(c_theta);
    }
    run->AddObject(mainfolTwim);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_TWIMvsMUSIC_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofTwimvsMusicOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofTwimvsMusicOnlineSpectra::Reset_Histo";

    if (fHitItemsTwim && fHitItemsMusic)
    {
        fh2_hit_z->Reset();
        fh2_hit_theta->Reset();
    }
}

void R3BSofTwimvsMusicOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTwimvsMusicOnlineSpectra::Exec FairRootManager not found";

    // Fill hit data
    if (fHitItemsTwim && fHitItemsTwim->GetEntriesFast() > 0 && fHitItemsMusic && fHitItemsMusic->GetEntriesFast() > 0)
    {
        Float_t z1 = 0., z2 = 0., theta1 = 0., theta2 = 0.;
        // MUSIC
        Int_t nHits1 = fHitItemsMusic->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits1; ihit++)
        {
            R3BMusicHitData* hit = (R3BMusicHitData*)fHitItemsMusic->At(ihit);
            if (!hit)
                continue;
            z1 = hit->GetZcharge();
            theta1 = hit->GetTheta() * 1000.; // mrad
        }
        // TWIM
        Int_t nHits2 = fHitItemsTwim->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits2; ihit++)
        {
            R3BSofTwimHitData* hit = (R3BSofTwimHitData*)fHitItemsTwim->At(ihit);
            if (!hit)
                continue;
            z2 = hit->GetZcharge();
            theta2 = hit->GetTheta() * 1000.; // mrad
        }
        // Fill histograms
        fh2_hit_z->Fill(z1, z2);
        fh2_hit_theta->Fill(theta1, theta2);
    }

    fNEvents += 1;
}

void R3BSofTwimvsMusicOnlineSpectra::FinishEvent()
{
    if (fHitItemsMusic)
    {
        fHitItemsMusic->Clear();
    }
    if (fHitItemsTwim)
    {
        fHitItemsTwim->Clear();
    }
}

void R3BSofTwimvsMusicOnlineSpectra::FinishTask()
{
    if (fHitItemsTwim && fHitItemsMusic)
    {
        fh2_hit_z->Write();
        fh2_hit_theta->Write();
    }
}

ClassImp(R3BSofTwimvsMusicOnlineSpectra)
