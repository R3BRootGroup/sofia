// ------------------------------------------------------------------
// -----          R3BSofGladFieldPar source file                -----
// -----      Created 22/02/21  by J.L. Rodriguez-Sanchez       -----
// ------------------------------------------------------------------

#include "R3BSofGladFieldPar.h"

// ---- Standard Constructor ---------------------------------------------------
R3BSofGladFieldPar::R3BSofGladFieldPar(const TString& name, const TString& title, const TString& context)
    : FairParGenericSet(name, title, context)
    , fBz(0.)
    , fEffLength(0.)
    , fFieldCentre(0.)
{}

// ----  Destructor ------------------------------------------------------------
R3BSofGladFieldPar::~R3BSofGladFieldPar() { clear(); }

// ----  Method clear ----------------------------------------------------------
void R3BSofGladFieldPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BSofGladFieldPar::putParams(FairParamList* list)
{
    LOG(info) << "R3BSofGladFieldPar::putParams() called";
    if (!list) {
        return;
    }
    list->add("Bz", fBz);
    list->add("EffLength", fEffLength);
    list->add("FieldCentre", fFieldCentre);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofGladFieldPar::getParams(FairParamList* list)
{
    LOG(info) << "R3BSofGladFieldPar::getParams() called";
    if (!list) {
        return kFALSE;
    }

    if (!list->fill("Bz", &fBz))
        return kFALSE;
    if (!list->fill("EffLength", &fEffLength))
        return kFALSE;
    if (!list->fill("FieldCentre", &fFieldCentre))
        return kFALSE;

    // printParams();

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofGladFieldPar::printParams()
{
    LOG(info) << "R3BSofGladFieldPar: GLAD field parameters: ";
    LOG(info) << "Field Bz: " << fBz << " [T]";
    LOG(info) << "Effective length: " << fEffLength << " [cm]";
    LOG(info) << "Field centre in z: " << fFieldCentre << " [cm]";
}

ClassImp(R3BSofGladFieldPar);
