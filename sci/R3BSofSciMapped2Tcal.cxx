#include "R3BSofSciMapped2Tcal.h"
#include "R3BSofSciMappedData.h"
#include "R3BSofComRefMappedData.h"

#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "FairRootManager.h"
#include "FairLogger.h"


R3BSofSciMapped2Tcal::R3BSofSciMapped2Tcal()
  : FairTask("R3BSofSciMapped2Tcal",1)
  , fMappedItemsComRef(NULL)
  , fMappedItemsSci(NULL)
  , fTcalPar(NULL)
  , fTcalItems(new TClonesArray("R3BSofSciTcalData"))
  , fNumTcalItems(0)
  , fNevent(0)
{
}


R3BSofSciMapped2Tcal::~R3BSofSciMapped2Tcal()
{
  if(fTcalItems){
    delete fTcalItems;
    fTcalItems=NULL;
  }
}


InitStatus R3BSofSciMapped2Tcal::Init()
{

  LOG(INFO) << "R3BSofSciMapped2Tcal: Init";

  FairRootManager* rm = FairRootManager::Instance();
  if (!rm) { 
    LOG(ERROR) << "R3BSofSciMapped2Tcal::Init() Couldn't instance the FairRootManager";
    return kFATAL;
  }

  // --- ----------------- --- //
  // --- INPUT MAPPED DATA --- //
  // --- ----------------- --- //

  // Common reference signal
  fMappedItemsComRef = (TClonesArray*)rm->GetObject("SofComRef");  // see Instance->Register in R3BSofComRefReader.cxx  
  if (!fMappedItemsComRef){
    LOG(ERROR)<<"R3BSofSciMapped2Tcal::Init() Couldn't get handle on SofComRef Mapped Items";
    return kFATAL;
  }
  else
    LOG(INFO) << " R3BSofSciMapped2Tcal::Init() SofComRef Mapped items found";


  // scintillator at S2 and cave C
  fMappedItemsSci = (TClonesArray*)rm->GetObject("SofSci");        // see Instance->Register in R3BSofSciReader.cxx
  if (!fMappedItemsSci){
    LOG(ERROR)<<"R3BSofSciMapped2Tcal::Init() Couldn't get handle on SofSci container";
    return kFATAL;
  }
  else
    LOG(INFO) << " R3BSofSciMapped2Tcal::Init() SofSci Mapped items found";
  

  // --- -------------------------- --- //
  // --- CHECK THE TCALPAR VALIDITY --- //
  // --- -------------------------- --- //
  if(fTcalPar->GetNumSignals()==0){
    LOG(ERROR) << " There are no Tcal parameters for SofSci";
    return kFATAL;
  }
  else
    LOG(INFO) << " Number of signals for SofSci with defined tcal parameters : " << fTcalPar->GetNumSignals();
  

  // --- ---------------- --- //
  // --- OUTPUT TCAL DATA --- //
  // --- ---------------- --- //
  
  // Register output array in tree
  rm->Register("SofSciTcal","TcalDim", fTcalItems, kTRUE);

  LOG(INFO) << "R3BSofSciMapped2Tcal: Init DONE !";

  return kSUCCESS;
}


void R3BSofSciMapped2Tcal::SetParContainers()
{
  fTcalPar = (R3BSofTcalPar*)FairRuntimeDb::instance()->getContainer("SofSciTcalPar");
  if (!fTcalPar){
    LOG(ERROR) << "R3BSofSciMapped2Tcal::SetParContainers() : Could not get access to SofSciTcalPar-Container.";
    return;
  }
  else
    LOG(INFO) << "R3BSofSciMapped2Tcal::SetParContainers() : SofSciTcalPar-Container found with " << fTcalPar->GetNumSignals() << " signals";
}


InitStatus R3BSofSciMapped2Tcal::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}


void R3BSofSciMapped2Tcal::Exec(Option_t* option)
{
  UShort_t iDet;
  UShort_t iCh;
  UInt_t iTf;
  UInt_t iTc;
  Double_t tns;

  // --- -------------------------------------------- --- //
  // --- START WITH THE COMMON REFERENCE SIGNALS DATA --- //
  // --- -------------------------------------------- --- //
  Int_t nHitsPerEvent_SofComRef = fMappedItemsComRef->GetEntries();
  for(int ihit=0; ihit<nHitsPerEvent_SofComRef; ihit++){
    R3BSofComRefMappedData* hit = (R3BSofComRefMappedData*)fMappedItemsComRef->At(ihit);
    if(!hit) continue;
    iDet = hit->GetDetector();
    iCh  = 1;
    iTf  = hit->GetTimeFine();
    iTc  = hit->GetTimeCoarse();
    if((iDet<1)||(iDet>fTcalPar->GetNumDetectors())) {
      LOG(INFO) << "R3BSofSciMapped2Tcal::Exec() : In SofComRefMappedData, iDet = " << iDet << "is out of range, item skipped ";
      continue;
    }
    tns = CalculateTimeNs(iDet,iCh,iTf,iTc);
    new((*fTcalItems)[fNumTcalItems++]) R3BSofSciTcalData(iDet,iCh,tns);
  }

  // --- ------------------------------------ --- //
  // --- CONTINUE WITH THE SCINTILLATORS DATA --- //
  // --- ------------------------------------ --- //
  Int_t nHitsPerEvent_SofSci = fMappedItemsSci->GetEntries();
  for(int ihit=0; ihit<nHitsPerEvent_SofSci; ihit++){
    R3BSofSciMappedData* hit = (R3BSofSciMappedData*)fMappedItemsSci->At(ihit);
    if(!hit) continue;
    iDet = hit->GetDetector();
    iCh  = hit->GetPmt()+1;
    iTf  = hit->GetTimeFine();
    iTc  = hit->GetTimeCoarse();
    if((iDet<1)||(iDet>fTcalPar->GetNumDetectors())) {
      LOG(INFO) << "R3BSofSciMapped2Tcal::Exec() : In SofSciMappedData, iDet = " << iDet << "is out of range, item skipped ";
      continue;
    }
    if((iCh<2)||(iCh>fTcalPar->GetNumChannels())) {
      LOG(INFO) << "R3BSofSciMapped2Tcal::Exec() : In SofSciMappedData, iCh = " << iCh << "is out of range, item skipped ";
      continue;
    }
    tns = CalculateTimeNs(iDet,iCh,iTf,iTc);
    new((*fTcalItems)[fNumTcalItems++]) R3BSofSciTcalData(iDet,iCh,tns);
  }
    
  ++fNevent;

}  

void R3BSofSciMapped2Tcal::FinishEvent()
{
  fTcalItems->Clear();
  fNumTcalItems = 0;
}


void R3BSofSciMapped2Tcal::FinishTask()
{

}



Double_t R3BSofSciMapped2Tcal::CalculateTimeNs(UShort_t iDet, UShort_t iCh, UInt_t iTf, UInt_t iTc)
{
  UInt_t   rank = iTf + fTcalPar->GetNumTcalParsPerSignal()*((iDet-1)*fTcalPar->GetNumChannels() + (iCh-1));
  Double_t iPar = (Double_t)fTcalPar->GetSignalTcalParams(rank);
  Double_t r = (Double_t)rand.Rndm()-0.5;
  Double_t iTf_ns;
  Double_t iTc_ns = (Double_t)iTc*5.;
  //  std::cout << "R3BSofSciMapped2Tcal::CalculateTimeNs : iDet=" << iDet << ", iCh=" << iCh << ", iTf=" << iTf << ", rank=" << rank  << std::endl; 

  if(r<0){
    Double_t iParPrev = fTcalPar->GetSignalTcalParams(rank-1);
    iTf_ns = iPar + r*(iPar -  iParPrev);
  }
  else{
    Double_t iParNext = fTcalPar->GetSignalTcalParams(rank+1);
    iTf_ns = iPar + r*(iParNext -  iPar);
  }
  
  //std::cout << "Tf_ns=" << iTf_ns << ", iTc_ns=" << iTc_ns << " : TimeNs = " << 5.*iTc_ns - iTf_ns << std::endl;
  return (iTc_ns - iTf_ns);
}

ClassImp(R3BSofSciMapped2Tcal)
