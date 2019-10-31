// ------------------------------------------------------------
// -----                  R3BSofMwpcOnlineSpectra         -----
// -----    Created 29/09/19  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofMwpcOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BSofMwpcCalData.h"
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
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>

using namespace std;

R3BSofMwpcOnlineSpectra::R3BSofMwpcOnlineSpectra()
    : FairTask("SofMwpcOnlineSpectra", 1)
    , fCalItemsMwpc(NULL)
    , fNameDet("MWPC")
    , fNEvents(0)
{
}

R3BSofMwpcOnlineSpectra::R3BSofMwpcOnlineSpectra(const TString& name, Int_t iVerbose, const TString& namedet)
    : FairTask(name, iVerbose)
    , fCalItemsMwpc(NULL)
    , fNameDet(namedet)
    , fNEvents(0)
{
}

R3BSofMwpcOnlineSpectra::~R3BSofMwpcOnlineSpectra()
{
    LOG(INFO) << "R3BSof" + fNameDet + "OnlineSpectra::Delete instance";
    if (fCalItemsMwpc)
        delete fCalItemsMwpc;
}

InitStatus R3BSofMwpcOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSof" + fNameDet + "OnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSof" + fNameDet + "OnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // get access to mapped data of the active target
    fCalItemsMwpc = (TClonesArray*)mgr->GetObject(fNameDet + "CalData");
    if (!fCalItemsMwpc)
    {
        return kFATAL;
    }

    // Create histograms for detectors
    TString Name1;
    TString Name2;

    cMWPCCal = new TCanvas(fNameDet + "_cal", fNameDet + " cal info", 10, 10, 800, 700);
    cMWPCCal->Divide(2, 1);

    // MWPC: Cal data
    Name1 = "fh1_" + fNameDet + "_posx_cal";
    Name2 = fNameDet + ": Position X";
    if (fNameDet != "Mwpc3")
        fh1_mwpc_cal[0] = new TH1F(Name1, Name2, 64, 0.5, 64.5);
    else
        fh1_mwpc_cal[0] = new TH1F(Name1, Name2, 288, 0.5, 288.5);
    fh1_mwpc_cal[0]->GetXaxis()->SetTitle("Position X [pads]");
    fh1_mwpc_cal[0]->GetYaxis()->SetTitle("Counts");
    fh1_mwpc_cal[0]->GetYaxis()->SetTitleOffset(1.15);
    fh1_mwpc_cal[0]->GetXaxis()->CenterTitle(true);
    fh1_mwpc_cal[0]->GetYaxis()->CenterTitle(true);
    fh1_mwpc_cal[0]->GetXaxis()->SetLabelSize(0.045);
    fh1_mwpc_cal[0]->GetXaxis()->SetTitleSize(0.045);
    fh1_mwpc_cal[0]->GetYaxis()->SetLabelSize(0.045);
    fh1_mwpc_cal[0]->GetYaxis()->SetTitleSize(0.045);
    cMWPCCal->cd(1);
    fh1_mwpc_cal[0]->Draw("");

    Name1 = "fh1_" + fNameDet + "_posy_cal";
    Name2 = fNameDet + ": Position Y";
    if (fNameDet != "Mwpc3")
        fh1_mwpc_cal[1] = new TH1F(Name1, Name2, 40, 0.5, 40.5);
    else
        fh1_mwpc_cal[1] = new TH1F(Name1, Name2, 120, 0.5, 120.5);
    fh1_mwpc_cal[1]->GetXaxis()->SetTitle("Position Y [pads]");
    fh1_mwpc_cal[1]->GetYaxis()->SetTitle("Counts");
    fh1_mwpc_cal[1]->GetYaxis()->SetTitleOffset(1.15);
    fh1_mwpc_cal[1]->GetXaxis()->CenterTitle(true);
    fh1_mwpc_cal[1]->GetYaxis()->CenterTitle(true);
    fh1_mwpc_cal[1]->GetXaxis()->SetLabelSize(0.045);
    fh1_mwpc_cal[1]->GetXaxis()->SetTitleSize(0.045);
    fh1_mwpc_cal[1]->GetYaxis()->SetLabelSize(0.045);
    fh1_mwpc_cal[1]->GetYaxis()->SetTitleSize(0.045);
    cMWPCCal->cd(2);
    fh1_mwpc_cal[1]->Draw("");

    cMWPCCal2D = new TCanvas(fNameDet + "_cal2D", fNameDet + " cal 2D info", 10, 10, 800, 700);

    Name1 = "fh2_" + fNameDet + "_posXy_cal";
    Name2 = fNameDet + ": Position X vs Y";
    if (fNameDet != "Mwpc3")
        fh2_mwpc_cal = new TH2F(Name1, Name2, 64, 0.5, 64.5, 40, 0.5, 40.5);
    else
        fh2_mwpc_cal = new TH2F(Name1, Name2, 288, 0.5, 288.5, 120, 0.5, 120.5);
    fh2_mwpc_cal->GetXaxis()->SetTitle("Position X [pads]");
    fh2_mwpc_cal->GetYaxis()->SetTitle("Position Y [pads]");
    fh2_mwpc_cal->GetYaxis()->SetTitleOffset(1.1);
    fh2_mwpc_cal->GetXaxis()->CenterTitle(true);
    fh2_mwpc_cal->GetYaxis()->CenterTitle(true);
    fh2_mwpc_cal->GetXaxis()->SetLabelSize(0.045);
    fh2_mwpc_cal->GetXaxis()->SetTitleSize(0.045);
    fh2_mwpc_cal->GetYaxis()->SetLabelSize(0.045);
    fh2_mwpc_cal->GetYaxis()->SetTitleSize(0.045);
    fh2_mwpc_cal->Draw("col");

    // MAIN FOLDER-AT
    TFolder* mainfolMW0 = new TFolder(fNameDet, fNameDet + " info");
    mainfolMW0->Add(cMWPCCal);
    mainfolMW0->Add(cMWPCCal2D);
    run->AddObject(mainfolMW0);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_" + fNameDet + "_HIST",
                                          Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofMwpcOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSof" + fNameDet + "OnlineSpectra::Reset_Histo";
    // Cal data
    for (Int_t i = 0; i < 2; i++)
        fh1_mwpc_cal[i]->Reset();
    fh2_mwpc_cal->Reset();
}

void R3BSofMwpcOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSof" + fNameDet + "OnlineSpectra::Exec FairRootManager not found";

    // Fill Cal data
    if (fCalItemsMwpc && fCalItemsMwpc->GetEntriesFast())
    {
        Int_t nHits = fCalItemsMwpc->GetEntriesFast();
        Int_t maxpadx = -1, maxpady = -1, maxqx = 0, maxqy = 0;
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofMwpcCalData* hit = (R3BSofMwpcCalData*)fCalItemsMwpc->At(ihit);
            if (!hit)
                continue;
            if (hit->GetPlane() == 1)
            {
                fh1_mwpc_cal[0]->Fill(hit->GetPad());
                if (hit->GetQ() > maxqx)
                {
                    maxpadx = hit->GetPad();
                    maxqx = hit->GetQ();
                }
            }
            if (hit->GetPlane() == 3)
            {
                fh1_mwpc_cal[1]->Fill(hit->GetPad());
                if (hit->GetQ() > maxqy)
                {
                    maxpady = hit->GetPad();
                    maxqy = hit->GetQ();
                }
            }
        }
        if (maxpadx > -1 && maxpady > -1)
            fh2_mwpc_cal->Fill(maxpadx, maxpady);
    }

    fNEvents += 1;
}

void R3BSofMwpcOnlineSpectra::FinishEvent()
{

    if (fCalItemsMwpc)
    {
        fCalItemsMwpc->Clear();
    }
}

void R3BSofMwpcOnlineSpectra::FinishTask()
{

    if (fCalItemsMwpc)
    {
        cMWPCCal->Write();
        cMWPCCal2D->Write();
    }
}

ClassImp(R3BSofMwpcOnlineSpectra)
