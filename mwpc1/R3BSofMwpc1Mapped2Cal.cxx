// ----------------------------------------------------------------------
// -----		                R3BSofMwpc1Mapped2Cal 			            -----
// -----          Created 15/10/19  by G. García Jiménez            -----
// -----  by modifying J.L. Rodriguez-Sanchez  classes for Mwpc2    -----
// ----------------------------------------------------------------------


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
#include "R3BSofMwpcMappedData.h"
#include "R3BSofMwpcCalData.h"
#include "R3BSofMwpc1Mapped2Cal.h"
#include "R3BSofMwpc1CalPar.h"



//R3BSofMwpc1Mapped2Cal: Default Constructor --------------------------
R3BSofMwpc1Mapped2Cal::R3BSofMwpc1Mapped2Cal() :
  FairTask("R3B MWPC1 Calibrator",1),
  NumPadX(0),
  NumPadY(0),
  NumParams(0),
  CalParams(NULL),
  fCal_Par(NULL),
  fMwpcMappedDataCA(NULL),
  fMwpcCalDataCA(NULL),
  fOnline(kFALSE)
{
}


//R3BSofMwpc1Mapped2Cal: Standard Constructor --------------------------
R3BSofMwpc1Mapped2Cal::R3BSofMwpc1Mapped2Cal(const char* name, Int_t iVerbose) :
  FairTask(name, iVerbose),
  NumPadX(0),
  NumPadY(0),
  NumParams(0),
  CalParams(NULL),
  fCal_Par(NULL),
  fMwpcMappedDataCA(NULL),
  fMwpcCalDataCA(NULL),
  fOnline(kFALSE)
{
}


//Virtual R3BSofMwpc1Mapped2Cal: Destructor
R3BSofMwpc1Mapped2Cal::~R3BSofMwpc1Mapped2Cal()
{
  LOG(INFO) << "R3BSofMwpc1Mapped2Cal: Delete instance";
  if(fMwpcMappedDataCA) delete fMwpcMappedDataCA;
  if(fMwpcCalDataCA) delete fMwpcCalDataCA;
}



void R3BSofMwpc1Mapped2Cal::SetParContainers() {

  //Parameter Container
  //Reading PadCalPar from FairRuntimeDb
  FairRuntimeDb* rtdb = FairRuntimeDb::instance();
  if (!rtdb) {
    LOG(ERROR)<<"FairRuntimeDb not opened!";
  }

  fCal_Par=(R3BSofMwpc1CalPar*)rtdb->getContainer("mwpc1CalPar");
  if (!fCal_Par) {
    LOG(ERROR)<<"R3BSofMwpc1Mapped2Cal::Init() Couldn't get handle on mwpc1CalPar container";
  }
  else{
    LOG(INFO)<<"R3BSofMwpc1Mapped2Cal:: mwpc1CalPar container open";
  }
}



void R3BSofMwpc1Mapped2Cal::SetParameter(){

  //--- Parameter Container ---
  NumPadX = fCal_Par->GetNumPadsX();//Number of Pads in X
  NumPadY = fCal_Par->GetNumPadsY();//Number of Pads in Y
  NumParams=fCal_Par->GetNumParametersFit();//Number of parameters in the Fit

  LOG(INFO)<<"R3BSofMwpc1Mapped2Cal: NumPadX: "<< NumPadX;
  LOG(INFO)<<"R3BSofMwpc1Mapped2Cal: NumPadY: "<< NumPadY;
  LOG(INFO)<<"R3BSofMwpc1Mapped2Cal: Number of fit parameters: "<< NumParams;

  CalParams= new TArrayI();
  Int_t array_size = (NumPadX+NumPadY)*NumParams;
  CalParams->Set(array_size);
  CalParams=fCal_Par->GetPadCalParams();//Array with the Cal parameters
}




// -----   Public method Init   --------------------------------------------
InitStatus R3BSofMwpc1Mapped2Cal::Init()
{
  LOG(INFO) << "R3BSofMwpc1Mapped2Cal: Init";

  //INPUT DATA
  FairRootManager* rootManager = FairRootManager::Instance();
  if (!rootManager) { return kFATAL;}

  fMwpcMappedDataCA = (TClonesArray*)rootManager->GetObject("Mwpc1MappedData");
  if (!fMwpcMappedDataCA) { return kFATAL;}

  //OUTPUT DATA
  //Calibrated data
  fMwpcCalDataCA = new TClonesArray("R3BSofMwpcCalData",10);

  if(!fOnline){
  rootManager->Register("Mwpc1CalData", "MWPC1 Cal", fMwpcCalDataCA, kTRUE);
  }else{
  rootManager->Register("Mwpc1CalData", "MWPC1 Cal", fMwpcCalDataCA, kFALSE);
  }

  SetParameter();
  return kSUCCESS;
}


// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofMwpc1Mapped2Cal::ReInit()
{
  SetParContainers();
  return kSUCCESS;
}


// -----   Public method Execution   --------------------------------------------
void R3BSofMwpc1Mapped2Cal::Exec(Option_t* option)
{
  //Reset entries in output arrays, local arrays
  Reset();

  if (!fCal_Par) {
    LOG(ERROR)<<"NO Container Parameter!, pedestals will be set to zero";
  }

  //Reading the Input -- Mapped Data --
  Int_t nHits = fMwpcMappedDataCA->GetEntries();
  if(nHits>(NumPadX+NumPadY) && nHits>0)LOG(WARNING) << "R3BSofMwpc1Mapped2Cal: nHits>(NumPadX+NumPadY)";
  if(!nHits) return;

  R3BSofMwpcMappedData** mappedData;
  mappedData=new R3BSofMwpcMappedData*[nHits];
  UChar_t planeId;
  UChar_t padId;
  UShort_t charge;
  UShort_t pedestal=0;
  Int_t nbpad=0;

  for(Int_t i = 0; i < nHits; i++) {
    mappedData[i] = (R3BSofMwpcMappedData*)(fMwpcMappedDataCA->At(i));
    planeId = mappedData[i]->GetPlane();
    padId = mappedData[i]->GetPad();
    if(planeId==1)//Xdown
     nbpad=padId*NumParams;
    else if(planeId==2)//Xup
     nbpad=(padId+NumPadX/2)*NumParams;
    else if(planeId==3)//Y
     nbpad=(padId+NumPadX)*NumParams;
    else LOG(ERROR)<<"Plane "<<planeId<<" does not exist in MWPC1";

    pedestal=CalParams->GetAt(nbpad);
    charge = mappedData[i]->GetQ()-pedestal;

    //We accept the hit if the charge is larger than zero
    if(charge>0){
     AddCalData(planeId,padId,charge);
    }
  }
  if(mappedData) delete mappedData;
  return;
}



// -----   Protected method Finish   --------------------------------------------
void R3BSofMwpc1Mapped2Cal::Finish()
{
}

// -----   Public method Reset   ------------------------------------------------
void R3BSofMwpc1Mapped2Cal::Reset()
{
  LOG(DEBUG) << "Clearing Mwpc1CalData Structure";
  if(fMwpcCalDataCA)fMwpcCalDataCA->Clear();
}




// -----   Private method AddCalData  --------------------------------------------
R3BSofMwpcCalData* R3BSofMwpc1Mapped2Cal::AddCalData(UChar_t plane, UChar_t pad, UShort_t charge)
{
  //It fills the R3BSofMwpcCalData
  TClonesArray& clref = *fMwpcCalDataCA;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) R3BSofMwpcCalData(plane,pad,charge);
}

ClassImp(R3BSofMwpc1Mapped2Cal)
