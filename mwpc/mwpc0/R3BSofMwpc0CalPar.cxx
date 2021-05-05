// ----------------------------------------------------------------------
// -----                                                            -----
// -----		 R3BSofMwpc0CalPar 			    -----
// ----- 	Created 7/10/2019 by G.García Jiménez		    -----
// ----------------------------------------------------------------------

#include "R3BSofMwpc0CalPar.h"
#include "FairLogger.h"
#include "FairParamList.h"

#include "TMath.h"
#include "TString.h"

#include <iostream>

// ---- Standard Constructor ---------------------------------------------------
R3BSofMwpc0CalPar::R3BSofMwpc0CalPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
    , fNumPadsX(64)
    , fNumPadsY(40)
    , fNumParamsFit(1)
{
    fPadCalParams = new TArrayI(104); //(64 Pads in X + 40 Pads in Y) x 1 Calibration Parameter (Pedestal)
}

// ----  Destructor ------------------------------------------------------------
R3BSofMwpc0CalPar::~R3BSofMwpc0CalPar()
{
    clear();
    if (fPadCalParams)
        delete fPadCalParams;
}

// ----  Method clear ----------------------------------------------------------
void R3BSofMwpc0CalPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BSofMwpc0CalPar::putParams(FairParamList* list)
{

    LOG(INFO) << "R3BSofMwpc0CalPar::putParams() called";
    if (!list)
    {
        return;
    }

    Int_t array_size = (fNumPadsX + fNumPadsY) * fNumParamsFit;
    LOG(INFO) << "Array Size: " << array_size;

    fPadCalParams->Set(array_size);

    list->add("mwpc0CalPar", *fPadCalParams);
    list->add("mwpc0PadXNumberPar", fNumPadsX);
    list->add("mwpc0PadYNumberPar", fNumPadsY);
    list->add("mwpc0ParamsFitPar", fNumParamsFit);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofMwpc0CalPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofMwpc0CalPar::getParams() called";
    if (!list)
    {
        return kFALSE;
    }

    if (!list->fill("mwpc0PadXNumberPar", &fNumPadsX))
    {
        return kFALSE;
    }
    if (!list->fill("mwpc0PadYNumberPar", &fNumPadsY))
    {
        return kFALSE;
    }
    if (!list->fill("mwpc0ParamsFitPar", &fNumParamsFit))
    {
        return kFALSE;
    }

    Int_t array_size = (fNumPadsX + fNumPadsY) * fNumParamsFit;
    LOG(INFO) << "Array Size: " << array_size;
    fPadCalParams->Set(array_size);

    if (!(list->fill("mwpc0CalPar", fPadCalParams)))
    {
        LOG(INFO) << "---Could not initialize mwpc0CalPar";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofMwpc0CalPar::printParams()
{
    LOG(INFO) << "R3BSofMwpc0CalPar: SOFIA MWPC0 Calibration Parameters: ";
    Int_t array_size = (fNumPadsX + fNumPadsY) * fNumParamsFit;

    for (Int_t i = 0; i < (fNumPadsX + fNumPadsY); i++)
    {
        if (i < fNumPadsX)
        {
            LOG(INFO) << "MWPC0 Plane X Pad Number: " << i + 1;
        }
        else
        {
            LOG(INFO) << "MWPC0 Plane Y Pad Number: " << i + 1 - fNumPadsX;
        }
        for (Int_t j = 0; j < fNumParamsFit; j++)
        {
            LOG(INFO) << "FitParam(" << j << ") = " << fPadCalParams->GetAt(i * fNumParamsFit + j);
        }
    }
}
