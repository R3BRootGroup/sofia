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

    /** Method to set up the experiment ID **/
    void SetExpId(Int_t exp) { fExpId = exp; }

    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    void SetParameter();

    Int_t fNumSec;
    Int_t fNumAnodes;
    Int_t fNumParams;
    Int_t fNumPosParams;
    Int_t fNumAnodesRef;
    Int_t fNumAnodesTrig;
    Int_t fMaxMult;
    Int_t fExpId;
    TArrayF* CalParams;
    TArrayF* PosParams;

    Int_t mulanode[4][16 + 4];
    Double_t fE[4][20][16 + 4];
    Double_t fDT[4][20][16 + 4];

    Bool_t fOnline; // Don't store data for online

    R3BSofTwimCalPar* fCal_Par;      /**< Parameter container. >*/
    TClonesArray* fTwimMappedDataCA; /**< Array with Mapped-input data. >*/
    TClonesArray* fTwimCalDataCA;    /**< Array with Cal-output data. >*/

    /** Private method AddCalData **/
    // Adds a SofTwimCalData to the TwimCalCollection
    R3BSofTwimCalData* AddCalData(Int_t secID, Int_t anodeID, Double_t dtime, Double_t energy);

  public:
    // Class definition
    ClassDef(R3BSofTwimMapped2Cal, 1)
};

#endif
