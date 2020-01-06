// ------------------------------------------------------------
// -----              R3BSofToFWOnlineSpectra             -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofToFWOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BSofMwpcCalData.h"
#include "R3BSofSciSingleTcalData.h"
#include "R3BSofToFWMappedData.h"
#include "R3BSofToFWTcalData.h"
#include "R3BSofTwimHitData.h"
#include "THttpServer.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "TCanvas.h"
#include "TFolder.h"
#include "TH1.h"
#include "TH2.h"
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

R3BSofToFWOnlineSpectra::R3BSofToFWOnlineSpectra()
    : FairTask("SofToFWOnlineSpectra", 1)
    , fMappedItemsToFW(NULL)
    , fTcalItemsToFW(NULL)
    , fHitItemsTwim(NULL)
    , fCalItemsMwpc(NULL)
    , fNEvents(0)
{
}

R3BSofToFWOnlineSpectra::R3BSofToFWOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fMappedItemsToFW(NULL)
    , fTcalItemsToFW(NULL)
    , fHitItemsTwim(NULL)
    , fCalItemsMwpc(NULL)
    , fNEvents(0)
{
}

R3BSofToFWOnlineSpectra::~R3BSofToFWOnlineSpectra()
{
    LOG(INFO) << "R3BSofToFWOnlineSpectra::Delete instance";
    if (fMappedItemsToFW)
        delete fMappedItemsToFW;
    if (fTcalItemsToFW)
        delete fTcalItemsToFW;
    if (fHitItemsTwim)
        delete fHitItemsTwim;
    if (fCalItemsMwpc)
        delete fCalItemsMwpc;
}

InitStatus R3BSofToFWOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofToFWOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofToFWOnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // --- ------------------------------------- --- //
    // --- get access to mapped data of the TOFW --- //
    // --- ------------------------------------- --- //
    fMappedItemsToFW = (TClonesArray*)mgr->GetObject("SofToFWMappedData");
    if (!fMappedItemsToFW)
    {
        return kFATAL;
    }

    // --- ----------------------------------- --- //
    // --- get access to tcal data of the TOFW --- //
    // --- ----------------------------------- --- //
    fTcalItemsToFW = (TClonesArray*)mgr->GetObject("SofToFWTcalData");
    if (!fTcalItemsToFW)
    {
        return kFATAL;
    }

    // --- ----------------------------------------- --- //
    // --- get access to single tcal data of the SCI --- //
    // --- ----------------------------------------- --- //
    fSingleTcalItemsSci = (TClonesArray*)mgr->GetObject("SofSciSingleTcalData");
    if (!fSingleTcalItemsSci)
    {
        return kFATAL;
    }

    // get access to hit data of the TWIM
    fHitItemsTwim = (TClonesArray*)mgr->GetObject("TwimHitData");
    if (!fHitItemsTwim)
        LOG(WARNING) << "R3BSofToFWOnlineSpectra: TwimHitData not found";

    // get access to cal data of the MWPC3
    fCalItemsMwpc = (TClonesArray*)mgr->GetObject("Mwpc3CalData");
    if (!fCalItemsMwpc)
        LOG(WARNING) << "R3BSofToFWOnlineSpectra: Mwpc3CalData not found";

    // --- ------------------------------- --- //
    // --- Create histograms for detectors --- //
    // --- ------------------------------- --- //
    char Name1[255];
    char Name2[255];

    cToFWMult = new TCanvas("ToFW_mult", "ToFW_mult", 0, 0, 1000, 500);
    cToFWMult->Divide(2, 1);

    for (Int_t j = 0; j < NbChs; j++)
    {

        // === MULT AT MAPPED LEVEL === //
        sprintf(Name1, "SofToFW_Pmt%i_MultPerPlastic", j + 1);
        fh2_mult[j] = new TH2I(Name1, Name1, NbDets + 2, -0.5, NbDets + 1.5, 5, -0.5, 4.5);
        fh2_mult[j]->GetXaxis()->SetTitle("plastic number");
        fh2_mult[j]->GetYaxis()->SetTitle("multiplicity per plastic");
        cToFWMult->cd(j + 1);
        fh2_mult[j]->Draw("COL");

        // === FINE TIME === //
        sprintf(Name1, "SofToFW_FineTime_Pmt%i", j + 1);
        cToFWFineTime[j] = new TCanvas(Name1, Name1, 10, 10, 1000, 900);
        cToFWFineTime[j]->Divide(7, 4);
        for (Int_t i = 0; i < NbDets; i++)
        {
            sprintf(Name1, "SofToFW%i_FineTimePmt%i", i + 1, j + 1);
            fh1_finetime[i * NbChs + j] = new TH1I(Name1, Name1, 1000, 0, 1000);
            cToFWFineTime[j]->cd(i + 1);
            fh1_finetime[i * NbChs + j]->Draw("");
        }
    }

    // === RAW POSITION === //
    sprintf(Name1, "SofToFW_RawPos_AtTcal_Mult1");
    cToFWRawPos = new TCanvas(Name1, Name1, 10, 10, 1000, 900);
    cToFWRawPos->Divide(7, 4);
    for (Int_t i = 0; i < NbDets; i++)
    {
        sprintf(Name1, "SofToFW%i_RawPosAtTcal_Mult1", i + 1);
        fh1_RawPos_AtTcalMult1[i] = new TH1F(Name1, Name1, 40000, -20, 20);
        fh1_RawPos_AtTcalMult1[i]->GetXaxis()->SetTitle("Raw position [ns with one bin/ps]");
        fh1_RawPos_AtTcalMult1[i]->GetYaxis()->SetTitle("Counts per bin");
        fh1_RawPos_AtTcalMult1[i]->GetXaxis()->CenterTitle(true);
        fh1_RawPos_AtTcalMult1[i]->GetYaxis()->CenterTitle(true);
        fh1_RawPos_AtTcalMult1[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_RawPos_AtTcalMult1[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_RawPos_AtTcalMult1[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_RawPos_AtTcalMult1[i]->GetYaxis()->SetTitleSize(0.045);
        cToFWRawPos->cd(i + 1);
        fh1_RawPos_AtTcalMult1[i]->Draw("");
    }

    // === ENE RAW === //
    for(Int_t i=0; i< NbChs; i++)
    {
	sprintf(Name1, "SofToFW_EneRaw_pmt%i", i+1);
    	cToFWEneRaw[i] = new TCanvas(Name1,Name1, 10, 10, 1000, 900);
	cToFWEneRaw[i]->Divide(7,4);
	
	for(Int_t j=0; j< NbDets; j++){
	  sprintf(Name1, "SofTofW%i_EneRaw_pmt%i", j+1, i+1);
          fh1_EneRaw[j*NbChs+i] = new TH1D(Name1, Name1, 5000, 0, 5000);
          fh1_EneRaw[j*NbChs+i]->GetXaxis()->SetTitle("Raw Energy [channel]");
          fh1_EneRaw[j*NbChs+i]->GetYaxis()->SetTitle("Counts per bin");
          fh1_EneRaw[j*NbChs+i]->GetXaxis()->CenterTitle(true);
          fh1_EneRaw[j*NbChs+i]->GetYaxis()->CenterTitle(true);
          fh1_EneRaw[j*NbChs+i]->GetXaxis()->SetLabelSize(0.045);
          fh1_EneRaw[j*NbChs+i]->GetXaxis()->SetTitleSize(0.045);
          fh1_EneRaw[j*NbChs+i]->GetYaxis()->SetLabelSize(0.045);
          fh1_EneRaw[j*NbChs+i]->GetYaxis()->SetTitleSize(0.045);
          cToFWEneRaw[i]->cd(j + 1);
          fh1_EneRaw[j*NbChs+i]->Draw("");
	}

    }

    // === RAW TIME-OF-Flight === //
    for (Int_t i = 0; i < NbDets; i++)
    {
        sprintf(Name1, "SofToFW_P%i_RawTof_AtTcal_Mult1", i + 1);
        cToFWRawTof[i] = new TCanvas(Name1, Name1, 10, 10, 1000, 900);
        sprintf(Name1, "SofToFW%i_RawTofAtTcal_Mult1", i + 1);
        fh1_RawTof_AtTcalMult1[i] = new TH1D(Name1, Name1, 100000, -100, 100);
        fh1_RawTof_AtTcalMult1[i]->GetXaxis()->SetTitle("Raw time-of-flight [ns with one bin/ps]");
        fh1_RawTof_AtTcalMult1[i]->GetYaxis()->SetTitle("Counts per bin");
        fh1_RawTof_AtTcalMult1[i]->GetXaxis()->CenterTitle(true);
        fh1_RawTof_AtTcalMult1[i]->GetYaxis()->CenterTitle(true);
        fh1_RawTof_AtTcalMult1[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_RawTof_AtTcalMult1[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_RawTof_AtTcalMult1[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_RawTof_AtTcalMult1[i]->GetYaxis()->SetTitleSize(0.045);
        cToFWRawTof[i]->cd();
        fh1_RawTof_AtTcalMult1[i]->Draw("");
    }

    // === Twim vs TIME-OF-Flight === //
    for (Int_t i = 0; i < NbDets; i++)
    {
        sprintf(Name1, "Twim_vs_ToF_Plastic_%i", i + 1);
        cTwimvsTof[i] = new TCanvas(Name1, Name1, 10, 10, 1000, 900);
        sprintf(Name1, "fh2_Twim_vs_ToF_Plastic_%i", i + 1);
        sprintf(Name2, "Twim vs ToF for plastic %i", i + 1);
        fh2_Twim_Tof[i] = new TH2F(Name1, Name2, 7000, -87, -65, 1000, 0, 40);
        fh2_Twim_Tof[i]->GetXaxis()->SetTitle("Raw time-of-flight [ns with one bin/ps]");
        fh2_Twim_Tof[i]->GetYaxis()->SetTitle("Charge Z");
        fh2_Twim_Tof[i]->GetXaxis()->CenterTitle(true);
        fh2_Twim_Tof[i]->GetYaxis()->CenterTitle(true);
        fh2_Twim_Tof[i]->GetXaxis()->SetLabelSize(0.045);
        fh2_Twim_Tof[i]->GetXaxis()->SetTitleSize(0.045);
        fh2_Twim_Tof[i]->GetYaxis()->SetLabelSize(0.045);
        fh2_Twim_Tof[i]->GetYaxis()->SetTitleSize(0.045);
        cTwimvsTof[i]->cd();
        fh2_Twim_Tof[i]->Draw("col");
    }

    sprintf(Name1, "Mwpc3X_vs_ToF_Plastic");
    sprintf(Name2, "Mwpc3X vs ToF-Plastic number");
    cMwpc3XvsTof = new TCanvas(Name1, Name2, 10, 10, 1000, 900);
    sprintf(Name1, "fh2_Mwpc3X_vs_ToF_Plastic");
    sprintf(Name2, "Mwpc3X vs ToF for plastic number");
    fh2_Mwpc3X_Tof = new TH2F(Name1, Name2, 28 * 8, 0.5, 28.5, 288 * 8, 0.5, 288.5);
    fh2_Mwpc3X_Tof->GetXaxis()->SetTitle("ToFW-Plastic number");
    fh2_Mwpc3X_Tof->GetYaxis()->SetTitle("MWPC3-X [pads]");
    fh2_Mwpc3X_Tof->GetXaxis()->CenterTitle(true);
    fh2_Mwpc3X_Tof->GetYaxis()->CenterTitle(true);
    fh2_Mwpc3X_Tof->GetXaxis()->SetLabelSize(0.045);
    fh2_Mwpc3X_Tof->GetXaxis()->SetTitleSize(0.045);
    fh2_Mwpc3X_Tof->GetYaxis()->SetLabelSize(0.045);
    fh2_Mwpc3X_Tof->GetYaxis()->SetTitleSize(0.045);
    cMwpc3XvsTof->cd();
    fh2_Mwpc3X_Tof->Draw("col");

    sprintf(Name1, "Mwpc3Y_vs_PosToF_Plastic");
    sprintf(Name2, "Mwpc3Y vs PosToF-Plastic 11");
    cMwpc3YvsPosTof = new TCanvas(Name1, Name2, 10, 10, 1000, 900);
    sprintf(Name1, "fh2_Mwpc3Y_vs_PosToF_Plastic");
    sprintf(Name2, "Mwpc3Y vs PosToF for plastic number");
    fh2_Mwpc3Y_PosTof = new TH2F(Name1, Name2, 1000, -20, 20, 240, 0, 120);
    fh2_Mwpc3Y_PosTof->GetXaxis()->SetTitle("Pos ToFW-Plastic [ps]");
    fh2_Mwpc3Y_PosTof->GetYaxis()->SetTitle("MWPC3-Y [pads]");
    fh2_Mwpc3Y_PosTof->GetXaxis()->CenterTitle(true);
    fh2_Mwpc3Y_PosTof->GetYaxis()->CenterTitle(true);
    fh2_Mwpc3Y_PosTof->GetXaxis()->SetLabelSize(0.045);
    fh2_Mwpc3Y_PosTof->GetXaxis()->SetTitleSize(0.045);
    fh2_Mwpc3Y_PosTof->GetYaxis()->SetLabelSize(0.045);
    fh2_Mwpc3Y_PosTof->GetYaxis()->SetTitleSize(0.045);
    cMwpc3YvsPosTof->cd();
    fh2_Mwpc3Y_PosTof->Draw("col");

    // --- --------------- --- //
    // --- MAIN FOLDER-ToFW --- //
    // --- --------------- --- //
    TFolder* mainfolToFW = new TFolder("SOFTOFW", "SOFTOFW info");
    for (Int_t j = 0; j < NbChs; j++)
    {
        mainfolToFW->Add(cToFWFineTime[j]);
        mainfolToFW->Add(cToFWEneRaw[j]);
    }
    mainfolToFW->Add(cToFWMult);
    mainfolToFW->Add(cToFWRawPos);
    mainfolToFW->Add(cMwpc3XvsTof);
    mainfolToFW->Add(cMwpc3YvsPosTof);

    // --- --------------- ---
    // --- FOLDER-ToFW raw data
    // --- --------------- ---
    TFolder* folToFWRawTof = new TFolder("RawTof_AtTcal_Mult1", "TOFW Raw info");
    for (Int_t i = 0; i < NbDets; i++)
        folToFWRawTof->Add(cToFWRawTof[i]);

    // --- --------------- ---
    // --- FOLDER-Twim vs ToFW
    // --- --------------- ---
    TFolder* folTwimvsToF = new TFolder("Twim_vs_ToFW", "Twim vs TOFW info");
    for (Int_t i = 0; i < NbDets; i++)
        folTwimvsToF->Add(cTwimvsTof[i]);

    mainfolToFW->Add(folToFWRawTof);
    mainfolToFW->Add(folTwimvsToF);
    run->AddObject(mainfolToFW);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_SOFTOFW_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofToFWOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofToFWOnlineSpectra::Reset_Histo";
    for (Int_t j = 0; j < NbChs; j++)
    {
        // === MULT === //
        fh2_mult[j]->Reset();
        for (Int_t i = 0; i < NbDets; i++)
        {
            // === FINE TIME === //
            fh1_finetime[i * NbChs + j]->Reset();
            fh1_EneRaw[i * NbChs + j]->Reset();
        }
    }

    for (Int_t i = 0; i < NbDets; i++)
    {
        // === RAW POSITION === //
        fh1_RawPos_AtTcalMult1[i]->Reset();
    }
    for (UShort_t i = 0; i < NbDets; i++)
        fh2_Twim_Tof[i]->Reset();

    fh2_Mwpc3X_Tof->Reset();
    fh2_Mwpc3Y_PosTof->Reset();
}

void R3BSofToFWOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofToFWOnlineSpectra::Exec FairRootManager not found";

    Int_t nHits;
    UShort_t iDet; // 0-bsed
    UShort_t iCh;  // 0-based
    Double_t iRawTimeNs[NbDets * 2];
    UShort_t mult[NbDets * NbChs];

    // --- -------------- --- //
    // --- initialisation --- //
    // --- -------------- --- //
    for (UShort_t i = 0; i < NbDets; i++)
    {
        for (UShort_t j = 0; j < NbChs; j++)
        {
            mult[i * NbChs + j] = 0;
        }
    }

    if (fMappedItemsToFW && fMappedItemsToFW->GetEntriesFast() && fTcalItemsToFW && fTcalItemsToFW->GetEntriesFast())
    {

        // --- --------------------- --- //
        // --- loop over mapped data --- //
        // --- --------------------- --- //
        nHits = fMappedItemsToFW->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofToFWMappedData* hitmapped = (R3BSofToFWMappedData*)fMappedItemsToFW->At(ihit);
            if (!hitmapped)
                continue;
            iDet = hitmapped->GetDetector() - 1;
            iCh = hitmapped->GetPmt() - 1;
            mult[iDet * NbChs + iCh]++;
            fh1_finetime[iDet * NbChs + iCh]->Fill(hitmapped->GetTimeFine());
	    fh1_EneRaw[iDet * NbChs + iCh]->Fill(hitmapped->GetEnergy());
        }

        // --- ------------------- --- //
        // --- loop over tcal data --- //
        // --- ------------------- --- //
        nHits = fTcalItemsToFW->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofToFWTcalData* hittcal = (R3BSofToFWTcalData*)fTcalItemsToFW->At(ihit);
            if (!hittcal)
                continue;
            if (hittcal->GetPmt() == 3)
                continue;
            iDet = hittcal->GetDetector() - 1;
            iCh = hittcal->GetPmt() - 1;
            iRawTimeNs[iDet * 2 + iCh] = hittcal->GetRawTimeNs();
            ;
        }

        // --- ----------------------------------- --- //
        // --- Get the Time at the Start detectors --- //
        // --- ----------------------------------- --- //
        Double_t TrawStart = -1000000.;
        if (fSingleTcalItemsSci && (fSingleTcalItemsSci->GetEntriesFast() == 1))
        {
            R3BSofSciSingleTcalData* hitsingletcalsci = (R3BSofSciSingleTcalData*)fSingleTcalItemsSci->At(0);
            if (hitsingletcalsci)
            {
                TrawStart = hitsingletcalsci->GetRawTimeNs(1);
            }
        } // end of if mult=1 in the Start

        // Get hit data Twim
        Double_t twimZ = 0.;
        if (fHitItemsTwim && fHitItemsTwim->GetEntriesFast() > 0)
        {
            nHits = fHitItemsTwim->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BSofTwimHitData* hit = (R3BSofTwimHitData*)fHitItemsTwim->At(ihit);
                if (!hit)
                    continue;
                twimZ = hit->GetZcharge();
            }
        }

        // Get cal data MWPC3
        Double_t mwpc3x = -1., qmax = -100.;
        Double_t mwpc3y = -1., qmay = -100.;
        if (fCalItemsMwpc && fCalItemsMwpc->GetEntriesFast() > 0)
        {
            nHits = fCalItemsMwpc->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BSofMwpcCalData* hit = (R3BSofMwpcCalData*)fCalItemsMwpc->At(ihit);
                if (!hit)
                    continue;
                if (hit->GetQ() > qmax && hit->GetPlane() == 1)
                {
                    mwpc3x = hit->GetPad();
                    qmax = hit->GetQ();
                }
                if (hit->GetQ() > qmay && hit->GetPlane() == 3)
                {
                    mwpc3y = hit->GetPad();
                    qmay = hit->GetQ();
                }
            }
        }

        // --- ----------------------------------------- --- //
        // --- filling some histogramms outside the loop --- //
        // --- ----------------------------------------- --- //
        Double_t tofw = 0.;
        for (UShort_t i = 0; i < NbDets; i++)
        {
            for (UShort_t j = 0; j < NbChs; j++)
            {
                fh2_mult[j]->Fill(i + 1, mult[i * NbChs + j]);
            }
            if ((mult[iDet * NbChs] == 1) && (mult[iDet * NbChs + 1] == 1))
            {
                fh1_RawPos_AtTcalMult1[i]->Fill((Float_t)(iRawTimeNs[i * 2] - iRawTimeNs[i * 2 + 1]));
                if (TrawStart != -1000000.)
                {
                    tofw = (0.5 * (iRawTimeNs[i * 2] + iRawTimeNs[i * 2 + 1])) - TrawStart;
                    fh1_RawTof_AtTcalMult1[i]->Fill(tofw);
                    if (twimZ > 0)
                    {
                        fh2_Twim_Tof[i]->Fill(tofw, twimZ);
                    }
                    if (mwpc3x > 0 && tofw < -62. && tofw > -90. && twimZ > 0)
                    {
                        fh2_Mwpc3X_Tof->Fill(i + gRandom->Uniform(-0.5, 0.5), mwpc3x + gRandom->Uniform(-0.5, 0.5));
                    }
                    if (i == 10 && mwpc3y > 0 && tofw < -62. && tofw > -90. && twimZ > 0)
                    {
                        fh2_Mwpc3Y_PosTof->Fill(1. * (iRawTimeNs[i * 2] - iRawTimeNs[i * 2 + 1]),
                                                mwpc3y + gRandom->Uniform(-0.5, 0.5));
                    }
                }
            } // end of if mult=1 in the plastic
        }
    }

    fNEvents += 1;
}

void R3BSofToFWOnlineSpectra::FinishEvent()
{
    if (fMappedItemsToFW)
    {
        fMappedItemsToFW->Clear();
    }
    if (fTcalItemsToFW)
    {
        fTcalItemsToFW->Clear();
    }
    if (fSingleTcalItemsSci)
    {
        fSingleTcalItemsSci->Clear();
    }
    if (fHitItemsTwim)
    {
        fHitItemsTwim->Clear();
    }
    if (fCalItemsMwpc)
    {
        fCalItemsMwpc->Clear();
    }
}

void R3BSofToFWOnlineSpectra::FinishTask()
{

    if (fMappedItemsToFW)
    {
        cToFWMult->Write();
        for (UShort_t j = 0; j < NbChs; j++)
        {
            fh2_mult[j]->Write();
            cToFWFineTime[j]->Write();
            for (UShort_t i = 0; i < NbDets; i++)
            {
                fh1_finetime[i * NbChs + j]->Write();
                fh1_EneRaw[i * NbChs + j]->Write();
            }
        }
    }

    if (fTcalItemsToFW)
    {
        cToFWRawPos->Write();
        for (UShort_t i = 0; i < NbDets; i++)
        {
            fh1_RawPos_AtTcalMult1[i]->Write();
            if (fSingleTcalItemsSci)
            {
                cToFWRawTof[i]->Write();
                fh1_RawTof_AtTcalMult1[i]->Write();
            }
        }
    }

    if (fHitItemsTwim && fTcalItemsToFW)
    {
        for (UShort_t i = 0; i < NbDets; i++)
            cTwimvsTof[i]->Write();
    }

    if (fCalItemsMwpc && fTcalItemsToFW)
    {
        cMwpc3XvsTof->Write();
        cMwpc3YvsPosTof->Write();
    }
}

ClassImp(R3BSofToFWOnlineSpectra)
