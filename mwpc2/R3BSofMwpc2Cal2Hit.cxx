// -------------------------------------------------------------------------
// -----         R3BSofMwpc2Cal2Hit source file                        -----
// -----             Created 11/10/19  by J.L. Rodriguez-Sanchez       -----
// -------------------------------------------------------------------------

//ROOT headers
#include "TClonesArray.h"
#include "TMath.h"

//Fair headers
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include <iomanip>

//MWPC headers
#include "R3BSofMwpc0CalData.h"
#include "R3BSofMwpc0HitData.h"
#include "R3BSofMwpc2Cal2Hit.h"

//R3BSofMwpc2Cal2Hit: Default Constructor --------------------------
R3BSofMwpc2Cal2Hit::R3BSofMwpc2Cal2Hit() : 
  FairTask("R3B Hit-MWPC2 Task",1),
  fMwpcCalDataCA(NULL),
  fMwpcHitDataCA(NULL),
  fwx(3.125),// in mm
  fwy(5.000),// in mm
  fSize(200.0),// in mm
  fOnline(kFALSE)
{
}

//R3BSofMwpc2Cal2Hit: Standard Constructor --------------------------
R3BSofMwpc2Cal2Hit::R3BSofMwpc2Cal2Hit(const char* name, Int_t iVerbose) :
  FairTask(name, iVerbose),
  fMwpcCalDataCA(NULL),
  fMwpcHitDataCA(NULL),
  fwx(3.125),// in mm
  fwy(5.000),// in mm
  fSize(200.0),// in mm
  fOnline(kFALSE)
{
}

//Virtual R3BSofMwpc2Cal2Hit: Destructor
R3BSofMwpc2Cal2Hit::~R3BSofMwpc2Cal2Hit()
{
  LOG(INFO) << "R3BSofMwpc2Cal2Hit: Delete instance";
  if(fMwpcCalDataCA) delete fMwpcCalDataCA;
  if(fMwpcHitDataCA) delete fMwpcHitDataCA;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofMwpc2Cal2Hit::Init()
{
  LOG(INFO) << "R3BSofMwpc2Cal2Hit: Init";

  //INPUT DATA
  FairRootManager* rootManager = FairRootManager::Instance();
  if (!rootManager) { return kFATAL;}
  
  fMwpcCalDataCA = (TClonesArray*)rootManager->GetObject("Mwpc2CalData");
  if (!fMwpcCalDataCA) { return kFATAL;}
   
  //OUTPUT DATA
  //Hit data
  fMwpcHitDataCA = new TClonesArray("R3BSofMwpc0HitData",10);

  if(!fOnline){
  rootManager->Register("Mwpc2HitData", "MWPC2 Hit", fMwpcHitDataCA, kTRUE);
  }else{
  rootManager->Register("Mwpc2HitData", "MWPC2 Hit", fMwpcHitDataCA, kFALSE);
  }

  return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofMwpc2Cal2Hit::ReInit()
{
  return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofMwpc2Cal2Hit::Exec(Option_t* option)
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
  Int_t padmxu=-1,padmxd=-1, padmy=-1;
  Int_t q=0,qmxu=0,qmxd=0,qleft=0,qright=0;
  Int_t qmy=0,qdown=0,qup=0;
  Double_t x=0.,y=0.;

  for(Int_t i = 0; i < NbPadsX; i++)fx[i]=0;
  for(Int_t i = 0; i < NbPadsY; i++)fy[i]=0;

  for(Int_t i = 0; i < nHits; i++) {
    calData[i] = (R3BSofMwpc0CalData*)(fMwpcCalDataCA->At(i));
    planeId = calData[i]->GetPlane();
    padId = calData[i]->GetPad();
    q = calData[i]->GetQ();
    if(planeId==1)
      fx[padId]=q;
    else if(planeId==2)
      fx[padId+NbPadsX/2]=q;
    else 
      fy[padId]=q;
  }
  //Add Hit data ----
  if(padmxu>-1&&padmy>-1){
  //TODO: Look for right combinations
   AddHitData(x,y);
  }


  if(calData) delete calData;
  return;
}

// -----   Protected method to obtain the position X ----------------------------
Double_t R3BSofMwpc2Cal2Hit::GetPostionX(Int_t qmax, Int_t padmax, Int_t qleft, Int_t qright)
{
 Double_t a3= TMath::Pi()*fwx/(TMath::ACosH(0.5*(TMath::Sqrt(qmax/qleft)+TMath::Sqrt(qmax/qright))));
 Double_t a2= (a3/TMath::Pi()) * TMath::ATanH( (TMath::Sqrt(qmax/qleft)-TMath::Sqrt(qmax/qright)) / (2*TMath::SinH(TMath::Pi()*fwx/a3)) );

 return (-1.*padmax*fwx+(fSize/2)-(fwx/2)-a2);//Left is positive and right negative
}

// -----   Protected method to obtain the position Y ----------------------------
Double_t R3BSofMwpc2Cal2Hit::GetPostionY(Int_t qmax, Int_t padmax, Int_t qdown, Int_t qup)
{
 Double_t a3= TMath::Pi()*fwy/(TMath::ACosH(0.5*(TMath::Sqrt(qmax/qdown)+TMath::Sqrt(qmax/qup))));
 Double_t a2= (a3/TMath::Pi()) * TMath::ATanH( (TMath::Sqrt(qmax/qdown)-TMath::Sqrt(qmax/qup)) / (2*TMath::SinH(TMath::Pi()*fwy/a3)) );

 return (padmax*fwy-(fSize/2)+(fwy/2)+a2);

}

// -----   Public method Finish  ------------------------------------------------
void R3BSofMwpc2Cal2Hit::Finish()
{
}

// -----   Public method Reset   ------------------------------------------------
void R3BSofMwpc2Cal2Hit::Reset()
{
  LOG(DEBUG) << "Clearing Mwpc2HitData Structure";
  if(fMwpcHitDataCA)fMwpcHitDataCA->Clear();
}


// -----   Private method AddHitData  --------------------------------------------
R3BSofMwpc0HitData* R3BSofMwpc2Cal2Hit::AddHitData(Double_t x, Double_t y)
{
  //It fills the R3BSofMwpc0HitData
  TClonesArray& clref = *fMwpcHitDataCA;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) R3BSofMwpc0HitData(x,y);
}

ClassImp(R3BSofMwpc2Cal2Hit)
