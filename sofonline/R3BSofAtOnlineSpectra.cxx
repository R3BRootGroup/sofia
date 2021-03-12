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

R3BSofAtOnlineSpectra::R3BSofAtOnlineSpectra()
    : FairTask("SofAtOnlineSpectra", 1)
    , fMappedItemsAt(NULL)
    , fNumAnodes(4)
    , fNEvents(0)
{
}

R3BSofAtOnlineSpectra::R3BSofAtOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fMappedItemsAt(NULL)
    , fNumAnodes(4)
    , fNEvents(0)
{
}

R3BSofAtOnlineSpectra::~R3BSofAtOnlineSpectra()
{
    LOG(INFO) << "R3BSofAtOnlineSpectra::Delete instance";
    if (fMappedItemsAt)
        delete fMappedItemsAt;
}

InitStatus R3BSofAtOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofAtOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofAtOnlineSpectra::Init FairRootManager not found";
    header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // === get access to mapped data of the active target === //
    fMappedItemsAt = (TClonesArray*)mgr->GetObject("AtMappedData");
    if (!fMappedItemsAt)
    {
        return kFATAL;
    }

    // === Create histograms === //
    char Name1[255];
    char Name2[255];

    // MAPPED DATA - MULTIPLICITY
    cAtMap_mult = new TCanvas("at_mult", "Active Target - multiplicity", 10, 10, 800, 700);
    cAtMap_mult->Divide(2, 2);

    fh1_atmap_mult = new TH1F("at_SumMult", "AT - MULTIPLICITY", fNumAnodes + 1, -0.5, fNumAnodes + 0.5);
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
        new TH1F("at_SumMult_wo_pu", "AT - MULTIPLICITY - pile up rejection", fNumAnodes + 1, -0.5, fNumAnodes + 0.5);
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

    fh2_atmap_mult = new TH2F("at_MultVsAnode",
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

    fh2_atmap_mult_wo_pu = new TH2F("at_MultVsAnode_wo_pu",
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
    cAtMap_E = new TCanvas("at_E", "at_E", 10, 10, 800, 700);
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
    cAtMap_EvsE = new TCanvas("at_EvsE", "at_EvsE", 10, 10, 800, 700);
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
    cAtMap_EvsE_mult1_nopu = new TCanvas("at_EvsE_mult1_nopu", "at_EvsE_mult1_nopu", 10, 10, 800, 700);
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

    // === MAIN FOLDER-AT === //

    TFolder* mainfolAt = new TFolder("ActiveTarget", "At info");
    if (fMappedItemsAt)
    {
        mainfolAt->Add(cAtMap_mult);
        mainfolAt->Add(cAtMap_E);
        mainfolAt->Add(cAtMap_EvsE);
        mainfolAt->Add(cAtMap_EvsE_mult1_nopu);
    }
    run->AddObject(mainfolAt);

    // === Register command to reset histograms === /
    run->GetHttpServer()->RegisterCommand("Reset_AT_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

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
            if (mult[a] > 0 && mult[a + 1] > 0)
            {
                fh2_atmap_EvsE[a]->Fill(E[a], E[a + 1]);
            }
        }
        if (mult[0] > 0 && mult[fNumAnodes - 1] > 0)
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
        if (mult[0] > 0 && mult[fNumAnodes - 1] > 0)
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
    } // end of if(MappedData)

    fNEvents += 1;
}

void R3BSofAtOnlineSpectra::FinishEvent()
{

    if (fMappedItemsAt)
    {
        fMappedItemsAt->Clear();
    }
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
}

ClassImp(R3BSofAtOnlineSpectra)
