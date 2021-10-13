// ------------------------------------------------------------------
// -----             R3BSofTrimHitPar source file                ----
// ------------------------------------------------------------------

#include "R3BSofTrimHitPar.h"

#include "FairLogger.h"
#include "FairParamList.h"

#include "TArrayF.h"
#include "TArrayI.h"
#include "TMath.h"
#include "TString.h"

#include <iostream>

// ---- Standard Constructor ---------------------------------------------------
R3BSofTrimHitPar::R3BSofTrimHitPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
    , fNumSections(3)
    , fNumSignalsPerSection(3) // 3 if triangular, 6 if rectangular
    , fNumCorrDeltaDTParsPerSignal(4)
    , fNumCorrBetaParsPerSection(3)
{
    fEnergyAlignOffsets = new TArrayF(fNumSections);
    fEnergyAlignGains = new TArrayF(fNumSections);
    fEnergyCorrBetaPars = new TArrayF(fNumSections * fNumCorrBetaParsPerSection);
    fEnergyCorrDeltaDTPars = new TArrayF(fNumSections * fNumSignalsPerSection * fNumCorrDeltaDTParsPerSignal);
}

// ----  Destructor ------------------------------------------------------------
R3BSofTrimHitPar::~R3BSofTrimHitPar()
{
    clear();
    if (fEnergyCorrDeltaDTPars)
        delete fEnergyCorrDeltaDTPars;
    if (fEnergyAlignOffsets)
        delete fEnergyAlignOffsets;
    if (fEnergyAlignGains)
        delete fEnergyAlignGains;
    if (fEnergyCorrBetaPars)
        delete fEnergyCorrBetaPars;
}

// ----  Method clear ----------------------------------------------------------
void R3BSofTrimHitPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BSofTrimHitPar::putParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofTrimHitPar::putParams() called";
    if (!list)
    {
        return;
    }
    Int_t array_size;

    array_size = fNumSections;
    LOG(INFO) << "Array Size align gains: " << array_size;
    fEnergyAlignGains->Set(array_size);

    array_size = fNumSections;
    LOG(INFO) << "Array Size align offset: " << array_size;
    fEnergyAlignOffsets->Set(array_size);

    array_size = fNumSections * fNumCorrBetaParsPerSection;
    LOG(INFO) << "Array Size corr beta parameters: " << array_size;
    fEnergyCorrBetaPars->Set(array_size);

    array_size = fNumSections * fNumSignalsPerSection * fNumCorrDeltaDTParsPerSignal;
    LOG(INFO) << "Array Size corr deltaDT parameters: " << array_size;
    fEnergyCorrDeltaDTPars->Set(array_size);

    list->add("trimNumSections", fNumSections);
    list->add("trimNumCorrDeltaDTParsPerSignal", fNumCorrDeltaDTParsPerSignal);
    list->add("trimNumSignalsPerSection", fNumSignalsPerSection);
    list->add("trimNumCorrBetaParsPerSection", fNumCorrBetaParsPerSection);
    list->add("trimEnergyCorrDeltaDTPars", *fEnergyCorrDeltaDTPars);
    list->add("trimEnergyAlignOffsets", *fEnergyAlignOffsets);
    list->add("trimEnergyAlignGains", *fEnergyAlignGains);
    list->add("trimEnergyCorrBetaPars", *fEnergyCorrBetaPars);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofTrimHitPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofTrimHitPar::getParams() called";
    if (!list)
    {
        return kFALSE;
    }

    if (!list->fill("trimNumSections", &fNumSections))
    {
        return kFALSE;
    }

    if (!list->fill("trimNumCorrDeltaDTParsPerSignal", &fNumCorrDeltaDTParsPerSignal))
    {
        return kFALSE;
    }

    if (!list->fill("trimNumSignalsPerSection", &fNumSignalsPerSection))
    {
        return kFALSE;
    }

    if (!list->fill("trimNumCorrBetaParsPerSection", &fNumCorrBetaParsPerSection))
    {
        return kFALSE;
    }

    Int_t array_size;

    array_size = fNumSections * fNumSignalsPerSection * fNumCorrDeltaDTParsPerSignal;
    LOG(INFO) << "Array Size for deltaDT per pair in use: " << array_size;
    fEnergyCorrDeltaDTPars->Set(array_size);
    if (!(list->fill("trimEnergyCorrDeltaDTPars", fEnergyCorrDeltaDTPars)))
    {
        LOG(INFO) << "---Could not initialize trimEnergyCorrDeltaDTPars";
        return kFALSE;
    }

    array_size = fNumSections;
    LOG(INFO) << "Array Size for align offset in use: " << array_size;
    fEnergyAlignOffsets->Set(array_size);
    if (!(list->fill("trimEnergyAlignOffsets", fEnergyAlignOffsets)))
    {
        LOG(INFO) << "---Could not initialize trimEnergyAlignOffsets";
        return kFALSE;
    }
    LOG(INFO) << "Array Size for align gain in use: " << array_size;
    fEnergyAlignGains->Set(array_size);
    if (!(list->fill("trimEnergyAlignGains", fEnergyAlignGains)))
    {
        LOG(INFO) << "---Could not initialize trimEnergyAlignGains";
        return kFALSE;
    }

    array_size = fNumSections * fNumCorrBetaParsPerSection;
    LOG(INFO) << "Array Size for beta corr in use: " << array_size;
    fEnergyCorrBetaPars->Set(array_size);
    if (!(list->fill("trimEnergyCorrBetaPars", fEnergyCorrBetaPars)))
    {
        LOG(INFO) << "---Could not initialize trimEnergyCorrBetaPars";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofTrimHitPar::printParams()
{

    LOG(INFO) << "R3BSofTrimHitPar: Triple MUSIC energy aligned gain per pair of down/up anode: ";
    for (Int_t sec = 0; sec < fNumSections; sec++)
    {
        LOG(INFO) << "Trim section: " << sec + 1;
        for (Int_t sig = 0; sig < fNumSignalsPerSection; sig++)
        {
            LOG(INFO) << "signal : " << sig + 1;
            for (Int_t deg = 0; deg < fNumCorrDeltaDTParsPerSignal; deg++)
            {
                LOG(INFO) << "p: " << deg << " = " << GetEnergyCorrDeltaDTPar(sec + 1, sig + 1, deg);
            }
        }
    }
    for (Int_t s = 0; s < fNumSections; s++)
    {
        LOG(INFO) << "Trim section: " << s + 1 << " : Energy Align Offset = " << GetEnergyAlignOffset(s + 1);
        LOG(INFO) << "Trim section: " << s + 1 << " : Energy Align Gain = " << GetEnergyAlignGain(s + 1);
    }

    LOG(INFO) << "R3BSofTrimHitPar: Triple MUSIC energy per section correction from beta: ";
    for (Int_t s = 0; s < fNumSections; s++)
    {
        LOG(INFO) << "Trim section: " << s + 1;
        for (Int_t deg = 0; deg < fNumCorrBetaParsPerSection; deg++)
        {
            LOG(INFO) << "P" << deg << " = " << GetEnergyCorrBetaPar(s + 1, deg);
        }
    }
}

ClassImp(R3BSofTrimHitPar)
