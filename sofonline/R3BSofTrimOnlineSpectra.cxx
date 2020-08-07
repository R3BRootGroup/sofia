/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofTrimOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BSofTrimCalData.h"
#include "R3BSofTrimMappedData.h"
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

using namespace std;

R3BSofTrimOnlineSpectra::R3BSofTrimOnlineSpectra()
    : FairTask("SofTrimOnlineSpectra", 1)
    , fMappedItemsTrim(NULL)
    , fCalItemsTrim(NULL)
    , fNEvents(0)
    , fNumSections(3)
    , fNumAnodes(6)
    , fNumTref(1)
    , fNumTtrig(1)
{
    fNumPairs = fNumAnodes / 2;
}

R3BSofTrimOnlineSpectra::R3BSofTrimOnlineSpectra(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fMappedItemsTrim(NULL)
    , fCalItemsTrim(NULL)
    , fNEvents(0)
    , fNumSections(3)
    , fNumAnodes(6)
    , fNumTref(1)
    , fNumTtrig(1)
{
    fNumPairs = fNumAnodes / 2;
}

R3BSofTrimOnlineSpectra::~R3BSofTrimOnlineSpectra()
{
    LOG(INFO) << "R3BSofTrimOnlineSpectra::Delete instance";
    if (fMappedItemsTrim)
        delete fMappedItemsTrim;
    if (fCalItemsTrim)
        delete fCalItemsTrim;
}

InitStatus R3BSofTrimOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofTrimOnlineSpectra::Init() fNumSections = " << fNumSections;

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTrimOnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // === get access to mapped data of the Triple-MUSIC
    fMappedItemsTrim = (TClonesArray*)mgr->GetObject("TrimMappedData");
    if (!fMappedItemsTrim)
    {
        LOG(FATAL) << " R3BSofTrimOnlineSpectra::Init(), TrimMappedData not found";
        return kFATAL;
    }

    // === get access to cal data of the Triple-MUSIC === //
    fCalItemsTrim = (TClonesArray*)mgr->GetObject("TrimCalData");
    if (!fCalItemsTrim)
    {
        LOG(FATAL) << " R3BSofTrimOnlineSpectra::Init(), TrimCalData not found";
        return kFATAL;
    }

    // === Create histograms for detectors
    char Name1[255];
    char Name2[255];

    // === =========== === //
    // === MAPPED DATA === //
    // === =========== === //

    // --- TRIM: 1-D MAPPED data for ENERGY, DRIFT-TIME and MULTIPLICITY
    cTrimMap_E = new TCanvas*[fNumSections];
    fh1_trimmap_E = new TH1F*[fNumSections * fNumAnodes];
    cTrimMap_DT = new TCanvas*[fNumSections];
    fh1_trimmap_DT = new TH1F*[fNumSections * fNumAnodes];
    cTrimMap_Mult = new TCanvas*[fNumSections];
    fh1_trimmap_Mult = new TH1F*[fNumSections];

    for (Int_t i = 0; i < fNumSections; i++)
    {
        sprintf(Name1, "Trim_Emap_Sec_%d", i + 1);
        sprintf(Name2, "Section %d", i + 1);
        cTrimMap_E[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);
        cTrimMap_E[i]->Divide(2, 3); // 2 col and 3 raws (if triangular anodes: one raw per pair)

        sprintf(Name1, "Trim_DTmap_Sec_%d", i + 1);
        sprintf(Name2, "Section %d", i + 1);
        cTrimMap_DT[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);
        cTrimMap_DT[i]->Divide(2, 3);

        for (Int_t j = 0; j < fNumAnodes; j++)
        {
            sprintf(Name1, "fh1_trim_Emap_sec%d_a%d", i + 1, j + 1);
            sprintf(Name2, "Sec %d:Anode %d", i + 1, j + 1);
            fh1_trimmap_E[j + fNumAnodes * i] = new TH1F(Name1, Name2, 8192, 0, 8192);
            fh1_trimmap_E[j + fNumAnodes * i]->GetXaxis()->SetTitle("Raw Energy [channels], if mult==1");
            fh1_trimmap_E[j + fNumAnodes * i]->GetYaxis()->SetTitle("Counts");
            fh1_trimmap_E[j + fNumAnodes * i]->GetYaxis()->SetTitleOffset(1.1);
            fh1_trimmap_E[j + fNumAnodes * i]->GetXaxis()->CenterTitle(true);
            fh1_trimmap_E[j + fNumAnodes * i]->GetYaxis()->CenterTitle(true);
            fh1_trimmap_E[j + fNumAnodes * i]->GetXaxis()->SetLabelSize(0.045);
            fh1_trimmap_E[j + fNumAnodes * i]->GetXaxis()->SetTitleSize(0.045);
            fh1_trimmap_E[j + fNumAnodes * i]->GetYaxis()->SetLabelSize(0.045);
            fh1_trimmap_E[j + fNumAnodes * i]->GetYaxis()->SetTitleSize(0.045);
            fh1_trimmap_E[j + fNumAnodes * i]->SetFillColor(31);
            cTrimMap_E[i]->cd(j + 1);
            fh1_trimmap_E[j + fNumAnodes * i]->Draw("");

            sprintf(Name1, "fh1_trim_DTmap_sec%d_a%d", i + 1, j + 1);
            sprintf(Name2, "Sec %d:Anode %d", i + 1, j + 1);
            fh1_trimmap_DT[j + fNumAnodes * i] = new TH1F(Name1, Name2, 40000, 0, 40000);
            fh1_trimmap_DT[j + fNumAnodes * i]->GetXaxis()->SetTitle(
                "Drift time [channels, 100ps TDC resolution], if mult==1");
            fh1_trimmap_DT[j + fNumAnodes * i]->GetYaxis()->SetTitle("Counts");
            fh1_trimmap_DT[j + fNumAnodes * i]->GetYaxis()->SetTitleOffset(1.1);
            fh1_trimmap_DT[j + fNumAnodes * i]->GetXaxis()->CenterTitle(true);
            fh1_trimmap_DT[j + fNumAnodes * i]->GetYaxis()->CenterTitle(true);
            fh1_trimmap_DT[j + fNumAnodes * i]->GetXaxis()->SetLabelSize(0.045);
            fh1_trimmap_DT[j + fNumAnodes * i]->GetXaxis()->SetTitleSize(0.045);
            fh1_trimmap_DT[j + fNumAnodes * i]->GetYaxis()->SetLabelSize(0.045);
            fh1_trimmap_DT[j + fNumAnodes * i]->GetYaxis()->SetTitleSize(0.045);
            fh1_trimmap_DT[j + fNumAnodes * i]->SetFillColor(31);
            cTrimMap_DT[i]->cd(j + 1);
            fh1_trimmap_DT[j + fNumAnodes * i]->Draw("");
        }

        sprintf(Name1, "Trim_multiplicity_Sec_%d", i + 1);
        sprintf(Name2, "TRIM: Multiplicity per anode in section %d", i + 1);
        cTrimMap_Mult[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);

        sprintf(Name1, "fh1_trim_mult_sec_%d", i + 1);
        sprintf(Name2, "Trim: Multiplicity section %d", i + 1);
        fh1_trimmap_Mult[i] =
            new TH1F(Name1, Name2, fNumAnodes + fNumTref + fNumTtrig, 0.5, fNumAnodes + fNumTref + fNumTtrig + 0.5);
        fh1_trimmap_Mult[i]->GetXaxis()->SetTitle("Anode");
        fh1_trimmap_Mult[i]->GetYaxis()->SetTitle("Counts");
        fh1_trimmap_Mult[i]->GetYaxis()->SetTitleOffset(1.1);
        fh1_trimmap_Mult[i]->GetXaxis()->CenterTitle(true);
        fh1_trimmap_Mult[i]->GetYaxis()->CenterTitle(true);
        fh1_trimmap_Mult[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_trimmap_Mult[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_trimmap_Mult[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_trimmap_Mult[i]->GetYaxis()->SetTitleSize(0.045);
        fh1_trimmap_Mult[i]->SetFillColor(31);
        fh1_trimmap_Mult[i]->Draw("");
    } // end of for(i, fNumSections) 1D-MAPPED

    // --- TRIM: 1-D MAPPED data for Delta T = Tref - Ttrig for each section
    cTrimMap_DeltaTrefTtrig = new TCanvas("Trim_TrefTtrig", "Delta T (Tref-Trig)", 10, 10, 800, 700);
    cTrimMap_DeltaTrefTtrig->Divide(fNumSections, 1);
    fh1_trimmap_DeltaTrefTtrig = new TH1F*[fNumSections];

    for (Int_t i = 0; i < fNumSections; i++)
    {
        sprintf(Name1, "fh1_trim_Sec%d_DeltaTrefTtrig", i + 1);
        sprintf(Name2, "Delta T (Tref-Trig) in section %d [channels, 100ps TDC resolution] 1ns/bin", i + 1);
        fh1_trimmap_DeltaTrefTtrig[i] = new TH1F(Name1, Name2, 8000, -40000, 40000);
        cTrimMap_DeltaTrefTtrig->cd(i + 1);
        fh1_trimmap_DeltaTrefTtrig[i]->Draw("");
    }

    // --- TRIM: 2-D MAPPED data for ENERGY vs DRIFT-TIME and DRIFT-TIME vs DRIFT-TIME
    cTrimMap_EvsDT = new TCanvas*[fNumSections];
    fh2_trimmap_EvsDT = new TH2F*[fNumSections * fNumAnodes];
    cTrimMap_DTvsDT = new TCanvas*[fNumSections];
    fh2_trimmap_DTvsDT = new TH2F*[fNumSections * (fNumAnodes - 1)];
    for (Int_t i = 0; i < fNumSections; i++)
    {
        sprintf(Name1, "Trim_EvsDT_Sec_%d", i + 1);
        sprintf(Name2, "Section %d", i + 1);
        cTrimMap_EvsDT[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);

        cTrimMap_EvsDT[i]->Divide(2, 3);
        sprintf(Name1, "Trim_DTvsDT_Sec_%d", i + 1);
        sprintf(Name2, "DTvsDT Section %d", i + 1);
        cTrimMap_DTvsDT[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);
        cTrimMap_DTvsDT[i]->Divide(2, 3);

        for (Int_t j = 0; j < fNumAnodes; j++)
        {
            sprintf(Name1, "fh1_trim_EvsDT_sec%d_a%d", i + 1, j + 1);
            sprintf(Name2, "Sec %d:Anode %d", i + 1, j + 1);
            fh2_trimmap_EvsDT[j + fNumAnodes * i] = new TH2F(Name1, Name2, 512, 0, 8192, 800, 0, 40000);
            fh2_trimmap_EvsDT[j + fNumAnodes * i]->GetXaxis()->SetTitle(
                "Energy [channels, 8k ADC resolution], if mult==1");
            fh2_trimmap_EvsDT[j + fNumAnodes * i]->GetYaxis()->SetTitle(
                "Drift time [channels, 100ps TDC resolution], if mult==1");
            fh2_trimmap_EvsDT[j + fNumAnodes * i]->GetYaxis()->SetTitleOffset(1.1);
            fh2_trimmap_EvsDT[j + fNumAnodes * i]->GetXaxis()->CenterTitle(true);
            fh2_trimmap_EvsDT[j + fNumAnodes * i]->GetYaxis()->CenterTitle(true);
            fh2_trimmap_EvsDT[j + fNumAnodes * i]->GetXaxis()->SetLabelSize(0.045);
            fh2_trimmap_EvsDT[j + fNumAnodes * i]->GetXaxis()->SetTitleSize(0.045);
            fh2_trimmap_EvsDT[j + fNumAnodes * i]->GetYaxis()->SetLabelSize(0.045);
            fh2_trimmap_EvsDT[j + fNumAnodes * i]->GetYaxis()->SetTitleSize(0.045);
            cTrimMap_EvsDT[i]->cd(j + 1);
            fh2_trimmap_EvsDT[j + fNumAnodes * i]->Draw("col");
        }

        for (Int_t j = 0; j < fNumAnodes - 1; j++)
        {
            sprintf(Name1, "fh2_trimmap_DTvsDT_sec%d_a%d", i + 1, j);
            sprintf(Name2, "Sec %d: DT_%d vs DT_%d", i + 1, j + 1, j + 2);
            fh2_trimmap_DTvsDT[j + (fNumAnodes - 1) * i] = new TH2F(Name1, Name2, 800, 0, 40000, 800, 0, 40000);
            sprintf(Name1, "Drift time %d [channels, 100ps TDC resolution]", j + 1);
            sprintf(Name2, "Drift time %d [channels, 100ps TDC resolution]", j + 2);
            fh2_trimmap_DTvsDT[j + (fNumAnodes - 1) * i]->GetXaxis()->SetTitle(Name1);
            fh2_trimmap_DTvsDT[j + (fNumAnodes - 1) * i]->GetYaxis()->SetTitle(Name2);
            fh2_trimmap_DTvsDT[j + (fNumAnodes - 1) * i]->GetYaxis()->SetTitleOffset(1.1);
            fh2_trimmap_DTvsDT[j + (fNumAnodes - 1) * i]->GetXaxis()->CenterTitle(true);
            fh2_trimmap_DTvsDT[j + (fNumAnodes - 1) * i]->GetYaxis()->CenterTitle(true);
            fh2_trimmap_DTvsDT[j + (fNumAnodes - 1) * i]->GetXaxis()->SetLabelSize(0.045);
            fh2_trimmap_DTvsDT[j + (fNumAnodes - 1) * i]->GetXaxis()->SetTitleSize(0.045);
            fh2_trimmap_DTvsDT[j + (fNumAnodes - 1) * i]->GetYaxis()->SetLabelSize(0.045);
            fh2_trimmap_DTvsDT[j + (fNumAnodes - 1) * i]->GetYaxis()->SetTitleSize(0.045);
            cTrimMap_DTvsDT[i]->cd(j + 1);
            fh2_trimmap_DTvsDT[j + (fNumAnodes - 1) * i]->Draw("col");
        }
    } // end of for (i, fNumSections) 2-D MAPPED

    // === ======== === //
    // === CAL DATA === //
    // === ======== === //

    LOG(INFO) << "START THE INIT OF CAL HISTO";
    cTrimCal_Ene = new TCanvas*[fNumSections];
    cTrimCal_DT = new TCanvas*[fNumSections];

    fh1_trimcal_Esub = new TH1F*[fNumSections * fNumAnodes];
    fh1_trimcal_Ematch = new TH1F*[fNumSections * fNumAnodes];
    fh1_trimcal_DTraw = new TH1D*[fNumSections * fNumAnodes];
    fh1_trimcal_DTalign = new TH1D*[fNumSections * fNumAnodes];

    for (Int_t i = 0; i < fNumSections; i++)
    {
        LOG(INFO) << "SECTION = " << i + 1;
        sprintf(Name1, "TrimCal_E_PerAnode_Sec_%d", i + 1);
        sprintf(Name2, "Section %d", i + 1);
        cTrimCal_Ene[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);
        cTrimCal_Ene[i]->Divide(2, 3); // 2 col and 3 raws (if triangular anodes: one raw per pair)

        sprintf(Name1, "TrimCal_DT_PerAnode_Sec_%d", i + 1);
        sprintf(Name2, "Section %d", i + 1);
        cTrimCal_DT[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);
        cTrimCal_DT[i]->Divide(2, 3); // 2 col and 3 raws (if triangular anodes: one raw per pair)

        for (Int_t j = 0; j < fNumAnodes; j++)
        {
            sprintf(Name1, "fh1_trimcal_Esub_sec%d_a%d", i + 1, j + 1);
            sprintf(Name2, "Sec %d:Anode %d", i + 1, j + 1);
            fh1_trimcal_Esub[j + fNumAnodes * i] = new TH1F(Name1, Name2, 8192, 0, 8192);
            fh1_trimcal_Esub[j + fNumAnodes * i]->GetXaxis()->SetTitle("Energy [channels], if mult==1");
            fh1_trimcal_Esub[j + fNumAnodes * i]->GetYaxis()->SetTitle("Counts");
            fh1_trimcal_Esub[j + fNumAnodes * i]->GetYaxis()->SetTitleOffset(1.1);
            fh1_trimcal_Esub[j + fNumAnodes * i]->GetXaxis()->CenterTitle(true);
            fh1_trimcal_Esub[j + fNumAnodes * i]->GetYaxis()->CenterTitle(true);
            fh1_trimcal_Esub[j + fNumAnodes * i]->GetXaxis()->SetLabelSize(0.045);
            fh1_trimcal_Esub[j + fNumAnodes * i]->GetXaxis()->SetTitleSize(0.045);
            fh1_trimcal_Esub[j + fNumAnodes * i]->GetYaxis()->SetLabelSize(0.045);
            fh1_trimcal_Esub[j + fNumAnodes * i]->GetYaxis()->SetTitleSize(0.045);
            fh1_trimcal_Esub[j + fNumAnodes * i]->SetLineColor(kBlue);
            cTrimCal_Ene[i]->cd(j + 1);
            fh1_trimcal_Esub[j + fNumAnodes * i]->Draw("");

            sprintf(Name1, "fh1_trimcal_Ematch_sec%d_a%d", i + 1, j + 1);
            sprintf(Name2, "Sec %d:Anode %d", i + 1, j + 1);
            fh1_trimcal_Ematch[j + fNumAnodes * i] = new TH1F(Name1, Name2, 8192, 0, 8192);
            fh1_trimcal_Ematch[j + fNumAnodes * i]->GetXaxis()->SetTitle("Energy [channels], if mult==1");
            fh1_trimcal_Ematch[j + fNumAnodes * i]->GetYaxis()->SetTitle("Counts");
            fh1_trimcal_Ematch[j + fNumAnodes * i]->GetYaxis()->SetTitleOffset(1.1);
            fh1_trimcal_Ematch[j + fNumAnodes * i]->GetXaxis()->CenterTitle(true);
            fh1_trimcal_Ematch[j + fNumAnodes * i]->GetYaxis()->CenterTitle(true);
            fh1_trimcal_Ematch[j + fNumAnodes * i]->GetXaxis()->SetLabelSize(0.045);
            fh1_trimcal_Ematch[j + fNumAnodes * i]->GetXaxis()->SetTitleSize(0.045);
            fh1_trimcal_Ematch[j + fNumAnodes * i]->GetYaxis()->SetLabelSize(0.045);
            fh1_trimcal_Ematch[j + fNumAnodes * i]->GetYaxis()->SetTitleSize(0.045);
            fh1_trimcal_Ematch[j + fNumAnodes * i]->SetLineColor(kRed);
            cTrimCal_Ene[i]->cd(j + 1);
            fh1_trimcal_Ematch[j + fNumAnodes * i]->Draw("same");

            sprintf(Name1, "fh1_trimcal_DTraw_sec%d_a%d", i + 1, j + 1);
            sprintf(Name2, "Sec %d:Anode %d", i + 1, j + 1);
            fh1_trimcal_DTraw[j + fNumAnodes * i] = new TH1D(Name1, Name2, 40000, 0, 40000);
            fh1_trimcal_DTraw[j + fNumAnodes * i]->GetXaxis()->SetTitle(
                "Drift Time [channels, 100 ps TDC resolution], if mult==1");
            fh1_trimcal_DTraw[j + fNumAnodes * i]->GetYaxis()->SetTitle("Counts");
            fh1_trimcal_DTraw[j + fNumAnodes * i]->GetYaxis()->SetTitleOffset(1.1);
            fh1_trimcal_DTraw[j + fNumAnodes * i]->GetXaxis()->CenterTitle(true);
            fh1_trimcal_DTraw[j + fNumAnodes * i]->GetYaxis()->CenterTitle(true);
            fh1_trimcal_DTraw[j + fNumAnodes * i]->GetXaxis()->SetLabelSize(0.045);
            fh1_trimcal_DTraw[j + fNumAnodes * i]->GetXaxis()->SetTitleSize(0.045);
            fh1_trimcal_DTraw[j + fNumAnodes * i]->GetYaxis()->SetLabelSize(0.045);
            fh1_trimcal_DTraw[j + fNumAnodes * i]->GetYaxis()->SetTitleSize(0.045);
            fh1_trimcal_DTraw[j + fNumAnodes * i]->SetLineColor(kBlue);
            cTrimCal_DT[i]->cd(j + 1);
            fh1_trimcal_DTraw[j + fNumAnodes * i]->Draw("");

            sprintf(Name1, "fh1_trimcal_DTalign_sec%d_a%d", i + 1, j + 1);
            sprintf(Name2, "Sec %d:Anode %d", i + 1, j + 1);
            fh1_trimcal_DTalign[j + fNumAnodes * i] = new TH1D(Name1, Name2, 40000, 0, 40000);
            fh1_trimcal_DTalign[j + fNumAnodes * i]->GetXaxis()->SetTitle(
                "Drift Time [channels, 100 ps TDC resolution], if mult==1");
            fh1_trimcal_DTalign[j + fNumAnodes * i]->GetYaxis()->SetTitle("Counts");
            fh1_trimcal_DTalign[j + fNumAnodes * i]->GetYaxis()->SetTitleOffset(1.1);
            fh1_trimcal_DTalign[j + fNumAnodes * i]->GetXaxis()->CenterTitle(true);
            fh1_trimcal_DTalign[j + fNumAnodes * i]->GetYaxis()->CenterTitle(true);
            fh1_trimcal_DTalign[j + fNumAnodes * i]->GetXaxis()->SetLabelSize(0.045);
            fh1_trimcal_DTalign[j + fNumAnodes * i]->GetXaxis()->SetTitleSize(0.045);
            fh1_trimcal_DTalign[j + fNumAnodes * i]->GetYaxis()->SetLabelSize(0.045);
            fh1_trimcal_DTalign[j + fNumAnodes * i]->GetYaxis()->SetTitleSize(0.045);
            fh1_trimcal_DTalign[j + fNumAnodes * i]->SetLineColor(kRed);
            cTrimCal_DT[i]->cd(j + 1);
            fh1_trimcal_DTalign[j + fNumAnodes * i]->Draw("sames");

        } // end of loop over the anodes
    }     // end of loop over the sections

    // === ================ === //
    // === MAIN FOLDER-Trim === //
    // === ================ === //

    TFolder* mainfolTrim = new TFolder("Trim", "Trim info");
    if (fMappedItemsTrim)
    {
        for (Int_t i = 0; i < fNumSections; i++)
            mainfolTrim->Add(cTrimMap_E[i]);
        for (Int_t i = 0; i < fNumSections; i++)
            mainfolTrim->Add(cTrimMap_DT[i]);
        for (Int_t i = 0; i < fNumSections; i++)
            mainfolTrim->Add(cTrimMap_Mult[i]);
        mainfolTrim->Add(cTrimMap_DeltaTrefTtrig);
        for (Int_t i = 0; i < fNumSections; i++)
        {
            mainfolTrim->Add(cTrimMap_EvsDT[i]);
            mainfolTrim->Add(cTrimMap_DTvsDT[i]);
        }
    }
    if (fCalItemsTrim)
    {
        for (Int_t i = 0; i < fNumSections; i++)
            mainfolTrim->Add(cTrimCal_Ene[i]);
        for (Int_t i = 0; i < fNumSections; i++)
            mainfolTrim->Add(cTrimCal_DT[i]);
    }
    run->AddObject(mainfolTrim);

    // === Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_TRIM_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofTrimOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofTrimOnlineSpectra::Reset_Histo";

    // Mapped data
    if (fMappedItemsTrim)
    {
        for (Int_t i = 0; i < fNumSections; i++)
        {
            fh1_trimmap_DeltaTrefTtrig[i]->Reset();
            fh1_trimmap_Mult[i]->Reset();
            for (Int_t j = 0; j < fNumAnodes; j++)
            {
                fh1_trimmap_E[i * fNumAnodes + j]->Reset();
                fh1_trimmap_DT[i * fNumAnodes + j]->Reset();
                fh2_trimmap_EvsDT[i * fNumAnodes + j]->Reset();
            }
            for (Int_t j = 0; j < fNumAnodes - 1; j++)
            {
                fh2_trimmap_DTvsDT[i * (fNumAnodes - 1) + j]->Reset();
            }
        }
    }
    // Cal data
    if (fCalItemsTrim)
    {
        for (Int_t i = 0; i < fNumSections; i++)
        {
            for (Int_t j = 0; j < fNumAnodes; j++)
            {
                fh1_trimcal_Esub[i * fNumAnodes + j]->Reset();
                fh1_trimcal_Ematch[i * fNumAnodes + j]->Reset();
                fh1_trimcal_DTraw[i * fNumAnodes + j]->Reset();
                fh1_trimcal_DTalign[i * fNumAnodes + j]->Reset();
            }
        }
    }
}

void R3BSofTrimOnlineSpectra::Exec(Option_t* option)
{

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTrimOnlineSpectra::Exec FairRootManager not found";

    Int_t nHits;

    // === MAPPED data : Section ID: GetSecID() returns 1-based section number
    // ===               Anode ID: GetAnodeID returns 1-based anode number
    // ===               For Anode ID [1:6] : Eraw, Traw
    // ===               For Anode ID 7     : 0,    Traw of the ref signal
    // ===               For Anode ID 8     : 0,    Traw of the trig signal
    if (fMappedItemsTrim && fMappedItemsTrim->GetEntriesFast() > 0)
    {

        Double_t Eraw[fNumSections][fNumAnodes];
        Double_t Traw[fNumSections][fNumAnodes + fNumTref + fNumTtrig];
        UInt_t mult[fNumSections][fNumAnodes + fNumTref + fNumTtrig];
        for (Int_t j = 0; j < fNumSections; j++)
            for (Int_t i = 0; i < fNumAnodes; i++)
                Eraw[j][i] = 0.; // attention, only first hit
        for (Int_t j = 0; j < fNumSections; j++)
            for (Int_t i = 0; i < fNumAnodes + fNumTref + fNumTtrig; i++)
            {
                Traw[j][i] = -1.; // attention, only first hit
                mult[j][i] = 0;
            }

        nHits = fMappedItemsTrim->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTrimMappedData* hit = (R3BSofTrimMappedData*)fMappedItemsTrim->At(ihit);
            if (!hit)
                continue;
            fh1_trimmap_Mult[hit->GetSecID() - 1]->Fill(hit->GetAnodeID());
            mult[hit->GetSecID() - 1][hit->GetAnodeID() - 1]++;
            // ATTENTION: take into accound only the first hit
            if (mult[hit->GetSecID() - 1][hit->GetAnodeID() - 1] == 1)
            {
                if (hit->GetEnergy() < 8192 && hit->GetEnergy() > 0) // Eraw=0 for Tref and Ttrig
                    Eraw[hit->GetSecID() - 1][hit->GetAnodeID() - 1] = hit->GetEnergy();
                Traw[hit->GetSecID() - 1][hit->GetAnodeID() - 1] = hit->GetTime();
            }
        }

        for (Int_t s = 0; s < fNumSections; s++)
        {
            if (mult[s][fNumAnodes] == 1 && mult[s][fNumAnodes + 1] == 1)
                fh1_trimmap_DeltaTrefTtrig[s]->Fill(Traw[s][fNumAnodes] - Traw[s][fNumAnodes + 1]);
            for (Int_t a = 0; a < fNumAnodes; a++)
            {
                if (mult[s][a] == 1)
                {
                    fh1_trimmap_E[s * fNumAnodes + a]->Fill(Eraw[s][a]);
                    if (mult[s][fNumAnodes] == 1)
                    {
                        fh1_trimmap_DT[s * fNumAnodes + a]->Fill(Traw[s][a] - Traw[s][fNumAnodes]);
                        fh2_trimmap_EvsDT[s * fNumAnodes + a]->Fill(Traw[s][a] - Traw[s][fNumAnodes], Eraw[s][a]);
                    }
                }
            }
            for (Int_t a = 0; a < fNumAnodes - 1; a++)
                if (mult[s][a] == 1 && mult[s][a + 1] == 1 && mult[s][fNumAnodes] == 1)
                    fh2_trimmap_DTvsDT[s * (fNumAnodes - 1) + a]->Fill(Traw[s][a] - Traw[s][fNumAnodes],
                                                                       Traw[s][a + 1] - Traw[s][fNumAnodes]);
        } // end of for(fNumSections)

    } // end of if (MappedItemsTrim)

    // === CAL data
    Int_t iSec, iAnode;
    if (fCalItemsTrim && fCalItemsTrim->GetEntriesFast() > 0)
    {
        nHits = fCalItemsTrim->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTrimCalData* hit = (R3BSofTrimCalData*)fCalItemsTrim->At(ihit);
            if (!hit)
                continue;
            iSec = hit->GetSecID() - 1;
            iAnode = hit->GetAnodeID() - 1;
            fh1_trimcal_Esub[iAnode + iSec * fNumAnodes]->Fill(hit->GetEnergySub());
            fh1_trimcal_Ematch[iAnode + iSec * fNumAnodes]->Fill(hit->GetEnergyMatch());
            fh1_trimcal_DTraw[iAnode + iSec * fNumAnodes]->Fill(hit->GetDriftTimeRaw());
            fh1_trimcal_DTalign[iAnode + iSec * fNumAnodes]->Fill(hit->GetDriftTimeAligned());
        } // end of lopp over the cal data
    }

    fNEvents += 1;
}

void R3BSofTrimOnlineSpectra::FinishEvent()
{
    if (fMappedItemsTrim)
        fMappedItemsTrim->Clear();
    if (fCalItemsTrim)
        fCalItemsTrim->Clear();
}

void R3BSofTrimOnlineSpectra::FinishTask()
{
    if (fMappedItemsTrim)
    {
        cTrimMap_DeltaTrefTtrig->Write();
        for (Int_t i = 0; i < fNumSections; i++)
        {
            cTrimMap_E[i]->Write();
            cTrimMap_DT[i]->Write();
            cTrimMap_Mult[i]->Write();
            cTrimMap_EvsDT[i]->Write();
            cTrimMap_DTvsDT[i]->Write();
        }
    }

    if (fCalItemsTrim)
    {
        for (Int_t s = 0; s < fNumSections; s++)
        {
            cTrimCal_Ene[s]->Write();
            cTrimCal_DT[s]->Write();
        }
    }
}

ClassImp(R3BSofTrimOnlineSpectra)
