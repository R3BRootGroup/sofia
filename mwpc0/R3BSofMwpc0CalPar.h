/////////////////////////////////////////////////////////////
//							     R3BSofMwpc0CalPar 						 	      //
//					Created 7/10/2019 by G.García Jiménez					//
////////////////////////////////////////////////////////////



#ifndef R3BSOFMWPC0CALPAR_H
#define R3BSOFMWPC0CALPAR_H

#include "FairParGenericSet.h"
#include "TObject.h"
#include <TObjString.h>
#include "TObjArray.h"
#include "TArrayF.h"


class FairParamList;

class R3BSofMwpc0CalPar : public FairParGenericSet {

 public:

  // Default Constructor
  R3BSofMwpc0CalPar();

  R3BSofMwpc0CalPar(const char* name = "mwpc0CalPar", const char* title = "Sofia Mwpc0 Cal Parameters",
                     const char* context ="SofMwpc0CalParContext");

  virtual ~R3BSofMwpc0CalPar();

  virtual void clear();

  virtual void putParams(FairParamList* list);

  Bool_t getParams(FairParamList* list);

  void printParams();

  // Accessors

  const Double_t GetNumPadsX(){return fNumPadsX;}
  const Double_t GetNumPadsY(){return fNumPadsY;}

  const Double_t GetNumParametersFit(){return fNumParamsFit;}
  TArrayF* GetPadCalParams(){return fPadCalParams;}

  void SetNumPadsX(Int_t numberPadsX){fNumPadsX=numberPadsX;}
  void SetNumPadsY(Int_t numberPadsY){fNumPadsY=numberPadsY;}
  void SetNumParametersFit(Int_t numberParams){fNumParamsFit=numberParams;}
  void SetPadCalParams(Float_t cc, Int_t ii ){fPadCalParams->AddAt(cc, ii);}

private:

  TArrayF* fPadCalParams;
  UChar_t fNumPadsX;
  UChar_t fNumPadsY;
  UChar_t fNumParamsFit;


  const R3BSofMwpc0CalPar& operator = (const mwpc0CalPar&); // An assignment operator

  R3BSofMwpc0CalPar( const mwpc0CalPar&);                // A copy constructor

  ClassDef(R3BSofMwpc0CalPar,1);
 };

}

#endif
