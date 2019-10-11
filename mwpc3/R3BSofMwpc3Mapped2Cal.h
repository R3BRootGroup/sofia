// ----------------------------------------------------------------------
// -----                     R3BSofMwpc3Mapped2Cal                  -----
// -----             Created 11/10/19  by G. García Jiménez         -----
// -----             by modifying J.L 's class for Mwpc0            -----
// ----------------------------------------------------------------------

#ifndef R3BSOFMWPC3MAPPED2CAL_H
#define R3BSOFMWPC3MAPPED2CAL_H

#include "FairTask.h"
#include "TH1F.h"
#include "R3BSofMwpcCalData.h"
#include "R3BSofMwpcMappedData.h"
#include <TRandom.h>

class TClonesArray;
class R3BSofMwpc3CalPar;

class R3BSofMwpc3Mapped2Cal : public FairTask {

 public:
  /* ---- Default constructor ---- */
  R3BSofMwpc3Mapped2Cal();

  /* ---- Standard constructor ---- */
  R3BSofMwpc3Mapped2Cal(const char* name, Int_t iVerbose=1);

  /* ---- Destructor ---- */
  virtual ~R3BSofMwpc3Mapped2Cal();

  /* ---- Virtual method Exec ---- */
  virtual void Exec(Option_t* option);

  /* ---- Virtual method Reset ---- */
  virtual void Reset();

  virtual void SetParContainers();

  //Fair specific
  /* ---- Virtual method Init ---- */
  virtual InitStatus Init();

  /* ---- Virtual method ReInit ---- */
  virtual InitStatus ReInit();

  /* ---- Virtual method Finish ---- */
  virtual void Finish();

  void SetOnline(Bool_t option){ fOnline=option; }

 private:

  void SetParameter();

  Int_t NumPadX;
  Int_t NumPadY;
  Int_t NumParams;
  TArrayI* CalParams;

  Bool_t fOnline; //Don't store data for online

  R3BSofMwpc3CalPar* fCal_Par;     /* ---- Parameter container ---- */
  TClonesArray* fMwpcMappedDataCA; /* ---- Array with Mapped- input data ---- */
  TClonesArray* fMwpcCalDataCA;    /* ---- Array with Cal- output data ---- */

  /* ---- Private method AddCalData ---- */
  // Adds a SofMwpc3CalData to the MwpcCalCollection
  R3BSofMwpcCalData* AddCalData(UChar_t plane, UChar_t pad, UShort_t charge);

 public:
  /* ---- Class definition ---- */
  ClassDef(R3BSofMwpc3Mapped2Cal, 1)
};

#endif
