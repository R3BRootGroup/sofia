// ------------------------------------------------------------
// -----                  R3BSofOnlineSpectra             -----
// -----    Created 29/09/19  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BSofAtOnlineSpectra.h"
#include "R3BSofMwpcOnlineSpectra.h"
#include "R3BSofTwimOnlineSpectra.h"
#include "R3BSofSciOnlineSpectra.h"
#include "R3BMusicOnlineSpectra.h"
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

R3BSofOnlineSpectra::R3BSofOnlineSpectra()
    : FairTask("SofiaOnlineSpectra", 1)
    , fEventHeader(nullptr)
    , fAtOnline(NULL)
    , fMwpc0Online(NULL)
    , fMwpc1Online(NULL)
    , fMwpc2Online(NULL)
    , fMwpc3Online(NULL)
    , fTwimOnline(NULL)
    , fSciOnline(NULL)
    , fMusicOnline(NULL)
    , fNEvents(0)
{
}

R3BSofOnlineSpectra::R3BSofOnlineSpectra(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fEventHeader(nullptr)
    , fAtOnline(NULL)
    , fMwpc0Online(NULL)
    , fMwpc1Online(NULL)
    , fMwpc2Online(NULL)
    , fMwpc3Online(NULL)
    , fTwimOnline(NULL)
    , fSciOnline(NULL)
    , fMusicOnline(NULL)
    , fNEvents(0)
{
}

R3BSofOnlineSpectra::~R3BSofOnlineSpectra() { LOG(INFO) << "R3BSofOnlineSpectra::Delete instance"; }

InitStatus R3BSofOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofOnlineSpectra::Init FairRootManager not found";
    fEventHeader = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    if (NULL == run)
        LOG(FATAL) << "R3BSofOnlineSpectra::Init FairRunOnline not found";
    run->GetHttpServer()->Register("", this);

    // Looking for AT online
    fAtOnline = (R3BSofAtOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofAtOnlineSpectra");
    if (!fAtOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::Init SofAtOnlineSpectra not found";

    // Looking for Mwpc0 online
    fMwpc0Online = (R3BSofMwpcOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofMwpc0OnlineSpectra");
    if (!fMwpc0Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::Init SofMwpc0OnlineSpectra not found";

    // Looking for Mwpc1 online
    fMwpc1Online = (R3BSofMwpcOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofMwpc1OnlineSpectra");
    if (!fMwpc1Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::Init SofMwpc1OnlineSpectra not found";

    // Looking for Mwpc2 online
    fMwpc2Online = (R3BSofMwpcOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofMwpc2OnlineSpectra");
    if (!fMwpc2Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::Init SofMwpc2OnlineSpectra not found";

    // Looking for Mwpc3 online
    fMwpc3Online = (R3BSofMwpcOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofMwpc3OnlineSpectra");
    if (!fMwpc3Online)
        LOG(WARNING) << "R3BSofOnlineSpectra::Init SofMwpc3OnlineSpectra not found";

    // Looking for Twim online
    fTwimOnline = (R3BSofTwimOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofTwimOnlineSpectra");
    if (!fTwimOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::Init SofTwimOnlineSpectra not found";

    // Looking for Sci online
    fSciOnline = (R3BSofSciOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofSciOnlineSpectra");
    if (!fSciOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::Init SofSciOnlineSpectra not found";

    // Looking for Music online
    fMusicOnline = (R3BMusicOnlineSpectra*)FairRunOnline::Instance()->GetTask("R3BMusicOnlineSpectra");
    if (!fMusicOnline)
        LOG(WARNING) << "R3BSofOnlineSpectra::Init R3BMusicOnlineSpectra not found";

    // Triggers
    cTrigger = new TCanvas("Triggers", "Trigger information", 10, 10, 800, 700);
    fh1_trigger = new TH1F("fh1_trigger", "Trigger information", 16, -0.5, 15.5);
    fh1_trigger->GetXaxis()->SetTitle("Trigger number");
    fh1_trigger->GetYaxis()->SetTitle("Counts");
    fh1_trigger->GetXaxis()->CenterTitle(true);
    fh1_trigger->GetYaxis()->CenterTitle(true);
    fh1_trigger->GetXaxis()->SetLabelSize(0.04);
    fh1_trigger->GetXaxis()->SetTitleSize(0.04);
    fh1_trigger->GetYaxis()->SetTitleOffset(1.1);
    fh1_trigger->GetXaxis()->SetTitleOffset(1.1);
    fh1_trigger->GetYaxis()->SetLabelSize(0.04);
    fh1_trigger->GetYaxis()->SetTitleSize(0.04);
    fh1_trigger->SetFillColor(kBlue + 2);
    fh1_trigger->Draw("");

    // MAIN FOLDER-SOFIA
    TFolder* mainfolsof = new TFolder("SOFIA", "SOFIA WhiteRabbit and trigger info");
    mainfolsof->Add(cTrigger);
    run->AddObject(mainfolsof);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_GENERAL_HIST", Form("/Objects/%s/->Reset_GENERAL_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofOnlineSpectra::Reset_GENERAL_Histo()
{
    LOG(INFO) << "R3BSofOnlineSpectra::Reset_General_Histo";
    fh1_trigger->Reset();
    // Reset AT histograms if they exist somewhere
    if (fAtOnline)
        fAtOnline->Reset_Histo();
    // Reset Mwpc0 histograms if they exist somewhere
    if (fMwpc0Online)
        fMwpc0Online->Reset_Histo();
    // Reset Mwpc1 histograms if they exist somewhere
    if (fMwpc1Online)
        fMwpc1Online->Reset_Histo();
    // Reset Mwpc2 histograms if they exist somewhere
    if (fMwpc2Online)
        fMwpc2Online->Reset_Histo();
    // Reset Mwpc3 histograms if they exist somewhere
    if (fMwpc3Online)
        fMwpc3Online->Reset_Histo();
    // Reset Twim histograms if they exist somewhere
    if (fTwimOnline)
        fTwimOnline->Reset_Histo();
    // Reset Sci histograms if they exist somewhere
    if (fSciOnline)
        fSciOnline->Reset_Histo();
    // Reset Music histograms if they exist somewhere
    if (fMusicOnline)
        fMusicOnline->Reset_Histo();
}

void R3BSofOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofOnlineSpectra::Exec FairRootManager not found";

    // Fill histogram with trigger information
    fh1_trigger->Fill(fEventHeader->GetTrigger());

    fNEvents += 1;
}

void R3BSofOnlineSpectra::FinishEvent() {}

void R3BSofOnlineSpectra::FinishTask()
{
    // Write trigger canvas in the root file
    cTrigger->Write();
}

ClassImp(R3BSofOnlineSpectra)
