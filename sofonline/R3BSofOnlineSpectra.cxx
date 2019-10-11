// ------------------------------------------------------------
// -----                  R3BSofOnlineSpectra             -----
// -----    Created 29/09/19  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data  
 */

#include "R3BSofOnlineSpectra.h"
#include "R3BSofAtOnlineSpectra.h"
#include "R3BSofMwpc0OnlineSpectra.h"
#include "R3BSofTwimOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "THttpServer.h"

#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "FairRootManager.h"
#include "FairLogger.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TFolder.h"
#include "TVector3.h"

#include "TClonesArray.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <array>
#include "TRandom.h"
#include "TMath.h"
#include "TLegend.h"
#include "TLegendEntry.h"


using namespace std;

R3BSofOnlineSpectra::R3BSofOnlineSpectra()
  : FairTask("SofiaOnlineSpectra", 1)
  , fEventHeader(nullptr)
  , fAtOnline(NULL)
  , fMwpc0Online(NULL)
  , fTwimOnline(NULL)
  , fNEvents(0) 
{
}

R3BSofOnlineSpectra::R3BSofOnlineSpectra(const char* name, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , fEventHeader(nullptr)
  , fAtOnline(NULL)
  , fMwpc0Online(NULL)
  , fTwimOnline(NULL)
  , fNEvents(0)
{
}

R3BSofOnlineSpectra::~R3BSofOnlineSpectra() {
  LOG(INFO) << "R3BSofOnlineSpectra::Delete instance";

}

InitStatus R3BSofOnlineSpectra::Init() {
  
  LOG(INFO) << "R3BSofOnlineSpectra::Init ";
  
  // try to get a handle on the EventHeader. EventHeader may not be 
  // present though and hence may be null. Take care when using.
  
  FairRootManager* mgr = FairRootManager::Instance();
  if (NULL == mgr)
     LOG(FATAL)<<"R3BSofOnlineSpectra::Init FairRootManager not found";
  fEventHeader = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

  FairRunOnline *run = FairRunOnline::Instance();
  if (NULL == run)
     LOG(FATAL)<<"R3BSofOnlineSpectra::Init FairRunOnline not found";
  run->GetHttpServer()->Register("",this);

  //Looking for AT online
  fAtOnline = (R3BSofAtOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofAtOnlineSpectra");
  if (!fAtOnline)
     LOG(WARNING)<<"R3BSofOnlineSpectra::Init SofAtOnlineSpectra not found";

  //Looking for Mwpc0 online
  fMwpc0Online = (R3BSofMwpc0OnlineSpectra*)FairRunOnline::Instance()->GetTask("SofMwpc0OnlineSpectra");
  if (!fMwpc0Online)
     LOG(WARNING)<<"R3BSofOnlineSpectra::Init SofMwpc0OnlineSpectra not found";

  //Looking for Twim online
  fTwimOnline = (R3BSofTwimOnlineSpectra*)FairRunOnline::Instance()->GetTask("SofTwimOnlineSpectra");
  if (!fTwimOnline)
     LOG(WARNING)<<"R3BSofOnlineSpectra::Init SofTwimOnlineSpectra not found";

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
  fh1_trigger->SetFillColor(kBlue+2);
  fh1_trigger->Draw("");

  //MAIN FOLDER-SOFIA
  TFolder* mainfolsof = new TFolder("SOFIA","SOFIA WhiteRabbit and trigger info");  
  mainfolsof->Add(cTrigger);
  run->AddObject(mainfolsof);

  //Register command to reset histograms
  run->GetHttpServer()->RegisterCommand("Reset_GENERAL_HIST", Form("/Objects/%s/->Reset_GENERAL_Histo()", GetName()));

  return kSUCCESS;
}

void R3BSofOnlineSpectra::Reset_GENERAL_Histo()
{
  LOG(INFO) << "R3BSofOnlineSpectra::Reset_General_Histo";
  fh1_trigger->Reset();
  //Reset AT histograms if they exist somewhere
  if (fAtOnline)
   fAtOnline->Reset_Histo();
  //Reset Mwpc0 histograms if they exist somewhere
  if (fMwpc0Online)
   fMwpc0Online->Reset_Histo();
  //Reset Twim histograms if they exist somewhere
  if (fTwimOnline)
   fTwimOnline->Reset_Histo();
}

void R3BSofOnlineSpectra::Exec(Option_t* option) {
  FairRootManager* mgr = FairRootManager::Instance();
  if (NULL == mgr)
    LOG(FATAL) << "R3BSofOnlineSpectra::Exec FairRootManager not found";

  //Fill histogram with trigger information
  fh1_trigger->Fill(fEventHeader->GetTrigger());

  fNEvents += 1;
}

void R3BSofOnlineSpectra::FinishEvent() {

}

void R3BSofOnlineSpectra::FinishTask() {
  //Write trigger canva in the root file
  cTrigger->Write();
}

ClassImp(R3BSofOnlineSpectra)
