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
#include "R3BVftxTrigMappedData.h"
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

R3BSofCorrOnlineSpectra::R3BSofCorrOnlineSpectra()
    : FairTask("SofCorrOnlineSpectra", 1)
    , fCorrmMapped(NULL)
    , fCorrvMapped(NULL)
    , fVftxTrigMapped(NULL)
    , fFirstX_Corrv(10)
    , fLastX_Corrv(900)
    , fVftxModuleId_Corrv(6)
    , fNEvents(0)
{
    fNsPerBin_Corrv = (fLastX_Corrv - fFirstX_Corrv) / 5.;
}

R3BSofCorrOnlineSpectra::R3BSofCorrOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fCorrmMapped(NULL)
    , fCorrvMapped(NULL)
    , fVftxTrigMapped(NULL)
    , fFirstX_Corrv(10)
    , fLastX_Corrv(900)
    , fVftxModuleId_Corrv(6)
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
    if (fVftxTrigMapped)
        delete fVftxTrigMapped;
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

    // --- ----- --- //
    // --- CORRM --- //
    // --- ----- --- //
    fCorrmMapped = (TClonesArray*)mgr->GetObject("CorrmMappedData");
    if (!fCorrmMapped)
    {
        LOG(ERROR) << "R3BSofCorrOnlineSpectra:: CorrmMappedData not found";
        return kFATAL;
    }

    // --- ----- --- //
    // --- CORRV --- //
    // --- ----- --- //
    fCorrvMapped = (TClonesArray*)mgr->GetObject("CorrvMappedData");
    if (!fCorrvMapped)
    {
        LOG(ERROR) << "R3BSofCorrOnlineSpectra:: CorrvMappedData not found";
        return kFATAL;
    }

    // --- --------- --- //
    // --- VFTX TRIG --- //
    // --- --------- --- //
    fVftxTrigMapped = (TClonesArray*)mgr->GetObject("VftxTrigMappedData");
    if (!fVftxTrigMapped)
    {
        LOG(ERROR) << "R3BSofCorrOnlineSpectra:: VftxTrigMappedData not found";
        return kFATAL;
    }

    // Create histograms
    char Name1[255];
    char Name2[255];

    // Fine Time of the correlation signal at sofia_vftx data
    cMapFT_CorrV = new TCanvas("CorrFineTime", "CorrFineTime", 10, 10, 800, 700);
    cMapFT_CorrV->cd();

    fh1_FineTime_CorrV = new TH1F("FineTime_CorrV", "FineTime_CorrV", 1000, 1, 1001);
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
    cMapFT_CorrV->cd();
    fh1_FineTime_CorrV->Draw("");

    // Delta T = T(Correlation) - T(trig)
    cMap_DeltaT = new TCanvas("DeltaT", "DeltaT", 10, 10, 800, 700);
    cMap_DeltaT->Divide(1,3);
    fh1_DeltaCorrTrig = new TH1F*[3];

    fh1_DeltaCorrTrig[0] = new TH1F("DeltaT_SofCorrv", "DeltaT_SofCorrv", 10000, -5000, 5000);
    fh1_DeltaCorrTrig[0]->GetXaxis()->SetTitle("Tcorr - Ttrig [ns]");
    fh1_DeltaCorrTrig[0]->GetYaxis()->SetTitle("counts");
    fh1_DeltaCorrTrig[0]->GetYaxis()->SetTitleOffset(1.1);
    fh1_DeltaCorrTrig[0]->GetXaxis()->CenterTitle(true);
    fh1_DeltaCorrTrig[0]->GetYaxis()->CenterTitle(true);
    fh1_DeltaCorrTrig[0]->GetXaxis()->SetLabelSize(0.045);
    fh1_DeltaCorrTrig[0]->GetXaxis()->SetTitleSize(0.045);
    fh1_DeltaCorrTrig[0]->GetYaxis()->SetLabelSize(0.045);
    fh1_DeltaCorrTrig[0]->GetYaxis()->SetTitleSize(0.045);
    fh1_DeltaCorrTrig[0]->SetLineColor(kBlue);
    cMap_DeltaT->cd(1);
    fh1_DeltaCorrTrig[0]->Draw("");

    fh1_DeltaCorrTrig[1] = new TH1F("DeltaT_SofCorrm", "DeltaT_SofCorrm", 10000, -5000, 5000);
    fh1_DeltaCorrTrig[1]->GetXaxis()->SetTitle("Tcorr - Ttrig [100 ps]");
    fh1_DeltaCorrTrig[1]->GetYaxis()->SetTitle("counts");
    fh1_DeltaCorrTrig[1]->GetYaxis()->SetTitleOffset(1.1);
    fh1_DeltaCorrTrig[1]->GetXaxis()->CenterTitle(true);
    fh1_DeltaCorrTrig[1]->GetYaxis()->CenterTitle(true);
    fh1_DeltaCorrTrig[1]->GetXaxis()->SetLabelSize(0.045);
    fh1_DeltaCorrTrig[1]->GetXaxis()->SetTitleSize(0.045);
    fh1_DeltaCorrTrig[1]->GetYaxis()->SetLabelSize(0.045);
    fh1_DeltaCorrTrig[1]->GetYaxis()->SetTitleSize(0.045);
    fh1_DeltaCorrTrig[1]->SetLineColor(kBlue);
    cMap_DeltaT->cd(2);
    fh1_DeltaCorrTrig[1]->Draw("");

    //fh1_DeltaCorrTrig[2] = new TH1F("DeltaT_S2Corr", "DeltaT_S2Corr", 10000, -5000, 5000);
    //fh1_DeltaCorrTrig[2]->GetXaxis()->SetTitle("Tcorr - Ttrig [ns]");
    //fh1_DeltaCorrTrig[2]->GetYaxis()->SetTitle("counts");
    //fh1_DeltaCorrTrig[2]->GetYaxis()->SetTitleOffset(1.1);
    //fh1_DeltaCorrTrig[2]->GetXaxis()->CenterTitle(true);
    //fh1_DeltaCorrTrig[2]->GetYaxis()->CenterTitle(true);
    //fh1_DeltaCorrTrig[2]->GetXaxis()->SetLabelSize(0.045);
    //fh1_DeltaCorrTrig[2]->GetXaxis()->SetTitleSize(0.045);
    //fh1_DeltaCorrTrig[2]->GetYaxis()->SetLabelSize(0.045);
    //fh1_DeltaCorrTrig[2]->GetYaxis()->SetTitleSize(0.045);
    //fh1_DeltaCorrTrig[2]->SetLineColor(kBlue);
    //cMap_DeltaT->cd(3);
    //fh1_DeltaCorrTrig[2]->Draw("");

		// Correlation between the Delta T
		cMap_Corr = new TCanvas("CorrDeltaT","CorrDeltaT", 10, 10, 800, 700);
		cMap_Corr->Divide(3,1);	
		fh2_Correlation = new TH2F*[3];

		fh2_Correlation[0] = new TH2F("Corrm_vs_Corrv","Corrm_vs_Corrv",1000,-5000,5000,1000,-5000,5000);
		fh2_Correlation[0]->GetXaxis()->SetTitle("DeltaT CorrV [ns]");
		fh2_Correlation[0]->GetYaxis()->SetTitle("DeltaT CorrM [100 ps]");
		cMap_Corr->cd(1);
		fh2_Correlation[0]->Draw("col");
		
		
    // Folder
    TFolder* FoldCorr = new TFolder("Corr", "Correlation");
    if (fCorrvMapped)
    {
        FoldCorr->Add(cMapFT_CorrV);

        if (fCorrmMapped && fVftxTrigMapped)
        {
            FoldCorr->Add(cMap_DeltaT);
						FoldCorr->Add(cMap_DeltaT);
        }
    }

    run->AddObject(FoldCorr);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_Corr_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));
    return kSUCCESS;
}

void R3BSofCorrOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofCorrOnlineSpectra::Reset_Histo";

    // Mapped Data
    if (fCorrvMapped)
    {
        fh1_FineTime_CorrV->Reset();
        if (fVftxTrigMapped)
        {
            fh1_DeltaCorrTrig[0]->Reset();
						if(fCorrmMapped)
						{
							fh2_Correlation[0]->Reset();
						}
		    }
    }
    if (fCorrmMapped)
    {
        fh1_DeltaCorrTrig[1]->Reset();
    }
}

void R3BSofCorrOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofCorrOnlineSpectra::Exec FairRootManager not found";

    Double_t TimeCorrV_Ns = -1000000000.;
    Double_t DeltaT_CorrV = -1000000000.;
    if (fCorrvMapped && fCorrvMapped->GetEntriesFast() > 0)
    {
        for (Int_t entry1 = 0; entry1 < fCorrvMapped->GetEntriesFast(); entry1++)
        {
            R3BSofCorrvMappedData* corrvmap = (R3BSofCorrvMappedData*)fCorrvMapped->At(entry1);
            if (!corrvmap)
                continue;
            fh1_FineTime_CorrV->Fill(corrvmap->GetFineTimeCorr());
            TimeCorrV_Ns = 5. * (Double_t)corrvmap->GetCoarseTimeCorr() -
                           fNsPerBin_Corrv * (Double_t)(corrvmap->GetFineTimeCorr() - fFirstX_Corrv);

            if (fVftxTrigMapped && fVftxTrigMapped->GetEntriesFast())
            {
                for (Int_t entry2 = 0; entry2 < fVftxTrigMapped->GetEntriesFast(); entry2++)
                {
                    R3BVftxTrigMappedData* vftxtrmap = (R3BVftxTrigMappedData*)fVftxTrigMapped->At(entry2);
                    if (!vftxtrmap)
                        continue;
                    if (vftxtrmap->GetModuleID() == fVftxModuleId_Corrv)
                    {
                        DeltaT_CorrV = TimeCorrV_Ns - 5. * (Double_t)vftxtrmap->GetTimeTrig();
                        fh1_DeltaCorrTrig[0]->Fill(DeltaT_CorrV);
                    }
                } // end of loop over entry2
            }
        } // end of loop over entry1
    }// end of if(CorrVMappedData)

		Double_t DeltaT_CorrM;
		if (fCorrmMapped && fCorrmMapped->GetEntriesFast()>0)
		{
			for(Int_t entry=0; entry<fCorrmMapped->GetEntriesFast(); entry++)
			{
				R3BSofCorrmMappedData* corrmmap = (R3BSofCorrmMappedData*)fCorrmMapped->At(entry);
				if(!corrmmap) continue;
				DeltaT_CorrM = corrmmap->GetTimeTrig() - corrmmap->GetTimeCorr();
				fh1_DeltaCorrTrig[1]->Fill(DeltaT_CorrM);
				fh2_Correlation[0]->Fill(DeltaT_CorrV, DeltaT_CorrM);
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
    if (fVftxTrigMapped)
        fVftxTrigMapped->Clear();
}

void R3BSofCorrOnlineSpectra::FinishTask()
{
    if (fCorrvMapped)
    {
        cMapFT_CorrV->Write();
        if (fVftxTrigMapped && fCorrmMapped)
        {
            cMap_DeltaT->Write();
						cMap_Corr->Write();
				}
    }
}

ClassImp(R3BSofCorrOnlineSpectra)
