// ------------------------------------------------------------
// -----            R3BSofScalersOnlineSpectra            -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofScalersOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BSofScalersMappedData.h"
#include "R3BTrloiiData.h"
#include "THttpServer.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "TCanvas.h"
#include "TFolder.h"
#include "TH1.h"
#include "TH1D.h"
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

R3BSofScalersOnlineSpectra::R3BSofScalersOnlineSpectra()
    : FairTask("SofScalersOnlineSpectra", 1)
    , fMappedItemsScalers(NULL)
    , fMappedItemsTrloii(NULL)
    , fNEvents(0)
    , fNSpill(0)
    , read_trloii(false)
{
}

R3BSofScalersOnlineSpectra::R3BSofScalersOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fMappedItemsScalers(NULL)
    , fMappedItemsTrloii(NULL)
    , fNEvents(0)
    , fNSpill(0)
    , read_trloii(false)
{
}

R3BSofScalersOnlineSpectra::~R3BSofScalersOnlineSpectra()
{
    LOG(INFO) << "R3BSofScalersOnlineSpectra::Delete instance";
    if (fMappedItemsScalers)
        delete fMappedItemsScalers;
    if (read_trloii && fMappedItemsTrloii)
        delete fMappedItemsTrloii;
}

InitStatus R3BSofScalersOnlineSpectra::Init()
{
    LOG(INFO) << "R3BSofScalersOnlineSpectra::Init ";

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofScalersOnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // --- ---------------------------------------- --- //
    // --- get access to mapped data of the scalers --- //
    // --- ---------------------------------------- --- //
    fMappedItemsScalers = (TClonesArray*)mgr->GetObject("SofScalersMappedData");
    if (!fMappedItemsScalers)
    {
        return kFATAL;
    }
    if (read_trloii)
    {
        fMappedItemsTrloii = (TClonesArray*)mgr->GetObject("TrloiiData");
        if (!fMappedItemsTrloii)
        {
            return kFATAL;
        }
    }

    // --- ------------------------------- --- //
    // --- Create histograms for detectors --- //
    // --- ------------------------------- --- //
    char Name1[255];
    char Name2[255];
    static int l_NbChannelsPerScaler[NbScalers] = NbChannelsPerScaler;
    TString NameTrloii[3] = { "MAIN", "S2", "S8" };
    TString NameScaler[4] = { "RAW", "BDT", "ADT", "ARD" };

    for (Int_t i = 0; i < NbScalers; i++)
    {
        // === Histograms for the Mapped data === //
        sprintf(Name1, "SofScalers%i", i + 1);
        cScalersGeneralView[i] = new TCanvas(Name1, Name1, 10, 10, 800, 700);

        if (i < 2)
        {
            sprintf(Name1, "SofScalers%i_GeneralView", i + 1);
        }
        else
        {
            sprintf(Name1, "Trloii_Scaler_%s_%s", NameTrloii[(i - 2) / 4].Data(), NameScaler[(i - 2) % 4].Data());
        }
        fh1_GeneralView[i] = new TH1D(Name1, Name1, l_NbChannelsPerScaler[i] + 2, 0.5, l_NbChannelsPerScaler[i] + 2.5);
        fh1_GeneralView[i]->GetXaxis()->SetTitle("Channel number (starting from 1)");
        fh1_GeneralView[i]->GetYaxis()->SetTitle("Counts per bin");
        fh1_GeneralView[i]->GetXaxis()->CenterTitle(true);
        fh1_GeneralView[i]->GetYaxis()->CenterTitle(true);
        fh1_GeneralView[i]->GetXaxis()->SetLabelSize(0.025);
        fh1_GeneralView[i]->GetXaxis()->SetTitleSize(0.025);
        fh1_GeneralView[i]->GetYaxis()->SetLabelSize(0.025);
        fh1_GeneralView[i]->GetYaxis()->SetTitleSize(0.025);

        cScalersGeneralView[i]->cd();
        gPad->SetGridx(1);
        if (i > 1)
            gPad->SetLogy(1);
        gPad->SetBottomMargin(10);
        fh1_GeneralView[i]->Draw("");
        fh1_GeneralView[i]->ls();
    }

    fh1_GeneralView[0]->GetXaxis()->SetBinLabel(1, "SciL");
    fh1_GeneralView[0]->GetXaxis()->SetBinLabel(2, "SciR");
    fh1_GeneralView[0]->GetXaxis()->SetBinLabel(3, "MS_r4l71");
    fh1_GeneralView[0]->GetXaxis()->SetBinLabel(4, "OR_MWup");
    fh1_GeneralView[0]->GetXaxis()->SetBinLabel(5, "OR_R3Bmus");
    fh1_GeneralView[0]->GetXaxis()->SetBinLabel(6, "OR_Twim1");
    fh1_GeneralView[0]->GetXaxis()->SetBinLabel(7, "OR_Twim2");
    fh1_GeneralView[0]->GetXaxis()->SetBinLabel(8, "unused");
    fh1_GeneralView[0]->GetXaxis()->SetLabelOffset(0);
    fh1_GeneralView[0]->GetXaxis()->SetTitleOffset(2.2);

    for (Int_t plastic = 0; plastic < 28; plastic++)
    {
        sprintf(Name1, "P%02dU", plastic + 1);
        sprintf(Name2, "P%02dD", plastic + 1);
        fh1_GeneralView[1]->GetXaxis()->SetBinLabel(plastic * 2 + 1, Name1);
        fh1_GeneralView[1]->GetXaxis()->SetBinLabel(plastic * 2 + 2, Name2);
    }
    fh1_GeneralView[1]->GetXaxis()->SetBinLabel(57, "unused");
    fh1_GeneralView[1]->GetXaxis()->SetTitleOffset(1.4);
    fh1_GeneralView[1]->SetCanExtend(TH1::kAllAxes);
    fh1_GeneralView[1]->LabelsDeflate("X");
    fh1_GeneralView[1]->LabelsOption("v");

    cRate = new TCanvas("cRate", "cRate", 10, 10, 1200, 1200);
    cRate->SetLogy(1);
    cRate->cd();
    // h_RatePerSpill = new TGraph(8);
    h_RatePerSpill = new TH1D("h_RatePerSpill", "Averaged rates per spill for each detectors", 10, 0.5, 10.5);
    h_RatePerSpill->GetXaxis()->SetBinLabel(1, "S2 Left");
    h_RatePerSpill->GetXaxis()->SetBinLabel(2, "S2 Right");
    h_RatePerSpill->GetXaxis()->SetBinLabel(3, "S8 Left");
    h_RatePerSpill->GetXaxis()->SetBinLabel(4, "S8 Right");
    h_RatePerSpill->GetXaxis()->SetBinLabel(5, "CaveC Left");
    h_RatePerSpill->GetXaxis()->SetBinLabel(6, "CaveC Right");
    h_RatePerSpill->GetXaxis()->SetBinLabel(7, "Min Bias BDT");
    h_RatePerSpill->GetXaxis()->SetBinLabel(8, "CALIFA BDT");
    h_RatePerSpill->GetXaxis()->SetBinLabel(9, "NEULAND BDT");
    h_RatePerSpill->GetXaxis()->SetBinLabel(10, "Trigger ADT");
    h_RatePerSpill->Draw("HIST TEXT0");
    // h_RatePerSpill->Draw("same text");

    // --- ------------------- --- //
    // --- MAIN FOLDER-Scalers --- //
    // --- ------------------- --- //
    TFolder* mainfolScalers = new TFolder("SOFSCALERS", "SOFSCALERS info");
    for (Int_t i = 0; i < NbScalers; i++)
    {
        mainfolScalers->Add(cScalersGeneralView[i]);
    }
    run->AddObject(mainfolScalers);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_SOFSCALERS_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofScalersOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofScalersOnlineSpectra::Reset_Histo";
    for (Int_t i = 0; i < NbScalers; i++)
    {
        // === accumulated statistics per channel === //
        fh1_GeneralView[i]->Reset();
    }
}

void R3BSofScalersOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofScalersOnlineSpectra::Exec FairRootManager not found";

    if (fMappedItemsScalers && fMappedItemsScalers->GetEntriesFast())
    {
        // --- --------------------- --- //
        // --- loop over mapped data --- //
        // --- --------------------- --- //
        for (Int_t ihit = 0; ihit < fMappedItemsScalers->GetEntriesFast(); ihit++)
        {
            R3BSofScalersMappedData* hitmapped = (R3BSofScalersMappedData*)fMappedItemsScalers->At(ihit);
            if (!hitmapped)
                continue;
            fh1_GeneralView[hitmapped->GetScaler() - 1]->Fill(hitmapped->GetChannel(), hitmapped->GetValue());
            //
            if (hitmapped->GetScaler() != 1)
                continue;
            if (hitmapped->GetValue() == 0)
                continue;
            ULong64_t offset = 0; // fScaler[hitmapped->GetChannel() - 1];
            fScaler[hitmapped->GetChannel() - 1] = hitmapped->GetValue();
            // if (offset == 0); (hitmapped->GetValue() < offset || offset == 0) continue;
            if (hitmapped->GetChannel() == 0 + 1)
                fcounts[4] += hitmapped->GetValue() - offset;
            else if (hitmapped->GetChannel() == 1 + 1)
                fcounts[5] += hitmapped->GetValue() - offset;
            else
                continue;
            LOG(DEBUG) << hitmapped->GetChannel() << " " << hitmapped->GetValue() << " " << offset << " " << fcounts[4];
        }
    }
    if (read_trloii && fMappedItemsTrloii && fMappedItemsTrloii->GetEntriesFast())
    {
        // --- --------------------- --- //
        // --- loop over mapped data --- //
        // --- --------------------- --- //
        for (Int_t ihit = 0; ihit < fMappedItemsTrloii->GetEntriesFast(); ihit++)
        {
            R3BTrloiiData* hitmapped = (R3BTrloiiData*)fMappedItemsTrloii->At(ihit);
            if (!hitmapped)
                continue;
            if (hitmapped->GetCounts() == 0)
                continue;
            // if (fTrloii[hitmapped->GetType() - 1][hitmapped->GetCh() - 1] == 0)
            // fTrloii[hitmapped->GetType() - 1][hitmapped->GetCh() - 1] = hitmapped->GetCounts();
            ULong64_t offset = fTrloii[hitmapped->GetType() - 1][hitmapped->GetCh() - 1];
            fh1_GeneralView[hitmapped->GetType() - 1 + 2]->Fill(hitmapped->GetCh(), hitmapped->GetCounts() - offset);
            fTrloii[hitmapped->GetType() - 1][hitmapped->GetCh() - 1] = hitmapped->GetCounts();
            //
            if (hitmapped->GetCounts() - offset < 10 || offset == 0)
                continue;
            else if (hitmapped->GetType() == 4 + 1 && hitmapped->GetCh() == 9 + 1)
                fcounts[0] += hitmapped->GetCounts() - offset;
            else if (hitmapped->GetType() == 4 + 1 && hitmapped->GetCh() == 10 + 1)
                fcounts[1] += hitmapped->GetCounts() - offset;
            else if (hitmapped->GetType() == 8 + 1 && hitmapped->GetCh() == 0 + 1)
                fcounts[2] += hitmapped->GetCounts() - offset;
            else if (hitmapped->GetType() == 8 + 1 && hitmapped->GetCh() == 1 + 1)
                fcounts[3] += hitmapped->GetCounts() - offset;
            // else if (hitmapped->GetType() == 0 + 1 && hitmapped->GetCh() == 0 + 1) fcounts[4] +=
            // hitmapped->GetCounts() - offset;
            // else if (hitmapped->GetType() == 0 + 1 && hitmapped->GetCh() == 1 + 1) fcounts[5] +=
            // hitmapped->GetCounts() - offset;
            else if (hitmapped->GetType() == 1 + 1 && hitmapped->GetCh() == 0 + 1)
                fcounts[6] += hitmapped->GetCounts() - offset;
            else if (hitmapped->GetType() == 1 + 1 && hitmapped->GetCh() == 1 + 1)
                fcounts[7] += hitmapped->GetCounts() - offset;
            else if (hitmapped->GetType() == 1 + 1 && hitmapped->GetCh() == 2 + 1)
                fcounts[8] += hitmapped->GetCounts() - offset;
            else if (hitmapped->GetType() == 2 + 1 && hitmapped->GetCh() == 0 + 1)
            {
                fcounts[9] += hitmapped->GetCounts() - offset;
                fNSpill++;
            }
            else
                continue;
            // LOG(INFO) << fNSpill << " " << hitmapped->GetType() << " " << hitmapped->GetCh() << " " <<
            // hitmapped->GetCounts() - offset << " " << fcounts[7];
            LOG(DEBUG) << hitmapped->GetType() << " " << hitmapped->GetCh() << " " << hitmapped->GetCounts() << " "
                       << offset;
        }
    }
    fNEvents += 1;
}

void R3BSofScalersOnlineSpectra::FinishEvent()
{
    if (fMappedItemsScalers)
    {
        fMappedItemsScalers->Clear();
    }
    if (read_trloii && fMappedItemsTrloii)
    {
        fMappedItemsTrloii->Clear();
    }
}

void R3BSofScalersOnlineSpectra::FinishTask()
{
    for (UShort_t i = 0; i < 10; i++)
    {
        frate[i] = (double)fcounts[i] / (double)fNSpill;
        // h_RatePerSpill->SetPoint(i,(double)i,frate[i]);
        h_RatePerSpill->Fill((double)i + 1, frate[i]);
        std::cout << frate[i] << " ";
    }
    h_RatePerSpill->Write();
    cRate->Write();
    std::cout << std::endl;
    if (fMappedItemsScalers && (!read_trloii || fMappedItemsTrloii))
    {
        for (UShort_t i = 0; i < NbScalers; i++)
        {
            fh1_GeneralView[i]->Write();
            cScalersGeneralView[i]->Write();
            fh1_GeneralView[i]->ls();
            for (UShort_t j = 0; j < fh1_GeneralView[i]->GetNbinsX(); j++)
            {
                if (fh1_GeneralView[i]->GetBinContent(j) == 0)
                    continue;
                // std::cout << fh1_GeneralView[i]->GetXaxis()->GetBinCenter(j) << " " << (uint32_t)
                // fh1_GeneralView[i]->GetBinContent(j) << std::endl;
            }
        }
    }
}

ClassImp(R3BSofScalersOnlineSpectra)
