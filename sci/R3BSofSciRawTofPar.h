#ifndef __R3BSOFSCIRAWTOFPAR_H__
#define __R3BSOFSCIRAWTOFPAR_H__

#include "FairParGenericSet.h" // for FairParGenericSet
#include "TArrayF.h"
#include "TObjArray.h"
#include "TObject.h"
#include <TObjString.h>
#include "detectors_cfg.h"
class FairParamList;

class R3BSofSciRawTofPar : public FairParGenericSet
{
  
 public:
  /** Standard constructor **/
  R3BSofSciRawTofPar(const char* name = "SofSciRawTofPar",
		     const char* title = "Sof Sci Parameters",
		     const char* context = "SofSciParContext");
  
  /** Destructor **/
  virtual ~R3BSofSciRawTofPar();
  
  /** Method to reset all parameters **/
  virtual void clear();
  
  /** Method to store all parameters using FairRuntimeDB **/
  virtual void putParams(FairParamList* list);
  
  /** Method to retrieve all parameters using FairRuntimeDB**/
  Bool_t getParams(FairParamList* list);
  
  /** Method to print values of parameters to the standard output **/
  void printParams();
  
  /** Accessor functions **/
  const Int_t GetFirstStart()
  {
    return fFirstStaSci;
  }
  const Int_t GetFirstStop()
  {
    return fFirstStoSci;
  }
  const Int_t GetNumSignals() 
    { 
	return fNumSignals; 
    }
  const Double_t GetNumParsPerSignal() 
    { 
      return fNumParsPerSignal; 
    }
  TArrayF* GetAllSignalsRawTofParams() 
    { 
      return fAllSignalsRawTofParams;
    } 
  Double_t GetSignalRawTofParams(UInt_t rank) 
    { 
      return (Double_t)fAllSignalsRawTofParams->GetAt(rank); 
    }
  void SetFirstStart(Int_t detFirstStart)
  {
    fFirstStaSci = detFirstStart;
  }
  void SetFirstStop(Int_t detFirstStop)
  {
    fFirstStoSci = detFirstStop;
  }
  void SetNumSignals(Int_t nsig)
    { 
      fNumSignals = nsig;   
    }
  void SetNumParsPerSignal(Int_t npars)
    { 
      fNumParsPerSignal = npars; 
    }
  void SetSignalParams(Double_t parval, UInt_t rank)     
    { 
      fAllSignalsRawTofParams->AddAt(parval, rank); 
    }
  
  
 private:
  TArrayF* fAllSignalsRawTofParams; // Calibration Parameters for all signals of one detector
  Int_t fFirstStaSci;  // if nDets>2, start the mult selection with detectors number fFirstStaSci (1-based)
  Int_t fFirstStoSci;  //                                                        and fFirstStoSci (1-based)
  Int_t fNumSignals;   // = number of Tof
  Int_t fNumParsPerSignal; // = 2
  const R3BSofSciRawTofPar& operator=(const R3BSofSciRawTofPar&); 
  R3BSofSciRawTofPar(const R3BSofSciRawTofPar&); // a copy constructor
  
  ClassDef(R3BSofSciRawTofPar, 1);
};

#endif //__R3BSOFSCIRAWTOFPAR_H__
