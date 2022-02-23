// ------------------------------------------------------------
// -----          R3BAmsCorrelationOnlineSpectra          -----
// -----    Created 31/01/20  by J.L. Rodriguez-Sanchez   -----
// -----      Fill AMS-CALIFA-MUSICs online histograms    -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with AMS-CALIFA-MUSICs online data
 */

#include "R3BAmsCorrelationOnlineSpectra.h"
#include "R3BAmsHitData.h"
#include "R3BCalifaHitData.h"
#include "R3BEventHeader.h"
#include "R3BMusicHitData.h"
#include "R3BSofTrimHitData.h"
#include "R3BTwimHitData.h"
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
#include "TRandom.h"
#include "TVector3.h"

#include "TClonesArray.h"
#include "TMath.h"
#include "TVector3.h"
#include <array>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

R3BAmsCorrelationOnlineSpectra::R3BAmsCorrelationOnlineSpectra()
    : FairTask("AmsCorrelationOnlineSpectra", 1)
    , fEventHeader(nullptr)
    , fHitItemsAms(NULL)
    , fHitItemsMus(NULL)
    , fHitItemsTrim(NULL)
    , fHitItemsTwim(NULL)
    , fHitItemsCalifa(NULL)
    , fZproj(20.)
    , fTrigger(-1)
    , fNEvents(0)
    , fNbDet(6)
    , fMinProtonE(50000.)
    , fHitCalifaHist_bins(500)
    , fHitCalifaHist_max(4000)
{
}

R3BAmsCorrelationOnlineSpectra::R3BAmsCorrelationOnlineSpectra(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fEventHeader(nullptr)
    , fHitItemsAms(NULL)
    , fHitItemsMus(NULL)
    , fHitItemsTrim(NULL)
    , fHitItemsTwim(NULL)
    , fHitItemsCalifa(NULL)
    , fZproj(20.)
    , fTrigger(-1)
    , fNEvents(0)
    , fNbDet(6)
    , fMinProtonE(50000.)
    , fHitCalifaHist_bins(500)
    , fHitCalifaHist_max(4000)
{
}

R3BAmsCorrelationOnlineSpectra::~R3BAmsCorrelationOnlineSpectra()
{
    LOG(INFO) << "R3BAmsCorrelationOnlineSpectra: Delete instance";
    if (fHitItemsAms)
        delete fHitItemsAms;
    if (fHitItemsMus)
        delete fHitItemsMus;
    if (fHitItemsTrim)
        delete fHitItemsTrim;
    if (fHitItemsTwim)
        delete fHitItemsTwim;
    if (fHitItemsCalifa)
        delete fHitItemsCalifa;
}

InitStatus R3BAmsCorrelationOnlineSpectra::Init()
{
    LOG(INFO) << "R3BAmsCorrelationOnlineSpectra::Init ";

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(fatal) << "R3BAmsCorrelationOnlineSpectra::Init FairRootManager not found";

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // get access to tpat data
    fEventHeader = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    // get access to Hit data
    fHitItemsAms = (TClonesArray*)mgr->GetObject("AmsHitData");
    if (!fHitItemsAms)
    {
        LOG(WARNING) << "R3BAmsCorrelationOnlineSpectra: AmsHitData not found";
       // return kFATAL;
    }

    // get access to hit data of the TWIM
    fHitItemsTwim = (TClonesArray*)mgr->GetObject("TwimHitData");
    if (!fHitItemsTwim)
        LOG(ERROR) << "R3BAmsCorrelationOnlineSpectra: TwimHitData not found";

    // get access to hit data of the MUSIC
    fHitItemsMus = (TClonesArray*)mgr->GetObject("MusicHitData");
    if (!fHitItemsMus)
        LOG(WARNING) << "R3BAmsCorrelationOnlineSpectra: MusicHitData not found";

    // get access to hit data of the Triple-MUSIC
    fHitItemsTrim = (TClonesArray*)mgr->GetObject("TrimHitData");
    if (!fHitItemsTrim)
        LOG(WARNING) << "R3BAmsCorrelationOnlineSpectra: TrimHitData not found";

    if (!fHitItemsTrim && !fHitItemsMus)
        LOG(ERROR) << "R3BAmsCorrelationOnlineSpectra: MusicHitData and/or TrimHitData not found";

    // get access to Hit data
    fHitItemsCalifa = (TClonesArray*)mgr->GetObject("CalifaHitData");
    if (!fHitItemsCalifa)
        LOG(ERROR) << "R3BAmsCorrelationOnlineSpectra::CalifaHitData not found";

    // create histograms of all detectors

    //  CANVAS 1  -------------------------------
    cHit[0] = new TCanvas("AMS_hit_right_layer1", "Hit info", 10, 10, 500, 500);
    cHit[0]->Divide(2, 2);
    cHit[1] = new TCanvas("AMS_hit_right_layer2_top", "Hit info", 10, 10, 500, 500);
    cHit[1]->Divide(2, 2);
    cHit[2] = new TCanvas("AMS_hit_right_layer2_bottom", "Hit info", 10, 10, 500, 500);
    cHit[2]->Divide(2, 2);
    cHit[3] = new TCanvas("AMS_hit_left_layer1", "Hit info", 10, 10, 500, 500);
    cHit[3]->Divide(2, 2);
    cHit[4] = new TCanvas("AMS_hit_left_layer2_top", "Hit info", 10, 10, 500, 500);
    cHit[4]->Divide(2, 2);
    cHit[5] = new TCanvas("AMS_hit_left_layer2_bottom", "Hit info", 10, 10, 500, 500);
    cHit[5]->Divide(2, 2);

    //  CANVAS 2  -------------------------------
    cHitAngles = new TCanvas("AMS_Theta_vs_Phi", "Theta vs Phi per layer", 10, 10, 500, 500);
    cHitAngles->Divide(2, 1);

    //  CANVAS 3  -------------------------------
    cHitEnergyCor = new TCanvas("AMS_E-inner_vs_E-outer", "Energy-inner vs Energy-outer per side", 10, 10, 500, 500);
    cHitEnergyCor->Divide(2, 1);

    //  CANVAS 4  -------------------------------
    cHitThetaCor =
        new TCanvas("AMS_Theta-inner_vs_Theta-outer", "Theta-inner vs Theta-outer per side", 10, 10, 500, 500);
    cHitThetaCor->Divide(2, 1);

    //  CANVAS 5  -------------------------------
    cHitPhiCor = new TCanvas("AMS_Phi-inner_vs_Phi-outer", "Phi-inner vs Phi-outer per side", 10, 10, 500, 500);
    cHitPhiCor->Divide(2, 1);

    char Name1[255];
    char Name2[255];
    char Name3[255];

    // Hit data
    for (Int_t i = 0; i < fNbDet; i++)
    { // one histo per detector
        sprintf(Name1, "fh_Ams_hit_Mul_%d", i + 1);
        sprintf(Name2, "Multiplicity Det %d", i + 1);
        fh_Ams_hit_Mul[i] = new TH1F(Name1, Name2, 30, -0.5, 29.5);
        fh_Ams_hit_Mul[i]->GetXaxis()->SetTitle("Multiplicity");
        fh_Ams_hit_Mul[i]->GetYaxis()->SetTitle("Counts");
        fh_Ams_hit_Mul[i]->GetXaxis()->CenterTitle(true);
        fh_Ams_hit_Mul[i]->GetYaxis()->CenterTitle(true);
        fh_Ams_hit_Mul[i]->GetYaxis()->SetTitleOffset(1.1);
        fh_Ams_hit_Mul[i]->GetXaxis()->SetTitleOffset(1.1);
        cHit[i]->cd(1);
        fh_Ams_hit_Mul[i]->Draw("");

        sprintf(Name1, "fh_Ams_hit_Pos_%d", i + 1);
        sprintf(Name2, "S-side vs K-side for AMS Det %d", i + 1);
        fh_Ams_hit_Pos[i] = new TH2F(Name1, Name2, 350, 0, 70., 200, 0, 40.);
        fh_Ams_hit_Pos[i]->GetXaxis()->SetTitle("S-side [mm]");
        fh_Ams_hit_Pos[i]->GetYaxis()->SetTitle("K-side [mm]");
        fh_Ams_hit_Pos[i]->GetXaxis()->CenterTitle(true);
        fh_Ams_hit_Pos[i]->GetYaxis()->CenterTitle(true);
        fh_Ams_hit_Pos[i]->GetYaxis()->SetTitleOffset(1.1);
        fh_Ams_hit_Pos[i]->GetXaxis()->SetTitleOffset(1.1);
        cHit[i]->cd(2);
        fh_Ams_hit_Pos[i]->Draw("col");

        sprintf(Name1, "fh_Ams_hit_Energies_%d", i + 1);
        sprintf(Name2, "Energy_S vs Energy_K for AMS Det %d", i + 1);
        fh_Ams_hit_E[i] = new TH2F(Name1, Name2, 2000, 0, 1000., 2000, 0, 1000.);
        fh_Ams_hit_E[i]->GetXaxis()->SetTitle("Energy_S [ADC units]");
        fh_Ams_hit_E[i]->GetYaxis()->SetTitle("Energy_K [ADC units]");
        fh_Ams_hit_E[i]->GetXaxis()->CenterTitle(true);
        fh_Ams_hit_E[i]->GetYaxis()->CenterTitle(true);
        fh_Ams_hit_E[i]->GetYaxis()->SetTitleOffset(1.2);
        fh_Ams_hit_E[i]->GetXaxis()->SetTitleOffset(1.1);
        cHit[i]->cd(3);
        fh_Ams_hit_E[i]->Draw("col");

        sprintf(Name1, "fh_Ams_hit_Energy_vs_theta_%d", i + 1);
        sprintf(Name2, "Energy_S vs Theta for AMS Det %d", i + 1);
        fh_Ams_hit_E_theta[i] = new TH2F(Name1, Name2, 300, 0., 90., 500, 0., 1000.);
        fh_Ams_hit_E_theta[i]->GetXaxis()->SetTitle("Theta [degrees]");
        fh_Ams_hit_E_theta[i]->GetYaxis()->SetTitle("Energy_S [ADC units]");
        fh_Ams_hit_E_theta[i]->GetXaxis()->CenterTitle(true);
        fh_Ams_hit_E_theta[i]->GetYaxis()->CenterTitle(true);
        fh_Ams_hit_E_theta[i]->GetYaxis()->SetTitleOffset(1.1);
        fh_Ams_hit_E_theta[i]->GetXaxis()->SetTitleOffset(1.1);
        cHit[i]->cd(4);
        fh_Ams_hit_E_theta[i]->Draw("col");
    }

    for (Int_t i = 0; i < 2; i++) // two layers: inner (1) and outer (2)
    {
        if (i == 0)
        {
            sprintf(Name1, "fh2_Ams_theta_vs_phi_layer_%d", i + 1);
            sprintf(Name2, "AMS: theta vs phi for layer %d (inner)", i + 1);
        }
        else
        {
            sprintf(Name1, "fh2_Ams_theta_vs_phi_layer_%d", i + 1);
            sprintf(Name2, "AMS: theta vs phi for layer %d (outer)", i + 1);
        }
        fh2_ams_theta_phi[i] = new TH2F(Name1, Name2, 90, 0., 90., 180, -180., 180.);
        fh2_ams_theta_phi[i]->GetXaxis()->SetTitle("Theta [degrees]");
        fh2_ams_theta_phi[i]->GetYaxis()->SetTitle("Phi [degrees]");
        fh2_ams_theta_phi[i]->GetXaxis()->SetTitleOffset(1.1);
        fh2_ams_theta_phi[i]->GetYaxis()->SetTitleOffset(1.1);
        fh2_ams_theta_phi[i]->GetXaxis()->CenterTitle(true);
        fh2_ams_theta_phi[i]->GetYaxis()->CenterTitle(true);
        cHitAngles->cd(i + 1);
        fh2_ams_theta_phi[i]->Draw("COLZ");
    }

    for (Int_t i = 0; i < 2; i++) // two sides: right (1) and left (2)
    {
        // Energy correlations per side
        if (i == 0)
        {
            sprintf(Name1, "fh2_Ams_Einner_vs_Eouter_right");
            sprintf(Name2, "AMS: E-inner vs E-outer for right side");
        }
        else
        {
            sprintf(Name1, "fh2_Ams_Einner_vs_Eouter_left");
            sprintf(Name2, "AMS: E-inner vs E-outer for left side");
        }
        fh2_ams_e1_e2[i] = new TH2F(Name1, Name2, 500, 0., 1000., 500, 0., 1000.);
        fh2_ams_e1_e2[i]->GetXaxis()->SetTitle("E-inner [ADC units]");
        fh2_ams_e1_e2[i]->GetYaxis()->SetTitle("E-outer [ADC units]");
        fh2_ams_e1_e2[i]->GetXaxis()->SetTitleOffset(1.1);
        fh2_ams_e1_e2[i]->GetYaxis()->SetTitleOffset(1.4);
        fh2_ams_e1_e2[i]->GetXaxis()->CenterTitle(true);
        fh2_ams_e1_e2[i]->GetYaxis()->CenterTitle(true);
        cHitEnergyCor->cd(i + 1);
        fh2_ams_e1_e2[i]->Draw("COLZ");

        // Theta correlations per side
        if (i == 0)
        {
            sprintf(Name1, "fh2_Ams_#thetainner_vs_#thetaouter_right");
            sprintf(Name2, "AMS: #theta-inner vs #theta-outer for right side");
        }
        else
        {
            sprintf(Name1, "fh2_Ams_#thetainner_vs_#thetaouter_left");
            sprintf(Name2, "AMS: #theta-inner vs #theta-outer for left side");
        }
        fh2_ams_theta1_theta2[i] = new TH2F(Name1, Name2, 90, 0., 90., 90, 0., 90.);
        fh2_ams_theta1_theta2[i]->GetXaxis()->SetTitle("#theta-inner [degrees]");
        fh2_ams_theta1_theta2[i]->GetYaxis()->SetTitle("#theta-outer [degrees]");
        fh2_ams_theta1_theta2[i]->GetXaxis()->SetTitleOffset(1.);
        fh2_ams_theta1_theta2[i]->GetYaxis()->SetTitleOffset(1.5);
        fh2_ams_theta1_theta2[i]->GetXaxis()->CenterTitle(true);
        fh2_ams_theta1_theta2[i]->GetYaxis()->CenterTitle(true);
        cHitThetaCor->cd(i + 1);
        fh2_ams_theta1_theta2[i]->Draw("COLZ");

        // Phi correlations per side
        if (i == 0)
        {
            sprintf(Name1, "fh2_Ams_#phiinner_vs_#phiouter_right");
            sprintf(Name2, "AMS: #phi-inner vs #phi-outer for right side");
            fh2_ams_phi1_phi2[i] = new TH2F(Name1, Name2, 180, 90., 270., 180, 90., 270.);
        }
        else
        {
            sprintf(Name1, "fh2_Ams_#phiinner_vs_#phiouter_left");
            sprintf(Name2, "AMS: #phi-inner vs #phi-outer for left side");
            fh2_ams_phi1_phi2[i] = new TH2F(Name1, Name2, 180, -180., 180., 180, -180., 180.);
        }
        fh2_ams_phi1_phi2[i]->GetXaxis()->SetTitle("#phi-inner [degrees]");
        fh2_ams_phi1_phi2[i]->GetYaxis()->SetTitle("#phi-outer [degrees]");
        fh2_ams_phi1_phi2[i]->GetXaxis()->SetTitleOffset(1.);
        fh2_ams_phi1_phi2[i]->GetYaxis()->SetTitleOffset(1.5);
        fh2_ams_phi1_phi2[i]->GetXaxis()->CenterTitle(true);
        fh2_ams_phi1_phi2[i]->GetYaxis()->CenterTitle(true);
        cHitPhiCor->cd(i + 1);
        fh2_ams_phi1_phi2[i]->Draw("COLZ");
    }

    // CALIFA
    Double_t bins = fHitCalifaHist_bins;
    Double_t maxE = fHitCalifaHist_max;
    Double_t minE = 0.;

    // CANVAS Multiplicity
    cCalifaMult = new TCanvas("Califa_correlated_multiplicity", "Califa_Multiplicity", 10, 10, 500, 500);
    fh1_Califa_MultHit = new TH1F("fh1sof_Califa_MultHit", "Califa multiplicity", 21, -0.5, 20.5);
    fh1_Califa_MultHit->GetXaxis()->SetTitle("Multiplicity");
    fh1_Califa_MultHit->GetXaxis()->CenterTitle(true);
    fh1_Califa_MultHit->GetYaxis()->CenterTitle(true);
    fh1_Califa_MultHit->GetYaxis()->SetTitleOffset(1.2);
    fh1_Califa_MultHit->GetXaxis()->SetTitleOffset(1.1);
    fh1_Califa_MultHit->SetLineColor(kRed);
    fh1_Califa_MultHit->Draw();

    // CANVAS Energy correlations between hits
    cCalifaCoinE = new TCanvas("Califa_Energy_correlation_hits", "Energy correlations, hit level", 10, 10, 500, 500);

    fh2_Califa_coinE =
        new TH2F("fh2sof_Califa_energy_correlations", "Califa energy correlations", bins, minE, maxE, bins, minE, maxE);
    fh2_Califa_coinE->GetXaxis()->SetTitle("Energy (keV)");
    fh2_Califa_coinE->GetYaxis()->SetTitle("Energy (keV)");
    fh2_Califa_coinE->GetYaxis()->SetTitleOffset(1.2);
    fh2_Califa_coinE->GetXaxis()->CenterTitle(true);
    fh2_Califa_coinE->GetYaxis()->CenterTitle(true);
    fh2_Califa_coinE->Draw("COLZ");

    // CANVAS Theta correlations between hits
    cCalifaCoinTheta = new TCanvas("Califa_Theta_correlation_hits", "Theta correlations, hit level", 10, 10, 500, 500);

    fh2_Califa_coinTheta =
        new TH2F("fh2sof_Califa_theta_correlations", "Califa theta correlations", 50, 0, 100, 50, 0, 100);
    fh2_Califa_coinTheta->GetXaxis()->SetTitle("Theta [degrees]");
    fh2_Califa_coinTheta->GetYaxis()->SetTitle("Theta [degrees]");
    fh2_Califa_coinTheta->GetYaxis()->SetTitleOffset(1.2);
    fh2_Califa_coinTheta->GetXaxis()->CenterTitle(true);
    fh2_Califa_coinTheta->GetYaxis()->CenterTitle(true);
    fh2_Califa_coinTheta->Draw("COLZ");

    // CANVAS Phi correlations between hits
    cCalifaCoinPhi = new TCanvas("Califa_Phi_correlation_hits", "Phi correlations, hit level", 10, 10, 500, 500);

    fh2_Califa_coinPhi =
        new TH2F("fh2sof_Califa_phi_correlations", "Califa phi correlations", 90, -180, 180, 90, -180, 180);
    fh2_Califa_coinPhi->GetXaxis()->SetTitle("Phi [degrees]");
    fh2_Califa_coinPhi->GetYaxis()->SetTitle("Phi [degrees]");
    fh2_Califa_coinPhi->GetYaxis()->SetTitleOffset(1.2);
    fh2_Califa_coinPhi->GetXaxis()->CenterTitle(true);
    fh2_Califa_coinPhi->GetYaxis()->CenterTitle(true);
    fh2_Califa_coinPhi->Draw("COLZ");

    // CANVAS Theta vs Phi
    cCalifa_angles = new TCanvas("Califa_theta_vs_phi", "Theta vs Phi", 10, 10, 500, 500);
    fh2_Califa_theta_phi = new TH2F("fh2sof_Califa_theta_vs_phi", "Califa theta vs phi", 50, 0, 90, 180, -180, 180);
    fh2_Califa_theta_phi->GetXaxis()->SetTitle("Theta [degrees]");
    fh2_Califa_theta_phi->GetYaxis()->SetTitle("Phi [degrees]");
    fh2_Califa_theta_phi->GetYaxis()->SetTitleOffset(1.2);
    fh2_Califa_theta_phi->GetXaxis()->CenterTitle(true);
    fh2_Califa_theta_phi->GetYaxis()->CenterTitle(true);
    fh2_Califa_theta_phi->Draw("COLZ");

    // CANVAS Theta vs energy
    sprintf(Name1, "Califa_calorimeter_Theta_vs_Energy");
    sprintf(Name2, "fh2sof_Califa_theta_vs_total_energy");
    sprintf(Name3, "Califa theta vs energy for full calorimeter");
    cCalifa_theta_energy = new TCanvas(Name1, Name1, 10, 10, 500, 500);
    fh2_Califa_theta_energy = new TH2F(Name2, Name3, 360, 0, 90, bins, minE, maxE);
    fh2_Califa_theta_energy->GetXaxis()->SetTitle("Theta [degrees]");
    fh2_Califa_theta_energy->GetYaxis()->SetTitle("Energy [keV]");
    fh2_Califa_theta_energy->GetYaxis()->SetTitleOffset(1.4);
    fh2_Califa_theta_energy->GetXaxis()->CenterTitle(true);
    fh2_Califa_theta_energy->GetYaxis()->CenterTitle(true);
    fh2_Califa_theta_energy->Draw("COLZ");

    // CANVAS Total energy
    sprintf(Name1, "Califa_calorimeter_total_Energy_per_hit");
    sprintf(Name2, "fh1sof_Califa_total_energy");
    sprintf(Name3, "Califa total energy per hit for the full calorimeter");
    cCalifa_hitenergy = new TCanvas(Name1, Name1, 10, 10, 500, 500);
    fh1_Califa_total_energy = new TH1F(Name2, Name3, bins, minE, maxE);
    fh1_Califa_total_energy->GetXaxis()->SetTitle("Energy [keV]");
    fh1_Califa_total_energy->GetYaxis()->SetTitle("Counts");
    fh1_Califa_total_energy->GetYaxis()->SetTitleOffset(1.4);
    fh1_Califa_total_energy->GetXaxis()->CenterTitle(true);
    fh1_Califa_total_energy->GetYaxis()->CenterTitle(true);
    fh1_Califa_total_energy->SetFillColor(29);
    fh1_Califa_total_energy->SetLineColor(1);
    fh1_Califa_total_energy->SetLineWidth(2);
    fh1_Califa_total_energy->Draw("");
    gPad->SetLogy();

    // CANVAS opening angle
    sprintf(Name1, "Califa_Opening_angle_hit");
    sprintf(Name2, "fh1_Califa_Opening");
    sprintf(Name3, "Califa Opening angle (cond. Z-1)");
    cCalifa_opening = new TCanvas(Name1, Name1, 10, 10, 500, 500);
    fh1_openangle = new TH1F(Name2, Name3, 160, 10, 170);
    fh1_openangle->GetXaxis()->SetTitle("Opening angle [degrees]");
    fh1_openangle->GetYaxis()->SetTitle("Counts");
    fh1_openangle->GetXaxis()->CenterTitle(true);
    fh1_openangle->GetYaxis()->CenterTitle(true);
    fh1_openangle->GetYaxis()->SetTitleOffset(1.2);
    fh1_openangle->GetXaxis()->SetTitleOffset(1.2);
    fh1_openangle->SetFillColor(8);
    fh1_openangle->SetLineColor(1);
    fh1_openangle->SetLineWidth(2);
    fh1_openangle->Draw("");

    // MAIN FOLDER-AMS-CALIFA-MUSICs
    TFolder* mainfol = new TFolder("CALIFA-AMS", "AMS-CALIFA-MUSICs correlation info");

    // Folder for hit data
    if (1) //((fHitItemsMus||fHitItemsTrim) && fHitItemsTwim)
    {
        TFolder* hitfol = new TFolder("Hit", "Hit AMS-CALIFA-MUSICs correlation info");
        for (Int_t i = 0; i < fNbDet; i++)
            hitfol->Add(cHit[i]);
        if (fHitItemsAms)
            mainfol->Add(hitfol);

        TFolder* CorSidefol =
            new TFolder("Correlations_per_side", "Hit AMS-CALIFA-MUSICs info for correlations per side");

        if (fHitItemsAms)
        {
            CorSidefol->Add(cHitAngles);
            CorSidefol->Add(cHitThetaCor);
            CorSidefol->Add(cHitPhiCor);
            CorSidefol->Add(cHitEnergyCor);
        }
        if (fHitItemsCalifa)
        {
            CorSidefol->Add(cCalifaMult);
            CorSidefol->Add(cCalifaCoinE);
            CorSidefol->Add(cCalifaCoinTheta);
            CorSidefol->Add(cCalifaCoinPhi);
            CorSidefol->Add(cCalifa_angles);
            CorSidefol->Add(cCalifa_opening);
            CorSidefol->Add(cCalifa_theta_energy);
            CorSidefol->Add(cCalifa_hitenergy);
        }
        mainfol->Add(CorSidefol);
    }
    run->AddObject(mainfol);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_AMS_CALIFA_MUSICs", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BAmsCorrelationOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BAmsCorrelationOnlineSpectra::Reset_Histo";

    // Hit AMS data
    if (fHitItemsAms)
    {
        for (Int_t i = 0; i < fNbDet; i++)
        {
            fh_Ams_hit_Mul[i]->Reset();
            fh_Ams_hit_Pos[i]->Reset();
            fh_Ams_hit_E[i]->Reset();
            fh_Ams_hit_E_theta[i]->Reset();
        }

        for (Int_t i = 0; i < 2; i++)
        {
            fh2_ams_theta_phi[i]->Reset();
            fh2_ams_theta1_theta2[i]->Reset();
            fh2_ams_phi1_phi2[i]->Reset();
            fh2_ams_e1_e2[i]->Reset();
        }
    }
    // Hit CALIFA data
    if (fHitItemsCalifa)
    {
        fh1_Califa_MultHit->Reset();
        fh2_Califa_coinE->Reset();
        fh2_Califa_coinTheta->Reset();
        fh2_Califa_coinPhi->Reset();
        fh2_Califa_theta_phi->Reset();
        fh2_Califa_theta_energy->Reset();
        fh1_Califa_total_energy->Reset();
        fh1_openangle->Reset();
    }
}

void R3BAmsCorrelationOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BAmsCorrelationOnlineSpectra::Exec FairRootManager not found";

    // Fill histogram with trigger information

    Int_t tpatbin;
    Int_t tpat = 0;
    if (fEventHeader->GetTpat() > 0)
    {
        for (Int_t i = 0; i < 16; i++)
        {
            tpatbin = (fEventHeader->GetTpat() & (1 << i));
            if (tpatbin != 0)
                tpat = i + 1;
            //      fh1_trigger->Fill(i + 1);
        }
    }
    else if (fEventHeader->GetTpat() == 0)
    {
        // fh1_trigger->Fill(0);
    }
    else
    {
        LOG(INFO) << fNEvents << " " << fEventHeader->GetTpat();
    }

    // Fill Califa-hit data
    /*   if (fHitItemsCalifa && fHitItemsCalifa->GetEntriesFast() > 0)
       {
           Int_t nHits = fHitItemsCalifa->GetEntriesFast();

           Double_t theta = 0., phi = 0., ene = 0.;

           for (Int_t ihit = 0; ihit < nHits; ihit++)
           {
               R3BCalifaHitData* hit = (R3BCalifaHitData*)fHitItemsCalifa->At(ihit);
               if (!hit)
                   continue;
               theta = hit->GetTheta() / TMath::Pi() * 180.;
               phi = hit->GetPhi() / TMath::Pi() * 180.;
               ene = hit->GetEnergy();
           }
       }
   */
    Double_t z_music = 0., z_twim = 0.;

    // Fill music-hit data
    if (fHitItemsMus && !fHitItemsTrim && fHitItemsMus->GetEntriesFast() > 0)
    {
        Int_t nHits = fHitItemsMus->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BMusicHitData* hit = (R3BMusicHitData*)fHitItemsMus->At(ihit);
            if (!hit)
                continue;
            z_music = hit->GetZcharge();
        }
    }

    // Fill triple music-hit data
    if (!fHitItemsMus && fHitItemsTrim && fHitItemsTrim->GetEntriesFast() > 0)
    {
        Int_t nHits = fHitItemsTrim->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTrimHitData* hit = (R3BSofTrimHitData*)fHitItemsTrim->At(ihit);
            if (!hit)
                continue;
            z_music = hit->GetZcharge();
        }
    }

    // Fill Twim-hit data
    if (fHitItemsTwim && fHitItemsTwim->GetEntriesFast() > 0)
    {
        Int_t nHits = fHitItemsTwim->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BTwimHitData* hit = (R3BTwimHitData*)fHitItemsTwim->At(ihit);
            if (!hit)
                continue;
            z_twim += hit->GetZcharge();
        }
    }

    // Conditions to fill AMS histograms
    // if ((fZproj + 0.5) > z_music && (fZproj - 0.5) < z_music && z_twim > (fZproj - 1.5) && z_twim < (fZproj - 0.5))

    // if(tpat>0)std::cout<<tpat <<std::endl;

    if( tpat==4 && z_twim>90. && z_twim<92.) 
    {
        // Fill Califa-hit data
        if (fHitItemsCalifa && fHitItemsCalifa->GetEntriesFast() > 0)
        {
            Int_t nHits = fHitItemsCalifa->GetEntriesFast();
            fh1_Califa_MultHit->Fill(nHits);

            Double_t theta = 0., phi = 0.;
            Double_t califa_theta[nHits];
            Double_t califa_phi[nHits];
            Double_t califa_e[nHits];
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BCalifaHitData* hit = (R3BCalifaHitData*)fHitItemsCalifa->At(ihit);
                if (!hit)
                    continue;
                theta = hit->GetTheta() * TMath::RadToDeg();
                phi = hit->GetPhi() * TMath::RadToDeg();
                califa_theta[ihit] = theta;
                califa_phi[ihit] = phi;
                califa_e[ihit] = hit->GetEnergy();
                fh2_Califa_theta_phi->Fill(theta, phi);
                fh2_Califa_theta_energy->Fill(theta + gRandom->Uniform(-1.5, 1.5), hit->GetEnergy());
                fh1_Califa_total_energy->Fill(hit->GetEnergy());
            }

            TVector3 master[2];
            Double_t maxEL = 0., maxER = 0.;
            for (Int_t i1 = 0; i1 < nHits; i1++)
            {

                if (califa_e[i1] > maxER && TMath::Abs(califa_phi[i1]) > 150.) // wixhausen
                {
                    master[0].SetMagThetaPhi(
                        1., califa_theta[i1] * TMath::DegToRad(), califa_phi[i1] * TMath::DegToRad());
                    maxER = califa_e[i1];
                }
                if (califa_e[i1] > maxEL && TMath::Abs(califa_phi[i1]) < 60.)
                { // messel
                    master[1].SetMagThetaPhi(
                        1., califa_theta[i1] * TMath::DegToRad(), califa_phi[i1] * TMath::DegToRad());
                    maxEL = califa_e[i1];
                }
            }
            if (maxEL > fMinProtonE && maxER > fMinProtonE)
            {
                fh1_openangle->Fill(master[0].Angle(master[1]) * TMath::RadToDeg());
            }

            // Comparison of hits to get energy, theta and phi correlations between them
            for (Int_t i1 = 0; i1 < nHits; i1++)
                for (Int_t i2 = i1 + 1; i2 < nHits; i2++)
                    if (gRandom->Uniform(0., 1.) < 0.5)
                    {
                        fh2_Califa_coinE->Fill(califa_e[i1], califa_e[i2]);
                        fh2_Califa_coinTheta->Fill(califa_theta[i1], califa_theta[i2]);
                        fh2_Califa_coinPhi->Fill(califa_phi[i1], califa_phi[i2]);
                    }
                    else
                    {
                        fh2_Califa_coinE->Fill(califa_e[i2], califa_e[i1]);
                        fh2_Califa_coinTheta->Fill(califa_theta[i2], califa_theta[i1]);
                        fh2_Califa_coinPhi->Fill(califa_phi[i2], califa_phi[i1]);
                    }
            //}

            // Fill AMS-hit data
            if (fHitItemsAms && fHitItemsAms->GetEntriesFast() > 0)
            {
                Int_t mulhit[fNbDet];
                for (Int_t i = 0; i < fNbDet; i++)
                    mulhit[i] = 0;

                Float_t Emaxhit[fNbDet]; // just look for the max. energy per detector
                Float_t Thetamaxhit[fNbDet];
                Float_t Phimaxhit[fNbDet];
                for (Int_t i = 0; i < fNbDet; i++)
                {
                    Emaxhit[i] = 0.;
                    Thetamaxhit[i] = 0.; // at 0 degrees we have nothing!
                    Phimaxhit[i] = 90.;  // at 90 degrees we have nothing!
                }

                nHits = fHitItemsAms->GetEntriesFast();
                Int_t DetId = -1;
                // std::cout << nHits << std::endl;
                for (Int_t ihit = 0; ihit < nHits; ihit++)
                {
                    R3BAmsHitData* hit = (R3BAmsHitData*)fHitItemsAms->At(ihit);
                    if (!hit)
                        continue;
                    DetId = hit->GetDetId();
                    fh_Ams_hit_Pos[DetId]->Fill(hit->GetPos_S(), hit->GetPos_K());
                    fh_Ams_hit_E[DetId]->Fill(hit->GetEnergyS(), hit->GetEnergyK());
                    fh_Ams_hit_E_theta[DetId]->Fill(hit->GetTheta() * TMath::RadToDeg(), hit->GetEnergyS());
                    mulhit[DetId]++;
                    if (DetId == 0 || DetId == 3) // inner detectors, layer 1
                        fh2_ams_theta_phi[0]->Fill(hit->GetTheta() * TMath::RadToDeg(),
                                                   hit->GetPhi() * TMath::RadToDeg());
                    else // outer detectors, layer 2
                        fh2_ams_theta_phi[1]->Fill(hit->GetTheta() * TMath::RadToDeg(),
                                                   hit->GetPhi() * TMath::RadToDeg());
                    // look for the max. energy per AMS detector
                    if (Emaxhit[DetId] < hit->GetEnergyS())
                    {
                        Emaxhit[DetId] = hit->GetEnergyS();
                        Thetamaxhit[DetId] = hit->GetTheta() * TMath::RadToDeg();
                        if (DetId > 2)
                            Phimaxhit[DetId] = hit->GetPhi() * TMath::RadToDeg();
                        else
                        {
                            if (hit->GetPhi() < 0.)
                                Phimaxhit[DetId] = (TMath::Pi() + hit->GetPhi()) * TMath::RadToDeg() + 180.;
                            else
                                Phimaxhit[DetId] = hit->GetPhi() * TMath::RadToDeg();
                        }
                    }
                }
                for (Int_t i = 0; i < fNbDet; i++)
                    fh_Ams_hit_Mul[i]->Fill(mulhit[i]);
                if (Emaxhit[0] > 0. && (Emaxhit[1] > 0. || Emaxhit[2] > 0.))
                {
                    fh2_ams_e1_e2[0]->Fill(Emaxhit[0], TMath::Max(Emaxhit[1], Emaxhit[2]));
                    if (Emaxhit[1] > Emaxhit[2])
                    {
                        fh2_ams_theta1_theta2[0]->Fill(Thetamaxhit[0], Thetamaxhit[1]);
                        fh2_ams_phi1_phi2[0]->Fill(Phimaxhit[0], Phimaxhit[1]);
                    }
                    else if (Emaxhit[2] > Emaxhit[1])
                    {
                        fh2_ams_theta1_theta2[0]->Fill(Thetamaxhit[0], Thetamaxhit[2]);
                        fh2_ams_phi1_phi2[0]->Fill(Phimaxhit[0], Phimaxhit[2]);
                    }
                }
                if (Emaxhit[3] > 0. && (Emaxhit[4] > 0. || Emaxhit[5] > 0.))
                {
                    fh2_ams_e1_e2[1]->Fill(Emaxhit[3], TMath::Max(Emaxhit[4], Emaxhit[5]));
                    if (Emaxhit[4] > Emaxhit[5])
                    {
                        fh2_ams_theta1_theta2[1]->Fill(Thetamaxhit[3], Thetamaxhit[4]);
                        fh2_ams_phi1_phi2[1]->Fill(Phimaxhit[3], Phimaxhit[4]);
                    }
                    else if (Emaxhit[5] > Emaxhit[4])
                    {
                        fh2_ams_theta1_theta2[1]->Fill(Thetamaxhit[3], Thetamaxhit[5]);
                        fh2_ams_phi1_phi2[1]->Fill(Phimaxhit[3], Phimaxhit[5]);
                    }
                }
            }
        }
    } // z from music and z-1 from twim

    fNEvents += 1;
}

void R3BAmsCorrelationOnlineSpectra::FinishEvent()
{
    if (fHitItemsAms)
    {
        fHitItemsAms->Clear();
    }
    if (fHitItemsTwim)
    {
        fHitItemsTwim->Clear();
    }
    if (fHitItemsMus)
    {
        fHitItemsMus->Clear();
    }
    if (fHitItemsTrim)
    {
        fHitItemsTrim->Clear();
    }
    if (fHitItemsCalifa)
    {
        fHitItemsCalifa->Clear();
    }
}

void R3BAmsCorrelationOnlineSpectra::FinishTask()
{
    if (fHitItemsAms)
    {
        for (Int_t i = 0; i < fNbDet; i++)
            cHit[i]->Write();
        cHitAngles->Write();
        cHitThetaCor->Write();
        cHitPhiCor->Write();
        cHitEnergyCor->Write();
    }
    // Write canvas for Hit data
    if (fHitItemsCalifa)
    {
        cCalifaMult->Write();
        cCalifaCoinE->Write();
        cCalifaCoinTheta->Write();
        cCalifaCoinPhi->Write();
        cCalifa_angles->Write();
        cCalifa_theta_energy->Write();
        cCalifa_hitenergy->Write();
        cCalifa_opening->Write();
    }
}

ClassImp(R3BAmsCorrelationOnlineSpectra)
