// *** *************************************************************** *** //
// ***                  R3BSofSciTcal2SingleTcal                        *** //
// *** ---> whatever multiplicity per PMT, select the propoer hit      *** //
// *** *************************************************************** *** //

#ifndef R3BSOFSCI_TCAL2SINGLETCAL
#define R3BSOFSCI_TCAL2SINGLETCAL

#include "FairTask.h"

#include "R3BSofSciRawPosPar.h"
#include "R3BSofSciRawTofPar.h"
#include "R3BSofSciSingleTcalData.h"

#include "TClonesArray.h"
#include "TMath.h"
#include "TRandom.h"
class TRandom3;

class R3BSofSciTcal2SingleTcal : public FairTask
{

  public:
    // --- Default constructor --- //
    R3BSofSciTcal2SingleTcal();

    // --- Standard constructor --- //
    R3BSofSciTcal2SingleTcal(const char* name, Int_t iVerbose = 1);

    // --- Destructor --- //
    virtual ~R3BSofSciTcal2SingleTcal();

    virtual InitStatus Init();
    virtual void SetParContainers();
    virtual InitStatus ReInit();
    virtual void Exec(Option_t* option);
    virtual void FinishEvent();
    virtual void FinishTask();

    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    TClonesArray* fTcal;
    TClonesArray* fSingleTcal;
    R3BSofSciRawPosPar* fRawPosPar;
    R3BSofSciRawTofPar* fRawTofPar;

    Bool_t fOnline; // Don't store data for online

    UInt_t fNumSingleTcal;

    UInt_t fNevent;

    TRandom rand;

    R3BSofSciSingleTcalData* AddSingleTcalData(UShort_t iDet, Double_t traw, Double_t posraw, Double_t tofrawS2, Double_t tofrawS8);

  public:
    ClassDef(R3BSofSciTcal2SingleTcal, 1)
};

#endif // R3BSOFSCI_TCAL2SINGLETCAL
