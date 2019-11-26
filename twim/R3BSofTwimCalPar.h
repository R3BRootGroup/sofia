// ------------------------------------------------------------------
// -----             R3BSofTwimCalPar source file               -----
// -----         Created 24/11/19  by J.L. Rodriguez-Sanchez    -----
// ------------------------------------------------------------------

#ifndef R3BSofTwimCalPar_H
#define R3BSofTwimCalPar_H

#include "FairParGenericSet.h" // for FairParGenericSet

#include "TArrayF.h"
#include "TObjArray.h"
#include "TObject.h"
#include <TObjString.h>

class FairParamList;

class R3BSofTwimCalPar : public FairParGenericSet
{

  public:
    /** Standard constructor **/
    R3BSofTwimCalPar(const char* name = "musicCalPar",
                     const char* title = "MUSIC Cal Parameters",
                     const char* context = "MusicCalParContext");

    /** Destructor **/
    virtual ~R3BSofTwimCalPar();

    /** Method to reset all parameters **/
    virtual void clear();

    /** Method to store all parameters using FairRuntimeDB **/
    virtual void putParams(FairParamList* list);

    /** Method to retrieve all parameters using FairRuntimeDB**/
    Bool_t getParams(FairParamList* list);

    /** Method to print values of parameters to the standard output **/
    void printParams();

    /** Accessor functions **/
    const Int_t GetNumSec() { return fNumSections; }
    const Int_t GetNumAnodes() { return fNumAnodes; }
    const Int_t GetNumParametersFit() { return fNumParamsFit; }
    TArrayF* GetTwimCalParams() { return fAnodeCalParams; }

    void SetNumSec(Int_t numberSections) { fNumSections = numberSections; }
    void SetNumAnodes(Int_t numberAnodes) { fNumAnodes = numberAnodes; }
    void SetNumParametersFit(Int_t numberParams) { fNumParamsFit = numberParams; }
    void SetTwimCalParams(Double_t cc, Int_t ii) { fAnodeCalParams->AddAt(cc, ii); }

    // Create more Methods if you need them!

  private:
    TArrayF* fAnodeCalParams; // Calibration Parameters for anodes
    Int_t fNumSections;       // number of sections
    Int_t fNumAnodes;         // number of anodes
    Int_t fNumParamsFit;      /* number of cal parameters in the fit
                                 gaus: A_fit & B_fit & C_fit*/

    const R3BSofTwimCalPar& operator=(const R3BSofTwimCalPar&); /*< an assignment operator>*/

    R3BSofTwimCalPar(const R3BSofTwimCalPar&); /*< a copy constructor >*/

    ClassDef(R3BSofTwimCalPar, 1);
};

#endif
