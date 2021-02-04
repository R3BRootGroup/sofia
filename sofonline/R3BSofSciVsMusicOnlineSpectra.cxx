// ------------------------------------------------------------
// -----          R3BSofScivsMusicOnlineSpectra           -----
// -----   Fill SofSci vs R3B-MUSIC online histograms     -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofSciVsMusicOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BMusicCalData.h"
#include "R3BMusicHitData.h"
#include "R3BSofSciCalData.h"
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

R3BSofSciVsMusicOnlineSpectra::R3BSofSciVsMusicOnlineSpectra()
    : FairTask("SofSciVsMusicOnlineSpectra", 1)
    , fTcalSci(NULL)
    , fCalSci(NULL)
    , fMusHitItems(NULL)
    , fMusCalItems(NULL)
    , fNEvents(0)
    , fNbDetectors(2)
    , fNbChannels(3)
    , fIdS2(1)
    , fIdS8(0)
    , fBrho0(9.048)
    , fDS2(7000.)
    , fDCC(20000.)
{
    fCalTofS2min = new TArrayF(4);
    fCalTofS2max = new TArrayF(4);
    fCalTofS8min = new TArrayF(2);
    fCalTofS8max = new TArrayF(2);
}

R3BSofSciVsMusicOnlineSpectra::R3BSofSciVsMusicOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fTcalSci(NULL)
    , fCalSci(NULL)
    , fMusHitItems(NULL)
    , fMusCalItems(NULL)
    , fNEvents(0)
    , fNbDetectors(2)
    , fNbChannels(3)
    , fIdS2(1)
    , fIdS8(0)
    , fBrho0(9.048)
    , fDS2(7000.)
    , fDCC(20000.)
{
    fCalTofS2min = new TArrayF(4);
    fCalTofS2max = new TArrayF(4);
    fCalTofS8min = new TArrayF(2);
    fCalTofS8max = new TArrayF(2);
}

R3BSofSciVsMusicOnlineSpectra::~R3BSofSciVsMusicOnlineSpectra()
{
    LOG(INFO) << "R3BSofSciVsMusicOnlineSpectra::Delete instance";
    if (fTcalSci)
        delete fTcalSci;
    if (fCalSci)
        delete fCalSci;
    if (fMusHitItems)
        delete fMusHitItems;
    if (fMusCalItems)
        delete fMusCalItems;
}

InitStatus R3BSofSciVsMusicOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofSciVsMusicOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofSciVsMusicOnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // --- ---------------------------------- --- //
    // --- get access to tcal data of the Sci --- //
    // --- ---------------------------------- --- //
    fTcalSci = (TClonesArray*)mgr->GetObject("SofSciTcalData");
    if (!fTcalSci)
    {
        return kFATAL;
    }

    // --- --------------------------------- --- //
    // --- get access to cal data of the Sci --- //
    // --- --------------------------------- --- //
    fCalSci = (TClonesArray*)mgr->GetObject("SofSciCalData");
    if (!fCalSci)
    {
        return kFATAL;
    }

    // --- --------------------------------------- --- //
    // --- get access to hit data of the R3B-MUSIC --- //
    // --- --------------------------------------- --- //
    fMusHitItems = (TClonesArray*)mgr->GetObject("MusicHitData");
    if (!fMusHitItems)
        LOG(WARNING) << "R3BSofSciVsMusicOnlineSpectra: MusicHitData not found";

    // --- --------------------------------------- --- //
    // --- get access to cal data of the R3B-MUSIC --- //
    // --- --------------------------------------- --- //
    fMusCalItems = (TClonesArray*)mgr->GetObject("MusicCalData");
    if (!fMusCalItems)
        LOG(WARNING) << "R3BSofSciVsMusicOnlineSpectra: MusicCalData not found";

    // --- ------------------------------- --- //
    // --- Create histograms for detectors --- //
    // --- ------------------------------- --- //
    char Name1[255];
    char Name2[255];

    cMusicZvsCalPos = new TCanvas*[fNbDetectors];
    fh2_MusZvsCalPos = new TH2F*[fNbDetectors];

    // === R3B MUSIC CHARGE VERSUS RAW POSITION === //
    for (Int_t i = 0; i < fNbDetectors; i++)
    {
        sprintf(Name1, "R3BMusZ_vs_PosSci%02d", i + 1);
        cMusicZvsCalPos[i] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
        cMusicZvsCalPos[i]->cd();
        sprintf(Name1, "MusZ_vs_CalPos_Sci%02d", i + 1);
        fh2_MusZvsCalPos[i] = new TH2F(Name1, Name1, 2000, -100, 100, 1200, 1, 41);
        fh2_MusZvsCalPos[i]->GetXaxis()->SetTitle("(RIGHT,Wix. side) -->  SofSci X [mm] --> (LEFT,Mes. side) -->");
        fh2_MusZvsCalPos[i]->GetYaxis()->SetTitle("Charge (Z)");
        fh2_MusZvsCalPos[i]->GetYaxis()->SetTitleOffset(1.1);
        fh2_MusZvsCalPos[i]->GetXaxis()->CenterTitle(true);
        fh2_MusZvsCalPos[i]->GetYaxis()->CenterTitle(true);
        fh2_MusZvsCalPos[i]->GetXaxis()->SetLabelSize(0.045);
        fh2_MusZvsCalPos[i]->GetXaxis()->SetTitleSize(0.045);
        fh2_MusZvsCalPos[i]->GetYaxis()->SetLabelSize(0.045);
        fh2_MusZvsCalPos[i]->GetYaxis()->SetTitleSize(0.045);
        fh2_MusZvsCalPos[i]->Draw("colz");
    }

    // === R3B-MUSIC Hit data vs Sci CalPos === //
    cMusicDTvsCalPos = new TCanvas("R3BMusDT_vs_SciPos", "Music DT vs SciCalPos", 10, 10, 800, 700);
    cMusicDTvsCalPos->cd();
    fh2_MusDTvsCalPos = new TH2F("fh2_MusDT_vs_SciPos", "R3B-MUSIC DT vs SciCalPos", 1400, -70, 70, 800, -20, 20);
    fh2_MusDTvsCalPos->GetXaxis()->SetTitle("(RIGHT,Wix. side) -->  SofSci X [mm] --> (LEFT,Mes. side) -->");
    fh2_MusDTvsCalPos->GetYaxis()->SetTitle("Drift Time (mm)");
    fh2_MusDTvsCalPos->GetYaxis()->SetTitleOffset(1.1);
    fh2_MusDTvsCalPos->GetXaxis()->CenterTitle(true);
    fh2_MusDTvsCalPos->GetYaxis()->CenterTitle(true);
    fh2_MusDTvsCalPos->GetXaxis()->SetLabelSize(0.045);
    fh2_MusDTvsCalPos->GetXaxis()->SetTitleSize(0.045);
    fh2_MusDTvsCalPos->GetYaxis()->SetLabelSize(0.045);
    fh2_MusDTvsCalPos->GetYaxis()->SetTitleSize(0.045);
    fh2_MusDTvsCalPos->Draw("col");

    // === MUSIC HIT DATA VERSUS TOF FROM S2
    if (fIdS2 > 0)
    {
        cMusicZvsCalTofS2 = new TCanvas*[fNbDetectors - fIdS2];
        fh2_MusZvsCalTofS2 = new TH2F*[fNbDetectors - fIdS2];
        Float_t minS2 = 0, maxS2 = 1;

        for (Int_t dstop = fIdS2; dstop < fNbDetectors; dstop++)
        {
            sprintf(Name1, "R3BMusZ_vs_CalTofS2-Sci%02d", dstop + 1);
            cMusicZvsCalTofS2[dstop - fIdS2] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
            cMusicZvsCalTofS2[dstop - fIdS2]->cd();

            minS2 = GetTofS2min(dstop - fIdS2);
            maxS2 = GetTofS2max(dstop - fIdS2);
            sprintf(Name1, "MusZ_vs_CalTof_S2_to_Sci%02d", dstop + 1);
            fh2_MusZvsCalTofS2[dstop - fIdS2] =
                new TH2F(Name1, Name1, 100. * (maxS2 - minS2), minS2, maxS2, 1200, 1, 41);
            fh2_MusZvsCalTofS2[dstop - fIdS2]->GetXaxis()->SetTitle("ToFS2 [ns]");
            fh2_MusZvsCalTofS2[dstop - fIdS2]->GetYaxis()->SetTitle("Charge (Z)");
            fh2_MusZvsCalTofS2[dstop - fIdS2]->GetYaxis()->SetTitleOffset(1.1);
            fh2_MusZvsCalTofS2[dstop - fIdS2]->GetXaxis()->CenterTitle(true);
            fh2_MusZvsCalTofS2[dstop - fIdS2]->GetYaxis()->CenterTitle(true);
            fh2_MusZvsCalTofS2[dstop - fIdS2]->GetXaxis()->SetLabelSize(0.045);
            fh2_MusZvsCalTofS2[dstop - fIdS2]->GetXaxis()->SetTitleSize(0.045);
            fh2_MusZvsCalTofS2[dstop - fIdS2]->GetYaxis()->SetLabelSize(0.045);
            fh2_MusZvsCalTofS2[dstop - fIdS2]->GetYaxis()->SetTitleSize(0.045);
            fh2_MusZvsCalTofS2[dstop - fIdS2]->Draw("colz");
        }
    }

    // === MUSIC HIT DATA VERSUS TOF FROM S8
    if (fIdS8 > 0)
    {
        cMusicZvsCalTofS8 = new TCanvas*[fNbDetectors - fIdS8];
        fh2_MusZvsCalTofS8 = new TH2F*[fNbDetectors - fIdS8];
        Float_t minS8 = 0, maxS8 = 1;
        for (Int_t dstop = fIdS8; dstop < fNbDetectors; dstop++)
        {
            sprintf(Name1, "R3BMusZ_vs_CalTofS8-Sci%02d", dstop + 1);
            cMusicZvsCalTofS8[dstop - fIdS8] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
            cMusicZvsCalTofS8[dstop - fIdS8]->cd();

            minS8 = GetTofS8min(dstop - fIdS8);
            maxS8 = GetTofS8max(dstop - fIdS8);
            sprintf(Name1, "MusZ_vs_CalTof_S8_to_Sci%02d", dstop + 1);
            fh2_MusZvsCalTofS8[dstop - fIdS8] =
                new TH2F(Name1, Name1, 100 * (maxS8 - minS8), minS8, maxS8, 1200, 1, 41);
            fh2_MusZvsCalTofS8[dstop - fIdS8]->GetXaxis()->SetTitle("ToFS8 [ns]");
            fh2_MusZvsCalTofS8[dstop - fIdS8]->GetYaxis()->SetTitle("Charge (Z)");
            fh2_MusZvsCalTofS8[dstop - fIdS8]->GetYaxis()->SetTitleOffset(1.1);
            fh2_MusZvsCalTofS8[dstop - fIdS8]->GetXaxis()->CenterTitle(true);
            fh2_MusZvsCalTofS8[dstop - fIdS8]->GetYaxis()->CenterTitle(true);
            fh2_MusZvsCalTofS8[dstop - fIdS8]->GetXaxis()->SetLabelSize(0.045);
            fh2_MusZvsCalTofS8[dstop - fIdS8]->GetXaxis()->SetTitleSize(0.045);
            fh2_MusZvsCalTofS8[dstop - fIdS8]->GetYaxis()->SetLabelSize(0.045);
            fh2_MusZvsCalTofS8[dstop - fIdS8]->GetYaxis()->SetTitleSize(0.045);
            fh2_MusZvsCalTofS8[dstop - fIdS8]->Draw("colz");
        }
    }

    // === HIT DATA AoverQ VERSUS Q === //
    if (fIdS2 > 0)
    {
        cAqvsq = new TCanvas("FRSv_AoverQ_vs_Q", "A/q versus q 2D", 10, 10, 800, 700);
        cAqvsq->cd();
        fh2_Aqvsq = new TH2F("fh2v_Aq_vs_q_frs", "FRS: A/q vs q", 3000, 1., 3, 1300, 8, 39.5);
        fh2_Aqvsq->GetXaxis()->SetTitle("A/q");
        fh2_Aqvsq->GetYaxis()->SetTitle("Z [Charge units]");
        fh2_Aqvsq->GetYaxis()->SetTitleOffset(1.1);
        fh2_Aqvsq->GetXaxis()->CenterTitle(true);
        fh2_Aqvsq->GetYaxis()->CenterTitle(true);
        fh2_Aqvsq->GetXaxis()->SetLabelSize(0.045);
        fh2_Aqvsq->GetXaxis()->SetTitleSize(0.045);
        fh2_Aqvsq->GetYaxis()->SetLabelSize(0.045);
        fh2_Aqvsq->GetYaxis()->SetTitleSize(0.045);
        fh2_Aqvsq->Draw("colz");

        cAqvsq_mult1Tref = new TCanvas("Z_vs_AoverQ", "Z versus A/q versus if multTref==1", 10, 10, 800, 700);
        cAqvsq_mult1Tref->cd();
        fh2_Aqvsq_mult1Tref = new TH2F("fh2_AoQ_vs_q_mult1Tref", "q vs A/q if mult Tref==1", 2000, 2., 3, 1200, 10, 40);
        fh2_Aqvsq_mult1Tref->GetXaxis()->SetTitle("A/q");
        fh2_Aqvsq_mult1Tref->GetYaxis()->SetTitle("Z [Charge units]");
        fh2_Aqvsq_mult1Tref->GetYaxis()->SetTitleOffset(1.1);
        fh2_Aqvsq_mult1Tref->GetXaxis()->CenterTitle(true);
        fh2_Aqvsq_mult1Tref->GetYaxis()->CenterTitle(true);
        fh2_Aqvsq_mult1Tref->GetXaxis()->SetLabelSize(0.045);
        fh2_Aqvsq_mult1Tref->GetXaxis()->SetTitleSize(0.045);
        fh2_Aqvsq_mult1Tref->GetYaxis()->SetLabelSize(0.045);
        fh2_Aqvsq_mult1Tref->GetYaxis()->SetTitleSize(0.045);
        fh2_Aqvsq_mult1Tref->Draw("colz");
    }

    // --- --------------- --- //
    // --- MAIN FOLDER-Sci --- //
    // --- --------------- --- //
    TFolder* mainfol = new TFolder("SofSci-R3BMusic", "SofSci vs R3B-Music");
    mainfol->Add(cMusicDTvsCalPos);
    for (UShort_t d = 0; d < fNbDetectors; d++)
    {
        mainfol->Add(cMusicZvsCalPos[d]);
    }
    if (fIdS8 > 0)
    {
        for (UShort_t d = fIdS8; d < fNbDetectors; d++)
        {
            mainfol->Add(cMusicZvsCalTofS8[d - fIdS8]);
        }
    }
    if (fIdS2 > 0)
    {
        for (UShort_t d = fIdS2; d < fNbDetectors; d++)
        {
            mainfol->Add(cMusicZvsCalTofS2[d - fIdS2]);
        }
        mainfol->Add(cAqvsq);
        mainfol->Add(cAqvsq_mult1Tref);
    }
    run->AddObject(mainfol);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_SOFSCI_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofSciVsMusicOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofSciVsMusicOnlineSpectra::Reset_Histo";

    fh2_MusDTvsCalPos->Reset();

    for (Int_t d = 0; d < fNbDetectors; d++)
    {
        fh2_MusZvsCalPos[d]->Reset();
    }

    if (fIdS2 > 0)
    {

        for (Int_t d = fIdS2; d < fNbDetectors; d++)
        {
            fh2_MusZvsCalTofS2[d - fIdS2]->Reset();
        }
        fh2_Aqvsq->Reset();
        fh2_Aqvsq_mult1Tref->Reset();
    }

    if (fIdS8)
    {
        for (Int_t d = fIdS8; d < fNbDetectors; d++)
        {
            fh2_MusZvsCalTofS8[d - fIdS8]->Reset();
        }
    }
}

void R3BSofSciVsMusicOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofSciVsMusicOnlineSpectra::Exec FairRootManager not found";

    Int_t nHits;
    Int_t multTcal[fNbDetectors * 3];
    Double_t TofS2[fNbDetectors - fIdS2];
    Double_t TofS8[fNbDetectors - fIdS8];
    Double_t Gamma, Brho, xS2;

    for (Int_t det = 0; det < fNbDetectors; det++)
    {
        for (Int_t pmt = 0; pmt < 3; pmt++)
        {
            multTcal[det * 3 + pmt] = 0;
        }
    }

    // --- -------------- --- //
    // --- MUSIC Hit data --- //
    // --- -------------- --- //
    Double_t MusicZ = 0.;
    Double_t MusicDT = -1000000.;
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

    // --- -------------- --- //
    // --- MUSIC Cal data --- //
    // --- -------------- --- //
    if (fMusCalItems && fMusCalItems->GetEntriesFast() > 0)
    {
        nHits = fMusCalItems->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BMusicCalData* hit = (R3BMusicCalData*)fMusCalItems->At(ihit);
            if (!hit)
                continue;
            if (hit->GetAnodeID() == 5)
                MusicDT = hit->GetDTime();
        }
    }

    // --- ---------------- --- //
    // --- SofSci tcal data --- //
    // --- ---------------- --- //
    if (fTcalSci && fTcalSci->GetEntriesFast())
    {
        nHits = fTcalSci->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofSciTcalData* hit = (R3BSofSciTcalData*)fTcalSci->At(ihit);
            if (!hit)
                continue;
            multTcal[3 * (hit->GetDetector() - 1) + hit->GetPmt() - 1]++;
        }
    }
    // --- --------------- --- //
    // --- SofSci cal data --- //
    // --- --------------- --- //
    if (fCalSci && fCalSci->GetEntriesFast())
    {
        xS2 = 0.;
        Gamma = 1.;

        for (Int_t i = fIdS2; i < fNbDetectors; i++)
            TofS2[i - fIdS2] = 0;
        for (Int_t i = fIdS8; i < fNbDetectors; i++)
            TofS8[i - fIdS8] = 0;
        nHits = fCalSci->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofSciCalData* hit = (R3BSofSciCalData*)fCalSci->At(ihit);
            if (!hit)
            {
                continue;
            }
            fh2_MusZvsCalPos[hit->GetDetector() - 1]->Fill(hit->GetPosMm(), MusicZ);
            if (hit->GetDetector() == fNbDetectors)
            {
                fh2_MusDTvsCalPos->Fill(hit->GetPosMm(), MusicDT);
            }
            if (hit->GetDetector() == fIdS2)
            {
                xS2 = hit->GetPosMm();
            }
            if (fIdS2 > 0 && hit->GetDetector() > fIdS2)
            {
                fh2_MusZvsCalTofS2[hit->GetDetector() - 1 - fIdS2]->Fill(hit->GetTofNs_S2(), MusicZ);
                if (hit->GetDetector() == fNbDetectors)
                {
                    Gamma = 1. / (TMath::Sqrt(1. - TMath::Power(hit->GetBeta_S2(), 2)));
                    // if X is increasing from left to right:
                    //    Brho = fBhro0 * (1 - xMwpc0/fDCC + xS2/fDS2)
                    // in R3BRoot, X is increasing from right to left
                    //    Bro = fBrho0 * (1 + xMwpc0/fDCC - xS2/fDS2)
                    Brho = fBrho0 * (1. - xS2 / fDS2); // + X_mwpc0/fDCC
                    fh2_Aqvsq->Fill(Brho / (3.10716 * Gamma * hit->GetBeta_S2()), MusicZ);
                    if (multTcal[3 * (fNbDetectors - 1) + 2] == 1 && multTcal[3 * (fIdS2 - 1) + 2] == 1)
                    {
                        fh2_Aqvsq_mult1Tref->Fill(Brho / (3.10716 * Gamma * hit->GetBeta_S2()), MusicZ);
                    }
                }
            }
            if (fIdS8 > 0 && hit->GetDetector() > fIdS8)
                fh2_MusZvsCalTofS8[hit->GetDetector() - 1 - fIdS8]->Fill(hit->GetTofNs_S8(), MusicZ);
        }
    }
    fNEvents += 1;
}

// -----   Public method Reset   ------------------------------------------------
void R3BSofSciVsMusicOnlineSpectra::Reset() { LOG(DEBUG) << "Clearing TofWHitData Structure"; }

// -----   Public method Finish   -----------------------------------------------
void R3BSofSciVsMusicOnlineSpectra::FinishEvent()
{
    if (fTcalSci)
    {
        fTcalSci->Clear();
    }
    if (fCalSci)
    {
        fCalSci->Clear();
    }
    if (fMusHitItems)
    {
        fMusHitItems->Clear();
    }
    if (fMusCalItems)
    {
        fMusCalItems->Clear();
    }
}

void R3BSofSciVsMusicOnlineSpectra::FinishTask()
{
    if (fMusCalItems)
    {
        fh2_MusDTvsCalPos->Write();
        for (Int_t d = 0; d < fNbDetectors; d++)
        {
            fh2_MusZvsCalPos[d]->Write();
        }
        if (fIdS2 > 0)
        {
            fh2_Aqvsq->Write();
            fh2_Aqvsq_mult1Tref->Write();
            for (Int_t d = fIdS2; d < fNbDetectors; d++)
                fh2_MusZvsCalTofS2[d - fIdS2]->Write();
        }
        if (fIdS8 > 0)
        {
            for (Int_t d = fIdS8; d < fNbDetectors; d++)
                fh2_MusZvsCalTofS8[d - fIdS8]->Write();
        }
    }
}
