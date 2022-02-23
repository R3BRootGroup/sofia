// ------------------------------------------------------------
// -----                  R3BSofAtOnlineSpectra           -----
// -----    Created 29/09/19  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofAtOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BSofAtMappedData.h"
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

R3BSofAtOnlineSpectra::R3BSofAtOnlineSpectra()
    : FairTask("SofAtOnlineSpectra", 1)
    , fMappedItemsAt(NULL)
    , fHitItemsTwim(NULL)
    , fNumAnodes(4)
    , fNEvents(0)
{
    for (Int_t a = 0; a < fNumAnodes; a++)
        fcutg[a] = new TCutG();
}

R3BSofAtOnlineSpectra::R3BSofAtOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fMappedItemsAt(NULL)
    , fHitItemsTwim(NULL)
    , fNumAnodes(4)
    , fNEvents(0)
{
    for (Int_t a = 0; a < fNumAnodes; a++)
        fcutg[a] = new TCutG();
}

R3BSofAtOnlineSpectra::~R3BSofAtOnlineSpectra()
{
    LOG(INFO) << "R3BSofAtOnlineSpectra::Delete instance";
    if (fMappedItemsAt)
        delete fMappedItemsAt;
    if (fHitItemsTwim)
        delete fHitItemsTwim;
}

InitStatus R3BSofAtOnlineSpectra::Init()
{
    LOG(INFO) << "R3BSofAtOnlineSpectra::Init ";

    // Try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofAtOnlineSpectra::Init FairRootManager not found";
    header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // Get access to mapped data of the active target
    fMappedItemsAt = (TClonesArray*)mgr->GetObject("AtMappedData");
    if (!fMappedItemsAt)
    {
        LOG(ERROR) << "R3BSofAtOnlineSpectra::AtMappedData not found";
        return kFATAL;
    }

    // Get access to hit data of the twin music
    fHitItemsTwim = (TClonesArray*)mgr->GetObject("TwimHitData");
    if (!fHitItemsTwim)
        LOG(WARNING) << "R3BSofAtOnlineSpectra::TwimHitData not found";

    // Create histograms
    char Name1[255];
    char Name2[255];

    // MAPPED DATA - MULTIPLICITY
    cAtMap_mult = new TCanvas("At_mult", "Active Target - multiplicity", 10, 10, 800, 700);
    cAtMap_mult->Divide(2, 2);

    fh1_atmap_mult = new TH1F("At_SumMult", "AT - MULTIPLICITY", fNumAnodes + 1, -0.5, fNumAnodes + 0.5);
    fh1_atmap_mult->GetXaxis()->SetTitle("Anode (1-based)");
    fh1_atmap_mult->GetYaxis()->SetTitle("Total Multiplicity");
    fh1_atmap_mult->GetYaxis()->SetTitleOffset(1.1);
    fh1_atmap_mult->GetXaxis()->CenterTitle(true);
    fh1_atmap_mult->GetYaxis()->CenterTitle(true);
    fh1_atmap_mult->GetXaxis()->SetLabelSize(0.045);
    fh1_atmap_mult->GetXaxis()->SetTitleSize(0.045);
    fh1_atmap_mult->GetYaxis()->SetLabelSize(0.045);
    fh1_atmap_mult->GetYaxis()->SetTitleSize(0.045);
    fh1_atmap_mult->SetLineColor(kBlue);
    cAtMap_mult->cd(1);
    fh1_atmap_mult->Draw("");

    fh1_atmap_mult_wo_pu =
        new TH1F("At_SumMult_wo_pu", "AT - MULTIPLICITY - pile up rejection", fNumAnodes + 1, -0.5, fNumAnodes + 0.5);
    fh1_atmap_mult_wo_pu->GetXaxis()->SetTitle("Anode (1-based)");
    fh1_atmap_mult_wo_pu->GetYaxis()->SetTitle("Total Multiplicity");
    fh1_atmap_mult_wo_pu->GetYaxis()->SetTitleOffset(1.1);
    fh1_atmap_mult_wo_pu->GetXaxis()->CenterTitle(true);
    fh1_atmap_mult_wo_pu->GetYaxis()->CenterTitle(true);
    fh1_atmap_mult_wo_pu->GetXaxis()->SetLabelSize(0.045);
    fh1_atmap_mult_wo_pu->GetXaxis()->SetTitleSize(0.045);
    fh1_atmap_mult_wo_pu->GetYaxis()->SetLabelSize(0.045);
    fh1_atmap_mult_wo_pu->GetYaxis()->SetTitleSize(0.045);
    fh1_atmap_mult_wo_pu->SetLineColor(kBlue);
    cAtMap_mult->cd(2);
    fh1_atmap_mult_wo_pu->Draw("");

    fh2_atmap_mult = new TH2F("At_MultVsAnode",
                              "Active Target - MultPerEvent vs Anodes",
                              fNumAnodes + 1,
                              -0.5,
                              fNumAnodes + 0.5,
                              10,
                              -0.5,
                              9.5);
    fh2_atmap_mult->GetXaxis()->SetTitle("Anode (1-based)");
    fh2_atmap_mult->GetYaxis()->SetTitle("Multiplicity Per Event");
    fh2_atmap_mult->GetYaxis()->SetTitleOffset(1.1);
    fh2_atmap_mult->GetXaxis()->CenterTitle(true);
    fh2_atmap_mult->GetYaxis()->CenterTitle(true);
    fh2_atmap_mult->GetXaxis()->SetLabelSize(0.045);
    fh2_atmap_mult->GetXaxis()->SetTitleSize(0.045);
    fh2_atmap_mult->GetYaxis()->SetLabelSize(0.045);
    fh2_atmap_mult->GetYaxis()->SetTitleSize(0.045);
    cAtMap_mult->cd(3);
    fh2_atmap_mult->Draw("colz");

    fh2_atmap_mult_wo_pu = new TH2F("At_MultVsAnode_wo_pu",
                                    "Active Target - MultPerEvent vs Anodes - pile up rejection",
                                    fNumAnodes + 1,
                                    -0.5,
                                    fNumAnodes + 0.5,
                                    10,
                                    -0.5,
                                    9.5);
    fh2_atmap_mult_wo_pu->GetXaxis()->SetTitle("Anode (1-based)");
    fh2_atmap_mult_wo_pu->GetYaxis()->SetTitle("Multiplicity Per Event");
    fh2_atmap_mult_wo_pu->GetYaxis()->SetTitleOffset(1.1);
    fh2_atmap_mult_wo_pu->GetXaxis()->CenterTitle(true);
    fh2_atmap_mult_wo_pu->GetYaxis()->CenterTitle(true);
    fh2_atmap_mult_wo_pu->GetXaxis()->SetLabelSize(0.045);
    fh2_atmap_mult_wo_pu->GetXaxis()->SetTitleSize(0.045);
    fh2_atmap_mult_wo_pu->GetYaxis()->SetLabelSize(0.045);
    fh2_atmap_mult_wo_pu->GetYaxis()->SetTitleSize(0.045);
    cAtMap_mult->cd(4);
    fh2_atmap_mult_wo_pu->Draw("colz");

    // MAPPED 1D DATA - ENERGY
    cAtMap_E = new TCanvas("At_E", "at_E", 10, 10, 800, 700);
    cAtMap_E->Divide(1, fNumAnodes);
    fh1_atmap_E = new TH1F*[fNumAnodes];
    fh1_atmap_E_mult1_wo_pu_ov = new TH1F*[fNumAnodes];
    for (Int_t a = 0; a < fNumAnodes; a++)
    {
        sprintf(Name1, "AT_E_anode%d", a + 1);
        sprintf(Name2, "AT - Energy - Anode %d", a + 1);
        fh1_atmap_E[a] = new TH1F(Name1, Name2, 65000, 0., 65000); // MDPP16 on 16 bits
        fh1_atmap_E[a]->GetXaxis()->SetTitle("Raw Energy [channels]");
        fh1_atmap_E[a]->GetYaxis()->SetTitle("Counts");
        fh1_atmap_E[a]->GetYaxis()->SetTitleOffset(1.1);
        fh1_atmap_E[a]->GetXaxis()->CenterTitle(true);
        fh1_atmap_E[a]->GetYaxis()->CenterTitle(true);
        fh1_atmap_E[a]->GetXaxis()->SetLabelSize(0.045);
        fh1_atmap_E[a]->GetXaxis()->SetTitleSize(0.045);
        fh1_atmap_E[a]->GetYaxis()->SetLabelSize(0.045);
        fh1_atmap_E[a]->GetYaxis()->SetTitleSize(0.045);
        fh1_atmap_E[a]->SetLineColor(kBlue);
        cAtMap_E->cd(a + 1);
        fh1_atmap_E[a]->Draw("");

        sprintf(Name1, "AT_E_m1_anode%d", a + 1);
        sprintf(Name2, "AT - Energy - Anode %d - mult=1, pu and ov rejection", a + 1);
        fh1_atmap_E_mult1_wo_pu_ov[a] = new TH1F(Name1, Name2, 65000, 0., 65000); // MDPP16 on 16 bits
        fh1_atmap_E_mult1_wo_pu_ov[a]->GetXaxis()->SetTitle("Raw Energy [channels]");
        fh1_atmap_E_mult1_wo_pu_ov[a]->GetYaxis()->SetTitle("Counts");
        fh1_atmap_E_mult1_wo_pu_ov[a]->GetYaxis()->SetTitleOffset(1.1);
        fh1_atmap_E_mult1_wo_pu_ov[a]->GetXaxis()->CenterTitle(true);
        fh1_atmap_E_mult1_wo_pu_ov[a]->GetYaxis()->CenterTitle(true);
        fh1_atmap_E_mult1_wo_pu_ov[a]->GetXaxis()->SetLabelSize(0.045);
        fh1_atmap_E_mult1_wo_pu_ov[a]->GetXaxis()->SetTitleSize(0.045);
        fh1_atmap_E_mult1_wo_pu_ov[a]->GetYaxis()->SetLabelSize(0.045);
        fh1_atmap_E_mult1_wo_pu_ov[a]->GetYaxis()->SetTitleSize(0.045);
        fh1_atmap_E_mult1_wo_pu_ov[a]->SetLineColor(kRed);
        cAtMap_E->cd(a + 1);
        fh1_atmap_E_mult1_wo_pu_ov[a]->Draw("sames");
    }

    // MAPPED 2D DATA - ENERGY
    cAtMap_EvsE = new TCanvas("At_EvsE", "at_EvsE", 10, 10, 800, 700);
    cAtMap_EvsE->Divide(2, 2);
    fh2_atmap_EvsE = new TH2F*[fNumAnodes];
    for (Int_t a = 0; a < fNumAnodes - 1; a++)
    {
        sprintf(Name1, "AT_E_anode%d_vs_E_anode%d", a + 2, a + 1);
        sprintf(Name2, "AT - Energy - Anode %d vs Anode %d", a + 2, a + 1);
        fh2_atmap_EvsE[a] = new TH2F(Name1, Name2, 650, 0., 65000, 650, 0., 65000); // MDPP16 on 16 bits
        fh2_atmap_EvsE[a]->GetXaxis()->SetTitle("Raw Energy [channels]");
        fh2_atmap_EvsE[a]->GetYaxis()->SetTitle("Raw Energy [channels]");
        fh2_atmap_EvsE[a]->GetYaxis()->SetTitleOffset(1.1);
        fh2_atmap_EvsE[a]->GetXaxis()->CenterTitle(true);
        fh2_atmap_EvsE[a]->GetYaxis()->CenterTitle(true);
        fh2_atmap_EvsE[a]->GetXaxis()->SetLabelSize(0.045);
        fh2_atmap_EvsE[a]->GetXaxis()->SetTitleSize(0.045);
        fh2_atmap_EvsE[a]->GetYaxis()->SetLabelSize(0.045);
        fh2_atmap_EvsE[a]->GetYaxis()->SetTitleSize(0.045);
        cAtMap_EvsE->cd(a + 1);
        fh2_atmap_EvsE[a]->Draw("col");
        if (fcutg[a]->GetN() > 0)
            fcutg[a]->Draw("same");
    }

    sprintf(Name1, "AT_E_anode%d_vs_E_anode%d", fNumAnodes, 1);
    sprintf(Name2, "AT - Energy - Anode %d vs Anode %d", fNumAnodes, 1);
    fh2_atmap_EvsE[fNumAnodes - 1] = new TH2F(Name1, Name2, 650, 0., 65000, 650, 0., 65000); // MDPP16 on 16 bits
    fh2_atmap_EvsE[fNumAnodes - 1]->GetXaxis()->SetTitle("Raw Energy [channels]");
    fh2_atmap_EvsE[fNumAnodes - 1]->GetYaxis()->SetTitle("Raw Energy [channels]");
    fh2_atmap_EvsE[fNumAnodes - 1]->GetYaxis()->SetTitleOffset(1.1);
    fh2_atmap_EvsE[fNumAnodes - 1]->GetXaxis()->CenterTitle(true);
    fh2_atmap_EvsE[fNumAnodes - 1]->GetYaxis()->CenterTitle(true);
    fh2_atmap_EvsE[fNumAnodes - 1]->GetXaxis()->SetLabelSize(0.045);
    fh2_atmap_EvsE[fNumAnodes - 1]->GetXaxis()->SetTitleSize(0.045);
    fh2_atmap_EvsE[fNumAnodes - 1]->GetYaxis()->SetLabelSize(0.045);
    fh2_atmap_EvsE[fNumAnodes - 1]->GetYaxis()->SetTitleSize(0.045);
    cAtMap_EvsE->cd(fNumAnodes);
    fh2_atmap_EvsE[fNumAnodes - 1]->Draw("col");

    // MAPPED 2D DATA - ENERGY  MULT == 1 && PU KFALSE
    cAtMap_EvsE_mult1_nopu = new TCanvas("At_EvsE_mult1_nopu", "At_EvsE_mult1_nopu", 10, 10, 800, 700);
    cAtMap_EvsE_mult1_nopu->Divide(2, 2);
    fh2_atmap_EvsE_mult1_nopu = new TH2F*[fNumAnodes];
    for (Int_t a = 0; a < fNumAnodes - 1; a++)
    {
        sprintf(Name1, "AT_E_anode%d_vs_E_anode%d_mult1_nopu", a + 2, a + 1);
        sprintf(Name2, "AT - Energy - Anode %d vs Anode %d_mult1_nopu", a + 2, a + 1);
        fh2_atmap_EvsE_mult1_nopu[a] = new TH2F(Name1, Name2, 650, 0., 65000, 650, 0., 65000); // MDPP16 on 16 bits
        fh2_atmap_EvsE_mult1_nopu[a]->GetXaxis()->SetTitle("Raw Energy [channels]");
        fh2_atmap_EvsE_mult1_nopu[a]->GetYaxis()->SetTitle("Raw Energy [channels]");
        fh2_atmap_EvsE_mult1_nopu[a]->GetYaxis()->SetTitleOffset(1.1);
        fh2_atmap_EvsE_mult1_nopu[a]->GetXaxis()->CenterTitle(true);
        fh2_atmap_EvsE_mult1_nopu[a]->GetYaxis()->CenterTitle(true);
        fh2_atmap_EvsE_mult1_nopu[a]->GetXaxis()->SetLabelSize(0.045);
        fh2_atmap_EvsE_mult1_nopu[a]->GetXaxis()->SetTitleSize(0.045);
        fh2_atmap_EvsE_mult1_nopu[a]->GetYaxis()->SetLabelSize(0.045);
        fh2_atmap_EvsE_mult1_nopu[a]->GetYaxis()->SetTitleSize(0.045);
        cAtMap_EvsE_mult1_nopu->cd(a + 1);
        fh2_atmap_EvsE_mult1_nopu[a]->Draw("col");
    }
    /*
        sprintf(Name1, "AT_E_anode%d_vs_E_anode%d", fNumAnodes, 1);
        sprintf(Name2, "AT - Energy - Anode %d vs Anode %d", fNumAnodes, 1);
        fh2_atmap_EvsE[fNumAnodes - 1] = new TH2F(Name1, Name2, 650, 0., 65000, 650, 0., 65000); // MDPP16 on 16 bits
        fh2_atmap_EvsE[fNumAnodes - 1]->GetXaxis()->SetTitle("Raw Energy [channels]");
        fh2_atmap_EvsE[fNumAnodes - 1]->GetYaxis()->SetTitle("Raw Energy [channels]");
        fh2_atmap_EvsE[fNumAnodes - 1]->GetYaxis()->SetTitleOffset(1.1);
        fh2_atmap_EvsE[fNumAnodes - 1]->GetXaxis()->CenterTitle(true);
        fh2_atmap_EvsE[fNumAnodes - 1]->GetYaxis()->CenterTitle(true);
        fh2_atmap_EvsE[fNumAnodes - 1]->GetXaxis()->SetLabelSize(0.045);
        fh2_atmap_EvsE[fNumAnodes - 1]->GetXaxis()->SetTitleSize(0.045);
        fh2_atmap_EvsE[fNumAnodes - 1]->GetYaxis()->SetLabelSize(0.045);
        fh2_atmap_EvsE[fNumAnodes - 1]->GetYaxis()->SetTitleSize(0.045);
        cAtMap_EvsE->cd(fNumAnodes);
        fh2_atmap_EvsE[fNumAnodes - 1]->Draw("col");
    */
    // MAPPED 2D DATA - ENERGY  MULT == 1 && PU KFALSE
    /*    cAtMap_EvsE_mult1_nopu = new TCanvas("at_EvsE_mult1_nopu", "at_EvsE_mult1_nopu", 10, 10, 800, 700);
        cAtMap_EvsE_mult1_nopu->Divide(2, 2);
        fh2_atmap_EvsE_mult1_nopu = new TH2F*[fNumAnodes];
        for (Int_t a = 0; a < fNumAnodes - 1; a++)
        {
            sprintf(Name1, "AT_E_anode%d_vs_E_anode%d_mult1_nopu", a + 2, a + 1);
            sprintf(Name2, "AT - Energy - Anode %d vs Anode %d_mult1_nopu", a + 2, a + 1);
            fh2_atmap_EvsE_mult1_nopu[a] = new TH2F(Name1, Name2, 650, 0., 65000, 650, 0., 65000); // MDPP16 on 16 bits
            fh2_atmap_EvsE_mult1_nopu[a]->GetXaxis()->SetTitle("Raw Energy [channels]");
            fh2_atmap_EvsE_mult1_nopu[a]->GetYaxis()->SetTitle("Raw Energy [channels]");
            fh2_atmap_EvsE_mult1_nopu[a]->GetYaxis()->SetTitleOffset(1.1);
            fh2_atmap_EvsE_mult1_nopu[a]->GetXaxis()->CenterTitle(true);
            fh2_atmap_EvsE_mult1_nopu[a]->GetYaxis()->CenterTitle(true);
            fh2_atmap_EvsE_mult1_nopu[a]->GetXaxis()->SetLabelSize(0.045);
            fh2_atmap_EvsE_mult1_nopu[a]->GetXaxis()->SetTitleSize(0.045);
            fh2_atmap_EvsE_mult1_nopu[a]->GetYaxis()->SetLabelSize(0.045);
            fh2_atmap_EvsE_mult1_nopu[a]->GetYaxis()->SetTitleSize(0.045);
            cAtMap_EvsE_mult1_nopu->cd(a + 1);
            fh2_atmap_EvsE_mult1_nopu[a]->Draw("col");
        }
    */
    sprintf(Name1, "AT_E_anode%d_vs_E_anode%d_mult1_nopu", fNumAnodes, 1);
    sprintf(Name2, "AT - Energy - Anode %d vs Anode %d_mult1_nopu", fNumAnodes, 1);
    fh2_atmap_EvsE_mult1_nopu[fNumAnodes - 1] =
        new TH2F(Name1, Name2, 650, 0., 65000, 650, 0., 65000); // MDPP16 on 16 bits
    fh2_atmap_EvsE_mult1_nopu[fNumAnodes - 1]->GetXaxis()->SetTitle("Raw Energy [channels]");
    fh2_atmap_EvsE_mult1_nopu[fNumAnodes - 1]->GetYaxis()->SetTitle("Raw Energy [channels]");
    fh2_atmap_EvsE_mult1_nopu[fNumAnodes - 1]->GetYaxis()->SetTitleOffset(1.1);
    fh2_atmap_EvsE_mult1_nopu[fNumAnodes - 1]->GetXaxis()->CenterTitle(true);
    fh2_atmap_EvsE_mult1_nopu[fNumAnodes - 1]->GetYaxis()->CenterTitle(true);
    fh2_atmap_EvsE_mult1_nopu[fNumAnodes - 1]->GetXaxis()->SetLabelSize(0.045);
    fh2_atmap_EvsE_mult1_nopu[fNumAnodes - 1]->GetXaxis()->SetTitleSize(0.045);
    fh2_atmap_EvsE_mult1_nopu[fNumAnodes - 1]->GetYaxis()->SetLabelSize(0.045);
    fh2_atmap_EvsE_mult1_nopu[fNumAnodes - 1]->GetYaxis()->SetTitleSize(0.045);
    cAtMap_EvsE_mult1_nopu->cd(fNumAnodes);
    fh2_atmap_EvsE_mult1_nopu[fNumAnodes - 1]->Draw("col");

    // Correlations with the twin music charge
    cTwimZs = new TCanvas*[fNumAnodes - 1];
    fh1_Twimhit_Zl = new TH1F*[fNumAnodes - 1];
    fh1_Twimhit_Zr = new TH1F*[fNumAnodes - 1];
    fh2_Twimhit_ZrZl = new TH2F*[fNumAnodes - 1];
    for (Int_t i = 0; i < fNumAnodes - 1; i++)
    {
        sprintf(Name1, "Twim_ZL_vs_ZR_SecAt_%d", i + 1);
        cTwimZs[i] = new TCanvas(Name1, "", 10, 10, 800, 700);
        cTwimZs[i]->Divide(2, 1);
        cTwimZs[i]->cd(1);
        sprintf(Name1, "fh1_Twim_zl_SecAt_%d", i + 1);
        sprintf(Name2, "Twim: ZL(blue) vs ZR(red) for section %d in AT", i + 1);
        fh1_Twimhit_Zl[i] = new TH1F(Name1, Name2, 1500, 1, 98);
        fh1_Twimhit_Zl[i]->GetXaxis()->SetTitle("Z [atomic number]");
        fh1_Twimhit_Zl[i]->GetYaxis()->SetTitle("Counts");
        fh1_Twimhit_Zl[i]->GetYaxis()->SetTitleOffset(1.1);
        fh1_Twimhit_Zl[i]->GetXaxis()->CenterTitle(true);
        fh1_Twimhit_Zl[i]->GetYaxis()->CenterTitle(true);
        fh1_Twimhit_Zl[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_Twimhit_Zl[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_Twimhit_Zl[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_Twimhit_Zl[i]->GetYaxis()->SetTitleSize(0.045);
        fh1_Twimhit_Zl[i]->SetLineColor(4);
        fh1_Twimhit_Zl[i]->Draw("");
        sprintf(Name1, "fh1_Twim_zr_SecAt_%d", i + 1);
        fh1_Twimhit_Zr[i] = new TH1F(Name1, "Twim: ZR", 1500, 1, 98);
        fh1_Twimhit_Zr[i]->SetLineColor(2);
        fh1_Twimhit_Zr[i]->Draw("same");
        cTwimZs[i]->cd(2);
        sprintf(Name1, "fh2_Twim_zlzr_SecAt_%d", i + 1);
        sprintf(Name2, "Twim: ZL vs ZR for section %d in AT", i + 1);
        fh2_Twimhit_ZrZl[i] = new TH2F(Name1, Name2, 1500, 1, 98, 1500, 1, 98);
        fh2_Twimhit_ZrZl[i]->GetXaxis()->SetTitle("Charge ZL");
        fh2_Twimhit_ZrZl[i]->GetYaxis()->SetTitle("Charge ZR");
        fh2_Twimhit_ZrZl[i]->GetYaxis()->SetTitleOffset(1.1);
        fh2_Twimhit_ZrZl[i]->GetXaxis()->CenterTitle(true);
        fh2_Twimhit_ZrZl[i]->GetYaxis()->CenterTitle(true);
        fh2_Twimhit_ZrZl[i]->GetXaxis()->SetLabelSize(0.045);
        fh2_Twimhit_ZrZl[i]->GetXaxis()->SetTitleSize(0.045);
        fh2_Twimhit_ZrZl[i]->GetYaxis()->SetLabelSize(0.045);
        fh2_Twimhit_ZrZl[i]->GetYaxis()->SetTitleSize(0.045);
        fh2_Twimhit_ZrZl[i]->Draw("colz");
    }

    cTwimZsum = new TCanvas("At_Twim_Zsum", "ZSum correlated to At sections", 10, 10, 800, 700);
    fh1_twim_ZSum[0] = new TH1F("fh1_Twim_ZSum_Atsec1", "Twim: ZL+ZR with AT conditions", 800, 60, 100);
    fh1_twim_ZSum[0]->GetXaxis()->SetTitle("Fissioning system --> Z [atomic number]");
    fh1_twim_ZSum[0]->GetYaxis()->SetTitle("Counts");
    fh1_twim_ZSum[0]->GetYaxis()->SetTitleOffset(1.1);
    fh1_twim_ZSum[0]->GetXaxis()->CenterTitle(true);
    fh1_twim_ZSum[0]->GetYaxis()->CenterTitle(true);
    fh1_twim_ZSum[0]->GetXaxis()->SetLabelSize(0.045);
    fh1_twim_ZSum[0]->GetXaxis()->SetTitleSize(0.045);
    fh1_twim_ZSum[0]->GetYaxis()->SetLabelSize(0.045);
    fh1_twim_ZSum[0]->GetYaxis()->SetTitleSize(0.045);
    fh1_twim_ZSum[0]->SetLineColor(4);
    fh1_twim_ZSum[0]->Draw("");
    fh1_twim_ZSum[1] = new TH1F("fh1_Twim_ZSum_Atsec2", "", 800, 60, 100);
    fh1_twim_ZSum[1]->SetLineColor(2);
    fh1_twim_ZSum[1]->Draw("same");
    fh1_twim_ZSum[2] = new TH1F("fh1_Twim_ZSum_Atsec3", "", 800, 60, 100);
    fh1_twim_ZSum[2]->SetLineColor(3);
    fh1_twim_ZSum[2]->Draw("same");

    TLegend* leg = new TLegend(0.05, 0.9, 0.39, 0.9999, NULL, "brNDC");
    leg->SetBorderSize(0);
    leg->SetTextFont(62);
    leg->SetTextSize(0.03);
    leg->SetLineColor(1);
    leg->SetLineStyle(1);
    leg->SetLineWidth(1);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    TLegendEntry* entry = leg->AddEntry("null", "AT Section 1", "l");
    entry->SetLineColor(4);
    entry->SetLineStyle(1);
    entry->SetLineWidth(3);
    entry->SetTextFont(62);
    entry = leg->AddEntry("null", "AT Section 2", "l");
    entry->SetLineColor(2);
    entry->SetLineStyle(1);
    entry->SetLineWidth(3);
    entry->SetTextFont(62);
    entry = leg->AddEntry("null", "AT Section 3", "l");
    entry->SetLineColor(3);
    entry->SetLineStyle(1);
    entry->SetLineWidth(3);
    entry->SetTextFont(62);
    leg->Draw();

    // Mapped data folder for the Active Target
    TFolder* MapfolAt = new TFolder("Map", "At mapped info");
    if (fMappedItemsAt)
    {
        MapfolAt->Add(cAtMap_mult);
        MapfolAt->Add(cAtMap_E);
        MapfolAt->Add(cAtMap_EvsE);
        MapfolAt->Add(cAtMap_EvsE_mult1_nopu);
    }

    // Hit data folder for the twin music correlated with the AT
    TFolder* folAtvsTwim = new TFolder("At_vs_Twim", "At vs twim info");
    for (Int_t i = 0; i < fNumAnodes - 1; i++)
    {
        folAtvsTwim->Add(cTwimZs[i]);
    }
    folAtvsTwim->Add(cTwimZsum);

    // MAIN FOLDER-AT
    TFolder* mainfolAt = new TFolder("ActiveTarget", "At info");
    mainfolAt->Add(MapfolAt);
    if (fHitItemsTwim)
        mainfolAt->Add(folAtvsTwim);

    run->AddObject(mainfolAt);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_ActiveTarget_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofAtOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofAtOnlineSpectra::Reset_Histo";

    // Mapped Data
    if (fMappedItemsAt)
    {
        fh1_atmap_mult->Reset();
        fh1_atmap_mult_wo_pu->Reset();
        fh2_atmap_mult->Reset();
        fh2_atmap_mult_wo_pu->Reset();
        for (Int_t a = 0; a < fNumAnodes; a++)
        {
            fh1_atmap_E[a]->Reset();
            fh1_atmap_E_mult1_wo_pu_ov[a]->Reset();
            fh2_atmap_EvsE[a]->Reset();
            fh2_atmap_EvsE_mult1_nopu[a]->Reset();
        }
    }
    // Hit twim Data
    if (fHitItemsTwim)
        for (Int_t i = 0; i < fNumAnodes - 1; i++)
        {
            fh1_Twimhit_Zl[i]->Reset();
            fh1_Twimhit_Zr[i]->Reset();
            fh2_Twimhit_ZrZl[i]->Reset();
            fh1_twim_ZSum[i]->Reset();
        }
}

void R3BSofAtOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofAtOnlineSpectra::Exec FairRootManager not found";

    Int_t nHits;
    if (fMappedItemsAt && fMappedItemsAt->GetEntriesFast() > 0)
    {

        // local variables
        Int_t E[fNumAnodes];
        Int_t mult[fNumAnodes];
        Bool_t pu[fNumAnodes];
        Bool_t ov[fNumAnodes];
        Int_t iAnode;

        // initialization of these variables
        for (Int_t a = 0; a < fNumAnodes; a++)
        {
            E[a] = -1;
            mult[a] = 0;
            pu[a] = kFALSE;
            ov[a] = kFALSE;
        }

        // number of entries per event in the AtMappedData TClonesArray
        nHits = fMappedItemsAt->GetEntriesFast();

        // loop over the entries
        for (Int_t entry = 0; entry < nHits; entry++)
        {
            // get the mapped data
            R3BSofAtMappedData* map = (R3BSofAtMappedData*)fMappedItemsAt->At(entry);
            if (!map)
                continue;
            iAnode = map->GetAnodeID() - 1; // iAnode is 0-based
            E[iAnode] = map->GetEnergy();   // outside of the loop --> last entry if mult>1
            if (map->GetPileupStatus() == kTRUE)
                pu[iAnode] = kTRUE; // at least one entry has pile up
            if (map->GetOverflowStatus() == kTRUE)
                ov[iAnode] = kTRUE; // at least one entry has overflow
            mult[iAnode]++;
            fh1_atmap_mult->Fill(iAnode + 1);
            fh1_atmap_E[iAnode]->Fill(E[iAnode]);
        } // end of loop over the Mapped data

        for (Int_t a = 0; a < fNumAnodes - 1; a++)
        {
            if (mult[a] == 1 && mult[a + 1] == 1)
            {
                fh2_atmap_EvsE[a]->Fill(E[a], E[a + 1]);
            }
        }
        if (mult[0] == 1 && mult[fNumAnodes - 1] == 1)
        { //      x ,  y
            fh2_atmap_EvsE[fNumAnodes - 1]->Fill(E[0], E[fNumAnodes - 1]);
        }

        // ajout gui
        for (Int_t a = 0; a < fNumAnodes - 1; a++)
        {
            if (mult[a] == 1 && mult[a + 1] == 1 && pu[a] == kFALSE && pu[a + 1] == kFALSE)
            {
                fh2_atmap_EvsE_mult1_nopu[a]->Fill(E[a], E[a + 1]);
            }
        }
        if (mult[0] == 1 && mult[fNumAnodes - 1] == 1 && pu[0] == kFALSE && pu[fNumAnodes - 1] == kFALSE)
        { //      x ,  y
            fh2_atmap_EvsE_mult1_nopu[fNumAnodes - 1]->Fill(E[0], E[fNumAnodes - 1]);
        }

        for (Int_t a = 0; a < fNumAnodes; a++)
        {
            if (mult[a] > 0)
            {
                fh2_atmap_mult->Fill(a + 1, mult[a]);
                if (pu[a] == kFALSE)
                { // no entry is flaged with the pile-up bit
                    fh1_atmap_mult_wo_pu->Fill(a + 1);
                    fh2_atmap_mult_wo_pu->Fill(a + 1, mult[a]);
                }
            }
            if (mult[a] == 1 && pu[a] == kFALSE && ov[a] == kFALSE)
            {
                fh1_atmap_E_mult1_wo_pu_ov[a]->Fill(E[a]);
            }
        }

        // Fill twin-music data only if we have data for the active target
        for (Int_t a = 0; a < fNumAnodes - 1; a++)
        {
            if (mult[a] == 1 && mult[a + 1] == 1 && pu[a] == kFALSE && pu[a + 1] == kFALSE)
            {
                if (fHitItemsTwim && fHitItemsTwim->GetEntriesFast() > 0 && fcutg[a]->IsInside(E[a], E[a + 1]))
                {
                    nHits = fHitItemsTwim->GetEntriesFast();
                    Float_t zr = 0., zl = 0.;
                    for (Int_t ihit = 0; ihit < nHits; ihit++)
                    {
                        R3BTwimHitData* hit = (R3BTwimHitData*)fHitItemsTwim->At(ihit);
                        if (!hit)
                            continue;
                        // FIXME: this is defined only for the experiment 4-march-2021
                        if (hit->GetSecID() == 0 || hit->GetSecID() == 1)
                            zl = hit->GetZcharge();
                        else if (hit->GetSecID() == 2 || hit->GetSecID() == 3)
                            zr = hit->GetZcharge();
                    }

                    if (zl > 0.)
                        fh1_Twimhit_Zl[a]->Fill(zl);
                    if (zr > 0.)
                        fh1_Twimhit_Zr[a]->Fill(zr);

                    if (zr > 0. && zl > 0.)
                    {
                        fh2_Twimhit_ZrZl[a]->Fill(zl, zr);
                        fh1_twim_ZSum[a]->Fill(zl + zr);
                    }
                }
            }
        }

    } // end of if(MappedData)

    fNEvents += 1;
}

void R3BSofAtOnlineSpectra::FinishEvent()
{
    if (fMappedItemsAt)
        fMappedItemsAt->Clear();
    if (fHitItemsTwim)
        fHitItemsTwim->Clear();
}

void R3BSofAtOnlineSpectra::FinishTask()
{
    if (fMappedItemsAt)
    {
        cAtMap_mult->Write();
        cAtMap_E->Write();
        cAtMap_EvsE->Write();
        cAtMap_EvsE_mult1_nopu->Write();
    }
    if (fHitItemsTwim)
    {
        for (Int_t i = 0; i < fNumAnodes - 1; i++)
            cTwimZs[i]->Write();
        cTwimZsum->Write();
    }
}

ClassImp(R3BSofAtOnlineSpectra)
