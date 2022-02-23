// ------------------------------------------------------------
// -----             R3BSofStatusOnlineSpectra            -----
// -----    Created 15/03/21  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with detector status
 */

#include "R3BSofStatusOnlineSpectra.h"
#include "R3BAmsMappedData.h"
#include "R3BCalifaMappedData.h"
#include "R3BEventHeader.h"
#include "R3BMwpcMappedData.h"
#include "R3BSofTofWMappedData.h"
#include "R3BSofTrimMappedData.h"
#include "R3BTwimMappedData.h"
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

R3BSofStatusOnlineSpectra::R3BSofStatusOnlineSpectra()
    : FairTask("SofStatusOnlineSpectra", 1)
    , fEventHeader(nullptr)
    , fWRItemsMaster(NULL)
    , fWRItemsSofia(NULL)
    , fWRItemsAms(NULL)
    , fWRItemsCalifa(NULL)
    , fWRItemsNeuland(NULL)
    , fWRItemsS2(NULL)
    , fWRItemsS8(NULL)
    , fNEvents(0)
    , fMwpc0MappedDataCA(NULL)
    , fTrimMappedData(NULL)
    , fMappedItemsCalifa(NULL)
    , fMappedItemsAms(NULL)
    , fMwpc1MappedDataCA(NULL)
    , fTwimMappedDataCA(NULL)
    , fMwpc2MappedDataCA(NULL)
    , fMwpc3MappedDataCA(NULL)
    , fTofWMappedDataCA(NULL)
{
}

R3BSofStatusOnlineSpectra::R3BSofStatusOnlineSpectra(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fEventHeader(nullptr)
    , fWRItemsMaster(NULL)
    , fWRItemsSofia(NULL)
    , fWRItemsAms(NULL)
    , fWRItemsCalifa(NULL)
    , fWRItemsNeuland(NULL)
    , fWRItemsS2(NULL)
    , fWRItemsS8(NULL)
    , fNEvents(0)
    , fMwpc0MappedDataCA(NULL)
    , fTrimMappedData(NULL)
    , fMappedItemsCalifa(NULL)
    , fMappedItemsAms(NULL)
    , fMwpc1MappedDataCA(NULL)
    , fTwimMappedDataCA(NULL)
    , fMwpc2MappedDataCA(NULL)
    , fMwpc3MappedDataCA(NULL)
    , fTofWMappedDataCA(NULL)
{
}

R3BSofStatusOnlineSpectra::~R3BSofStatusOnlineSpectra()
{
    LOG(INFO) << "R3BSofStatusOnlineSpectra::Delete instance";
    if (fWRItemsMaster)
        delete fWRItemsMaster;
    if (fWRItemsSofia)
        delete fWRItemsSofia;
    if (fWRItemsAms)
        delete fWRItemsAms;
    if (fWRItemsCalifa)
        delete fWRItemsCalifa;
    if (fWRItemsNeuland)
        delete fWRItemsNeuland;
    if (fWRItemsS2)
        delete fWRItemsS2;
    if (fWRItemsS8)
        delete fWRItemsS8;
    if (fMwpc0MappedDataCA)
        delete fMwpc0MappedDataCA;
    if (fTrimMappedData)
        delete fTrimMappedData;
    if (fMappedItemsCalifa)
        delete fMappedItemsCalifa;
    if (fMappedItemsAms)
        delete fMappedItemsAms;
    if (fMwpc1MappedDataCA)
        delete fMwpc1MappedDataCA;
    if (fTwimMappedDataCA)
        delete fTwimMappedDataCA;
    if (fMwpc2MappedDataCA)
        delete fMwpc2MappedDataCA;
    if (fMwpc3MappedDataCA)
        delete fMwpc3MappedDataCA;
    if (fTofWMappedDataCA)
        delete fTofWMappedDataCA;
}

InitStatus R3BSofStatusOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofStatusOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofStatusOnlineSpectra::Init FairRootManager not found";
    fEventHeader = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    if (NULL == run)
        LOG(FATAL) << "R3BSofStatusOnlineSpectra::Init FairRunOnline not found";
    run->GetHttpServer()->Register("", this);

    // get access to WR-Master data
    fWRItemsMaster = (TClonesArray*)mgr->GetObject("WRMasterData");
    if (!fWRItemsMaster)
    {
        LOG(WARNING) << "R3BSofStatusOnlineSpectra::WRMasterData not found";
    }

    // get access to WR-Sofia data
    fWRItemsSofia = (TClonesArray*)mgr->GetObject("SofWRData");
    if (!fWRItemsSofia)
    {
        LOG(WARNING) << "R3BSofStatusOnlineSpectra::SofWRData not found";
    }

    // get access to WR-Ams data
    fWRItemsAms = (TClonesArray*)mgr->GetObject("WRAmsData");
    if (!fWRItemsAms)
    {
        LOG(WARNING) << "R3BSofStatusOnlineSpectra::WRAmsData not found";
    }

    // get access to WR-Califa data
    fWRItemsCalifa = (TClonesArray*)mgr->GetObject("WRCalifaData");
    if (!fWRItemsCalifa)
    {
        LOG(WARNING) << "R3BSofStatusOnlineSpectra::WRCalifaData not found";
    }

    // get access to WR-Neuland data
    fWRItemsNeuland = (TClonesArray*)mgr->GetObject("WRNeulandData");
    if (!fWRItemsNeuland)
    {
        LOG(WARNING) << "R3BSofStatusOnlineSpectra::WRNeulandData not found";
    }

    // get access to WR-S2 data
    fWRItemsS2 = (TClonesArray*)mgr->GetObject("WRS2Data");
    if (!fWRItemsS2)
    {
        LOG(WARNING) << "R3BSofStatusOnlineSpectra::WRS2Data not found";
    }

    // get access to WR-S8 data
    fWRItemsS8 = (TClonesArray*)mgr->GetObject("WRS8Data");
    if (!fWRItemsS8)
    {
        LOG(WARNING) << "R3BSofStatusOnlineSpectra::WRS8Data not found";
    }

    fMwpc0MappedDataCA = (TClonesArray*)mgr->GetObject("Mwpc0MappedData");
    if (!fMwpc0MappedDataCA)
    {
        return kFATAL;
    }

    fTrimMappedData = (TClonesArray*)mgr->GetObject("TrimMappedData");
    if (!fTrimMappedData)
    {
        return kFATAL;
    }

    fMappedItemsCalifa = (TClonesArray*)mgr->GetObject("CalifaMappedData");
    if (!fMappedItemsCalifa)
    {
        LOG(ERROR) << "R3BSofStatusOnlineSpectra::Couldn't get handle on CalifaMappedData.";
    }

    fMappedItemsAms = (TClonesArray*)mgr->GetObject("AmsMappedData");
    if (!fMappedItemsAms)
    {
        LOG(ERROR) << "R3BSofStatusOnlineSpectra::Couldn't get handle on AmsMappedData.";
    }

    fMwpc1MappedDataCA = (TClonesArray*)mgr->GetObject("Mwpc1MappedData");
    if (!fMwpc1MappedDataCA)
    {
        return kFATAL;
    }

    fTwimMappedDataCA = (TClonesArray*)mgr->GetObject("TwimMappedData");
    if (!fTwimMappedDataCA)
    {
        return kFATAL;
    }

    fMwpc2MappedDataCA = (TClonesArray*)mgr->GetObject("Mwpc2MappedData");
    if (!fMwpc2MappedDataCA)
    {
        return kFATAL;
    }
    fMwpc3MappedDataCA = (TClonesArray*)mgr->GetObject("Mwpc3MappedData");
    if (!fMwpc3MappedDataCA)
    {
        return kFATAL;
    }

    fTofWMappedDataCA = (TClonesArray*)mgr->GetObject("SofTofWMappedData");
    if (!fTofWMappedDataCA)
    {
        return kFATAL;
    }

    for (int i = 0; i < 16; i++)
        fCounterTpats[i] = 0;

    for (int i = 0; i < 10; i++)
        fCounterDet[i] = 0;

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
    gPad->SetLogy(1);
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
    stack_wrs = new THStack("stack_wrs", "WR-Sofia - WR-Other");
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
    stack_wrs->Add(fh1_wrs[0]); //->Draw("");
    fh1_wrs[1] = new TH1F("fh1_WR_Sofia_Califa_Messel", "", 1200, -4100, 4100);
    fh1_wrs[1]->SetLineColor(4);
    fh1_wrs[1]->SetLineWidth(3);
    if (fWRItemsCalifa)
        stack_wrs->Add(fh1_wrs[1]); //->Draw("same");
    fh1_wrs[2] = new TH1F("fh1_WR_Sofia_Neuland", "", 1200, -4100, 4100);
    fh1_wrs[2]->SetLineColor(3);
    fh1_wrs[2]->SetLineWidth(3);
    if (fWRItemsNeuland)
        stack_wrs->Add(fh1_wrs[2]); //->Draw("same");
    fh1_wrs[3] = new TH1F("fh1_WR_Sofia_S2", "", 1200, -4100, 4100);
    fh1_wrs[3]->SetLineColor(1);
    fh1_wrs[3]->SetLineWidth(3);
    if (fWRItemsS2)
        stack_wrs->Add(fh1_wrs[3]); //->Draw("same");
    fh1_wrs[4] = new TH1F("fh1_WR_Sofia_S8", "", 1200, -4100, 4100);
    fh1_wrs[4]->SetLineColor(5);
    fh1_wrs[4]->SetLineWidth(3);
    if (fWRItemsS8)
        stack_wrs->Add(fh1_wrs[4]); //->Draw("same");
    fh1_wrs[5] = new TH1F("fh1_WR_Sofia_Ams", "", 1200, -4100, 4100);
    fh1_wrs[5]->SetLineColor(7);
    fh1_wrs[5]->SetLineWidth(3);
    if (fWRItemsAms)
        stack_wrs->Add(fh1_wrs[5]); //->Draw("same");
    stack_wrs->Draw("nostack");

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
    if (fWRItemsAms)
    {
        entry = leg->AddEntry("null", "AMS", "l");
        entry->SetLineColor(7);
        entry->SetLineStyle(1);
        entry->SetLineWidth(3);
        entry->SetTextFont(62);
    }
    leg->Draw();

    sprintf(Name1, "Detector_Status");
    cDetGeneralView = new TCanvas(Name1, Name1, 10, 10, 800, 700);
    cDetGeneralView->SetBottomMargin(0.15);

    gh = new TGraph(10);
    gh->SetTitle("Detector efficiency with respect TPats 1-8");
    gh->SetMarkerStyle(20);
    gh->SetMarkerSize(2);
    gh->SetMarkerColor(2);

    fh1_GeneralView = new TH1F("Detector_eff", "Detector efficiency with respect TPats 1-8", 10, 0.5, 10.5);
    fh1_GeneralView->SetMinimum(0.);
    fh1_GeneralView->SetMaximum(1.2);
    fh1_GeneralView->GetXaxis()->CenterTitle(true);
    fh1_GeneralView->GetYaxis()->CenterTitle(true);
    fh1_GeneralView->GetXaxis()->SetLabelSize(0.045);
    fh1_GeneralView->GetXaxis()->SetTitleSize(0.045);
    fh1_GeneralView->GetYaxis()->SetLabelSize(0.045);
    fh1_GeneralView->GetYaxis()->SetTitleSize(0.045);
    gPad->SetGridx(1);
    gPad->SetGridy(1);

    fh1_GeneralView->GetXaxis()->SetBinLabel(1, "Mw0");
    fh1_GeneralView->GetXaxis()->SetBinLabel(2, "Trim");
    fh1_GeneralView->GetXaxis()->SetBinLabel(3, "Califa");
    fh1_GeneralView->GetXaxis()->SetBinLabel(4, "Ams");
    fh1_GeneralView->GetXaxis()->SetBinLabel(5, "Mw1");
    fh1_GeneralView->GetXaxis()->SetBinLabel(6, "Twim");
    fh1_GeneralView->GetXaxis()->SetBinLabel(7, "Mw2");
    fh1_GeneralView->GetXaxis()->SetBinLabel(8, "Mw3");
    fh1_GeneralView->GetXaxis()->SetBinLabel(9, "TofW");
    fh1_GeneralView->GetXaxis()->SetBinLabel(10, "NeuLand");
    fh1_GeneralView->GetXaxis()->SetLabelOffset(0);
    fh1_GeneralView->GetXaxis()->SetTitleOffset(2.2);
    gh->SetHistogram(fh1_GeneralView);
    gh->Draw("ap");

    cTotNbTrig = new TCanvas("Display_triggers", "Trigger display", 10, 10, 800, 700);
    cTotNbTrig->cd();
    fh1_display = new TH1F("Display", "Display trigger info (Total events)", 10, 0., 10.);
    fh1_display->GetXaxis()->SetLabelSize(0.0);
    fh1_display->GetYaxis()->SetLabelSize(0.0);
    fh1_display->GetXaxis()->SetAxisColor(0);
    fh1_display->GetYaxis()->SetAxisColor(0);
    fh1_display->SetLineColor(0);
    fh1_display->Draw();

    tex1 = new TLatex(1, 0.8, "Start: ");
    tex1->SetTextFont(42);
    tex1->SetTextSize(0.1);
    tex1->SetLineWidth(2);
    tex1->Draw();

    tex2 = new TLatex(1, 0.6, "Start+Fission: ");
    tex2->SetTextFont(42);
    tex2->SetTextSize(0.1);
    tex2->SetLineWidth(2);
    tex2->Draw();

    tex3 = new TLatex(1, 0.4, "Start+p2p: ");
    tex3->SetTextFont(42);
    tex3->SetTextSize(0.1);
    tex3->SetLineWidth(2);
    tex3->Draw();

    tex4 = new TLatex(1, 0.2, "Start+Fission+p2p: ");
    tex4->SetTextFont(42);
    tex4->SetTextSize(0.1);
    tex4->SetLineWidth(2);
    tex4->Draw();

    // MAIN FOLDER-SOFIA
    TFolder* mainfolsof = new TFolder("Status", "SOFIA WhiteRabbit, detectors and trigger info");
    mainfolsof->Add(cTrigger);
    mainfolsof->Add(cDetGeneralView);
    mainfolsof->Add(cTotNbTrig);
    if (fWRItemsMaster && fWRItemsSofia)
        mainfolsof->Add(cWr);
    if (fWRItemsSofia && fWRItemsCalifa)
        mainfolsof->Add(cWrs);
    run->AddObject(mainfolsof);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_GENERAL_HIST", Form("/Objects/%s/->Reset_Status_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofStatusOnlineSpectra::Reset_GENERAL_Histo()
{
    LOG(INFO) << "R3BSofStatusOnlineSpectra::Reset_General_Histo";
    fh1_trigger->Reset();
    fh1_GeneralView->Reset();
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
        if (fWRItemsAms)
            fh1_wrs[5]->Reset();
    }
}

void R3BSofStatusOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofStatusOnlineSpectra::Exec FairRootManager not found";

    // Fill histogram with trigger information

    Int_t tpatbin;
    bool ok = kFALSE;
    if (fEventHeader->GetTpat() > 0)
    {
        for (Int_t i = 0; i < 16; i++)
        {
            tpatbin = (fEventHeader->GetTpat() & (1 << i));
            if (tpatbin != 0)
            {
                fh1_trigger->Fill(i + 1);
                fCounterTpats[i]++;
                if (i + 1 > 0 && i + 1 < 5)
                    ok = true;
            }
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
        // AMS
        if (fWRItemsAms && fWRItemsAms->GetEntriesFast() > 0)
        {
            nHits = fWRItemsAms->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BWRData* hit = (R3BWRData*)fWRItemsAms->At(ihit);
                if (!hit)
                    continue;
                fh1_wrs[5]->Fill(int64_t(wrs[0] - hit->GetTimeStamp()));
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

    if (ok)
    {
        fNEvents += 1;

        if (fMwpc0MappedDataCA && fMwpc0MappedDataCA->GetEntries() > 0)
        {
            Int_t nHits = fMwpc0MappedDataCA->GetEntries();
            R3BMwpcMappedData** mappedData = new R3BMwpcMappedData*[nHits];
            Int_t planeId;
            Int_t nbpadx = 0;
            Int_t nbpady = 0;
            for (Int_t i = 0; i < nHits; i++)
            {
                mappedData[i] = (R3BMwpcMappedData*)(fMwpc0MappedDataCA->At(i));
                planeId = mappedData[i]->GetPlane();
                if (planeId == 1)
                    nbpadx++;
                else if (planeId == 3)
                    nbpady++;
            }
            if (mappedData)
                delete mappedData;

            if (nbpadx > 0 && nbpady > 0)
                fCounterDet[0]++;
        }

        if (fTrimMappedData && fTrimMappedData->GetEntries() > 0)
        {
            Int_t nHits = fTrimMappedData->GetEntries();
            R3BSofTrimMappedData** mappedData = new R3BSofTrimMappedData*[nHits];
            Int_t sumanodes = 0;
            for (Int_t i = 0; i < nHits; i++)
            {
                mappedData[i] = (R3BSofTrimMappedData*)(fTrimMappedData->At(i));
                if (mappedData[i]->GetAnodeID() < 7)
                    sumanodes++;
            }
            if (sumanodes > 12)
            {
                fCounterDet[1]++;
            }

            if (mappedData)
                delete mappedData;
        }

        if (fMappedItemsCalifa && fMappedItemsCalifa->GetEntries() > 0)
        {
            Int_t nHits = fMappedItemsCalifa->GetEntries();
            Int_t nCry = 0;
            R3BCalifaMappedData** mappedData = new R3BCalifaMappedData*[nHits];
            for (Int_t i = 0; i < nHits; i++)
            {
                mappedData[i] = (R3BCalifaMappedData*)(fMappedItemsCalifa->At(i));
                if (mappedData[i]->GetCrystalId() > 2432 && mappedData[i]->GetEnergy() > 2.)
                    nCry++;
            }
            if (mappedData)
                delete mappedData;
            if (nCry > 0)
                fCounterDet[2]++;
        }

        if (fMappedItemsAms && fMappedItemsAms->GetEntries() > 0)
        {
            Int_t nHits = fMappedItemsAms->GetEntries();
            Int_t ndetr = 0, ndetl = 0;
            R3BAmsMappedData** mappedData = new R3BAmsMappedData*[nHits];
            for (Int_t i = 0; i < nHits; i++)
            {
                mappedData[i] = (R3BAmsMappedData*)(fMappedItemsAms->At(i));
                if (mappedData[i]->GetDetectorId() < 3)
                    ndetr++;
                else
                    ndetl++;
            }
            if (mappedData)
                delete mappedData;
            if (ndetr > 1 && ndetl > 1)
                fCounterDet[3]++;
        }

        if (fMwpc1MappedDataCA && fMwpc1MappedDataCA->GetEntries() > 0)
        {
            Int_t nHits = fMwpc1MappedDataCA->GetEntries();
            R3BMwpcMappedData** mappedData = new R3BMwpcMappedData*[nHits];
            Int_t planeId;
            Int_t nbpadx = 0;
            Int_t nbpady = 0;
            for (Int_t i = 0; i < nHits; i++)
            {
                mappedData[i] = (R3BMwpcMappedData*)(fMwpc1MappedDataCA->At(i));
                planeId = mappedData[i]->GetPlane();
                if (planeId == 1 || planeId == 2)
                    nbpadx++;
                else if (planeId == 3)
                    nbpady++;
            }
            if (mappedData)
                delete mappedData;

            if (nbpadx > 0 && nbpady > 0)
                fCounterDet[4]++;
        }

        if (fTwimMappedDataCA && fTwimMappedDataCA->GetEntries() > 0)
        {
            Int_t nHits = fTwimMappedDataCA->GetEntries();
            R3BTwimMappedData** mappedData = new R3BTwimMappedData*[nHits];
            Int_t secId = 0;
            Int_t anodeId = 0;
            Int_t sumanodes = 0;
            for (Int_t i = 0; i < nHits; i++)
            {
                mappedData[i] = (R3BTwimMappedData*)(fTwimMappedDataCA->At(i));
                secId = mappedData[i]->GetSecID();
                anodeId = mappedData[i]->GetAnodeID();
                if (anodeId < 16)
                    sumanodes++;
            }
            if (mappedData)
                delete mappedData;
            if (sumanodes > 12)
                fCounterDet[5]++;
        }

        if (fMwpc2MappedDataCA && fMwpc2MappedDataCA->GetEntries() > 0)
        {
            Int_t nHits = fMwpc2MappedDataCA->GetEntries();
            R3BMwpcMappedData** mappedData = new R3BMwpcMappedData*[nHits];
            Int_t planeId;
            Int_t nbpadx = 0;
            Int_t nbpady = 0;
            for (Int_t i = 0; i < nHits; i++)
            {
                mappedData[i] = (R3BMwpcMappedData*)(fMwpc2MappedDataCA->At(i));
                planeId = mappedData[i]->GetPlane();
                if (planeId == 1 || planeId == 2)
                    nbpadx++;
                else if (planeId == 3)
                    nbpady++;
            }
            if (mappedData)
                delete mappedData;

            if (nbpadx > 0 && nbpady > 0)
                fCounterDet[6]++;
        }

        if (fMwpc3MappedDataCA && fMwpc3MappedDataCA->GetEntries() > 0)
        {
            Int_t nHits = fMwpc3MappedDataCA->GetEntries();
            R3BMwpcMappedData** mappedData = new R3BMwpcMappedData*[nHits];
            Int_t planeId;
            Int_t nbpadx = 0;
            Int_t nbpady = 0;
            for (Int_t i = 0; i < nHits; i++)
            {
                mappedData[i] = (R3BMwpcMappedData*)(fMwpc3MappedDataCA->At(i));
                planeId = mappedData[i]->GetPlane();
                if (planeId == 1)
                    nbpadx++;
                else if (planeId == 3)
                    nbpady++;
            }
            if (mappedData)
                delete mappedData;

            if (nbpadx > 0 && nbpady > 0)
                fCounterDet[7]++;
        }

        if (fTofWMappedDataCA && fTofWMappedDataCA->GetEntries() > 0)
        {

            Int_t nHits = fTofWMappedDataCA->GetEntries();
            R3BSofTofWMappedData** mappedData = new R3BSofTofWMappedData*[nHits];
            UShort_t iDet = -1;
            for (Int_t i = 0; i < nHits; i++)
            {
                mappedData[i] = (R3BSofTofWMappedData*)fTofWMappedDataCA->At(i);
                iDet = mappedData[i]->GetDetector();
            }
            if (mappedData)
                delete mappedData;
            if (iDet > -1)
                fCounterDet[8]++;
        }
    }

    // Update histograms
    if (fNEvents > 9999)
    {

        for (int i = 0; i < 10; i++)
        {
            gh->SetPoint(i, i + 1., fCounterDet[i] / fNEvents);
            gh->Draw("ap");
            // std::cout<<fCounterDet[i] <<" "<< fNEvents<<std::endl;
            cDetGeneralView->Modified();
            cDetGeneralView->cd();
        }

        cTotNbTrig->Clear();
        char Name1[255];
        fh1_display->Draw();
        sprintf(Name1, "Start: %d", (int)fCounterTpats[0]);
        tex1->DrawLatex(1, 0.8, Name1);
        tex1->Draw();

        sprintf(Name1, "Start+Fission: %d", (int)fCounterTpats[1]);
        tex2->DrawLatex(1, 0.6, Name1);
        tex2->Draw();

        sprintf(Name1, "Start+p2p: %d", (int)fCounterTpats[2]);
        tex3->DrawLatex(1, 0.4, Name1);
        tex3->Draw();

        sprintf(Name1, "Start+Fission+p2p: %d", (int)fCounterTpats[3]);
        tex4->DrawLatex(1, 0.2, Name1);
        tex4->Draw();
        cTotNbTrig->Modified();
        cTotNbTrig->cd();

        for (int j = 0; j < 10; j++)
        {
            fCounterDet[j] = 0.;
        }
        fNEvents = 0;
    }
}

void R3BSofStatusOnlineSpectra::FinishEvent()
{

    LOG(DEBUG) << "Clearing Structure";

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

void R3BSofStatusOnlineSpectra::FinishTask()
{
    // Write trigger canvas in the root file
    cTrigger->Write();
    cDetGeneralView->Write();
    cTotNbTrig->Write();
    if (fWRItemsMaster && fWRItemsSofia)
    {
        cWr->Write();
        cWrs->Write();
    }
}

ClassImp(R3BSofStatusOnlineSpectra)
