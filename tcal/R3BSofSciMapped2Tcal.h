// *** *************************************************************** *** //
// ***                  R3BSofSciMapped2Tcal                           *** //
// ***    convert Mapped data to tcal data                             *** //
// ***    from the fine and coarse times, calculate a raw time in ns   *** //
// *** *************************************************************** *** //

#ifndef R3BSOFSCI_MAPPED2TCAL
#define R3BSOFSCI_MAPPED2TCAL

#include "FairTask.h"

// ROOT headers
#include "TClonesArray.h"
#include "TMath.h"
#include "TRandom3.h"
class TRandom3;

// Fair headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

// SofSci headers
#include "R3BSofSciMappedData.h"
#include "R3BSofSciTcalData.h"
#include "R3BSofTcalPar.h"

class R3BSofSciMapped2Tcal : public FairTask
{

  public:
    // --- Default constructor --- //
    R3BSofSciMapped2Tcal();

    // --- Standard constructor --- //
    R3BSofSciMapped2Tcal(const char* name, Int_t iVerbose = 1);

    // --- Destructor --- //
    virtual ~R3BSofSciMapped2Tcal();

    virtual void Exec(Option_t* option);

    virtual void SetParContainers();

    virtual InitStatus Init();

    virtual InitStatus ReInit();

    virtual void Finish();

    /** Virtual method Reset **/
    virtual void Reset();

    void SetOnline(Bool_t option) { fOnline = option; }

    Double_t CalculateTimeNs(UShort_t det, UShort_t pmt, UInt_t tf, UInt_t tc);

  private:
    Bool_t fOnline; // Don't store data for online

    TClonesArray* fMapped;   // input data - SofSci
    R3BSofTcalPar* fTcalPar; // tcal parameters container - SofSci
    TClonesArray* fTcal;     // output data

    UInt_t fNevent;

    TRandom3 rand;

    /** Private method CalData **/
    //** Adds a TcalData to the detector
    R3BSofSciTcalData* AddTcalData(Int_t det, Int_t ch, Double_t tns, UInt_t clock);

  public:
    ClassDef(R3BSofSciMapped2Tcal, 1)
};

#endif // R3BSOFSCI_MAPPED2TCAL
