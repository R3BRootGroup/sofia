// ------------------------------------------------------------
// -----                  R3BSofAtOnlineSpectra           -----
// -----    Created 29/09/19  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

#ifndef R3BSofAtOnlineSpectra_H
#define R3BSofAtOnlineSpectra_H

#include "FairTask.h"
#include "TCanvas.h"
#include "TCutG.h"
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
class R3BSofAtOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofAtOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofAtOnlineSpectra(const char* name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofAtOnlineSpectra();

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

    void SetSelection(Int_t section, TCutG* c) { fcutg[section - 1] = c; }

    void SetNumAnodes(Int_t num) { fNumAnodes = num; }

  private:
    Int_t fNumAnodes;

    TClonesArray* fMappedItemsAt; /**< Array with mapped items. */
    TClonesArray* fHitItemsTwim;  /**< Array with hit items. */

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header.      */
    Int_t fNEvents;         /**< Event counter.     */
    Int_t fReset;
    TCutG* fcutg[6];

    // Canvas
    TCanvas* cAtMap_mult;
    TCanvas* cAtMap_E;
    TCanvas* cAtMap_EvsE;
    TCanvas* cAtMap_EvsE_mult1_nopu;
    TCanvas** cTwimZs;
    TCanvas* cTwimZsum;

    // Histograms for Mapped data
    TH1F* fh1_atmap_mult;
    TH1F* fh1_atmap_mult_wo_pu;
    TH2F* fh2_atmap_mult;
    TH2F* fh2_atmap_mult_wo_pu;
    TH1F** fh1_atmap_E;
    TH1F** fh1_atmap_E_mult1_wo_pu_ov;
    TH2F** fh2_atmap_EvsE;
    TH2F** fh2_atmap_EvsE_mult1_nopu;
    TH1F** fh1_Twimhit_Zl;
    TH1F** fh1_Twimhit_Zr;
    TH2F** fh2_Twimhit_ZrZl;
    TH1F* fh1_twim_ZSum[3];

  public:
    ClassDef(R3BSofAtOnlineSpectra, 0)
};

#endif
