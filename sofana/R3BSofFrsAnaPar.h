// ------------------------------------------------------------------
// -----         R3BSofFrsAnaPar source file                    -----
// -----         Created 27/01/20  by J.L. Rodriguez-Sanchez    -----
// -----         Revised 07/08/20  by R. Taniuchi               -----
// ------------------------------------------------------------------

#ifndef R3BSofFrsAnaPar_H
#define R3BSofFrsAnaPar_H

#include "TString.h"
#include "TArrayF.h"
#include "TArrayI.h"
#include "TObject.h"
#include <iostream>

#include "FairParGenericSet.h"
#include "FairLogger.h"
#include "FairParamList.h"

class FairParamList;

class R3BSofFrsAnaPar : public FairParGenericSet
{
  public:
    /** Standard constructor **/
    R3BSofFrsAnaPar(const TString& name = "soffrsAnaPar",
                    const TString& title = "FRS-S2-Cave-C Parameters",
                    const TString& context = "SofFRSParContext");

    /** Destructor **/
    virtual ~R3BSofFrsAnaPar();

    /** Method to reset all parameters **/
    virtual void clear();

    /** Method to store all parameters using FairRuntimeDB **/
    virtual void putParams(FairParamList* list);

    /** Method to retrieve all parameters using FairRuntimeDB**/
    Bool_t getParams(FairParamList* list);

    /** Method to print values of parameters to the standard output **/
    void printParams();

    /** Accessor functions **/
    const Float_t GetBrho() { return fBrho0; }
    const Int_t GetNumTof() { return fNumTof; }
    const Int_t GetStaSciId(Int_t i) { return fStaSciId->GetAt(i); }
    const Int_t GetStoSciId(Int_t i) { return fStoSciId->GetAt(i); }
    const Float_t GetPathLength(Int_t i) { return fPathLength->GetAt(i); }
    const Float_t GetTofOffset(Int_t i) { return fTofOffset->GetAt(i); }
    const Int_t GetUseS2x(Int_t i) { return fUseS2x->GetAt(i); }
    const Float_t GetS2PosCoef() { return fS2PosCoef; }
    const Float_t GetS2PosOffset() { return fS2PosOffset; }

    void SetBrho(Float_t brho) { fBrho0 = brho; }
    void SetNumTof(Int_t num) { fNumTof = num; }
    void SetStaSciId(Int_t i, Int_t val) { fStaSciId->AddAt(val, i); }
    void SetStoSciId(Int_t i, Int_t val) {  fStoSciId->AddAt(val, i); }
    void SetPathLength(Int_t i, Float_t val) {  fPathLength->AddAt(val, i); }
    void SetTofOffset(Int_t i, Float_t val) { fTofOffset->AddAt(val, i); }
    void SetUseS2x(Int_t i, Int_t val) { fUseS2x->AddAt(val, i); }
    void SetS2PosCoef(Float_t val) { fS2PosCoef = val; }
    void SetS2PosOffset(Float_t val) { fS2PosOffset = val; }
    /** Create more Methods if you need them! **/

  private:
    Float_t fBrho0;
    Int_t fNumTof;
    TArrayI* fStaSciId;
    TArrayI* fStoSciId;
    TArrayF* fPathLength;
    TArrayF* fTofOffset;
    TArrayI* fUseS2x;
    Float_t fS2PosCoef;
    Float_t fS2PosOffset;
    const R3BSofFrsAnaPar& operator=(const R3BSofFrsAnaPar&); /*< an assignment operator>*/

    R3BSofFrsAnaPar(const R3BSofFrsAnaPar&); /*< a copy constructor >*/

    ClassDef(R3BSofFrsAnaPar, 1);
};

#endif
