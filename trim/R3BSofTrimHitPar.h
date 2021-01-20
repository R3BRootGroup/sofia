// ------------------------------------------------------------------
// -----             R3BSofTrimHitPar source file               -----
// ------------------------------------------------------------------

#ifndef R3BSofTrimHitPar_H
#define R3BSofTrimHitPar_H

#include "FairParGenericSet.h" // for FairParGenericSet

#include "TArrayF.h"
#include "TArrayD.h"
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
    const Int_t GetNumSections() { return fNumSections; }
    const Int_t GetNumAlignGainsPerSection() { return fNumAlignGainsPerSection;}
    const Int_t GetNumCorrBetaParsPerSection() {return fNumCorrBetaParsPerSection; }

    Float_t  GetEnergyAlignGain(Int_t section, Int_t index) { return fEnergyAlignGains->GetAt((section-1)*fNumAlignGainsPerSection + index); } // index is 0-based
    TArrayF* GetEnergyAlignGains() { return fEnergyAlignGains; }
    Float_t  GetEnergyCorrBetaPar(Int_t section, Int_t degree) { return fEnergyCorrBetaPars->GetAt((section-1)*fNumCorrBetaParsPerSection + degree);} // degree is 0-based
    TArrayF* GetEnergyCorrBetaPars() { return fEnergyCorrBetaPars; }

    void SetNumSections(Int_t num) { fNumSections = num; }
    void SetNumAlignGainsPerSection(Int_t num) { fNumAlignGainsPerSection = num; }
    void SetNumCorrBetaParsPerSection(Int_t num) { fNumCorrBetaParsPerSection = num; }

    void SetEnergyAlignGain(Float_t val, Int_t section, Int_t index)  { fEnergyAlignGains->AddAt(val, (section-1)*fNumAlignGainsPerSection + index);}

    void SetEnergyCorrBetaPar(Float_t val, Int_t section, Int_t degree) { fEnergyCorrBetaPars->AddAt(val, (section-1)*fNumCorrBetaParsPerSection + degree);} // degree is 0-based

  private:
    Int_t fNumSections;               // number of sections
    Int_t fNumAlignGainsPerSection;
    Int_t fNumCorrBetaParsPerSection; // if pol2 -> 3 parameters (P0, P1, P2) P0 at degree=0, P1 at degree=1, P2 at degree=2 
    TArrayF* fEnergyAlignGains;       
    TArrayF* fEnergyCorrBetaPars;    

    const R3BSofTrimHitPar& operator=(const R3BSofTrimHitPar&); /*< an assignment operator>*/

    R3BSofTrimHitPar(const R3BSofTrimHitPar&); /*< a copy constructor >*/

    ClassDef(R3BSofTrimHitPar, 1);
};

#endif
