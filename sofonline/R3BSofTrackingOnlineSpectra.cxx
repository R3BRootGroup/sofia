// ------------------------------------------------------------
// -----             R3BSofTrackingOnlineSpectra          -----
// -----    Created 28/01/20  by J.L. Rodriguez-Sanchez   -----
// -----        Fill tracking online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms for the tracking of ion with GLAD
 */

#include "R3BSofTrackingOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BMusicHitData.h"
#include "R3BMwpcHitData.h"
#include "R3BSofTrackingData.h"
#include "R3BTwimHitData.h"
#include "THttpServer.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "TArrow.h"
#include "TCanvas.h"
#include "TFolder.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLatex.h"
#include "TLine.h"
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
#include <stdlib.h>

R3BSofTrackingOnlineSpectra::R3BSofTrackingOnlineSpectra()
    : FairTask("SofTrackingOnlineSpectra", 1)
    , fNEvents(0)
    , fMwpc0HitDataCA(NULL)
    , fMwpc1HitDataCA(NULL)
    , fMwpc2HitDataCA(NULL)
    , fMwpc3HitDataCA(NULL)
    , fMusicHitDataCA(NULL)
    , fTwimHitDataCA(NULL)
    , fTofWHitDataCA(NULL)
    , fTrackingDataCA(NULL)
    , fDist_acelerator_glad(3718.0)
    , fPosTarget(1970.)
    , fWidthTarget(30.)
    , fZ_max(40.)
    , fZ_min(0.)
{
}

R3BSofTrackingOnlineSpectra::R3BSofTrackingOnlineSpectra(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNEvents(0)
    , fMwpc0HitDataCA(NULL)
    , fMwpc1HitDataCA(NULL)
    , fMwpc2HitDataCA(NULL)
    , fMwpc3HitDataCA(NULL)
    , fMusicHitDataCA(NULL)
    , fTwimHitDataCA(NULL)
    , fTofWHitDataCA(NULL)
    , fTrackingDataCA(NULL)
    , fDist_acelerator_glad(3718.0)
    , fPosTarget(1970.)
    , fWidthTarget(30.)
    , fZ_max(40.)
    , fZ_min(0.)
{
}

R3BSofTrackingOnlineSpectra::~R3BSofTrackingOnlineSpectra()
{
    LOG(INFO) << "R3BSofTrackingOnlineSpectra::Delete instance";

    if (fMwpc0HitDataCA)
    {
        delete fMwpc0HitDataCA;
    }
    if (fMwpc1HitDataCA)
    {
        delete fMwpc1HitDataCA;
    }
    if (fMwpc2HitDataCA)
    {
        delete fMwpc2HitDataCA;
    }
    if (fMusicHitDataCA)
    {
        delete fMusicHitDataCA;
    }
    if (fTwimHitDataCA)
    {
        delete fTwimHitDataCA;
    }
    if (fMwpc3HitDataCA)
    {
        delete fMwpc3HitDataCA;
    }
    if (fTofWHitDataCA)
    {
        delete fTofWHitDataCA;
    }
    if (fTrackingDataCA)
    {
        delete fTrackingDataCA;
    }
}

InitStatus R3BSofTrackingOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofTrackingOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTrackingOnlineSpectra::Init FairRootManager not found";

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    fMwpc0HitDataCA = (TClonesArray*)mgr->GetObject("Mwpc0HitData");
    if (!fMwpc0HitDataCA)
    {
        LOG(ERROR) << "R3BSofTrackingOnlineSpectra: Mwpc0HitData not found";
        return kFATAL;
    }

    fMusicHitDataCA = (TClonesArray*)mgr->GetObject("MusicHitData");
    if (!fMusicHitDataCA)
    {
        LOG(ERROR) << "R3BSofTrackingOnlineSpectra: MusicHitData not found";
        // return kFATAL;
    }

    fMwpc1HitDataCA = (TClonesArray*)mgr->GetObject("Mwpc1HitData");
    if (!fMwpc1HitDataCA)
    {
        LOG(WARNING) << "R3BSofTrackingOnlineSpectra: Mwpc1HitData not found";
    }

    fMwpc2HitDataCA = (TClonesArray*)mgr->GetObject("Mwpc1HitData");
    if (!fMwpc2HitDataCA)
    {
        LOG(ERROR) << "R3BSofTrackingOnlineSpectra: Mwpc1HitData not found";
        return kFATAL;
    }

    fMwpc3HitDataCA = (TClonesArray*)mgr->GetObject("Mwpc3HitData");
    if (!fMwpc3HitDataCA)
    {
        LOG(ERROR) << "R3BSofTrackingOnlineSpectra: Mwpc3HitData not found";
        return kFATAL;
    }

    fTrackingDataCA = (TClonesArray*)mgr->GetObject("SofTrackingData");
    if (!fTrackingDataCA)
    {
        LOG(ERROR) << "R3BSofTrackingOnlineSpectra: SofTrackingData not found";
        // return kFATAL;
    }

    fTwimHitDataCA = (TClonesArray*)mgr->GetObject("TwimHitData");
    if (!fTwimHitDataCA)
    {
        LOG(ERROR) << "R3BSofTrackingOnlineSpectra: TwimHitData not found";
        // return kFATAL;
    }

    // Create histograms for detectors
    TString Name1;
    TString Name2;

    // Hit data, tracking plane X-Z
    cTrackingXZ = new TCanvas("Tracking_before_GLAD_XZ", "Tracking plane XZ info", 10, 10, 800, 700);

    Name1 = "fh2_tracking_planeXZ";
    Name2 = "Tracking planeXZ before GLAD";
    Int_t histoYlim = 150;
    fh2_tracking_planeXZ = new TH2F(Name1, Name2, 400, 0., fDist_acelerator_glad, 400, -1. * histoYlim, histoYlim);
    fh2_tracking_planeXZ->GetXaxis()->SetTitle("Beam direction-Z [mm]");
    fh2_tracking_planeXZ->GetYaxis()->SetTitle("(Wixhausen)<---  X [mm]  ---> (Messel)");
    fh2_tracking_planeXZ->GetYaxis()->SetTitleOffset(1.1);
    fh2_tracking_planeXZ->GetXaxis()->CenterTitle(true);
    fh2_tracking_planeXZ->GetYaxis()->CenterTitle(true);
    fh2_tracking_planeXZ->GetXaxis()->SetLabelSize(0.045);
    fh2_tracking_planeXZ->GetXaxis()->SetTitleSize(0.045);
    fh2_tracking_planeXZ->GetYaxis()->SetLabelSize(0.045);
    fh2_tracking_planeXZ->GetYaxis()->SetTitleSize(0.045);
    fh2_tracking_planeXZ->Draw("colz");

    // Target indicated as a hole
    TLine* l1 = new TLine(fPosTarget, fWidthTarget / 2., fPosTarget, histoYlim);
    l1->SetLineColor(2);
    l1->SetLineWidth(2);
    l1->Draw();
    TLine* l2 = new TLine(fPosTarget, -1. * fWidthTarget / 2., fPosTarget, -1. * histoYlim);
    l2->SetLineColor(2);
    l2->SetLineWidth(2);
    l2->Draw();

    TArrow* arrow = new TArrow(200., 120., 800., 120., 0.02, ">");
    arrow->SetLineColor(3);
    arrow->SetFillStyle(1001);
    arrow->SetLineWidth(3);
    arrow->Draw();

    TLatex latex;
    latex.SetTextSize(0.045);
    latex.SetTextAlign(13);
    latex.SetTextColor(3);
    latex.DrawLatex(200., histoYlim - 10., "Beam");
    latex.SetTextColor(1);
    latex.DrawLatex(100., -1. * histoYlim + 20., "Accelerator");
    latex.SetTextColor(2);
    latex.DrawLatex(fPosTarget + 28., 1. * histoYlim - 20., "Target pos.");
    latex.SetTextColor(1);
    latex.DrawLatex(fDist_acelerator_glad - 600., -1. * histoYlim + 20., "GLAD wind.");

    // Hit data, tracking plane Y-Z
    cTrackingYZ = new TCanvas("Tracking_before_GLAD_YZ", "Tracking plane YZ info", 10, 10, 800, 700);
    Name1 = "fh2_tracking_planeYZ";
    Name2 = "Tracking planeYZ before GLAD";
    fh2_tracking_planeYZ = new TH2F(Name1, Name2, 400, 0., fDist_acelerator_glad, 400, -1. * histoYlim, histoYlim);
    fh2_tracking_planeYZ->GetXaxis()->SetTitle("Beam direction-Z [mm]");
    fh2_tracking_planeYZ->GetYaxis()->SetTitle("Y [mm]");
    fh2_tracking_planeYZ->GetYaxis()->SetTitleOffset(1.1);
    fh2_tracking_planeYZ->GetXaxis()->CenterTitle(true);
    fh2_tracking_planeYZ->GetYaxis()->CenterTitle(true);
    fh2_tracking_planeYZ->GetXaxis()->SetLabelSize(0.045);
    fh2_tracking_planeYZ->GetXaxis()->SetTitleSize(0.045);
    fh2_tracking_planeYZ->GetYaxis()->SetLabelSize(0.045);
    fh2_tracking_planeYZ->GetYaxis()->SetTitleSize(0.045);
    fh2_tracking_planeYZ->Draw("colz");

    latex.SetTextSize(0.045);
    latex.SetTextAlign(13);
    latex.SetTextColor(3);
    latex.DrawLatex(200., histoYlim - 10., "Beam");
    latex.SetTextColor(1);
    latex.DrawLatex(100., -1. * histoYlim + 20., "Accelerator");
    latex.SetTextColor(2);
    latex.DrawLatex(fPosTarget + 28., 1. * histoYlim - 20., "Target pos.");
    latex.SetTextColor(1);
    latex.DrawLatex(fDist_acelerator_glad - 600., -1. * histoYlim + 20., "GLAD wind.");
    l1->Draw();
    l2->Draw();
    arrow->Draw();

    // Hit data, Beam profile X-Y at target position
    cBeamProfileTarget = new TCanvas("Beam_profile_XY_at_target", "Beam profile XY info", 10, 10, 800, 700);
    Name1 = "fh2_beam_profile_XY";
    Name2 = "Beam profile-XY at target position";
    fh2_target_PosXY = new TH2F(Name1, Name2, 200, -100., 100., 200, -100., 100.);
    fh2_target_PosXY->GetXaxis()->SetTitle("(Wixhausen)<---  X [mm]  ---> (Messel)");
    fh2_target_PosXY->GetYaxis()->SetTitle("Y [mm]");
    fh2_target_PosXY->GetYaxis()->SetTitleOffset(1.1);
    fh2_target_PosXY->GetXaxis()->CenterTitle(true);
    fh2_target_PosXY->GetYaxis()->CenterTitle(true);
    fh2_target_PosXY->GetXaxis()->SetLabelSize(0.045);
    fh2_target_PosXY->GetXaxis()->SetTitleSize(0.045);
    fh2_target_PosXY->GetYaxis()->SetLabelSize(0.045);
    fh2_target_PosXY->GetYaxis()->SetTitleSize(0.045);
    fh2_target_PosXY->Draw("colz");

    cBeta = new TCanvas("Beta", "Beta info", 10, 10, 800, 700);

    // Hit data, beta
    Name1 = "fh1_beta_tofw";
    Name2 = "#beta, velocity in units of c";
    fh1_beta = new TH1F(Name1, Name2, 1200, 0.2, 1.0);
    fh1_beta->GetXaxis()->SetTitle("#beta");
    fh1_beta->GetYaxis()->SetTitle("Counts");
    fh1_beta->GetYaxis()->SetTitleOffset(1.15);
    fh1_beta->GetXaxis()->CenterTitle(true);
    fh1_beta->GetYaxis()->CenterTitle(true);
    fh1_beta->GetXaxis()->SetLabelSize(0.045);
    fh1_beta->GetXaxis()->SetTitleSize(0.045);
    fh1_beta->GetYaxis()->SetLabelSize(0.045);
    fh1_beta->GetYaxis()->SetTitleSize(0.045);
    fh1_beta->SetFillColor(2);
    fh1_beta->SetLineColor(1);
    fh1_beta->Draw("");

    // Hit data, brho
    cBrho = new TCanvas("Brho", "Brho info", 10, 10, 800, 700);

    Name1 = "fh1_brho_glad";
    Name2 = "GLAD: Brho";
    fh1_brho = new TH1F(Name1, Name2, 600, 4., 15.);
    fh1_brho->GetXaxis()->SetTitle("Brho [Tm]");
    fh1_brho->GetYaxis()->SetTitle("Counts");
    fh1_brho->GetYaxis()->SetTitleOffset(1.15);
    fh1_brho->GetXaxis()->CenterTitle(true);
    fh1_brho->GetYaxis()->CenterTitle(true);
    fh1_brho->GetXaxis()->SetLabelSize(0.045);
    fh1_brho->GetXaxis()->SetTitleSize(0.045);
    fh1_brho->GetYaxis()->SetLabelSize(0.045);
    fh1_brho->GetYaxis()->SetTitleSize(0.045);
    fh1_brho->SetFillColor(2);
    fh1_brho->SetLineColor(1);
    fh1_brho->Draw("");

    // Hit data, Aq_vs_q
    cAqvsq = new TCanvas("Aq_vs_q", "A/q_vs_q 2D info", 10, 10, 800, 700);

    Name1 = "fh2_Aq_vs_q";
    Name2 = "Tracking GLAD: A/q vs q";
    fh2_Aqvsq = new TH2F(Name1, Name2, 900, 0.8, 3., (fZ_max - fZ_min) * 10., fZ_min, fZ_max);
    fh2_Aqvsq->GetXaxis()->SetTitle("A/q");
    fh2_Aqvsq->GetYaxis()->SetTitle("Charge (Z)");
    fh2_Aqvsq->GetYaxis()->SetTitleOffset(1.1);
    fh2_Aqvsq->GetXaxis()->CenterTitle(true);
    fh2_Aqvsq->GetYaxis()->CenterTitle(true);
    fh2_Aqvsq->GetXaxis()->SetLabelSize(0.045);
    fh2_Aqvsq->GetXaxis()->SetTitleSize(0.045);
    fh2_Aqvsq->GetYaxis()->SetLabelSize(0.045);
    fh2_Aqvsq->GetYaxis()->SetTitleSize(0.045);
    fh2_Aqvsq->Draw("col");

    // Hit data, Mwpc3 vs beta
    cMwpc3vsBeta = new TCanvas("Mwpc3_vs_beta", "Mwpc3_vs_Beta 2D info", 10, 10, 800, 700);

    Name1 = "fh2_Mwpc3X_vs_beta";
    Name2 = "GLAD: Mwpc3-X vs #beta";
    fh2_Mwpc3vsbeta = new TH2F(Name1, Name2, 1800, -450., 450., 500, 0.2, 1.0);
    fh2_Mwpc3vsbeta->GetXaxis()->SetTitle("(Wixhausen)<---    Mwpc3-X [mm]    ---> (Messel)");
    fh2_Mwpc3vsbeta->GetYaxis()->SetTitle("#beta");
    fh2_Mwpc3vsbeta->GetYaxis()->SetTitleOffset(1.1);
    fh2_Mwpc3vsbeta->GetXaxis()->CenterTitle(true);
    fh2_Mwpc3vsbeta->GetYaxis()->CenterTitle(true);
    fh2_Mwpc3vsbeta->GetXaxis()->SetLabelSize(0.045);
    fh2_Mwpc3vsbeta->GetXaxis()->SetTitleSize(0.045);
    fh2_Mwpc3vsbeta->GetYaxis()->SetLabelSize(0.045);
    fh2_Mwpc3vsbeta->GetYaxis()->SetTitleSize(0.045);
    fh2_Mwpc3vsbeta->Draw("col");

    // Hit data, Z versus beta
    cZvsBeta = new TCanvas("TwimZ_vs_Beta", "Z versus Beta", 10, 10, 800, 700);
    fh2_ZvsBeta =
        new TH2F("VvsBeta", "Z from Twim vs Beta from TofW", 1200, 0.5, 0.9, (fZ_max - fZ_min) * 20., fZ_min, fZ_max);
    fh2_ZvsBeta->GetXaxis()->SetTitle("#beta");
    fh2_ZvsBeta->GetYaxis()->SetTitle("Charge-Z");
    fh2_ZvsBeta->GetYaxis()->SetTitleOffset(1.1);
    fh2_ZvsBeta->GetXaxis()->CenterTitle(true);
    fh2_ZvsBeta->GetYaxis()->CenterTitle(true);
    fh2_ZvsBeta->GetXaxis()->SetLabelSize(0.045);
    fh2_ZvsBeta->GetXaxis()->SetTitleSize(0.045);
    fh2_ZvsBeta->GetYaxis()->SetLabelSize(0.045);
    fh2_ZvsBeta->GetYaxis()->SetTitleSize(0.045);
    cZvsBeta->cd();
    fh2_ZvsBeta->Draw("colz");

    // MAIN FOLDER
    TFolder* mainfol = new TFolder("Tracking_Cave", "Tracking info");
    mainfol->Add(cTrackingXZ);
    mainfol->Add(cTrackingYZ);
    mainfol->Add(cBeamProfileTarget);
    mainfol->Add(cBeta);
    mainfol->Add(cBrho);
    mainfol->Add(cMwpc3vsBeta);
    mainfol->Add(cAqvsq);
    mainfol->Add(cZvsBeta);
    run->AddObject(mainfol);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_Tracking_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofTrackingOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofTrackingOnlineSpectra::Reset_Histo";

    fh1_beta->Reset();
    fh1_brho->Reset();
    fh2_Aqvsq->Reset();
    fh2_Mwpc3vsbeta->Reset();
    fh2_tracking_planeXZ->Reset();
    fh2_tracking_planeYZ->Reset();
    fh2_target_PosXY->Reset();
    fh2_ZvsBeta->Reset();
}

void R3BSofTrackingOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTrackingOnlineSpectra::Exec FairRootManager not found";

    Double_t mwpc0x = -300., mwpc0y = -300., mwpc1y = 0., mwpc2y = 0., anglemus = 0., zrand = 0., mwpc3x = -10000.;
    Double_t xtarget = -500., ytarget = -500.;

    // Fill mwpc0 Hit data
    if (fMwpc0HitDataCA && fMwpc0HitDataCA->GetEntriesFast() > 0)
    {
        Int_t nHits = fMwpc0HitDataCA->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BMwpcHitData* hit = (R3BMwpcHitData*)fMwpc0HitDataCA->At(ihit);
            if (!hit)
                continue;
            mwpc0x = hit->GetX();
            mwpc0y = hit->GetY() + 11.; // Offset in Y for alignment with the beam line
        }

        // Fill mwpc2 Hit data
        if (fMwpc2HitDataCA && fMwpc2HitDataCA->GetEntriesFast() > 0 && mwpc0y > -300.)
        {
            nHits = fMwpc2HitDataCA->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BMwpcHitData* hit = (R3BMwpcHitData*)fMwpc2HitDataCA->At(ihit);
                if (!hit)
                    continue;
                Double_t angX = (hit->GetX() - mwpc0x) / 2835.;
                Double_t angY = (hit->GetY() - mwpc0y) / 2835.;
                if (TMath::Abs(angX) < 0.075 && TMath::Abs(angY) < 0.075)
                {
                    mwpc1y = hit->GetY() - 6.0;
                    zrand = gRandom->Uniform(0., fDist_acelerator_glad);
                    fh2_tracking_planeYZ->Fill(zrand, mwpc0y + (mwpc1y - mwpc0y) / 2835. * zrand);
                    ytarget = mwpc0y + (hit->GetY() - mwpc0y) / 2835. * fPosTarget;
                    fh2_tracking_planeXZ->Fill(zrand, mwpc0x + (hit->GetX() - mwpc0x) / 2835. * zrand);
                    xtarget = mwpc0x + (hit->GetX() - mwpc0x) / 2835. * fPosTarget;
                }
            }

            // Fill music hit data
            /*        if (fMusicHitDataCA && fMusicHitDataCA->GetEntriesFast() > 0 && mwpc0x > -300.)
                    {
                        nHits = fMusicHitDataCA->GetEntriesFast();
                        for (Int_t ihit = 0; ihit < nHits; ihit++)
                        {
                            R3BMusicHitData* hit = (R3BMusicHitData*)fMusicHitDataCA->At(ihit);
                            if (!hit)
                                continue;
                            anglemus = hit->GetTheta();
                        }
                        zrand = gRandom->Uniform(0., fDist_acelerator_glad);
                        fh2_tracking_planeXZ->Fill(zrand, mwpc0x + anglemus * zrand);
                        xtarget = mwpc0x + anglemus * fPosTarget;
                    }
            */
            if (xtarget > -500. && ytarget > -500.)
                fh2_target_PosXY->Fill(xtarget, ytarget);
        }
    }

    // Fill tracking data from GLAD
    if (fTrackingDataCA && fTrackingDataCA->GetEntriesFast() > 0)
    {
        // Fill mwpc3 Hit data
        if (fMwpc3HitDataCA && fMwpc3HitDataCA->GetEntriesFast() > 0)
        {
            Int_t nHits = fMwpc3HitDataCA->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BMwpcHitData* hit = (R3BMwpcHitData*)fMwpc3HitDataCA->At(ihit);
                if (!hit)
                    continue;
                mwpc3x = hit->GetX();
            }
        }

        Int_t nHits = fTrackingDataCA->GetEntriesFast();
        Int_t nHitsTwim = fTwimHitDataCA->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTrackingData* hit = (R3BSofTrackingData*)fTrackingDataCA->At(ihit);
            if (!hit)
                continue;
            fh1_beta->Fill(hit->GetBeta());
            fh1_brho->Fill(hit->GetBrho());
            fh2_Aqvsq->Fill(hit->GetAq(), hit->GetZ());
            if (mwpc3x > -10000.)
            {
                fh2_Mwpc3vsbeta->Fill(mwpc3x, hit->GetBeta());
                if (nHitsTwim == 1)
                {
                    R3BTwimHitData* hitTwim = (R3BTwimHitData*)fTwimHitDataCA->At(0);
                    fh2_ZvsBeta->Fill(hit->GetBeta(), hitTwim->GetZcharge());
                }
            }
        }
    }

    fNEvents += 1;
}

void R3BSofTrackingOnlineSpectra::FinishEvent()
{
    if (fMwpc0HitDataCA)
    {
        fMwpc0HitDataCA->Clear();
    }

    if (fMwpc1HitDataCA)
    {
        fMwpc1HitDataCA->Clear();
    }

    if (fMusicHitDataCA)
    {
        fMusicHitDataCA->Clear();
    }

    if (fTwimHitDataCA)
    {
        fTwimHitDataCA->Clear();
    }

    if (fMwpc2HitDataCA)
    {
        fMwpc2HitDataCA->Clear();
    }

    if (fMwpc3HitDataCA)
    {
        fMwpc3HitDataCA->Clear();
    }

    if (fTofWHitDataCA)
    {
        fTofWHitDataCA->Clear();
    }

    if (fTrackingDataCA)
    {
        fTrackingDataCA->Clear();
    }
}

void R3BSofTrackingOnlineSpectra::FinishTask()
{
    if (fTrackingDataCA)
    {
        cBeta->Write();
        cBrho->Write();
        cMwpc3vsBeta->Write();
        cAqvsq->Write();
        if (fTwimHitDataCA)
            fh2_ZvsBeta->Write();
    }
    if (fMwpc0HitDataCA)
    {
        cTrackingXZ->Write();
        cTrackingYZ->Write();
        cBeamProfileTarget->Write();
    }
}

ClassImp(R3BSofTrackingOnlineSpectra)
