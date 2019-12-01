// --------------------------------------------------------------
// -----          R3BSofTwimHitPar source file              -----
// -----     Created 24/11/19  by J.L. Rodriguez-Sanchez    -----
// --------------------------------------------------------------

#ifndef R3BSofTwimHitPar_H
#define R3BSofTwimHitPar_H

#include "FairParGenericSet.h" // for FairParGenericSet

#include "TArrayF.h"
#include "TObjArray.h"
#include "TObject.h"
#include <TObjString.h>

class FairParamList;

class R3BSofTwimHitPar : public FairParGenericSet
{

  public:
    /** Standard constructor **/
    R3BSofTwimHitPar(const char* name = "twimHitPar",
                     const char* title = "TWIM Hit Parameters",
                     const char* context = "TwimHitParContext");

    /** Destructor **/
    virtual ~R3BSofTwimHitPar();

    /** Method to reset all parameters **/
    virtual void clear();

    /** Method to store all parameters using FairRuntimeDB **/
    virtual void putParams(FairParamList* list);

    /** Method to retrieve all parameters using FairRuntimeDB**/
    Bool_t getParams(FairParamList* list);

    /** Method to print values of parameters to the standard output **/
    void printParams();

    /** Accessor functions **/
    const Int_t GetNumSec() { return fNumSec; }
    const Double_t GetNumParametersFit() { return fNumParamsFit; }
    TArrayF* GetDetectorHitParams() { return fDetHitParams; }

    void SetNumSec(Int_t nbsec) { fNumSec = nbsec; }
    void SetNumParametersFit(Int_t numberParams) { fNumParamsFit = numberParams; }
    void SetDetectorHitParams(Double_t cc, Int_t ii) { fDetHitParams->AddAt(cc, ii); }

    // Create more Methods if you need them!

  private:
    TArrayF* fDetHitParams; // Calibration Parameters for detector
    Int_t fNumSec;
    Int_t fNumParamsFit; /* number of cal parameters in the fit
                            gaus: A_fit & B_fit & C_fit*/

    const R3BSofTwimHitPar& operator=(const R3BSofTwimHitPar&); /*< an assignment operator>*/

    R3BSofTwimHitPar(const R3BSofTwimHitPar&); /*< a copy constructor >*/

    ClassDef(R3BSofTwimHitPar, 1);
};

#endif
