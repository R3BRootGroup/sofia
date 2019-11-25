// *** *************************************************************** *** //
// ***                  R3BSofSciMapped2Tcal                           *** //
// *** convert Mapped data to tcal data :
// *** ---> from the fine and coarse times, calculate a raw time in ns *** //
// *** *************************************************************** *** //

#ifndef R3BSOFSCI_MAPPED2TCAL
#define R3BSOFSCI_MAPPED2TCAL

#include "FairTask.h"

#include "R3BSofTcalPar.h"
#include "R3BSofSciTcalData.h"

#include "TClonesArray.h"
#include "TMath.h"
#include "TRandom.h"
class TRandom3;

class R3BSofSciMapped2Tcal : public FairTask
{

 public:
  // --- Default constructor --- //
  R3BSofSciMapped2Tcal();
  
  // --- Standard constructor --- //
  R3BSofSciMapped2Tcal(const char* name, Int_t iVerbose=1);

  // --- Destructor --- // 
  virtual ~R3BSofSciMapped2Tcal();

  virtual InitStatus Init();         
  virtual void SetParContainers();
  virtual InitStatus ReInit();
  virtual void Exec(Option_t* option);
  virtual void FinishEvent();
  virtual void FinishTask();
  Double_t CalculateTimeNs(UShort_t det, UShort_t pmt, UInt_t tf, UInt_t tc);

 private:
  TClonesArray*  fMapped;             // input data - SofSci
  R3BSofTcalPar* fTcalPar;            // tcal parameters container - SofSci
  TClonesArray*  fTcalItems;          // output data
  
  UInt_t fNumTcalItems;               // number of Tcal items per event

  UInt_t fNevent;

  TRandom rand;

 public:
  ClassDef(R3BSofSciMapped2Tcal, 1)

};

#endif  // R3BSOFSCI_MAPPED2TCAL
