#include "R3BSofToFWMapped2Tcal.h"
#include "R3BSofToFWMappedData.h"

#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "FairRootManager.h"
#include "FairLogger.h"


R3BSofToFWMapped2Tcal::R3BSofToFWMapped2Tcal()
  : FairTask("R3BSofToFWMapped2Tcal",1)
  , fMapped(NULL)
  , fTcalPar(NULL)
  , fTcal(new TClonesArray("R3BSofToFWTcalData"))
  , fNumTcal(0)
  , fNevent(0)
{
}


R3BSofToFWMapped2Tcal::~R3BSofToFWMapped2Tcal()
{
  if(fTcal){
    delete fTcal;
    fTcal=NULL;
  }
}


InitStatus R3BSofToFWMapped2Tcal::Init()
{

  LOG(INFO) << "R3BSofToFWMapped2Tcal: Init";

  FairRootManager* rm = FairRootManager::Instance();
  if (!rm) { 
    LOG(ERROR) << "R3BSofToFWMapped2Tcal::Init() Couldn't instance the FairRootManager";
    return kFATAL;
  }

  // --- ----------------- --- //
  // --- INPUT MAPPED DATA --- //
  // --- ----------------- --- //

  // scintillator at S2 and cave C
  fMapped = (TClonesArray*)rm->GetObject("SofToFWMappedData");  // see Instance->Register() in R3BSofToFWReader.cxx
  if (!fMapped){
    LOG(ERROR)<<"R3BSofToFWMapped2Tcal::Init() Couldn't get handle on SofToFWMappedData container";
    return kFATAL;
  }
  else
    LOG(INFO) << " R3BSofToFWMapped2Tcal::Init() SofToFWMappedData items found";
  

  // --- -------------------------- --- //
  // --- CHECK THE TCALPAR VALIDITY --- //
  // --- -------------------------- --- //
  if(fTcalPar->GetNumSignals()==0){
    LOG(ERROR) << " There are no Tcal parameters for SofToFW";
    return kFATAL;
  }
  else{
    LOG(INFO) << "R3BSofToFWMapped2Tcal::Init() : fNumSignals=" << fTcalPar->GetNumSignals();
    LOG(INFO) << " R3BSofToFWMapped2Tcal::Init() : fNumDetectors=" << fTcalPar->GetNumDetectors();
    LOG(INFO) << "  R3BSofToFWMapped2Tcal::Init() : fNumChannels=" << fTcalPar->GetNumChannels();
  }

  // --- ---------------- --- //
  // --- OUTPUT TCAL DATA --- //
  // --- ---------------- --- //
  
  // Register output array in tree
  rm->Register("SofToFWTcalData","SofToFW", fTcal, kTRUE);

  LOG(INFO) << "R3BSofToFWMapped2Tcal: Init DONE !";

  return kSUCCESS;
}


void R3BSofToFWMapped2Tcal::SetParContainers()
{
  fTcalPar = (R3BSofTcalPar*)FairRuntimeDb::instance()->getContainer("SofToFWTcalPar");
  if (!fTcalPar){
    LOG(ERROR) << "R3BSofToFWMapped2Tcal::SetParContainers() : Could not get access to SofToFWTcalPar-Container.";
    return;
  }
  else
    LOG(INFO) << "R3BSofToFWMapped2Tcal::SetParContainers() : SofToFWTcalPar-Container found with " << fTcalPar->GetNumSignals() << " signals";
}


InitStatus R3BSofToFWMapped2Tcal::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}


void R3BSofToFWMapped2Tcal::Exec(Option_t* option)
{
  UShort_t iDet;
  UShort_t iCh;
  UInt_t iTf;
  UInt_t iTc;
  Double_t tns;

  Int_t nHitsPerEvent_SofToFW = fMapped->GetEntries();
  for(int ihit=0; ihit<nHitsPerEvent_SofToFW; ihit++){
    R3BSofToFWMappedData* hit = (R3BSofToFWMappedData*)fMapped->At(ihit);
    if(!hit) continue;
    iDet = hit->GetDetector();
    iCh  = hit->GetPmt();
    iTf  = hit->GetTimeFine();
    iTc  = hit->GetTimeCoarse();
    if((iDet<1)||(iDet>fTcalPar->GetNumDetectors())) {
      LOG(INFO) << "R3BSofToFWMapped2Tcal::Exec() : In SofToFWMappedData, iDet = " << iDet << "is out of range, item skipped ";
      continue;
    }
    if((iCh<1)||(iCh>fTcalPar->GetNumChannels())) {
      LOG(INFO) << "R3BSofToFWMapped2Tcal::Exec() : In SofToFWMappedData, iCh = " << iCh << "is out of range, item skipped ";
      continue;
    }
    tns = CalculateTimeNs(iDet,iCh,iTf,iTc);
    new((*fTcal)[fNumTcal++]) R3BSofToFWTcalData(iDet,iCh,tns);
  }
    
  ++fNevent;

}  

void R3BSofToFWMapped2Tcal::FinishEvent()
{
  fTcal->Clear();
  fNumTcal = 0;
}


void R3BSofToFWMapped2Tcal::FinishTask()
{

}



Double_t R3BSofToFWMapped2Tcal::CalculateTimeNs(UShort_t iDet, UShort_t iCh, UInt_t iTf, UInt_t iTc)
{
  UInt_t   rank = iTf + fTcalPar->GetNumTcalParsPerSignal()*((iDet-1)*fTcalPar->GetNumChannels() + (iCh-1));
  Double_t iPar = (Double_t)fTcalPar->GetSignalTcalParams(rank);
  Double_t r = (Double_t)rand.Rndm()-0.5;
  Double_t iTf_ns;
  Double_t iTc_ns = (Double_t)iTc*5.;

  if(r<0){
    Double_t iParPrev = fTcalPar->GetSignalTcalParams(rank-1);
    iTf_ns = iPar + r*(iPar -  iParPrev);
  }
  else{
    Double_t iParNext = fTcalPar->GetSignalTcalParams(rank+1);
    iTf_ns = iPar + r*(iParNext -  iPar);
  }
  
  return (iTc_ns - iTf_ns);
}

ClassImp(R3BSofToFWMapped2Tcal)
