// ------------------------------------------------------------
// -----                  R3BSofMwpc0OnlineSpectra        -----
// -----    Created 29/09/19  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data  
 */

#include "R3BSofMwpc0OnlineSpectra.h"
#include "R3BSofMwpcCalData.h"
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

R3BSofMwpc0OnlineSpectra::R3BSofMwpc0OnlineSpectra()
  : FairTask("SofMwpc0OnlineSpectra", 1)
  , fCalItemsMwpc0(NULL)
  , fNEvents(0) 
{
}

R3BSofMwpc0OnlineSpectra::R3BSofMwpc0OnlineSpectra(const char* name, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , fCalItemsMwpc0(NULL)
  , fNEvents(0)
{
}

R3BSofMwpc0OnlineSpectra::~R3BSofMwpc0OnlineSpectra() {
  LOG(INFO) << "R3BSofMwpc0OnlineSpectra::Delete instance";
  if(fCalItemsMwpc0) delete fCalItemsMwpc0;
}

InitStatus R3BSofMwpc0OnlineSpectra::Init() {
  
  LOG(INFO) << "R3BSofMwpc0OnlineSpectra::Init ";
  
  // try to get a handle on the EventHeader. EventHeader may not be 
  // present though and hence may be null. Take care when using.
  
  FairRootManager* mgr = FairRootManager::Instance();
  if (NULL == mgr)
     LOG(FATAL)<<"R3BSofMwpc0OnlineSpectra::Init FairRootManager not found";
  header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

  FairRunOnline *run = FairRunOnline::Instance();
  run->GetHttpServer()->Register("",this);

  //get access to mapped data of the active target
  fCalItemsMwpc0 = (TClonesArray*)mgr->GetObject("Mwpc0CalData");
  if (!fCalItemsMwpc0) { return kFATAL;}


  //Create histograms for detectors 
  char Name1[255];
  char Name2[255];

  cMWPC0Cal = new TCanvas("MWPC0_cal", "MWPC0 cal info", 10, 10, 800, 700);
  cMWPC0Cal->Divide(2,1);

  //MWPC0: Cal data
  sprintf(Name1, "fh1_mwpc0_posx_cal");
  sprintf(Name2, "Position X");
  fh1_mwpc0_cal[0] = new TH1F(Name1,Name2,64,0.5,64.5);
  fh1_mwpc0_cal[0]->GetXaxis()->SetTitle("Position X [pads]");
  fh1_mwpc0_cal[0]->GetYaxis()->SetTitle("Counts");
  fh1_mwpc0_cal[0]->GetYaxis()->SetTitleOffset(1.15);
  fh1_mwpc0_cal[0]->GetXaxis()->CenterTitle(true);
  fh1_mwpc0_cal[0]->GetYaxis()->CenterTitle(true);
  fh1_mwpc0_cal[0]->GetXaxis()->SetLabelSize(0.045);
  fh1_mwpc0_cal[0]->GetXaxis()->SetTitleSize(0.045);
  fh1_mwpc0_cal[0]->GetYaxis()->SetLabelSize(0.045);
  fh1_mwpc0_cal[0]->GetYaxis()->SetTitleSize(0.045);
  cMWPC0Cal->cd(1);
  fh1_mwpc0_cal[0]->Draw("");

  sprintf(Name1, "fh1_mwpc0_posy_cal");
  sprintf(Name2, "Position Y");
  fh1_mwpc0_cal[1] = new TH1F(Name1,Name2,40,0.5,40.5);
  fh1_mwpc0_cal[1]->GetXaxis()->SetTitle("Position Y [pads]");
  fh1_mwpc0_cal[1]->GetYaxis()->SetTitle("Counts");
  fh1_mwpc0_cal[1]->GetYaxis()->SetTitleOffset(1.15);
  fh1_mwpc0_cal[1]->GetXaxis()->CenterTitle(true);
  fh1_mwpc0_cal[1]->GetYaxis()->CenterTitle(true);
  fh1_mwpc0_cal[1]->GetXaxis()->SetLabelSize(0.045);
  fh1_mwpc0_cal[1]->GetXaxis()->SetTitleSize(0.045);
  fh1_mwpc0_cal[1]->GetYaxis()->SetLabelSize(0.045);
  fh1_mwpc0_cal[1]->GetYaxis()->SetTitleSize(0.045);
  cMWPC0Cal->cd(2);
  fh1_mwpc0_cal[1]->Draw("");

  cMWPC0Cal2D = new TCanvas("MWPC0_cal2D", "MWPC0 cal 2D info", 10, 10, 800, 700);

  sprintf(Name1, "fh2_mwpc0_posXy_cal");
  sprintf(Name2, "Position X vs Y");
  fh2_mwpc0_cal = new TH2F(Name1,Name2,64,0.5,64.5,40,0.5,40.5);
  fh2_mwpc0_cal->GetXaxis()->SetTitle("Position X [pads]");
  fh2_mwpc0_cal->GetYaxis()->SetTitle("Position Y [pads]");
  fh2_mwpc0_cal->GetYaxis()->SetTitleOffset(1.1);
  fh2_mwpc0_cal->GetXaxis()->CenterTitle(true);
  fh2_mwpc0_cal->GetYaxis()->CenterTitle(true);
  fh2_mwpc0_cal->GetXaxis()->SetLabelSize(0.045);
  fh2_mwpc0_cal->GetXaxis()->SetTitleSize(0.045);
  fh2_mwpc0_cal->GetYaxis()->SetLabelSize(0.045);
  fh2_mwpc0_cal->GetYaxis()->SetTitleSize(0.045);
  fh2_mwpc0_cal->Draw("col");

  //MAIN FOLDER-AT
  TFolder* mainfolMW0 = new TFolder("MWPC0","MWPC0 info");  
  mainfolMW0->Add(cMWPC0Cal);
  mainfolMW0->Add(cMWPC0Cal2D);
  run->AddObject(mainfolMW0);

  //Register command to reset histograms
  run->GetHttpServer()->RegisterCommand("Reset_MWPC0_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

  return kSUCCESS;
}

void R3BSofMwpc0OnlineSpectra::Reset_Histo()
{
  LOG(INFO) << "R3BSofMwpc0OnlineSpectra::Reset_Histo";
  //Cal data
  for(Int_t i=0;i<2;i++)
  fh1_mwpc0_cal[i]->Reset();
  fh2_mwpc0_cal->Reset();
}

void R3BSofMwpc0OnlineSpectra::Exec(Option_t* option) {
  FairRootManager* mgr = FairRootManager::Instance();
  if (NULL == mgr)
    LOG(FATAL) << "R3BSofMwpc0OnlineSpectra::Exec FairRootManager not found";

  //Fill Cal data
  if(fCalItemsMwpc0 && fCalItemsMwpc0->GetEntriesFast()){
    Int_t nHits = fCalItemsMwpc0->GetEntriesFast();
    Int_t maxpadx=-1,maxpady=-1, maxqx=0,maxqy=0;
    for (Int_t ihit = 0; ihit < nHits; ihit++){
      R3BSofMwpcCalData* hit = 
	(R3BSofMwpcCalData*)fCalItemsMwpc0->At(ihit);
      if (!hit) continue;
        if(hit->GetPlane()==1){
          fh1_mwpc0_cal[0]->Fill(hit->GetPad());
          if(hit->GetQ()>maxqx){
              maxpadx=hit->GetPad();
              maxqx=hit->GetQ();
          }
        }
        if(hit->GetPlane()==3){
          fh1_mwpc0_cal[1]->Fill(hit->GetPad());
          if(hit->GetQ()>maxqy){
              maxpady=hit->GetPad();
              maxqy=hit->GetQ();
          }
        }
    }
    if(maxpadx>-1&&maxpady>-1)fh2_mwpc0_cal->Fill(maxpadx,maxpady);
  }

  fNEvents += 1;
}


void R3BSofMwpc0OnlineSpectra::FinishEvent() {

    if (fCalItemsMwpc0)
    {
        fCalItemsMwpc0->Clear();
    }

}


void R3BSofMwpc0OnlineSpectra::FinishTask() {

    if(fCalItemsMwpc0){
        cMWPC0Cal->Write();
        cMWPC0Cal2D->Write();
    }

}

ClassImp(R3BSofMwpc0OnlineSpectra)
