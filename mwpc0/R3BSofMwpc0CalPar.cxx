/////////////////////////////////////////////////////////////
//							R3BSofMwpc0CalPar Source file						 	//
//					Created 7/10/2019 by G.García Jiménez					//
////////////////////////////////////////////////////////////



# include "R3BSofMwpc0CalPar.h"
#include "FairLogger.h"
#include "FairParamList.h"

#include "TString.h"
#include "TMath.h"

#include <iostream>

using std::cout;
using std::endl;

R3BSofMwpc0CalPar::R3BSofMwpc0CalPar(const char* name,
					       const char* title,
					       const char* context)
  : FairParGenericSet(name,title,context) {
  fPadCalParams= new TArrayF(132); //64 Pads x 2 Planes x 1 Calibration Parameter (Pedestal)
  fNumPadsX=64;
	fNumPadsY=64;
  fNumParamsFit=1;  // Pedestal Substraction
}


R3BSofMwpc0CalPar::~R3BSofMwpc0CalPar(){
 clear();
 delete fPadCalParams;

}


void R3BSofMwpc0CalPar::clear() {

 status=KFALSE;
 resetInputVersions();

  }

//====== Put Parameters ======

void R3BSofMwpc0CalPar::putParams(FairParamList* list) {

  LOG(INFO) <<"R3BSofMwpc0CalPar::putParams() called";
   if (!list){ return; }

   Int_t array_size = (fNumPadsX + fNumPadsY)*fNumParamsFit;
   LOG(INFO) <<"Array Size: "<<array_size;

   fPadCalParams->Set(array_size);

   list->add("mwpc0CalPar", *fPadCalParams);
   list->add("mwpc0PadXNumberPar", fNumPadsX);
	 list->add("mwpc0PadYNumberPar", fNumPadsY);
   list->add("mwpc0ParamsFitPar", fNumParamsFit);

}

Bool_t R3BSofMwpc0CalPar::getParams(FairParamList* list) {
  LOG(INFO) <<"R3BSofMwpc0CalPar::getParams() called";
  if (!list){ return kFALSE;}

  if (!list->fill("mwpc0PadXNumberPar", &fNumPadsX) ) {
    return kFALSE;
  }
	if (!list->fill("mwpc0PadYNumberPar", &fNumPadsY) ) {
    return kFALSE;
  }
  if (!list->fill("mwpc0ParamsFitPar", &fNumParamsFit) ) {
    return kFALSE;
  }

  Int_t array_size = (fNumPadsX + fNumPadsY)*fNumParamsFit;
  LOG(INFO) <<"Array Size: "<<array_size;
  fPadCalParams->Set(array_size);

  if (!(list->fill("mwpc0CalPar",fPadCalParams))){
    LOG(INFO)<< "---Could not initialize mwpc0CalPar";
    return kFALSE;
  }

  return kTRUE;
}


void R3BSofMwpc0CalPar::printParams() {
  LOG(INFO) << "R3BSofMwpc0CalPar: SOFIA MWPC0 Calibration Parameters: ";
  Int_t array_size = (fNumPadsX + fNumPadsY)*fNumParamsFit;

  for(Int_t i=0;i<(fNumPadsX +fNumPadsY);i++) {
    if (i<63) { cout << "MWPC0 Plane X Pad Number: " << i+1 << endl;}
		else {cout << "MWPC0 Plane Y Pad Number: " << i+1 << endl;}
    for(Int_t j=0;j<fNumParamsFit;j++) {
      cout << "FitParam("<<j<<") = "<<fPadCalParams->GetAt(i*fNumParamsFit+j) << endl;
    }
  }
}
