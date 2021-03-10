// ------------------------------------------------------------
// -----                  R3BSofOnlineSpectra             -----
// -----    Created 29/09/19  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

#ifndef R3BSofOnlineSpectra_H
#define R3BSofOnlineSpectra_H

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
class R3BSofAtOnlineSpectra;
class R3BSofMwpcOnlineSpectra;
class R3BSofMwpcCorrelationOnlineSpectra;
class R3BSofTrimOnlineSpectra;
class R3BSofTwimOnlineSpectra;
class R3BSofTwimvsMusicOnlineSpectra;
class R3BSofTwimvsTrimOnlineSpectra;
class R3BSofSciOnlineSpectra;
class R3BSofSciVsMusicOnlineSpectra;
class R3BSofSciVsMwpc0OnlineSpectra;
class R3BSofTofWOnlineSpectra;
class R3BSofScalersOnlineSpectra;
class R3BMusicOnlineSpectra;
class R3BAmsOnlineSpectra;
class R3BCalifaOnlineSpectra;
class R3BSofFrsOnlineSpectra;
class R3BSofTrackingOnlineSpectra;
class R3BSofTrackingFissionOnlineSpectra;

/**
 * This taks reads General SOFIA data and plots online histograms
 */
class R3BSofOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofOnlineSpectra(const TString& name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofOnlineSpectra();

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
    virtual void Reset_GENERAL_Histo();

  private:
    TClonesArray* fWRItemsMaster;  /**< Array with WR-Master items. */
    TClonesArray* fWRItemsSofia;   /**< Array with WR-Sofia items. */
    TClonesArray* fWRItemsCalifa;  /**< Array with WR-Califa items. */
    TClonesArray* fWRItemsNeuland; /**< Array with WR-Neuland items. */
    TClonesArray* fWRItemsS2;      /**< Array with WR-S2 items. */
    TClonesArray* fWRItemsS8;      /**< Array with WR-S8 items. */

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* fEventHeader; /**< Event header.      */
    Int_t fTrigger;               /**< Trigger value.     */
    Int_t fNEvents;               /**< Event counter.     */

    R3BSofAtOnlineSpectra* fAtOnline;
    R3BSofMwpcOnlineSpectra* fMwpc0Online;
    R3BSofMwpcCorrelationOnlineSpectra* fMwpc01Online;
    R3BSofMwpcCorrelationOnlineSpectra* fMwpc02Online;
    R3BSofMwpcCorrelationOnlineSpectra* fMwpc12Online;
    R3BSofMwpcCorrelationOnlineSpectra* fMwpc23Online;
    R3BSofMwpcOnlineSpectra* fMwpc1Online;
    R3BSofMwpcOnlineSpectra* fMwpc2Online;
    R3BSofMwpcOnlineSpectra* fMwpc3Online;
    R3BSofTrimOnlineSpectra* fTrimOnline;
    R3BSofTwimOnlineSpectra* fTwimOnline;
    R3BSofTwimvsMusicOnlineSpectra* fTwimVsMusicOnline;
    R3BSofTwimvsTrimOnlineSpectra* fTwimVsTrimOnline;
    R3BSofSciOnlineSpectra* fSciOnline;
    R3BSofSciVsMusicOnlineSpectra* fSciVsMusOnline;
    R3BSofSciVsMwpc0OnlineSpectra* fSciVsMw0Online;
    R3BSofTofWOnlineSpectra* fTofWOnline;
    R3BSofScalersOnlineSpectra* fScalersOnline;
    R3BMusicOnlineSpectra* fMusicOnline;
    R3BAmsOnlineSpectra* fAmsOnline;
    R3BCalifaOnlineSpectra* fCalifaOnline;
    R3BSofFrsOnlineSpectra* fFrsOnline;
    R3BSofTrackingOnlineSpectra* fTrackOnline;
    R3BSofTrackingFissionOnlineSpectra* fTrackFFOnline;

    // Canvas
    TCanvas *cTrigger, *cWr, *cWrs;

    // Unpack
    TH1F *fh1_trigger, *fh1_wr[2];
    TH1F* fh1_wrs[5];

  public:
    ClassDef(R3BSofOnlineSpectra, 0)
};

#endif
