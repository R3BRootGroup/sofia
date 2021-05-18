// ----------------------------------------------------------------------
// -----                                                            -----
// -----		 R3BSofMwpc2CalPar 			      -----
// -----          Created 10/10/19  by J.L. Rodriguez-Sanchez       -----
// ----------------------------------------------------------------------

#include "R3BSofMwpc2CalPar.h"
#include "FairLogger.h"
#include "FairParamList.h"

#include "TMath.h"
#include "TString.h"

#include <iostream>

// ---- Standard Constructor ---------------------------------------------------
R3BSofMwpc2CalPar::R3BSofMwpc2CalPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
    , fNumPadsX(128)
    , fNumPadsY(40)
    , fNumParamsFit(1)
{
    fPadCalParams = new TArrayI((fNumPadsX + fNumPadsY) * fNumParamsFit);
}

// ----  Destructor ------------------------------------------------------------
R3BSofMwpc2CalPar::~R3BSofMwpc2CalPar()
{
    clear();
    delete fPadCalParams;
}

// ----  Method clear ----------------------------------------------------------
void R3BSofMwpc2CalPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BSofMwpc2CalPar::putParams(FairParamList* list)
{

    LOG(INFO) << "R3BSofMwpc2CalPar::putParams() called";
    if (!list)
    {
        return;
    }

    Int_t array_size = (fNumPadsX + fNumPadsY) * fNumParamsFit;
    LOG(INFO) << "Array Size: " << array_size;

    fPadCalParams->Set(array_size);

    list->add("mwpc2CalPar", *fPadCalParams);
    list->add("mwpc2PadXNumberPar", fNumPadsX);
    list->add("mwpc2PadYNumberPar", fNumPadsY);
    list->add("mwpc2ParamsFitPar", fNumParamsFit);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofMwpc2CalPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofMwpc2CalPar::getParams() called";
    if (!list)
    {
        return kFALSE;
    }

    if (!list->fill("mwpc2PadXNumberPar", &fNumPadsX))
    {
        return kFALSE;
    }
    if (!list->fill("mwpc2PadYNumberPar", &fNumPadsY))
    {
        return kFALSE;
    }
    if (!list->fill("mwpc2ParamsFitPar", &fNumParamsFit))
    {
        return kFALSE;
    }

    Int_t array_size = (fNumPadsX + fNumPadsY) * fNumParamsFit;
    LOG(INFO) << "Array Size: " << array_size;
    fPadCalParams->Set(array_size);

    if (!(list->fill("mwpc2CalPar", fPadCalParams)))
    {
        LOG(INFO) << "---Could not initialize mwpc2CalPar";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofMwpc2CalPar::printParams()
{
    LOG(INFO) << "R3BSofMwpc2CalPar: SOFIA MWPC2 Calibration Parameters: ";
    Int_t array_size = (fNumPadsX + fNumPadsY) * fNumParamsFit;

    for (Int_t i = 0; i < (fNumPadsX + fNumPadsY); i++)
    {
        if (i < fNumPadsX / 2)
        {
            LOG(INFO) << "MWPC2 Plane X down, Pad Number: " << i + 1;
        }
        else if (i < fNumPadsX)
        {
            LOG(INFO) << "MWPC2 Plane X up, Pad Number: " << i + 1 - fNumPadsX / 2;
        }
        else
        {
            LOG(INFO) << "MWPC2 Plane Y, Pad Number: " << i + 1 - fNumPadsX;
        }
        for (Int_t j = 0; j < fNumParamsFit; j++)
        {
            LOG(INFO) << "FitParam(" << j << ") = " << fPadCalParams->GetAt(i * fNumParamsFit + j);
        }
    }
}

ClassImp(R3BSofMwpc2CalPar)
