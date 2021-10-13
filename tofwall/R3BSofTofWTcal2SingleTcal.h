// *** *************************************************************** *** //
// ***                  R3BSofTofWTcal2SingleTcal                        *** //
// *** *************************************************************** *** //

#ifndef R3BSOFTOFW_TCAL2SINGLETCAL
#define R3BSOFTOFW_TCAL2SINGLETCAL

#include "FairTask.h"

#include "R3BSofSciRawTofPar.h"

#include "TClonesArray.h"
#include "TMath.h"
#include "TRandom.h"
class TRandom3;

#include "R3BSofSciSingleTcalData.h"
#include "R3BSofTofWSingleTcalData.h"
#include "R3BSofTofWTcalData.h"

class R3BSofTofWTcal2SingleTcal : public FairTask
{

  public:
    // --- Default constructor --- //
    R3BSofTofWTcal2SingleTcal();
    R3BSofTofWTcal2SingleTcal(Int_t numPaddles, Int_t numPmts);

    // --- Standard constructor --- //
    R3BSofTofWTcal2SingleTcal(const char* name, Int_t iVerbose = 1);

    // --- Destructor --- //
    virtual ~R3BSofTofWTcal2SingleTcal();

    virtual InitStatus Init();

    virtual void SetParContainers();

    virtual InitStatus ReInit();

    virtual void Exec(Option_t* option);

    /** Virtual method Reset **/
    virtual void Reset();

    /** Virtual method Finish **/
    virtual void Finish();

    void SetOnline(Bool_t option) { fOnline = option; }

    void SetNumPaddles(Int_t n) { fNumPaddles = n; }
    void SetNumPmts(Int_t n) { fNumPmts = n; }

  private:
    TClonesArray* fSciSingleTcal;      // input data
    TClonesArray* fTofWTcal;           // input data
    TClonesArray* fTofWSingleTcal;     // output data
    R3BSofSciRawTofPar* fSciRawTofPar; // needed to get the Cave C Sci ID

    Bool_t fOnline; // Don't store data for online

    UInt_t fNumSingleTcal;
    UInt_t fNevent;
    Int_t fNumPaddles;
    Int_t fNumPmts;

    TRandom rand;

    R3BSofTofWSingleTcalData* AddHitData(Int_t plastic, Double_t time, Double_t tof, Double_t pos);

  public:
    ClassDef(R3BSofTofWTcal2SingleTcal, 1)
};

#endif // R3BSOFTOFW_TCAL2SINGLETCAL
