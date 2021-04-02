// ------------------------------------------------------------
// -----          R3BSofScivsTrimOnlineSpectra           -----
// -----   Fill SofSci vs R3B-MUSIC online histograms     -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofSciVsTrimOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BSofSciCalData.h"
#include "R3BSofSciSingleTcalData.h"
#include "R3BSofTrimCalData.h"
#include "R3BSofTrimHitData.h"
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

R3BSofSciVsTrimOnlineSpectra::R3BSofSciVsTrimOnlineSpectra()
    : FairTask("SofSciVsTrimOnlineSpectra", 1)
    , fSciSTcal(NULL)
    , fSciCal(NULL)
    , fTrimCal(NULL)
    , fTrimHit(NULL)
    , fNEvents(0)
    , fNbDetectors(2)
    , fNbChannels(3)
    , fIdS2(1)
    , fBrho0(9.048)
    , fDS2(7000.)
    , fDCC(20000.)
{
    fTofS2min = new TArrayF(4);
    fTofS2max = new TArrayF(4);
    fEmin = new TArrayF(4);
    fEmax = new TArrayF(4);
}

R3BSofSciVsTrimOnlineSpectra::R3BSofSciVsTrimOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fSciSTcal(NULL)
    , fSciCal(NULL)
    , fTrimCal(NULL)
    , fTrimHit(NULL)
    , fNEvents(0)
    , fNbDetectors(2)
    , fNbChannels(3)
    , fIdS2(1)
    , fBrho0(9.048)
    , fDS2(7000.)
    , fDCC(20000.)
{
    fTofS2min = new TArrayF(4);
    fTofS2max = new TArrayF(4);
    fEmin = new TArrayF(4);
    fEmax = new TArrayF(4);
}

R3BSofSciVsTrimOnlineSpectra::~R3BSofSciVsTrimOnlineSpectra()
{
    LOG(INFO) << "R3BSofSciVsTrimOnlineSpectra::Delete instance";
    if (fSciSTcal)
        delete fSciSTcal;
    if (fSciCal)
        delete fSciCal;
    if (fTrimCal)
        delete fTrimCal;
    if (fTrimHit)
        delete fTrimHit;
}

InitStatus R3BSofSciVsTrimOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofSciVsTrimOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofSciVsTrimOnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // --- ---------------------------------- --- //
    // --- get access to tcal data of the Sci --- //
    // --- ---------------------------------- --- //
    fSciSTcal = (TClonesArray*)mgr->GetObject("SofSciSingleTcalData");
    if (!fSciSTcal)
    {
        return kFATAL;
    }

    // --- --------------------------------- --- //
    // --- get access to cal data of the Sci --- //
    // --- --------------------------------- --- //
    fSciCal = (TClonesArray*)mgr->GetObject("SofSciCalData");
    if (!fSciCal)
    {
        return kFATAL;
    }

    // --- ------------------------------------------ --- //
    // --- get access to hit data of the Triple-MUSIC --- //
    // --- ------------------------------------------ --- //
    fTrimCal = (TClonesArray*)mgr->GetObject("TrimCalData");
    if (!fTrimCal)
    {
        LOG(WARNING) << "R3BSofSciVsTrimOnlineSpectra: TrimCalData not found";
    }

    // --- ------------------------------------------ --- //
    // --- get access to hit data of the Triple-MUSIC --- //
    // --- ------------------------------------------ --- //
    fTrimHit = (TClonesArray*)mgr->GetObject("TrimHitData");
    if (!fTrimHit)
    {
        LOG(WARNING) << "R3BSofSciVsTrimOnlineSpectra: TrimHitData not found";
    }

    // --- ------------------------------- --- //
    // --- Create histograms for detectors --- //
    // --- ------------------------------- --- //
    char Name1[255];
    char Name2[255];

    // === TRIPLE-MUSIC DRIFT TIME VERSUS RAW POSITION IN SCI === //
    if (fSciSTcal && fTrimCal)
    {
        c_TrimDT_vs_SciPosRaw = new TCanvas*[fNbDetectors];
        fh2_TrimDT_vs_SciPosRaw = new TH2F*[fNbDetectors * 3];
        for (Int_t i = 0; i < fNbDetectors; i++)
        {
            sprintf(Name1, "TrimDT_vs_Sci%02dPosRaw", i + 1);
            c_TrimDT_vs_SciPosRaw[i] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
            c_TrimDT_vs_SciPosRaw[i]->Divide(1, 3);
            for (Int_t j = 0; j < 3; j++)
            {
                sprintf(Name1, "DTalignedS%02d_vs_PosRawSci%02d", j + 1, i + 1);
                fh2_TrimDT_vs_SciPosRaw[i * 3 + j] = new TH2F(Name1, Name1, 200, -10, 10, 600, -10000, 30000);
                fh2_TrimDT_vs_SciPosRaw[i * 3 + j]->GetXaxis()->SetTitle(
                    "(RIGHT,Wix. side) -->  SofSci X [mm] --> (LEFT,Mes. side) -->");
                fh2_TrimDT_vs_SciPosRaw[i * 3 + j]->GetYaxis()->SetTitle("DTaligned [ns, 100ps resolution]");
                fh2_TrimDT_vs_SciPosRaw[i * 3 + j]->GetYaxis()->SetTitleOffset(1.1);
                fh2_TrimDT_vs_SciPosRaw[i * 3 + j]->GetXaxis()->CenterTitle(true);
                fh2_TrimDT_vs_SciPosRaw[i * 3 + j]->GetYaxis()->CenterTitle(true);
                fh2_TrimDT_vs_SciPosRaw[i * 3 + j]->GetXaxis()->SetLabelSize(0.045);
                fh2_TrimDT_vs_SciPosRaw[i * 3 + j]->GetXaxis()->SetTitleSize(0.045);
                fh2_TrimDT_vs_SciPosRaw[i * 3 + j]->GetYaxis()->SetLabelSize(0.045);
                fh2_TrimDT_vs_SciPosRaw[i * 3 + j]->GetYaxis()->SetTitleSize(0.045);
                c_TrimDT_vs_SciPosRaw[i]->cd(j + 1);
                fh2_TrimDT_vs_SciPosRaw[i * 3 + j]->Draw("colz");
            }
        }
    }

    // === TRIPLE-MUSIC ENERGY LOSS VERSUS POSITION / BETA / AoQ IN SCI === //
    if (fSciSTcal && fTrimHit)
    {
        c_TrimEraw_vs_SciPosRaw = new TCanvas*[fNbDetectors];
        fh2_TrimEraw_vs_SciPosRaw = new TH2F*[fNbDetectors * 3];
        if (fSciCal)
        {
            c_TrimE_vs_SciPosCal = new TCanvas*[fNbDetectors];
            fh2_TrimE_vs_SciPosCal = new TH2F*[fNbDetectors * 3];

            c_TrimE_vs_BetaS2 = new TCanvas("TrimE_vs_BetaS2", "TrimE_vs_BetaS2", 10, 10, 800, 700);
            c_TrimE_vs_BetaS2->Divide(2, 2);
            fh2_TrimE_vs_BetaS2 = new TH2F*[4];

            c_TrimE_vs_AoQraw = new TCanvas("TrimE_vs_AoQraw", "TrimE_vs_AoQraw", 10, 10, 800, 700);
            c_TrimE_vs_AoQraw->Divide(2, 2);
            fh2_TrimE_vs_AoQraw = new TH2F*[4];

            c_TrimZ_vs_AoQ = new TCanvas("TrimZvs_AoQ", "TrimZ_vs_AoQ", 10, 10, 800, 700);
            c_TrimZ_vs_AoQ->Divide(2, 2);
            fh2_TrimZ_vs_AoQ = new TH2F*[4];

            c_AoQ_vs_PosS2_condTrim = new TCanvas("AoQ_vs_PosS2_ifTrim", "AoQ_vs_PosS2_ifTrim", 10, 10, 800, 700);
            c_AoQ_vs_PosS2_condTrim->Divide(2, 2);
            fh2_AoQ_vs_PosS2_condTrim = new TH2F*[4];

            for (Int_t section = 0; section < 3; section++)
            {
                c_TrimE_vs_BetaS2->cd(section + 1);
                sprintf(Name1, "TrimES%02d_vs_BetaS2", section + 1);
                fh2_TrimE_vs_BetaS2[section] = new TH2F(Name1, Name1, 1300, 0.76, 0.89, 900, 15000, 35000);
                fh2_TrimE_vs_BetaS2[section]->GetXaxis()->SetTitle("Beta from S2");
                fh2_TrimE_vs_BetaS2[section]->GetYaxis()->SetTitle(Form("E section %i", section + 1));
                fh2_TrimE_vs_BetaS2[section]->Draw("COL");

                c_TrimE_vs_AoQraw->cd(section + 1);
                sprintf(Name1, "TrimES%02d_vs_AoQraw", section + 1);
                fh2_TrimE_vs_AoQraw[section] = new TH2F(Name1, Name1, 800, 2.20, 2.60, 900, 15000, 35000);
                fh2_TrimE_vs_AoQraw[section]->GetXaxis()->SetTitle("A/Q");
                fh2_TrimE_vs_AoQraw[section]->GetYaxis()->SetTitle(Form("E section %i", section + 1));
                fh2_TrimE_vs_AoQraw[section]->Draw("COL");

                c_TrimZ_vs_AoQ->cd(section + 1);
                sprintf(Name1, "TrimZ_S%02d_vs_AoQ", section + 1);
                fh2_TrimZ_vs_AoQ[section] = new TH2F(Name1, Name1, 800, 2.20, 2.60, 1000, 55, 95);
                fh2_TrimZ_vs_AoQ[section]->GetXaxis()->SetTitle("A/Q");
                fh2_TrimZ_vs_AoQ[section]->GetYaxis()->SetTitle(Form("Z section %i", section + 1));
                fh2_TrimZ_vs_AoQ[section]->Draw("COL");

                c_AoQ_vs_PosS2_condTrim->cd(section + 1);
                sprintf(Name1, "AoQ_vs_PosS2_condTrimiES%02d", section + 1);
                fh2_AoQ_vs_PosS2_condTrim[section] = new TH2F(Name1, Name1, 1000, -100, 100, 500, 2.20, 2.40);
                fh2_AoQ_vs_PosS2_condTrim[section]->GetYaxis()->SetTitle("A/Q");
                fh2_AoQ_vs_PosS2_condTrim[section]->GetXaxis()->SetTitle("Pos At S2 [mm]");
                fh2_AoQ_vs_PosS2_condTrim[section]->Draw("COL");
            }
            c_TrimE_vs_BetaS2->cd(4);
            sprintf(Name1, "TrimEmax_vs_BetaS2");
            fh2_TrimE_vs_BetaS2[3] = new TH2F(Name1, Name1, 1300, 0.76, 0.89, 900, 15000, 35000);
            fh2_TrimE_vs_BetaS2[3]->GetXaxis()->SetTitle("Beta from S2");
            fh2_TrimE_vs_BetaS2[3]->GetYaxis()->SetTitle("Emax");
            fh2_TrimE_vs_BetaS2[3]->Draw("COL");

            c_TrimE_vs_AoQraw->cd(4);
            sprintf(Name1, "TrimEmax_vs_AoQraw");
            fh2_TrimE_vs_AoQraw[3] = new TH2F(Name1, Name1, 800, 2.20, 2.60, 900, 15000, 35000);
            fh2_TrimE_vs_AoQraw[3]->GetXaxis()->SetTitle("A/Q");
            fh2_TrimE_vs_AoQraw[3]->GetYaxis()->SetTitle("Emax");
            fh2_TrimE_vs_AoQraw[3]->Draw("COL");

            c_TrimZ_vs_AoQ->cd(4);
            sprintf(Name1, "TrimZmax_vs_AoQ");
            fh2_TrimZ_vs_AoQ[3] = new TH2F(Name1, Name1, 800, 2.20, 2.60, 1000, 55, 95);
            fh2_TrimZ_vs_AoQ[3]->GetXaxis()->SetTitle("A/Q");
            fh2_TrimZ_vs_AoQ[3]->GetYaxis()->SetTitle("Zmax");
            fh2_TrimZ_vs_AoQ[3]->Draw("COL");

            c_AoQ_vs_PosS2_condTrim->cd(4);
            sprintf(Name1, "AoQ_vs_PosS2_condTrimESum");
            fh2_AoQ_vs_PosS2_condTrim[3] = new TH2F(Name1, Name1, 800, -60, 10, 500, 2.20, 2.40);
            fh2_AoQ_vs_PosS2_condTrim[3]->GetXaxis()->SetTitle("Pos At S2 [mm]");
            fh2_AoQ_vs_PosS2_condTrim[3]->GetYaxis()->SetTitle("A/Q");
            fh2_AoQ_vs_PosS2_condTrim[3]->Draw("COL");
        }
        for (Int_t i = 0; i < fNbDetectors; i++)
        {
            sprintf(Name1, "TrimEraw_vs_Sci%02dPosRaw", i + 1);
            c_TrimEraw_vs_SciPosRaw[i] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
            c_TrimEraw_vs_SciPosRaw[i]->Divide(1, 3);
            if (fSciCal)
            {
                sprintf(Name1, "TrimE_vs_Sci%02dPosCal", i + 1);
                c_TrimE_vs_SciPosCal[i] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
                c_TrimE_vs_SciPosCal[i]->Divide(1, 3);
            }
            for (Int_t j = 0; j < 3; j++)
            {
                sprintf(Name1, "ErawS%02d_vs_PosRawSci%02d", j + 1, i + 1);
                fh2_TrimEraw_vs_SciPosRaw[i * 3 + j] = new TH2F(Name1, Name1, 200, -10, 10, 900, 15000, 35000);
                fh2_TrimEraw_vs_SciPosRaw[i * 3 + j]->GetXaxis()->SetTitle(
                    "(RIGHT,Wix. side) -->  SofSci X [ns] --> (LEFT,Mes. side) -->");
                fh2_TrimEraw_vs_SciPosRaw[i * 3 + j]->GetYaxis()->SetTitle("Eraw [channels]");
                fh2_TrimEraw_vs_SciPosRaw[i * 3 + j]->GetYaxis()->SetTitleOffset(1.1);
                fh2_TrimEraw_vs_SciPosRaw[i * 3 + j]->GetXaxis()->CenterTitle(true);
                fh2_TrimEraw_vs_SciPosRaw[i * 3 + j]->GetYaxis()->CenterTitle(true);
                fh2_TrimEraw_vs_SciPosRaw[i * 3 + j]->GetXaxis()->SetLabelSize(0.045);
                fh2_TrimEraw_vs_SciPosRaw[i * 3 + j]->GetXaxis()->SetTitleSize(0.045);
                fh2_TrimEraw_vs_SciPosRaw[i * 3 + j]->GetYaxis()->SetLabelSize(0.045);
                fh2_TrimEraw_vs_SciPosRaw[i * 3 + j]->GetYaxis()->SetTitleSize(0.045);
                c_TrimEraw_vs_SciPosRaw[i]->cd(j + 1);
                fh2_TrimEraw_vs_SciPosRaw[i * 3 + j]->Draw("colz");

                if (fSciCal)
                {
                    sprintf(Name1, "ES%02d_vs_PosCalSci%02d", j + 1, i + 1);
                    fh2_TrimE_vs_SciPosCal[i * 3 + j] = new TH2F(Name1, Name1, 1000, -20, 20, 900, 15000, 35000);
                    fh2_TrimE_vs_SciPosCal[i * 3 + j]->GetXaxis()->SetTitle(
                        "(RIGWix. side) -->  SofSci X [mm] --> (LEFT,Mes. side) -->");
                    fh2_TrimE_vs_SciPosCal[i * 3 + j]->GetYaxis()->SetTitle("E [channels]");
                    fh2_TrimE_vs_SciPosCal[i * 3 + j]->GetYaxis()->SetTitleOffset(1.1);
                    fh2_TrimE_vs_SciPosCal[i * 3 + j]->GetXaxis()->CenterTitle(true);
                    fh2_TrimE_vs_SciPosCal[i * 3 + j]->GetYaxis()->CenterTitle(true);
                    fh2_TrimE_vs_SciPosCal[i * 3 + j]->GetXaxis()->SetLabelSize(0.045);
                    fh2_TrimE_vs_SciPosCal[i * 3 + j]->GetXaxis()->SetTitleSize(0.045);
                    fh2_TrimE_vs_SciPosCal[i * 3 + j]->GetYaxis()->SetLabelSize(0.045);
                    fh2_TrimE_vs_SciPosCal[i * 3 + j]->GetYaxis()->SetTitleSize(0.045);
                    c_TrimE_vs_SciPosCal[i]->cd(j + 1);
                    fh2_TrimE_vs_SciPosCal[i * 3 + j]->Draw("colz");
                }
            }
        }
    }

    // --- --------------- --- //
    // --- MAIN FOLDER-Sci --- //
    // --- --------------- --- //
    TFolder* mainfol = new TFolder("SofSci-SofTrim", "SofSci vs SofTrim");
    if (fSciSTcal && fTrimCal)
    {
        for (UShort_t d = 0; d < fNbDetectors; d++)
        {
            mainfol->Add(c_TrimDT_vs_SciPosRaw[d]);
        }
    }
    if (fSciSTcal && fTrimHit)
    {
        for (UShort_t d = 0; d < fNbDetectors; d++)
        {
            mainfol->Add(c_TrimEraw_vs_SciPosRaw[d]);
        }
    }
    if (fSciCal && fTrimHit)
    {
        for (UShort_t d = 0; d < fNbDetectors; d++)
        {
            mainfol->Add(c_TrimE_vs_SciPosCal[d]);
        }
        mainfol->Add(c_TrimE_vs_BetaS2);
        mainfol->Add(c_TrimE_vs_AoQraw);
        mainfol->Add(c_TrimZ_vs_AoQ);
        mainfol->Add(c_AoQ_vs_PosS2_condTrim);
    }

    run->AddObject(mainfol);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_SofSciVsTrim_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofSciVsTrimOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofSciVsTrimOnlineSpectra::Reset_Histo";

    if (fSciSTcal && fTrimCal)
    {
        for (Int_t i = 0; i < fNbDetectors * 3; i++)
        {
            fh2_TrimDT_vs_SciPosRaw[i]->Reset();
        }
    }
    if (fSciSTcal && fTrimHit)
    {
        for (Int_t i = 0; i < fNbDetectors * 3; i++)
        {
            fh2_TrimEraw_vs_SciPosRaw[i]->Reset();
        }
    }

    if (fSciSTcal && fTrimHit)
    {
        for (Int_t i = 0; i < fNbDetectors * 3; i++)
        {
            fh2_TrimE_vs_SciPosCal[i]->Reset();
        }
        for (Int_t i = 0; i < 4; i++)
        {
            fh2_TrimE_vs_BetaS2[i]->Reset();
            fh2_TrimE_vs_AoQraw[i]->Reset();
            fh2_TrimZ_vs_AoQ[i]->Reset();
            fh2_AoQ_vs_PosS2_condTrim[i]->Reset();
        }
    }
}

void R3BSofSciVsTrimOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofSciVsTrimOnlineSpectra::Exec FairRootManager not found";

    Int_t nHits;
    Float_t Eraw[3];
    Double_t DT[3];
    Float_t E[3];
    Float_t Z[3];
    for (int section = 0; section < 3; section++)
    {
        Eraw[section] = -1.;
        E[section] = -1.;
        DT[section] = -1000000.;
    }

    // --- --------------------- --- //
    // --- TRIPLE-MUSIC Cal data --- //
    // --- --------------------- --- //
    if (fTrimCal && fTrimCal->GetEntriesFast() > 0)
    {
        nHits = fTrimCal->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTrimCalData* hit = (R3BSofTrimCalData*)fTrimCal->At(ihit);
            if (!hit)
                continue;
            if (hit->GetAnodeID() == 3)
                DT[hit->GetSecID() - 1] = hit->GetDriftTimeAligned();
        }
    }

    // --- --------------------- --- //
    // --- TRIPLE-MUSIC Hit data --- //
    // --- --------------------- --- //
    if (fTrimHit && fTrimHit->GetEntriesFast() > 0)
    {
        nHits = fTrimHit->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTrimHitData* hit = (R3BSofTrimHitData*)fTrimHit->At(ihit);
            if (!hit)
                continue;
            Eraw[hit->GetSecID() - 1] = hit->GetEnergyRaw();
            E[hit->GetSecID() - 1] = hit->GetEnergyTheta();
            Z[hit->GetSecID() - 1] = hit->GetZcharge();
        }
    }

    // --- ---------------------- --- //
    // --- SofSci SingleTCal data --- //
    // --- ---------------------- --- //
    if (fSciSTcal && fSciSTcal->GetEntriesFast() && fTrimCal && fTrimHit)
    {
        nHits = fSciSTcal->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofSciSingleTcalData* hit = (R3BSofSciSingleTcalData*)fSciSTcal->At(ihit);
            if (!hit)
                continue;
            for (Int_t s = 0; s < 3; s++)
            {
                fh2_TrimEraw_vs_SciPosRaw[(hit->GetDetector() - 1) * 3 + s]->Fill(hit->GetRawPosNs(), Eraw[s]);
                fh2_TrimDT_vs_SciPosRaw[(hit->GetDetector() - 1) * 3 + s]->Fill(hit->GetRawPosNs(), DT[s]);
            }
        }
    }

    // --- --------------- --- //
    // --- SofSci Cal data --- //
    // --- --------------- --- //
    Double_t xS2 = 0.;
    Double_t xCC = 0.;
    Double_t BetaS2 = 1.;
    Double_t Gamma = 1.;
    if (fSciCal && fSciCal->GetEntriesFast() && fTrimHit)
    {
        nHits = fSciCal->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofSciCalData* hit = (R3BSofSciCalData*)fSciCal->At(ihit);
            if (!hit)
                continue;
            if (hit->GetDetector() == fNbDetectors)
            {
                BetaS2 = hit->GetBeta_S2();
                Gamma = 1. / (TMath::Sqrt(1. - TMath::Power(BetaS2, 2)));
                xCC = hit->GetPosMm();
            }
            if (hit->GetDetector() == fIdS2)
            {
                xS2 = hit->GetPosMm();
            }
            for (Int_t s = 0; s < 3; s++)
            {
                fh2_TrimE_vs_SciPosCal[(hit->GetDetector() - 1) * 3 + s]->Fill(hit->GetPosMm(), E[s]);
            }
        }
    }

    // --- ----------------------------- --- //
    // --- secondary beam identification --- //
    // --- ----------------------------- --- //
    // if X is increasing from left to right:
    //    Brho = fBhro0 * (1 - xMwpc0/fDCC + xS2/fDS2)
    // in R3BRoot, X is increasing from right to left
    //    Bro = fBrho0 * (1 + xMwpc0/fDCC - xS2/fDS2)
    Double_t Brho = fBrho0 * (1. - xS2 / fDS2); // + X_mwpc0/fDCC
    Double_t AoQraw = Brho / (3.10716 * Gamma * BetaS2);
    Double_t Emax = 0.;
    Int_t nSections = 0;
    for (Int_t section = 0; section < 3; section++)
    {
        if (E[section] > 500.)
        {
            nSections++;
            fh2_TrimE_vs_BetaS2[section]->Fill(BetaS2, E[section]);
            fh2_TrimE_vs_AoQraw[section]->Fill(AoQraw, E[section]);
            fh2_TrimZ_vs_AoQ[section]->Fill(AoQraw, Z[section]);
            if (17000 < E[section] && E[section] < 17400)
            {
                fh2_AoQ_vs_PosS2_condTrim[section]->Fill(xS2, AoQraw);
            }
        }
    }
    Emax = TMath::Max(E[0], E[1]);
    fh2_TrimE_vs_BetaS2[3]->Fill(BetaS2, Emax);
    fh2_TrimE_vs_AoQraw[3]->Fill(AoQraw, Emax);
    fh2_TrimZ_vs_AoQ[3]->Fill(AoQraw, TMath::Max(Z[0], Z[1]));
    if (17000 < Emax && Emax < 17400)
    {
        fh2_AoQ_vs_PosS2_condTrim[3]->Fill(xS2, AoQraw);
    }

    fNEvents += 1;
}

// -----   Public method Reset   ------------------------------------------------
void R3BSofSciVsTrimOnlineSpectra::Reset() { LOG(DEBUG) << "Clearing TofWHitData Structure"; }

// -----   Public method Finish   -----------------------------------------------
void R3BSofSciVsTrimOnlineSpectra::FinishEvent()
{
    if (fSciSTcal)
    {
        fSciSTcal->Clear();
    }
    if (fSciCal)
    {
        fSciCal->Clear();
    }
    if (fTrimCal)
    {
        fTrimCal->Clear();
    }
    if (fTrimHit)
    {
        fTrimHit->Clear();
    }
}

void R3BSofSciVsTrimOnlineSpectra::FinishTask()
{
    if (fTrimCal && fSciSTcal)
    {
        for (Int_t i = 0; i < fNbDetectors * 3; i++)
        {
            fh2_TrimDT_vs_SciPosRaw[i]->Write();
        }
    }
    if (fTrimHit && fSciSTcal)
    {
        for (Int_t i = 0; i < fNbDetectors * 3; i++)
        {
            fh2_TrimEraw_vs_SciPosRaw[i]->Write();
        }
    }
    if (fTrimHit && fSciCal)
    {
        for (Int_t i = 0; i < fNbDetectors * 3; i++)
        {
            fh2_TrimE_vs_SciPosCal[i]->Write();
        }
        for (Int_t i = 0; i < 4; i++)
        {
            fh2_TrimE_vs_BetaS2[i]->Write();
            fh2_TrimE_vs_AoQraw[i]->Write();
            fh2_TrimZ_vs_AoQ[i]->Write();
            fh2_AoQ_vs_PosS2_condTrim[i]->Write();
        }
    }
}
