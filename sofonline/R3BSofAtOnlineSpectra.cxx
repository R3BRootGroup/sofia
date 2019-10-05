// ------------------------------------------------------------
// -----                  R3BSofAtOnlineSpectra           -----
// -----    Created 29/09/19  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data  
 */

#include "R3BSofAtOnlineSpectra.h"
#include "R3BSofAtMappedData.h"
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

R3BSofAtOnlineSpectra::R3BSofAtOnlineSpectra()
  : FairTask("SofiaAtOnlineSpectra", 1)
  , fMappedItemsAt(NULL)
  , fNEvents(0) 
{
}

R3BSofAtOnlineSpectra::R3BSofAtOnlineSpectra(const char* name, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , fMappedItemsAt(NULL)
  , fNEvents(0)
{
}

R3BSofAtOnlineSpectra::~R3BSofAtOnlineSpectra() {
  LOG(INFO) << "R3BSofAtOnlineSpectra::Delete instance";
  if(fMappedItemsAt) delete fMappedItemsAt;
}

InitStatus R3BSofAtOnlineSpectra::Init() {
  
  LOG(INFO) << "R3BSofAtOnlineSpectra::Init ";
  
  // try to get a handle on the EventHeader. EventHeader may not be 
  // present though and hence may be null. Take care when using.
  
  FairRootManager* mgr = FairRootManager::Instance();
  if (NULL == mgr)
     LOG(FATAL)<<"R3BSofAtOnlineSpectra::Init FairRootManager not found";
  header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

  FairRunOnline *run = FairRunOnline::Instance();
  run->GetHttpServer()->Register("",this);

  //get access to mapped data of the active target
  fMappedItemsAt = (TClonesArray*)mgr->GetObject("AtMappedData");
  if (!fMappedItemsAt) { return kFATAL;}


  //Create histograms for detectors 
  char Name1[255];
  char Name2[255];

  cAtMap = new TCanvas("AT_map", "AT mapped info", 10, 10, 800, 700);
  cAtMap->Divide(2,2);

  char Name3[255];
  char Name4[255];
  //AT: Map data
  for(Int_t i=0;i<3;i++){// 3 correlations between sections
    sprintf(Name1, "fh2_at_energy_map_%d", i);
    sprintf(Name2, "Section A%d vs Section A%d", i+1,i+2);
    sprintf(Name3, "A%d [channels]", i+1);
    sprintf(Name4, "A%d [channels]", i+2);
    fh2_atmap[i] = new TH2F(Name1,Name2,1000,0,8192,1000,0,8192);
    fh2_atmap[i]->GetXaxis()->SetTitle(Name3);
    fh2_atmap[i]->GetYaxis()->SetTitle(Name4);
    fh2_atmap[i]->GetYaxis()->SetTitleOffset(1.1);
    fh2_atmap[i]->GetXaxis()->CenterTitle(true);
    fh2_atmap[i]->GetYaxis()->CenterTitle(true);
    fh2_atmap[i]->GetXaxis()->SetLabelSize(0.045);
    fh2_atmap[i]->GetXaxis()->SetTitleSize(0.045);
    fh2_atmap[i]->GetYaxis()->SetLabelSize(0.045);
    fh2_atmap[i]->GetYaxis()->SetTitleSize(0.045);
    cAtMap->cd(i+1);
    fh2_atmap[i]->Draw("col");
  }

  fh1_atmapevt = new TH1F("fh_at_events","Events per section",4,0.5,4.5);
  fh1_atmapevt->GetXaxis()->SetTitle("AT section");
  fh1_atmapevt->GetYaxis()->SetTitle("Counts");
  fh1_atmapevt->GetYaxis()->SetTitleOffset(1.);
  fh1_atmapevt->GetXaxis()->CenterTitle(true);
  fh1_atmapevt->GetYaxis()->CenterTitle(true);
  fh1_atmapevt->GetXaxis()->SetLabelSize(0.045);
  fh1_atmapevt->GetXaxis()->SetTitleSize(0.045);
  fh1_atmapevt->GetYaxis()->SetLabelSize(0.045);
  fh1_atmapevt->GetYaxis()->SetTitleSize(0.045);
  cAtMap->cd(4);
  fh1_atmapevt->Draw("");


  //MAIN FOLDER-AT
  TFolder* mainfolAt = new TFolder("AT","Active target info");  
  mainfolAt->Add(cAtMap);
  run->AddObject(mainfolAt);

  //Register command to reset histograms
  run->GetHttpServer()->RegisterCommand("Reset_AT_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

  return kSUCCESS;
}

void R3BSofAtOnlineSpectra::Reset_Histo()
{
  LOG(INFO) << "R3BSofAtOnlineSpectra::Reset_Histo";
  //Map data
  for(Int_t i=0;i<3;i++)
  fh2_atmap[i]->Reset();
  fh1_atmapevt->Reset();
}

void R3BSofAtOnlineSpectra::Exec(Option_t* option) {
  FairRootManager* mgr = FairRootManager::Instance();
  if (NULL == mgr)
    LOG(FATAL) << "R3BSofAtOnlineSpectra::Exec FairRootManager not found";

  //Fill mapped data
  if(fMappedItemsAt && fMappedItemsAt->GetEntriesFast()){
    Int_t atmap[4]={0,0,0,0};
    Int_t nHits = fMappedItemsAt->GetEntriesFast();
    for (Int_t ihit = 0; ihit < nHits; ihit++){
      R3BSofAtMappedData* hit = 
	(R3BSofAtMappedData*)fMappedItemsAt->At(ihit);
      if (!hit) continue;
        atmap[hit->GetSecID()]=hit->GetEnergy();
        fh1_atmapevt->Fill(hit->GetSecID()+1);
    }
    if(atmap[0]>0&&atmap[1]>0)fh2_atmap[0]->Fill(atmap[0],atmap[1]);
    if(atmap[1]>0&&atmap[2]>0)fh2_atmap[1]->Fill(atmap[1],atmap[2]);
    if(atmap[2]>0&&atmap[3]>0)fh2_atmap[2]->Fill(atmap[2],atmap[3]);
  }

  fNEvents += 1;
}


void R3BSofAtOnlineSpectra::FinishEvent() {

    if (fMappedItemsAt)
    {
        fMappedItemsAt->Clear();
    }

}


void R3BSofAtOnlineSpectra::FinishTask() {

    if(fMappedItemsAt){
        cAtMap->Write();
    }

}

ClassImp(R3BSofAtOnlineSpectra)
