// ------------------------------------------------------------
// -----         R3BSofTwimvsTrimOnlineSpectra            -----
// -----    Created 03/03/21  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

#ifndef R3BSofTwimvsTrimOnlineSpectra_H
#define R3BSofTwimvsTrimOnlineSpectra_H

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
 * This taks reads TWIM data and plots online histograms
 */
class R3BSofTwimvsTrimOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofTwimvsTrimOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofTwimvsTrimOnlineSpectra(const TString& name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofTwimvsTrimOnlineSpectra();

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
    TClonesArray* fHitItemsTwim; /**< Array with hit-Twim items. */
    TClonesArray* fHitItemsTrim; /**< Array with hit-Trim items. */

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header.      */
    Int_t fNEvents;         /**< Event counter.     */

    // Histograms for Hit data
    TH2F* fh2_hit_z;
    TH2F* fh2_hit_theta;

  public:
    ClassDef(R3BSofTwimvsTrimOnlineSpectra, 1)
};

#endif
