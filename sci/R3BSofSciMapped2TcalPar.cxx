#include "R3BSofSciMapped2TcalPar.h"

#include "R3BSofSciMappedData.h"
#include "R3BSofComRefMappedData.h"
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


//R3BSofSciMapped2TcalPar: Default Constructor --------------------------
R3BSofSciMapped2TcalPar::R3BSofSciMapped2TcalPar() 
  : FairTask("R3BSofSciMapped2TcalPar",1)
  , fNumDetectors(2)
  , fNumSections(1)
  , fNumChannels(3)
  , fNumTcalParsPerSignal(1000)
  , fMinStatistics(0)
  , fMappedSci(NULL)
  , fMappedComRef(NULL)
  , fTcalPar(NULL)
  , fOutputFile(NULL) 
{
  fNumSignals = fNumDetectors * fNumSections * fNumChannels;
}

//R3BSofSciMapped2TcalPar: Standard Constructor --------------------------
R3BSofSciMapped2TcalPar::R3BSofSciMapped2TcalPar(const char* name, Int_t iVerbose) 
  : FairTask(name, iVerbose)
  , fNumDetectors(2)
  , fNumSections(1)
  , fNumChannels(3)
  , fNumTcalParsPerSignal(1000)
  , fMinStatistics(0)
  , fMappedSci(NULL)
  , fMappedComRef(NULL)
  , fTcalPar(NULL)
  , fOutputFile(NULL) 

{
  fNumSignals = fNumDetectors * fNumSections * fNumChannels;
}

//R3BSofSciMapped2TcalPar: Destructor ----------------------------------------
R3BSofSciMapped2TcalPar::~R3BSofSciMapped2TcalPar() 
{
  if(fTcalPar) delete fTcalPar;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofSciMapped2TcalPar::Init() {

  LOG(INFO) << "R3BSofSciMapped2TcalPar: Init" << FairLogger::endl;

  FairRootManager* rm = FairRootManager::Instance();
  if (!rm) { return kFATAL;}
  
  // --- ----------------- --- //
  // --- INPUT MAPPED DATA --- //
  // --- ----------------- --- //

  // Common reference signal
  fMappedComRef = (TClonesArray*)rm->GetObject("SofComRef");  // see Instance->Register in R3BSofComRefReader.cxx  
  if (!fMappedComRef){
    LOG(ERROR)<<"R3BSofSciMapped2TcalPar::Init() Couldn't get handle on SofComRef container"<<FairLogger::endl;
    return kFATAL;
  }

  // scintillator at S2 and cave C
  fMappedSci = (TClonesArray*)rm->GetObject("SofSci");        // see Instance->Register in R3BSofSciReader.cxx
  if (!fMappedSci){
    LOG(ERROR)<<"R3BSofSciMapped2TcalPar::Init() Couldn't get handle on SofSci container"<<FairLogger::endl;
    return kFATAL;
  }


  // --- --------------------------------- --- //
  // --- SOF SCI TCAL PARAMETERS CONTAINER --- //
  // --- --------------------------------- --- //

  FairRuntimeDb* rtdb = FairRuntimeDb::instance();
  if (!rtdb) { return kFATAL;}  

  fTcalPar=(R3BSofTcalPar*)rtdb->getContainer("SofSciTcalPar");
  if (!fTcalPar) {
    LOG(ERROR)<<"R3BSofSciMapped2TcalPar::Init() Couldn't get handle on SofSciTcalPar container"<<FairLogger::endl;
    return kFATAL;
  }

  // --- ---------------------- --- //
  // --- HISTOGRAMS DECLARATION --- //
  // --- ---------------------- --- //
  // --- In 1-based numbering:  --- //
  // --- detector 1 : at S2     --- //
  // --- detector 2 : at cave C --- //
  // --- channel 1  : ComRef    --- //
  // --- channel 2  : Pmt Down  --- //
  // --- channel 3  : Pmt Up    --- //
  // --- ---------------------- --- //
  char name[100];  
  fh_TimeFineBin = new TH1F*[fNumSignals];
  fh_TimeFineNs  = new TH1F*[fNumSignals];
  for(Int_t det=0; det<fNumDetectors; det++){
    for(Int_t ch=0; ch<fNumChannels; ch++){
      sprintf(name,"TimeFineBin_Sci%i_Ch%i_Sig%i",det+1, ch+1,det*fNumChannels+ch);
      fh_TimeFineBin[det*fNumChannels+ch] = new TH1F(name,name,fNumTcalParsPerSignal,0,fNumTcalParsPerSignal);
      sprintf(name,"TimeFineNs_Sci%i_Ch%i_Sig%i",det+1, ch+1,det*fNumChannels+ch);
      fh_TimeFineNs[det*fNumChannels+ch] = new TH1F(name,name,fNumTcalParsPerSignal,0,fNumTcalParsPerSignal);
    }
  }
  
  return kSUCCESS;
}

// -----   Public method ReInit   --------------------------------------------
InitStatus R3BSofSciMapped2TcalPar::ReInit() {
  
  
  return kSUCCESS;
}

// -----   Public method Exec   --------------------------------------------
void R3BSofSciMapped2TcalPar::Exec(Option_t* opt) {
  
  // --- ----------------------------------- --- //
  // --- LOOP OVER MAPPED HITS FOR SofComRef --- //
  // --- ----------------------------------- --- //
  // --- The common reference is the 
  // --- accepted trigger (also called 
  // --- master start, MT) 
  // --- sent at S2 and cave C
  // --- ----------------------------------- --- //

  // nHitsComRef = number of hits per event
  // for the common reference, this nHitsComRef is equal to 2 (one hit at S2 and one hit at cave C)
  UInt_t nHitsComRef = fMappedComRef->GetEntries(); 
  UInt_t iSignalComRef;
  for (UInt_t ihit=0; ihit<nHitsComRef; ihit++){
    R3BSofComRefMappedData* hitComRef = (R3BSofComRefMappedData*)fMappedComRef->At(ihit);
    if (!hitComRef){ 
      LOG(WARNING) << "R3BSofSciMapped2TcalPar::Exec() : could not get hitComRef" << FairLogger::endl;
      continue; 
    }           
    
    // *** ******************************************* *** //
    // ***         Numbers of det and channel          *** //
    // ***   in Mapped and TcalPAr Data are 1-based    *** //
    // *** ******************************************* *** //
    // *** Common Reference at S2 (Mapped Data):       *** //
    // ***     * det=1                                 *** //
    // *** Common Reference at S2 (Tcal Data):         *** //
    // ***     * det=1                                 *** //
    // ***     * channel=1                             *** //
    // ***     * signal=0                              *** //
    // *** ******************************************* *** //
    // *** Common Reference at Cave C (Mapped Data):   *** //
    // ***     * det=2                                 *** //
    // *** Common Reference at Cave C (Tcal Data):     *** //
    // ***     * det=2                                 *** //
    // ***     * channel=1                             *** //
    // ***     * signal=3                              *** //
    // *** ******************************************* *** //
    iSignalComRef   = (hitComRef->GetDetector()-1)*fNumChannels;
    if((iSignalComRef==0)||(iSignalComRef==3))
      fh_TimeFineBin[iSignalComRef]->Fill(hitComRef->GetTimeFine());
    else
      LOG(ERROR) << "R3BSofSciMapped2TcalPar::Exec() Signal number out of range for ComRef: "<< iSignalComRef << " instead of 0 or 3"<< FairLogger::endl;
  }// end of loop over the number of hits per event in MappedComRef


  // --- -------------------------------- --- //
  // --- LOOP OVER MAPPED HITS FOR SofSci --- //
  // --- -------------------------------- --- //
  
  // nHitsSci = number of hits per event
  // for the scintillator this number of hits can be very large especially for the detector at S2
  UInt_t nHitsSci = fMappedSci->GetEntries();    // can be very high especially for S2 detector
  UInt_t iSignalSci;
  for (UInt_t ihit=0; ihit<nHitsSci; ihit++){
    R3BSofSciMappedData* hitSci = (R3BSofSciMappedData*)fMappedSci->At(ihit);
    if (!hitSci){
      LOG(WARNING) << "R3BSofSciMapped2TcalPar::Exec() : could not get hitSci" << FairLogger::endl;
      continue; // should not happen
    }           

    // *** ******************************************* *** //
    // ***         Numbers of det and channel          *** //
    // ***   in Mapped and TcalPar Data are 1-based    *** //
    // *** ******************************************* *** //
    // *** SofSci at S2, Pmt Right (Mapped Data)       *** //
    // ***     * det=1                                 *** //
    // ***     * channel=1                             *** //
    // *** SofSci at S2, Pmt Right (Tcal Data)         *** //
    // ***     * det=1                                 *** //
    // ***     * channel=2                             *** //
    // ***     * signal=1                              *** //
    // *** ******************************************* *** //
    // *** SofSci at S2, Pmt Left (Mapped Data)        *** //
    // ***     * det=1                                 *** //
    // ***     * channel=2                             *** //
    // *** SofSci at S2, Pmt Left (Tcal Data)          *** //
    // ***     * det=1                                 *** //
    // ***     * channel=3                             *** //
    // ***     * signal=2                              *** //
    // *** ******************************************* *** //
    // *** SofSci at Cave C, Pmt Right (Mapped Data)   *** //
    // ***     * det=2                                 *** //
    // ***     * channel=1                             *** //
    // *** SofSci at Cave C, Pmt Right(Tcal Data)      *** //
    // ***     * det=2                                 *** //
    // ***     * channel=2                             *** //
    // ***     * signal=4                              *** //
    // *** ******************************************* *** //
    // *** SofSci at Cave C, Pmt Left (Mapped Data)    *** //
    // ***     * det=2                                 *** //
    // ***     * channel=2                             *** //
    // *** SofSci at Cave C, Pmt Left (Tcal Data)      *** //
    // ***     * det=2                                 *** //
    // ***     * channel=3                             *** //
    // ***     * signal=5                              *** //
    // *** ******************************************* *** //
    iSignalSci = (hitSci->GetDetector()-1)*fNumChannels + hitSci->GetPmt();
    if((0<iSignalSci)&&(iSignalSci<fNumSignals)&&(iSignalSci!=3))
      fh_TimeFineBin[iSignalSci]->Fill(hitSci->GetTimeFine());
    else
      LOG(ERROR) << "R3BSofSciMapped2TcalPar::Exec() Number of signals out of range: "<< iSignalSci << " instead of [0,"<< fNumSignals << "]" << FairLogger::endl;
      
  }// end of loop over the number of hits per event in MappedSci
}

// ---- Public method Reset   --------------------------------------------------
void R3BSofSciMapped2TcalPar::Reset() 
{
}

void R3BSofSciMapped2TcalPar::FinishEvent() 
{
}

// ---- Public method Finish   --------------------------------------------------
void R3BSofSciMapped2TcalPar::FinishTask() 
{  
  CalculateVftxTcalParams();
  fTcalPar->printParams();
}


//------------------
void R3BSofSciMapped2TcalPar::CalculateVftxTcalParams()
{
  LOG(INFO) << "R3BSofSciMapped2TcalPar: CalculateVftxTcalParams()" << FairLogger::endl;
  
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
    fh_TimeFineNs[sig]->Write(); // empty histo if stat <fMinStatistics
    fh_TimeFineBin[sig]->Write();
  }
  fTcalPar->setChanged();
  return;
  
}

ClassImp(R3BSofSciMapped2TcalPar)
  
