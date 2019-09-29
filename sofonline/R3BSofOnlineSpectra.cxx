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
  , fNEvents(0) 
{
}

R3BSofOnlineSpectra::R3BSofOnlineSpectra(const char* name, Int_t iVerbose)
  : FairTask(name, iVerbose)
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
  header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

  FairRunOnline *run = FairRunOnline::Instance();
  run->GetHttpServer()->Register("",this);





    //MAIN FOLDER-FRS
    TFolder* mainfolFrs = new TFolder("SOFIA","SOFIA info");  
    //mainfolFrs->Add(cTrigger);
    run->AddObject(mainfolFrs);


    //Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_ALL_HIST", Form("/Objects/%s/->Reset_GENERAL_Histo()", GetName()));
//    run->GetHttpServer()->RegisterCommand("Reset_Detectors@SO", Form("/Objects/%s/->Reset_SEETRAM_Histo()", GetName()));

  return kSUCCESS;
}



void R3BSofOnlineSpectra::Reset_GENERAL_Histo()
{
    LOG(INFO) << "R3BSofOnlineSpectra::Reset_General_Histo";

}

void R3BSofOnlineSpectra::Exec(Option_t* option) {
  FairRootManager* mgr = FairRootManager::Instance();
  if (NULL == mgr)
    LOG(FATAL) << "R3BSofOnlineSpectra::Exec FairRootManager not found";




  fNEvents += 1;
}


void R3BSofOnlineSpectra::FinishEvent() {


}


void R3BSofOnlineSpectra::FinishTask() {


}

ClassImp(R3BSofOnlineSpectra)
