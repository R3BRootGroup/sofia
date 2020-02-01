// --------------------------------------------------------------
// -----          R3BSofTwimHitPar source file              -----
// -----     Created 24/11/19  by J.L. Rodriguez-Sanchez    -----
// --------------------------------------------------------------

#ifndef R3BSofTwimHitPar_H
#define R3BSofTwimHitPar_H

#include "FairParGenericSet.h" // for FairParGenericSet

#include "TArrayF.h"
#include "TArrayI.h"
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
    const Int_t GetNumParZFit() { return fNumParamsZFit; }
    const Int_t GetNumAnodes() { return fNumAnodes; }
    const Int_t GetInUse(Int_t sec, Int_t anode) { return fIn_use->GetAt((sec - 1) * 16 + anode - 1); }
    const Float_t GetAnodePos(Int_t anode) { return fAnode_pos->GetAt(anode - 1); }
    TArrayF* GetZHitPar() { return fDetZHitParams; }

    void SetNumSec(Int_t nbsec) { fNumSec = nbsec; }
    void SetNumAnodes(Int_t nbAnodes) { fNumAnodes = nbAnodes; }
    void SetNumParZFit(Int_t nbParams) { fNumParamsZFit = nbParams; }
    void SetInUse(Int_t value, Int_t anode) { fIn_use->AddAt(value, anode - 1); }
    void SetZHitPar(Double_t cc, Int_t ii) { fDetZHitParams->AddAt(cc, ii); }
    void SetAnodePos(Float_t value, Int_t anode) { fAnode_pos->AddAt(value, anode - 1); }

    // Create more Methods if you need them!

  private:
    TArrayF* fDetZHitParams; // Calibration Parameters for charge Z
    TArrayI* fIn_use;        // 1: anode ready, 0:otherwise
    TArrayF* fAnode_pos;     // Position of each anode along the beam direction
    Int_t fNumSec;
    Int_t fNumAnodes;     // Number of anodes
    Int_t fNumParamsZFit; // number of hit parameters in the fit for charge Z

    const R3BSofTwimHitPar& operator=(const R3BSofTwimHitPar&); /*< an assignment operator>*/

    R3BSofTwimHitPar(const R3BSofTwimHitPar&); /*< a copy constructor >*/

    ClassDef(R3BSofTwimHitPar, 1);
};

#endif
