// ------------------------------------------------------------
// -----          R3BAmsCorrelationOnlineSpectra          -----
// -----    Created 31/01/20  by J.L. Rodriguez-Sanchez   -----
// -----     Fill AMS-CALIFA-MUSICs online histograms     -----
// ------------------------------------------------------------

#ifndef R3BAmsCorrelationOnlineSpectra_H
#define R3BAmsCorrelationOnlineSpectra_H

#include "FairTask.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TMath.h"
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

class TClonesArray;
class R3BEventHeader;

/**
 * This taks reads AMS-CALIFA-MUSICs data and plots correlated online histograms
 */
class R3BAmsCorrelationOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BAmsCorrelationOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BAmsCorrelationOnlineSpectra(const TString& name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BAmsCorrelationOnlineSpectra();

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

    void Reset_Histo();

  private:
    TClonesArray* fHitItemsAms;    /**< Array with hit items. */

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header. */
    Int_t fTrigger;         /**< Trigger value. */
    Int_t fNEvents;         /**< Event counter. */

    TCanvas* cHit[6];
    TCanvas* cHitAngles;
    TCanvas* cHitEnergyCor;
    TCanvas* cHitThetaCor;
    TCanvas* cHitPhiCor;

    TH2F* fh_Ams_hit_Pos[6];
    TH2F* fh_Ams_hit_E[6];
    TH2F* fh_Ams_hit_E_theta[6];
    TH1F* fh_Ams_hit_Mul[6];
    TH2F* fh2_ams_theta_phi[2];
    TH2F* fh2_ams_e1_e2[2];
    TH2F* fh2_ams_theta1_theta2[2];
    TH2F* fh2_ams_phi1_phi2[2];

    // TString fAmsFile;        	      /**< Config file name. */
    Int_t fNbDet; /**< Number of AMS detectors. */

  public:
    ClassDef(R3BAmsCorrelationOnlineSpectra, 1)
};

#endif
