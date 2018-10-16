#include "R3BSofTrimMapped2TcalPar.h"

#include "R3BSofMusicMadcVftxMappedData.h"
#include "R3BSofTcalPar.h"

#include "R3BEventHeader.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"
#include "TGeoManager.h"

#include "TClonesArray.h"
#include "TGeoMatrix.h"
#include "TH1F.h"
#include "TF1.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TRandom.h"
#include "TVector3.h"

#include <iostream>
#include <stdlib.h>


//R3BSofTrimMapped2TcalPar: Default Constructor --------------------------
R3BSofTrimMapped2TcalPar::R3BSofTrimMapped2TcalPar() 
  : FairTask("R3BSofTrimMapped2TcalPar",1)
  , fNumDetectors(1)
  , fNumSections(3)
  , fNumChannels(6)
  , fNumTcalParsPerSignal(1000)
  , fMinStatistics(0)
  , fMappedTrim(NULL)
  , fTcalPar(NULL)
  , fOutputFile(NULL) 
{
  fNumSignals = fNumDetectors * fNumSections * fNumChannels;
}

//R3BSofTrimMapped2TcalPar: Standard Constructor --------------------------
R3BSofTrimMapped2TcalPar::R3BSofTrimMapped2TcalPar(const char* name, Int_t iVerbose) 
  : FairTask(name, iVerbose)
  , fNumDetectors(1)
  , fNumSections(3)
  , fNumChannels(6)
  , fNumTcalParsPerSignal(1000)
  , fMinStatistics(0)
  , fMappedTrim(NULL)
  , fTcalPar(NULL)
  , fOutputFile(NULL) 

{
  fNumSignals = fNumDetectors * fNumSections * fNumChannels;
}

//R3BSofTrimMapped2TcalPar: Destructor ----------------------------------------
R3BSofTrimMapped2TcalPar::~R3BSofTrimMapped2TcalPar() 
{
  if(fTcalPar) delete fTcalPar;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofTrimMapped2TcalPar::Init() {

  LOG(INFO) << "R3BSofTrimMapped2TcalPar: Init" << FairLogger::endl;

  FairRootManager* rm = FairRootManager::Instance();
  if (!rm) { return kFATAL;}
  
  // --- ----------------- --- //
  // --- INPUT MAPPED DATA --- //
  // --- ----------------- --- //

  fMappedTrim = (TClonesArray*)rm->GetObject("SofTrimMadcVftx");        // see Instance->Register in R3BSofTrimMadcVftxReader.cxx
  if (!fMappedTrim){return kFATAL;}


  // --- ---------------------------------- --- //
  // --- SOF TRIM TCAL PARAMETERS CONTAINER --- //
  // --- ---------------------------------- --- //
  FairRuntimeDb* rtdb = FairRuntimeDb::instance();
  if (!rtdb) { return kFATAL;}  
  fTcalPar=(R3BSofTcalPar*)rtdb->getContainer("SofTrimTcalPar");
  if (!fTcalPar) {
    LOG(ERROR)<<"R3BSofTrimMapped2TcalPar::Init() Couldn't get handle on SofTrimTcalPar container"<<FairLogger::endl;
    return kFATAL;
  }

  // --- ---------------------- --- //
  // --- HISTOGRAMS DECLARATION --- //
  // --- ---------------------- --- //

  char name[100];  
  fh_TimeFineBin = new TH1F*[fNumSignals];
  fh_TimeFineNs  = new TH1F*[fNumSignals];
  for(Int_t plane=0; plane<fNumDetectors; plane++){
    for(Int_t section=0; section<fNumSections; section++){
      for(Int_t anode=0; anode<fNumChannels; anode++){
	sprintf(name,"TimeFineBin_Trim_P%i_S%i_A%i_Sig%i",plane+1,section+1,anode+1,plane*fNumSections*fNumChannels+section*fNumChannels+anode);
	fh_TimeFineBin[plane*fNumSections*fNumChannels+section*fNumChannels+anode] = new TH1F(name,name,fNumTcalParsPerSignal,0,fNumTcalParsPerSignal);
	sprintf(name,"TimeFineNs_Trim_P%i_S%i_A%i_Sig%i",plane+1,section+1,anode+1,plane*fNumSections*fNumChannels+section*fNumChannels+anode);
	fh_TimeFineNs[plane*fNumSections*fNumChannels+section*fNumChannels+anode] = new TH1F(name,name,fNumTcalParsPerSignal,0,fNumTcalParsPerSignal);
      }
    }
  }
  
  return kSUCCESS;
}

// -----   Public method ReInit   --------------------------------------------
InitStatus R3BSofTrimMapped2TcalPar::ReInit() {
  
  
  return kSUCCESS;
}

// -----   Public method Exec   --------------------------------------------
void R3BSofTrimMapped2TcalPar::Exec(Option_t* opt) {
  
  // --- --------------------- --- //
  // --- LOOP OVER MAPPED HITS --- //
  // --- --------------------- --- //
    
  // nHits = number of hits per event
  UInt_t nHits    = fMappedTrim->GetEntries();  

  // loop over the number of hits per event
  for (UInt_t ihit=0; ihit<nHits; ihit++){
    R3BSofMusicMadcVftxMappedData* hit = (R3BSofMusicMadcVftxMappedData*)fMappedTrim->At(ihit);
    if (!hit){
      LOG(WARNING) << "R3BSofTrimMapped2TcalPar::Exec() : could not get hit" << FairLogger::endl;
      continue; // should not happen
    }           
    
    // *** ******************************************************************** *** //
    // *** Mapped and TcalPar are 1-based for the plane/section/anode numbering *** //
    // *** ******************************************************************** *** //
    // *** GEOMETRY OF THE TRIPLE MUSIC                                         *** // 
    // ***        * ONE SINGLE ANODE PLANE                ---> plane   = 1      *** //
    // ***        * THREE SECTIONS ALONG THE Z AXIS       ---> section = [1..3] *** //
    // ***        * SIX ANODES PER SECTION                ---> anode   = [1..6] *** //
    // *** ******************************************************************** *** //

    UInt_t iSignal = (hit->GetPlane()-1)*fNumSections*fNumChannels + (hit->GetSection()-1)*fNumChannels + hit->GetAnode()-1;
    if((0<=iSignal)&&(iSignal<fNumSignals))
      fh_TimeFineBin[iSignal]->Fill(hit->GetTimeFine());
    else
      LOG(ERROR) << "R3BSofTrimMapped2TcalPar::Exec() Number of signals out of range: "<< iSignal << " instead of [0,"<< fNumSignals << "]" << FairLogger::endl;
      
  }// end of loop over the number of hits per event in MappedTrim
}

// ---- Public method Reset   --------------------------------------------------
void R3BSofTrimMapped2TcalPar::Reset() 
{
}

void R3BSofTrimMapped2TcalPar::FinishEvent() 
{
}

// ---- Public method Finish   --------------------------------------------------
void R3BSofTrimMapped2TcalPar::FinishTask() 
{  
  CalculateVftxTcalParams();
  fTcalPar->printParams();
}


//------------------
void R3BSofTrimMapped2TcalPar::CalculateVftxTcalParams()
{
  LOG(INFO) << "R3BSofTrimMapped2TcalPar: CalculateVftxTcalParams()" << FairLogger::endl;
  
  fTcalPar->SetNumDetectors(fNumDetectors);
  fTcalPar->SetNumSections(fNumSections);
  fTcalPar->SetNumChannels(fNumChannels);
  fTcalPar->SetNumSignals(fNumDetectors,fNumSections,fNumChannels);
  fTcalPar->SetNumTcalParsPerSignal(fNumTcalParsPerSignal);

  UInt_t IntegralTot;
  UInt_t IntegralPartial;
  Double_t Bin2Ns[fNumTcalParsPerSignal];

  for(Int_t sig=0; sig<fNumSignals; sig++){
    if(fh_TimeFineBin[sig]->GetEntries()>fMinStatistics){
      IntegralTot = fh_TimeFineBin[sig]->Integral();
      IntegralPartial = 0;
      for(Int_t bin=0; bin<fNumTcalParsPerSignal; bin++){
	IntegralPartial+=fh_TimeFineBin[sig]->GetBinContent(bin+1);
	Bin2Ns[bin] = 5.*((Double_t)IntegralPartial) / (Double_t)IntegralTot;
	fh_TimeFineNs[sig]->SetBinContent(bin+1,Bin2Ns[bin]);
	fTcalPar->SetSignalTcalParams(Bin2Ns[bin],sig*fNumTcalParsPerSignal+bin);
      }
    }
    fh_TimeFineNs[sig]->Write(); // empty if stat < fMinStatistics
    fh_TimeFineBin[sig]->Write();
  }
  fTcalPar->setChanged();
  return;
  
}

ClassImp(R3BSofTrimMapped2TcalPar)
  
