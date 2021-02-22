// ------------------------------------------------------------
// -----                R3BSofSciVsMwpc0OnlineSpectra            -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

#ifndef R3BSofSciVsMwpc0OnlineSpectra_H
#define R3BSofSciVsMwpc0OnlineSpectra_H

#include "FairTask.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2D.h"
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
 * This taks reads SCI data and plots online histograms
 */
class R3BSofSciVsMwpc0OnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofSciVsMwpc0OnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofSciVsMwpc0OnlineSpectra(const char* name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofSciVsMwpc0OnlineSpectra();

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

    /** Virtual method Reset **/
    virtual void Reset();

    void SetNbDetectors(Int_t ndets) { fNbDetectors = ndets; }
    Int_t GetNbDetectors() { return fNbDetectors; }

  private:
    TClonesArray* fSTcalSci; /**< Array with SofSci single tcal data */
    TClonesArray* fCalSci;   /**< Array with SofSci cal data */
    TClonesArray* fHitMwpc0; /**< Array with SofMwpc0 hit data */

    Int_t fNbDetectors;

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header.      */
    Int_t fNEvents;         /**< Event counter.     */

    // Canvas
    TCanvas* cMwpc0vsRawPos;
    TCanvas* cMwpc0vsCalPos;

    // Histogram for correlation with Mwpc0
    TH2F* fh2_Mwpc0XvsRawPos;
    TH2F* fh2_Mwpc0YvsRawPos;
    TH2F* fh2_Mwpc0XvsCalPos;
    TH2F* fh2_Mwpc0YvsCalPos;

    // check how many raw pos found

  public:
    ClassDef(R3BSofSciVsMwpc0OnlineSpectra, 1)
};

#endif
