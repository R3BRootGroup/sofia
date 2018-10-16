#include "R3BSofTwimMapped2TcalPar.h"

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


//R3BSofTwimMapped2TcalPar: Default Constructor --------------------------
R3BSofTwimMapped2TcalPar::R3BSofTwimMapped2TcalPar() 
  : FairTask("R3BSofTwimMapped2TcalPar",1)
  , fNumDetectors(2)
  , fNumSections(2)
  , fNumChannels(16)
  , fNumTcalParsPerSignal(1000)
  , fMinStatistics(0)
  , fMappedTwim(NULL)
  , fTcalPar(NULL)
  , fOutputFile(NULL) 
{
  fNumSignals = fNumDetectors * fNumSections * fNumChannels;
}

//R3BSofTwimMapped2TcalPar: Standard Constructor --------------------------
R3BSofTwimMapped2TcalPar::R3BSofTwimMapped2TcalPar(const char* name, Int_t iVerbose) 
  : FairTask(name, iVerbose)
  , fNumDetectors(2)
  , fNumSections(2)
  , fNumChannels(16)
  , fNumTcalParsPerSignal(1000)
  , fMinStatistics(0)
  , fMappedTwim(NULL)
  , fTcalPar(NULL)
  , fOutputFile(NULL) 

{
  fNumSignals = fNumDetectors * fNumSections * fNumChannels;
}

//R3BSofTwimMapped2TcalPar: Destructor ----------------------------------------
R3BSofTwimMapped2TcalPar::~R3BSofTwimMapped2TcalPar() 
{
  if(fTcalPar) delete fTcalPar;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofTwimMapped2TcalPar::Init() {

  LOG(INFO) << "R3BSofTwimMapped2TcalPar: Init" << FairLogger::endl;

  FairRootManager* rm = FairRootManager::Instance();
  if (!rm) { return kFATAL;}
  
  // --- ----------------- --- //
  // --- INPUT MAPPED DATA --- //
  // --- ----------------- --- //

  fMappedTwim = (TClonesArray*)rm->GetObject("SofTwimMadcVftx");        // see Instance->Register in R3BSofTwimMadcVftxReader.cxx
  if (!fMappedTwim){return kFATAL;}


  // --- ---------------------------------- --- //
  // --- SOF TWIM TCAL PARAMETERS CONTAINER --- //
  // --- ---------------------------------- --- //
  FairRuntimeDb* rtdb = FairRuntimeDb::instance();
  if (!rtdb) { return kFATAL;}  
  fTcalPar=(R3BSofTcalPar*)rtdb->getContainer("SofTwimTcalPar");
  if (!fTcalPar) {
    LOG(ERROR)<<"R3BSofTwimMapped2TcalPar::Init() Couldn't get handle on SofTwimTcalPar container"<<FairLogger::endl;
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
	sprintf(name,"TimeFineBin_Twim_P%i_S%i_A%i_Sig%i",plane+1,section+1,anode+1,plane*fNumSections*fNumChannels+section*fNumChannels+anode);
	fh_TimeFineBin[plane*fNumSections*fNumChannels+section*fNumChannels+anode] = new TH1F(name,name,fNumTcalParsPerSignal,0,fNumTcalParsPerSignal);
	sprintf(name,"TimeFineNs_Twim_P%i_S%i_A%i_Sig%i",plane+1,section+1,anode+1,plane*fNumSections*fNumChannels+section*fNumChannels+anode);
	fh_TimeFineNs[plane*fNumSections*fNumChannels+section*fNumChannels+anode] = new TH1F(name,name,fNumTcalParsPerSignal,0,fNumTcalParsPerSignal);
      }
    }
  }
  
  return kSUCCESS;
}

// -----   Public method ReInit   --------------------------------------------
InitStatus R3BSofTwimMapped2TcalPar::ReInit() {
  
  
  return kSUCCESS;
}

// -----   Public method Exec   --------------------------------------------
void R3BSofTwimMapped2TcalPar::Exec(Option_t* opt) {
  
  // --- --------------------- --- //
  // --- LOOP OVER MAPPED HITS --- //
  // --- --------------------- --- //
    
  // nHits = number of hits per event
  UInt_t nHits    = fMappedTwim->GetEntries();  

  // loop over the number of hits per event
  for (UInt_t ihit=0; ihit<nHits; ihit++){
    R3BSofMusicMadcVftxMappedData* hit = (R3BSofMusicMadcVftxMappedData*)fMappedTwim->At(ihit);
    if (!hit){
      LOG(WARNING) << "R3BSofTwimMapped2TcalPar::Exec() : could not get hit" << FairLogger::endl;
      continue; // should not happen
    }           
    
    // *** ******************************************************************** *** //
    // *** Mapped and TcalPar are 1-based for the plane/section/anode numbering *** //
    // *** ******************************************************************** *** //
    // *** GEOMETRY OF THE TWIN-MUSIC                                           *** // 
    // ***      * TWO ANODE PLANES                                              *** //
    // ***        ---> left plane = 1                                           *** //
    // ***        ---> right plane = 2                                          *** //
    // ***      * TWO SECTIONS (EACH ANODE PLANE IS VERTICALLY SEGMENTED)       *** //
    // ***        ---> lower section = 1                                        *** //
    // ***        ---> upper section = 2                                        *** //
    // ***      * SIXTEEN ANODES PER SECTION ALONG THE BEAM DIRECTION (Z AXIS)  *** //
    // ***        ---> anode = [1..16]                                          *** //
    // *** ******************************************************************** *** //

    UInt_t iSignal = (hit->GetPlane()-1)*fNumSections*fNumChannels + (hit->GetSection()-1)*fNumChannels + hit->GetAnode()-1;
    if((0<=iSignal)&&(iSignal<fNumSignals))
      fh_TimeFineBin[iSignal]->Fill(hit->GetTimeFine());
    else
      LOG(ERROR) << "R3BSofTwimMapped2TcalPar::Exec() Number of signals out of range: "<< iSignal << " instead of [0,"<< fNumSignals << "]" << FairLogger::endl;
      
  }// end of loop over the number of hits per event in MappedTwim
}

// ---- Public method Reset   --------------------------------------------------
void R3BSofTwimMapped2TcalPar::Reset() 
{
}

void R3BSofTwimMapped2TcalPar::FinishEvent() 
{
}

// ---- Public method Finish   --------------------------------------------------
void R3BSofTwimMapped2TcalPar::FinishTask() 
{  
  CalculateVftxTcalParams();
  fTcalPar->printParams();
}


//------------------
void R3BSofTwimMapped2TcalPar::CalculateVftxTcalParams()
{
  LOG(INFO) << "R3BSofTwimMapped2TcalPar: CalculateVftxTcalParams()" << FairLogger::endl;
  
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
    fh_TimeFineNs[sig]->Write();  // will be empty if stat < fMinStatistics
    fh_TimeFineBin[sig]->Write();
  }
  fTcalPar->setChanged();
  return;
  
}

ClassImp(R3BSofTwimMapped2TcalPar)
  
