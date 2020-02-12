#include "R3BSofTofWTcal2SingleTcal.h"

#include "R3BSofSciSingleTcalData.h"
#include "R3BSofToFWSingleTcalData.h"
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
  , fOnline(kFALSE)
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
  
  // --- --------------------------------- --- //
  // --- INPUT SINGLETCAL DATA FROM SofSci --- //
  // --- --------------------------------- --- //

  fSciSingleTcal = (TClonesArray*)rm->GetObject("SofSciSingleTcalData");  
  if (!fSciSingleTcal){
    LOG(ERROR)<<"R3BSofTofWTcal2SingleTcal::Init() Couldn't get handle on SofSciSingleTcalData container";
    return kFATAL;
  }

  // --- ----------------------- --- //
  // --- OUTPUT SINGLE TCAL DATA --- //
  // --- ----------------------- --- //
  
  // Register output array in tree
  fToFWSingleTcal = new TClonesArray("R3BSofToFWSingleTcalData", 10);

    if (!fOnline)
    {
        rm->Register("SofToFWSingleTcalData","SofToFW", fToFWSingleTcal, kTRUE);
    }
    else
    {
        rm->Register("SofToFWSingleTcalData","SofToFW", fToFWSingleTcal, kFALSE);
    }

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

  // Reset entries in output arrays, local arrays
  Reset();

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
  //  LOG(ERROR) << "dimension of SingleTcal TClonesArray for SofSci should be 1 but is" << nHitsPerEvent_SofSci;
  }

  if(nHitsPerEvent_SofSci==1){

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
          AddHitData(d+1, iRawTime, iRawTof, iRawPos);
      }
    }    
    ++fNevent;
  }
  }
}  

// -----   Public method Reset   ------------------------------------------------
void R3BSofTofWTcal2SingleTcal::Reset()
{
    LOG(DEBUG) << "Clearing SofToFWSingleTcalData structure";
    if (fToFWSingleTcal)
        fToFWSingleTcal->Clear();
}

void R3BSofTofWTcal2SingleTcal::Finish()
{

}

// -----   Private method AddData  --------------------------------------------
R3BSofToFWSingleTcalData* R3BSofTofWTcal2SingleTcal::AddHitData(Int_t plastic, Double_t time, Double_t tof, Double_t pos)
{
    // It fills the R3BSofToFWSingleTcalData
    TClonesArray& clref = *fToFWSingleTcal;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofToFWSingleTcalData(plastic, time, tof, pos);
}

ClassImp(R3BSofTofWTcal2SingleTcal)
