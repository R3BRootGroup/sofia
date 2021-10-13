// ------------------------------------------------------------------
// -----         R3BSofFragmentAnaPar source file               -----
// -----         Created 17/12/20  by R. Taniuchi               -----
// ------------------------------------------------------------------

#include "R3BSofFragmentAnaPar.h"

// ---- Standard Constructor ---------------------------------------------------
R3BSofFragmentAnaPar::R3BSofFragmentAnaPar(const TString& name, const TString& title, const TString& context)
    : FairParGenericSet(name, title, context)
    , fNumSci(28)
{
    fIn_use = new TArrayI(fNumSci);
    fTofW_Offset = new TArrayF(fNumSci);
    fEffective_Length = new TArrayF(fNumSci);
}

// ----  Destructor ------------------------------------------------------------
R3BSofFragmentAnaPar::~R3BSofFragmentAnaPar()
{
    clear();
    if (fIn_use)
        delete fIn_use;
    if (fTofW_Offset)
        delete fTofW_Offset;
    if (fEffective_Length)
        delete fEffective_Length;
}

// ----  Method clear ----------------------------------------------------------
void R3BSofFragmentAnaPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BSofFragmentAnaPar::putParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofFragmentAnaPar::putParams() called";
    if (!list)
    {
        return;
    }

    list->add("NumTofWPaddle", fNumSci);
    Int_t array_sci = fNumSci;
    LOG(INFO) << "Nb Paddle: " << array_sci;
    fIn_use->Set(array_sci);
    list->add("tofwInUsePar", *fIn_use);
    fTofW_Offset->Set(array_sci);
    list->add("tofwOffset", *fTofW_Offset);
    fEffective_Length->Set(array_sci);
    list->add("tofwEffectiveLength", *fEffective_Length);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofFragmentAnaPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofFragmentAnaPar::getParams() called";
    if (!list)
    {
        return kFALSE;
    }

    if (!list->fill("NumTofWPaddle", &fNumSci))
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

    fTofW_Offset->Set(array_sci);
    if (!(list->fill("tofwOffset", fTofW_Offset)))
    {
        LOG(INFO) << "---Could not initialize tofwOffset";
        return kFALSE;
    }

    fEffective_Length->Set(array_sci);
    if (!(list->fill("tofwEffectiveLength", fEffective_Length)))
    {
        LOG(INFO) << "---Could not initialize tofwEffectiveLength";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofFragmentAnaPar::printParams()
{
    LOG(INFO) << "R3BSofFragmentAnaPar: detector Parameters";
    LOG(INFO) << "R3BSofFragmentAnaPar: " << fNumSci << " sci in use: ";

    for (Int_t s = 0; s < fNumSci; s++)
    {

        LOG(INFO) << "Sci " << s + 1 << " in use " << fIn_use->GetAt(s) << ", Position: " << fTofW_Offset->GetAt(s)
                  << ", Tof: " << fEffective_Length->GetAt(s);
    }
}
