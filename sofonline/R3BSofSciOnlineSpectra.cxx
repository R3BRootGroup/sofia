// ------------------------------------------------------------
// -----                  R3BSofSciOnlineSpectra           -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofSciOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BMusicHitData.h"
#include "R3BSofMwpcCalData.h"
#include "R3BSofSciMappedData.h"
#include "R3BSofSciTcalData.h"
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

R3BSofSciOnlineSpectra::R3BSofSciOnlineSpectra()
    : FairTask("SofSciOnlineSpectra", 1)
    , fMappedItemsSci(NULL)
    , fTcalItemsSci(NULL)
    , fMusHitItems(NULL)
    , fCalItemsMwpc0(NULL)
    , fNEvents(0)
{
}

R3BSofSciOnlineSpectra::R3BSofSciOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fMappedItemsSci(NULL)
    , fTcalItemsSci(NULL)
    , fMusHitItems(NULL)
    , fCalItemsMwpc0(NULL)
    , fNEvents(0)
{
}

R3BSofSciOnlineSpectra::~R3BSofSciOnlineSpectra()
{
    LOG(INFO) << "R3BSofSciOnlineSpectra::Delete instance";
    if (fMappedItemsSci)
        delete fMappedItemsSci;
    if (fTcalItemsSci)
        delete fTcalItemsSci;
    if (fMusHitItems)
        delete fMusHitItems;
    if (fCalItemsMwpc0)
        delete fCalItemsMwpc0;
}

InitStatus R3BSofSciOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofSciOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofSciOnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // --- ------------------------------------ --- //
    // --- get access to mapped data of the SCI --- //
    // --- ------------------------------------ --- //
    fMappedItemsSci = (TClonesArray*)mgr->GetObject("SofSciMappedData");
    if (!fMappedItemsSci)
    {
        return kFATAL;
    }

    // --- ---------------------------------- --- //
    // --- get access to tcal data of the SCI --- //
    // --- ---------------------------------- --- //
    fTcalItemsSci = (TClonesArray*)mgr->GetObject("SofSciTcalData");
    if (!fTcalItemsSci)
    {
        return kFATAL;
    }

    // get access to hit data of the MUSIC
    fMusHitItems = (TClonesArray*)mgr->GetObject("MusicHitData");
    if (!fMusHitItems)
        LOG(WARNING) << "R3BSofSciOnlineSpectra: MusicHitData not found";

    // get access to cal data of the MWPC0
    fCalItemsMwpc0 = (TClonesArray*)mgr->GetObject("Mwpc0CalData");
    if (!fCalItemsMwpc0)
        LOG(WARNING) << "R3BSofSciOnlineSpectra: Mwpc0CalData not found";

    // --- ------------------------------- --- //
    // --- Create histograms for detectors --- //
    // --- ------------------------------- --- //
    char Name1[255];
    char Name2[255];

    for (Int_t i = 0; i < NbDetectors; i++)
    {

        // === FINE TIME AND MULT === //
        sprintf(Name1, "SofSci%i_MultAndFt", i + 1);
        cSciMult[i] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
        cSciMult[i]->Divide(2, 2);
        for (Int_t j = 0; j < NbChannels; j++)
        {
            sprintf(Name1, "SofSci%i_FineTimeCh%i", i + 1, j + 1);
            fh1_finetime[i * NbChannels + j] = new TH1I(Name1, Name1, 1000, 0, 1000);
            cSciMult[i]->cd(j + 1);
            fh1_finetime[i * NbChannels + j]->Draw("");
        }
        sprintf(Name1, "SofSci%i_MultPerChannel", i + 1);
        fh2_mult[i] = new TH2I(Name1, Name1, NbChannels, 0.5, NbChannels + 0.5, 20, -0.5, 19.5);
        fh2_mult[i]->GetXaxis()->SetTitle("channel: 1=PMT R,    2=PMT L,    3=COMMON REF");
        fh2_mult[i]->GetYaxis()->SetTitle("multiplicity per channel");
        cSciMult[i]->cd(4);
        fh2_mult[i]->Draw("COL");

        // === RAW POSITION === //
        sprintf(Name1, "SofSci%i_RawPos", i + 1);
        cSciRawPos[i] = new TCanvas(Name1, Name1, 10, 10, 500, 500);
        sprintf(Name1, "SofSci%i_RawPosAtTcal_Mult1", i + 1);
        fh1_RawPos_AtTcalMult1[i] = new TH1F(Name1, Name1, 10000, -10, 10);
        fh1_RawPos_AtTcalMult1[i]->GetXaxis()->SetTitle("Raw position [ns with one bin/ps]");
        fh1_RawPos_AtTcalMult1[i]->GetYaxis()->SetTitle("Counts per bin");
        fh1_RawPos_AtTcalMult1[i]->GetXaxis()->CenterTitle(true);
        fh1_RawPos_AtTcalMult1[i]->GetYaxis()->CenterTitle(true);
        fh1_RawPos_AtTcalMult1[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_RawPos_AtTcalMult1[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_RawPos_AtTcalMult1[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_RawPos_AtTcalMult1[i]->GetYaxis()->SetTitleSize(0.045);
        cSciRawPos[i]->cd();
        fh1_RawPos_AtTcalMult1[i]->Draw("");
    }

    // Music Hit data vs SCI-RawPos
    TCanvas* cMusicZvsRawPos =
        new TCanvas("Musicchargez_vs_RawPosAtTcal_Mult1", "Music charge Z vs RawPosAtTcal_Mult1", 10, 10, 800, 700);
    fh2_MusZvsRawPos =
        new TH2F("fh2_Musicchargez_vs_RawPos", "Music charge Z vs RawPosAtTcal_Mult1", 10000, -7, 7, 240, 0, 40);
    fh2_MusZvsRawPos->GetXaxis()->SetTitle("Raw position [ns with one bin/ps]");
    fh2_MusZvsRawPos->GetYaxis()->SetTitle("Charge (Z)");
    fh2_MusZvsRawPos->GetYaxis()->SetTitleOffset(1.1);
    fh2_MusZvsRawPos->GetXaxis()->CenterTitle(true);
    fh2_MusZvsRawPos->GetYaxis()->CenterTitle(true);
    fh2_MusZvsRawPos->GetXaxis()->SetLabelSize(0.045);
    fh2_MusZvsRawPos->GetXaxis()->SetTitleSize(0.045);
    fh2_MusZvsRawPos->GetYaxis()->SetLabelSize(0.045);
    fh2_MusZvsRawPos->GetYaxis()->SetTitleSize(0.045);
    fh2_MusZvsRawPos->Draw("col");

    // Mwpc0 cal data vs SCI-RawPos
    TCanvas* cMwpc0vsRawPos =
        new TCanvas("Mwpc0_vs_RawPosAtTcal_Mult1", "Mwpc0-X vs RawPosAtTcal_Mult1", 10, 10, 800, 700);
    fh2_Mwpc0vsRawPos = new TH2F("fh2_Mwpc_vs_RawPos", "Mwpc0-X vs RawPosAtTcal_Mult1", 10000, -7, 7, 258, 0.5, 64.5);
    fh2_Mwpc0vsRawPos->GetXaxis()->SetTitle("Raw position [ns with one bin/ps]");
    fh2_Mwpc0vsRawPos->GetYaxis()->SetTitle("Mwpc0-X [pads]");
    fh2_Mwpc0vsRawPos->GetYaxis()->SetTitleOffset(1.1);
    fh2_Mwpc0vsRawPos->GetXaxis()->CenterTitle(true);
    fh2_Mwpc0vsRawPos->GetYaxis()->CenterTitle(true);
    fh2_Mwpc0vsRawPos->GetXaxis()->SetLabelSize(0.045);
    fh2_Mwpc0vsRawPos->GetXaxis()->SetTitleSize(0.045);
    fh2_Mwpc0vsRawPos->GetYaxis()->SetLabelSize(0.045);
    fh2_Mwpc0vsRawPos->GetYaxis()->SetTitleSize(0.045);
    fh2_Mwpc0vsRawPos->Draw("col");

    // --- --------------- --- //
    // --- MAIN FOLDER-Sci --- //
    // --- --------------- --- //
    TFolder* mainfolSci = new TFolder("SOFSCI", "SOFSCI info");
    for (Int_t i = 0; i < NbDetectors; i++)
    {
        mainfolSci->Add(cSciMult[i]);
        mainfolSci->Add(cSciRawPos[i]);
    }
    mainfolSci->Add(cMusicZvsRawPos);
    mainfolSci->Add(cMwpc0vsRawPos);
    run->AddObject(mainfolSci);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_SOFSCI_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofSciOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofSciOnlineSpectra::Reset_Histo";
    for (Int_t i = 0; i < NbDetectors; i++)
    {
        // === MULT AND FINE TIME === //
        fh2_mult[i]->Reset();
        for (Int_t j = 0; j < NbChannels; j++)
        {
            fh1_finetime[i * NbChannels + j]->Reset();
        }
        // === RAW POSITION === //
        fh1_RawPos_AtTcalMult1[i]->Reset();
    }
    fh2_MusZvsRawPos->Reset();
    fh2_Mwpc0vsRawPos->Reset();
}

void R3BSofSciOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofSciOnlineSpectra::Exec FairRootManager not found";

    Int_t nHits;
    UShort_t iDet; // 0-bsed
    UShort_t iCh;  // 0-based
    Double_t iRawTimeNs[NbDetectors * 2];
    UShort_t mult[NbDetectors * NbChannels];

    // --- -------------- --- //
    // --- initialisation --- //
    // --- -------------- --- //
    for (UShort_t i = 0; i < NbDetectors; i++)
    {
        for (UShort_t j = 0; j < NbChannels; j++)
        {
            mult[i * NbChannels + j] = 0;
        }
    }

    // MUSIC hit data
    Double_t MusicZ = 0.;
    if (fMusHitItems && fMusHitItems->GetEntriesFast() > 0)
    {
        nHits = fMusHitItems->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BMusicHitData* hit = (R3BMusicHitData*)fMusHitItems->At(ihit);
            if (!hit)
                continue;
            MusicZ = hit->GetZcharge();
        }
    }

    if (fMappedItemsSci && fMappedItemsSci->GetEntriesFast() && fTcalItemsSci && fTcalItemsSci->GetEntriesFast())
    {

        // --- --------------------- --- //
        // --- loop over mapped data --- //
        // --- --------------------- --- //
        nHits = fMappedItemsSci->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofSciMappedData* hitmapped = (R3BSofSciMappedData*)fMappedItemsSci->At(ihit);
            if (!hitmapped)
                continue;
            iDet = hitmapped->GetDetector() - 1;
            iCh = hitmapped->GetPmt() - 1;
            mult[iDet * NbChannels + iCh]++;
            fh1_finetime[iDet * NbChannels + iCh]->Fill(hitmapped->GetTimeFine());
        }

        // --- ------------------- --- //
        // --- loop over tcal data --- //
        // --- ------------------- --- //
        nHits = fTcalItemsSci->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofSciTcalData* hittcal = (R3BSofSciTcalData*)fTcalItemsSci->At(ihit);
            if (!hittcal)
                continue;
            if (hittcal->GetPmt() == 3)
                continue;
            iDet = hittcal->GetDetector() - 1;
            iCh = hittcal->GetPmt() - 1;
            iRawTimeNs[iDet * 2 + iCh] = hittcal->GetRawTimeNs();
            ;
        }

        // Get cal data MWPC0
        Double_t mwpc0x = -1., qmax = -100.;
        if (fCalItemsMwpc0 && fCalItemsMwpc0->GetEntriesFast() > 0)
        {
            nHits = fCalItemsMwpc0->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BSofMwpcCalData* hit = (R3BSofMwpcCalData*)fCalItemsMwpc0->At(ihit);
                if (!hit)
                    continue;
                if (hit->GetQ() > qmax && hit->GetPlane() == 1)
                {
                    mwpc0x = hit->GetPad();
                    qmax = hit->GetQ();
                }
            }
        }

        // --- ----------------------------------------- --- //
        // --- filling some histogramms outside the loop --- //
        // --- ----------------------------------------- --- //
        Double_t possci = 0.;
        for (UShort_t i = 0; i < NbDetectors; i++)
        {
            for (UShort_t j = 0; j < NbChannels; j++)
            {
                fh2_mult[i]->Fill(j + 1, mult[i * NbChannels + j]);
            }
            if ((mult[iDet * NbChannels] == 1) && (mult[iDet * NbChannels + 1] == 1))
            {
                // x position increases from left to right : TrawRIGHT - TrawLEFT
                possci = iRawTimeNs[i * 2] - iRawTimeNs[i * 2 + 1];
                fh1_RawPos_AtTcalMult1[i]->Fill(possci);
                if (MusicZ > 0.)
                    fh2_MusZvsRawPos->Fill(possci, MusicZ);

                if (mwpc0x > 0 && possci > -10. && possci < 10.)
                {
                    fh2_Mwpc0vsRawPos->Fill(possci, mwpc0x + gRandom->Uniform(-0.5, 0.5));
                }
            }
        }
    }
    fNEvents += 1;
}

void R3BSofSciOnlineSpectra::FinishEvent()
{
    if (fMappedItemsSci)
    {
        fMappedItemsSci->Clear();
    }
    if (fTcalItemsSci)
    {
        fTcalItemsSci->Clear();
    }
    if (fMusHitItems)
    {
        fMusHitItems->Clear();
    }
    if (fCalItemsMwpc0)
    {
        fCalItemsMwpc0->Clear();
    }
}

void R3BSofSciOnlineSpectra::FinishTask()
{

    if (fMappedItemsSci)
    {
        for (UShort_t i = 0; i < NbDetectors; i++)
        {
            fh2_mult[i]->Write();
            cSciMult[i]->Write();
            for (UShort_t j = 0; j < NbChannels; j++)
            {
                fh1_finetime[i * NbChannels + j]->Write();
            }
        }
    }

    if (fTcalItemsSci)
    {
        for (UShort_t i = 0; i < NbDetectors; i++)
        {
            fh1_RawPos_AtTcalMult1[i]->Write();
            cSciRawPos[i]->Write();
        }
        if (fMusHitItems)
            fh2_MusZvsRawPos->Write();
        if (fCalItemsMwpc0)
            fh2_Mwpc0vsRawPos->Write();
    }
}

ClassImp(R3BSofSciOnlineSpectra)
