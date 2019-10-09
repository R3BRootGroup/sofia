// ----------------------------------------------------------------------
// -----                                                            -----
// -----                     R3BSofMwpc0Cal2Hit                     -----
// -----             Created 09/10/19  by J.L. Rodriguez-Sanchez    -----
// ----------------------------------------------------------------------

#ifndef R3BSofMwpc0Cal2Hit_H
#define R3BSofMwpc0Cal2Hit_H

#include "FairTask.h"
#include "TH1F.h"
#include "R3BSofMwpc0CalData.h"
#include "R3BSofMwpc0HitData.h"
#include <TRandom.h>

class TClonesArray;

class R3BSofMwpc0Cal2Hit : public FairTask {
  
 public:
  /** Default constructor **/
  R3BSofMwpc0Cal2Hit();

  /** Standard constructor **/
  R3BSofMwpc0Cal2Hit(const char* name, Int_t iVerbose=1);
  
  /** Destructor **/
  virtual ~R3BSofMwpc0Cal2Hit();
  
  /** Virtual method Exec **/
  virtual void Exec(Option_t* option);
  
  /** Virtual method Reset **/
  virtual void Reset();
  
  //Fair specific
  /** Virtual method Init **/
  virtual InitStatus Init();
  
  /** Virtual method ReInit **/
  virtual InitStatus ReInit();
  
  /** Virtual method Finish **/
  virtual void Finish();

  void SetOnline(Bool_t option){ fOnline=option; }

 private:

  Bool_t fOnline;//Don't store data for online

  TClonesArray* fMwpcCalDataCA;  /**< Array with Cal input data. >*/
  TClonesArray* fMwpcHitDataCA;  /**< Array with Hit output data. >*/
  
  /** Private method AddHitData **/
  // Adds a SofMwpc0HitData to the MwpcHitCollection
  R3BSofMwpc0HitData* AddHitData(Double_t x, Double_t y);

 public:
  //Class definition
  ClassDef(R3BSofMwpc0Cal2Hit, 1)
};

#endif
