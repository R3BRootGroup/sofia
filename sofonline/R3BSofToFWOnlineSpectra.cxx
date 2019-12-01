// ------------------------------------------------------------
// -----                  R3BSofToFWOnlineSpectra           -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofToFWOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BSofSciSingleTcalData.h"
#include "R3BSofToFWMappedData.h"
#include "R3BSofToFWTcalData.h"
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
    , fNEvents(0)
{
}

R3BSofToFWOnlineSpectra::R3BSofToFWOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fMappedItemsToFW(NULL)
    , fTcalItemsToFW(NULL)
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

    // --- --------------- --- //
    // --- MAIN FOLDER-ToFW --- //
    // --- --------------- --- //
    TFolder* mainfolToFW = new TFolder("SOFTOFW", "SOFTOFW info");
    for (Int_t j = 0; j < NbChs; j++)
    {
        mainfolToFW->Add(cToFWFineTime[j]);
    }
    mainfolToFW->Add(cToFWMult);
    mainfolToFW->Add(cToFWRawPos);
    for (Int_t i = 0; i < NbDets; i++)
        mainfolToFW->Add(cToFWRawTof[i]);
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
        }
    }

    for (Int_t i = 0; i < NbDets; i++)
    {
        // === RAW POSITION === //
        fh1_RawPos_AtTcalMult1[i]->Reset();
    }
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

        // --- ----------------------------------------- --- //
        // --- filling some histogramms outside the loop --- //
        // --- ----------------------------------------- --- //
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
                    fh1_RawTof_AtTcalMult1[i]->Fill((Double_t)(0.5 * (iRawTimeNs[i * 2] + iRawTimeNs[i * 2 + 1])) -
                                                    TrawStart);
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
}

ClassImp(R3BSofToFWOnlineSpectra)
