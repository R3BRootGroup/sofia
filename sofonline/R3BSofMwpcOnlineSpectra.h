// ------------------------------------------------------------
// -----                  R3BSofMwpcOnlineSpectra         -----
// -----    Created 29/09/19  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

#ifndef R3BSofMwpcOnlineSpectra_H
#define R3BSofMwpcOnlineSpectra_H

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

/**
 * This taks reads MWPC data and plots online histograms
 */
class R3BSofMwpcOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofMwpcOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     * @param namedet a name of the detector.
     */
    R3BSofMwpcOnlineSpectra(const TString& name, Int_t iVerbose = 1, const TString& namedet = "MWPC");

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofMwpcOnlineSpectra();

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

  private:
    TClonesArray* fCalItemsMwpc; /**< Array with cal items. */

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header.      */
    Int_t fNEvents;         /**< Event counter.     */
    TString fNameDet;

    // Canvas
    TCanvas *cMWPCCal, *cMWPCCal2D;

    // Histograms for Mapped data
    TH2F* fh2_mwpc_cal;
    TH1F* fh1_mwpc_cal[2];

    // Histograms for Mapped data
    // TH2F* fh2_atcal[3];

  public:
    ClassDef(R3BSofMwpcOnlineSpectra, 1)
};

#endif
