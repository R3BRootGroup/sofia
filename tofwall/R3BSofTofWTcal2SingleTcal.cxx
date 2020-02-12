#include "R3BSofTofWTcal2SingleTcal.h"

#include "R3BSofSciSingleTcalData.h"
#include "R3BSofToFWTcalData.h"

#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "FairRootManager.h"
#include "FairLogger.h"
#include "detectors_cfg.h"

R3BSofTofWTcal2SingleTcal::R3BSofTofWTcal2SingleTcal()
  : FairTask("R3BSofTofWTcal2SingleTcal",1)
  , fSciSingleTcal(NULL)
  , fToFWTcal(NULL)
  , fToFWSingleTcal(NULL)
  , fNevent(0)
{
}

R3BSofTofWTcal2SingleTcal::~R3BSofTofWTcal2SingleTcal()
{
  if(fSciSingleTcal){
    delete fSciSingleTcal;
  }
  if(fToFWTcal){
    delete fToFWTcal;
  }
  if(fToFWSingleTcal){
    delete fToFWSingleTcal;
  }
}


void R3BSofTofWTcal2SingleTcal::SetParContainers()
{
}

InitStatus R3BSofTofWTcal2SingleTcal::Init()
{

  LOG(INFO) << "R3BSofTofWTcal2SingleTcal: Init";

  FairRootManager* rm = FairRootManager::Instance();
  if (!rm) { 
    LOG(ERROR) << "R3BSofTofWTcal2SingleTcal::Init() Couldn't instance the FairRootManager";
    return kFATAL;
  }

  // --- ----------------------------- --- //
  // --- INPUT TCAL DATA FROM ToF WALL --- //
  // --- ----------------------------- --- //

  fToFWTcal = (TClonesArray*)rm->GetObject("SofToFWTcalData");  
  if (!fToFWTcal){
    LOG(ERROR)<<"R3BSofTofWTcal2SingleTcal::Init() Couldn't get handle on SofToFWTcalData container";
    return kFATAL;
  }
  else
    LOG(INFO) << "R3BSofTofWTcal2SingleTcal::Init() SofToFWTcalData items found";
  
  // --- --------------------------------- --- //
  // --- INPUT SINGLETCAL DATA FROM SofSci --- //
  // --- --------------------------------- --- //

  fSciSingleTcal = (TClonesArray*)rm->GetObject("SofSciSingleTcalData");  
  if (!fSciSingleTcal){
    LOG(ERROR)<<"R3BSofTofWTcal2SingleTcal::Init() Couldn't get handle on SofSciSingleTcalData container";
    return kFATAL;
  }
  else
    LOG(INFO) << "R3BSofTofWTcal2SingleTcal::Init() SofSciSingleTcalData items found";

  // --- ----------------------- --- //
  // --- OUTPUT SINGLE TCAL DATA --- //
  // --- ----------------------- --- //
  
  // Register output array in tree
  fToFWSingleTcal = new TClonesArray("R3BSofToFWSingleTcalData");
  rm->Register("SofToFWSingleTcalData","SofToFW", fToFWSingleTcal, kTRUE);
  LOG(INFO) << "R3BSofTofWTcal2SingleTcal::Init() R3BSofToFWSingleTcalData items created";

  LOG(INFO) << "R3BSofTofWTcal2SingleTcal: Init DONE !";

  return kSUCCESS;
}



InitStatus R3BSofTofWTcal2SingleTcal::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}


void R3BSofTofWTcal2SingleTcal::Exec(Option_t* option)
{

  int nDets = int(NUMBER_OF_SOFTOFW_PLASTICS);
  int nChs = int(NUMBER_OF_SOFTOFW_PMTS_PER_PLASTIC);
  UShort_t iDet; // 0-based
  UShort_t iCh;  // 0-based
  Double_t iTraw[nDets*nChs][16];
  UShort_t mult[nDets*nChs];
  UShort_t mult_max=0;
  
  for(UShort_t i=0; i<nDets*nChs; i++) mult[i]=0;

  // --- ------------------------------------------- --- //
  // --- SOFSCI: GET THE Traw FROM THE SCI AT CAVE C --- //
  // --- ------------------------------------------- --- //
  Int_t nHitsPerEvent_SofSci  = fSciSingleTcal->GetEntries();
  if(nHitsPerEvent_SofSci!=1)
  {
    LOG(ERROR) << "dimension of SingleTcal TClonesArray for SofSci should be 1 but is" << nHitsPerEvent_SofSci;
  }
  R3BSofSciSingleTcalData* hitSci = (R3BSofSciSingleTcalData*)fSciSingleTcal->At(0);
  Double_t iRawTime_SofSci = hitSci->GetRawTimeNs(ID_SOFSCI_CAVEC);

  // --- ------------------------------------------------------------------------- --- //
  // --- SOFTOFW: CALCULATE THE RAW TIME, TOF AND POSITION FOR THE PLASTICS HITTED --- //
  // --- ------------------------------------------------------------------------- --- //
  Int_t nHitsPerEvent_SofToFW = fToFWTcal->GetEntries();
  // get the multiplicity per PMT
  for(int ihit=0; ihit<nHitsPerEvent_SofToFW; ihit++){
    R3BSofToFWTcalData* hit = (R3BSofToFWTcalData*)fToFWTcal->At(ihit);
    if(!hit)             continue;
    iDet  = hit->GetDetector()-1;
    iCh   = hit->GetPmt()-1;
    if(mult_max>=16)     continue; // if multiplicity in a Pmt is higher than 16 are discarded, this code cannot handle it
    iTraw[iDet*nChs+iCh][mult[iDet*nChs+iCh]] = hit->GetRawTimeNs();
    mult[iDet*nChs+iCh]++;
    if (mult[iDet*nChs+iCh]>mult_max) mult_max=mult[iDet*nChs+iCh];
  }// end of loop over the TClonesArray of Tcal data
  
  if (nHitsPerEvent_SofToFW>0)
  {
    Double_t iRawPos;
    Double_t iRawTime;
    Double_t iRawTof;
    for(UShort_t d=0; d<nDets;d++) 
    {
      iRawPos = -1000000.;
      iRawTime = -1000000.;
      iRawTof = -1000000.;
      // check mult==1 for the PMTup and PMTdown
      if ((mult[d*nChs+1]==1) && (mult[d*nChs]==1))
      {
	  iRawPos  = iTraw[d*nChs+1][0]-iTraw[d*nChs][0]; // Raw position = Tdown - Tup
	  iRawTime = 0.5*(iTraw[d*nChs][0]+iTraw[d*nChs+1][0]);
	  iRawTof  = 0.5*(iRawTime - iRawTime_SofSci);     
	  new((*fToFWSingleTcal)[fNumSingleTcal++]) R3BSofToFWSingleTcalData;  
          R3BSofToFWSingleTcalData * fItem = (R3BSofToFWSingleTcalData*)fToFWSingleTcal->At(fNumSingleTcal-1);
	  fItem->SetDetector(d+1);
	  fItem->SetRawTimeNs(iRawTime);
	  fItem->SetRawPosNs(iRawPos);
	  fItem->SetRawTofNs(iRawTof);
      }
    }    
    ++fNevent;
  }
}  




void R3BSofTofWTcal2SingleTcal::FinishEvent()
{
  fToFWSingleTcal->Clear();
  fNumSingleTcal = 0;
  
}


void R3BSofTofWTcal2SingleTcal::FinishTask()
{

}


ClassImp(R3BSofTofWTcal2SingleTcal)
