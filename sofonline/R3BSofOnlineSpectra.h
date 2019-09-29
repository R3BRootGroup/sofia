// ------------------------------------------------------------
// -----                  R3BSofOnlineSpectra             -----
// -----    Created 29/09/19  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

#ifndef R3BSofOnlineSpectra_H
#define R3BSofOnlineSpectra_H

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
class R3BSofOnlineSpectra : public FairTask {
  
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
  R3BSofOnlineSpectra(const char* name, Int_t iVerbose = 1);
  
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
  void Reset_GENERAL_Histo();

  
  private:
  
//  TClonesArray* fMappedItemsFrs;     /**< Array with mapped items.           */

  
  // check for trigger should be done globablly (somewhere else)
  R3BEventHeader* header;               /**< Event header.      */
  Int_t fTrigger;                       /**< Trigger value.     */
  Int_t fNEvents;        	   	/**< Event counter.     */

  
  //Canvas


  
  public:
  ClassDef(R3BSofOnlineSpectra, 1)
};

#endif
