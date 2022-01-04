// ------------------------------------------------------------------
// -----            R3BSofTofWHitPar source file                -----
// -----       Created 20/02/20  by J.L. Rodriguez-Sanchez      -----
// ------------------------------------------------------------------

#include "R3BSofTofWHitPar.h"

#include "FairLogger.h"
#include "FairParamList.h"

#include "TArrayF.h"
#include "TMath.h"
#include "TString.h"

#include <iostream>

// ---- Standard Constructor ---------------------------------------------------
R3BSofTofWHitPar::R3BSofTofWHitPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
    , fNumSci(28)
    , fTofAlign(30.0)
{
    fIn_use = new TArrayI(fNumSci);
    fSci_tof = new TArrayF(fNumSci);
    fSci_posoffset = new TArrayF(fNumSci);
    fSci_posslope = new TArrayF(fNumSci);
}

// ----  Destructor ------------------------------------------------------------
R3BSofTofWHitPar::~R3BSofTofWHitPar()
{
    clear();
    if (fIn_use)
        delete fIn_use;
    if (fSci_tof)
        delete fSci_tof;
    if (fSci_posoffset)
        delete fSci_posoffset;
    if (fSci_posslope)
        delete fSci_posslope;
}

// ----  Method clear ----------------------------------------------------------
void R3BSofTofWHitPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BSofTofWHitPar::putParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofTofWHitPar::putParams() called";
    if (!list)
    {
        return;
    }

    list->add("tofwHitSciPar", fNumSci);
    list->add("tofwAlignTofPar", fTofAlign);
    Int_t array_sci = fNumSci;
    LOG(INFO) << "Nb Sci: " << array_sci;
    fIn_use->Set(array_sci);
    list->add("tofwInUsePar", *fIn_use);
    fSci_posoffset->Set(array_sci);
    list->add("tofwPosOffsetPar", *fSci_posoffset);
    fSci_posslope->Set(array_sci);
    list->add("tofwPosSlopePar", *fSci_posslope);
    fSci_tof->Set(array_sci);
    list->add("tofwTofPar", *fSci_tof);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofTofWHitPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofTofWHitPar::getParams() called";
    if (!list)
    {
        return kFALSE;
    }

    if (!list->fill("tofwHitSciPar", &fNumSci))
    {
        LOG(FATAL) << "Could not initialize tofwInUsePar";
        return kFALSE;
    }

    if (!list->fill("tofwAlignTofPar", &fTofAlign))
    {
        LOG(FATAL) << "Could not initialize tofwAlignTofPar";
        return kFALSE;
    }

    Int_t array_sci = fNumSci;
    LOG(INFO) << "Nb Sci: " << array_sci;
    fIn_use->Set(array_sci);
    if (!(list->fill("tofwInUsePar", fIn_use)))
    {
        LOG(FATAL) << "Could not initialize tofwInUsePar";
        return kFALSE;
    }

    fSci_posoffset->Set(array_sci);
    if (!(list->fill("tofwPosOffsetPar", fSci_posoffset)))
    {
        LOG(FATAL) << "Could not initialize tofwPosOffsetPar";
        return kFALSE;
    }

    fSci_posslope->Set(array_sci);
    if (!(list->fill("tofwPosSlopePar", fSci_posslope)))
    {
        LOG(FATAL) << "Could not initialize tofwPosSlopePar";
        return kFALSE;
    }

    fSci_tof->Set(array_sci);
    if (!(list->fill("tofwTofPar", fSci_tof)))
    {
        LOG(FATAL) << "Could not initialize tofwTofPar";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofTofWHitPar::printParams()
{
    LOG(INFO) << "R3BSofTofWHitPar: detector Parameters";
    LOG(INFO) << "R3BSofTofWHitPar: " << fNumSci << " sci in use: ";
    LOG(INFO) << "R3BSofTofWHitPar: Tof alignment (in ns)" << fTofAlign;

    for (Int_t s = 0; s < fNumSci; s++)
    {

        LOG(INFO) << "Sci " << s + 1 << " in use " << fIn_use->GetAt(s) << ", Position: " << fSci_posoffset->GetAt(s)
                  << "," << fSci_posslope->GetAt(s) << ", Tof: " << fSci_tof->GetAt(s);
    }
}
