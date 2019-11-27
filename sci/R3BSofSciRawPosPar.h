#ifndef __R3BSOFSCIRAWPOSPAR_H__
#define __R3BSOFSCIRAWPOSPAR_H__

#include "FairParGenericSet.h" // for FairParGenericSet

#include "TArrayF.h"
#include "TObjArray.h"
#include "TObject.h"
#include <TObjString.h>

class FairParamList;

class R3BSofSciRawPosPar : public FairParGenericSet
{
  
 public:
  /** Standard constructor **/
  R3BSofSciRawPosPar(const char* name = "SofSciRawPosPar",
			 const char* title = "Sof Sci Parameters",
			 const char* context = "SofSciParContext");
  
  /** Destructor **/
  virtual ~R3BSofSciRawPosPar();
  
  /** Method to reset all parameters **/
  virtual void clear();
  
  /** Method to store all parameters using FairRuntimeDB **/
  virtual void putParams(FairParamList* list);
  
  /** Method to retrieve all parameters using FairRuntimeDB**/
  Bool_t getParams(FairParamList* list);
  
  /** Method to print values of parameters to the standard output **/
  void printParams();
  
  /** Accessor functions **/
  const Double_t GetNumDetectors()     { return fNumDetectors; }
  const Double_t GetNumChannels()      { return fNumChannels; }
  const Double_t GetNumSignals()       { return fNumSignals; }
  const Double_t GetNumParsPerSignal() { return fNumParsPerSignal; }
  TArrayF* GetAllSignalsTcalParams()   { return fAllSignalsRawPosParams; }
  Double_t GetSignalTcalParams(UInt_t rank) { return (Double_t)fAllSignalsRawPosParams->GetAt(rank); }
  
  void SetNumDetectors(Int_t ndets)         { fNumDetectors = ndets; }
  void SetNumChannels(Int_t nchs)           { fNumChannels = nchs;   }
  void SetNumSignals(Int_t ndets)           { fNumSignals = ndets;   }
  void SetNumParsPerSignal(Int_t npars)     { fNumParsPerSignal = npars; }
  void SetSignalParams(Double_t parval, UInt_t rank)     { fAllSignalsRawPosParams->AddAt(parval, rank); }
  
  /** Create more Methods if you need them! **/
  
 private:
  TArrayF* fAllSignalsRawPosParams; // Calibration Parameters for all signals of one detector
  Int_t fNumChannels;               // number of detectors 
  Int_t fNumDetectors;              // number of channels of SofSciTcal data = 3 (need for Tcal2SingleTcal)
  Int_t fNumSignals;                // =fNumDetectors if RawPos 
  Int_t fNumParsPerSignal;
  const R3BSofSciRawPosPar& operator=(const R3BSofSciRawPosPar&); /*< an assignment operator>*/
  
  R3BSofSciRawPosPar(const R3BSofSciRawPosPar&); // a copy constructor
  
  ClassDef(R3BSofSciRawPosPar, 1);
};

#endif //__R3BSOFSCIRAWPOSPAR_H__
