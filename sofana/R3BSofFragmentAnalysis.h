// ----------------------------------------------------------------------
// -----                                                            -----
// -----                     R3BSofFragmentAnalysis                 -----
// -----             Created 09/02/20  by J.L. Rodriguez-Sanchez    -----
// ----------------------------------------------------------------------

#ifndef R3BSofFragmentAnalysis_H
#define R3BSofFragmentAnalysis_H

#include "FairTask.h"
#include "R3BSofTrackingData.h"
#include "TF1.h"
#include "TH1F.h"
#include <TRandom.h>

class TClonesArray;

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

  private:
    void SetParameter();

    Double_t fOffsetAq, fOffsetZ; // Offsets in A/q and Z

    Double_t frho_Cave, fBfield_Glad;
    Double_t fDist_mw3_tof;
    Double_t fDist_start_glad;

    Bool_t fOnline; // Don't store data for online

    TClonesArray* fMwpcHitDataCA;  /**< Array with Mwpc Hit-input data. >*/
    TClonesArray* fTwimHitDataCA;  /**< Array with Twim Hit-input data. >*/
    TClonesArray* fTofWHitDataCA;  /**< Array with ToF Hit-input data. >*/
    TClonesArray* fTrackingDataCA; /**< Array with Tracking-output data. >*/

    /** Private method TrackingData **/
    //** Adds a TrackingData to the analysis
    R3BSofTrackingData* AddData(Double_t z, Double_t aq, Double_t beta, Double_t length, Double_t brho);

  public:
    // Class definition
    ClassDef(R3BSofFragmentAnalysis, 1)
};

#endif
