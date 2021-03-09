/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofTrimOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BSofTrimCalData.h"
#include "R3BSofTrimHitData.h"
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
    , fHitItemsTrim(NULL)
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
    , fHitItemsTrim(NULL)
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
    if (fHitItemsTrim)
        delete fHitItemsTrim;
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

    // === get access to hit data of the Triple-MUSIC === //
    fHitItemsTrim = (TClonesArray*)mgr->GetObject("TrimHitData");
    if (!fHitItemsTrim)
    {
        LOG(FATAL) << " R3BSofTrimOnlineSpectra::Init(), TrimHitData not found";
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
            fh1_trimmap_E[j + fNumAnodes * i] = new TH1F(Name1, Name2, 16384, 0, 65536);
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
            fh2_trimmap_EvsDT[j + fNumAnodes * i] = new TH2F(Name1, Name2, 1310, 0, 65500, 800, 0, 40000);
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

    cTrimCal_Ene = new TCanvas*[fNumSections];
    cTrimCal_DT = new TCanvas*[fNumSections];

    fh1_trimcal_Esub = new TH1F*[fNumSections * fNumAnodes];
    fh1_trimcal_Ematch = new TH1F*[fNumSections * fNumAnodes];
    fh1_trimcal_DTraw = new TH1D*[fNumSections * fNumAnodes];
    fh1_trimcal_DTalign = new TH1D*[fNumSections * fNumAnodes];

    for (Int_t i = 0; i < fNumSections; i++)
    {
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
            fh1_trimcal_Esub[j + fNumAnodes * i] = new TH1F(Name1, Name2, 16384, 0, 65536);
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
            fh1_trimcal_Ematch[j + fNumAnodes * i] = new TH1F(Name1, Name2, 16384, 0, 65536);
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

    // === ======== === //
    // === HIT DATA === //
    // === ======== === //

    cTrimHit_E = new TCanvas("TrimHit_E", "Energy at Hit level", 10, 10, 800, 700);
    cTrimHit_E->Divide(4, 3);
    cTrimHit_Z = new TCanvas("TrimHit_Z", "Atomic Number at Hit level", 10, 10, 800, 700);
    cTrimHit_Z->Divide(2, 3);

    fh1_trimhit_Eraw = new TH1F*[fNumSections];
    fh1_trimhit_Ebeta = new TH1F*[fNumSections];
    fh1_trimhit_Edt = new TH1F*[fNumSections];
    fh1_trimhit_Etheta = new TH1F*[fNumSections];
    fh1_trimhit_Z = new TH1F*[fNumSections + 2];

    for (Int_t i = 0; i < fNumSections; i++)
    {
        sprintf(Name1, "fh1_trimhit_Eraw_sec%d", i + 1);
        sprintf(Name2, "Ene raw Sec %d", i + 1);
        fh1_trimhit_Eraw[i] = new TH1F(Name1, Name2, 16384, 0, 65536);
        fh1_trimhit_Eraw[i]->GetXaxis()->SetTitle("Energy [channels], if mult==1");
        fh1_trimhit_Eraw[i]->GetYaxis()->SetTitle("Counts");
        fh1_trimhit_Eraw[i]->GetYaxis()->SetTitleOffset(1.1);
        fh1_trimhit_Eraw[i]->GetXaxis()->CenterTitle(true);
        fh1_trimhit_Eraw[i]->GetYaxis()->CenterTitle(true);
        fh1_trimhit_Eraw[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_trimhit_Eraw[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_trimhit_Eraw[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_trimhit_Eraw[i]->GetYaxis()->SetTitleSize(0.045);
        fh1_trimhit_Eraw[i]->SetLineColor(kBlue);
        cTrimHit_E->cd(4 * i + 1);
        fh1_trimhit_Eraw[i]->Draw("");

        sprintf(Name1, "fh1_trimhit_Ebeta_sec%d", i + 1);
        sprintf(Name2, "Ene corrected from beta Sec %d", i + 1);
        fh1_trimhit_Ebeta[i] = new TH1F(Name1, Name2, 16384, 0, 65536);
        fh1_trimhit_Ebeta[i]->GetXaxis()->SetTitle("Energy [channels], if mult==1");
        fh1_trimhit_Ebeta[i]->GetYaxis()->SetTitle("Counts");
        fh1_trimhit_Ebeta[i]->GetYaxis()->SetTitleOffset(1.1);
        fh1_trimhit_Ebeta[i]->GetXaxis()->CenterTitle(true);
        fh1_trimhit_Ebeta[i]->GetYaxis()->CenterTitle(true);
        fh1_trimhit_Ebeta[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_trimhit_Ebeta[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_trimhit_Ebeta[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_trimhit_Ebeta[i]->GetYaxis()->SetTitleSize(0.045);
        fh1_trimhit_Ebeta[i]->SetLineColor(kOrange + 10);
        cTrimHit_E->cd(4 * i + 2);
        fh1_trimhit_Ebeta[i]->Draw("sames");

        sprintf(Name1, "fh1_trimhit_Edt_sec%d", i + 1);
        sprintf(Name2, "Ene corrected from DT Sec %d", i + 1);
        fh1_trimhit_Edt[i] = new TH1F(Name1, Name2, 16384, 0, 65536);
        fh1_trimhit_Edt[i]->GetXaxis()->SetTitle("Energy [channels], if mult==1");
        fh1_trimhit_Edt[i]->GetYaxis()->SetTitle("Counts");
        fh1_trimhit_Edt[i]->GetYaxis()->SetTitleOffset(1.1);
        fh1_trimhit_Edt[i]->GetXaxis()->CenterTitle(true);
        fh1_trimhit_Edt[i]->GetYaxis()->CenterTitle(true);
        fh1_trimhit_Edt[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_trimhit_Edt[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_trimhit_Edt[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_trimhit_Edt[i]->GetYaxis()->SetTitleSize(0.045);
        fh1_trimhit_Edt[i]->SetLineColor(8);
        cTrimHit_E->cd(4 * i + 3);
        fh1_trimhit_Edt[i]->Draw("sames");

        sprintf(Name1, "fh1_trimhit_Etheta_sec%d", i + 1);
        sprintf(Name2, "Ene corrected from theta Sec %d", i + 1);
        fh1_trimhit_Etheta[i] = new TH1F(Name1, Name2, 16384, 0, 65536);
        fh1_trimhit_Etheta[i]->GetXaxis()->SetTitle("Energy [channels], if mult==1");
        fh1_trimhit_Etheta[i]->GetYaxis()->SetTitle("Counts");
        fh1_trimhit_Etheta[i]->GetYaxis()->SetTitleOffset(1.1);
        fh1_trimhit_Etheta[i]->GetXaxis()->CenterTitle(true);
        fh1_trimhit_Etheta[i]->GetYaxis()->CenterTitle(true);
        fh1_trimhit_Etheta[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_trimhit_Etheta[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_trimhit_Etheta[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_trimhit_Etheta[i]->GetYaxis()->SetTitleSize(0.045);
        fh1_trimhit_Etheta[i]->SetLineColor(kRed);
        cTrimHit_E->cd(4 * i + 4);
        fh1_trimhit_Etheta[i]->Draw("sames");

        sprintf(Name1, "fh1_trimhit_Z_sec%d", i + 1);
        sprintf(Name2, "Charge Sec %d", i + 1);
        fh1_trimhit_Z[i] = new TH1F(Name1, Name2, 1000, 0, 100);
        fh1_trimhit_Z[i]->GetXaxis()->SetTitle("Charge (Q) if mult==1");
        fh1_trimhit_Z[i]->GetYaxis()->SetTitle("Counts");
        fh1_trimhit_Z[i]->GetYaxis()->SetTitleOffset(1.1);
        fh1_trimhit_Z[i]->GetXaxis()->CenterTitle(true);
        fh1_trimhit_Z[i]->GetYaxis()->CenterTitle(true);
        fh1_trimhit_Z[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_trimhit_Z[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_trimhit_Z[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_trimhit_Z[i]->GetYaxis()->SetTitleSize(0.045);
        fh1_trimhit_Z[i]->SetLineColor(kBlue + i);
        cTrimHit_Z->cd(i + 1);
        fh1_trimhit_Z[i]->Draw();
    }

    sprintf(Name1, "fh1_trimhit_Zmax_sec1_vs_sec2");
    sprintf(Name2, "Zmax - Sec 1 VS Sec 2");
    fh1_trimhit_Z[3] = new TH1F(Name1, Name2, 1000, 0, 100);
    fh1_trimhit_Z[3]->GetXaxis()->SetTitle("Atomic Numbers (Z) if mult==1");
    fh1_trimhit_Z[3]->GetYaxis()->SetTitle("Counts");
    fh1_trimhit_Z[3]->GetYaxis()->SetTitleOffset(1.1);
    fh1_trimhit_Z[3]->GetXaxis()->CenterTitle(true);
    fh1_trimhit_Z[3]->GetYaxis()->CenterTitle(true);
    fh1_trimhit_Z[3]->GetXaxis()->SetLabelSize(0.045);
    fh1_trimhit_Z[3]->GetXaxis()->SetTitleSize(0.045);
    fh1_trimhit_Z[3]->GetYaxis()->SetLabelSize(0.045);
    fh1_trimhit_Z[3]->GetYaxis()->SetTitleSize(0.045);
    fh1_trimhit_Z[3]->SetLineColor(kRed + 3);
    cTrimHit_Z->cd(4);
    fh1_trimhit_Z[3]->Draw();

    sprintf(Name1, "fh1_trimhit_Zmax");
    sprintf(Name2, "Zmax");
    fh1_trimhit_Z[4] = new TH1F(Name1, Name2, 1000, 0, 100);
    fh1_trimhit_Z[4]->GetXaxis()->SetTitle("Atomic Numbers (Z) if mult==1");
    fh1_trimhit_Z[4]->GetYaxis()->SetTitle("Counts");
    fh1_trimhit_Z[4]->GetYaxis()->SetTitleOffset(1.1);
    fh1_trimhit_Z[4]->GetXaxis()->CenterTitle(true);
    fh1_trimhit_Z[4]->GetYaxis()->CenterTitle(true);
    fh1_trimhit_Z[4]->GetXaxis()->SetLabelSize(0.045);
    fh1_trimhit_Z[4]->GetXaxis()->SetTitleSize(0.045);
    fh1_trimhit_Z[4]->GetYaxis()->SetLabelSize(0.045);
    fh1_trimhit_Z[4]->GetYaxis()->SetTitleSize(0.045);
    fh1_trimhit_Z[4]->SetLineColor(kRed);
    cTrimHit_Z->cd(5);
    fh1_trimhit_Z[4]->Draw();

    cTrimHit_CorrDep = new TCanvas*[fNumSections];
    fh2_trimhit_Ebeta_vs_DT = new TH2F*[fNumSections];
    fh2_trimhit_Edt_vs_DT = new TH2F*[fNumSections];
    fh2_trimhit_Edt_vs_theta = new TH2F*[fNumSections];
    fh2_trimhit_Etheta_vs_theta = new TH2F*[fNumSections];

    for (Int_t i = 0; i < fNumSections; i++)
    {
        sprintf(Name1, "TrimHit_Corr_Sec_%d", i + 1);
        sprintf(Name2, "Section %d", i + 1);
        cTrimHit_CorrDep[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);
        cTrimHit_CorrDep[i]->Divide(2, 2);

        sprintf(Name1, "Ebeta_vs_DT_Sec_%d", i + 1);
        sprintf(Name2, "Ebeta VS DT %d", i + 1);
        fh2_trimhit_Ebeta_vs_DT[i] = new TH2F(Name1, Name2, 800, 0, 40000, 1280, 1000, 65000);
        fh2_trimhit_Ebeta_vs_DT[i]->GetXaxis()->SetTitle("Drift Time [channels, 100ps TDC resolution], if mult==1");
        fh2_trimhit_Ebeta_vs_DT[i]->GetYaxis()->SetTitle("Energy corr beta [channels], if mult==1");
        fh2_trimhit_Ebeta_vs_DT[i]->GetYaxis()->SetTitleOffset(1.1);
        fh2_trimhit_Ebeta_vs_DT[i]->GetXaxis()->CenterTitle(true);
        fh2_trimhit_Ebeta_vs_DT[i]->GetYaxis()->CenterTitle(true);
        fh2_trimhit_Ebeta_vs_DT[i]->GetXaxis()->SetLabelSize(0.045);
        fh2_trimhit_Ebeta_vs_DT[i]->GetXaxis()->SetTitleSize(0.045);
        fh2_trimhit_Ebeta_vs_DT[i]->GetYaxis()->SetLabelSize(0.045);
        fh2_trimhit_Ebeta_vs_DT[i]->GetYaxis()->SetTitleSize(0.045);
        cTrimHit_CorrDep[i]->cd(1);
        fh2_trimhit_Ebeta_vs_DT[i]->Draw("col");

        sprintf(Name1, "Edt_vs_DT_Sec_%d", i + 1);
        sprintf(Name2, "Edt VS DT %d", i + 1);
        fh2_trimhit_Edt_vs_DT[i] = new TH2F(Name1, Name2, 800, 0, 40000, 1280, 1000, 65000);
        fh2_trimhit_Edt_vs_DT[i]->GetXaxis()->SetTitle("Drift Time [channels, 100ps TDC resolution], if mult==1");
        fh2_trimhit_Edt_vs_DT[i]->GetYaxis()->SetTitle("Energy corr drift time [channels], if mult==1");
        fh2_trimhit_Edt_vs_DT[i]->GetYaxis()->SetTitleOffset(1.1);
        fh2_trimhit_Edt_vs_DT[i]->GetXaxis()->CenterTitle(true);
        fh2_trimhit_Edt_vs_DT[i]->GetYaxis()->CenterTitle(true);
        fh2_trimhit_Edt_vs_DT[i]->GetXaxis()->SetLabelSize(0.045);
        fh2_trimhit_Edt_vs_DT[i]->GetXaxis()->SetTitleSize(0.045);
        fh2_trimhit_Edt_vs_DT[i]->GetYaxis()->SetLabelSize(0.045);
        fh2_trimhit_Edt_vs_DT[i]->GetYaxis()->SetTitleSize(0.045);
        cTrimHit_CorrDep[i]->cd(2);
        fh2_trimhit_Edt_vs_DT[i]->Draw("col");

        sprintf(Name1, "Edt_vs_theta_Sec_%d", i + 1);
        sprintf(Name2, "Edt VS theta %d", i + 1);
        fh2_trimhit_Edt_vs_theta[i] = new TH2F(Name1, Name2, 800, -8, 8, 1280, 1000, 65000);
        fh2_trimhit_Edt_vs_theta[i]->GetXaxis()->SetTitle("DTdiff [channels, 100ps TDC resolution], if mult==1");
        fh2_trimhit_Edt_vs_theta[i]->GetYaxis()->SetTitle("Energy corr drift time [channels], if mult==1");
        fh2_trimhit_Edt_vs_theta[i]->GetYaxis()->SetTitleOffset(1.1);
        fh2_trimhit_Edt_vs_theta[i]->GetXaxis()->CenterTitle(true);
        fh2_trimhit_Edt_vs_theta[i]->GetYaxis()->CenterTitle(true);
        fh2_trimhit_Edt_vs_theta[i]->GetXaxis()->SetLabelSize(0.045);
        fh2_trimhit_Edt_vs_theta[i]->GetXaxis()->SetTitleSize(0.045);
        fh2_trimhit_Edt_vs_theta[i]->GetYaxis()->SetLabelSize(0.045);
        fh2_trimhit_Edt_vs_theta[i]->GetYaxis()->SetTitleSize(0.045);
        cTrimHit_CorrDep[i]->cd(3);
        fh2_trimhit_Edt_vs_theta[i]->Draw("col");

        sprintf(Name1, "Etheta_vs_theta_Sec_%d", i + 1);
        sprintf(Name2, "Etheta VS theta %d", i + 1);
        fh2_trimhit_Etheta_vs_theta[i] = new TH2F(Name1, Name2, 800, -8, 8, 1280, 1000, 65000);
        fh2_trimhit_Etheta_vs_theta[i]->GetXaxis()->SetTitle("DTdiff [channels, 100ps TDC resolution], if mult==1");
        fh2_trimhit_Etheta_vs_theta[i]->GetYaxis()->SetTitle("Energy corr theta [channels], if mult==1");
        fh2_trimhit_Etheta_vs_theta[i]->GetYaxis()->SetTitleOffset(1.1);
        fh2_trimhit_Etheta_vs_theta[i]->GetXaxis()->CenterTitle(true);
        fh2_trimhit_Etheta_vs_theta[i]->GetYaxis()->CenterTitle(true);
        fh2_trimhit_Etheta_vs_theta[i]->GetXaxis()->SetLabelSize(0.045);
        fh2_trimhit_Etheta_vs_theta[i]->GetXaxis()->SetTitleSize(0.045);
        fh2_trimhit_Etheta_vs_theta[i]->GetYaxis()->SetLabelSize(0.045);
        fh2_trimhit_Etheta_vs_theta[i]->GetYaxis()->SetTitleSize(0.045);
        cTrimHit_CorrDep[i]->cd(4);
        fh2_trimhit_Etheta_vs_theta[i]->Draw("col");
    }

    cTrimHit_EvsE = new TCanvas("TrimHit_EvsE", "EvsE at Hit level", 10, 10, 800, 700);
    cTrimHit_EvsE->Divide(2, 4);

    fh2_trimhit_EvsE_raw = new TH2F*[2];
    fh2_trimhit_EvsE_beta = new TH2F*[2];
    fh2_trimhit_EvsE_dt = new TH2F*[2];
    fh2_trimhit_EvsE_theta = new TH2F*[2];

    sprintf(Name1, "ErawS2_vs_ErawS1");
    sprintf(Name2, "Eraw[Sec2] vs Eraw [Sec1]");
    fh2_trimhit_EvsE_raw[0] = new TH2F(Name1, Name2, 1280, 1000, 65000, 1280, 1000, 65000);
    fh2_trimhit_EvsE_raw[0]->GetXaxis()->SetTitle("Energy raw - Sec1 [channels], if mult==1");
    fh2_trimhit_EvsE_raw[0]->GetYaxis()->SetTitle("Energy raw - Sec2 [channels], if mult==1");
    fh2_trimhit_EvsE_raw[0]->GetYaxis()->SetTitleOffset(1.1);
    fh2_trimhit_EvsE_raw[0]->GetXaxis()->CenterTitle(true);
    fh2_trimhit_EvsE_raw[0]->GetYaxis()->CenterTitle(true);
    fh2_trimhit_EvsE_raw[0]->GetXaxis()->SetLabelSize(0.045);
    fh2_trimhit_EvsE_raw[0]->GetXaxis()->SetTitleSize(0.045);
    fh2_trimhit_EvsE_raw[0]->GetYaxis()->SetLabelSize(0.045);
    fh2_trimhit_EvsE_raw[0]->GetYaxis()->SetTitleSize(0.045);
    cTrimHit_EvsE->cd(1);
    fh2_trimhit_EvsE_raw[0]->Draw("col");

    sprintf(Name1, "ErawS3_vs_MaxEraw");
    sprintf(Name2, "Eraw[Sec3] vs MaxEraw [Sec1,Sec2]");
    fh2_trimhit_EvsE_raw[1] = new TH2F(Name1, Name2, 1280, 1000, 65000, 1280, 1000, 65000);
    fh2_trimhit_EvsE_raw[1]->GetXaxis()->SetTitle("Energy raw - Max (Sec1,Sec2) [channels], if mult==1");
    fh2_trimhit_EvsE_raw[1]->GetYaxis()->SetTitle("Energy raw - Sec3 [channels], if mult==1");
    fh2_trimhit_EvsE_raw[1]->GetYaxis()->SetTitleOffset(1.1);
    fh2_trimhit_EvsE_raw[1]->GetXaxis()->CenterTitle(true);
    fh2_trimhit_EvsE_raw[1]->GetYaxis()->CenterTitle(true);
    fh2_trimhit_EvsE_raw[1]->GetXaxis()->SetLabelSize(0.045);
    fh2_trimhit_EvsE_raw[1]->GetXaxis()->SetTitleSize(0.045);
    fh2_trimhit_EvsE_raw[1]->GetYaxis()->SetLabelSize(0.045);
    fh2_trimhit_EvsE_raw[1]->GetYaxis()->SetTitleSize(0.045);
    cTrimHit_EvsE->cd(2);
    fh2_trimhit_EvsE_raw[1]->Draw("col");

    sprintf(Name1, "EbetaS2_vs_EbetaS1");
    sprintf(Name2, "Ebeta[Sec2] vs Ebeta [Sec1]");
    fh2_trimhit_EvsE_beta[0] = new TH2F(Name1, Name2, 1280, 1000, 65000, 1280, 1000, 65000);
    fh2_trimhit_EvsE_beta[0]->GetXaxis()->SetTitle("Energy beta - Sec1 [channels], if mult==1");
    fh2_trimhit_EvsE_beta[0]->GetYaxis()->SetTitle("Energy beta - Sec2 [channels], if mult==1");
    fh2_trimhit_EvsE_beta[0]->GetYaxis()->SetTitleOffset(1.1);
    fh2_trimhit_EvsE_beta[0]->GetXaxis()->CenterTitle(true);
    fh2_trimhit_EvsE_beta[0]->GetYaxis()->CenterTitle(true);
    fh2_trimhit_EvsE_beta[0]->GetXaxis()->SetLabelSize(0.045);
    fh2_trimhit_EvsE_beta[0]->GetXaxis()->SetTitleSize(0.045);
    fh2_trimhit_EvsE_beta[0]->GetYaxis()->SetLabelSize(0.045);
    fh2_trimhit_EvsE_beta[0]->GetYaxis()->SetTitleSize(0.045);
    cTrimHit_EvsE->cd(3);
    fh2_trimhit_EvsE_beta[0]->Draw("col");

    sprintf(Name1, "EbetaS3_vs_MaxEbeta");
    sprintf(Name2, "Ebeta[Sec3] vs MaxEbeta [Sec1,Sec2]");
    fh2_trimhit_EvsE_beta[1] = new TH2F(Name1, Name2, 1280, 1000, 65000, 1280, 1000, 65000);
    fh2_trimhit_EvsE_beta[1]->GetXaxis()->SetTitle("Energy beta - Max (Sec1,Sec2) [channels], if mult==1");
    fh2_trimhit_EvsE_beta[1]->GetYaxis()->SetTitle("Energy beta - Sec3 [channels], if mult==1");
    fh2_trimhit_EvsE_beta[1]->GetYaxis()->SetTitleOffset(1.1);
    fh2_trimhit_EvsE_beta[1]->GetXaxis()->CenterTitle(true);
    fh2_trimhit_EvsE_beta[1]->GetYaxis()->CenterTitle(true);
    fh2_trimhit_EvsE_beta[1]->GetXaxis()->SetLabelSize(0.045);
    fh2_trimhit_EvsE_beta[1]->GetXaxis()->SetTitleSize(0.045);
    fh2_trimhit_EvsE_beta[1]->GetYaxis()->SetLabelSize(0.045);
    fh2_trimhit_EvsE_beta[1]->GetYaxis()->SetTitleSize(0.045);
    cTrimHit_EvsE->cd(4);
    fh2_trimhit_EvsE_beta[1]->Draw("col");

    sprintf(Name1, "EdtS2_vs_EdtS1");
    sprintf(Name2, "Edt[Sec2] vs Edt [Sec1]");
    fh2_trimhit_EvsE_dt[0] = new TH2F(Name1, Name2, 1280, 1000, 65000, 1280, 1000, 65000);
    fh2_trimhit_EvsE_dt[0]->GetXaxis()->SetTitle("Energy dt - Sec1 [channels], if mult==1");
    fh2_trimhit_EvsE_dt[0]->GetYaxis()->SetTitle("Energy dt - Sec2 [channels], if mult==1");
    fh2_trimhit_EvsE_dt[0]->GetYaxis()->SetTitleOffset(1.1);
    fh2_trimhit_EvsE_dt[0]->GetXaxis()->CenterTitle(true);
    fh2_trimhit_EvsE_dt[0]->GetYaxis()->CenterTitle(true);
    fh2_trimhit_EvsE_dt[0]->GetXaxis()->SetLabelSize(0.045);
    fh2_trimhit_EvsE_dt[0]->GetXaxis()->SetTitleSize(0.045);
    fh2_trimhit_EvsE_dt[0]->GetYaxis()->SetLabelSize(0.045);
    fh2_trimhit_EvsE_dt[0]->GetYaxis()->SetTitleSize(0.045);
    cTrimHit_EvsE->cd(5);
    fh2_trimhit_EvsE_dt[0]->Draw("col");

    sprintf(Name1, "EdtS3_vs_MaxEdt");
    sprintf(Name2, "Edt[Sec3] vs MaxEdt [Sec1,Sec2]");
    fh2_trimhit_EvsE_dt[1] = new TH2F(Name1, Name2, 1280, 1000, 65000, 1280, 1000, 65000);
    fh2_trimhit_EvsE_dt[1]->GetXaxis()->SetTitle("Energy dt - Max (Sec1,Sec2) [channels], if mult==1");
    fh2_trimhit_EvsE_dt[1]->GetYaxis()->SetTitle("Energy dt - Sec3 [channels], if mult==1");
    fh2_trimhit_EvsE_dt[1]->GetYaxis()->SetTitleOffset(1.1);
    fh2_trimhit_EvsE_dt[1]->GetXaxis()->CenterTitle(true);
    fh2_trimhit_EvsE_dt[1]->GetYaxis()->CenterTitle(true);
    fh2_trimhit_EvsE_dt[1]->GetXaxis()->SetLabelSize(0.045);
    fh2_trimhit_EvsE_dt[1]->GetXaxis()->SetTitleSize(0.045);
    fh2_trimhit_EvsE_dt[1]->GetYaxis()->SetLabelSize(0.045);
    fh2_trimhit_EvsE_dt[1]->GetYaxis()->SetTitleSize(0.045);
    cTrimHit_EvsE->cd(6);
    fh2_trimhit_EvsE_dt[1]->Draw("col");

    sprintf(Name1, "EthetaS2_vs_EthetaS1");
    sprintf(Name2, "Etheta[Sec2] vs Etheta [Sec1]");
    fh2_trimhit_EvsE_theta[0] = new TH2F(Name1, Name2, 1280, 1000, 65000, 1280, 1000, 65000);
    fh2_trimhit_EvsE_theta[0]->GetXaxis()->SetTitle("Energy theta - Sec1 [channels], if mult==1");
    fh2_trimhit_EvsE_theta[0]->GetYaxis()->SetTitle("Energy theta - Sec2 [channels], if mult==1");
    fh2_trimhit_EvsE_theta[0]->GetYaxis()->SetTitleOffset(1.1);
    fh2_trimhit_EvsE_theta[0]->GetXaxis()->CenterTitle(true);
    fh2_trimhit_EvsE_theta[0]->GetYaxis()->CenterTitle(true);
    fh2_trimhit_EvsE_theta[0]->GetXaxis()->SetLabelSize(0.045);
    fh2_trimhit_EvsE_theta[0]->GetXaxis()->SetTitleSize(0.045);
    fh2_trimhit_EvsE_theta[0]->GetYaxis()->SetLabelSize(0.045);
    fh2_trimhit_EvsE_theta[0]->GetYaxis()->SetTitleSize(0.045);
    cTrimHit_EvsE->cd(7);
    fh2_trimhit_EvsE_theta[0]->Draw("col");

    sprintf(Name1, "EthetaS3_vs_MaxEtheta");
    sprintf(Name2, "Etheta[Sec3] vs MaxEtheta [Sec1,Sec2]");
    fh2_trimhit_EvsE_theta[1] = new TH2F(Name1, Name2, 1280, 1000, 65000, 1280, 1000, 65000);
    fh2_trimhit_EvsE_theta[1]->GetXaxis()->SetTitle("Energy theta - Max (Sec1,Sec2) [channels], if mult==1");
    fh2_trimhit_EvsE_theta[1]->GetYaxis()->SetTitle("Energy theta - Sec3 [channels], if mult==1");
    fh2_trimhit_EvsE_theta[1]->GetYaxis()->SetTitleOffset(1.1);
    fh2_trimhit_EvsE_theta[1]->GetXaxis()->CenterTitle(true);
    fh2_trimhit_EvsE_theta[1]->GetYaxis()->CenterTitle(true);
    fh2_trimhit_EvsE_theta[1]->GetXaxis()->SetLabelSize(0.045);
    fh2_trimhit_EvsE_theta[1]->GetXaxis()->SetTitleSize(0.045);
    fh2_trimhit_EvsE_theta[1]->GetYaxis()->SetLabelSize(0.045);
    fh2_trimhit_EvsE_theta[1]->GetYaxis()->SetTitleSize(0.045);
    cTrimHit_EvsE->cd(8);
    fh2_trimhit_EvsE_theta[1]->Draw("col");

    sprintf(Name1, "TrimHit_ZvsZ");
    sprintf(Name2, "Z vs Z in Triple-MUSIC");
    cTrimHit_ZvsZ = new TCanvas(Name1, Name2, 10, 10, 800, 700);
    cTrimHit_ZvsZ->Divide(1, 2);

    fh2_trimhit_ZvsZ = new TH2F*[2];

    sprintf(Name1, "ZS2_vs_ZS1");
    sprintf(Name2, "Z[Sec2] vs Z[Sec1]");
    fh2_trimhit_ZvsZ[0] = new TH2F(Name1, Name2, 900, 10, 100, 900, 10, 100);
    fh2_trimhit_ZvsZ[0]->GetXaxis()->SetTitle("Charge - Sec1, if mult==1");
    fh2_trimhit_ZvsZ[0]->GetYaxis()->SetTitle("Charge - Sec2, if mult==1");
    fh2_trimhit_ZvsZ[0]->GetYaxis()->SetTitleOffset(1.1);
    fh2_trimhit_ZvsZ[0]->GetXaxis()->CenterTitle(true);
    fh2_trimhit_ZvsZ[0]->GetYaxis()->CenterTitle(true);
    fh2_trimhit_ZvsZ[0]->GetXaxis()->SetLabelSize(0.045);
    fh2_trimhit_ZvsZ[0]->GetXaxis()->SetTitleSize(0.045);
    fh2_trimhit_ZvsZ[0]->GetYaxis()->SetLabelSize(0.045);
    fh2_trimhit_ZvsZ[0]->GetYaxis()->SetTitleSize(0.045);
    cTrimHit_ZvsZ->cd(1);
    fh2_trimhit_ZvsZ[0]->Draw("col");

    sprintf(Name1, "ZS3_vs_MaxZ");
    sprintf(Name2, "Z[Sec3] vs MaxZ [Sec1,Sec2]");
    fh2_trimhit_ZvsZ[1] = new TH2F(Name1, Name2, 900, 10, 100, 900, 10, 100);
    fh2_trimhit_ZvsZ[1]->GetXaxis()->SetTitle("Charge - Max(Sec1,Sec2), if mult==1");
    fh2_trimhit_ZvsZ[1]->GetYaxis()->SetTitle("Charge - Sec3, if mult==1");
    fh2_trimhit_ZvsZ[1]->GetYaxis()->SetTitleOffset(1.1);
    fh2_trimhit_ZvsZ[1]->GetXaxis()->CenterTitle(true);
    fh2_trimhit_ZvsZ[1]->GetYaxis()->CenterTitle(true);
    fh2_trimhit_ZvsZ[1]->GetXaxis()->SetLabelSize(0.045);
    fh2_trimhit_ZvsZ[1]->GetXaxis()->SetTitleSize(0.045);
    fh2_trimhit_ZvsZ[1]->GetYaxis()->SetLabelSize(0.045);
    fh2_trimhit_ZvsZ[1]->GetYaxis()->SetTitleSize(0.045);
    cTrimHit_ZvsZ->cd(2);
    fh2_trimhit_ZvsZ[1]->Draw("col");

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
    if (fHitItemsTrim)
    {
        mainfolTrim->Add(cTrimHit_E);
        for (Int_t i = 0; i < fNumSections; i++)
            mainfolTrim->Add(cTrimHit_CorrDep[i]);
        mainfolTrim->Add(cTrimHit_EvsE);
        mainfolTrim->Add(cTrimHit_ZvsZ);
        mainfolTrim->Add(cTrimHit_Z);
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
    // Hit data
    if (fHitItemsTrim)
    {
        for (Int_t i = 0; i < fNumSections; i++)
        {
            fh1_trimhit_Eraw[i]->Reset();
            fh1_trimhit_Ebeta[i]->Reset();
            fh1_trimhit_Edt[i]->Reset();
            fh1_trimhit_Etheta[i]->Reset();
            fh1_trimhit_Z[i]->Reset();
            fh2_trimhit_Ebeta_vs_DT[i]->Reset();
            fh2_trimhit_Edt_vs_DT[i]->Reset();
            fh2_trimhit_Edt_vs_theta[i]->Reset();
            fh2_trimhit_Etheta_vs_theta[i]->Reset();
        }
        for (Int_t i = 0; i < 2; i++)
        {
            fh2_trimhit_EvsE_raw[i]->Reset();
            fh2_trimhit_EvsE_beta[i]->Reset();
            fh2_trimhit_EvsE_dt[i]->Reset();
            fh2_trimhit_EvsE_theta[i]->Reset();
            fh2_trimhit_ZvsZ[i]->Reset();
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
            R3BSofTrimMappedData* mapitem = (R3BSofTrimMappedData*)fMappedItemsTrim->At(ihit);
            if (!mapitem)
                continue;
            fh1_trimmap_Mult[mapitem->GetSecID() - 1]->Fill(mapitem->GetAnodeID());
            mult[mapitem->GetSecID() - 1][mapitem->GetAnodeID() - 1]++;
            // ATTENTION: take into accound only the first hit
            if (mult[mapitem->GetSecID() - 1][mapitem->GetAnodeID() - 1] == 1)
            {
                if (mapitem->GetEnergy() < 8192 && mapitem->GetEnergy() > 0) // Eraw=0 for Tref and Ttrig
                    Eraw[mapitem->GetSecID() - 1][mapitem->GetAnodeID() - 1] = mapitem->GetEnergy();
                Traw[mapitem->GetSecID() - 1][mapitem->GetAnodeID() - 1] = mapitem->GetTime();
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

    // === extract some values from TClonesArray
    Int_t iSec, iAnode;
    Float_t DTaligned[fNumSections], DTdiff;
    Float_t Eraw[fNumSections], Ebeta[fNumSections], Edt[fNumSections], Etheta[fNumSections];
    Float_t Z[fNumSections];
    for (Int_t s = 0; s < fNumSections; s++)
    {
        DTaligned[s] = 0.;
        DTdiff = 0.;
        Eraw[s] = 0.;
        Ebeta[s] = 0.;
        Edt[s] = 0.;
        Etheta[s] = 0.;
        Z[s] = 0.;
    }

    // === CAL data
    if (fCalItemsTrim && fCalItemsTrim->GetEntriesFast() > 0)
    {
        nHits = fCalItemsTrim->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTrimCalData* calitem = (R3BSofTrimCalData*)fCalItemsTrim->At(ihit);
            if (!calitem)
                continue;
            iSec = calitem->GetSecID() - 1;
            iAnode = calitem->GetAnodeID() - 1;
            fh1_trimcal_Esub[iAnode + iSec * fNumAnodes]->Fill(calitem->GetEnergySub());
            fh1_trimcal_Ematch[iAnode + iSec * fNumAnodes]->Fill(calitem->GetEnergyMatch());
            fh1_trimcal_DTraw[iAnode + iSec * fNumAnodes]->Fill(calitem->GetDriftTimeRaw());
            fh1_trimcal_DTalign[iAnode + iSec * fNumAnodes]->Fill(calitem->GetDriftTimeAligned());
            if (iAnode == 1)
                DTaligned[iSec] = (Float_t)calitem->GetDriftTimeAligned();
        }                                                // end of lopp over the cal data
        DTdiff = (Float_t)(DTaligned[1] - DTaligned[0]); // to be changed to DTaligned[2]-DTaligned[0]
    }

    // === HIT data

    if (fHitItemsTrim && fHitItemsTrim->GetEntriesFast() > 0)
    {
        nHits = fHitItemsTrim->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTrimHitData* hititem = (R3BSofTrimHitData*)fHitItemsTrim->At(ihit);
            if (!hititem)
                continue;
            iSec = hititem->GetSecID() - 1;
            Eraw[iSec] = hititem->GetEnergyRaw();
            Ebeta[iSec] = hititem->GetEnergyBeta();
            Edt[iSec] = hititem->GetEnergyDT();
            Etheta[iSec] = hititem->GetEnergyTheta();
            Z[iSec] = hititem->GetZcharge();
            fh1_trimhit_Eraw[iSec]->Fill(Eraw[iSec]);
            fh1_trimhit_Ebeta[iSec]->Fill(Ebeta[iSec]);
            fh1_trimhit_Edt[iSec]->Fill(Edt[iSec]);
            fh1_trimhit_Etheta[iSec]->Fill(Etheta[iSec]);
            fh1_trimhit_Z[iSec]->Fill(Z[iSec]);
            fh2_trimhit_Ebeta_vs_DT[iSec]->Fill(DTaligned[iSec], Ebeta[iSec]);
            fh2_trimhit_Edt_vs_DT[iSec]->Fill(DTaligned[iSec], Edt[iSec]);
            fh2_trimhit_Edt_vs_theta[iSec]->Fill(DTdiff, Edt[iSec]);
            fh2_trimhit_Etheta_vs_theta[iSec]->Fill(DTdiff, Etheta[iSec]);
        } // end of loop over the HitData TClonesArray
        fh2_trimhit_EvsE_raw[0]->Fill(Eraw[0], Eraw[1]);
        fh2_trimhit_EvsE_raw[1]->Fill(TMath::Max(Eraw[0], Eraw[1]), Eraw[2]);
        fh2_trimhit_EvsE_beta[0]->Fill(Ebeta[0], Ebeta[1]);
        fh2_trimhit_EvsE_beta[1]->Fill(TMath::Max(Ebeta[0], Ebeta[1]), Ebeta[2]);
        fh2_trimhit_EvsE_dt[0]->Fill(Edt[0], Edt[1]);
        fh2_trimhit_EvsE_dt[1]->Fill(TMath::Max(Edt[0], Edt[1]), Edt[2]);
        fh2_trimhit_EvsE_theta[0]->Fill(Etheta[0], Etheta[1]);
        fh2_trimhit_EvsE_theta[1]->Fill(TMath::Max(Etheta[0], Etheta[1]), Etheta[2]);
        fh2_trimhit_ZvsZ[0]->Fill(Z[0], Z[1]);
        fh2_trimhit_ZvsZ[1]->Fill(TMath::Max(Z[0], Z[1]), Z[2]);
    }

    fNEvents += 1;
}

void R3BSofTrimOnlineSpectra::FinishEvent()
{
    if (fMappedItemsTrim)
        fMappedItemsTrim->Clear();
    if (fCalItemsTrim)
        fCalItemsTrim->Clear();
    if (fHitItemsTrim)
        fHitItemsTrim->Clear();
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
    if (fHitItemsTrim)
    {
        for (Int_t s = 0; s < fNumSections; s++)
        {
            cTrimHit_CorrDep[s]->Write();
        }
        cTrimHit_E->Write();
        cTrimHit_Z->Write();
        cTrimHit_EvsE->Write();
        cTrimHit_ZvsZ->Write();
    }
}

ClassImp(R3BSofTrimOnlineSpectra)
