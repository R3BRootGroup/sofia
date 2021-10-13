// ------------------------------------------------------------
// -----                 R3BSofStatusOnlineSpectra        -----
// -----    Created 15/03/21  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

#ifndef R3BSofStatusOnlineSpectra_H
#define R3BSofStatusOnlineSpectra_H

#include "FairTask.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TH2F.h"
#include "THStack.h"
#include "TLatex.h"
#include "TMath.h"
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

class TClonesArray;
class R3BEventHeader;

/**
 * This taks reads General SOFIA data and plots online histograms
 */
class R3BSofStatusOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofStatusOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofStatusOnlineSpectra(const TString& name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofStatusOnlineSpectra();

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
    TClonesArray* fWRItemsAms;     /**< Array with WR-Ams items. */
    TClonesArray* fWRItemsNeuland; /**< Array with WR-Neuland items. */
    TClonesArray* fWRItemsS2;      /**< Array with WR-S2 items. */
    TClonesArray* fWRItemsS8;      /**< Array with WR-S8 items. */

    TClonesArray* fMwpc0MappedDataCA; /**< Array with Mapped input data. >*/
    TClonesArray* fTrimMappedData;    /**< Array with Mapped-input data. >*/
    TClonesArray* fMappedItemsCalifa;
    TClonesArray* fMappedItemsAms;
    TClonesArray* fMwpc1MappedDataCA; /**< Array with Mapped input data. >*/
    TClonesArray* fMwpc2MappedDataCA; /**< Array with Mapped input data. >*/
    TClonesArray* fMwpc3MappedDataCA; /**< Array with Mapped input data. >*/
    TClonesArray* fTwimMappedDataCA;  /**< Array with Mapped-input data. >*/
    TClonesArray* fTofWMappedDataCA;

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* fEventHeader; /**< Event header.      */
    Int_t fTrigger;               /**< Trigger value.     */
    Float_t fNEvents;             /**< Event counter.     */
    Float_t fCounterTpats[16];
    Float_t fCounterRates[16];
    Float_t fCounterDet[10];

    // Canvas
    TCanvas *cTrigger, *cWr, *cWrs, *cTotNbTrig, *cDetGeneralView;

    TLatex *tex1, *tex2, *tex3, *tex4;

    // Unpack
    TH1F *fh1_trigger, *fh1_wr[2];
    TH1F* fh1_GeneralView;
    TH1F* fh1_wrs[6];
    THStack* stack_wrs;
    TH1F* fh1_display;
    TGraph* gh;

  public:
    ClassDef(R3BSofStatusOnlineSpectra, 0)
};

#endif
