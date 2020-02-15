// ------------------------------------------------------------
// -----                  R3BSofScalersOnlineSpectra           -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofScalersOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BSofScalersMappedData.h"
#include "THttpServer.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "TCanvas.h"
#include "TFolder.h"
#include "TH1.h"
#include "TH1I.h"
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
    , fNEvents(0)
{
}

R3BSofScalersOnlineSpectra::R3BSofScalersOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fMappedItemsScalers(NULL)
    , fNEvents(0)
{
}

R3BSofScalersOnlineSpectra::~R3BSofScalersOnlineSpectra()
{
    LOG(INFO) << "R3BSofScalersOnlineSpectra::Delete instance";
    if (fMappedItemsScalers)
        delete fMappedItemsScalers;
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

    // --- ------------------------------- --- //
    // --- Create histograms for detectors --- //
    // --- ------------------------------- --- //
    char Name1[255];
    char Name2[255];
    static int l_NbChannelsPerScaler[NbScalers] = NbChannelsPerScaler;

    for (Int_t i = 0; i < NbScalers; i++)
    {
        // === Histograms for the Mapped data === //
        sprintf(Name1, "SofScalers%i", i + 1);
        cScalersGeneralView[i] = new TCanvas(Name1, Name1, 10, 10, 800, 700);

        sprintf(Name1, "SofScalers%i_GeneralView", i + 1);
        fh1_GeneralView[i] = new TH1I(Name1, Name1, l_NbChannelsPerScaler[i] + 2, 0.5, l_NbChannelsPerScaler[i] + 2.5);
        fh1_GeneralView[i]->GetXaxis()->SetTitle("Channel number (starting from 1)");
        fh1_GeneralView[i]->GetYaxis()->SetTitle("Counts per bin");
        fh1_GeneralView[i]->GetXaxis()->CenterTitle(true);
        fh1_GeneralView[i]->GetYaxis()->CenterTitle(true);
        fh1_GeneralView[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_GeneralView[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_GeneralView[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_GeneralView[i]->GetYaxis()->SetTitleSize(0.045);

        cScalersGeneralView[i]->cd();
        gPad->SetGridx(1);
        // gPad->SetLogy(1);
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
}

void R3BSofScalersOnlineSpectra::FinishTask()
{
    if (fMappedItemsScalers)
    {
        for (UShort_t i = 0; i < NbScalers; i++)
        {
            fh1_GeneralView[i]->Write();
            cScalersGeneralView[i]->Write();
        }
    }
}

ClassImp(R3BSofScalersOnlineSpectra)
