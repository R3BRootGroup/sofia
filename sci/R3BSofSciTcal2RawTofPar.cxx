#include "R3BSofSciTcal2RawTofPar.h"

#include "R3BSofSciTcalData.h"
#include "R3BSofSciRawTofPar.h"

#include "R3BEventHeader.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"
#include "TGeoManager.h"

#include "TClonesArray.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TRandom.h"
#include "TVector3.h"

#include <iostream>
#include <stdlib.h>






// *** ************************************ *** //
// *** SofSci Pmt Right (Tcal Data)         *** //
// ***     * channel=1                      *** //
// *** ************************************ *** //
// *** SofSci Pmt Left (Tcal Data)          *** //
// ***     * channel=2                      *** //
// *** ************************************ *** //
// *** SofSci Common Ref (Tcal Data)        *** //
// ***     * channel=3                      *** //
// *** ************************************ *** //


//R3BSofSciTcal2RawTofPar: Default Constructor --------------------------
R3BSofSciTcal2RawTofPar::R3BSofSciTcal2RawTofPar() 
  : FairTask("R3BSofSciTcal2RawTofPar",1)
  , fFirstStaSci(1)
  , fNumSignals(NUMBER_OF_SOFSCI_DETECTORS-1)
  , fNumParsPerSignal(2)
  , fMinStatistics(0)
  , fTcal(NULL)
  , fRawTofPar(NULL)
  , fOutputFile(NULL) 
{
}

//R3BSofSciTcal2RawTofPar: Standard Constructor --------------------------
R3BSofSciTcal2RawTofPar::R3BSofSciTcal2RawTofPar(const char* name, Int_t iVerbose) 
  : FairTask(name, iVerbose)
  , fFirstStaSci(1)
  , fNumSignals(NUMBER_OF_SOFSCI_DETECTORS-1)
  , fNumParsPerSignal(2)
  , fMinStatistics(0)
  , fTcal(NULL)
  , fRawTofPar(NULL)
  , fOutputFile(NULL) 

{
}

//R3BSofSciTcal2RawTofPar: Destructor ----------------------------------------
R3BSofSciTcal2RawTofPar::~R3BSofSciTcal2RawTofPar() 
{
  if(fRawTofPar) delete fRawTofPar;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofSciTcal2RawTofPar::Init() {

  LOG(INFO) << "R3BSofSciTcal2RawTofPar: Init";

  FairRootManager* rm = FairRootManager::Instance();
  if (!rm) { return kFATAL;}
  
  // --- --------------- --- //
  // --- INPUT TCAL DATA --- //
  // --- ---------------- --- //

  fTcal = (TClonesArray*)rm->GetObject("SofSciTcalData");      
  if (!fTcal){
    LOG(ERROR)<<"R3BSofSciTcal2RawTofPar::Init() Couldn't get handle on SofSciTcalData container";
    return kFATAL;
  }

  // --- ---------------------------------------- --- //
  // --- SOF SCI SINGLE TCAL PARAMETERS CONTAINER --- //
  // --- ---------------------------------------- --- //

  FairRuntimeDb* rtdb = FairRuntimeDb::instance();
  if (!rtdb) { return kFATAL;}  

  fRawTofPar=(R3BSofSciRawTofPar*)rtdb->getContainer("SofSciRawTofPar");
  if (!fRawTofPar) {
    LOG(ERROR)<<"R3BSofSciTcal2RawTofPar::Init() Couldn't get handle on SofSciRawTofPar container";
    return kFATAL;
  }

  // --- ---------------------- --- //
  // --- HISTOGRAMS DECLARATION --- //
  // --- ---------------------- --- //

  char name[100];  
  fh_RawTofMult1 = new TH1D*[fNumSignals];
  for(Int_t detstart=0; detstart<NUMBER_OF_SOFSCI_DETECTORS-1; detstart++){
    sprintf(name,"TofRaw_Sci%i_to_Sci%i",detstart+1,ID_SOFSCI_CAVEC);
     fh_RawTofMult1[detstart] = new TH1D(name,name,40000,-2000,2000);
  }

  return kSUCCESS;
}

// -----   Public method ReInit   --------------------------------------------
InitStatus R3BSofSciTcal2RawTofPar::ReInit() {
  return kSUCCESS;
}

// -----   Public method Exec   --------------------------------------------
void R3BSofSciTcal2RawTofPar::Exec(Option_t* opt) {

  // --- ------------------------------ --- //
  // --- LOOP OVER TCAL HITS FOR SofSci --- //
  // --- ------------------------------ --- //
  
  // nHitsSci = number of hits per event
  UInt_t nHitsSci = fTcal->GetEntries();    // can be very high especially for S2 detector
  UShort_t mult[NUMBER_OF_SOFSCI_DETECTORS*NUMBER_OF_SOFSCI_CHANNELS];
  Double_t iRawTimeNs[NUMBER_OF_SOFSCI_DETECTORS*NUMBER_OF_SOFSCI_CHANNELS];
  Double_t iTrawStart, iTrawStop;
  UShort_t iDet; // 0 based Det number
  UShort_t iPmt; // 0 based Pmt number
  UShort_t rank;

  for(UShort_t d=0; d<NUMBER_OF_SOFSCI_DETECTORS; d++){
    for(UShort_t ch=0; ch<NUMBER_OF_SOFSCI_CHANNELS; ch++){
      mult[d*NUMBER_OF_SOFSCI_CHANNELS+ch] = 0;
      iRawTimeNs[d*NUMBER_OF_SOFSCI_CHANNELS+ch] = 0;
    }
  }

  // CALCULATE THE MULTIPLICITY FOR EACH SIGNAL
  for (UInt_t ihit=0; ihit<nHitsSci; ihit++){
    R3BSofSciTcalData* hitSci = (R3BSofSciTcalData*)fTcal->At(ihit);
    if (!hitSci){
      LOG(WARNING) << "R3BSofSciTcal2RawTofPar::Exec() : could not get hitSci";
      continue; // should not happen
    }           
    iDet = hitSci->GetDetector()-1; // get the 0 based DetiMax=20 number
    iPmt = hitSci->GetPmt()-1;      // get the 0 based Pmt number
    iRawTimeNs[iDet*NUMBER_OF_SOFSCI_CHANNELS+iPmt] = hitSci->GetRawTimeNs();
    mult[iDet*NUMBER_OF_SOFSCI_CHANNELS+iPmt]++;
  }// end of for(ihit) 

  // FILL THE HISTOGRAM ONLY FOR MULT=1 IN RIGHT AND MULT=1 IN LEFT
  UShort_t dstop = ID_SOFSCI_CAVEC-1;
  for(UShort_t dstart=0; dstart<NUMBER_OF_SOFSCI_DETECTORS-1; dstart++){
    // check if mult=1 at RIGHT PMT [0] and mult=1 at LEFT PMT [1]
    if( (mult[dstart*NUMBER_OF_SOFSCI_CHANNELS]==1) && (mult[dstart*NUMBER_OF_SOFSCI_CHANNELS+1]==1) &&
        (mult[dstop*NUMBER_OF_SOFSCI_CHANNELS]==1)  && (mult[dstop*NUMBER_OF_SOFSCI_CHANNELS+1]==1) ){
      iTrawStart = 0.5*(iRawTimeNs[dstart*NUMBER_OF_SOFSCI_CHANNELS]+iRawTimeNs[dstart*NUMBER_OF_SOFSCI_CHANNELS+1]);
      iTrawStop = 0.5*(iRawTimeNs[dstop*NUMBER_OF_SOFSCI_CHANNELS]+iRawTimeNs[dstop*NUMBER_OF_SOFSCI_CHANNELS+1]);
      fh_RawTofMult1[dstart]->Fill(iTrawStop-iTrawStart+iRawTimeNs[dstart*NUMBER_OF_SOFSCI_CHANNELS+2]-iRawTimeNs[dstop*NUMBER_OF_SOFSCI_CHANNELS+2]);
    }
  }
}


// ---- Public method Reset   --------------------------------------------------
void R3BSofSciTcal2RawTofPar::Reset() 
{
}

void R3BSofSciTcal2RawTofPar::FinishEvent() 
{
}

// ---- Public method Finish   --------------------------------------------------
void R3BSofSciTcal2RawTofPar::FinishTask() 
{  
  CalculateRawTofParams();
  fRawTofPar->printParams();
}


// ------------------------------
void R3BSofSciTcal2RawTofPar::CalculateRawTofParams()
{
  LOG(INFO) << "R3BSofSciTcal2RawTofPar: CalculateRawTofParams()";
  
  fRawTofPar->SetFirstStart(fFirstStaSci);
  fRawTofPar->SetNumSignals(fNumSignals);
  fRawTofPar->SetNumParsPerSignal(fNumParsPerSignal);

  Double_t iMax=0;
  Int_t bin, binLimit; 
  for(Int_t sig=0; sig<fNumSignals; sig++){
    iMax = fh_RawTofMult1[sig]->GetMaximum();
    if(fh_RawTofMult1[sig]->GetEntries()>fMinStatistics && iMax>0){
      //LOWER LIMIT
      bin=1;
      binLimit=1;
      while ((bin<=fh_RawTofMult1[sig]->GetNbinsX())&&(binLimit==1)){
	if(fh_RawTofMult1[sig]->GetBinContent(bin)>(iMax/1000.)) binLimit=bin;
	bin++;
      }
      fRawTofPar->SetSignalParams(fh_RawTofMult1[sig]->GetBinLowEdge(binLimit),sig*2);
      //HIGHER LIMIT
      bin=fh_RawTofMult1[sig]->GetNbinsX();
      binLimit=fh_RawTofMult1[sig]->GetNbinsX();
      while ((bin>=1)&&(binLimit==fh_RawTofMult1[sig]->GetNbinsX())){
	if(fh_RawTofMult1[sig]->GetBinContent(bin)>(iMax/1000.)) binLimit=bin;
	bin--;
      }
      fRawTofPar->SetSignalParams(fh_RawTofMult1[sig]->GetBinLowEdge(binLimit),sig*2+1);
    }
    fh_RawTofMult1[sig]->Write();
  }

  fRawTofPar->setChanged();
  return;
  
}


ClassImp(R3BSofSciTcal2RawTofPar)
  
