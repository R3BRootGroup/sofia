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
{
    fSci_tof = new TArrayF(fNumSci);
    fSci_pos = new TArrayF(fNumSci);
    fIn_use = new TArrayI(fNumSci);
    fSci_vel0 = new TArrayF(fNumSci);
    fSci_vel1 = new TArrayF(fNumSci);
}

// ----  Destructor ------------------------------------------------------------
R3BSofTofWHitPar::~R3BSofTofWHitPar()
{
    clear();
    if (fIn_use)
        delete fIn_use;
    if (fSci_pos)
        delete fSci_pos;
    if (fSci_tof)
        delete fSci_tof;
    if (fSci_vel0)
        delete fSci_vel0;
    if (fSci_vel1)
        delete fSci_vel1;
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
    Int_t array_sci = fNumSci;
    LOG(INFO) << "Nb Sci: " << array_sci;
    fIn_use->Set(array_sci);
    list->add("tofwInUsePar", *fIn_use);
    fSci_pos->Set(array_sci);
    list->add("tofwPosPar", *fSci_pos);
    fSci_tof->Set(array_sci);
    list->add("tofwTofPar", *fSci_tof);
    fSci_vel0->Set(array_sci);
    list->add("tofwVelPar0", *fSci_vel0);
    fSci_vel1->Set(array_sci);
    list->add("tofwVelPar1", *fSci_vel1);
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
        return kFALSE;
    }

    Int_t array_sci = fNumSci;
    LOG(INFO) << "Nb Sci: " << array_sci;
    fIn_use->Set(array_sci);
    if (!(list->fill("tofwInUsePar", fIn_use)))
    {
        LOG(INFO) << "---Could not initialize tofwInUsePar";
        return kFALSE;
    }

    fSci_pos->Set(array_sci);
    if (!(list->fill("tofwPosPar", fSci_pos)))
    {
        LOG(INFO) << "---Could not initialize tofwPosPar";
        return kFALSE;
    }

    fSci_tof->Set(array_sci);
    if (!(list->fill("tofwTofPar", fSci_tof)))
    {
        LOG(INFO) << "---Could not initialize tofwTofPar";
        return kFALSE;
    }

    fSci_vel0->Set(array_sci);
    if (!(list->fill("tofwVelPar0", fSci_vel0)))
    {
        LOG(INFO) << "---Could not initialize tofwVelPar0";
        return kFALSE;
    }

    fSci_vel1->Set(array_sci);
    if (!(list->fill("tofwVelPar1", fSci_vel1)))
    {
        LOG(INFO) << "---Could not initialize tofwVelPar1";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofTofWHitPar::printParams()
{
    LOG(INFO) << "R3BSofTofWHitPar: detector Parameters";
    LOG(INFO) << "R3BSofTofWHitPar: " << fNumSci << " sci in use: ";

    for (Int_t s = 0; s < fNumSci; s++)
    {

        LOG(INFO) << "Sci " << s + 1 << " in use " << fIn_use->GetAt(s) << ", Position: " << fSci_pos->GetAt(s)
                  << ", Tof: " << fSci_tof->GetAt(s) << ", Vel0: " << fSci_vel0->GetAt(s)
                  << ", Vel1: " << fSci_vel1->GetAt(s);
    }
}
