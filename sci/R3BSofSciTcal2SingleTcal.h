// *** *************************************************************** *** //
// ***                  R3BSofSciTcal2SingleTcal                        *** //
// *** ---> whatever multiplicity per PMT, select the propoer hit      *** //
// *** *************************************************************** *** //

#ifndef R3BSOFSCI_TCAL2SINGLETCAL
#define R3BSOFSCI_TCAL2SINGLETCAL

#include "FairTask.h"

#include "R3BSofSciRawPosPar.h"
#include "R3BSofSciSingleTcalData.h"

#include "TClonesArray.h"
#include "TMath.h"
#include "TRandom.h"
class TRandom3;

class R3BSofSciTcal2SingleTcal : public FairTask
{

 public:
  // --- Default constructor --- //
  R3BSofSciTcal2SingleTcal();
  
  // --- Standard constructor --- //
  R3BSofSciTcal2SingleTcal(const char* name, Int_t iVerbose=1);

  // --- Destructor --- // 
  virtual ~R3BSofSciTcal2SingleTcal();

  virtual InitStatus Init();         
  virtual void SetParContainers();
  virtual InitStatus ReInit();
  virtual void Exec(Option_t* option);
  virtual void FinishEvent();
  virtual void FinishTask();

 private:
  TClonesArray*  fTcal;            
  R3BSofSciRawPosPar* fRawPosPar;          
  TClonesArray*  fSingleTcal;         
  
  UInt_t fNumSingleTcal;     

  UInt_t fNevent;

  TRandom rand;

 public:
  ClassDef(R3BSofSciTcal2SingleTcal, 1)

};

#endif  // R3BSOFSCI_TCAL2SINGLETCAL
