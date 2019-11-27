#include "R3BSofSciMapped2Tcal.h"
#include <iomanip>

// --- Default Constructor
R3BSofSciMapped2Tcal::R3BSofSciMapped2Tcal()
  : FairTask("R3BSofSciMapped2Tcal",1)
  , fNumTcal(0)
  , fNevent(0)
  , fTcal(NULL)
  , fMapped(NULL)
  , fTcalPar(NULL)
  , fOnline (kFALSE)
{
}

// --- Standard Constructor
R3BSofSciMapped2Tcal::R3BSofSciMapped2Tcal(const char* name, Int_t iVerbose)
  : FairTask(name,iVerbose)
  , fNumTcal(0)
  , fNevent(0)
  , fTcal(NULL)
  , fMapped(NULL)
  , fTcalPar(NULL)
  , fOnline (kFALSE)
{
}

// --- Destructor
R3BSofSciMapped2Tcal::~R3BSofSciMapped2Tcal()
{
  LOG(INFO) << "R3BSofSciMapped2Tcal: Delete instance";
  if(fMapped){
    delete fMapped;
  }
  if(fTcal){
    delete fTcal;
  }
}

// --- Parameter container : reading SofSciTcalPar from FairRuntimeDb
void R3BSofSciMapped2Tcal::SetParContainers()
{
  FairRuntimeDb* rtdb = FairRuntimeDb::instance();
  if(!rtdb){
    LOG(ERROR) << "FairRuntimeDb not opened!";
  }

  fTcalPar = (R3BSofTcalPar*)rtdb->getContainer("SofSciTcalPar");
  if (!fTcalPar){
    LOG(ERROR) << "R3BSofSciMapped2Tcal::SetParContainers() : Could not get access to SofSciTcalPar-Container.";
    return;
  }
  else{
    LOG(INFO) << "R3BSofSciMapped2Tcal::SetParContainers() : SofSciTcalPar-Container found with " 
	      << fTcalPar->GetNumSignals() << " signals";
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
  fMapped = (TClonesArray*)rm->GetObject("SofSciMappedData"); 
  if (!fMapped){
    LOG(ERROR)<<"R3BSofSciMapped2Tcal::Init() Couldn't get handle on SofSciMappedData container";
    return kFATAL;
  }
  else
    LOG(INFO) << "R3BSofSciMapped2Tcal::Init() SofSciMappedData items found";
  
  // --- ---------------- --- //
  // --- OUTPUT TCAL DATA --- //
  // --- ---------------- --- //
  fTcal = new TClonesArray("R3BSofSciTcalData");
  rm->Register("SofSciTcalData","SofSci", fTcal, kTRUE);
  LOG(INFO) << "R3BSofSciMapped2Tcal::Init() output R3BSofSciTcalData ";


  // --- -------------------------- --- //
  // --- CHECK THE TCALPAR VALIDITY --- //
  // --- -------------------------- --- //
  if(fTcalPar->GetNumSignals()==0){
    LOG(ERROR) << "R3BSofSciMapped2Tcal::Init() : There are no Tcal parameters for SofSci";
    return kFATAL;
  }
  else{
    LOG(INFO) << "R3BSofSciMapped2Tcal::Init(): fNumSignals=" << fTcalPar->GetNumSignals();
    LOG(INFO) << " R3BSofSciMapped2Tcal::Init(): fNumDetectors=" << fTcalPar->GetNumDetectors(); 
    LOG(INFO) << "  R3BSofSciMapped2Tcal::Init(): fNumChannels=" << fTcalPar->GetNumChannels();
  }
  return kSUCCESS;
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

  Int_t nHitsPerEvent_SofSci = fMapped->GetEntries();
  for(int ihit=0; ihit<nHitsPerEvent_SofSci; ihit++){
    R3BSofSciMappedData* hit = (R3BSofSciMappedData*)fMapped->At(ihit);
    if(!hit) continue;
    iDet = hit->GetDetector();
    iCh  = hit->GetPmt();
    iTf  = hit->GetTimeFine();
    iTc  = hit->GetTimeCoarse();
    if((iDet<1)||(iDet>fTcalPar->GetNumDetectors())) {
      LOG(INFO) << "R3BSofSciMapped2Tcal::Exec() : In SofSciMappedData, iDet = " << iDet << "is out of range, item skipped ";
      continue;
    }
    if((iCh<1)||(iCh>fTcalPar->GetNumChannels())) {
      LOG(INFO) << "R3BSofSciMapped2Tcal::Exec() : In SofSciMappedData, iCh = " << iCh << "is out of range, item skipped ";
      continue;
    }
    tns = CalculateTimeNs(iDet,iCh,iTf,iTc);
    new((*fTcal)[fNumTcal++]) R3BSofSciTcalData(iDet,iCh,tns);
  }
    
  ++fNevent;

}  

void R3BSofSciMapped2Tcal::FinishEvent()
{
  fTcal->Clear();
  fNumTcal = 0;
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
