// *** *************************************************************** *** //
// ***                  R3BSofSciTcal2SingleTcal                        *** //
// *** ---> whatever multiplicity per PMT, select the propoer hit      *** //
// *** *************************************************************** *** //

#ifndef R3BSOFSCI_TCAL2SINGLETCAL
#define R3BSOFSCI_TCAL2SINGLETCAL

#include "FairTask.h"

#include "R3BSofSciRawPosPar.h"
#include "R3BSofSciSingleTcalData.h"
#include "detectors_cfg.h"

#ifdef NUMBER_OF_SOFSCI_TOF
#include "R3BSofSciRawTofPar.h"
#endif

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

  void SetOnline(Bool_t option) { fOnline = option; }

 private:
  Int_t GetTofRank(Int_t idstart, Int_t idstop)
  {
    Int_t fFirstTofNumber = 0; 
    for(UShort_t istart=0; istart<NUMBER_OF_SOFSCI_DETECTORS-1; istart++)
    {
      for(UShort_t istop=istart+1; istop<NUMBER_OF_SOFSCI_DETECTORS; istop++)
      {
        fFirstTofNumber++;
        if ( ((istart+1)==idstart) && ((istop+1)==idstop) ) break;
      }
     } 
    return fFirstTofNumber;
  }

  TClonesArray*  fTcal;            
  TClonesArray*  fSingleTcal;         
  R3BSofSciRawPosPar* fRawPosPar;
#ifdef NUMBER_OF_SOFSCI_TOF
  R3BSofSciRawTofPar* fRawTofPar;
#endif
  
  Bool_t fOnline; // Don't store data for online

  UInt_t fNumSingleTcal;     

  UInt_t fNevent;

  TRandom rand;

 public:
  ClassDef(R3BSofSciTcal2SingleTcal, 1)

};

#endif  // R3BSOFSCI_TCAL2SINGLETCAL
