#ifndef __R3BSOFTCALPAR_H__
#define __R3BSOFTCALPAR_H__

#include "FairParGenericSet.h" // for FairParGenericSet

#include "TObject.h"
#include <TObjString.h>
#include "TObjArray.h"
#include "TArrayF.h"

class FairParamList;

class R3BSofTcalPar : public FairParGenericSet {
  
 public:
  
  /** Standard constructor **/
  R3BSofTcalPar( const char* name    = "SofTcalPar",
		 const char* title   = "SofTcal Parameters",
		 const char* context = "SofTcalParContext");
  
  /** Destructor **/
  virtual ~R3BSofTcalPar();
  
  /** Method to reset all parameters **/
  virtual void clear();
  
  /** Method to store all parameters using FairRuntimeDB **/
  virtual void putParams(FairParamList* list); 
  
  /** Method to retrieve all parameters using FairRuntimeDB**/
  Bool_t getParams(FairParamList* list);
  
  /** Method to print values of parameters to the standard output **/
  void printParams();
  
  /** Accessor functions **/
  const Double_t GetNumDetectorss() {return fNumDetectors;}
  const Double_t GetNumSections() {return fNumSections;}
  const Double_t GetNumChannels() {return fNumChannels;}
  const Double_t GetNumSignals() {return fNumSignals;}
  TArrayF* GetAllSignalsTcalParams() {return fAllSignalsTcalParams;}

  void SetNumDetectors(Int_t NumberOfDetectors)   {fNumDetectors=NumberOfDetectors;}  
  void SetNumSections(Int_t NumberOfSections)     {fNumSections=NumberOfSections;}
  void SetNumChannels(Int_t NumberOfChannels)     {fNumChannels=NumberOfChannels;}
  void SetNumSignals(Int_t NumberOfDetectors, 
		     Int_t NumberOfSections, 
		     Int_t NumberOfChannels){
    fNumSignals=NumberOfDetectors*NumberOfSections*NumberOfChannels;
  }
  void SetNumTcalParsPerSignal(Int_t NumberOfTcalParsPerSignal) {fNumTcalParsPerSignal=NumberOfTcalParsPerSignal;}
  void SetSignalTcalParams(Double_t ft_ns, Int_t rank){fAllSignalsTcalParams->AddAt(ft_ns, rank);}
  
  /** Create more Methods if you need them! **/
  
 private:
  
  TArrayF* fAllSignalsTcalParams;    // Calibration Parameters for all signals of one detector
  Int_t fNumDetectors;  // number of detectors (=2 for Sci, =28 for ToFW, =1 for Trim, =2 for Twim)
  Int_t fNumSections;	// number of sections  (=1 for Sci, =1 for ToFW, =3 for Trim, =2 for Twim)
  Int_t fNumChannels;   // number of channels  (=3 for Sci, =2 for ToFW, =6 for Twim, =16 for Twim)
  Int_t fNumSignals;    // =fNumDetectors * fNumSections * fNumChannels
  Int_t fNumTcalParsPerSignal;
  const R3BSofTcalPar& operator=(const R3BSofTcalPar&);/*< an assignment operator>*/
  
  R3BSofTcalPar(const R3BSofTcalPar&);  // a copy constructor 
  
  ClassDef(R3BSofTcalPar,1);
};

#endif //__R3BSOFTCALPAR_H__
