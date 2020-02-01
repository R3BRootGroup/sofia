// -----------------------------------------------------------------
// -----                                                       -----
// -----                R3BSofTwimMapped2Cal                   -----
// -----        Created 24/11/19  by J.L. Rodriguez-Sanchez    -----
// -----------------------------------------------------------------

#ifndef R3BSofTwimMapped2Cal_H
#define R3BSofTwimMapped2Cal_H

#include "FairTask.h"
#include "R3BSofTwimCalData.h"
#include "R3BSofTwimMappedData.h"
#include "TH1F.h"
#include <TRandom.h>

#define MAX_MULT_TWIM_CAL 10
#define MAX_NB_TWIMSEC 1
#define MAX_NB_TWIMANODE 16
#define MAX_NB_TWIMTREF 2

class TClonesArray;
class R3BSofTwimCalPar;

class R3BSofTwimMapped2Cal : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofTwimMapped2Cal();

    /** Standard constructor **/
    R3BSofTwimMapped2Cal(const char* name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofTwimMapped2Cal();

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

    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    void SetParameter();

    Int_t fNumSec;
    Int_t fNumAnodes;
    Int_t fNumParams;
    Int_t fNumPosParams;
    Int_t fNumAnodesRef;
    Int_t fMaxMult;
    TArrayF* CalParams;
    TArrayF* PosParams;

    Int_t mulanode[MAX_NB_TWIMSEC][MAX_NB_TWIMANODE + MAX_NB_TWIMTREF];
    Double_t fE[MAX_NB_TWIMSEC][MAX_MULT_TWIM_CAL][MAX_NB_TWIMANODE + MAX_NB_TWIMTREF];
    Double_t fDT[MAX_NB_TWIMSEC][MAX_MULT_TWIM_CAL][MAX_NB_TWIMANODE + MAX_NB_TWIMTREF];

    Bool_t fOnline; // Don't store data for online

    R3BSofTwimCalPar* fCal_Par;      /**< Parameter container. >*/
    TClonesArray* fTwimMappedDataCA; /**< Array with Mapped-input data. >*/
    TClonesArray* fTwimCalDataCA;    /**< Array with Cal-output data. >*/

    /** Private method AddCalData **/
    // Adds a SofTwimCalData to the TwimCalCollection
    R3BSofTwimCalData* AddCalData(UShort_t secID, UShort_t anodeID, Double_t dtime, Double_t energy);

  public:
    // Class definition
    ClassDef(R3BSofTwimMapped2Cal, 1)
};

#endif
