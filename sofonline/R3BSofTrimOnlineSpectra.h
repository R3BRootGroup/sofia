#ifndef R3BSofTrimOnlineSpectra_H
#define R3BSofTrimOnlineSpectra_H

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

#include "detectors_cfg.h"

#define NbSectionsTrim NbSections_SofTrim
#define NbAnodesTrim   NbAnodesPerSection_SofTrim
#define NbTrefTrim     NbTrefPerSection_SofTrim
#define NbTtrigTrim    NbTtrigPerSection_SofTrim

class TClonesArray;
class R3BEventHeader;

/**
 * This taks reads TWIM data and plots online histograms
 */
class R3BSofTrimOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofTrimOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofTrimOnlineSpectra(const TString& name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofTrimOnlineSpectra();

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
    TClonesArray* fMappedItemsTrim; /**< Array with mapped items. */

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header.      */
    Int_t fNEvents;         /**< Event counter.     */

    // Canvas for Mapped data
    TCanvas* cTrimMap_E[NbSectionsTrim];
    TCanvas* cTrimMap_DT[NbSectionsTrim];
    TCanvas* cTrimMap_Mult[NbSectionsTrim]; 
    TCanvas* cTrimMap_DeltaTrefTtrig;
    TCanvas* cTrimMap_EvsDT[NbSectionsTrim];
    TCanvas* cTrimMap_DTvsDT[NbSectionsTrim];

    // Histograms for Mapped data
    TH1F* fh1_trimmap_E[NbSectionsTrim][NbAnodesTrim];
    TH1F* fh1_trimmap_DT[NbSectionsTrim][NbAnodesTrim];
    TH1F* fh1_trimmap_Mult[NbSectionsTrim];
    TH1F* fh1_trimmap_DeltaTrefTtrig[NbSectionsTrim];
    TH2F* fh2_trimmap_EvsDT[NbSectionsTrim][NbAnodesTrim];
    TH2F* fh2_trimmap_DTvsDT[NbSectionsTrim][NbAnodesTrim-1];

  public:
    ClassDef(R3BSofTrimOnlineSpectra, 1)
};

#endif
