// ----------------------------------------------------------------------
// -----                                                            -----
// -----		 R3BSofMwpc2CalPar 			    ----- 
// -----          Created 10/10/19  by J.L. Rodriguez-Sanchez       -----
// ----------------------------------------------------------------------

#ifndef R3BSofMwpc2CalPar_H
#define R3BSofMwpc2CalPar_H

#include "FairParGenericSet.h"
#include "TObject.h"
#include <TObjString.h>
#include "TObjArray.h"
#include "TArrayI.h"


class FairParamList;

class R3BSofMwpc2CalPar : public FairParGenericSet {

 public:

  /** Default Constructor **/
  R3BSofMwpc2CalPar();

  /** Standard constructor **/
  R3BSofMwpc2CalPar(const char* name = "mwpc2CalPar", 
                     const char* title = "Mwpc2 Cal Parameters",
                     const char* context ="Mwpc2CalParContext");

  /** Destructor **/
  virtual ~R3BSofMwpc2CalPar();

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

  const R3BSofMwpc2CalPar& operator = (const R3BSofMwpc2CalPar&); // An assignment operator

  R3BSofMwpc2CalPar( const R3BSofMwpc2CalPar&);                // A copy constructor

  ClassDef(R3BSofMwpc2CalPar,0);
 };

#endif
