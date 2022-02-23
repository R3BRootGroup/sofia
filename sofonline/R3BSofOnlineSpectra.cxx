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
#include "R3BSofCorrOnlineSpectra.h"
#include "R3BSofFrsOnlineSpectra.h"
#include "R3BMwpcCorrelationOnlineSpectra.h"
#include "R3BMwpcOnlineSpectra.h"
#include "R3BSofScalersOnlineSpectra.h"
#include "R3BSofSciOnlineSpectra.h"
#include "R3BSofSciVsMusicOnlineSpectra.h"
#include "R3BSofSciVsMwpc0OnlineSpectra.h"
#include "R3BSofSciVsTrimOnlineSpectra.h"
#include "R3BSofTofWOnlineSpectra.h"
#include "R3BSofTrackingFissionOnlineSpectra.h"
#include "R3BSofTrackingOnlineSpectra.h"
#include "R3BSofTrimOnlineSpectra.h"
#include "R3BSofTrimVsTofwOnlineSpectra.h"
#include "R3BTwimOnlineSpectra.h"
#include "R3BSofTwimvsMusicOnlineSpectra.h"
#include "R3BSofTwimvsTrimOnlineSpectra.h"
#include "R3BWRData.h"
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
    , fTwimVsMusicOnline(NULL)
    , fTwimVsTrimOnline(NULL)
    , fSciOnline(NULL)
    , fSciVsMusOnline(NULL)
    , fSciVsTrimOnline(NULL)
    , fSciVsMw0Online(NULL)
    , fTofWOnline(NULL)
    , fTrimVsTofwOnline(NULL)
    , fScalersOnline(NULL)
    , fMusicOnline(NULL)
    , fAmsOnline(NULL)
    , fCalifaOnline(NULL)
    , fFrsOnline(NULL)
    , fTrackOnline(NULL)
    , fTrackFFOnline(NULL)
    , fCorrOnline(NULL)
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
    , fTwimVsMusicOnline(NULL)
    , fTwimVsTrimOnline(NULL)
    , fSciOnline(NULL)
    , fSciVsMusOnline(NULL)
    , fSciVsTrimOnline(NULL)
    , fSciVsMw0Online(NULL)
    , fTofWOnline(NULL)
    , fTrimVsTofwOnline(NULL)
    , fScalersOnline(NULL)
    , fMusicOnline(NULL)
    , fAmsOnline(NULL)
    , fCalifaOnline(NULL)
    , fFrsOnline(NULL)
    , fTrackOnline(NULL)
    , fTrackFFOnline(NULL)
    , fCorrOnline(NULL)
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
    LOG(INFO) << "R3BSofOnlineSpectra::Init()";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofOnlineSpectra::Init FairRootManager not found";
    fEventHeader = (R3BEventHeader*)mgr->GetObject("EventHeader.");

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

    // Looking Correlation of DAQ subsystem online
    fCorrOnline = (R3BSofCorrOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofCorrOnlineSpectra");
    if (!fCorrOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofCorrOnlineSpectra not found";

    // Looking for AT online
    fAtOnline = (R3BSofAtOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofAtOnlineSpectra");
    if (!fAtOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofAtOnlineSpectra not found";

    // Looking for Mwpc0 online
    fMwpc0Online = (R3BMwpcOnlineSpectra*)FairRunOnline::Instance()->GetTask("Mwpc0OnlineSpectra");
    if (!fMwpc0Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::Mwpc0OnlineSpectra not found";

    // Looking for Mwpc0_1 online
    fMwpc01Online =
        (R3BMwpcCorrelationOnlineSpectra*)FairRunOnline::Instance()->GetTask("Mwpc0_1CorrelationOnlineSpectra");
    if (!fMwpc01Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::Mwpc0_1CorrelationOnlineSpectra not found";

    // Looking for Mwpc0_2 online
    fMwpc02Online =
        (R3BMwpcCorrelationOnlineSpectra*)FairRunOnline::Instance()->GetTask("Mwpc0_2CorrelationOnlineSpectra");
    if (!fMwpc02Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::Mwpc0_2CorrelationOnlineSpectra not found";

    // Looking for Mwpc1_2 online
    fMwpc12Online =
        (R3BMwpcCorrelationOnlineSpectra*)FairRunOnline::Instance()->GetTask("Mwpc1_2CorrelationOnlineSpectra");
    if (!fMwpc12Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::Mwpc1_2CorrelationOnlineSpectra not found";

    // Looking for Mwpc2_3 online
    fMwpc23Online =
        (R3BMwpcCorrelationOnlineSpectra*)FairRunOnline::Instance()->GetTask("Mwpc2_3CorrelationOnlineSpectra");
    if (!fMwpc23Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::Mwpc0_1CorrelationOnlineSpectra not found";

    fFrsOnline = (R3BSofFrsOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofFrsOnlineSpectra");
    if (!fFrsOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofFrsOnlineSpectra not found";

    // Looking for Mwpc1 online
    fMwpc1Online = (R3BMwpcOnlineSpectra*)FairRunOnline::Instance()->GetTask("Mwpc1OnlineSpectra");
    if (!fMwpc1Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::Mwpc1OnlineSpectra not found";

    // Looking for Mwpc2 online
    fMwpc2Online = (R3BMwpcOnlineSpectra*)FairRunOnline::Instance()->GetTask("Mwpc2OnlineSpectra");
    if (!fMwpc2Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::Mwpc2OnlineSpectra not found";

    // Looking for Mwpc3 online
    fMwpc3Online = (R3BMwpcOnlineSpectra*)FairRunOnline::Instance()->GetTask("Mwpc3OnlineSpectra");
    if (!fMwpc3Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::Mwpc3OnlineSpectra not found";

    // Looking for Trim online
    fTrimOnline = (R3BSofTrimOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofTrimOnlineSpectra");
    if (!fTrimOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofTrimOnlineSpectra not found";

    // Looking for Twim online
    fTwimOnline = (R3BTwimOnlineSpectra*)FairRunOnline::Instance()->GetTask("TwimOnlineSpectra");
    if (!fTwimOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::TwimOnlineSpectra not found";

    // Looking for Twim vs Music online
    fTwimVsMusicOnline =
        (R3BSofTwimvsMusicOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofTwimvsMusicOnlineSpectra");
    if (!fTwimVsMusicOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofTwimvsMusicOnlineSpectra not found";

    // Looking for Twim vs Trim online
    fTwimVsTrimOnline =
        (R3BSofTwimvsTrimOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofTwimvsTrimOnlineSpectra");
    if (!fTwimVsTrimOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofTwimvsTrimOnlineSpectra not found";

    // Looking for Sci online
    fSciOnline = (R3BSofSciOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofSciOnlineSpectra");
    if (!fSciOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofSciOnlineSpectra not found";

    // Looking for Sci Vs R3B_MUSIC online
    fSciVsMusOnline = (R3BSofSciVsMusicOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofSciVsMusicOnlineSpectra");
    if (!fSciVsMusOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofSciVsMusicOnlineSpectra not found";

    // Looking for Sci Vs TRIPLE-MUSIC online
    fSciVsTrimOnline = (R3BSofSciVsTrimOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofSciVsTrimOnlineSpectra");
    if (!fSciVsTrimOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofSciVsTrimOnlineSpectra not found";

    // Looking for Sci Vs MWPC0 online
    fSciVsMw0Online = (R3BSofSciVsMwpc0OnlineSpectra*)FairRunOnline::Instance()->GetTask("SofSciVsMwpc0OnlineSpectra");
    if (!fSciVsMw0Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofSciVsMwpc0OnlineSpectra not found";

    // Looking for TofW online
    fTofWOnline = (R3BSofTofWOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofTofWOnlineSpectra");
    if (!fTofWOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofTofWOnlineSpectra not found";

    // Looking for Trim vs Tofw online
    fTrimVsTofwOnline =
        (R3BSofTrimVsTofwOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofTrimVsTofwOnlineSpectra");
    if (!fTrimVsTofwOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofTrimVsTofwOnlineSpectra not found";

    // Looking for Scalers online
    fScalersOnline = (R3BSofScalersOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofScalersOnlineSpectra");
    if (!fScalersOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofScalersOnlineSpectra not found";

    // Looking for Music online
    fMusicOnline = (R3BMusicOnlineSpectra*)FairRunOnline::Instance()->GetTask("MusicOnlineSpectra");
    if (!fMusicOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::MusicOnlineSpectra not found";

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

    // Looking for Fission Tracking online
    fTrackFFOnline =
        (R3BSofTrackingFissionOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofTrackingFissionOnlineSpectra");
    if (!fTrackFFOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::SofTrackingFissionOnlineSpectra not found";

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
    fh1_wr[0] = new TH1F(
        "fh1_WRSE_Master_Sofia", "WR-Master - WRSE-Sofia(back) , WR-Master - WRME-Sofia(red) ", 1200, -4100, 4100);
    fh1_wr[0]->GetXaxis()->SetTitle("WRs difference");
    fh1_wr[0]->GetYaxis()->SetTitle("Counts");
    fh1_wr[0]->GetYaxis()->SetTitleOffset(1.3);
    fh1_wr[0]->GetXaxis()->CenterTitle(true);
    fh1_wr[0]->GetYaxis()->CenterTitle(true);
    fh1_wr[0]->SetFillColor(29);
    fh1_wr[0]->SetLineColor(1);
    fh1_wr[0]->SetLineWidth(2);
    fh1_wr[0]->Draw("");
    fh1_wr[1] = new TH1F("fh1_WRME_Master_Sofia", "WR-Master - WRME-Sofia", 1200, -4100, 4100);
    fh1_wr[1]->SetLineColor(2);
    fh1_wr[1]->SetLineWidth(2);
    fh1_wr[1]->Draw("same");

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
    {
        fh1_wr[0]->Reset();
        fh1_wr[1]->Reset();
    }
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
    // Reset Twim-Trim histograms if they exist somewhere
    if (fTwimVsTrimOnline)
        fTwimVsTrimOnline->Reset_Histo();
    // Reset Twim-Music histograms if they exist somewhere
    if (fTwimVsMusicOnline)
        fTwimVsMusicOnline->Reset_Histo();
    // Reset Sci histograms if they exist somewhere
    if (fSciOnline)
        fSciOnline->Reset_Histo();
    // Reset Sci vs R3B_MUSIC histograms if they exist somewhere
    if (fSciVsMusOnline)
        fSciVsMusOnline->Reset_Histo();
    // Reset Sci vs TRIPLE-MUSIC histograms if they exist somewhere
    if (fSciVsTrimOnline)
        fSciVsTrimOnline->Reset_Histo();
    // Reset Sci vs MWPC0 histograms if they exist somewhere
    if (fSciVsMw0Online)
        fSciVsMw0Online->Reset_Histo();
    // Reset Trim vs Tofw histograms if they exist somewhere
    if (fTrimVsTofwOnline)
        fTrimVsTofwOnline->Reset_Histo();
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
    // Reset Fission Tracking histograms if they exist somewhere
    if (fTrackFFOnline)
        fTrackFFOnline->Reset_Histo();
    // Reset Corr histograms if they exist somewhere
    if (fCorrOnline)
        fCorrOnline->Reset_Histo();
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
        int64_t wrs[2];
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BWRData* hit = (R3BWRData*)fWRItemsSofia->At(ihit);
            if (!hit)
                continue;
            wrs[ihit] = hit->GetTimeStamp();
        }

        // Califa
        if (fWRItemsCalifa && fWRItemsCalifa->GetEntriesFast() > 0)
        {
            nHits = fWRItemsCalifa->GetEntriesFast();
            int64_t wr[nHits];
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BWRData* hit = (R3BWRData*)fWRItemsCalifa->At(ihit);
                if (!hit)
                    continue;
                wr[ihit] = hit->GetTimeStamp();
            }
            fh1_wrs[0]->Fill(wrs[0] - wr[0]); // messel
            fh1_wrs[1]->Fill(wrs[0] - wr[1]); // wixhausen
        }
        // Neuland
        if (fWRItemsNeuland && fWRItemsNeuland->GetEntriesFast() > 0)
        {
            nHits = fWRItemsNeuland->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BWRData* hit = (R3BWRData*)fWRItemsNeuland->At(ihit);
                if (!hit)
                    continue;
                fh1_wrs[2]->Fill(int64_t(wrs[0] - hit->GetTimeStamp()));
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
                R3BWRData* hit = (R3BWRData*)fWRItemsS2->At(ihit);
                if (!hit)
                    continue;
                fh1_wrs[3]->Fill(int64_t(wrs[0] - hit->GetTimeStamp()));
            }
        }
        // S8
        if (fWRItemsS8 && fWRItemsS8->GetEntriesFast() > 0)
        {
            nHits = fWRItemsS8->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BWRData* hit = (R3BWRData*)fWRItemsS8->At(ihit);
                if (!hit)
                    continue;
                fh1_wrs[4]->Fill(int64_t(wrs[0] - hit->GetTimeStamp()));
            }
        }
        // Master
        if (fWRItemsMaster && fWRItemsMaster->GetEntriesFast() > 0)
        {
            nHits = fWRItemsMaster->GetEntriesFast();
            int64_t wrm = 0.;
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BWRData* hit = (R3BWRData*)fWRItemsMaster->At(ihit);
                if (!hit)
                    continue;
                wrm = hit->GetTimeStamp();
            }
            fh1_wr[0]->Fill(wrm - wrs[0]);
            fh1_wr[1]->Fill(wrm - wrs[1]);
        }
    }
    fNEvents += 1;
    return;
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
