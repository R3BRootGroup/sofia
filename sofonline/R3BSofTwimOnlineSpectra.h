// ------------------------------------------------------------
// -----                R3BSofTwimOnlineSpectra           -----
// -----    Created 06/10/19  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

#ifndef R3BSofTwimOnlineSpectra_H
#define R3BSofTwimOnlineSpectra_H

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

#define NbSections 4
#define NbAnodes 16

class TClonesArray;
class R3BEventHeader;

/**
 * This taks reads FRS data and plots online histograms
 */
class R3BSofTwimOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofTwimOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofTwimOnlineSpectra(const char* name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofTwimOnlineSpectra();

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
    TClonesArray* fMappedItemsTwim; /**< Array with mapped items. */

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header.      */
    Int_t fNEvents;         /**< Event counter.     */

    // Canvas
    TCanvas* cTwimMap_E[NbSections];

    // Histograms for Mapped data
    TH1F* fh1_twimmap_E[NbSections][NbAnodes];

    // Histograms for Cal data

  public:
    ClassDef(R3BSofTwimOnlineSpectra, 1)
};

#endif
