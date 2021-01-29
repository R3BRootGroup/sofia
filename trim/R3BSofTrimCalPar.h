// ------------------------------------------------------------------
// -----             R3BSofTrimCalPar source file               -----
// ------------------------------------------------------------------

#ifndef R3BSofTrimCalPar_H
#define R3BSofTrimCalPar_H

#include "FairParGenericSet.h" // for FairParGenericSet

#include "TArrayD.h"
#include "TArrayF.h"
#include "TObjArray.h"
#include "TObject.h"
#include <TObjString.h>

class FairParamList;

class R3BSofTrimCalPar : public FairParGenericSet
{

  public:
    /** Standard constructor **/
    R3BSofTrimCalPar(const char* name = "trimCalPar",
                     const char* title = "Triple MUSIC Cal Parameters",
                     const char* context = "TrimCalParContext");

    /** Destructor **/
    virtual ~R3BSofTrimCalPar();

    /** Method to reset all parameters **/
    virtual void clear();

    /** Method to store all parameters using FairRuntimeDB **/
    virtual void putParams(FairParamList* list);

    /** Method to retrieve all parameters using FairRuntimeDB**/
    Bool_t getParams(FairParamList* list);

    /** Method to print values of parameters to the standard output **/
    void printParams();

    /** Accessor functions **/
    const Int_t GetNumSections() { return fNumSections; }
    const Int_t GetNumAnodes() { return fNumAnodes; }
    Double_t GetDriftTimeOffset(Int_t section, Int_t anode)
    {
        return fDriftTimeOffsets->GetAt((anode - 1) + (section - 1) * 6);
    }
    Float_t GetEnergyPedestal(Int_t section, Int_t anode)
    {
        return fEnergyPedestals->GetAt((anode - 1) + (section - 1) * 6);
    }
    Float_t GetEnergyMatchGain(Int_t section, Int_t anode)
    {
        return fEnergyMatchGains->GetAt((anode - 1) + (section - 1) * 6);
    }
    TArrayD* GetDriftTimeOffsets() { return fDriftTimeOffsets; }
    TArrayF* GetEnergyPedestals() { return fEnergyPedestals; }
    TArrayF* GetEnergyMatchGains() { return fEnergyMatchGains; }

    void SetNumSections(Int_t num) { fNumSections = num; }
    void SetNumAnodes(Int_t num) { fNumAnodes = num; }
    void SetDriftTimeOffset(Double_t val, Int_t section, Int_t anode)
    {
        fDriftTimeOffsets->AddAt(val, (anode - 1) + (section - 1) * 6);
    }
    void SetEnergyPedestal(Float_t val, Int_t section, Int_t anode)
    {
        fEnergyPedestals->AddAt(val, (anode - 1) + (section - 1) * 6);
    }
    void SetEnergyMatchGain(Float_t val, Int_t section, Int_t anode)
    {
        fEnergyMatchGains->AddAt(val, (anode - 1) + (section - 1) * 6);
    }

    // Create more Methods if you need them!

  private:
    Int_t fNumSections;         // number of sections
    Int_t fNumAnodes;           // number of anodes per section
    TArrayD* fDriftTimeOffsets; // Drift Time offset to align drift time for a similar position
    TArrayF* fEnergyPedestals;  // Energy pedestals
    TArrayF* fEnergyMatchGains; // Gain matching of the triangular pair of anodes

    const R3BSofTrimCalPar& operator=(const R3BSofTrimCalPar&); /*< an assignment operator>*/

    R3BSofTrimCalPar(const R3BSofTrimCalPar&); /*< a copy constructor >*/

    ClassDef(R3BSofTrimCalPar, 1);
};

#endif
