// ----------------------------------------------------------------------
// -----                                                            -----
// -----                     R3BSofTwimMapped2Cal                  -----
// -----             Created 07/10/19  by J.L. Rodriguez-Sanchez    -----
// ----------------------------------------------------------------------

#ifndef R3BSofTwimMapped2Cal_H
#define R3BSofTwimMapped2Cal_H

#include "FairTask.h"
#include "TH1F.h"
#include "R3BSofTwimMapped2CalPar.h"
#include "R3BSofTwimCalData.h"
#include "R3BSofTwimMappedData.h"
#include <TRandom.h>

class TClonesArray;
class R3BSofTwimCalPar;

class R3BSofTwimMapped2Cal : public FairTask {
  
 public:
  /** Default constructor **/
  R3BSofTwimMapped2Cal();

  /** Standard constructor **/
  R3BSofTwimMapped2Cal(const char* name, Int_t iVerbose=1);
  
  /** Destructor **/
  virtual ~R3BSofTwimMapped2Cal();
  
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
 
  UChar_t NumSec;
  UChar_t NumAnodes;
  UChar_t NumParams;
  TArrayF* CalParams;

  Bool_t fOnline;//Don't store data for online
  
  R3BSofTwimCalPar* fCal_Par;      /**< Parameter container. >*/ 
  TClonesArray* fTwimMappedDataCA; /**< Array with Mapped- input data. >*/
  TClonesArray* fTwimCalDataCA;    /**< Array with Cal- output data. >*/
  
  /** Private method AddCalData **/
  // Adds a SofTwimCalData to the TwimCalCollection
  R3BSofTwimCalData* AddCalData(UChar_t secID, UChar_t anodeID, 
                                                Double_t dtime,
	                                        Double_t energy);

 public:
  //Class definition
  ClassDef(R3BSofTwimMapped2Cal, 1)
    };

#endif
