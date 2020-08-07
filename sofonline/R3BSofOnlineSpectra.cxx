// ------------------------------------------------------------
// -----                  R3BSofOnlineSpectra             -----
// -----    Created 29/09/19  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofOnlineSpectra.h"
#include "R3BAmsOnlineSpectra.h"
#include "R3BCalifaOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BMusicOnlineSpectra.h"
#include "R3BSofAtOnlineSpectra.h"
#include "R3BSofFrsOnlineSpectra.h"
#include "R3BSofMwpcCorrelationOnlineSpectra.h"
#include "R3BSofMwpcOnlineSpectra.h"
#include "R3BSofScalersOnlineSpectra.h"
#include "R3BSofSciOnlineSpectra.h"
#include "R3BSofTofWOnlineSpectra.h"
#include "R3BSofTrackingOnlineSpectra.h"
#include "R3BSofTrimOnlineSpectra.h"
#include "R3BSofTwimOnlineSpectra.h"
#include "R3BWRCalifaData.h"
#include "R3BWRMasterData.h"
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
#include "TLegend.h"
#include "TLegendEntry.h"
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

R3BSofOnlineSpectra::R3BSofOnlineSpectra()
    : FairTask("SofiaOnlineSpectra", 1)
    , fEventHeader(nullptr)
    , fAtOnline(NULL)
    , fMwpc0Online(NULL)
    , fMwpc01Online(NULL)
    , fMwpc02Online(NULL)
    , fMwpc12Online(NULL)
    , fMwpc23Online(NULL)
    , fMwpc1Online(NULL)
    , fMwpc2Online(NULL)
    , fMwpc3Online(NULL)
    , fTrimOnline(NULL)
    , fTwimOnline(NULL)
    , fSciOnline(NULL)
    , fTofWOnline(NULL)
    , fScalersOnline(NULL)
    , fMusicOnline(NULL)
    , fAmsOnline(NULL)
    , fCalifaOnline(NULL)
    , fFrsOnline(NULL)
    , fTrackOnline(NULL)
    , fWRItemsMaster(NULL)
    , fWRItemsSofia(NULL)
    , fWRItemsCalifa(NULL)
    , fWRItemsNeuland(NULL)
    , fWRItemsS2(NULL)
    , fWRItemsS8(NULL)
    , fNEvents(0)
{
}

R3BSofOnlineSpectra::R3BSofOnlineSpectra(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fEventHeader(nullptr)
    , fAtOnline(NULL)
    , fMwpc0Online(NULL)
    , fMwpc01Online(NULL)
    , fMwpc02Online(NULL)
    , fMwpc12Online(NULL)
    , fMwpc23Online(NULL)
    , fMwpc1Online(NULL)
    , fMwpc2Online(NULL)
    , fMwpc3Online(NULL)
    , fTrimOnline(NULL)
    , fTwimOnline(NULL)
    , fSciOnline(NULL)
    , fTofWOnline(NULL)
    , fScalersOnline(NULL)
    , fMusicOnline(NULL)
    , fAmsOnline(NULL)
    , fCalifaOnline(NULL)
    , fFrsOnline(NULL)
    , fTrackOnline(NULL)
    , fWRItemsMaster(NULL)
    , fWRItemsSofia(NULL)
    , fWRItemsCalifa(NULL)
    , fWRItemsNeuland(NULL)
    , fWRItemsS2(NULL)
    , fWRItemsS8(NULL)
    , fNEvents(0)
{
}

R3BSofOnlineSpectra::~R3BSofOnlineSpectra()
{

    LOG(INFO) << "R3BSofOnlineSpectra::Delete instance";
    if (fWRItemsMaster)
        delete fWRItemsMaster;
    if (fWRItemsSofia)
        delete fWRItemsSofia;
    if (fWRItemsCalifa)
        delete fWRItemsCalifa;
    if (fWRItemsNeuland)
        delete fWRItemsNeuland;
    if (fWRItemsS2)
        delete fWRItemsS2;
    if (fWRItemsS8)
        delete fWRItemsS8;
}

InitStatus R3BSofOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofOnlineSpectra::Init FairRootManager not found";
    fEventHeader = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    if (NULL == run)
        LOG(FATAL) << "R3BSofOnlineSpectra::Init FairRunOnline not found";
    run->GetHttpServer()->Register("", this);

    // get access to WR-Master data
    fWRItemsMaster = (TClonesArray*)mgr->GetObject("WRMasterData");
    if (!fWRItemsMaster)
    {
        LOG(WARNING) << "R3BSofOnlineSpectra::WRMasterData not found";
    }

    // get access to WR-Sofia data
    fWRItemsSofia = (TClonesArray*)mgr->GetObject("SofWRData");
    if (!fWRItemsSofia)
    {
        LOG(WARNING) << "R3BSofOnlineSpectra::SofWRData not found";
    }

    // get access to WR-Califa data
    fWRItemsCalifa = (TClonesArray*)mgr->GetObject("WRCalifaData");
    if (!fWRItemsCalifa)
    {
        LOG(WARNING) << "R3BSofOnlineSpectra::WRCalifaData not found";
    }

    // get access to WR-Neuland data
    fWRItemsNeuland = (TClonesArray*)mgr->GetObject("WRNeulandData");
    if (!fWRItemsNeuland)
    {
        LOG(WARNING) << "R3BSofOnlineSpectra::WRNeulandData not found";
    }

    // get access to WR-S2 data
    fWRItemsS2 = (TClonesArray*)mgr->GetObject("WRS2Data");
    if (!fWRItemsS2)
    {
        LOG(WARNING) << "R3BSofOnlineSpectra::WRS2Data not found";
    }

    // get access to WR-S8 data
    fWRItemsS8 = (TClonesArray*)mgr->GetObject("WRS8Data");
    if (!fWRItemsS8)
    {
        LOG(WARNING) << "R3BSofOnlineSpectra::WRS8Data not found";
    }

    // Looking for AT online
    fAtOnline = (R3BSofAtOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofAtOnlineSpectra");
    if (!fAtOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofAtOnlineSpectra not found";

    // Looking for Mwpc0 online
    fMwpc0Online = (R3BSofMwpcOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofMwpc0OnlineSpectra");
    if (!fMwpc0Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofMwpc0OnlineSpectra not found";

    // Looking for Mwpc0_1 online
    fMwpc01Online =
        (R3BSofMwpcCorrelationOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofMwpc0_1CorrelationOnlineSpectra");
    if (!fMwpc01Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofMwpc0_1CorrelationOnlineSpectra not found";

    // Looking for Mwpc0_2 online
    fMwpc02Online =
        (R3BSofMwpcCorrelationOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofMwpc0_2CorrelationOnlineSpectra");
    if (!fMwpc02Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofMwpc0_2CorrelationOnlineSpectra not found";

    // Looking for Mwpc1_2 online
    fMwpc12Online =
        (R3BSofMwpcCorrelationOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofMwpc1_2CorrelationOnlineSpectra");
    if (!fMwpc12Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofMwpc1_2CorrelationOnlineSpectra not found";

    // Looking for Mwpc2_3 online
    fMwpc23Online =
        (R3BSofMwpcCorrelationOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofMwpc2_3CorrelationOnlineSpectra");
    if (!fMwpc23Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofMwpc0_1CorrelationOnlineSpectra not found";

    fFrsOnline = (R3BSofFrsOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofFrsOnlineSpectra");
    if (!fFrsOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofFrsOnlineSpectra not found";

    // Looking for Mwpc1 online
    fMwpc1Online = (R3BSofMwpcOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofMwpc1OnlineSpectra");
    if (!fMwpc1Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofMwpc1OnlineSpectra not found";

    // Looking for Mwpc2 online
    fMwpc2Online = (R3BSofMwpcOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofMwpc2OnlineSpectra");
    if (!fMwpc2Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofMwpc2OnlineSpectra not found";

    // Looking for Mwpc3 online
    fMwpc3Online = (R3BSofMwpcOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofMwpc3OnlineSpectra");
    if (!fMwpc3Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofMwpc3OnlineSpectra not found";

    // Looking for Trim online
    fTrimOnline = (R3BSofTrimOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofTrimOnlineSpectra");
    if (!fTrimOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofTrimOnlineSpectra not found";

    // Looking for Twim online
    fTwimOnline = (R3BSofTwimOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofTwimOnlineSpectra");
    if (!fTwimOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofTwimOnlineSpectra not found";

    // Looking for Sci online
    fSciOnline = (R3BSofSciOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofSciOnlineSpectra");
    if (!fSciOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofSciOnlineSpectra not found";

    // Looking for TofW online
    fTofWOnline = (R3BSofTofWOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofTofWOnlineSpectra");
    if (!fTofWOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofTofWOnlineSpectra not found";

    // Looking for Scalers online
    fScalersOnline = (R3BSofScalersOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofScalersOnlineSpectra");
    if (!fScalersOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofScalersOnlineSpectra not found";

    // Looking for Music online
    fMusicOnline = (R3BMusicOnlineSpectra*)FairRunOnline::Instance()->GetTask("R3BMusicOnlineSpectra");
    if (!fMusicOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::R3BMusicOnlineSpectra not found";

    // Looking for AMS online
    fAmsOnline = (R3BAmsOnlineSpectra*)FairRunOnline::Instance()->GetTask("AmsOnlineSpectra");
    if (!fAmsOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::AmsOnlineSpectra not found";

    // Looking for CALIFA online
    fCalifaOnline = (R3BCalifaOnlineSpectra*)FairRunOnline::Instance()->GetTask("CALIFAOnlineSpectra");
    if (!fCalifaOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::CALIFAOnlineSpectra not found";

    // Looking for Tracking online
    fTrackOnline = (R3BSofTrackingOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofTrackingOnlineSpectra");
    if (!fTrackOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofTrackingOnlineSpectra not found";

    // Create histograms for detectors
    char Name1[255];
    char Name2[255];
    char Name3[255];

    // Triggers
    cTrigger = new TCanvas("Triggers", "Trigger information", 10, 10, 800, 700);
    fh1_trigger = new TH1F("fh1_trigger", "Trigger information: Tpat", 17, -0.5, 16.5);
    fh1_trigger->GetXaxis()->SetTitle("Trigger number (tpat)");
    fh1_trigger->GetYaxis()->SetTitle("Counts");
    fh1_trigger->GetXaxis()->CenterTitle(true);
    fh1_trigger->GetYaxis()->CenterTitle(true);
    fh1_trigger->GetXaxis()->SetLabelSize(0.04);
    fh1_trigger->GetXaxis()->SetTitleSize(0.04);
    fh1_trigger->GetYaxis()->SetTitleOffset(1.1);
    fh1_trigger->GetXaxis()->SetTitleOffset(1.1);
    fh1_trigger->GetYaxis()->SetLabelSize(0.04);
    fh1_trigger->GetYaxis()->SetTitleSize(0.04);
    fh1_trigger->SetFillColor(kBlue + 2);
    fh1_trigger->Draw("");

    // Difference between master and sofia WRs
    cWr = new TCanvas("WR_Master_Sofia", "WR_Master_Sofia", 10, 10, 500, 500);
    fh1_wr = new TH1F("fh1_WR_Master_Sofia", "WR-Master - WR-Sofia", 1200, -4100, 4100);
    fh1_wr->GetXaxis()->SetTitle("WRs difference");
    fh1_wr->GetYaxis()->SetTitle("Counts");
    fh1_wr->GetYaxis()->SetTitleOffset(1.3);
    fh1_wr->GetXaxis()->CenterTitle(true);
    fh1_wr->GetYaxis()->CenterTitle(true);
    fh1_wr->SetFillColor(29);
    fh1_wr->SetLineColor(1);
    fh1_wr->SetLineWidth(2);
    fh1_wr->Draw("");

    // Difference between Califa-Sofia WRs
    sprintf(Name1, "WRs_Sofia_vs_others");
    cWrs = new TCanvas(Name1, Name1, 10, 10, 500, 500);
    sprintf(Name2, "fh1_WR_Sofia_Wixhausen");
    sprintf(Name3, "WR-Sofia - WR-Other"); // Messel (blue), Wixhausen (red)
    fh1_wrs[0] = new TH1F(Name2, Name3, 1200, -4100, 4100);
    fh1_wrs[0]->GetXaxis()->SetTitle("WRs difference");
    fh1_wrs[0]->GetYaxis()->SetTitle("Counts");
    fh1_wrs[0]->GetYaxis()->SetTitleOffset(1.3);
    fh1_wrs[0]->GetXaxis()->CenterTitle(true);
    fh1_wrs[0]->GetYaxis()->CenterTitle(true);
    fh1_wrs[0]->SetLineColor(2);
    fh1_wrs[0]->SetLineWidth(3);
    gPad->SetLogy(1);
    fh1_wrs[0]->Draw("");
    fh1_wrs[1] = new TH1F("fh1_WR_Sofia_Califa_Messel", "", 1200, -4100, 4100);
    fh1_wrs[1]->SetLineColor(4);
    fh1_wrs[1]->SetLineWidth(3);
    if (fWRItemsCalifa)
        fh1_wrs[1]->Draw("same");
    fh1_wrs[2] = new TH1F("fh1_WR_Sofia_Neuland", "", 1200, -4100, 4100);
    fh1_wrs[2]->SetLineColor(3);
    fh1_wrs[2]->SetLineWidth(3);
    if (fWRItemsNeuland)
        fh1_wrs[2]->Draw("same");
    fh1_wrs[3] = new TH1F("fh1_WR_Sofia_S2", "", 1200, -4100, 4100);
    fh1_wrs[3]->SetLineColor(1);
    fh1_wrs[3]->SetLineWidth(3);
    if (fWRItemsS2)
        fh1_wrs[3]->Draw("same");
    fh1_wrs[4] = new TH1F("fh1_WR_Sofia_S8", "", 1200, -4100, 4100);
    fh1_wrs[4]->SetLineColor(5);
    fh1_wrs[4]->SetLineWidth(3);
    if (fWRItemsS8)
        fh1_wrs[4]->Draw("same");

    TLegend* leg = new TLegend(0.05, 0.9, 0.39, 0.9999, NULL, "brNDC");
    leg->SetBorderSize(0);
    leg->SetTextFont(62);
    leg->SetTextSize(0.03);
    leg->SetLineColor(1);
    leg->SetLineStyle(1);
    leg->SetLineWidth(1);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    TLegendEntry* entry = leg->AddEntry("null", "Califa_Wixhausen", "l");
    entry->SetLineColor(4);
    entry->SetLineStyle(1);
    entry->SetLineWidth(3);
    entry->SetTextFont(62);
    entry = leg->AddEntry("null", "Califa_Messel", "l");
    entry->SetLineColor(2);
    entry->SetLineStyle(1);
    entry->SetLineWidth(3);
    entry->SetTextFont(62);
    if (fWRItemsNeuland)
    {
        entry = leg->AddEntry("null", "Neuland", "l");
        entry->SetLineColor(3);
        entry->SetLineStyle(1);
        entry->SetLineWidth(3);
        entry->SetTextFont(62);
    }
    if (fWRItemsS2)
    {
        entry = leg->AddEntry("null", "S2", "l");
        entry->SetLineColor(1);
        entry->SetLineStyle(1);
        entry->SetLineWidth(3);
        entry->SetTextFont(62);
    }
    if (fWRItemsS8)
    {
        entry = leg->AddEntry("null", "S8", "l");
        entry->SetLineColor(5);
        entry->SetLineStyle(1);
        entry->SetLineWidth(3);
        entry->SetTextFont(62);
    }
    leg->Draw();

    // MAIN FOLDER-SOFIA
    TFolder* mainfolsof = new TFolder("SOFIA", "SOFIA WhiteRabbit and trigger info");
    mainfolsof->Add(cTrigger);
    if (fWRItemsMaster && fWRItemsSofia)
        mainfolsof->Add(cWr);
    if (fWRItemsSofia && fWRItemsCalifa)
        mainfolsof->Add(cWrs);
    run->AddObject(mainfolsof);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_GENERAL_HIST", Form("/Objects/%s/->Reset_GENERAL_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofOnlineSpectra::Reset_GENERAL_Histo()
{
    LOG(INFO) << "R3BSofOnlineSpectra::Reset_General_Histo";
    fh1_trigger->Reset();
    if (fWRItemsMaster && fWRItemsSofia)
        fh1_wr->Reset();
    if (fWRItemsCalifa && fWRItemsSofia)
    {
        fh1_wrs[0]->Reset();
        fh1_wrs[1]->Reset();
        if (fWRItemsNeuland)
            fh1_wrs[2]->Reset();
        if (fWRItemsS2)
            fh1_wrs[3]->Reset();
        if (fWRItemsS8)
            fh1_wrs[4]->Reset();
    }
    // Reset AT histograms if they exist somewhere
    if (fAtOnline)
        fAtOnline->Reset_Histo();
    // Reset Mwpc0 histograms if they exist somewhere
    if (fMwpc0Online)
        fMwpc0Online->Reset_Histo();
    // Reset Mwpc0_1 histograms if they exist somewhere
    if (fMwpc01Online)
        fMwpc01Online->Reset_Histo();
    // Reset Mwpc0_2 histograms if they exist somewhere
    if (fMwpc02Online)
        fMwpc02Online->Reset_Histo();
    // Reset Mwpc1_2 histograms if they exist somewhere
    if (fMwpc12Online)
        fMwpc12Online->Reset_Histo();
    // Reset Mwpc2_3 histograms if they exist somewhere
    if (fMwpc23Online)
        fMwpc23Online->Reset_Histo();
    // Reset Mwpc1 histograms if they exist somewhere
    if (fMwpc1Online)
        fMwpc1Online->Reset_Histo();
    // Reset Mwpc2 histograms if they exist somewhere
    if (fMwpc2Online)
        fMwpc2Online->Reset_Histo();
    // Reset Mwpc3 histograms if they exist somewhere
    if (fMwpc3Online)
        fMwpc3Online->Reset_Histo();
    // Reset Trim histograms if they exist somewhere
    if (fTrimOnline)
        fTrimOnline->Reset_Histo();
    // Reset Twim histograms if they exist somewhere
    if (fTwimOnline)
        fTwimOnline->Reset_Histo();
    // Reset Sci histograms if they exist somewhere
    if (fSciOnline)
        fSciOnline->Reset_Histo();
    // Reset Scalers histograms if they exist somewhere
    if (fScalersOnline)
        fScalersOnline->Reset_Histo();
    // Reset TofW histograms if they exist somewhere
    if (fTofWOnline)
        fTofWOnline->Reset_Histo();
    // Reset Music histograms if they exist somewhere
    if (fMusicOnline)
        fMusicOnline->Reset_Histo();
    // Reset AMS histograms if they exist somewhere
    if (fAmsOnline)
        fAmsOnline->Reset_AMS_Histo();
    // Reset Califa histograms if they exist somewhere
    if (fCalifaOnline)
        fCalifaOnline->Reset_CALIFA_Histo();
    // Reset FRS histograms if they exist somewhere
    if (fFrsOnline)
        fFrsOnline->Reset_Histo();
    // Reset Tracking histograms if they exist somewhere
    if (fTrackOnline)
        fTrackOnline->Reset_Histo();
}

void R3BSofOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofOnlineSpectra::Exec FairRootManager not found";

    // Fill histogram with trigger information

    Int_t tpatbin;
    if (fEventHeader->GetTpat() > 0)
    {
        for (Int_t i = 0; i < 16; i++)
        {
            tpatbin = (fEventHeader->GetTpat() & (1 << i));
            if (tpatbin != 0)
                fh1_trigger->Fill(i + 1);
        }
    }
    else if (fEventHeader->GetTpat() == 0)
    {
        fh1_trigger->Fill(0);
    }
    else
    {
        LOG(INFO) << fNEvents << " " << fEventHeader->GetTpat();
    }
    // fh1_trigger->Fill(fEventHeader->GetTpat());

    // WR data
    if (fWRItemsSofia && fWRItemsSofia->GetEntriesFast() > 0)
    {
        // SOFIA
        Int_t nHits = fWRItemsSofia->GetEntriesFast();
        int64_t wrs = 0.;
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BWRMasterData* hit = (R3BWRMasterData*)fWRItemsSofia->At(ihit);
            if (!hit)
                continue;
            wrs = hit->GetTimeStamp();
        }

        // Califa
        if (fWRItemsCalifa && fWRItemsCalifa->GetEntriesFast() > 0)
        {
            nHits = fWRItemsCalifa->GetEntriesFast();
            int64_t wr[nHits];
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BWRCalifaData* hit = (R3BWRCalifaData*)fWRItemsCalifa->At(ihit);
                if (!hit)
                    continue;
                wr[ihit] = hit->GetTimeStamp();
            }
            fh1_wrs[0]->Fill(wrs - wr[0]); // messel
            fh1_wrs[1]->Fill(wrs - wr[1]); // wixhausen
        }
        // Neuland
        if (fWRItemsNeuland && fWRItemsNeuland->GetEntriesFast() > 0)
        {
            nHits = fWRItemsNeuland->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BWRMasterData* hit = (R3BWRMasterData*)fWRItemsNeuland->At(ihit);
                if (!hit)
                    continue;
                fh1_wrs[2]->Fill(int64_t(wrs - hit->GetTimeStamp()));
            }
            // fh1_wrs[4]->GetMaximum();
            fh1_wrs[0]->SetMaximum(5. * fh1_wrs[2]->GetBinContent(fh1_wrs[2]->GetMaximumBin()));
        }
        // S2
        if (fWRItemsS2 && fWRItemsS2->GetEntriesFast() > 0)
        {
            nHits = fWRItemsS2->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BWRMasterData* hit = (R3BWRMasterData*)fWRItemsS2->At(ihit);
                if (!hit)
                    continue;
                fh1_wrs[3]->Fill(int64_t(wrs - hit->GetTimeStamp()));
            }
        }
        // S8
        if (fWRItemsS8 && fWRItemsS8->GetEntriesFast() > 0)
        {
            nHits = fWRItemsS8->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BWRMasterData* hit = (R3BWRMasterData*)fWRItemsS8->At(ihit);
                if (!hit)
                    continue;
                fh1_wrs[4]->Fill(int64_t(wrs - hit->GetTimeStamp()));
            }
        }
        // Master
        if (fWRItemsMaster && fWRItemsMaster->GetEntriesFast() > 0)
        {
            nHits = fWRItemsMaster->GetEntriesFast();
            int64_t wrm = 0.;
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BWRMasterData* hit = (R3BWRMasterData*)fWRItemsMaster->At(ihit);
                if (!hit)
                    continue;
                wrm = hit->GetTimeStamp();
            }
            fh1_wr->Fill(wrm - wrs);
        }
    }
    fNEvents += 1;
}

void R3BSofOnlineSpectra::FinishEvent()
{

    if (fWRItemsMaster)
    {
        fWRItemsMaster->Clear();
    }
    if (fWRItemsSofia)
    {
        fWRItemsSofia->Clear();
    }
    if (fWRItemsCalifa)
    {
        fWRItemsCalifa->Clear();
    }
    if (fWRItemsNeuland)
    {
        fWRItemsNeuland->Clear();
    }
    if (fWRItemsS2)
    {
        fWRItemsS2->Clear();
    }
    if (fWRItemsS8)
    {
        fWRItemsS8->Clear();
    }
}

void R3BSofOnlineSpectra::FinishTask()
{
    // Write trigger canvas in the root file
    cTrigger->Write();
    if (fWRItemsMaster && fWRItemsSofia)
    {
        cWr->Write();
        cWrs->Write();
    }
}

ClassImp(R3BSofOnlineSpectra)
