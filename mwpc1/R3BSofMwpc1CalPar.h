// ----------------------------------------------------------------------
// -----		                R3BSofMwpc1CalPar 			                -----
// -----          Created 15/10/19  by G. García Jiménez            -----
// -----  by modifying J.L. Rodriguez-Sanchez  classes for Mwpc2    -----
// ----------------------------------------------------------------------

#ifndef R3BSofMwpc1CalPar_H
#define R3BSofMwpc1CalPar_H

#include "FairParGenericSet.h"
#include "TObject.h"
#include <TObjString.h>
#include "TObjArray.h"
#include "TArrayI.h"


class FairParamList;

class R3BSofMwpc1CalPar : public FairParGenericSet {

 public:

  /** Default Constructor **/
  R3BSofMwpc1CalPar();

  /** Standard constructor **/
  R3BSofMwpc1CalPar(const char* name = "mwpc1CalPar",
                       const char* title = "Mwpc1 Cal Parameters",
                       const char* context ="Mwpc1CalParContext");

  /** Destructor **/
  virtual ~R3BSofMwpc1CalPar();

  /** Method to reset all parameters **/
  virtual void clear();

  /** Method to store all parameters using FairRuntimeDB **/
  virtual void putParams(FairParamList* list);

  /** Method to retrieve all parameters using FairRuntimeDB**/
  Bool_t getParams(FairParamList* list);

  /** Method to print values of parameters to the standard output **/
  void printParams();

  /** Accessor functions **/
  const Int_t GetNumPadsX(){return fNumPadsX;}
  const Int_t GetNumPadsY(){return fNumPadsY;}
  const Int_t GetNumParametersFit(){return fNumParamsFit;}

  TArrayI* GetPadCalParams(){return fPadCalParams;}

  void SetNumPadsX(Int_t numberPadsX){fNumPadsX=numberPadsX;}
  void SetNumPadsY(Int_t numberPadsY){fNumPadsY=numberPadsY;}
  void SetNumParametersFit(Int_t numberParams){fNumParamsFit=numberParams;}
  void SetPadCalParams(Int_t cc, Int_t ii ){fPadCalParams->AddAt(cc, ii);}

  /** Create more Methods if you need them! **/

 private:

  TArrayI* fPadCalParams;
  Int_t fNumPadsX;
  Int_t fNumPadsY;
  Int_t fNumParamsFit;

  const R3BSofMwpc1CalPar& operator = (const R3BSofMwpc1CalPar&); // An assignment operator

  R3BSofMwpc1CalPar( const R3BSofMwpc1CalPar&);                // A copy constructor

  ClassDef(R3BSofMwpc1CalPar,0);


  };
#endif