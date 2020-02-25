// -----------------------------------------------------------------
// -----         R3BSofSciSingleTCal2Hit source file           -----
// -----    Created 24/02/20  by J.L. Rodriguez-Sanchez        -----
// -----------------------------------------------------------------

#ifndef R3BSofSciSingleTCal2Hit_H
#define R3BSofSciSingleTCal2Hit_H

#include "FairTask.h"
#include "R3BSofSciHitData.h"
#include "R3BSofSciSingleTcalData.h"
#include <TRandom.h>

class TClonesArray;

class R3BSofSciSingleTCal2Hit : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofSciSingleTCal2Hit();

    /** Standard constructor **/
    R3BSofSciSingleTCal2Hit(const char* name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofSciSingleTCal2Hit();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* option);

    /** Virtual method Reset **/
    virtual void Reset();

    // Fair specific
    /** Virtual method Init **/
    virtual InitStatus Init();

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

    TClonesArray* fTCalDataCA; /**< Array with Cal input data. >*/
    TClonesArray* fHitDataCA;  /**< Array with Hit output data. >*/

    /** Private method AddHitData **/
    // Adds a SofSciHitData to the HitCollection
    R3BSofSciHitData* AddHitData(Int_t sci, Double_t x, Double_t tof);

  public:
    // Class definition
    ClassDef(R3BSofSciSingleTCal2Hit, 1)
};

#endif
