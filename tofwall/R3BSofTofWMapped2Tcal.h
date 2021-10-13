// *** *************************************************************** *** //
// ***                  R3BSofTofWMapped2Tcal                          *** //
// *** convert Mapped data to tcal data :                              *** //
// *** ---> from the fine and coarse times, calculate a raw time in ns *** //
// *** *************************************************************** *** //

#ifndef R3BSOFTOFW_MAPPED2TCAL_H
#define R3BSOFTOFW_MAPPED2TCAL_H

#include "FairTask.h"

#include "R3BSofTofWTcalData.h"

// ROOT headers
#include "TClonesArray.h"
#include "TMath.h"
#include "TRandom3.h"

class TRandom3;
class R3BSofTcalPar;

class R3BSofTofWMapped2Tcal : public FairTask
{

  public:
    // --- Default constructor --- //
    R3BSofTofWMapped2Tcal();

    // --- Standard constructor --- //
    R3BSofTofWMapped2Tcal(const char* name, Int_t iVerbose = 1);

    // --- Destructor --- //
    virtual ~R3BSofTofWMapped2Tcal();

    virtual void Exec(Option_t* option);

    virtual void SetParContainers();

    virtual InitStatus Init();

    virtual InitStatus ReInit();

    virtual void Finish();

    /** Virtual method Reset **/
    virtual void Reset();

    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    Bool_t fOnline; // Don't store data for online

    TClonesArray* fMapped; // input data - SofTofWMappedData
    TClonesArray* fTcal;   // output data

    R3BSofTcalPar* fTcalPar; // tcal parameters container

    UInt_t fNumTcal; // number of Tcal items per event

    UInt_t fNevent;
    TRandom3 rand;

    Double_t CalculateTimeNs(UShort_t det, UShort_t pmt, UInt_t tf, UInt_t tc);

    /** Private method AddTCalData **/
    R3BSofTofWTcalData* AddTCalData(UShort_t detector, UShort_t pmt, Double_t t);

  public:
    ClassDef(R3BSofTofWMapped2Tcal, 1)
};

#endif // R3BSOFTOFW_MAPPED2TCAL_H
