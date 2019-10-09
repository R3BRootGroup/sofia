// -------------------------------------------------------------------------
// -----         R3BSofMwpc0Cal2Hit source file                        -----
// -----             Created 09/10/19  by J.L. Rodriguez-Sanchez       -----
// -------------------------------------------------------------------------

//ROOT headers
#include "TClonesArray.h"
#include "TMath.h"
#include "TRandom.h"

//Fair headers
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include <iomanip>

//MWPC headers
#include "R3BSofMwpc0CalData.h"
#include "R3BSofMwpc0HitData.h"
#include "R3BSofMwpc0Cal2Hit.h"

//R3BSofMwpc0Cal2Hit: Default Constructor --------------------------
R3BSofMwpc0Cal2Hit::R3BSofMwpc0Cal2Hit() : 
  FairTask("R3B Hit-MWPC0 Task",1),
  fMwpcCalDataCA(NULL),
  fMwpcHitDataCA(NULL),
  fOnline(kFALSE)
{
}

//R3BSofMwpc0Cal2Hit: Standard Constructor --------------------------
R3BSofMwpc0Cal2Hit::R3BSofMwpc0Cal2Hit(const char* name, Int_t iVerbose) :
  FairTask(name, iVerbose),
  fMwpcCalDataCA(NULL),
  fMwpcHitDataCA(NULL),
  fOnline(kFALSE)
{  
}

//Virtual R3BSofMwpc0Cal2Hit: Destructor
R3BSofMwpc0Cal2Hit::~R3BSofMwpc0Cal2Hit()
{
  LOG(INFO) << "R3BSofMwpc0Cal2Hit: Delete instance";
  if(fMwpcCalDataCA) delete fMwpcCalDataCA;
  if(fMwpcHitDataCA) delete fMwpcHitDataCA;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofMwpc0Cal2Hit::Init()
{
  LOG(INFO) << "R3BSofMwpc0Cal2Hit: Init";

  //INPUT DATA
  FairRootManager* rootManager = FairRootManager::Instance();
  if (!rootManager) { return kFATAL;}
  
  fMwpcCalDataCA = (TClonesArray*)rootManager->GetObject("Mwpc0CalData");
  if (!fMwpcCalDataCA) { return kFATAL;}
   
  //OUTPUT DATA
  //Hit data
  fMwpcHitDataCA = new TClonesArray("R3BSofMwpc0HitData",10);

  if(!fOnline){
  rootManager->Register("Mwpc0HitData", "MWPC0 Hit", fMwpcHitDataCA, kTRUE);
  }else{
  rootManager->Register("Mwpc0HitData", "MWPC0 Hit", fMwpcHitDataCA, kFALSE);
  }

  return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofMwpc0Cal2Hit::ReInit()
{
  return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofMwpc0Cal2Hit::Exec(Option_t* option)
{
  //Reset entries in output arrays, local arrays
  Reset();
  
  //Reading the Input -- Cal Data --
  Int_t nHits = fMwpcCalDataCA->GetEntries();
  if(!nHits) return;
  
  // Data from cal level
  R3BSofMwpc0CalData** calData;
  calData=new R3BSofMwpc0CalData*[nHits];
  UChar_t planeId;
  UChar_t padId;
  UShort_t q;
  Double_t x=0.,y=0.;

  for(Int_t i = 0; i < nHits; i++) {
    calData[i] = (R3BSofMwpc0CalData*)(fMwpcCalDataCA->At(i));
    planeId = calData[i]->GetPlane();
    padId = calData[i]->GetPad();
    q = calData[i]->GetQ();

    //FIXME: put conditions!!
     AddHitData(x,y);

  }

  if(calData) delete calData;
  return;
}

// -----   Protected method Finish   --------------------------------------------
void R3BSofMwpc0Cal2Hit::Finish()
{
  
}

// -----   Public method Reset   ------------------------------------------------
void R3BSofMwpc0Cal2Hit::Reset()
{
  LOG(DEBUG) << "Clearing Mwpc0HitData Structure";
  if(fMwpcHitDataCA)fMwpcHitDataCA->Clear();
}


// -----   Private method AddHitData  --------------------------------------------
R3BSofMwpc0HitData* R3BSofMwpc0Cal2Hit::AddHitData(Double_t x, Double_t y)
{
  //It fills the R3BSofMwpc0HitData
  TClonesArray& clref = *fMwpcHitDataCA;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) R3BSofMwpc0HitData(x,y);
}

ClassImp(R3BSofMwpc0Cal2Hit)
