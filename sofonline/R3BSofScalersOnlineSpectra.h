// ------------------------------------------------------------
// ----             R3BSofScalersOnlineSpectra            -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

#ifndef R3BSofScalersOnlineSpectra_H
#define R3BSofScalersOnlineSpectra_H

#include "FairTask.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH1I.h"
#include "TH2F.h"
#include "TMath.h"
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#define NbScalers 14
#define NbChannelsPerScaler \
    {                       \
      7, 56 , 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16   \
    }

class TClonesArray;
class R3BEventHeader;

/**
 * This taks reads SCI data and plots online histograms
 */
class R3BSofScalersOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofScalersOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofScalersOnlineSpectra(const char* name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofScalersOnlineSpectra();

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

    void SetReadTrloii(Bool_t trloii) { read_trloii = trloii; }
    Bool_t GetReadTrloii() { return read_trloii; }

  private:
    TClonesArray* fMappedItemsScalers; /**< Array with mapped items. */
    TClonesArray* fMappedItemsTrloii;

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header.      */
    Int_t fNEvents, fNSpill; /**< Event counter.     */
    Bool_t read_trloii;
    ULong64_t fTrloii[16][16] = {{0}}, fScaler[8] = {0}, fcounts[10] = {0};
    Double_t frate[10] = {0.};

    // Canvas
    TCanvas* cScalersGeneralView[NbScalers], *cRate;

    // Histograms for Mapped data : accumulate statistics per channel
    TH1D* fh1_GeneralView[NbScalers], *h_RatePerSpill;

  public:
    ClassDef(R3BSofScalersOnlineSpectra, 1)
};

#endif
