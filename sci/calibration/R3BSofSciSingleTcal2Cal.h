#ifndef R3BSofSciSingleTCal2Cal_H
#define R3BSofSciSingleTCal2Cal_H 1

#include "FairTask.h"
#include "R3BSofSciCalData.h"
#include "R3BSofSciCalPosPar.h"
#include "R3BSofSciCalTofPar.h"
#include "R3BSofSciSingleTcalData.h"
#include "TClonesArray.h"
#include "TMath.h"
#include "TRandom.h"

class R3BSofSciSingleTcal2Cal : public FairTask
{
  public:
    // --- Default constructor --- //
    R3BSofSciSingleTcal2Cal();

    // --- Standard constructor --- //
    R3BSofSciSingleTcal2Cal(const char* name, Int_t iVerbose = 1);

    // --- Destructor --- //
    virtual ~R3BSofSciSingleTcal2Cal();

    // --- Virtual method Exec --- //
    virtual void Exec(Option_t* option);

    // --- Virtual method Reset --- //
    virtual void Reset();

    // --- Fair specific --- //
    virtual InitStatus Init();
    virtual void SetParContainers();
    virtual InitStatus ReInit();
    virtual void Finish(){};
    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    Bool_t fOnline; // Don't store data for online
    UInt_t fNevent;

    TClonesArray* fSingleTcal;   // array with SingleTcal input data
    TClonesArray* fCal;          // array with Cal output data
    R3BSofSciCalPosPar* fPosPar; // calib param for x position in [mm]
    R3BSofSciCalTofPar* fTofPar; // calib param for Beta and Tof calibration

    /** Private method AddHitData **/
    // Add a SofSciCalData
    R3BSofSciCalData* AddCalData(Int_t det, Double_t x, Double_t b2, Double_t b8, Double_t t2, Double_t t8);

  public:
    // Class definition
    ClassDef(R3BSofSciSingleTcal2Cal, 1)
};

#endif
