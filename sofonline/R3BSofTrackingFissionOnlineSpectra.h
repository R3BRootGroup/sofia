// ------------------------------------------------------------
// -----        R3BSofTrackingFissionOnlineSpectra        -----
// -----    Created 28/02/21  by J.L. Rodriguez-Sanchez   -----
// -----         Fill tracking online histograms          -----
// ------------------------------------------------------------

#ifndef R3BSofTrackingFissionOnlineSpectra_H
#define R3BSofTrackingFissionOnlineSpectra_H

#include "FairTask.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2F.h"
#include "TMath.h"
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

class TClonesArray;
class R3BEventHeader;
class R3BTGeoPar;

/**
 * This taks reads FRS data and plots online histograms
 */
class R3BSofTrackingFissionOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofTrackingFissionOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofTrackingFissionOnlineSpectra(const TString& name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofTrackingFissionOnlineSpectra();

    /**
     * Method for task initialization.
     * This function is called by the framework before
     * the event loop.
     * @return Initialization status. kSUCCESS, kERROR or kFATAL.
     */
    virtual InitStatus Init();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    /**
     * Method for event loop implementation.
     * Is called by the framework every time a new event is read.
     * @param option an execution option.
     */
    virtual void Exec(Option_t* option);

    /**
     * A method for finish of processing of an event.
     * Is called by the framework for each event after executing
     * the tasks.
     */
    virtual void FinishEvent();

    /**
     * Method for finish of the task execution.
     * Is called by the framework after processing the event loop.
     */
    virtual void FinishTask();

    virtual void SetParContainers();

    /**
     * Methods to clean histograms.
     */
    virtual void Reset_Histo();

    /**
     * Method to set up charge range in the histograms.
     */
    inline void Set_Charge_range(Float_t minz, Float_t maxz)
    {
        fZ_min = minz;
        fZ_max = maxz;
    }

    inline void Set_TargetPos(Float_t pos) { fPosTarget = pos; }

  private:
    TClonesArray* fMwpc0HitDataCA; /**< Array with Mwpc0 Hit-input data. >*/
    TClonesArray* fMwpc1HitDataCA; /**< Array with Mwpc1 Hit-input data. >*/
    TClonesArray* fMwpc2HitDataCA; /**< Array with Mwpc2 Hit-input data. >*/
    TClonesArray* fMwpc3HitDataCA; /**< Array with Mwpc3 Hit-input data. >*/
    TClonesArray* fTrimHitDataCA;  /**< Array with Triple Music Hit-input data. >*/
    TClonesArray* fTwimHitDataCA;  /**< Array with Twim Hit-input data. >*/
    TClonesArray* fTofWHitDataCA;  /**< Array with TofW Hit-input data. >*/
    TClonesArray* fTrackingDataCA; /**< Array with tracking data at Cave-C. >*/

    // Parameters
    R3BTGeoPar* fMw0GeoPar;
    R3BTGeoPar* fTargetGeoPar;
    R3BTGeoPar* fMw1GeoPar;
    R3BTGeoPar* fMw2GeoPar;
    R3BTGeoPar* fMw3GeoPar;
    R3BTGeoPar* fTofWGeoPar;

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header.      */
    Int_t fNEvents;         /**< Event counter.     */
    Float_t fPosTarget;
    Float_t fWidthTarget;
    Float_t fDist_acelerator_glad;
    Float_t fZ_max, fZ_min;

    // Canvas
    TCanvas* cBeta;
    TCanvas* cBrho;
    TCanvas* cAqvsq;
    TCanvas* cMwpc3vsBeta;
    TCanvas* cTrackingXZ;
    TCanvas* cTrackingYZ;
    TCanvas* cBeamProfileTarget;
    TCanvas* cZvsBeta;

    // Histograms for Hit data
    TH1F* fh1_beta;
    TH1F* fh1_brho;
    TH2F* fh2_Aqvsq;
    TH2F* fh2_Mwpc3vsbeta;
    TH2F* fh2_tracking_planeXZ;
    TH2F* fh2_tracking_planeYZ;
    TH2F* fh2_target_PosXY;
    TH2F* fh2_ZvsBeta;

  public:
    ClassDef(R3BSofTrackingFissionOnlineSpectra, 1)
};

#endif
