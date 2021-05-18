// ----------------------------------------------------------------------
// -----		      R3BSofMwpc1CalPar        	      -----
// -----          Created 15/10/19  by G. García Jiménez            -----
// -----  by modifying J.L. Rodriguez-Sanchez  classes for Mwpc2    -----
// ----------------------------------------------------------------------

#include "R3BSofMwpc1CalPar.h"
#include "FairLogger.h"
#include "FairParamList.h"

#include "TMath.h"
#include "TString.h"

#include <iostream>

// ---- Standard Constructor ---------------------------------------------------
R3BSofMwpc1CalPar::R3BSofMwpc1CalPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
    , fNumPadsX(128)
    , fNumPadsY(40)
    , fNumParamsFit(1)
{
    fPadCalParams = new TArrayI((fNumPadsX + fNumPadsY) * fNumParamsFit);
}

// ----  Destructor ------------------------------------------------------------
R3BSofMwpc1CalPar::~R3BSofMwpc1CalPar()
{
    clear();
    delete fPadCalParams;
}

// ----  Method clear ----------------------------------------------------------
void R3BSofMwpc1CalPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BSofMwpc1CalPar::putParams(FairParamList* list)
{

    LOG(INFO) << "R3BSofMwpc1CalPar::putParams() called";
    if (!list)
    {
        return;
    }

    Int_t array_size = (fNumPadsX + fNumPadsY) * fNumParamsFit;
    LOG(INFO) << "Array Size: " << array_size;

    fPadCalParams->Set(array_size);

    list->add("mwpc1CalPar", *fPadCalParams);
    list->add("mwpc1PadXNumberPar", fNumPadsX);
    list->add("mwpc1PadYNumberPar", fNumPadsY);
    list->add("mwpc1ParamsFitPar", fNumParamsFit);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofMwpc1CalPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofMwpc1CalPar::getParams() called";
    if (!list)
    {
        return kFALSE;
    }

    if (!list->fill("mwpc1PadXNumberPar", &fNumPadsX))
    {
        return kFALSE;
    }
    if (!list->fill("mwpc1PadYNumberPar", &fNumPadsY))
    {
        return kFALSE;
    }
    if (!list->fill("mwpc1ParamsFitPar", &fNumParamsFit))
    {
        return kFALSE;
    }

    Int_t array_size = (fNumPadsX + fNumPadsY) * fNumParamsFit;
    LOG(INFO) << "Array Size: " << array_size;
    fPadCalParams->Set(array_size);

    if (!(list->fill("mwpc1CalPar", fPadCalParams)))
    {
        LOG(INFO) << "---Could not initialize mwpc1CalPar";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofMwpc1CalPar::printParams()
{
    LOG(INFO) << "R3BSofMwpc1CalPar: SOFIA MWPC1 Calibration Parameters: ";
    Int_t array_size = (fNumPadsX + fNumPadsY) * fNumParamsFit;

    for (Int_t i = 0; i < (fNumPadsX + fNumPadsY); i++)
    {
        if (i < fNumPadsX / 2)
        {
            LOG(INFO) << "MWPC1 Plane X down, Pad Number: " << i + 1;
        }
        else if (i < fNumPadsX)
        {
            LOG(INFO) << "MWPC1 Plane X up, Pad Number: " << i + 1 - fNumPadsX / 2;
        }
        else
        {
            LOG(INFO) << "MWPC1 Plane Y, Pad Number: " << i + 1 - fNumPadsX;
        }
        for (Int_t j = 0; j < fNumParamsFit; j++)
        {
            LOG(INFO) << "FitParam(" << j << ") = " << fPadCalParams->GetAt(i * fNumParamsFit + j);
        }
    }
}

ClassImp(R3BSofMwpc1CalPar)
