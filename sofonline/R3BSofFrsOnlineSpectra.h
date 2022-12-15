// ------------------------------------------------------------
// -----                  R3BSofFrsOnlineSpectra          -----
// -----    Created 28/01/20  by J.L. Rodriguez-Sanchez   -----
// -----           Fill FRS online histograms             -----
// ------------------------------------------------------------

#ifndef R3BSofFrsOnlineSpectra_H
#define R3BSofFrsOnlineSpectra_H

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
 * This taks reads FRS data and plots online histograms
 */
class R3BSofFrsOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofFrsOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofFrsOnlineSpectra(const TString& name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofFrsOnlineSpectra();

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
    TClonesArray* fHitItemsFrs; /**< Array with hit items. */

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header.      */
    Int_t fNEvents;         /**< Event counter.     */

    // Canvas
    TCanvas* cBeta;
    TCanvas* cBrho;
    TCanvas* cAqvsq;
    TCanvas* cXs2vsBeta;

    // Histograms for Hit data
    TH1F* fh1_beta;
    TH1F* fh1_brho;
    TH2F* fh2_Aqvsq;
    TH2F* fh2_Xs2vsbeta;

  public:
    ClassDef(R3BSofFrsOnlineSpectra, 1)
};

#endif
