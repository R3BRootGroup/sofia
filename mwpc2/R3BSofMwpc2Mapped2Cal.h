// ----------------------------------------------------------------------
// -----                                                            -----
// -----                     R3BSofMwpc2Mapped2Cal                  -----
// -----             Created 10/10/19  by J.L. Rodriguez-Sanchez    -----
// ----------------------------------------------------------------------

#ifndef R3BSofMwpc2Mapped2Cal_H
#define R3BSofMwpc2Mapped2Cal_H

#include "FairTask.h"
#include "TH1F.h"
#include "R3BSofMwpcCalData.h"
#include "R3BSofMwpcMappedData.h"
#include <TRandom.h>

class TClonesArray;
class R3BSofMwpc2CalPar;

class R3BSofMwpc2Mapped2Cal : public FairTask {
  
 public:
  /** Default constructor **/
  R3BSofMwpc2Mapped2Cal();

  /** Standard constructor **/
  R3BSofMwpc2Mapped2Cal(const char* name, Int_t iVerbose=1);
  
  /** Destructor **/
  virtual ~R3BSofMwpc2Mapped2Cal();
  
  /** Virtual method Exec **/
  virtual void Exec(Option_t* option);
  
  /** Virtual method Reset **/
  virtual void Reset();
  
  virtual void SetParContainers();
  
  //Fair specific
  /** Virtual method Init **/
  virtual InitStatus Init();
  
  /** Virtual method ReInit **/
  virtual InitStatus ReInit();
  
  /** Virtual method Finish **/
  virtual void Finish();

  void SetOnline(Bool_t option){ fOnline=option; }

 private:
  
  void SetParameter();
 
  Int_t NumPadX;
  Int_t NumPadY;
  Int_t NumParams;
  TArrayI* CalParams;

  Bool_t fOnline;//Don't store data for online
  
  R3BSofMwpc2CalPar* fCal_Par;     /**< Parameter container. >*/ 
  TClonesArray* fMwpcMappedDataCA; /**< Array with Mapped- input data. >*/
  TClonesArray* fMwpcCalDataCA;    /**< Array with Cal- output data. >*/
  
  /** Private method AddCalData **/
  // Adds a SofMwpcCalData to the MwpcCalCollection
  R3BSofMwpcCalData* AddCalData(UChar_t plane, UChar_t pad, UShort_t charge);

 public:
  //Class definition
  ClassDef(R3BSofMwpc2Mapped2Cal, 0)
};

#endif
