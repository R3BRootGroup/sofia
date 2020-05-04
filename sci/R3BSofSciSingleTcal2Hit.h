// -----------------------------------------------------------------
// -----         R3BSofSciSingleTCal2Hit source file           -----
// -----    Created 24/02/20  by J.L. Rodriguez-Sanchez        -----
// -----------------------------------------------------------------

#ifndef R3BSofSciSingleTCal2Hit_H
#define R3BSofSciSingleTCal2Hit_H

#include "FairTask.h"
#include "R3BSofSciHitData.h"
#include "R3BSofSciSingleTcalData.h"
#include "R3BSofSciRawTofPar.h"
#include <TRandom.h>

class TClonesArray;

class R3BSofSciSingleTcal2Hit : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofSciSingleTcal2Hit();

    /** Standard constructor **/
    R3BSofSciSingleTcal2Hit(const char* name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofSciSingleTcal2Hit();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* option);

    /** Virtual method Reset **/
    virtual void Reset();

    // Fair specific
    /** Virtual method Init **/
    virtual InitStatus Init();

    virtual void SetParContainers();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    /** Virtual method Finish **/
    virtual void Finish();

    void SetOnline(Bool_t option) { fOnline = option; }

    void SetCalParams(Double_t tof, Double_t offsetTof)
    {
        fTof = tof;
        fOffsetTof = offsetTof;
    }

  private:
    Bool_t fOnline; // Don't store data for online
    Double_t fTof;
    Double_t fOffsetTof;

    TClonesArray* fSingleTcalDataCA; /**< Array with SingleTcal input data. >*/
    TClonesArray* fHitDataCA;  /**< Array with Hit output data. >*/
    R3BSofSciRawTofPar* fRawTofPar;

    /** Private method AddHitData **/
    // Adds a SofSciHitData to the HitCollection
    R3BSofSciHitData* AddHitData(Int_t sci, Double_t x, Double_t tof);

  public:
    // Class definition
    ClassDef(R3BSofSciSingleTcal2Hit, 1)
};

#endif
