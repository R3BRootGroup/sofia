// -----------------------------------------------------------------
// -----         R3BSofTofWTCal2Hit source file                -----
// -----    Created 15/02/20  by J.L. Rodriguez-Sanchez        -----
// -----------------------------------------------------------------

#ifndef R3BSofTofWTCal2Hit_H
#define R3BSofTofWTCal2Hit_H

#include "FairTask.h"
#include "R3BSofTofWTcalData.h"
#include "R3BSofTofWHitData.h"
#include "TH1F.h"
#include <TRandom.h>

class TClonesArray;

class R3BSofTofWTCal2Hit : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofTofWTCal2Hit();

    /** Standard constructor **/
    R3BSofTofWTCal2Hit(const char* name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofTofWTCal2Hit();

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

  private:
    Bool_t fOnline; // Don't store data for online

    TClonesArray* fTCalDataCA; /**< Array with Cal input data. >*/
    TClonesArray* fHitDataCA;  /**< Array with Hit output data. >*/

    /** Private method AddHitData **/
    // Adds a SofTofWHitData to the HitCollection
    R3BSofTofWHitData* AddHitData(Int_t paddle, Double_t x, Double_t y, Double_t tof);

  public:
    // Class definition
    ClassDef(R3BSofTofWTCal2Hit, 1)
};

#endif
