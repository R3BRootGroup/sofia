// ------------------------------------------------------------------
// -----             R3BSofTrimCalPar source file               -----
// ------------------------------------------------------------------

#include "R3BSofTrimCalPar.h"

#include "FairLogger.h"
#include "FairParamList.h"
#include "TArrayF.h"
#include "TArrayI.h"
#include "TMath.h"
#include "TString.h"

#include <iostream>

// ---- Standard Constructor ---------------------------------------------------
R3BSofTrimCalPar::R3BSofTrimCalPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
    , fNumSections(3)
    , fNumAnodes(6)
{
    fDriftTimeOffsets = new TArrayD(fNumSections * fNumAnodes);
    fEnergyPedestals = new TArrayF(fNumSections * fNumAnodes);
    fEnergyMatchGains = new TArrayF(fNumSections * fNumAnodes);
}

// ----  Destructor ------------------------------------------------------------
R3BSofTrimCalPar::~R3BSofTrimCalPar()
{
    clear();
    if (fDriftTimeOffsets)
        delete fDriftTimeOffsets;
    if (fEnergyPedestals)
        delete fEnergyPedestals;
    if (fEnergyMatchGains)
        delete fEnergyMatchGains;
}

// ----  Method clear ----------------------------------------------------------
void R3BSofTrimCalPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BSofTrimCalPar::putParams(FairParamList* list)
{
    LOG(info) << "R3BSofTrimCalPar::putParams() called";
    if (!list) {
        return;
    }
    Int_t array_size;
    array_size = fNumSections * fNumAnodes;
    LOG(info) << "Array Size: " << array_size;
    fDriftTimeOffsets->Set(array_size);
    fEnergyPedestals->Set(array_size);
    fEnergyMatchGains->Set(array_size);

    list->add("trimNumSections", fNumSections);
    list->add("trimNumAnodesPerSection", fNumAnodes);
    list->add("trimDriftTimeOffsets", *fDriftTimeOffsets);
    list->add("trimEnergyPedestals", *fEnergyPedestals);
    list->add("trimEnergyMatchGains", *fEnergyMatchGains);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofTrimCalPar::getParams(FairParamList* list)
{

    LOG(info) << "R3BSofTrimCalPar::getParams() called";
    if (!list) {
        return kFALSE;
    }

    if (!list->fill("trimNumSections", &fNumSections)) {
        return kFALSE;
    }

    if (!list->fill("trimNumAnodesPerSection", &fNumAnodes)) {
        return kFALSE;
    }

    Int_t array_anode = fNumSections * fNumAnodes;
    LOG(info) << "Array Size for anode in use: " << array_anode;

    fDriftTimeOffsets->Set(array_anode);
    if (!(list->fill("trimDriftTimeOffsets", fDriftTimeOffsets))) {
        LOG(info) << "---Could not initialize trimDriftTimeOffsets";
        return kFALSE;
    }

    fEnergyPedestals->Set(array_anode);
    if (!(list->fill("trimEnergyPedestals", fEnergyPedestals))) {
        LOG(info) << "---Could not initialize trimEnergyPedestals";
        return kFALSE;
    }

    fEnergyMatchGains->Set(array_anode);
    if (!(list->fill("trimEnergyMatchGains", fEnergyMatchGains))) {
        LOG(info) << "---Could not initialize trimEnergyMatchGains";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofTrimCalPar::printParams()
{
    LOG(info) << "R3BSofTrimCalPar: Triple MUSIC offsets for drift time: ";

    for (Int_t s = 0; s < fNumSections; s++) {
        LOG(info) << "Trim section: " << s + 1;
        for (Int_t a = 0; a < fNumAnodes; a++) {
            LOG(info) << "Anode number: " << a + 1 << ": Drift time offset = " << GetDriftTimeOffset(s + 1, a + 1);
        }
    }

    LOG(info) << "R3BSofTrimCalPar: Triple MUSIC energy pedestal: ";
    for (Int_t s = 0; s < fNumSections; s++) {
        LOG(info) << "Trim section: " << s + 1;
        for (Int_t a = 0; a < fNumAnodes; a++) {
            LOG(info) << "Anode number: " << a + 1 << ": Energy pedestal = " << GetEnergyPedestal(s + 1, a + 1);
        }
    }

    LOG(info) << "R3BSofTrimCalPar: Triple MUSIC energy match gain per pair of down/up anode: ";
    for (Int_t s = 0; s < fNumSections; s++) {
        LOG(info) << "Trim section: " << s + 1;
        for (Int_t a = 0; a < fNumAnodes; a++) {
            LOG(info) << "Anode number: " << a + 1 << ": Energy Match Gain = " << GetEnergyMatchGain(s + 1, a + 1);
        }
    }
}

ClassImp(R3BSofTrimCalPar)
