// *** *************************************************************** *** //
// ***                  R3BSofTofWTcal2SingleTcal                        *** //
// *** *************************************************************** *** //

#ifndef R3BSOFTOFW_TCAL2SINGLETCAL
#define R3BSOFTOFW_TCAL2SINGLETCAL

#include "FairTask.h"

#include "R3BSofToFWSingleTcalData.h"

#include "TClonesArray.h"
#include "TMath.h"
#include "TRandom.h"
class TRandom3;

class R3BSofTofWTcal2SingleTcal : public FairTask
{

 public:
  // --- Default constructor --- //
  R3BSofTofWTcal2SingleTcal();
  
  // --- Standard constructor --- //
  R3BSofTofWTcal2SingleTcal(const char* name, Int_t iVerbose=1);

  // --- Destructor --- // 
  virtual ~R3BSofTofWTcal2SingleTcal();

  virtual InitStatus Init();         
  virtual void SetParContainers();
  virtual InitStatus ReInit();
  virtual void Exec(Option_t* option);
  virtual void FinishEvent();
  virtual void FinishTask();

 private:
  TClonesArray*  fSciSingleTcal;  // input data            
  TClonesArray*  fToFWTcal;       // input data
  TClonesArray*  fToFWSingleTcal; // output data        
  UInt_t fNumSingleTcal;
  UInt_t fNevent;

  TRandom rand;

 public:
  ClassDef(R3BSofTofWTcal2SingleTcal, 1)

};

#endif  // R3BSOFTOFW_TCAL2SINGLETCAL
