#include "R3BSofSciTcal2RawPosPar.h"

#include "R3BSofSciTcalData.h"
#include "R3BSofSciRawPosPar.h"

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


// *** ******************************************* *** //
// *** SofSci at S2, Pmt Right (Tcal Data)         *** //
// ***     * det=1                                 *** //
// ***     * channel=1                             *** //
// *** ******************************************* *** //
// *** SofSci at S2, Pmt Left (Tcal Data)          *** //
// ***     * det=1                                 *** //
// ***     * channel=2                             *** //
// *** ******************************************* *** //
// *** SofSci at S2, Common Ref (Tcal Data)        *** //
// ***     * det=1                                 *** //
// ***     * channel=3                             *** //
// *** ******************************************* *** //
// *** SofSci at Cave C, Pmt Right (Tcal Data)     *** //
// ***     * det=2                                 *** //
// ***     * channel=1                             *** //
// *** ******************************************* *** //
// *** SofSci at Cave C, Pmt Left (Tcal Data)      *** //
// ***     * det=2                                 *** //
// ***     * channel=2                             *** //
// *** ******************************************* *** //
// *** SofSci at Cave C, Common Ref (Tcal Data)    *** //
// ***     * det=2                                 *** //
// ***     * channel=3                             *** //
// *** ******************************************* *** //


//R3BSofSciTcal2RawPosPar: Default Constructor --------------------------
R3BSofSciTcal2RawPosPar::R3BSofSciTcal2RawPosPar() 
  : FairTask("R3BSofSciTcal2RawPosPar",1)
  , fNumDetectors(NUMBER_OF_DETECTORS)
  , fNumChannels(NUMBER_OF_CHANNELS)
  , fNumSignals(NUMBER_OF_SIGNALS)
  , fNumParsPerSignal(2)
  , fMinStatistics(0)
  , fTcal(NULL)
  , fRawPosPar(NULL)
  , fOutputFile(NULL) 
{
}

//R3BSofSciTcal2RawPosPar: Standard Constructor --------------------------
R3BSofSciTcal2RawPosPar::R3BSofSciTcal2RawPosPar(const char* name, Int_t iVerbose) 
  : FairTask(name, iVerbose)
  , fNumDetectors(NUMBER_OF_DETECTORS)
  , fNumChannels(NUMBER_OF_CHANNELS)
  , fNumSignals(NUMBER_OF_SIGNALS)
  , fNumParsPerSignal(2)
  , fMinStatistics(0)
  , fTcal(NULL)
  , fRawPosPar(NULL)
  , fOutputFile(NULL) 

{
}

//R3BSofSciTcal2RawPosPar: Destructor ----------------------------------------
R3BSofSciTcal2RawPosPar::~R3BSofSciTcal2RawPosPar() 
{
  if(fRawPosPar) delete fRawPosPar;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofSciTcal2RawPosPar::Init() {

  LOG(INFO) << "R3BSofSciTcal2RawPosPar: Init";

  FairRootManager* rm = FairRootManager::Instance();
  if (!rm) { return kFATAL;}
  
  // --- --------------- --- //
  // --- INPUT TCAL DATA --- //
  // --- ---------------- --- //

  // scintillator at S2 and cave C
  fTcal = (TClonesArray*)rm->GetObject("SofSciTcalData");      
  if (!fTcal){
    LOG(ERROR)<<"R3BSofSciTcal2RawPosPar::Init() Couldn't get handle on SofSciTcalData container";
    return kFATAL;
  }


  // --- ---------------------------------------- --- //
  // --- SOF SCI SINGLE TCAL PARAMETERS CONTAINER --- //
  // --- ---------------------------------------- --- //

  FairRuntimeDb* rtdb = FairRuntimeDb::instance();
  if (!rtdb) { return kFATAL;}  

  fRawPosPar=(R3BSofSciRawPosPar*)rtdb->getContainer("SofSciRawPosPar");
  if (!fRawPosPar) {
    LOG(ERROR)<<"R3BSofSciTcal2RawPosPar::Init() Couldn't get handle on SofSciRawPosPar container";
    return kFATAL;
  }

  // --- ---------------------- --- //
  // --- HISTOGRAMS DECLARATION --- //
  // --- ---------------------- --- //

  char name[100];  
  fh_RawPosMult1 = new TH1D*[fNumSignals];
  for(Int_t det=0; det<fNumDetectors; det++){
    sprintf(name,"PosRaw_Sci%i",det+1);
    fh_RawPosMult1[det] = new TH1D(name,name,20000,-10,10);
  }
  
#if NUMBER_OF_DETECTORS==2
  sprintf(name,"RawTof_FromS2_to_CaveC");
  fh_RawTofMult1 = new TH1D(name,name,100000,-1000,1000);
#endif
  return kSUCCESS;
}

// -----   Public method ReInit   --------------------------------------------
InitStatus R3BSofSciTcal2RawPosPar::ReInit() {
  
  
  return kSUCCESS;
}

// -----   Public method Exec   --------------------------------------------
void R3BSofSciTcal2RawPosPar::Exec(Option_t* opt) {

  // --- ------------------------------ --- //
  // --- LOOP OVER TCAL HITS FOR SofSci --- //
  // --- ------------------------------ --- //
  
  // nHitsSci = number of hits per event
  UInt_t nHitsSci = fTcal->GetEntries();    // can be very high especially for S2 detector
  UShort_t mult[NUMBER_OF_DETECTORS*NUMBER_OF_CHANNELS];
  Double_t iRawTimeNs[NUMBER_OF_DETECTORS*NUMBER_OF_CHANNELS];
  UShort_t iDet; // 0 based Det number
  UShort_t iPmt; // 0 based Pmt number

  for(UShort_t d=0; d<NUMBER_OF_DETECTORS; d++){
    for(UShort_t ch=0; ch<NUMBER_OF_CHANNELS; ch++){
      mult[d*NUMBER_OF_CHANNELS+ch] = 0;
      iRawTimeNs[d*NUMBER_OF_CHANNELS+ch] = 0;
    }
  }

  // CALCULATE THE MULTIPLICITY FOR EACH SIGNAL
  for (UInt_t ihit=0; ihit<nHitsSci; ihit++){
    R3BSofSciTcalData* hitSci = (R3BSofSciTcalData*)fTcal->At(ihit);
    if (!hitSci){
      LOG(WARNING) << "R3BSofSciTcal2RawPosPar::Exec() : could not get hitSci";
      continue; // should not happen
    }           
    iDet = hitSci->GetDetector()-1; // get the 0 based Det number
    iPmt = hitSci->GetPmt()-1;      // get the 0 based Pmt number
    iRawTimeNs[iDet*NUMBER_OF_CHANNELS+iPmt] = hitSci->GetRawTimeNs();
    mult[iDet*NUMBER_OF_CHANNELS+iPmt]++;
  }// end of for(ihit) 

  // FILL THE HISTOGRAM ONLY FOR MULT=1 IN RIGHT AND MULT=1 IN LEFT
  for(UShort_t d=0; d<NUMBER_OF_DETECTORS; d++){
    // check if mult=1 at RIGHT PMT [0,3] and mult=1 at LEFT PMT [1,4]
    if((mult[d*NUMBER_OF_CHANNELS]==1)&&(mult[d*NUMBER_OF_CHANNELS+1]==1)){
       fh_RawPosMult1[d]->Fill(iRawTimeNs[d*NUMBER_OF_CHANNELS+1]-iRawTimeNs[d*NUMBER_OF_CHANNELS]);
    }
  }
}


// ---- Public method Reset   --------------------------------------------------
void R3BSofSciTcal2RawPosPar::Reset() 
{
}

void R3BSofSciTcal2RawPosPar::FinishEvent() 
{
}

// ---- Public method Finish   --------------------------------------------------
void R3BSofSciTcal2RawPosPar::FinishTask() 
{  
  CalculateRawPosRawPosParams();
  // TO DO AND SHOULD BE IN #define #else #endif  ON NUMBER_OF_DETECTORS CASE : CalculateRawTofRawPosParams();
  fRawPosPar->printParams();
}


// ------------------------------
void R3BSofSciTcal2RawPosPar::CalculateRawPosRawPosParams()
{
  LOG(INFO) << "R3BSofSciTcal2RawPosPar: CalculateRawPosRawPosParams()";
  
  fRawPosPar->SetNumDetectors(fNumDetectors);
  fRawPosPar->SetNumChannels(fNumChannels);
  fRawPosPar->SetNumSignals(fNumDetectors);
  fRawPosPar->SetNumParsPerSignal(fNumParsPerSignal);

  Double_t iMax;
  Int_t bin, binLimit; 
  for(Int_t sig=0; sig<fNumSignals; sig++){
    if(fh_RawPosMult1[sig]->GetEntries()>fMinStatistics){
      iMax = fh_RawPosMult1[sig]->GetMaximum();
      //LOWER LIMIT
      bin=1;
      binLimit=1;
      while ((bin<=fh_RawPosMult1[sig]->GetNbinsX())&&(binLimit==1)){
	if(fh_RawPosMult1[sig]->GetBinContent(bin)>(iMax/10000.)) binLimit=bin;
	bin++;
      }
      fRawPosPar->SetSignalParams(fh_RawPosMult1[sig]->GetBinLowEdge(binLimit),sig*2);
      //HIGHER LIMIT
      bin=fh_RawPosMult1[sig]->GetNbinsX();
      binLimit=fh_RawPosMult1[sig]->GetNbinsX();
      while ((bin>=1)&&(binLimit==fh_RawPosMult1[sig]->GetNbinsX())){
	if(fh_RawPosMult1[sig]->GetBinContent(bin)>(iMax/10000.)) binLimit=bin;
	bin--;
      }
      fRawPosPar->SetSignalParams(fh_RawPosMult1[sig]->GetBinLowEdge(binLimit),sig*2+1);
    }
    fh_RawPosMult1[sig]->Write();
  }

  fRawPosPar->setChanged();
  return;
  
}


ClassImp(R3BSofSciTcal2RawPosPar)
  
