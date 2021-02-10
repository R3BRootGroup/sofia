// -----------------------------------------------------------------
// -----         R3BSofTofWSingleTCal2Hit source file          -----
// -----    Created 15/02/20  by J.L. Rodriguez-Sanchez        -----
// -----------------------------------------------------------------

#ifndef R3BSofTofWSingleTCal2Hit_H
#define R3BSofTofWSingleTCal2Hit_H

// ROOT headers
#include "TClonesArray.h"
#include "TH1F.h"
#include "TMath.h"
#include <TRandom.h>

// Fair headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairTask.h"

#include <iomanip>

// TofW headers
#include "R3BSofTofWHitData.h"
#include "R3BSofTofWHitPar.h"
#include "R3BSofTofWSingleTcalData.h"
#include "R3BSofTofWTcalData.h"

class TClonesArray;
class R3BTGeoPar;

class R3BSofTofWSingleTCal2Hit : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofTofWSingleTCal2Hit();

    /** Standard constructor **/
    R3BSofTofWSingleTCal2Hit(const char* name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofTofWSingleTCal2Hit();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* option);

    /** Public method Experiment s455 **/
    virtual void S455();

    /** Public method Experiment s467 **/
    virtual void S467();

    /** Virtual method Reset **/
    virtual void Reset();

    // Fair specific
    virtual void SetParContainers();

    /** Virtual method Init **/
    virtual InitStatus Init();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    /** Virtual method Finish **/
    virtual void Finish();
    virtual void FinishEvent();

    void SetOnline(Bool_t option) { fOnline = option; }
    void SetExpId(Int_t exp) { fExpId = exp; }
    void SetTofLISE(Double_t tof) { fTof_lise = tof; }

    Double_t GetTofLISE() { return fTof_lise; }

  private:
    Bool_t fOnline; // Don't store data for online
    Int_t fExpId;

    TClonesArray* fTCalDataCA; /**< Array with Cal input data. >*/
    TClonesArray* fHitDataCA;  /**< Array with Hit output data. >*/
    Double_t fTof_lise;
    R3BSofTofWSingleTcalData** calDat;

    R3BTGeoPar* fTofWGeoPar;
    R3BSofTofWHitPar* fTofWHitPar;

    /** Private method AddHitData **/
    // Adds a SofTofWHitData to the HitCollection
    R3BSofTofWHitData* AddHitData(Int_t paddle, Double_t x, Double_t y, Double_t tof);

  public:
    // Class definition
    ClassDef(R3BSofTofWSingleTCal2Hit, 1)
};

#endif
