// ------------------------------------------------------------------
// -----         R3BSofFragmentAnaPar source file               -----
// -----         Created 17/12/20  by R. Taniuchi               -----
// ------------------------------------------------------------------

#ifndef R3BSofFragmentAnaPar_H
#define R3BSofFragmentAnaPar_H

#include "TString.h"
#include "TArrayF.h"
#include "TArrayI.h"
#include "TObject.h"
#include <iostream>

#include "FairParGenericSet.h"
#include "FairLogger.h"
#include "FairParamList.h"

class FairParamList;

class R3BSofFragmentAnaPar : public FairParGenericSet
{
  public:
    /** Standard constructor **/
    R3BSofFragmentAnaPar(const TString& name = "soffragmentAnaPar",
                    const TString& title = "Fragment in Cave-C Parameters",
                    const TString& context = "SofFagmentParContext");

    /** Destructor **/
    virtual ~R3BSofFragmentAnaPar();

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
    const Int_t GetInUse(Int_t sci) { return fIn_use->GetAt(sci - 1); }
    const Float_t GetTofWOffset(Int_t sci) { return fTofW_Offset->GetAt(sci - 1); }
    const Float_t GetEffectivLength(Int_t sci) { return fEffective_Length->GetAt(sci - 1); }
    
    void SetNumSci(Int_t nb) { fNumSci = nb; }
    void SetInUse(Int_t value, Int_t sci) { fIn_use->AddAt(value, sci - 1); }
    void SetTofWOffset(Float_t value, Int_t sci) { fTofW_Offset->AddAt(value, sci - 1); }
    void SetEffectivLength(Float_t value, Int_t sci) { fEffective_Length->AddAt(value, sci - 1); }

    /** Create more Methods if you need them! **/

  private:
    Int_t fNumSci;
    TArrayI* fIn_use;  // 1: in use, 0:otherwise
    TArrayF* fTofW_Offset;
    TArrayF* fEffective_Length;
    const R3BSofFragmentAnaPar& operator=(const R3BSofFragmentAnaPar&); /*< an assignment operator>*/

    R3BSofFragmentAnaPar(const R3BSofFragmentAnaPar&); /*< a copy constructor >*/

    ClassDef(R3BSofFragmentAnaPar, 1);
};

#endif
