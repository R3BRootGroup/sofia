// ------------------------------------------------------------
// -----          R3BSofSciVsPspxOnlineSpectra            -----
// -----   Fill SofSci vs Pspx online histograms          -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofSciVsPspxOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BPspxCalData.h"
#include "R3BPspxHitData.h"
#include "R3BSofSciCalData.h"
#include "R3BSofSciSingleTcalData.h"
#include "THttpServer.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "TCanvas.h"
#include "TFolder.h"
#include "TH2.h"

#include "TClonesArray.h"
#include "TMath.h"

R3BSofSciVsPspxOnlineSpectra::R3BSofSciVsPspxOnlineSpectra()
    : FairTask("SofSciVsPspxOnlineSpectra", 1)
    , fSciSTcal(NULL)
    , fSciCal(NULL)
    , fPspxCal(NULL)
    , fPspxHit(NULL)
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
}

R3BSofSciVsPspxOnlineSpectra::R3BSofSciVsPspxOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fSciSTcal(NULL)
    , fSciCal(NULL)
    , fPspxCal(NULL)
    , fPspxHit(NULL)
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
}

R3BSofSciVsPspxOnlineSpectra::~R3BSofSciVsPspxOnlineSpectra()
{
    LOG(INFO) << "R3BSofSciVsPspxOnlineSpectra::Delete instance";
    if (fSciSTcal)
        delete fSciSTcal;
    if (fSciCal)
        delete fSciCal;
    if (fPspxCal)
        delete fPspxCal;
    if (fPspxHit)
        delete fPspxHit;
}

InitStatus R3BSofSciVsPspxOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofSciVsPspxOnlineSpectra::Init()";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
    {
        LOG(FATAL) << "R3BSofSciVsPspxOnlineSpectra::Init FairRootManager not found";
        return kFATAL;
    }
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // --- ---------------------------------- --- //
    // --- get access to tcal data of the Sci --- //
    // --- ---------------------------------- --- //
    fSciSTcal = (TClonesArray*)mgr->GetObject("SofSciSingleTcalData");
    if (!fSciSTcal)
    {
        LOG(WARNING) << "R3BSofSciVsPspxOnlineSpectra::SofSciSingleTcalData not found";
        // return kFATAL;
    }

    // --- --------------------------------- --- //
    // --- get access to cal data of the Sci --- //
    // --- --------------------------------- --- //
    fSciCal = (TClonesArray*)mgr->GetObject("SofSciCalData");
    if (!fSciCal)
    {
        LOG(WARNING) << "R3BSofSciVsPspxOnlineSpectra::SofSciCalData not found";
        // return kFATAL;
    }

    // --- ------------------------------------------ --- //
    // --- get access to cal data of the Pspx         --- //
    // --- ------------------------------------------ --- //
    fPspxHit = (TClonesArray*)mgr->GetObject("Pspx1_xCal");
    if (!fPspxHit)
    {
        LOG(WARNING) << "R3BSofSciVsPspxOnlineSpectra: Pspx1_xCal not found";
    }

    // --- ------------------------------------------ --- //
    // --- get access to hit data of the Pspx         --- //
    // --- ------------------------------------------ --- //
    /*fPspxHit = (TClonesArray*)mgr->GetObject("Pspx1_xHit");
    if (!fPspxHit)
    {
        LOG(WARNING) << "R3BSofSciVsPspxOnlineSpectra: Pspx1_xHit not found";
    }*/

    // --- ------------------------------- --- //
    // --- Create histograms for detectors --- //
    // --- ------------------------------- --- //
    char Name1[255];
    char Name2[255];

    // Pspx ENERGY LOSS VERSUS POSITION / BETA / AoQ IN SCI
    if (fSciSTcal && fPspxHit)
    {
        if (fSciCal)
        {
            c_PspxE_vs_SciPosCal = new TCanvas*[fNbDetectors];
            fh2_PspxE_vs_SciPosCal = new TH2F*[fNbDetectors];

            for (Int_t i = 0; i < fNbDetectors; i++)
            {
                sprintf(Name1, "PspxE_vs_Sci%02dPosCal", i + 1);
                c_PspxE_vs_SciPosCal[i] = new TCanvas(Name1, Name1, 10, 10, 800, 700);

                sprintf(Name1, "EPspx1_vs_PosCalSci%02d", i + 1);
                fh2_PspxE_vs_SciPosCal[i] = new TH2F(Name1, Name1, 1000, -20, 20, 900, 15000, 35000);
                fh2_PspxE_vs_SciPosCal[i]->GetXaxis()->SetTitle(
                    "(RIGWix. side) -->  SofSci X [mm] --> (LEFT,Mes. side) -->");
                fh2_PspxE_vs_SciPosCal[i]->GetYaxis()->SetTitle("E [channels]");
                fh2_PspxE_vs_SciPosCal[i]->GetYaxis()->SetTitleOffset(1.1);
                fh2_PspxE_vs_SciPosCal[i]->GetXaxis()->CenterTitle(true);
                fh2_PspxE_vs_SciPosCal[i]->GetYaxis()->CenterTitle(true);
                fh2_PspxE_vs_SciPosCal[i]->GetXaxis()->SetLabelSize(0.045);
                fh2_PspxE_vs_SciPosCal[i]->GetXaxis()->SetTitleSize(0.045);
                fh2_PspxE_vs_SciPosCal[i]->GetYaxis()->SetLabelSize(0.045);
                fh2_PspxE_vs_SciPosCal[i]->GetYaxis()->SetTitleSize(0.045);
                // c_PspxE_vs_SciPosCal[i]->cd(j + 1);
                fh2_PspxE_vs_SciPosCal[i]->Draw("colz");
            }

            c_PspxE_vs_BetaS2 = new TCanvas("PspxE_vs_BetaS2", "PspxE_vs_BetaS2", 10, 10, 800, 700);
            // c_PspxE_vs_BetaS2->Divide(2, 2);
            fh2_PspxE_vs_BetaS2 = new TH2F*[1];

            c_PspxE_vs_AoQraw = new TCanvas("PspxE_vs_AoQraw", "PspxE_vs_AoQraw", 10, 10, 800, 700);
            // c_PspxE_vs_AoQraw->Divide(2, 2);
            fh2_PspxE_vs_AoQraw = new TH2F*[1];

            for (Int_t section = 0; section < 1; section++)
            {
                c_PspxE_vs_BetaS2->cd(section + 1);
                sprintf(Name1, "PspxES%02d_vs_BetaS2", section + 1);
                fh2_PspxE_vs_BetaS2[section] = new TH2F(Name1, Name1, 1300, 0.76, 0.89, 900, 15000, 35000);
                fh2_PspxE_vs_BetaS2[section]->GetXaxis()->SetTitle("Beta from S2");
                fh2_PspxE_vs_BetaS2[section]->GetYaxis()->SetTitle(Form("E Pspx %i", section + 1));
                fh2_PspxE_vs_BetaS2[section]->Draw("COL");

                c_PspxE_vs_AoQraw->cd(section + 1);
                sprintf(Name1, "PspxES%02d_vs_AoQraw", section + 1);
                fh2_PspxE_vs_AoQraw[section] = new TH2F(Name1, Name1, 800, 2.20, 2.60, 900, 15000, 35000);
                fh2_PspxE_vs_AoQraw[section]->GetXaxis()->SetTitle("A/Q");
                fh2_PspxE_vs_AoQraw[section]->GetYaxis()->SetTitle(Form("E Pspx %i", section + 1));
                fh2_PspxE_vs_AoQraw[section]->Draw("COL");
            }
        }
    }

    // --- --------------- --- //
    // --- MAIN FOLDER-Sci --- //
    // --- --------------- --- //
    TFolder* mainfol = new TFolder("SofSci-Pspx", "SofSci vs Pspx");

    if (fSciCal && fPspxHit)
    {
        for (UShort_t d = 0; d < fNbDetectors; d++)
        {
            mainfol->Add(c_PspxE_vs_SciPosCal[d]);
        }
        mainfol->Add(c_PspxE_vs_BetaS2);
        mainfol->Add(c_PspxE_vs_AoQraw);
    }

    run->AddObject(mainfol);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_SofSciVsPspx_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofSciVsPspxOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofSciVsPspxOnlineSpectra::Reset_Histo";

    if (fSciSTcal && fPspxHit)
    {
        for (UShort_t d = 0; d < fNbDetectors; d++)
            fh2_PspxE_vs_SciPosCal[d]->Reset();
        for (Int_t i = 0; i < 1; i++)
        {
            fh2_PspxE_vs_BetaS2[i]->Reset();
            fh2_PspxE_vs_AoQraw[i]->Reset();
        }
    }
}

void R3BSofSciVsPspxOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofSciVsPspxOnlineSpectra::Exec FairRootManager not found";

    Int_t nHits = 0;
    Float_t Emax = 0.;
    Float_t Posmax = -1000.;
    // --- -------------------
    // --- Pspx Hit data
    // --- ------------------
    if (fPspxHit && fPspxHit->GetEntriesFast() > 0)
    {
        nHits = fPspxHit->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BPspxHitData* hit = (R3BPspxHitData*)fPspxHit->At(ihit);
            if (!hit)
                continue;
            if (Emax < hit->GetEnergy())
            {
                Emax = hit->GetEnergy();
                Posmax = hit->GetPos();
            }
        }
    }

    // --- ---------------
    // --- SofSci Cal data
    // --- ---------------
    Double_t xS2 = 0.;
    Double_t xCC = 0.;
    Double_t BetaS2 = 1.;
    Double_t Gamma = 1.;
    if (fSciCal && fSciCal->GetEntriesFast() > 0 && fPspxHit)
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
            fh2_PspxE_vs_SciPosCal[hit->GetDetector() - 1]->Fill(hit->GetPosMm(), Emax);
        }

        // --- -----------------------------
        // --- secondary beam identification
        // --- -----------------------------
        // if X is increasing from left to right:
        //    Brho = fBhro0 * (1 - xMwpc0/fDCC + xS2/fDS2)
        // in R3BRoot, X is increasing from right to left
        //    Bro = fBrho0 * (1 + xMwpc0/fDCC - xS2/fDS2)
        Double_t Brho = fBrho0 * (1. - xS2 / fDS2); // + X_mwpc0/fDCC
        Double_t AoQraw = Brho / (3.10716 * Gamma * BetaS2);
        for (Int_t section = 0; section < 1; section++)
        {
            if (Emax > 0.)
            {
                fh2_PspxE_vs_BetaS2[section]->Fill(BetaS2, Emax);
                fh2_PspxE_vs_AoQraw[section]->Fill(AoQraw, Emax);
            }
        }
    }

    fNEvents += 1;
}

// -----   Public method Finish   -----------------------------------------------
void R3BSofSciVsPspxOnlineSpectra::FinishEvent()
{
    if (fSciSTcal)
    {
        fSciSTcal->Clear();
    }
    if (fSciCal)
    {
        fSciCal->Clear();
    }
    if (fPspxCal)
    {
        fPspxCal->Clear();
    }
    if (fPspxHit)
    {
        fPspxHit->Clear();
    }
}

void R3BSofSciVsPspxOnlineSpectra::FinishTask()
{
    if (fPspxHit && fSciCal)
    {
        for (UShort_t d = 0; d < fNbDetectors; d++)
            fh2_PspxE_vs_SciPosCal[d]->Write();
        for (Int_t i = 0; i < 1; i++)
        {
            fh2_PspxE_vs_BetaS2[i]->Write();
            fh2_PspxE_vs_AoQraw[i]->Write();
        }
    }
}
