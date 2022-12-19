// --------------------------------------------------------------
// -----          R3BSofTofWHitPar source file              -----
// -----     Created 20/02/20  by J.L. Rodriguez-Sanchez    -----
// --------------------------------------------------------------

#ifndef R3BSofTofWHitPar_H
#define R3BSofTofWHitPar_H

#include "FairParGenericSet.h" // for FairParGenericSet
#include "TArrayF.h"
#include "TArrayI.h"
#include "TObjArray.h"
#include "TObject.h"

#include <TObjString.h>

class FairParamList;

class R3BSofTofWHitPar : public FairParGenericSet
{
  public:
    /** Standard constructor **/
    R3BSofTofWHitPar(const char* name = "tofwHitPar",
                     const char* title = "TofW Hit Parameters",
                     const char* context = "TofWHitParContext");

    /** Destructor **/
    virtual ~R3BSofTofWHitPar();

    /** Method to reset all parameters **/
    virtual void clear();

    /** Method to store all parameters using FairRuntimeDB **/
    virtual void putParams(FairParamList* list);

    /** Method to retrieve all parameters using FairRuntimeDB**/
    Bool_t getParams(FairParamList* list);

    /** Method to print values of parameters to the standard output **/
    void printParams();

    /** Accessor functions **/
    const Int_t GetNumSci() { return fNumSci; }
    const Float_t GetTofAlign() { return fTofAlign; }
    const Int_t GetInUse(Int_t sci) { return fIn_use->GetAt(sci - 1); }
    const Float_t GetTofPar(Int_t sci) { return fSci_tof->GetAt(sci - 1); }
    const Float_t GetPosOffsetPar(Int_t sci) { return fSci_posoffset->GetAt(sci - 1); }
    const Float_t GetPosSlopePar(Int_t sci) { return fSci_posslope->GetAt(sci - 1); }

    void SetNumSci(Int_t nb) { fNumSci = nb; }
    void SetTofAlign(Float_t f) { fTofAlign = f; }
    void SetInUse(Int_t value, Int_t sci) { fIn_use->AddAt(value, sci - 1); }
    void SetTofPar(Float_t value, Int_t sci) { fSci_tof->AddAt(value, sci - 1); }
    void SetPosOffsetPar(Float_t value, Int_t sci) { fSci_posoffset->AddAt(value, sci - 1); }
    void SetPosSlopePar(Float_t value, Int_t sci) { fSci_posslope->AddAt(value, sci - 1); }

    // Create more Methods if you need them!

  private:
    TArrayF* fSci_tof;       // Calibration Parameters for tof
    TArrayF* fSci_posoffset; // Offset Calibration Parameters for pos
    TArrayF* fSci_posslope;  // Slope Calibration Parameters for pos
    TArrayI* fIn_use;        // 1: in use, 0:otherwise
    Float_t fTofAlign;       // Tof alignment
    Int_t fNumSci;

    const R3BSofTofWHitPar& operator=(const R3BSofTofWHitPar&); /*< an assignment operator>*/

    R3BSofTofWHitPar(const R3BSofTofWHitPar&); /*< a copy constructor >*/

  public:
    ClassDef(R3BSofTofWHitPar, 2);
};

#endif /* R3BSofTofWHitPar_H */
