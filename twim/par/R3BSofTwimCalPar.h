// ------------------------------------------------------------------
// -----             R3BSofTwimCalPar source file               -----
// -----         Created 24/11/19  by J.L. Rodriguez-Sanchez    -----
// ------------------------------------------------------------------

#ifndef R3BSofTwimCalPar_H
#define R3BSofTwimCalPar_H

#include "FairParGenericSet.h"
#include "TObject.h"

#include "TArrayF.h"
#include "TArrayI.h"
#include "TObjArray.h"
#include <TObjString.h>

class FairParamList;

class R3BSofTwimCalPar : public FairParGenericSet
{

  public:
    /** Standard constructor **/
    R3BSofTwimCalPar(const char* name = "twimCalPar",
                     const char* title = "TWIM Cal Parameters",
                     const char* context = "TwimCalParContext");

    /** Destructor **/
    virtual ~R3BSofTwimCalPar();

    /** Method to reset all parameters **/
    virtual void clear();

    /** Method to store all parameters using FairRuntimeDB **/
    virtual void putParams(FairParamList* list);

    /** Method to retrieve all parameters using FairRuntimeDB**/
    Bool_t getParams(FairParamList* list);

    /** Method to print values of parameters to the standard output **/
    virtual void print();
    void printParams();

    /** Accessor functions **/
    const Int_t GetNumSec() { return fNumSections; }
    const Int_t GetNumAnodes() { return fNumAnodes; }
    const Int_t GetNumParamsEFit() { return fNumParamsEFit; }
    const Int_t GetNumParamsPosFit() { return fNumParamsPosFit; }
    const Int_t GetInUse(Int_t sec, Int_t anode) { return fIn_use->GetAt((sec - 1) * 16 + anode - 1); }
    TArrayF* GetAnodeCalParams() { return fAnodeCalParams; }
    TArrayF* GetPosParams() { return fPosParams; }

    void SetNumSec(Int_t numberSections) { fNumSections = numberSections; }
    void SetNumAnodes(Int_t numberAnodes) { fNumAnodes = numberAnodes; }
    void SetNumParamsEFit(Int_t numberParams) { fNumParamsEFit = numberParams; }
    void SetNumParamsPosFit(Int_t numberParams) { fNumParamsPosFit = numberParams; }
    void SetInUse(Int_t value, Int_t sec, Int_t anode) { fIn_use->AddAt(value, (sec - 1) * 16 + anode - 1); }
    void SetAnodeCalParams(Float_t cc, Int_t ii) { fAnodeCalParams->AddAt(cc, ii); }
    void SetPosParams(Float_t cc, Int_t ii) { fPosParams->AddAt(cc, ii); }

    // Create more Methods if you need them!

  private:
    TArrayF* fAnodeCalParams; // Energy calibration parameters for anodes
    TArrayF* fPosParams;      // Position calibration parameters for anodes
    TArrayI* fIn_use;         // 1: anode ready, 0:otherwise
    Int_t fNumSections;       // number of sections
    Int_t fNumAnodes;         // number of anodes
    Int_t fNumParamsEFit;     /* number of cal parameters in the fit
                                gaus: A_fit & B_fit & C_fit*/
    Int_t fNumParamsPosFit;   // number of parameters for position calibration

    const R3BSofTwimCalPar& operator=(const R3BSofTwimCalPar&);
    R3BSofTwimCalPar(const R3BSofTwimCalPar&);

  public:
    ClassDef(R3BSofTwimCalPar, 1);
};

#endif /* R3BSofTwimCalPar_H */
