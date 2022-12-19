#ifndef __R3BSOFSCICALPOSPAR_H__
#define __R3BSOFSCICALPOSPAR_H__

#include "FairParGenericSet.h" // for FairParGenericSet
#include "TArrayF.h"
#include "TObjArray.h"
#include "TObject.h"

#include <TObjString.h>

class FairParamList;

class R3BSofSciCalPosPar : public FairParGenericSet
{

  public:
    /** Standard constructor **/
    R3BSofSciCalPosPar(const char* name = "SofSciCalPosPar",
                       const char* title = "Sof Sci CalPos Parameters",
                       const char* context = "SofSciCalPosParContext");

    /** Destructor **/
    virtual ~R3BSofSciCalPosPar();

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
    const Int_t GetNumParamsPerDet() { return fNumParamsPerDet; }
    const Int_t GetNumParams() { return fNumParams; }
    TArrayF* GetAllParams() { return fAllParams; }
    Float_t GetParam(Int_t rank) { return (Float_t)fAllParams->GetAt(rank); }

    void SetNumDets(Int_t ndets) { fNumDets = ndets; }
    void SetNumParamsPerDet(Int_t nparsperdet) { fNumParamsPerDet = nparsperdet; }
    void SetNumParams(Int_t npars) { fNumParams = npars; }
    void SetParam(Double_t parval, UInt_t rank) { fAllParams->AddAt(parval, rank); }

    /** Create more Methods if you need them! **/

  private:
    Int_t fNumDets;         // number of detectors
    Int_t fNumParamsPerDet; // if pol1: 2 parameters for each detector
    Int_t fNumParams;       // = fNumDets * fNumParsPerDet
    TArrayF* fAllParams;

    const R3BSofSciCalPosPar& operator=(const R3BSofSciCalPosPar&); /*< an assignment operator>*/

    R3BSofSciCalPosPar(const R3BSofSciCalPosPar&); // a copy constructor

    ClassDef(R3BSofSciCalPosPar, 1);
};

#endif //__R3BSOFSCICALPOSPAR_H__
