// ------------------------------------------------------------------
// -----             R3BSofTrimHitPar source file               -----
// ------------------------------------------------------------------

#ifndef R3BSofTrimHitPar_H
#define R3BSofTrimHitPar_H

#include "FairParGenericSet.h" // for FairParGenericSet

#include "TArrayD.h"
#include "TArrayF.h"
#include "TObjArray.h"
#include "TObject.h"
#include <TObjString.h>

class FairParamList;

class R3BSofTrimHitPar : public FairParGenericSet
{

  public:
    /** Standard constructor **/
    R3BSofTrimHitPar(const char* name = "trimHitPar",
                     const char* title = "Triple MUSIC Hit Parameters",
                     const char* context = "TrimHitParContext");

    /** Destructor **/
    virtual ~R3BSofTrimHitPar();

    /** Method to reset all parameters **/
    virtual void clear();

    /** Method to store all parameters using FairRuntimeDB **/
    virtual void putParams(FairParamList* list);

    /** Method to retrieve all parameters using FairRuntimeDB**/
    Bool_t getParams(FairParamList* list);

    /** Method to print values of parameters to the standard output **/
    void printParams();

    /** Accessor functions **/

    // === ================== === //
    // === NUMBER OF SECTIONS === //
    // === ================== === //

    const Int_t GetNumSections() { return fNumSections; }

    void SetNumSections(Int_t num) { fNumSections = num; }

    // === ============================= === //
    // === NUMBER OF SIGNALS PER SECTION === //
    // === ============================= === //

    const Int_t GetNumSignalsPerSection() { return fNumSignalsPerSection; }

    void SetNumSignalsPerSection(Int_t num) { fNumSignalsPerSection = num; }

    // === =================================== === //
    // === CORRECTION E[signal] VERSUS DeltaDT === //
    // === =================================== === //

    const Int_t GetNumCorrDeltaDTParsPerSignal() { return fNumCorrDeltaDTParsPerSignal; }

    void SetNumCorrDeltaDTParsPerSignal(Int_t num) { fNumCorrDeltaDTParsPerSignal = num; }

    Float_t GetEnergyCorrDeltaDTPar(Int_t section, Int_t signal, Int_t degree)
    {
        return fEnergyCorrDeltaDTPars->GetAt((section - 1) * (fNumCorrDeltaDTParsPerSignal * fNumSignalsPerSection) +
                                             (signal - 1) * fNumCorrDeltaDTParsPerSignal + degree);
    } // section is 1-based, signal is 1-based, degree is 0-based

    TArrayF* GetEnergyCorrDeltaDTPars() { return fEnergyCorrDeltaDTPars; }

    void SetEnergyCorrDeltaDTPar(Float_t val, Int_t section, Int_t signal, Int_t degree)
    {
        fEnergyCorrDeltaDTPars->AddAt(val,
                                      (section - 1) * (fNumSignalsPerSection * fNumCorrDeltaDTParsPerSignal) +
                                          (signal - 1) * fNumCorrDeltaDTParsPerSignal + degree);
    } // section is 1-based, signal is 1-based, degree is 0-based

    // === ==================================== === //
    // === ALIGNEMENT OF THE ENERGY PER SECTION === //
    // === ==================================== === //

    Float_t GetEnergyAlignOffset(Int_t section) { return fEnergyAlignOffsets->GetAt(section - 1); }
    Float_t GetEnergyAlignGain(Int_t section) { return fEnergyAlignGains->GetAt(section - 1); }

    TArrayF* GetEnergyAlignOffsets() { return fEnergyAlignOffsets; }
    TArrayF* GetEnergyAlignGains() { return fEnergyAlignGains; }

    void SetEnergyAlignOffset(Float_t val, Int_t section) { fEnergyAlignOffsets->AddAt(val, section - 1); }
    void SetEnergyAlignGain(Float_t val, Int_t section) { fEnergyAlignGains->AddAt(val, section - 1); }

    // === ================================================= === //
    // === CORRECTION IN BETA OF THE ENERGY LOSS PER SECTION === //
    // === ================================================= === //

    const Int_t GetNumCorrBetaParsPerSection() { return fNumCorrBetaParsPerSection; }

    void SetNumCorrBetaParsPerSection(Int_t num) { fNumCorrBetaParsPerSection = num; }

    Float_t GetEnergyCorrBetaPar(Int_t section, Int_t degree)
    {
        return fEnergyCorrBetaPars->GetAt((section - 1) * fNumCorrBetaParsPerSection + degree);
    } // degree is 0-based

    TArrayF* GetEnergyCorrBetaPars() { return fEnergyCorrBetaPars; }

    void SetEnergyCorrBetaPar(Float_t val, Int_t section, Int_t degree)
    {
        fEnergyCorrBetaPars->AddAt(val, (section - 1) * fNumCorrBetaParsPerSection + degree);
    } // section is 1-based, degree is 0-based

  private:
    Int_t fNumSections;                 // number of sections
    Int_t fNumSignalsPerSection;        // 6 if Rectangular anodes, 3 if Triangular anodes
    Int_t fNumCorrDeltaDTParsPerSignal; // if pol3 -> 4 parameters (P0, P1, P2, P3)
    Int_t fNumCorrBetaParsPerSection;   // if pol2 -> 3 parameters (P0, P1, P2)

    TArrayF* fEnergyCorrDeltaDTPars;
    TArrayF* fEnergyAlignOffsets;
    TArrayF* fEnergyAlignGains;
    TArrayF* fEnergyCorrBetaPars;

    const R3BSofTrimHitPar& operator=(const R3BSofTrimHitPar&); /*< an assignment operator>*/

    R3BSofTrimHitPar(const R3BSofTrimHitPar&); /*< a copy constructor >*/

    ClassDef(R3BSofTrimHitPar, 1);
};

#endif
