// -------------------------------------------------------------------------
// -----         R3BSofMwpc0Mapped2Cal source file                     -----
// -----             Created 07/10/19  by J.L. Rodriguez-Sanchez       -----
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
#include "R3BSofMwpc0MappedData.h"
#include "R3BSofMwpc0Mapped2Cal.h"
#include "R3BSofMwpc0CalPar.h"

//R3BSofMwpc0Mapped2Cal: Default Constructor --------------------------
R3BSofMwpc0Mapped2Cal::R3BSofMwpc0Mapped2Cal() : 
  FairTask("R3B MWPC0 Calibrator",1),
  NumPadX(0),
  NumPadY(0),
  CalParams(NULL),
  fCal_Par(NULL),
  fMwpcMappedDataCA(NULL),
  fMwpcCalDataCA(NULL),
  fOnline(kFALSE)
{
}

//R3BSofMwpc0Mapped2CalPar: Standard Constructor --------------------------
R3BSofMwpc0Mapped2Cal::R3BSofMwpc0Mapped2Cal(const char* name, Int_t iVerbose) :
  FairTask(name, iVerbose),
  NumPadX(0),
  NumPadY(0),
  CalParams(NULL),
  fCal_Par(NULL),
  fMwpcMappedDataCA(NULL),
  fMwpcCalDataCA(NULL),
  fOnline(kFALSE)
{  
}

//Virtual R3BSofMwpc0Mapped2Cal: Destructor
R3BSofMwpc0Mapped2Cal::~R3BSofMwpc0Mapped2Cal()
{
  LOG(INFO) << "R3BSofMwpc0Mapped2Cal: Delete instance";
  if(fMwpcMappedDataCA) delete fMwpcMappedDataCA;
  if(fMwpcCalDataCA) delete fMwpcCalDataCA;
}


void R3BSofMwpc0Mapped2Cal::SetParContainers() {
  
  //Parameter Container
  //Reading amsStripCalPar from FairRuntimeDb
  FairRuntimeDb* rtdb = FairRuntimeDb::instance();
  if (!rtdb) { 
    LOG(ERROR)<<"FairRuntimeDb not opened!";
  }
  
  fCal_Par=(R3BSofMwpc0CalPar*)rtdb->getContainer("mwpc0CalPar");
  if (!fCal_Par) {
    LOG(ERROR)<<"R3BSofMwpc0Mapped2CalPar::Init() Couldn't get handle on mwpc0CalPar container";
  }
  else{
    LOG(INFO)<<"R3BSofMwpc0Mapped2CalPar:: mwpc0CalPar container open";
  }
}


void R3BSofMwpc0Mapped2Cal::SetParameter(){
  
  //--- Parameter Container ---
  NumPadX = fCal_Par->GetNumPadsX();//Number of Detectors
  NumPadY = fCal_Par->GetNumPadsY();//Number of Strips
  NumParams=fCal_Par->GetNumParametersFit();//Number of Parameters

  LOG(INFO)<<"R3BSofMwpc0Mapped2Cal: NumPadX: "<< NumPadX;
  LOG(INFO)<<"R3BSofMwpc0Mapped2Cal: NumPadY: "<< NumPadY;
  
  CalParams= new TArrayF();
  Int_t array_size = (NumPadX+NumPadY)*NumParams;
  CalParams->Set(array_size);	
  CalParams=fCal_Par->GetMwpwCalParams();//Array with the Cal parameters

}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofMwpc0Mapped2Cal::Init()
{
  LOG(INFO) << "R3BSofMwpc0Mapped2Cal: Init";

  //INPUT DATA
  FairRootManager* rootManager = FairRootManager::Instance();
  if (!rootManager) { return kFATAL;}
  
  fMwpcMappedDataCA = (TClonesArray*)rootManager->GetObject("Mwpc0MappedData");
  if (!fMwpcMappedDataCA) { return kFATAL;}
   
  //OUTPUT DATA
  //Calibrated data
  fMwpcCalDataCA = new TClonesArray("R3BSofMwpc0CalData",10);

  if(!fOnline){
  rootManager->Register("Mwpc0CalData", "MWPC0 Cal", fMwpcCalDataCA, kTRUE);
  }else{
  rootManager->Register("Mwpc0CalData", "MWPC0 Cal", fMwpcCalDataCA, kFALSE);
  }

  SetParameter();
  return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofMwpc0Mapped2Cal::ReInit()
{
  SetParContainers();
  return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofMwpc0Mapped2Cal::Exec(Option_t* option)
{
  //Reset entries in output arrays, local arrays
  Reset();
  
  if (!fCal_Par) {
    LOG(ERROR)<<"NO Container Parameter!!";
  }  
  
  //Reading the Input -- Mapped Data --
  Int_t nHits = fMwpcMappedDataCA->GetEntries();
  if(nHits!=(NumPadX+NumPadY) && nHits>0)LOG(WARNING) << "R3BSofMwpc0Mapped2Cal: nHits!=(NumPadX+NumPadY)";
  if(!nHits) return;
  
  R3BSofMwpc0MappedData** mappedData;
  mappedData=new R3BSofMwpc0MappedData*[nHits];
  UChar_t planeId;
  UChar_t padId;
  Double_t charge;
  Double_t pedestal=0.;

  for(Int_t i = 0; i < nHits; i++) {
    mappedData[i] = (R3BSofMwpc0MappedData*)(fMwpcMappedDataCA->At(i));
    planeId = mappedData[i]->GetPlane();
    padId = mappedData[i]->GetPad();

    if(planeId==1)
     pedestal=CalParams->GetAt(padId);
    else if(planeId==3)
     pedestal=CalParams->GetAt(NumPadX+padId);
    else LOG(ERROR)<<"Plane "<<planeId<<" does not exist in MWPC0";

    charge = mappedData[i]->GetEnergy()-pedestal;

    //We accept the hit if the energy is larger than zero
    if(energy>0){
     AddCalData(planeId,padId,charge);
    }
  }

  if(mappedData) delete mappedData;
  return;
}

// -----   Protected method Finish   --------------------------------------------
void R3BSofMwpc0Mapped2Cal::Finish()
{
  
}

// -----   Public method Reset   ------------------------------------------------
void R3BSofMwpc0Mapped2Cal::Reset()
{
  LOG(DEBUG) << "Clearing Mwpc0CalData Structure";
  if(fMwpcCalDataCA)fMwpcCalDataCA->Clear();
}


// -----   Private method AddCalData  --------------------------------------------
R3BAmsStripCalData* R3BSofMwpc0Mapped2Cal::AddCalData(UChar_t plane, UChar_t pad, UShort_t charge)
{
  //It fills the R3BSofMwpc0CalData
  TClonesArray& clref = *fMwpcCalDataCA;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) R3BSofMwpc0CalData(plane,pad,charge);
}

ClassImp(R3BSofMwpc0Mapped2Cal)
