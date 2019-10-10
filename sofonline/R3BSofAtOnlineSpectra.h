// ------------------------------------------------------------
// -----                  R3BSofAtOnlineSpectra           -----
// -----    Created 29/09/19  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

#ifndef R3BSofAtOnlineSpectra_H
#define R3BSofAtOnlineSpectra_H

#include "FairTask.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <array>
#include "TMath.h"
#include <cstdlib>
#include "TH1.h"
#include "TH2F.h"
#include "TCanvas.h"

class TClonesArray;
class R3BEventHeader;

/**
 * This taks reads FRS data and plots online histograms 
 */
class R3BSofAtOnlineSpectra : public FairTask {
  
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


  private:
  
  TClonesArray* fMappedItemsAt;         /**< Array with mapped items. */
  
  // check for trigger should be done globablly (somewhere else)
  R3BEventHeader* header;               /**< Event header.      */
  Int_t fNEvents;        	   	/**< Event counter.     */
  Int_t fReset;

  //Canvas
  TCanvas* cAtMap;

  //Histograms for Mapped data
  TH2F* fh2_atmap[3];
  TH1F* fh1_atmapevt;

  //Histograms for cal data
  //TH2F* fh2_atcal[3];
  
  public:
  ClassDef(R3BSofAtOnlineSpectra, 0)
};

#endif
