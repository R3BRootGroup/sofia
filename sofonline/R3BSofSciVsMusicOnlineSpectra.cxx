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

    if (fIdS2 > 0)
    {
        // === HIT DATA AoverQ VERSUS Q === //

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

        // === Music E versus A/Q depending on rate at S2 === //
        cEvsAoQ = new TCanvas("R3BMusEsum_vs_AoQ", "Music Esum vs Aoq", 10, 10, 800, 700);
        cEvsAoQ->Divide(3, 2);
        cEcorrvsAoQ = new TCanvas("R3BMusEcorr_vs_AoQ", "Music Esum corr from beta vs Aoq", 10, 10, 800, 700);
        cEcorrvsAoQ->Divide(3, 2);
        fh2_EvsAoQ = new TH2F*[6];
        fh2_EcorrvsAoQ = new TH2F*[6];

        fh2_EvsAoQ[0] = new TH2F("E_vs_AoQ_mult1", "E_vs_AoQ_mult1", 800, 2.25, 2.650, 1100, 1000, 6500);
        fh2_EvsAoQ[1] = new TH2F("E_vs_AoQ_mult2-5", "E_vs_AoQ_mult2-5", 800, 2.25, 2.650, 1100, 1000, 6500);
        fh2_EvsAoQ[2] = new TH2F("E_vs_AoQ_mult6-10", "E_vs_AoQ_mult6-10", 800, 2.25, 2.650, 1100, 1000, 6500);
        fh2_EvsAoQ[3] = new TH2F("E_vs_AoQ_mult11-15", "E_vs_AoQ_mult11-16", 800, 2.25, 2.650, 1100, 1000, 6500);
        fh2_EvsAoQ[4] = new TH2F("E_vs_AoQ_mult16-20", "E_vs_AoQ_mult16-20", 800, 2.25, 2.650, 1100, 1000, 6500);
        fh2_EvsAoQ[5] = new TH2F("E_vs_AoQ_multup20", "E_vs_AoQ_multup20", 800, 2.25, 2.650, 1100, 1000, 6500);
        fh2_EcorrvsAoQ[0] = new TH2F("Ecorr_vs_AoQ_mult1", "Ecorr_vs_AoQ_mult1", 800, 2.25, 2.650, 1100, 1000, 6500);
        fh2_EcorrvsAoQ[1] =
            new TH2F("Ecorr_vs_AoQ_mult2-5", "Ecorr_vs_AoQ_mult2-5", 800, 2.25, 2.650, 1100, 1000, 6500);
        fh2_EcorrvsAoQ[2] =
            new TH2F("Ecorr_vs_AoQ_mult6-10", "Ecorr_vs_AoQ_mult6-10", 800, 2.25, 2.650, 1100, 1000, 6500);
        fh2_EcorrvsAoQ[3] =
            new TH2F("Ecorr_vs_AoQ_mult11-15", "Ecorr_vs_AoQ_mult11-15", 800, 2.25, 2.650, 1100, 1000, 6500);
        fh2_EcorrvsAoQ[4] =
            new TH2F("Ecorr_vs_AoQ_mult16-20", "Ecorr_vs_AoQ_mult16-20", 800, 2.25, 2.650, 1100, 1000, 6500);
        fh2_EcorrvsAoQ[5] =
            new TH2F("Ecorr_vs_AoQ_multup20", "Ecorr_vs_AoQ_multup20", 800, 2.25, 2.650, 1100, 1000, 6500);

        for (Int_t i = 0; i < 6; i++)
        {
            fh2_EvsAoQ[i]->GetXaxis()->SetTitle("A/Q");
            fh2_EvsAoQ[i]->GetYaxis()->SetTitle("Esum [channels]");
            fh2_EvsAoQ[i]->GetYaxis()->SetTitleOffset(1.1);
            fh2_EvsAoQ[i]->GetXaxis()->CenterTitle(true);
            fh2_EvsAoQ[i]->GetYaxis()->CenterTitle(true);
            fh2_EvsAoQ[i]->GetXaxis()->SetLabelSize(0.045);
            fh2_EvsAoQ[i]->GetXaxis()->SetTitleSize(0.045);
            fh2_EvsAoQ[i]->GetYaxis()->SetLabelSize(0.045);
            fh2_EvsAoQ[i]->GetYaxis()->SetTitleSize(0.045);
            cEvsAoQ->cd(i + 1);
            fh2_EvsAoQ[i]->Draw("colz");

            fh2_EcorrvsAoQ[i]->GetXaxis()->SetTitle("A/Q");
            fh2_EcorrvsAoQ[i]->GetYaxis()->SetTitle("Esum [channels]");
            fh2_EcorrvsAoQ[i]->GetYaxis()->SetTitleOffset(1.1);
            fh2_EcorrvsAoQ[i]->GetXaxis()->CenterTitle(true);
            fh2_EcorrvsAoQ[i]->GetYaxis()->CenterTitle(true);
            fh2_EcorrvsAoQ[i]->GetXaxis()->SetLabelSize(0.045);
            fh2_EcorrvsAoQ[i]->GetXaxis()->SetTitleSize(0.045);
            fh2_EcorrvsAoQ[i]->GetYaxis()->SetLabelSize(0.045);
            fh2_EcorrvsAoQ[i]->GetYaxis()->SetTitleSize(0.045);
            cEcorrvsAoQ->cd(i + 1);
            fh2_EcorrvsAoQ[i]->Draw("colz");
        }

        // === MUSIC Eraw vs BETA === //
        cEvsBeta = new TCanvas("MusE_vs_Beta", "MusE_vs_Beta", 10, 10, 800, 700);
        cEvsBeta->Divide(2, 1);

        fh2_ErawVsBeta = new TH2F("ErawVsBeta", "ErawVsBeta", 1200, 0.74, 0.80, 1100, 1000, 6500);
        cEvsBeta->cd(1);
        fh2_ErawVsBeta->GetYaxis()->SetTitle("Esum - R3B MUSIC");
        fh2_ErawVsBeta->GetXaxis()->SetTitle("Beta");
        fh2_ErawVsBeta->Draw("col");

        fh2_EcorrBetaVsBeta = new TH2F("EcorrBetaVsBeta", "EcorrBetaVsBeta", 1200, 0.74, 0.80, 1100, 1000, 6500);
        cEvsBeta->cd(2);
        fh2_EcorrBetaVsBeta->GetYaxis()->SetTitle("E corrected from beta - R3B MUSIC");
        fh2_EcorrBetaVsBeta->GetXaxis()->SetTitle("Beta");
        fh2_EcorrBetaVsBeta->Draw("col");

        // === MUSIC Ecorr vs BETA and DT === //
        cEcorr = new TCanvas("MusEcorr", "MusEcorr", 10, 10, 800, 700);
        cEcorr->Divide(3, 1);

        fh1_Esum = new TH1F("Esum", "Esum", 1075, 2200, 6500);
        fh1_EcorrBeta = new TH1F("EcorrBeta", "EcorrBeta", 1075, 2200, 6500);
        fh1_EcorrBetaDT = new TH1F("EcorrBetaDT", "EcorrBetaDT", 1075, 2200, 6500);
        cEcorr->cd(1);
        fh1_Esum->SetLineColor(kBlue);
        fh1_EcorrBeta->SetLineColor(kRed);
        fh1_EcorrBetaDT->SetLineColor(8);
        fh1_EcorrBetaDT->SetFillColor(8);
        fh1_EcorrBetaDT->Draw();
        fh1_Esum->Draw("same");
        fh1_EcorrBeta->Draw("same");

        fh2_EcorrBetaVsDT = new TH2F("EcorrBetaVsDT", "EcorrBetaVsDT", 800, -20, 20, 860, 2200, 6500);
        fh2_EcorrBetaVsDT->GetXaxis()->SetTitle("Drift Time [mm]");
        fh2_EcorrBetaVsDT->GetYaxis()->SetTitle("E corrected from beta");
        cEcorr->cd(2);
        fh2_EcorrBetaVsDT->Draw("COL");

        fh2_EcorrBetaDTVsDT = new TH2F("EcorrBetaDTVsDT", "EcorrBetaDTVsDT", 800, -20, 20, 860, 2200, 6500);
        fh2_EcorrBetaDTVsDT->GetXaxis()->SetTitle("Drift Time [mm]");
        fh2_EcorrBetaDTVsDT->GetYaxis()->SetTitle("E corrected from beta and DT");
        cEcorr->cd(3);
        fh2_EcorrBetaDTVsDT->Draw("COL");

        // === Music E corrected versus A/Q === //
        cEcorrvsAoQall = new TCanvas(
            "R3BMusEcorrVsAoQall", "Music Esum corr from beta and DT vs Aoq all multiplicities", 10, 10, 800, 700);
        fh2_EcorrVsAoQ_all = new TH2F("EcorrVsAoQ_allMult", "Ecorr_vs_AoQ_allMult", 800, 2.25, 2.650, 900, 2000, 6500);
        fh2_EcorrVsAoQ_all->GetXaxis()->SetTitle("mass-over-charge ratio A/Q");
        fh2_EcorrVsAoQ_all->GetYaxis()->SetTitle("E corrected from beta and DT [arb. unit]");
        cEcorrvsAoQall->cd();
        fh2_EcorrVsAoQ_all->Draw("col");

    } // end of if(fIdS2>0)

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
        mainfol->Add(cEvsAoQ);
        mainfol->Add(cEcorrvsAoQ);
        mainfol->Add(cEvsBeta);
        mainfol->Add(cEcorr);
        mainfol->Add(cEcorrvsAoQall);
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
        for (Int_t i = 0; i < 6; i++)
        {
            fh2_EvsAoQ[i]->Reset();
            fh2_EcorrvsAoQ[i]->Reset();
        }
        fh2_ErawVsBeta->Reset();
        fh2_EcorrBetaVsBeta->Reset();
        fh1_Esum->Reset();
        fh1_EcorrBeta->Reset();
        fh1_EcorrBetaDT->Reset();
        fh2_EcorrBetaVsDT->Reset();
        fh2_EcorrBetaDTVsDT->Reset();
        fh2_EcorrVsAoQ_all->Reset();
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
    Double_t Gamma, Brho, xS2, AoQ;

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
    Double_t Esum = 0.;

    if (fMusHitItems && fMusHitItems->GetEntriesFast() > 0)
    {
        nHits = fMusHitItems->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BMusicHitData* hit = (R3BMusicHitData*)fMusHitItems->At(ihit);
            if (!hit)
                continue;
            MusicZ = hit->GetZcharge();
            Esum = hit->GetEave();
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
            {
                MusicDT = hit->GetDTime();
            }
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
    Double_t Esum_vs_beta_p0 = 6047.07 / 3650.;
    Double_t Esum_vs_beta_p1 = 1975.55 / 3650.;
    Double_t Esum_vs_beta_p2 = -6626.4 / 3650.;
    Double_t Ebeta = 0.;
    Double_t Ebeta_vs_DT_p0 = 3404.87;
    Double_t Ebeta_vs_DT_p1 = -1.43623;
    Double_t EDT = 0.;
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
                    Ebeta = Esum / (Esum_vs_beta_p0 + Esum_vs_beta_p1 * hit->GetBeta_S2() +
                                    Esum_vs_beta_p2 * hit->GetBeta_S2() * hit->GetBeta_S2());
                    EDT = Ebeta * Ebeta_vs_DT_p0 / (Ebeta_vs_DT_p0 + Ebeta_vs_DT_p1 * MusicDT);
                    Gamma = 1. / (TMath::Sqrt(1. - TMath::Power(hit->GetBeta_S2(), 2)));
                    // if X is increasing from left to right:
                    //    Brho = fBhro0 * (1 - xMwpc0/fDCC + xS2/fDS2)
                    // in R3BRoot, X is increasing from right to left
                    //    Bro = fBrho0 * (1 + xMwpc0/fDCC - xS2/fDS2)
                    Brho = fBrho0 * (1. - xS2 / fDS2); // + X_mwpc0/fDCC
                    AoQ = Brho / (3.10716 * Gamma * hit->GetBeta_S2());
                    fh2_EcorrVsAoQ_all->Fill(AoQ, EDT);
                    fh2_Aqvsq->Fill(AoQ, MusicZ);
                    fh2_ErawVsBeta->Fill(hit->GetBeta_S2(), Esum);
                    fh2_EcorrBetaVsBeta->Fill(hit->GetBeta_S2(), Ebeta);
                    fh1_Esum->Fill(Esum);
                    fh1_EcorrBeta->Fill(Ebeta);
                    fh1_EcorrBetaDT->Fill(EDT);
                    fh2_EcorrBetaVsDT->Fill(MusicDT, Ebeta);
                    fh2_EcorrBetaDTVsDT->Fill(MusicDT, EDT);
                    if (multTcal[3 * (fNbDetectors - 1) + 2] == 1 && multTcal[3 * (fIdS2 - 1) + 2] == 1)
                    {
                        fh2_Aqvsq_mult1Tref->Fill(AoQ, MusicZ);

                        if (multTcal[3 * (fIdS2 - 1)] == 1 && multTcal[3 * (fIdS2 - 1) + 1] == 1)
                        {
                            fh2_EvsAoQ[0]->Fill(AoQ, Esum);
                            fh2_EcorrvsAoQ[0]->Fill(AoQ, EDT);
                        }
                        else if ((2 <= multTcal[3 * (fIdS2 - 1)] && multTcal[3 * (fIdS2 - 1)] <= 5) &&
                                 (2 <= multTcal[3 * (fIdS2 - 1) + 1] && multTcal[3 * (fIdS2 - 1) + 1] <= 5))
                        {
                            fh2_EvsAoQ[1]->Fill(AoQ, Esum);
                            fh2_EcorrvsAoQ[1]->Fill(AoQ, EDT);
                        }
                        else if ((6 <= multTcal[3 * (fIdS2 - 1)] && multTcal[3 * (fIdS2 - 1)] <= 10) &&
                                 (6 <= multTcal[3 * (fIdS2 - 1) + 1] && multTcal[3 * (fIdS2 - 1) + 1] <= 10))
                        {
                            fh2_EvsAoQ[2]->Fill(AoQ, Esum);
                            fh2_EcorrvsAoQ[2]->Fill(AoQ, EDT);
                        }
                        else if ((11 <= multTcal[3 * (fIdS2 - 1)] && multTcal[3 * (fIdS2 - 1)] <= 15) &&
                                 (11 <= multTcal[3 * (fIdS2 - 1) + 1] && multTcal[3 * (fIdS2 - 1) + 1] <= 15))
                        {
                            fh2_EvsAoQ[3]->Fill(AoQ, Esum);
                            fh2_EcorrvsAoQ[3]->Fill(AoQ, EDT);
                        }
                        else if ((16 <= multTcal[3 * (fIdS2 - 1)] && multTcal[3 * (fIdS2 - 1)] <= 20) &&
                                 (16 <= multTcal[3 * (fIdS2 - 1) + 1] && multTcal[3 * (fIdS2 - 1) + 1] <= 20))
                        {
                            fh2_EvsAoQ[4]->Fill(AoQ, Esum);
                            fh2_EcorrvsAoQ[4]->Fill(AoQ, EDT);
                        }
                        else if ((21 <= multTcal[3 * (fIdS2 - 1)]) && (21 <= multTcal[3 * (fIdS2 - 1) + 1]))
                        {
                            fh2_EvsAoQ[5]->Fill(AoQ, Esum);
                            fh2_EcorrvsAoQ[5]->Fill(AoQ, EDT);
                        }
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
            for (Int_t i = 0; i < 6; i++)
            {
                fh2_EvsAoQ[i]->Write();
                fh2_EcorrvsAoQ[i]->Write();
            }
            fh2_ErawVsBeta->Write();
            fh2_EcorrBetaVsBeta->Write();
            fh1_Esum->Write();
            fh1_EcorrBeta->Write();
            fh1_EcorrBetaDT->Write();
            fh2_EcorrBetaVsDT->Write();
            fh2_EcorrBetaDTVsDT->Write();
            fh2_EcorrVsAoQ_all->Write();
        }
        if (fIdS8 > 0)
        {
            for (Int_t d = fIdS8; d < fNbDetectors; d++)
                fh2_MusZvsCalTofS8[d - fIdS8]->Write();
        }
    }
}
