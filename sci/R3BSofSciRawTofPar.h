#ifndef __R3BSOFSCIRAWTOFPAR_H__
#define __R3BSOFSCIRAWTOFPAR_H__ 1

#include "FairParGenericSet.h" // for FairParGenericSet
#include "TArrayF.h"
#include "TObjArray.h"
#include "TObject.h"

#include <TObjString.h>

class FairParamList;

class R3BSofSciRawTofPar : public FairParGenericSet
{

  public:
    /** Standard constructor **/
    R3BSofSciRawTofPar(const char* name = "SofSciRawTofPar",
                       const char* title = "Sof Sci RawTof Parameters",
                       const char* context = "SofSciTofParContext");

    /** Destructor **/
    virtual ~R3BSofSciRawTofPar();

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
    const Int_t GetNumChannels() { return fNumChannels; }
    const Int_t GetDetIdCaveC() { return fDetIdCaveC; }
    const Int_t GetDetIdS2() { return fDetIdS2; }
    const Int_t GetDetIdS8() { return fDetIdS8; }
    const Int_t GetNumSignals() { return fNumSignals; }
    const Int_t GetNumParsPerSignal() { return fNumParsPerSignal; }
    TArrayF* GetAllSignalsRawTofParams() { return fAllSignalsRawTofParams; }
    Double_t GetSignalRawTofParams(Int_t rank) { return (Double_t)fAllSignalsRawTofParams->GetAt(rank); }

    /**   **/

    void SetNumDets(Int_t ndets) { fNumDets = ndets; }
    void SetNumChannels(Int_t nchs) { fNumChannels = nchs; }
    void SetDetIdCaveC(Int_t id) { fDetIdCaveC = id; }
    void SetDetIdS2(Int_t id) { fDetIdS2 = id; }
    void SetDetIdS8(Int_t id) { fDetIdS8 = id; }
    void SetNumSignals(Int_t nsig) { fNumSignals = nsig; }
    void SetNumParsPerSignal(Int_t npars) { fNumParsPerSignal = npars; }
    void SetSignalParams(Double_t parval, Int_t rank) { fAllSignalsRawTofParams->AddAt(parval, rank); }

  private:
    TArrayF* fAllSignalsRawTofParams; // Calibration Parameters for all signals of one detector
    Int_t fNumDets;
    Int_t fNumChannels;
    Int_t fDetIdS2;
    Int_t fDetIdS8;
    Int_t fDetIdCaveC;
    Int_t fNumSignals;       // = number of Tof spectra from det to det@cave C = fNumDets-1
    Int_t fNumParsPerSignal; // = 2
    const R3BSofSciRawTofPar& operator=(const R3BSofSciRawTofPar&);
    R3BSofSciRawTofPar(const R3BSofSciRawTofPar&); // a copy constructor

    ClassDef(R3BSofSciRawTofPar, 1);
};

#endif //__R3BSOFSCIRAWTOFPAR_H__
