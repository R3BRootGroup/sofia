#ifndef __R3BSofSciCalTofPar_H__
#define __R3BSofSciCalTofPar_H__

#include "FairParGenericSet.h" // for FairParGenericSet
#include "TArrayF.h"
#include "TObjArray.h"
#include "TObject.h"
#include <TObjString.h>

class FairParamList;

class R3BSofSciCalTofPar : public FairParGenericSet
{

  public:
    /** Standard constructor **/
    R3BSofSciCalTofPar(const char* name = "SofSciCalTofPar",
                       const char* title = "Sof Sci CalTof Parameters",
                       const char* context = "SofSciTofParContext");

    /** Destructor **/
    virtual ~R3BSofSciCalTofPar();

    /** Method to reset all parameters **/
    virtual void clear();

    /** Method to store all parameters using FairRuntimeDB **/
    virtual void putParams(FairParamList* list);

    /** Method to retrieve all parameters using FairRuntimeDB**/
    Bool_t getParams(FairParamList* list);

    /** Method to print values of parameters to the standard output **/
    void printParams();

    /** Accessor functions **/
    const Int_t GetNumDets() { return fNumDets; }
    const Int_t GetDetIdCaveC() { return fDetIdCaveC; }
    const Int_t GetDetIdS2() { return fDetIdS2; }
    const Int_t GetDetIdS8() { return fDetIdS8; }

    TArrayF* GetAllParams_Tof2InvV_FromS2() { return fTof2InvV_FromS2; }
    TArrayF* GetAllParams_Tof2InvV_FromS8() { return fTof2InvV_FromS8; }
    TArrayF* GetAllParams_FlightLength_FromS2() { return fFlightLength_FromS2; }
    TArrayF* GetAllParams_FlightLength_FromS8() { return fFlightLength_FromS8; }

    Float_t GetTof2InvV_FromS2(Int_t rank) { return (Float_t)fTof2InvV_FromS2->GetAt(rank); };
    Float_t GetTof2InvV_FromS8(Int_t rank) { return (Float_t)fTof2InvV_FromS8->GetAt(rank); };
    Float_t GetFlightLength_FromS2(Int_t rank) { return (Float_t)fFlightLength_FromS2->GetAt(rank); }
    Float_t GetFlightLength_FromS8(Int_t rank) { return (Float_t)fFlightLength_FromS8->GetAt(rank); }

    void SetNumDets(Int_t ndets) { fNumDets = ndets; }
    void SetDetIdCaveC(Int_t id) { fDetIdCaveC = id; }
    void SetDetIdS2(Int_t id) { fDetIdS2 = id; }
    void SetDetIdS8(Int_t id) { fDetIdS8 = id; }

    void SetTof2InvV_FromS2(Double_t parval, UInt_t rank) { fTof2InvV_FromS2->AddAt(parval, rank); }
    void SetTof2InvV_FromS8(Double_t parval, UInt_t rank) { fTof2InvV_FromS8->AddAt(parval, rank); }
    void SetFlightLength_FromS2(Double_t length, UInt_t rank) { fFlightLength_FromS2->AddAt(length, rank); }
    void SetFlightLength_FromS8(Double_t length, UInt_t rank) { fFlightLength_FromS8->AddAt(length, rank); }

  private:
    Int_t fNumDets; // number of detectors
    Int_t fDetIdS2;
    Int_t fDetIdS8;
    Int_t fDetIdCaveC;
    TArrayF* fTof2InvV_FromS2;     // two parameter per Tof
    TArrayF* fTof2InvV_FromS8;     // two parameter per Tof
    TArrayF* fFlightLength_FromS2; // one parameter per detector
    TArrayF* fFlightLength_FromS8; // one parameter per detector

    const R3BSofSciCalTofPar& operator=(const R3BSofSciCalTofPar&); /*< an assignment operator>*/

    R3BSofSciCalTofPar(const R3BSofSciCalTofPar&); // a copy constructor

    ClassDef(R3BSofSciCalTofPar, 1);
};

#endif //__R3BSofSciCalTofPar_H__
