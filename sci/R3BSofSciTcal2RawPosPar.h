#ifndef __R3BSOFSCITCAL2RAWPOSPAR_H__
#define __R3BSOFSCITCAL2RAWPOSPAR_H__

#include "FairTask.h"
#include "TH1F.h"
#include "TH1D.h"

class TClonesArray;
class R3BSofSciRawPosPar;
class R3BEventHeader;


// for the engineering run (fNumDetectors(1) instead of fNumDetectors(2)
#define NUMBER_OF_DETECTORS 1 
#define NUMBER_OF_CHANNELS 3 // 2PMT + Tref
#define NUMBER_OF_SIGNALS NUMBER_OF_DETECTORS


class R3BSofSciTcal2RawPosPar : public FairTask {
  
 public:
  
  /** Default constructor **/
  R3BSofSciTcal2RawPosPar();
  
  /** Standard constructor **/
  R3BSofSciTcal2RawPosPar(const char* name, Int_t iVerbose=1);
  
  /** Destructor **/
  virtual ~R3BSofSciTcal2RawPosPar();
  
  /** Virtual method Init **/
  virtual InitStatus Init();
  
  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);
  
  /** Virtual method FinishEvent **/
  virtual void FinishEvent();
  
  /** Virtual method FinishTask **/
  virtual void FinishTask(); 
  
  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Virtual method ReInit **/
  virtual InitStatus ReInit();
  
  /** Virtual method calculate the PosRaw Single Tcal Parameters **/
  virtual void CalculateRawPosRawPosParams();
  
 
  void SetOutputFile(const char *outFile);
  
  /** Accessor functions **/
  const Double_t GetNumDetectors() {return fNumDetectors;}
  const Double_t GetNumChannels()  {return fNumChannels;}
  const Double_t GetNumSignals()   {return fNumSignals;}
  const Int_t    GetMinStatistics(){return fMinStatistics;}

  void SetNumDetectors(Int_t n)   {fNumDetectors=n;}  
  void SetNumChannels(Int_t n)    {fNumChannels=n;}  
  void SetNumSignals(){
    if(fNumDetectors)    fNumSignals=fNumDetectors;
    //else LOG(ERROR) << "R3BSofSciTcal2RawPosPar; cannot defined fNumSignals without fNumDetectors";
  }
  void SetNumParsPerSignal(Int_t n) {
    fNumParsPerSignal=n;
  }
  void SetMinStatistics(Int_t minstat){
    fMinStatistics=minstat;
  }



 protected:
  Int_t fNumDetectors;  // number of detectors 2 if FRS, 1 if Cave C only
  Int_t fNumChannels;   // number of channels at the Tcal level
  Int_t fNumSignals;    // number of signal = fNumDetectors if RawPos used
  Int_t fNumParsPerSignal; // =2 for each signal
  Int_t fMinStatistics; // minimum statistics to proceed to the calibration

  // calibration parameters
  R3BSofSciRawPosPar* fRawPosPar;  

  // input data
  TClonesArray* fTcal;         
   
  // histograms
  TH1D** fh_RawPosMult1;
#if NUMBER_OF_DETECTORS==2
  TH1D* fh_RawTofMult1;
#endif
  char* fOutputFile;
  
 public:
  ClassDef(R3BSofSciTcal2RawPosPar,0);
};

#endif //__R3BSOFSCITCAL2RAWPOSPAR_H__
