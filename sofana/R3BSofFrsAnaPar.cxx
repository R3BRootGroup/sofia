// ------------------------------------------------------------------
// -----         R3BSofFrsAnaPar source file                    -----
// -----         Created 27/01/20  by J.L. Rodriguez-Sanchez    -----
// ------------------------------------------------------------------

#include "R3BSofFrsAnaPar.h"
#include "FairLogger.h"
#include "FairParamList.h"

#include "TArrayF.h"
#include "TMath.h"
#include "TString.h"

#include <iostream>

// ---- Standard Constructor ---------------------------------------------------
R3BSofFrsAnaPar::R3BSofFrsAnaPar(const TString& name, const TString& title, const TString& context)
    : FairParGenericSet(name, title, context)
    , fMagS2Cave(0)
    , fDispS2Cave(0)
    , fPathS2Cave(0)
    , fTOFS2Cave(0)
    , fPosFocalS2(0)
    , fPosFocalCave(0)
    , frho_S0_S2(0)
    , fBfield_S0_S2(0)
    , frho_S2_Cave(0)
    , fBfield_S2_Cave(0)
{
}

// ----  Destructor ------------------------------------------------------------
R3BSofFrsAnaPar::~R3BSofFrsAnaPar() { clear(); }

// ----  Method clear ----------------------------------------------------------
void R3BSofFrsAnaPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BSofFrsAnaPar::putParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofFrsAnaPar::putParams() called";
    if (!list)
    {
        return;
    }
    list->add("MagnificationS2Cave", fMagS2Cave);
    list->add("DisperisionS2Cave", fDispS2Cave);
    list->add("PathS2Cave", fPathS2Cave);
    list->add("ToFS2Cave", fTOFS2Cave);
    list->add("PosFocalS2", fPosFocalS2);
    list->add("PosFocalCave", fPosFocalCave);
    list->add("Rho_S0_S2", frho_S0_S2);
    list->add("Bfield_S0_S2", fBfield_S0_S2);
    list->add("Rho_S2_Cave", frho_S2_Cave);
    list->add("Bfield_S2_Cave", fBfield_S2_Cave);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofFrsAnaPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofFrsAnaPar::getParams() called";
    if (!list)
    {
        return kFALSE;
    }

    if (!list->fill("MagnificationS2Cave", &fMagS2Cave))
    {
        return kFALSE;
    }

    if (!list->fill("DisperisionS2Cave", &fDispS2Cave))
    {
        return kFALSE;
    }
    if (!list->fill("PathS2Cave", &fPathS2Cave))
    {
        return kFALSE;
    }
    if (!list->fill("ToFS2Cave", &fTOFS2Cave))
    {
        return kFALSE;
    }
    if (!list->fill("PosFocalS2", &fPosFocalS2))
    {
        return kFALSE;
    }
    if (!list->fill("PosFocalCave", &fPosFocalCave))
    {
        return kFALSE;
    }
    if (!list->fill("Rho_S0_S2", &frho_S0_S2))
    {
        return kFALSE;
    }
    if (!list->fill("Bfield_S0_S2", &fBfield_S0_S2))
    {
        return kFALSE;
    }
    if (!list->fill("Rho_S2_Cave", &frho_S2_Cave))
    {
        return kFALSE;
    }
    if (!list->fill("Bfield_S2_Cave", &fBfield_S2_Cave))
    {
        return kFALSE;
    }
    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofFrsAnaPar::printParams()
{
    LOG(INFO) << "R3BSofFrsAnaPar: Frs Analysis Parameters: ";
    LOG(INFO) << "MagnificationS2Cave " << fMagS2Cave;
    LOG(INFO) << "DisperisionS2Cave " << fDispS2Cave;
    LOG(INFO) << "PathS2Cave " << fPathS2Cave;
    LOG(INFO) << "ToFS2Cave " << fTOFS2Cave;
    LOG(INFO) << "PosFocalS2 " << fPosFocalS2;
    LOG(INFO) << "PosFocalCave " << fPosFocalCave;
    LOG(INFO) << "Rho_S0_S2 " << frho_S0_S2;
    LOG(INFO) << "Bfield_S0_S2 " << fBfield_S0_S2;
    LOG(INFO) << "Rho_S2_Cave " << frho_S2_Cave;
    LOG(INFO) << "Bfield_S2_Cave " << fBfield_S2_Cave;
}
