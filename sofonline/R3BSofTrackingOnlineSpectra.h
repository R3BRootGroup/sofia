// ------------------------------------------------------------
// -----             R3BSofTrackingOnlineSpectra          -----
// -----    Created 28/01/20  by J.L. Rodriguez-Sanchez   -----
// -----         Fill tracking online histograms          -----
// ------------------------------------------------------------

#ifndef R3BSofTrackingOnlineSpectra_H
#define R3BSofTrackingOnlineSpectra_H

#include "FairTask.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2F.h"
#include "TMath.h"
#include <R3BShared.h>

#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

class TClonesArray;
class R3BEventHeader;

/**
 * This taks reads FRS data and plots online histograms
 */
class R3BSofTrackingOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofTrackingOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofTrackingOnlineSpectra(const TString& name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofTrackingOnlineSpectra();

    /**
     * Method for task initialization.
     * This function is called by the framework before
     * the event loop.
     * @return Initialization status. kSUCCESS, kERROR or kFATAL.
     */
    virtual InitStatus Init();

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

  private:
    TClonesArray* fMwpc0HitDataCA; /**< Array with Mwpc0 Hit-input data. >*/
    TClonesArray* fMwpc1HitDataCA; /**< Array with Mwpc1 Hit-input data. >*/
    TClonesArray* fMwpc2HitDataCA; /**< Array with Mwpc2 Hit-input data. >*/
    TClonesArray* fMwpc3HitDataCA; /**< Array with Mwpc3 Hit-input data. >*/
    TClonesArray* fMusicHitDataCA; /**< Array with Music Hit-input data. >*/
    TClonesArray* fTwimHitDataCA;  /**< Array with Twim Hit-input data. >*/
    TClonesArray* fTofWHitDataCA;  /**< Array with TofW Hit-input data. >*/
    TClonesArray* fTrackingDataCA; /**< Array with tracking data at Cave-C. >*/

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
    ClassDef(R3BSofTrackingOnlineSpectra, 1)
};

#endif
