// --------------------------------------------------------
// -----                                              -----
// -----		 R3BSofMwpc3CalPar            -----
// -----       Created 11/10/2019 by G.García Jiménez -----
// --------------------------------------------------------

#ifndef R3BSOFMWPC3CALPAR_H
#define R3BSOFMWPC3CALPAR_H

#include "FairParGenericSet.h"
#include "TObject.h"
#include <TObjString.h>
#include "TObjArray.h"
#include "TArrayI.h"


class FairParamList;

class R3BSofMwpc3CalPar : public FairParGenericSet {

 public:
  /* ---- Default Constructor ---- */
  R3BSofMwpc3CalPar();

  /* ---- Standard Constructor ---- */
  R3BSofMwpc3CalPar (const char* name = "mwpc3CalPar",
                     const char* title = "Mwpc3 Cal Parameters",
                     const char* context = "Mwpc3CalParContext");

  /* ---- Destructor ----*/
  virtual ~R3BSofMwpc3CalPar();

  /* ---- Method to reset all parameters ---- */
  virtual void clear();

  /* ---- Method to store all parameters using FairRuntimeDB ---- */
  virtual void putParams(FairParamList* list);

  /* ---- Method to retrieve all parameters using FairRuntimeDB ---- */
  Bool_t getParams(FairParamList* list);

  /* ---- Method to print values of parameters to the standard output ---- */
  void printParams();

  /* ---- Accessor functions ---- */
  const Int_t GetNumPadsX(){return fNumPadsX;}
  const Int_t GetNumPadsY(){return fNumPadsY;}
  const Int_t GetNumParametersFit(){return fNumParamsFit;}

  TArrayI* GetPadCalParams(){return fPadCalParams;}

  void SetNumPadsX(Int_t numberPadsX){fNumPadsX=numberPadsX;}
  void SetNumPadsY(Int_t numberPadsY){fNumPadsY=numberPadsY;}
  void SetNumParametersFit(Int_t numberParams){fNumParamsFit=numberParams;}
  void SetPadCalParams(Int_t cc, Int_t ii ){fPadCalParams->AddAt(cc, ii);}

  /* ---- Create more Methods if you need them!  ---- */

 private:

  TArrayI* fPadCalParams;
  Int_t fNumPadsX;
  Int_t fNumPadsY;
  Int_t fNumParamsFit;

  /* ---- An assignment operator ---- */
  const R3BSofMwpc3CalPar& operator = (const R3BSofMwpc3CalPar&);

  /* ---- A copy constructor ---- */
  R3BSofMwpc3CalPar(const R3BSofMwpc3CalPar&);
  ClassDef(R3BSofMwpc3CalPar,1);
};

#endif
