// ------------------------------------------------------------
// -----                R3BSofCorrOnlineSpectra           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofCorrOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BSofCorrmMappedData.h"
#include "R3BSofCorrvMappedData.h"
#include "R3BSofSciTcalData.h"
#include "THttpServer.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "TCanvas.h"
#include "TFolder.h"
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

R3BSofCorrOnlineSpectra::R3BSofCorrOnlineSpectra()
    : FairTask("SofCorrOnlineSpectra", 1)
    , fCorrmMapped(NULL)
    , fCorrvMapped(NULL)
    , fSciTcal(NULL)
    , fFirstX_Corrv(10)
    , fLastX_Corrv(900)
    , fTrefId_Corrv(2)
    , fNEvents(0)
{
    fNsPerBin_Corrv = (fLastX_Corrv - fFirstX_Corrv) / 5.;
}

R3BSofCorrOnlineSpectra::R3BSofCorrOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fCorrmMapped(NULL)
    , fCorrvMapped(NULL)
    , fSciTcal(NULL)
    , fFirstX_Corrv(10)
    , fLastX_Corrv(900)
    , fTrefId_Corrv(6)
    , fNEvents(0)
{
    fNsPerBin_Corrv = (fLastX_Corrv - fFirstX_Corrv) / 5.;
}

R3BSofCorrOnlineSpectra::~R3BSofCorrOnlineSpectra()
{
    LOG(INFO) << "R3BSofCorrOnlineSpectra::Delete instance";
    if (fCorrmMapped)
        delete fCorrmMapped;
    if (fCorrvMapped)
        delete fCorrvMapped;
    if (fSciTcal)
        delete fSciTcal;
}

InitStatus R3BSofCorrOnlineSpectra::Init()
{
    LOG(INFO) << "R3BSofCorrOnlineSpectra::Init ";

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofCorrOnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // --- --------------- --- //
    // --- CORRM at MAPPED --- //
    // --- --------------- --- //
    fCorrmMapped = (TClonesArray*)mgr->GetObject("CorrmMappedData");
    if (!fCorrmMapped)
    {
        LOG(ERROR) << "R3BSofCorrOnlineSpectra:: CorrmMappedData not found";
        return kFATAL;
    }

    // --- --------------- --- //
    // --- CORRV at MAPPED --- //
    // --- --------------- --- //
    fCorrvMapped = (TClonesArray*)mgr->GetObject("CorrvMappedData");
    if (!fCorrvMapped)
    {
        LOG(ERROR) << "R3BSofCorrOnlineSpectra:: CorrvMappedData not found";
        return kFATAL;
    }

    // --- ----------- --- //
    // --- SCI at TCAL --- //
    // --- ----------- --- //
    fSciTcal = (TClonesArray*)mgr->GetObject("SofSciTcalData");
    if (!fSciTcal)
    {
        LOG(ERROR) << "R3BSofCorrOnlineSpectra:: SofSciTcalData not found";
        return kFATAL;
    }

    // Create histograms
    char Name1[255];
    char Name2[255];

    // SOFIA_VFTX DAQ SUBSYSTEM
    if (fCorrvMapped && fSciTcal && fCorrmMapped)
    {
        cMap_CorrV = new TCanvas("sofia_vftx", "sofia_vftx", 10, 10, 800, 700);
        cMap_CorrV->Divide(2, 3);

        fh1_FineTime_CorrV = new TH1D("FineTime_CorrV", "FineTime_CorrV", 1000, 1, 1001);
        fh1_FineTime_CorrV->GetXaxis()->SetTitle("FineTime");
        fh1_FineTime_CorrV->GetYaxis()->SetTitle("counts");
        fh1_FineTime_CorrV->GetYaxis()->SetTitleOffset(1.1);
        fh1_FineTime_CorrV->GetXaxis()->CenterTitle(true);
        fh1_FineTime_CorrV->GetYaxis()->CenterTitle(true);
        fh1_FineTime_CorrV->GetXaxis()->SetLabelSize(0.045);
        fh1_FineTime_CorrV->GetXaxis()->SetTitleSize(0.045);
        fh1_FineTime_CorrV->GetYaxis()->SetLabelSize(0.045);
        fh1_FineTime_CorrV->GetYaxis()->SetTitleSize(0.045);
        fh1_FineTime_CorrV->SetLineColor(kBlue);
        cMap_CorrV->cd(1);
        fh1_FineTime_CorrV->Draw("");

        fh1_CoarseTime_CorrV = new TH1D("CoarseTime_CorrV", "CoarseTime_CorrV", 10000, -500, 9500);
        fh1_CoarseTime_CorrV->GetXaxis()->SetTitle("CoarseTime");
        fh1_CoarseTime_CorrV->GetYaxis()->SetTitle("counts");
        fh1_CoarseTime_CorrV->GetYaxis()->SetTitleOffset(1.1);
        fh1_CoarseTime_CorrV->GetXaxis()->CenterTitle(true);
        fh1_CoarseTime_CorrV->GetYaxis()->CenterTitle(true);
        fh1_CoarseTime_CorrV->GetXaxis()->SetLabelSize(0.045);
        fh1_CoarseTime_CorrV->GetXaxis()->SetTitleSize(0.045);
        fh1_CoarseTime_CorrV->GetYaxis()->SetLabelSize(0.045);
        fh1_CoarseTime_CorrV->GetYaxis()->SetTitleSize(0.045);
        fh1_CoarseTime_CorrV->SetLineColor(kBlue);
        cMap_CorrV->cd(2);
        fh1_CoarseTime_CorrV->Draw("");

        sprintf(Name1, "RawTime_CorrV");
        fh1_RawTime_CorrV = new TH1D(Name1, Name1, 50500, -500, 50000);
        fh1_RawTime_CorrV->GetXaxis()->SetTitle("RawTime");
        fh1_RawTime_CorrV->GetYaxis()->SetTitle("counts");
        fh1_RawTime_CorrV->GetYaxis()->SetTitleOffset(1.1);
        fh1_RawTime_CorrV->GetXaxis()->CenterTitle(true);
        fh1_RawTime_CorrV->GetYaxis()->CenterTitle(true);
        fh1_RawTime_CorrV->GetXaxis()->SetLabelSize(0.045);
        fh1_RawTime_CorrV->GetXaxis()->SetTitleSize(0.045);
        fh1_RawTime_CorrV->GetYaxis()->SetLabelSize(0.045);
        fh1_RawTime_CorrV->GetYaxis()->SetTitleSize(0.045);
        fh1_RawTime_CorrV->SetLineColor(kBlue);
        cMap_CorrV->cd(3);
        fh1_RawTime_CorrV->Draw("");

        sprintf(Name1, "RawTime_SofSci%i_Tref", fTrefId_Corrv);
        fh1_RawTime_TrefCorrV = new TH1D(Name1, Name1, 50500, -500, 50000);
        fh1_RawTime_TrefCorrV->GetXaxis()->SetTitle("RawTime");
        fh1_RawTime_TrefCorrV->GetYaxis()->SetTitle("counts");
        fh1_RawTime_TrefCorrV->GetYaxis()->SetTitleOffset(1.1);
        fh1_RawTime_TrefCorrV->GetXaxis()->CenterTitle(true);
        fh1_RawTime_TrefCorrV->GetYaxis()->CenterTitle(true);
        fh1_RawTime_TrefCorrV->GetXaxis()->SetLabelSize(0.045);
        fh1_RawTime_TrefCorrV->GetXaxis()->SetTitleSize(0.045);
        fh1_RawTime_TrefCorrV->GetYaxis()->SetLabelSize(0.045);
        fh1_RawTime_TrefCorrV->GetYaxis()->SetTitleSize(0.045);
        fh1_RawTime_TrefCorrV->SetLineColor(kBlue);
        cMap_CorrV->cd(4);
        fh1_RawTime_TrefCorrV->Draw("");

        fh1_DeltaT_sofia_vftx = new TH1D("DeltaT_sofia_vftx", "DeltaT_sofia_vftx", 10000, 0, 1000);
        fh1_DeltaT_sofia_vftx->GetXaxis()->SetTitle("CoarseTime");
        fh1_DeltaT_sofia_vftx->GetYaxis()->SetTitle("counts");
        fh1_DeltaT_sofia_vftx->GetYaxis()->SetTitleOffset(1.1);
        fh1_DeltaT_sofia_vftx->GetXaxis()->CenterTitle(true);
        fh1_DeltaT_sofia_vftx->GetYaxis()->CenterTitle(true);
        fh1_DeltaT_sofia_vftx->GetXaxis()->SetLabelSize(0.045);
        fh1_DeltaT_sofia_vftx->GetXaxis()->SetTitleSize(0.045);
        fh1_DeltaT_sofia_vftx->GetYaxis()->SetLabelSize(0.045);
        fh1_DeltaT_sofia_vftx->GetYaxis()->SetTitleSize(0.045);
        fh1_DeltaT_sofia_vftx->SetLineColor(kBlue);
        cMap_CorrV->cd(5);
        fh1_DeltaT_sofia_vftx->Draw("");
    }

    // sofia_mesy
    if (fCorrmMapped)
    {
        cMap_CorrM = new TCanvas("sofia_mesy", "sofia_mesy", 10, 10, 800, 700);
        cMap_CorrM->Divide(2, 2);
        fh1_CorrM = new TH1D*[3];

        fh1_CorrM[0] = new TH1D("Time_CorrM", "Time_CorrM", 10000, 20000, 30000);
        fh1_CorrM[0]->GetXaxis()->SetTitle("TimeCorrM [100ps]");
        fh1_CorrM[0]->GetYaxis()->SetTitle("counts");
        fh1_CorrM[0]->GetYaxis()->SetTitleOffset(1.1);
        fh1_CorrM[0]->GetXaxis()->CenterTitle(true);
        fh1_CorrM[0]->GetYaxis()->CenterTitle(true);
        fh1_CorrM[0]->GetXaxis()->SetLabelSize(0.045);
        fh1_CorrM[0]->GetXaxis()->SetTitleSize(0.045);
        fh1_CorrM[0]->GetYaxis()->SetLabelSize(0.045);
        fh1_CorrM[0]->GetYaxis()->SetTitleSize(0.045);
        fh1_CorrM[0]->SetLineColor(kBlue);
        cMap_CorrM->cd(1);
        fh1_CorrM[0]->Draw("");

        fh1_CorrM[1] = new TH1D("Time_Trig_sofia_mesy", "Time_Trig_sofia_mesy", 65000, 0, 65000);
        fh1_CorrM[1]->GetXaxis()->SetTitle("Time Trig sofia_mesy  [100ps]");
        fh1_CorrM[1]->GetYaxis()->SetTitle("counts");
        fh1_CorrM[1]->GetYaxis()->SetTitleOffset(1.1);
        fh1_CorrM[1]->GetXaxis()->CenterTitle(true);
        fh1_CorrM[1]->GetYaxis()->CenterTitle(true);
        fh1_CorrM[1]->GetXaxis()->SetLabelSize(0.045);
        fh1_CorrM[1]->GetXaxis()->SetTitleSize(0.045);
        fh1_CorrM[1]->GetYaxis()->SetLabelSize(0.045);
        fh1_CorrM[1]->GetYaxis()->SetTitleSize(0.045);
        fh1_CorrM[1]->SetLineColor(kBlue);
        cMap_CorrM->cd(2);
        fh1_CorrM[1]->Draw("");
    }

    // Correlation between sofia_vftx et sofia_mesy
    if (fCorrvMapped && fSciTcal && fCorrmMapped)
    {
        cMap_Corr = new TCanvas("Corr_sofia_daqs", "Corr_sofia_daqs", 10, 10, 800, 700);
        fh2_Correlation = new TH2F*[3];

        fh2_Correlation[0] = new TH2F("Corrm_vs_Corrv", "Corrm_vs_Corrv", 1000, 250, 1000, 1000, 20000, 30000);
        fh2_Correlation[0]->GetXaxis()->SetTitle("DeltaT CorrV [ns]");
        fh2_Correlation[0]->GetYaxis()->SetTitle("T CorrM [100 ps]");
        cMap_Corr->cd();
        fh2_Correlation[0]->Draw("col");
    }

    // Folder
    TFolder* FoldCorr = new TFolder("Corr", "Correlation");
    if (fCorrvMapped && fSciTcal)
    {
        FoldCorr->Add(cMap_CorrV);
    }
    if (fCorrmMapped)
    {
        FoldCorr->Add(cMap_CorrM);
    }
    if (fCorrvMapped && fSciTcal && fCorrmMapped)
    {
        FoldCorr->Add(cMap_Corr);
    }

    run->AddObject(FoldCorr);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_Corr_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));
    return kSUCCESS;
}

void R3BSofCorrOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofCorrOnlineSpectra::Reset_Histo";

    if (fCorrvMapped && fSciTcal)
    {
        fh1_FineTime_CorrV->Reset();
        fh1_CoarseTime_CorrV->Reset();
        fh1_RawTime_CorrV->Reset();
        fh1_RawTime_TrefCorrV->Reset();
        fh1_DeltaT_sofia_vftx->Reset();
    }
    if (fCorrmMapped)
    {
        fh1_CorrM[0]->Reset();
        fh1_CorrM[1]->Reset();
    }
    if (fCorrvMapped && fSciTcal && fCorrmMapped)
    {
        fh2_Correlation[0]->Reset();
    }
}

void R3BSofCorrOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofCorrOnlineSpectra::Exec FairRootManager not found";

    Double_t CoarseCorrV = -1000000000.;
    Double_t FineCorrV = -1000000000.;
    Double_t TimeCorrV_Ns = -1000000000.;
    Double_t DeltaT_CorrV = -1000000000.;
    if (fCorrvMapped && fCorrvMapped->GetEntriesFast() > 0)
    {
        for (Int_t entry1 = 0; entry1 < fCorrvMapped->GetEntriesFast(); entry1++)
        {
            R3BSofCorrvMappedData* corrvmap = (R3BSofCorrvMappedData*)fCorrvMapped->At(entry1);
            if (!corrvmap)
                continue;
            FineCorrV = (Double_t)corrvmap->GetFineTimeCorr();
            CoarseCorrV = (Double_t)corrvmap->GetCoarseTimeCorr();
            TimeCorrV_Ns = 5. * CoarseCorrV - fNsPerBin_Corrv * (FineCorrV - (Double_t)fFirstX_Corrv);

            fh1_FineTime_CorrV->Fill(FineCorrV);
            fh1_CoarseTime_CorrV->Fill(CoarseCorrV);
            fh1_RawTime_CorrV->Fill(TimeCorrV_Ns);

            if (fSciTcal && fSciTcal->GetEntriesFast())
            {
                for (Int_t entry2 = 0; entry2 < fSciTcal->GetEntriesFast(); entry2++)
                {
                    R3BSofSciTcalData* scitcal = (R3BSofSciTcalData*)fSciTcal->At(entry2);
                    if (!scitcal)
                        continue;
                    if (scitcal->GetPmt() == 3 && scitcal->GetDetector() == fTrefId_Corrv)
                    {
                        fh1_RawTime_TrefCorrV->Fill(scitcal->GetRawTimeNs());
                        DeltaT_CorrV = TimeCorrV_Ns - scitcal->GetRawTimeNs();
                        fh1_DeltaT_sofia_vftx->Fill(DeltaT_CorrV);
                    }
                } // end of loop over entry2
            }
        } // end of loop over entry1
    }     // end of if(CorrVMappedData)

    Double_t DeltaT_CorrM;
    if (fCorrmMapped && fCorrmMapped->GetEntriesFast() > 0)
    {
        for (Int_t entry = 0; entry < fCorrmMapped->GetEntriesFast(); entry++)
        {
            R3BSofCorrmMappedData* corrmmap = (R3BSofCorrmMappedData*)fCorrmMapped->At(entry);
            if (!corrmmap)
                continue;
            fh1_CorrM[0]->Fill(corrmmap->GetTimeCorr());
            fh1_CorrM[1]->Fill(corrmmap->GetTimeTrig());
            fh2_Correlation[0]->Fill(DeltaT_CorrV, corrmmap->GetTimeCorr());
        }
    }

    fNEvents += 1;
}

void R3BSofCorrOnlineSpectra::FinishEvent()
{
    if (fCorrvMapped)
        fCorrvMapped->Clear();
    if (fCorrmMapped)
        fCorrmMapped->Clear();
    if (fSciTcal)
        fSciTcal->Clear();
}

void R3BSofCorrOnlineSpectra::FinishTask()
{
    if (fCorrvMapped && fSciTcal && fCorrmMapped)
    {
        cMap_CorrV->Write();
        cMap_CorrM->Write();
        cMap_Corr->Write();
    }
}

ClassImp(R3BSofCorrOnlineSpectra)
