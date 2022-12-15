// ------------------------------------------------------------
// -----        R3BSofTrackingFissionOnlineSpectra        -----
// -----    Created 28/02/21  by J.L. Rodriguez-Sanchez   -----
// -----        Fill tracking online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms for the tracking of fission events with GLAD
 */

#include "R3BSofTrackingFissionOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BMwpcHitData.h"
#include "R3BSofTrackingData.h"
#include "R3BSofTrimHitData.h"
#include "R3BTGeoPar.h"
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

// R3BSofTrackingFissionOnlineSpectra: Default Constructor --------------------------
R3BSofTrackingFissionOnlineSpectra::R3BSofTrackingFissionOnlineSpectra()
    : FairTask("SofTrackingFissionOnlineSpectra", 1)
    , fNEvents(0)
    , fMwpc0HitDataCA(NULL)
    , fMwpc1HitDataCA(NULL)
    , fMwpc2HitDataCA(NULL)
    , fMwpc3HitDataCA(NULL)
    , fTrimHitDataCA(NULL)
    , fTwimHitDataCA(NULL)
    , fTofWHitDataCA(NULL)
    , fTrackingDataCA(NULL)
    , fDist_acelerator_glad(3810.0)
    , fPosTarget(2000.)
    , fWidthTarget(30.)
    , fZ_max(94.)
    , fZ_min(0.)
{
}

// R3BSofTrackingFissionOnlineSpectra: Standard Constructor ----------------------
R3BSofTrackingFissionOnlineSpectra::R3BSofTrackingFissionOnlineSpectra(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNEvents(0)
    , fMwpc0HitDataCA(NULL)
    , fMwpc1HitDataCA(NULL)
    , fMwpc2HitDataCA(NULL)
    , fMwpc3HitDataCA(NULL)
    , fTrimHitDataCA(NULL)
    , fTwimHitDataCA(NULL)
    , fTofWHitDataCA(NULL)
    , fTrackingDataCA(NULL)
    , fDist_acelerator_glad(3810.0)
    , fPosTarget(2000.)
    , fWidthTarget(30.)
    , fZ_max(94.)
    , fZ_min(0.)
{
}

R3BSofTrackingFissionOnlineSpectra::~R3BSofTrackingFissionOnlineSpectra()
{
    LOG(INFO) << "R3BSofTrackingFissionOnlineSpectra::Delete instance";

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
    if (fTrimHitDataCA)
    {
        delete fTrimHitDataCA;
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

// -----   Public method SetParContainers   --------------------------------
void R3BSofTrackingFissionOnlineSpectra::SetParContainers()
{
    // Parameter Container
    // Reading softrackingAnaPar from FairRuntimeDb
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();

    fMw0GeoPar = (R3BTGeoPar*)rtdb->getContainer("Mwpc0GeoPar");
    if (!fMw0GeoPar)
    {
        LOG(ERROR) << "R3BSofTrackingFissionOnlineSpectra::SetParContainers() : Could not get access to Mwpc0GeoPar "
                      "container.";
        // return;
    }
    else
        LOG(INFO) << "R3BSofTrackingFissionOnlineSpectra::SetParContainers() : Container Mwpc0GeoPar found.";

    fTargetGeoPar = (R3BTGeoPar*)rtdb->getContainer("TargetGeoPar");
    if (!fTargetGeoPar)
    {
        LOG(WARNING) << "R3BSofTrackingFissionOnlineSpectra::SetParContainers() : Could not get access to TargetGeoPar "
                        "container.";
        // return;
    }
    else
        LOG(INFO) << "R3BSofTrackingFissionOnlineSpectra::SetParContainers() : Container TargetGeoPar found.";

    fMw1GeoPar = (R3BTGeoPar*)rtdb->getContainer("Mwpc1GeoPar");
    if (!fMw1GeoPar)
    {
        LOG(ERROR) << "R3BSofTrackingFissionOnlineSpectra::SetParContainers() : Could not get access to Mwpc1GeoPar "
                      "container.";
        // return;
    }
    else
        LOG(INFO) << "R3BSofTrackingFissionOnlineSpectra::SetParContainers() : Container Mwpc1GeoPar found.";

    fMw2GeoPar = (R3BTGeoPar*)rtdb->getContainer("Mwpc2GeoPar");
    if (!fMw2GeoPar)
    {
        LOG(ERROR) << "R3BSofTrackingFissionOnlineSpectra::SetParContainers() : Could not get access to Mwpc2GeoPar "
                      "container.";
        // return;
    }
    else
        LOG(INFO) << "R3BSofTrackingFissionOnlineSpectra::SetParContainers() : Container Mwpc2GeoPar found.";

    fMw3GeoPar = (R3BTGeoPar*)rtdb->getContainer("Mwpc3GeoPar");
    if (!fMw3GeoPar)
    {
        LOG(ERROR) << "R3BSofTrackingFissionOnlineSpectra::SetParContainers() : Could not get access to Mwpc3GeoPar "
                      "container.";
        // return;
    }
    else
        LOG(INFO) << "R3BSofTrackingFissionOnlineSpectra::SetParContainers() : Container Mwpc3GeoPar found.";

    fTofWGeoPar = (R3BTGeoPar*)rtdb->getContainer("TofwGeoPar");
    if (!fTofWGeoPar)
    {
        LOG(ERROR)
            << "R3BSofTrackingFissionOnlineSpectra::SetParContainers() : Could not get access to TofwGeoPar container.";
        // return;
    }
    else
        LOG(INFO) << "R3BSofTrackingFissionOnlineSpectra::SetParContainers() : Container TofwGeoPar found.";
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofTrackingFissionOnlineSpectra::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}

InitStatus R3BSofTrackingFissionOnlineSpectra::Init()
{
    LOG(INFO) << "R3BSofTrackingFissionOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTrackingFissionOnlineSpectra::Init FairRootManager not found";

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    fMwpc0HitDataCA = (TClonesArray*)mgr->GetObject("Mwpc0HitData");
    if (!fMwpc0HitDataCA)
    {
        LOG(ERROR) << "R3BSofTrackingFissionOnlineSpectra: Mwpc0HitData not found";
        return kFATAL;
    }

    fTrimHitDataCA = (TClonesArray*)mgr->GetObject("TrimHitData");
    if (!fTrimHitDataCA)
    {
        LOG(ERROR) << "R3BSofTrackingFissionOnlineSpectra: TrimHitData not found";
        // return kFATAL;
    }

    fMwpc1HitDataCA = (TClonesArray*)mgr->GetObject("Mwpc1HitData");
    if (!fMwpc1HitDataCA)
    {
        LOG(WARNING) << "R3BSofTrackingFissionOnlineSpectra: Mwpc1HitData not found";
    }

    fMwpc2HitDataCA = (TClonesArray*)mgr->GetObject("Mwpc2HitData");
    if (!fMwpc2HitDataCA)
    {
        LOG(ERROR) << "R3BSofTrackingFissionOnlineSpectra: Mwpc2HitData not found";
        return kFATAL;
    }

    fMwpc3HitDataCA = (TClonesArray*)mgr->GetObject("Mwpc3HitData");
    if (!fMwpc3HitDataCA)
    {
        LOG(ERROR) << "R3BSofTrackingFissionOnlineSpectra: Mwpc3HitData not found";
        return kFATAL;
    }

    fTrackingDataCA = (TClonesArray*)mgr->GetObject("SofTrackingData");
    if (!fTrackingDataCA)
    {
        LOG(ERROR) << "R3BSofTrackingFissionOnlineSpectra: SofTrackingData not found";
        return kFATAL;
    }

    fTwimHitDataCA = (TClonesArray*)mgr->GetObject("TwimHitData");
    if (!fTwimHitDataCA)
    {
        LOG(ERROR) << "R3BSofTrackingFissionOnlineSpectra: TwimHitData not found";
        // return kFATAL;
    }

    // Create histograms for detectors
    TString Name1;
    TString Name2;

    // Hit data, tracking plane X-Z
    cTrackingXZ = new TCanvas("Tracking_before_GLAD_XZ", "Tracking (Lab.) plane XZ info", 10, 10, 800, 700);

    Name1 = "fh2_tracking_planeXZ";
    Name2 = "Tracking (Lab.) planeXZ before GLAD";
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
    cTrackingYZ = new TCanvas("Tracking_before_GLAD_YZ", "Tracking (Lab.) plane YZ info", 10, 10, 800, 700);
    Name1 = "fh2_tracking_planeYZ";
    Name2 = "Tracking (Lab.) planeYZ before GLAD";
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
    Name2 = "Beam profile-XY (Lab.) at target position";
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

void R3BSofTrackingFissionOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofTrackingFissionOnlineSpectra::Reset_Histo";

    fh1_beta->Reset();
    fh1_brho->Reset();
    fh2_Aqvsq->Reset();
    fh2_Mwpc3vsbeta->Reset();
    fh2_tracking_planeXZ->Reset();
    fh2_tracking_planeYZ->Reset();
    fh2_target_PosXY->Reset();
    fh2_ZvsBeta->Reset();
}

void R3BSofTrackingFissionOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTrackingFissionOnlineSpectra::Exec FairRootManager not found";

    Double_t mwpc0x = -300., mwpc0y = -300., anglemus = 0., zrand = 0., mwpc3x = -10000.;
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
            mwpc0x = hit->GetX() + fMw0GeoPar->GetPosX() * 10.; // mm
            mwpc0y = hit->GetY() + fMw0GeoPar->GetPosY() * 10.; // mm
        }

        // Fill mwpc1 Hit data for spallation reactions
        if (fMwpc1HitDataCA && fMwpc1HitDataCA->GetEntriesFast() == 1 && mwpc0y > -100.)
        {
            nHits = fMwpc1HitDataCA->GetEntriesFast();
            Float_t mwpc1x = -150., mwpc1y = -150.;
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BMwpcHitData* hit = (R3BMwpcHitData*)fMwpc1HitDataCA->At(ihit);
                if (!hit)
                    continue;
                mwpc1x = hit->GetX() + fMw1GeoPar->GetPosX() * 10.;
                mwpc1y = hit->GetY() + fMw1GeoPar->GetPosY() * 10.;
                Double_t angX = (mwpc1x - mwpc0x) / (fMw1GeoPar->GetPosZ() - fMw0GeoPar->GetPosZ()) / 10.;
                Double_t angY = (mwpc1y - mwpc0y) / (fMw1GeoPar->GetPosZ() - fMw0GeoPar->GetPosZ()) / 10.;
                if (TMath::Abs(angX) < 0.075 && TMath::Abs(angY) < 0.075)
                {
                    // zrand = gRandom->Uniform(0., fPosTarget);
                    zrand = gRandom->Uniform(0., fDist_acelerator_glad);
                    fh2_tracking_planeYZ->Fill(zrand, mwpc0y + angY * zrand);
                    ytarget = mwpc0y + angY * fPosTarget;
                    fh2_tracking_planeXZ->Fill(zrand, mwpc0x + angX * zrand);
                    xtarget = mwpc0x + angX * fPosTarget;
                }
            }
            if (xtarget > -200. && ytarget > -200.)
                fh2_target_PosXY->Fill(xtarget, ytarget);

            // Fill mwpc2 Hit data for spallation reactions
            /*if (fMwpc2HitDataCA->GetEntriesFast() == 1)
            {
                nHits = fMwpc2HitDataCA->GetEntriesFast();
                for (Int_t ihit = 0; ihit < nHits; ihit++)
                {
                    R3BMwpcHitData* hit = (R3BMwpcHitData*)fMwpc2HitDataCA->At(ihit);
                    if (!hit)
                        continue;
                    Double_t angX = (hit->GetX() + fMw2GeoPar->GetPosX() * 10. - mwpc1x) /
                                    (fMw2GeoPar->GetPosZ() - fMw1GeoPar->GetPosZ()) / 10.;
                    Double_t angY = (hit->GetY() + fMw2GeoPar->GetPosY() * 10. - mwpc1y) /
                                    (fMw2GeoPar->GetPosZ() - fMw1GeoPar->GetPosZ()) / 10.;
                    if (TMath::Abs(angX) < 0.1 && TMath::Abs(angY) < 0.1)
                    {
                        zrand = gRandom->Uniform(fPosTarget, fDist_acelerator_glad);
                        fh2_tracking_planeYZ->Fill(zrand, ytarget + angY * zrand);
                        fh2_tracking_planeXZ->Fill(zrand, xtarget + angX * zrand);
                    }
                }
            }*/
        }

        // Fill mwpc1 & mwpc2 Hit data for fission events
        if (fMwpc1HitDataCA && fMwpc2HitDataCA && fMwpc1HitDataCA->GetEntriesFast() > 1 &&
            fMwpc2HitDataCA->GetEntriesFast() > 1 && mwpc0x > -100.)
        {
            Int_t nHits1 = fMwpc1HitDataCA->GetEntriesFast();
            Int_t nHits2 = fMwpc2HitDataCA->GetEntriesFast();
            for (Int_t ihit1 = 0; ihit1 < nHits1; ihit1++)
            {
                R3BMwpcHitData* hit1 = (R3BMwpcHitData*)fMwpc1HitDataCA->At(ihit1);
                if (!hit1)
                    continue;

                float mw1x = hit1->GetX() + fMw1GeoPar->GetPosX() * 10.;
                float mw1y = hit1->GetY() + fMw1GeoPar->GetPosY() * 10.;

                for (Int_t ihit2 = 0; ihit2 < nHits2; ihit2++)
                {
                    R3BMwpcHitData* hit2 = (R3BMwpcHitData*)fMwpc2HitDataCA->At(ihit2);
                    if (!hit2)
                        continue;
                    float mw2x = hit2->GetX() + fMw2GeoPar->GetPosX() * 10.;
                    float mw2y = hit2->GetY() + fMw2GeoPar->GetPosY() * 10.;

                    if ((mw1x > 0. && mw2x > 0.) || (mw1x < 0. && mw2x < 0.))
                    {
                        zrand = gRandom->Uniform(0., fDist_acelerator_glad - fPosTarget);
                        Double_t angX = (mw2x - mw1x) / (fMw2GeoPar->GetPosZ() - fMw1GeoPar->GetPosZ()) / 10.;
                        Double_t angY = (mw2y - mw1y) / (fMw2GeoPar->GetPosZ() - fMw1GeoPar->GetPosZ()) / 10.;
                        fh2_tracking_planeXZ->Fill(
                            zrand + fPosTarget,
                            mw1x + angX * (zrand - fMw1GeoPar->GetPosZ() * 10. -
                                           730.)); // 730mm is the target position with respect to (0,0,0)
                        fh2_tracking_planeYZ->Fill(zrand + fPosTarget,
                                                   mw1y + angY * (zrand - fMw1GeoPar->GetPosZ() * 10. - 730.));
                    }
                }
            }
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

void R3BSofTrackingFissionOnlineSpectra::FinishEvent()
{
    if (fMwpc0HitDataCA)
    {
        fMwpc0HitDataCA->Clear();
    }

    if (fMwpc1HitDataCA)
    {
        fMwpc1HitDataCA->Clear();
    }

    if (fTrimHitDataCA)
    {
        fTrimHitDataCA->Clear();
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

void R3BSofTrackingFissionOnlineSpectra::FinishTask()
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

ClassImp(R3BSofTrackingFissionOnlineSpectra)
