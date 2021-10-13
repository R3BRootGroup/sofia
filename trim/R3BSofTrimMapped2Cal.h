// -----------------------------------------------------------------
// -----                R3BSofTrimMapped2Cal                   -----
// -----------------------------------------------------------------

#ifndef R3BSofTrimMapped2Cal_H
#define R3BSofTrimMapped2Cal_H

#include "FairTask.h"
#include "R3BSofTrimCalData.h"
#include "R3BSofTrimMappedData.h"
#include "TH1F.h"
#include <TRandom.h>

#define MAX_MULT_TRIM_CAL 10

class TClonesArray;
class R3BSofTrimCalPar;

class R3BSofTrimMapped2Cal : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofTrimMapped2Cal();

    /** Standard constructor **/
    R3BSofTrimMapped2Cal(const char* name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofTrimMapped2Cal();

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
    Bool_t fOnline; // Don't store data for online

    R3BSofTrimCalPar* fCal_Par;    /**< Parameter container. >*/
    TClonesArray* fTrimMappedData; /**< Array with Mapped-input data. >*/
    TClonesArray* fTrimCalData;    /**< Array with Cal-output data. >*/

    Int_t fNumSections;
    Int_t fNumAnodes;
    Int_t fNumChannels;

    /** Private method AddCalData **/
    R3BSofTrimCalData* AddCalData(Int_t secID,
                                  Int_t anodeID,
                                  Double_t dtraw,
                                  Double_t dtal,
                                  Float_t esub,
                                  Float_t ematch);

  public:
    // Class definition
    ClassDef(R3BSofTrimMapped2Cal, 1)
};

#endif
