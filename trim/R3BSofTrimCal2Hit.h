// --------------------------------------------------------------
// -----                R3BSofTrimCal2Hit                   -----
// --------------------------------------------------------------

#ifndef R3BSofTrimCal2Hit_H
#define R3BSofTrimCal2Hit_H

#include "FairTask.h"
#include "R3BSofTrimHitData.h"
#include "TH1F.h"
#include <TRandom.h>

class TClonesArray;
class R3BSofTrimHitPar;

class R3BSofTrimCal2Hit : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofTrimCal2Hit();

    /** Standard constructor **/
    R3BSofTrimCal2Hit(const char* name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofTrimCal2Hit();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* option);

    /** Virtual method Reset **/
    virtual void Reset();

    virtual void SetParContainers();

    // Fair specific
    /** Virtual method Init **/
    virtual InitStatus Init();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    /** Virtual method Finish **/
    virtual void Finish();

    void SetNumSections(Int_t n) { fNumSections = n; }
    void SetNumAnodes(Int_t n) { fNumAnodes = n; }
    void SetOnline(Bool_t option) { fOnline = option; }
    void SetTriShape(Bool_t shape) { fTriShape = shape; }

  private:
    Int_t fNumSections;
    Int_t fNumAnodes;
    Bool_t fTriShape;
    Bool_t fOnline; // Don't store data for online

    R3BSofTrimHitPar* fTrimHitPar; // Parameter container
    TClonesArray* fTrimCalData;    // Array with Cal input data for Triple-MUSIC
    // to do : TClonesArray* fSciHitData;    // Array with Hit input data for incoming beam velocity
    TClonesArray* fTrimHitData; // Array with Hit output data for Triple-MUSIC

    // --- Private method --- //
    R3BSofTrimHitData* AddHitData(Int_t secID, Float_t Eraw, Float_t Ebeta, Float_t Edt, Float_t Etheta, Float_t Z);

  public:
    //--- Class definition --- //
    ClassDef(R3BSofTrimCal2Hit, 1)
};

#endif
