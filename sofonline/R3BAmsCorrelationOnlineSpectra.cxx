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
#include "R3BEventHeader.h"
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
    , fHitItemsAms(NULL)
    , fTrigger(-1)
    , fNEvents(0)
    , fNbDet(6)
{
}

R3BAmsCorrelationOnlineSpectra::R3BAmsCorrelationOnlineSpectra(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fHitItemsAms(NULL)
    , fTrigger(-1)
    , fNEvents(0)
    , fNbDet(6)
{
}

R3BAmsCorrelationOnlineSpectra::~R3BAmsCorrelationOnlineSpectra()
{
    LOG(INFO) << "R3BAmsCorrelationOnlineSpectra: Delete instance";
    if (fHitItemsAms)
        delete fHitItemsAms;
}

InitStatus R3BAmsCorrelationOnlineSpectra::Init()
{
    LOG(INFO) << "R3BAmsCorrelationOnlineSpectra::Init ";

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(fatal) << "R3BAmsCorrelationOnlineSpectra::Init FairRootManager not found";

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // get access to Hit data
    fHitItemsAms = (TClonesArray*)mgr->GetObject("AmsHitData");
    if (!fHitItemsAms)
    {
        return kFATAL;
    }

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

    // Hit data
    for (Int_t i = 0; i < fNbDet; i++)
    { // one histo per detector
        sprintf(Name1, "fh_Ams_hit_Mul_%d", i + 1);
        sprintf(Name2, "Multiplicity Det %d", i + 1);
        fh_Ams_hit_Mul[i] = new TH1F(Name1, Name2, 10, -0.5, 9.5);
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

    // MAIN FOLDER-AMS-CALIFA-MUSICs
    TFolder* mainfol = new TFolder("AMS-CALIFA-MUSICs", "AMS-CALIFA-MUSICs correlation info");

    // Folder for hit data
    if (fHitItemsAms)
    {
        TFolder* hitfol = new TFolder("Hit", "Hit AMS correlation info");
        for (Int_t i = 0; i < fNbDet; i++)
            hitfol->Add(cHit[i]);
        mainfol->Add(hitfol);

        TFolder* CorSidefol = new TFolder("Correlations_per_side", "Hit AMS info for correlations per side");
        CorSidefol->Add(cHitAngles);
        CorSidefol->Add(cHitThetaCor);
        CorSidefol->Add(cHitPhiCor);
        CorSidefol->Add(cHitEnergyCor);
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

    // Hit data
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
}

void R3BAmsCorrelationOnlineSpectra::Exec(Option_t* option)
{

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BAmsCorrelationOnlineSpectra::Exec FairRootManager not found";

    // Fill hit data
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

        Int_t nHits = fHitItemsAms->GetEntriesFast();
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
                fh2_ams_theta_phi[0]->Fill(hit->GetTheta() * TMath::RadToDeg(), hit->GetPhi() * TMath::RadToDeg());
            else // outer detectors, layer 2
                fh2_ams_theta_phi[1]->Fill(hit->GetTheta() * TMath::RadToDeg(), hit->GetPhi() * TMath::RadToDeg());
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

    fNEvents += 1;
}

void R3BAmsCorrelationOnlineSpectra::FinishEvent()
{
    if (fHitItemsAms)
    {
        fHitItemsAms->Clear();
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
}

ClassImp(R3BAmsCorrelationOnlineSpectra)
