#include "R3BSofSciTcal2SingleTcal.h"
#include "R3BSofSciTcalData.h"

#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "FairRootManager.h"
#include "FairLogger.h"


R3BSofSciTcal2SingleTcal::R3BSofSciTcal2SingleTcal()
  : FairTask("R3BSofSciTcal2SingleTcal",1)
  , fTcal(NULL)
  , fRawPosPar(NULL)
  , fSingleTcal(NULL)
  , fNumSingleTcal(0)
  , fNevent(0)
{
}

R3BSofSciTcal2SingleTcal::~R3BSofSciTcal2SingleTcal()
{
  if(fTcal){
    delete fTcal;
  }
  if(fSingleTcal){
    delete fSingleTcal;
  }
}


void R3BSofSciTcal2SingleTcal::SetParContainers()
{
  fRawPosPar = (R3BSofSciRawPosPar*)FairRuntimeDb::instance()->getContainer("SofSciRawPosPar");
  if (!fRawPosPar){
    LOG(ERROR) << "R3BSofSciTcal2SingleTcal::SetParContainers() : Could not get access to SofSciRawPosPar-Container.";
    return;
  }
  else
    LOG(INFO) << "R3BSofSciTcal2SingleTcal::SetParContainers() : SofSciRawPosPar-Container found with " << fRawPosPar->GetNumSignals() << " signals";
}

InitStatus R3BSofSciTcal2SingleTcal::Init()
{

  LOG(INFO) << "R3BSofSciTcal2SingleTcal: Init";

  FairRootManager* rm = FairRootManager::Instance();
  if (!rm) { 
    LOG(ERROR) << "R3BSofSciTcal2SingleTcal::Init() Couldn't instance the FairRootManager";
    return kFATAL;
  }

  // --- --------------- --- //
  // --- INPUT TCAL DATA --- //
  // --- --------------- --- //

  // scintillator at S2 and cave C
  fTcal = (TClonesArray*)rm->GetObject("SofSciTcalData");  
  if (!fTcal){
    LOG(ERROR)<<"R3BSofSciTcal2SingleTcal::Init() Couldn't get handle on SofSciTcalData container";
    return kFATAL;
  }
  else
    LOG(INFO) << "R3BSofSciTcal2SingleTcal::Init() SofSciTcalData items found";

  // --- ----------------------- --- //
  // --- OUTPUT SINGLE TCAL DATA --- //
  // --- ----------------------- --- //
  
  // Register output array in tree
  fSingleTcal = new TClonesArray("R3BSofSciSingleTcalData");
  rm->Register("SofSciSingleTcalData","SofSci", fSingleTcal, kTRUE);
  LOG(INFO) << "R3BSofSciTcal2SingleTcal::Init() R3BSofSciSingleTcalData items created";

  // --- -------------------------- --- //
  // --- CHECK THE TCALPAR VALIDITY --- //
  // --- -------------------------- --- //
  if(fRawPosPar->GetNumSignals()==0){
    LOG(ERROR) << " There are no Tcal parameters for SofSci";
    return kFATAL;
  }
  else{
    LOG(INFO) << " R3BSofSciTcal2SingleTcal::Init() : fNumDetectors=" << fRawPosPar->GetNumDetectors();
    LOG(INFO) << "  R3BSofSciTcal2SingleTcal::Init() : fNumSignals" << fRawPosPar->GetNumSignals();
  }

  LOG(INFO) << "R3BSofSciTcal2SingleTcal: Init DONE !";

  return kSUCCESS;
}



InitStatus R3BSofSciTcal2SingleTcal::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}


void R3BSofSciTcal2SingleTcal::Exec(Option_t* option)
{

  int nDets = int(fRawPosPar->GetNumDetectors());
  int nChs = int(fRawPosPar->GetNumChannels());
  UShort_t iDet; // 0-based
  UShort_t iCh;  // 0-based
  Double_t iTraw[nDets*nChs][16];
  UShort_t mult[nDets*nChs];
  UShort_t mult_max=0;
  
  for(UShort_t i=0; i<nDets*nChs; i++) mult[i]=0;

  Int_t nHitsPerEvent_SofSci = fTcal->GetEntries();
  for(int ihit=0; ihit<nHitsPerEvent_SofSci; ihit++){
    R3BSofSciTcalData* hit = (R3BSofSciTcalData*)fTcal->At(ihit);
    if(!hit)             continue;
    iDet  = hit->GetDetector()-1;
    iCh   = hit->GetPmt()-1;
    if(hit->GetPmt()==3) continue; // no interest HERE for the Common reference
    if(mult_max>=16)     continue; // if multiplicity in a Pmt is higher than 16 are discarded, this code cannot handle it
    iTraw[iDet*nChs+iCh][mult[iDet*nChs+iCh]] = hit->GetRawTimeNs();
    mult[iDet*nChs+iCh]++;
    if (mult[iDet*nChs+iCh]>mult_max) mult_max=mult[iDet*nChs+iCh];
  }// end of loop over the TClonesArray of Tcal data
  
  // LOOP OVER THE ENTRIES TO GET ALL THE POSSIBLE COMBINATION AND TO FIND THE GOOD ONE WITHOUT DOUBLE COUNTS
  if (nHitsPerEvent_SofSci>0){
    UShort_t maskR; // if mult_max>=16, change into UInt_t
    UShort_t maskL; // if mult_max>=16, change into UInt_t
    Double_t iRawPos;
    Double_t RawPos[nDets];
    Double_t RawTime[nDets];
    UShort_t mult_selectHits[nDets];
    for(UShort_t d=0; d<nDets;d++) {
      maskR= 0x0;
      maskL= 0x0;
      RawPos[d] = -1000000.;
      RawTime[d] = -1000000.;
      mult_selectHits[d] = 0;
      for(UShort_t multR=0; multR<mult[d*nChs]; multR++){
	for(UShort_t multL=0; multL<mult[d*nChs+1];multL++){
	  iRawPos = iTraw[d*nChs][multR]-iTraw[d*nChs+1][multL]; // Raw position = Tright - Tleft for x increasing from RIGHT to LEFT
	  if((fRawPosPar->GetSignalTcalParams(0)<=iRawPos)&&(iRawPos<=fRawPosPar->GetSignalTcalParams(1))){
	    // if this hit has already been used, continue
	    if((((maskR>>multR)&(0x1))==0) && (((maskL>multL)&(0x1))==0)){
	      // get the RawPos of the detector
	      RawPos[d] = iRawPos;
	      // calculate the iRawTime
	      RawTime[d] = 0.5*(iTraw[d*nChs][multL]+iTraw[d*nChs+1][multR]);
	      //tag which hit is used
	      maskR |= (0x1)<<multR;
	      maskL |= (0x1)<<multL;
	      //implement how many "good event" is found
	      // attention for a single detector at S2, this might not be sufficient if the searching window of the VFTX is too large
	      mult_selectHits[d]++;
	    }// end of check if this hit hasn't been already used
	  }// end of if(good raw position)
	}// end of loop over the hits of the left PMTs
      }// end of loop over the hits of the right PMTs
    }
    
    // ATTENTION : WE ONLY TAKE THE LAST VALUE OF THE RawPos WHEN SEVERAL OPTIONS ARE POSSIBLES
    // WITHOUT OTHER DETECTORS AT S2, THIS CAN HAPPENS THAT FOR HIGH RATE OR TOO LARGE SEARCHING WINDOW, WE CANNOT DISCRIMINATE LIKE THIS AND EVENT HAS TO BE DISCARDED

    new((*fSingleTcal)[fNumSingleTcal++]) R3BSofSciSingleTcalData;  
    R3BSofSciSingleTcalData * fItem = (R3BSofSciSingleTcalData*)fSingleTcal->At(fNumSingleTcal-1);
    for(UShort_t d=0; d<nDets; d++){
      fItem->SetRawTimeNs(d+1,RawTime[d]);
      fItem->SetRawPosNs(d+1,RawPos[d]);
      //#if NUMBER_OF_DETECTORS==2
      //fItem->SetRawTof(RawTof);
      //#endif
      fItem->SetMultPerDet(d+1,mult_selectHits[d]);
    }
  }
  
  ++fNevent;
}  




void R3BSofSciTcal2SingleTcal::FinishEvent()
{
  fSingleTcal->Clear();
  fNumSingleTcal = 0;
  
}


void R3BSofSciTcal2SingleTcal::FinishTask()
{

}


ClassImp(R3BSofSciTcal2SingleTcal)
