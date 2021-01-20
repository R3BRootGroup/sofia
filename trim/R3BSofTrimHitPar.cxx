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
    , fNumAlignGainsPerSection(3) // 3 if triangular, 6 if rectangular
    , fNumCorrBetaParsPerSection(3)
{
    fEnergyAlignGains = new TArrayF(fNumSections * fNumAlignGainsPerSection);
    fEnergyCorrBetaPars = new TArrayF(fNumSections * fNumCorrBetaParsPerSection);
}

// ----  Destructor ------------------------------------------------------------
R3BSofTrimHitPar::~R3BSofTrimHitPar()
{
    clear();
    if (fEnergyAlignGains)    delete fEnergyAlignGains;
    if (fEnergyCorrBetaPars)  delete fEnergyCorrBetaPars;
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
    array_size = fNumSections * fNumAlignGainsPerSection;
    LOG(INFO) << "Array Size align gains: " << array_size;
    fEnergyAlignGains->Set(array_size);

    array_size = fNumSections * fNumCorrBetaParsPerSection;
    LOG(INFO) << "Array Size corr beta parameters: " << array_size;
    fEnergyCorrBetaPars->Set(array_size);

    list->add("trimNumSections", fNumSections);
    list->add("trimNumAlignGainsPerSection", fNumAlignGainsPerSection);
    list->add("trimNumCorrBetaParsPerSection", fNumCorrBetaParsPerSection);
    list->add("trimEnergyAlignGains",*fEnergyAlignGains);
    list->add("trimEnergyCorrBetaPars",*fEnergyCorrBetaPars);

}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofTrimHitPar::getParams(FairParamList* list)
{
  
  LOG(INFO) << "R3BSofTrimHitPar::getParams() called";
  if (!list){
    return kFALSE;
  }
  
  if (!list->fill("trimNumSections", &fNumSections)){
    return kFALSE;
  }
  
  if (!list->fill("trimNumAlignGainsPerSection", &fNumAlignGainsPerSection)){
    return kFALSE;
  }
  
  if (!list->fill("trimNumCorrBetaParsPerSection", &fNumCorrBetaParsPerSection)){
    return kFALSE;
  }
  
  Int_t array_size;

  array_size = fNumSections * fNumAlignGainsPerSection;
  LOG(INFO) << "Array Size for align gain in use: " << array_size;  
  fEnergyAlignGains->Set(array_size);
  if (!(list->fill("trimEnergyAlignGains", fEnergyAlignGains))){
    LOG(INFO) << "---Could not initialize trimEnergyAlignGains";
    return kFALSE;
  }

  array_size = fNumSections * fNumCorrBetaParsPerSection;
  LOG(INFO) << "Array Size for beta corr in use: " << array_size;
  fEnergyCorrBetaPars->Set(array_size);
  if(!(list->fill("trimEnergyCorrBetaPars", fEnergyCorrBetaPars))){
    LOG(INFO) << "---Could not initialize trimEnergyCorrBetaPars";
    return kFALSE;
  }
  
  return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofTrimHitPar::printParams()
{
  
  LOG(INFO) << "R3BSofTrimHitPar: Triple MUSIC energy aligned gain per pair of down/up anode: ";
  for (Int_t s = 0; s < fNumSections; s++){
    LOG(INFO) << "Trim section: " << s+1;
    for (Int_t i = 0; i < fNumAlignGainsPerSection; i++){
      LOG(INFO) << "index: " << i << ": Energy Align Gain = " << GetEnergyAlignGain(s+1, i);
    }
  }

  LOG(INFO) << "R3BSofTrimHitPar: Triple MUSIC energy per section correction from beta: ";
  for (Int_t s = 0; s < fNumSections; s++){
    LOG(INFO) << "Trim section: " << s+1;
    for (Int_t p=0; p<fNumCorrBetaParsPerSection; p++){
      LOG(INFO) << "P" << p << " = " << GetEnergyCorrBetaPar(s+1,p);
    }
  }

}
