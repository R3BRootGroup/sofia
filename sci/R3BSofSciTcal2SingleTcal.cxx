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
  , fOnline(kFALSE)
  , fNevent(0)
{
#ifdef NUMBER_OF_SOFSCI_TOF
  fRawTofPar = NULL;
#endif
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
  fSingleTcal = new TClonesArray("R3BSofSciSingleTcalData",5);
    if (!fOnline)
    {
     rm->Register("SofSciSingleTcalData","SofSci", fSingleTcal, kTRUE);
    }
    else
    {
     rm->Register("SofSciSingleTcalData","SofSci", fSingleTcal, kFALSE);
    }


  LOG(INFO) << "R3BSofSciTcal2SingleTcal::Init() R3BSofSciSingleTcalData items created";

  // --- -------------------------- --- //
  // --- CHECK THE TCALPAR VALIDITY --- //
  // --- -------------------------- --- //
  if(fRawPosPar->GetNumSignals()==0){
    LOG(ERROR) << " There are no RawPosPar Tcal parameters for SofSci";
    return kFATAL;
  }
  else{
    LOG(INFO) << "  R3BSofSciTcal2SingleTcal::Init() : fRawPosPar: fNumSignals=" << fRawPosPar->GetNumSignals();
  }
#ifdef NUMBER_OF_SOFSCI_TOF
  if(fRawPosPar->GetNumSignals()==0){
    LOG(ERROR) << " There are not RawTofPar Tcal parameters for SofSci";
  }
  else{
    LOG(INFO) << "  R3BSofSciTcal2SingleTcak::Init() : fRawPosPar: fNumSignals=" << fRawTofPar->GetNumSignals();
  }
#endif

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

  int nDets = NUMBER_OF_SOFSCI_DETECTORS;
  int nChs = NUMBER_OF_SOFSCI_CHANNELS;
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
    UInt_t maskR[nDets]; // if mult_max>=32, doesn't work
    UInt_t maskL[nDets]; // if mult_max>=32, doesn't work
    Double_t iRawPos;
    Double_t RawPos[nDets];
    Double_t RawTime[nDets];
    UShort_t mult_selectHits[nDets];
#ifdef NUMBER_OF_SOFSCI_TOF
    int nTof = NUMBER_OF_SOFSCI_TOF;
    int dSta = fRawTofPar->GetFirstStart()-1;
    int dSto = fRawTofPar->GetFirstStop()-1;
    int rank = fRawTofPar->GetFirstRank()-1;
    Bool_t select[nDets];
    Double_t RawTof[nTof];
    for(UShort_t d=0; d<nDets; d++){
      select[d] = kFALSE;
      maskR[d] = 0x0;
      maskR[d] = 0x0;
      iRawTime[d]= -1000000.;
    }

    // SELECTION OF THE MULTIPLICITY LOOKING AT THE ToFraw 
    // * first, start with two selected Sci by the user
    for(UShort_t multRsta=0; multRsta<mult[dSta*nChs]; multRsta++){
      for(UShort_t multLsta=0; multLsta<mult[dSta*nChs+1];multLsto++){
	if((((maskR[dSta]>>multR)&(0x1))==1) || (((maskL[dSta]>multL)&(0x1))==1)) continue;
	iRawPos = 0.5 * (iTraw[dSta*nChs][multRsta] - iTraw[dSta*nChs+1][multLsta]);
	if ((fRawPosPar->GetSignalTcalParams(2*dSta)>iRawPos)||(iRawPos>fRawPosPar->GetSignalTcalParams(2*dSta+1))) continue;	
	for(UShort_t multRsto=0; multRsto<mult[dSto*nChs]; multRsto++){
	  for(UShort_t multLsto=0; multLsto<mult[dSto*nChs+1]; multLsto++){
	    if((((maskR[dSto]>>multR)&(0x1))==1) || (((maskL[dSto]>multL)&(0x1))==1)) continue;
	    iRawPos = 0.5 * (iTraw[dSto*nChs][multRsto] - iTraw[dSto*nChs+1][multLsto]);
	    if ((iRawPos<fRawPosPar->GetSignalTcalParams(2*dSto))||(iRawPos>fRawPosPar->GetSignalTcalParams(2*dSto+1))) continue;
	    iRawTime_dSta = 0.5 * (iTraw[dSta*nChs][multRsta]+iTraw[dSta*nChs+1][multLsta]);	
	    iRawTime_dSto = 0.5 * (iTraw[dSto*nChs][multRsto]+iTraw[dSto*nChs+1][multLsto]);
	    if( (fRawTofPar->GetSignalTcalParams(2*rank)<=(RawTime[dSto]-RawTime[dSta])) && 
		((RawTime[dSto]-RawTime[dSta])<=fRawTofPar->GetSignalTcalParams(2*rank))) {
	      RawTof[rank] = (RawTime[dSto]-RawTime[dSta];
	      RawPos[dSta] =  0.5 * (iTraw[dSto*nChs][multRsto] - iTraw[dSto*nChs+1][multLsto]);  	  
	      RawPos[dSta] =  0.5 * (iTraw[dSto*nChs][multRsto] - iTraw[dSto*nChs+1][multLsto]);  	      
	      RawTime[dSta] = iRawTime_dSta;
	      RawTime[dSto] = iRawTime_dSto;
	      select[dSta] = kTRUE;
	      select[dSto] = kTRUE;
	      maskR[dSta] |= (0x1)<<multRsta;
	      maskL[dSta] |= (0x1)<<multLsta;
	      maskR[dSto] |= (0x1)<<multRsto;
	      maskL[dSto] |= (0x1)<<multLsto;
	    }
	  }
	}
      }
    }
    // * second selection of the beam for the rest of the scintillators


#else
    for(UShort_t d=0; d<nDets;d++) {
      maskR[d]= 0x0;
      maskL[d]= 0x0;
      RawPos[d] = -1000000.;
      RawTime[d] = -1000000.;
      mult_selectHits[d] = 0;
      for(UShort_t multR=0; multR<mult[d*nChs]; multR++){
	for(UShort_t multL=0; multL<mult[d*nChs+1];multL++){
	  iRawPos = iTraw[d*nChs][multR]-iTraw[d*nChs+1][multL]; // Raw position = Tright - Tleft for x increasing from RIGHT to LEFT
	  // if the raw position is outside the range: continue
	  if(iRawPos<fRawPosPar->GetSignalTcalParams(2*d))   continue;
	  if(iRawPos>fRawPosPar->GetSignalTcalParams(2*d+1)) continue;
	  // if this hit has already been used, continue
	  if((((maskR[d]>>multR)&(0x1))==1) || (((maskL[d]>multL)&(0x1))==1)) continue;
	  // get the RawPos of the detector
	  RawPos[d] = iRawPos;
	  // calculate the iRawTime
	  RawTime[d] = 0.5*(iTraw[d*nChs][multL]+iTraw[d*nChs+1][multR]);
	  //tag which hit is used
	  maskR[d] |= (0x1)<<multR;
	  maskL[d] |= (0x1)<<multL;
	  //implement how many "good event" is found
	  // attention for a single detector at S2, this might not be sufficient if the searching window of the VFTX is too large
	   mult_selectHits[d]++;
	}// end of loop over the hits of the left PMTs
      }// end of loop over the hits of the right PMTs
    }
#endif // NUMBER_OF_SOFSCI_DETECTORS==1

    new((*fSingleTcal)[fNumSingleTcal++]) R3BSofSciSingleTcalData;  
    R3BSofSciSingleTcalData * fItem = (R3BSofSciSingleTcalData*)fSingleTcal->At(fNumSingleTcal-1);
    for(UShort_t d=0; d<nDets; d++){
      fItem->SetRawTimeNs(d+1,RawTime[d]);
      fItem->SetRawPosNs(d+1,RawPos[d]);
      fItem->SetMultPerDet(d+1,mult_selectHits[d]);
    }
#ifdef NUMBER_OF_SOFSCI_TOF
    for(UShort_t r=0; r<NUMBER_OF_SOFSCI_TOF;r++){
      fItem->SetRawTofNs(r,RawTof[r]); // check if Set(r or Set(r+1
    }
#endif
  }//end of if nHitsPerEvent_SofSci>0
  
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
