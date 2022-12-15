// ----------------------------------------------------------------------
// -----                                                            -----
// -----                     R3BSofFissionAnalysis                  -----
// -----             Created 14/02/21  by J.L. Rodriguez-Sanchez    -----
// ----------------------------------------------------------------------

#ifndef R3BSofFissionAnalysis_H
#define R3BSofFissionAnalysis_H

// ROOT headers
#include "TArrayF.h"
#include "TClonesArray.h"
#include "TF1.h"
#include "TH1F.h"
#include "TMath.h"
#include "TRandom.h"
#include "TRotation.h"
#include "TVector3.h"

#include <TRandom.h>
#include <iomanip>

// Fair headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairTask.h"

// SOFIA headers
#include "R3BSofTrackingData.h"
#include "R3BTwimHitPar.h"

class TClonesArray;
class R3BTGeoPar;
class R3BSofGladFieldPar;

class R3BSofFissionAnalysis : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofFissionAnalysis();

    /** Standard constructor **/
    R3BSofFissionAnalysis(const TString& name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofFissionAnalysis();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* option);

    /** Virtual method Reset **/
    virtual void Reset();

    /**
     * A method for finish of processing of an event.
     * Is called by the framework for each event after executing
     * the tasks.
     */
    virtual void FinishEvent();

    virtual void SetParContainers();

    // Fair specific
    /** Virtual method Init **/
    virtual InitStatus Init();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    /** Virtual method Finish **/
    virtual void Finish();

    /** Accessor to select online mode **/
    void SetOnline(Bool_t option) { fOnline = option; }

    /** Accessor functions **/
    void SetOffsetAq(Double_t theAq) { fOffsetAq = theAq; }

  private:
    void SetParameter();
    Double_t GetLength(Double_t mw1, Double_t mw2, Double_t mw3);
    Double_t GetBrho(Double_t mw1, Double_t mw2, Double_t mw3);
    Double_t GetVelocity(Double_t len, Double_t tof);
    Double_t GetAoverq(Double_t brho, Double_t vel);

    Bool_t fOnline;   // Don't store data for online

    // Parameters from par file
    Double_t fOffsetAq;   // Offset in A/q

    // Parameters set with accessor functions
    Double_t fFieldCentre, fEffLength, fBfield_Glad;

    R3BSofGladFieldPar* fGladPar;
    R3BTwimHitPar* fTwimPar;
    R3BTGeoPar* fMw0GeoPar;
    R3BTGeoPar* fTargetGeoPar;
    R3BTGeoPar* fMw1GeoPar;
    R3BTGeoPar* fMw2GeoPar;
    R3BTGeoPar* fMw3GeoPar;
    R3BTGeoPar* fTofWGeoPar;

    TClonesArray* fMwpc0HitDataCA; /**< Array with Mwpc Hit-input data. >*/
    TClonesArray* fTwimHitDataCA;  /**< Array with Twim Hit-input data. >*/
    TClonesArray* fMwpc1HitDataCA; /**< Array with Mwpc Hit-input data. >*/
    TClonesArray* fMwpc2HitDataCA; /**< Array with Mwpc Hit-input data. >*/
    TClonesArray* fMwpc3HitDataCA; /**< Array with Mwpc Hit-input data. >*/
    TClonesArray* fTofWHitDataCA;  /**< Array with ToF Hit-input data. >*/
    TClonesArray* fTrackingDataCA; /**< Array with Tracking-output data. >*/

    // Private method TrackingData
    R3BSofTrackingData* AddData(Double_t z, Double_t aq, Double_t beta, Double_t length, Double_t brho, Int_t paddle);

  public:
    // Class definition
    ClassDef(R3BSofFissionAnalysis, 1)
};

#endif /* R3BSofFissionAnalysis_H */
