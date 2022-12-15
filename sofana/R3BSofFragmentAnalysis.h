// ----------------------------------------------------------------------
// -----                                                            -----
// -----                     R3BSofFragmentAnalysis                 -----
// -----             Created 09/02/20  by J.L. Rodriguez-Sanchez    -----
// ----------------------------------------------------------------------

#ifndef R3BSofFragmentAnalysis_H
#define R3BSofFragmentAnalysis_H

// ROOT headers
#include "TArrayF.h"
#include "TClonesArray.h"
#include "TF1.h"
#include "TH1F.h"
#include "TMath.h"
#include "TRandom.h"
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
#include "R3BEventHeader.h"
#include "R3BLogger.h"
#include "R3BMusicHitData.h"
#include "R3BMwpcHitData.h"
#include "R3BSofFragmentAnaPar.h"
#include "R3BSofTofWHitData.h"
#include "R3BSofTrackingData.h"
#include "R3BTGeoPar.h"
#include "R3BTwimHitData.h"
#include "R3BTwimHitPar.h"

class TClonesArray;
class R3BEventHeader;
class R3BSofTrackingData;

class R3BSofFragmentAnalysis : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofFragmentAnalysis();

    /** Standard constructor **/
    R3BSofFragmentAnalysis(const TString& name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofFragmentAnalysis();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* option);

    /** Virtual method Reset **/
    virtual void Reset();

    /**
     * A method for finish of processing of an event.
     * Is called by the framework for each event after executing
     * the tasks.
     */
    virtual void FinishEvent() { Reset(); }

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
    void SetOffsetZ(Double_t theZ) { fOffsetZ = theZ; }
    void SetTofWPos(Double_t pos) { fTofWPos = pos; }

  private:
    void SetParameter();

    R3BEventHeader* header; /**< Event header. */
    Int_t fExpId;

    // Parameters set with accessor functions
    Double_t frho_Cave, fBfield_Glad, fTimeOffset, fTofWPos;
    Bool_t fOnline; // Don't store data for online
    R3BSofFragmentAnaPar* fFragPar;
    R3BTwimHitPar* fTwimPar;

    // Parameters from par file
    Float_t fTwimZ0 = 0., fTwimZ1 = 0., fTwimZ2 = 0.; // CalibPar for Twim
    Double_t fOffsetAq, fOffsetZ;                     // Offsets in A/q and Z
    // Double_t fDist_mw3_tof;
    // Double_t fDist_start_glad;

    TClonesArray* fMwpc0HitDataCA; /**< Array with Mwpc Hit-input data. >*/
    TClonesArray* fMwpc1HitDataCA; /**< Array with Mwpc Hit-input data. >*/
    TClonesArray* fMwpc2HitDataCA; /**< Array with Mwpc Hit-input data. >*/
    TClonesArray* fMwpc3HitDataCA; /**< Array with Mwpc Hit-input data. >*/
    TClonesArray* fMusicHitDataCA; /**< Array with R3BMusic Hit-input data. >*/
    TClonesArray* fTwimHitDataCA;  /**< Array with Twim Hit-input data. >*/
    TClonesArray* fTofWHitDataCA;  /**< Array with ToF Hit-input data. >*/
    TClonesArray* fTrackingDataCA; /**< Array with Tracking-output data. >*/
    TClonesArray* fRoluPosDataCA;  /**< Array with reconstructed ROLU positions. >*/

    R3BSofTofWHitData** HitTofW;
    R3BMusicHitData** HitMusic;
    R3BTwimHitData** HitTwim;
    R3BMwpcHitData** HitMwpc0;
    R3BMwpcHitData** HitMwpc1;
    R3BMwpcHitData** HitMwpc2;
    R3BMwpcHitData** HitMwpc3;

    R3BTGeoPar* fMw0GeoPar;
    R3BTGeoPar* fMw1GeoPar;
    R3BTGeoPar* fMw2GeoPar;
    R3BTGeoPar* fRoluGeoPar;

    /** Private method TrackingData **/
    //** Adds a TrackingData to the analysis
    R3BSofTrackingData* AddData(Double_t z, Double_t aq, Double_t beta, Double_t length, Double_t brho, Int_t paddle);
    R3BMwpcHitData* AddRoluPos(Double_t x, Double_t y);

  public:
    // Class definition
    ClassDef(R3BSofFragmentAnalysis, 1)
};

#endif
