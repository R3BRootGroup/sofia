#ifndef __R3BSOFSCIRAWPOSPAR_H__
#define __R3BSOFSCIRAWPOSPAR_H__

#include "FairParGenericSet.h"   // for FairParGenericSet
#include "TArrayF.h"
#include "TObjArray.h"
#include "TObject.h"

#include <TObjString.h>

class FairParamList;

class R3BSofSciRawPosPar : public FairParGenericSet
{

  public:
    /** Standard constructor **/
    R3BSofSciRawPosPar(const char* name = "SofSciRawPosPar",
                       const char* title = "Sof Sci RawPos Parameters",
                       const char* context = "SofSciPosParContext");

    /** Destructor **/
    virtual ~R3BSofSciRawPosPar();

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
    const Int_t GetNumPmts() { return fNumPmts; }
    const Int_t GetNumSignals() { return fNumSignals; }
    const Int_t GetNumParsPerSignal() { return fNumParsPerSignal; }
    TArrayF* GetAllSignalsAllParams() { return fAllRawPosParams; }
    Double_t GetParam(UInt_t rank) { return (Double_t)fAllRawPosParams->GetAt(rank); }

    void SetNumDets(Int_t ndets) { fNumDets = ndets; }
    void SetNumPmts(Int_t nchs) { fNumPmts = nchs; }
    void SetNumSignals(Int_t ndets) { fNumSignals = ndets; }
    void SetNumParsPerSignal(Int_t npars) { fNumParsPerSignal = npars; }
    void SetParam(Double_t parval, UInt_t rank) { fAllRawPosParams->AddAt(parval, rank); }

    /** Create more Methods if you need them! **/

  private:
    TArrayF* fAllRawPosParams;   // Calibration Parameters for all signals of one detector
    Int_t fNumDets;              // number of detectors
    Int_t fNumPmts;              // number of channels of SofSciTcal data = 3 (need for Tcal2SingleTcal)
    Int_t fNumSignals;           // =fNumDets if RawPos
    Int_t fNumParsPerSignal;
    const R3BSofSciRawPosPar& operator=(const R3BSofSciRawPosPar&); /*< an assignment operator>*/

    R3BSofSciRawPosPar(const R3BSofSciRawPosPar&);   // a copy constructor

    ClassDef(R3BSofSciRawPosPar, 1);
};

#endif   //__R3BSOFSCIRAWPOSPAR_H__
