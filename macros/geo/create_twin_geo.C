//
//   ----- General Macro for R3B simulation
//
//         Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
//
//         Last Update: 08/12/17 (Jose Luis)
//
//         Comments:
//       

#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"
#include "TGeoBBox.h"

// Create Matrix Unity
TGeoRotation *fGlobalRot = new TGeoRotation();

// Create a null translation
TGeoTranslation *fGlobalTrans = new TGeoTranslation();
TGeoRotation *fRefRot = NULL;

TGeoManager*  gGeoMan = NULL;

Double_t fThetaX = 0.;
Double_t fThetaY = 0.;
Double_t fThetaZ = 0.;
Double_t fPhi   = 0.;
Double_t fTheta = 0.;
Double_t fPsi   = 0.;
Double_t fX = 0.;
Double_t fY = 0.;
Double_t fZ = 0.;
Bool_t fLocalTrans = kFALSE;
Bool_t fLabTrans = kTRUE;


TGeoCombiTrans* GetGlobalPosition(TGeoCombiTrans *fRef);


void create_twin_geo(const char* geoTag = "twin")
{

  fGlobalTrans->SetTranslation(0.0,0.0,0.0);

  // -------   Load media from media file   -----------------------------------
  FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media_r3b.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;
  // --------------------------------------------------------------------------



  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = geoPath + "/sofia/geometry/twinmusic_";
  geoFileName = geoFileName + geoTag + ".geo.root";
  // --------------------------------------------------------------------------



  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* mAir      = geoMedia->getMedium("Air");
  if ( ! mAir ) Fatal("Main", "FairMedium Air not found");
  geoBuild->createMedium(mAir);
  TGeoMedium* pMedAir = gGeoMan->GetMedium("Air");
  if ( ! pMedAir ) Fatal("Main", "Medium Air not found");

  FairGeoMedium* mVac      = geoMedia->getMedium("vacuum");
  if ( ! mVac ) Fatal("Main", "FairMedium vacuum not found");
  geoBuild->createMedium(mVac);
  TGeoMedium* pMedVac = gGeoMan->GetMedium("vacuum");
  if ( ! pMedVac ) Fatal("Main", "Medium vacuum not found");
  
  FairGeoMedium* mSi      = geoMedia->getMedium("silicon");
  if ( ! mSi ) Fatal("Main", "FairMedium silicon not found");
  geoBuild->createMedium(mSi);
  TGeoMedium* pMedSi = gGeoMan->GetMedium("silicon");
  if ( ! pMedSi ) Fatal("Main", "Medium silicon not found");

  FairGeoMedium* mCopper      = geoMedia->getMedium("copper");
  if ( ! mCopper ) Fatal("Main", "FairMedium copper not found");
  geoBuild->createMedium(mCopper);
  TGeoMedium* pMedCu = gGeoMan->GetMedium("copper");
  if ( ! pMedCu ) Fatal("Main", "Medium copper not found");
  
  FairGeoMedium* mAl      = geoMedia->getMedium("aluminium");
  if ( ! mAl ) Fatal("Main", "FairMedium aluminium not found");
  geoBuild->createMedium(mAl);
  TGeoMedium* pMedAl = gGeoMan->GetMedium("aluminium");
  if ( ! pMedAl ) Fatal("Main", "Medium aluminium not found");

  FairGeoMedium* mFe      = geoMedia->getMedium("iron");
  if ( ! mFe ) Fatal("Main", "FairMedium iron not found");
  geoBuild->createMedium(mFe);
  TGeoMedium* pMedFe = gGeoMan->GetMedium("iron");
  if ( ! pMedFe ) Fatal("Main", "Medium iron not found");

  FairGeoMedium* mAr      = geoMedia->getMedium("ArCO2");
  if ( ! mAr ) Fatal("Main", "FairMedium ArCO2 not found");
  geoBuild->createMedium(mAr);
  TGeoMedium* pMedAr = gGeoMan->GetMedium("ArCO2");
  if ( ! pMedAr ) Fatal("Main", "Medium ArCO2 not found");

  // --------------------------------------------------------------------------


  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("TRAgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  // --------------------------------------------------------------------------

	// out-of-file geometry definition
	Double_t dx,dy,dz;
	Double_t rmin, rmax, rmin1, rmax1, rmin2, rmax2;
	Double_t thx, phx, thy, phy, thz, phz;
	Double_t  phi1, phi2;
	Double_t tx,ty,tz;
  
	TGeoRotation * zeroRot = new TGeoRotation; //zero rotation
	TGeoCombiTrans * tZero = new TGeoCombiTrans("tZero", 0., 0., 0., zeroRot);
	tZero->RegisterYourself();
  
  
	//-------------------------------------------------------------------
  
	// Fill Chamber: Vacuum or Air. Needed still: an external call interface for choosing which.
	TGeoMedium * pMedFill=pMedVac;
  //pMedFill = new TGeoMedium("Fill_Air", numed,pMat2, par);
  //pMedFill = (TGeoMedium*) pMedAir->Clone();
  //pMedFill->SetName("Fill_Air");
//  pMedFill = (TGeoMedium*) pMedVac->Clone();
//  pMedFill->SetName("Fill_Vacuum");
  
	//-------------------------------------------------------------------
   
  
	// Shape: World type: TGeoBBox
	//TGeoVolume* pWorld = gGeoManager->GetTopVolume();
	//pWorld->SetVisLeaves(kTRUE);

	
	TGeoVolume *pAWorld  =  gGeoManager->GetTopVolume();

        TGeoRotation *rot_twin = new TGeoRotation("TWINrot");
        rot_twin->RotateY(0.0);
    
        // Defintion of the Mother Volume
	
	//Double_t length = 54.;
	
	TGeoShape *pCBWorld = new TGeoBBox("TWIN_box",
									   25.0/2.0,
									   25.0/2.0,
									   43.0/2.0);
	
	TGeoVolume*
	pWorld  = new TGeoVolume("TWINWorld",pCBWorld, pMedAr);
	
	TGeoCombiTrans *t0 = new TGeoCombiTrans(0.,0.0,88.,rot_twin);
	TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);
	
	// add the sphere as Mother Volume
	pAWorld->AddNodeOverlap(pWorld, 0, pGlobalc);

  // pWorld->SetVisLeaves(kTRUE);
  // pWorld->SetLineColor(3);
  
  
	//**************************************************************//
	//*********************   Si Sensors ******************************//
	//***************************************************************/
  
	// Si Shape & volume: TraBox type: TGeoBBox
	dx = 10./2.;
	dy = 20./2.;
	dz = 40./2.;
	// Volume: TraLog
	TGeoVolume *TraLog = gGeoManager->MakeBox("TwinLog1",pMedAr,dx,dy,dz);
        TraLog->SetLineColor(2);

	TGeoVolume *TraLog1 = gGeoManager->MakeBox("TwinLog2",pMedAr,dx,dy,dz);
        TraLog1->SetLineColor(2);
  
	TGeoRotation *rotg = new TGeoRotation();
	rotg->RotateX(0.);
	rotg->RotateY(0.);
	rotg->RotateZ(0.);

	//TRANSFORMATION MATRICES
	//Combi transformation:
	dx = 0.0;
	dy = 0.0;
	dz = 0.0;
	
	TGeoCombiTrans*   pMatrix1 = new TGeoCombiTrans("", dx,dy,dz,rotg); 

	dx = -5.0501;
	dy = 0.000000;
	dz = 0.0000;
	
	TGeoCombiTrans*  pMatrix2 = new TGeoCombiTrans("", dx,dy,dz,rotg);
  
	//Combi transformation:
	dx = 5.0501;
	dy = 0.000000;
	dz = 0.0000;
	
	TGeoCombiTrans*   pMatrix4 = new TGeoCombiTrans("", dx,dy,dz,rotg);  


   dx = 0.10;
   dy = 20.0;
   dz = 40.5;
   TGeoShape *twin1 = new TGeoBBox("", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   twin_log = new TGeoVolume("cathode",twin1, pMedCu);
   twin_log->SetVisLeaves(kTRUE);
   twin_log->SetLineColor(3);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal1 = GetGlobalPosition(pMatrix1);

   pWorld->AddNode(twin_log, 0, pGlobal1);
   pWorld->AddNode(TraLog, 0, pMatrix2);
   pWorld->AddNode(TraLog1, 0, pMatrix4);

  
	/************ Assembling everything together ****************/
/*
	TGeoVolume *aTra = new TGeoVolumeAssembly("ATRA");

	aTra->AddNode(twin_log,1, pMatrix1);
	aTra->AddNode(TraLog,1, pMatrix2);
	aTra->AddNode(TraLog,2, pMatrix4);
  
	dx=tx=0.0;
	dy=ty=0.0;
	dz=tz=88.0;
  
	TGeoCombiTrans *t0 = new TGeoCombiTrans(tx,ty,tz,rotg);
	pWorld->AddNode(aTra,1, GetGlobalPosition(t0));
*/
   
  // ---------------   Finish   -----------------------------------------------
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
  // --------------------------------------------------------------------------
}



TGeoCombiTrans* GetGlobalPosition(TGeoCombiTrans *fRef)
{
  if (fLocalTrans == kTRUE ) {
    
    if ( ( fThetaX == 0 )  && ( fThetaY==0 )  && ( fThetaZ == 0 )
	 &&
	 ( fX == 0 ) && ( fY == 0 ) && ( fZ == 0 )
	 )  return fRef;
    
    
    // X axis
    Double_t xAxis[3] = { 1. , 0. , 0. };
    Double_t yAxis[3] = { 0. , 1. , 0. };
    Double_t zAxis[3] = { 0. , 0. , 1. };
    // Reference Rotation
    fRefRot = fRef->GetRotation();
    
    if (fRefRot) {
      Double_t mX[3] = {0.,0.,0.};
      Double_t mY[3] = {0.,0.,0.};
      Double_t mZ[3] = {0.,0.,0.};
      
      fRefRot->LocalToMasterVect(xAxis,mX);
      fRefRot->LocalToMasterVect(yAxis,mY);
      fRefRot->LocalToMasterVect(zAxis,mZ);
      
      Double_t a[4]={ mX[0],mX[1],mX[2], fThetaX };
      Double_t b[4]={ mY[0],mY[1],mY[2], fThetaY };
      Double_t c[4]={ mZ[0],mZ[1],mZ[2], fThetaZ };
      
      ROOT::Math::AxisAngle aX(a,a+4);
      ROOT::Math::AxisAngle aY(b,b+4);
      ROOT::Math::AxisAngle aZ(c,c+4);
      
      ROOT::Math::Rotation3D fMatX( aX );
      ROOT::Math::Rotation3D fMatY( aY );
      ROOT::Math::Rotation3D fMatZ( aZ );
      
      ROOT::Math::Rotation3D  fRotXYZ = (fMatZ * (fMatY * fMatX));
      
      //cout << fRotXYZ << endl;
      
      Double_t fRotable[9]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
      fRotXYZ.GetComponents(
			    fRotable[0],fRotable[3],fRotable[6],
			    fRotable[1],fRotable[4],fRotable[7],
			    fRotable[2],fRotable[5],fRotable[8]
			    );
      TGeoRotation *pRot = new TGeoRotation();
      pRot->SetMatrix(fRotable);
      TGeoCombiTrans *pTmp = new TGeoCombiTrans(*fGlobalTrans,*pRot);
      
      // ne peut pas etre applique ici
      // il faut differencier trans et rot dans la multi.
      TGeoRotation rot_id;
      rot_id.SetAngles(0.0,0.0,0.0);
      
      TGeoCombiTrans c1;
      c1.SetRotation(rot_id);
      const Double_t *t = pTmp->GetTranslation();
      c1.SetTranslation(t[0],t[1],t[2]);
      
      TGeoCombiTrans c2;
      c2.SetRotation(rot_id);
      const Double_t *tt = fRefRot->GetTranslation();
      c2.SetTranslation(tt[0],tt[1],tt[2]);
      
      TGeoCombiTrans cc = c1 * c2 ;
      
      TGeoCombiTrans c3;
      c3.SetRotation(pTmp->GetRotation());
      TGeoCombiTrans c4;
      c4.SetRotation(fRefRot);
      
      TGeoCombiTrans ccc = c3 * c4;
      
      TGeoCombiTrans pGlobal;
      pGlobal.SetRotation(ccc.GetRotation());
      const Double_t *allt = cc.GetTranslation();
      pGlobal.SetTranslation(allt[0],allt[1],allt[2]);
      
      return  ( new TGeoCombiTrans( pGlobal ) );
      
    }else{
      
      cout << "-E- R3BDetector::GetGlobalPosition() \
	      No. Ref. Transformation defined ! " << endl;
      cout << "-E- R3BDetector::GetGlobalPosition() \
	      cannot create Local Transformation " << endl;
      return NULL;
    } //! fRefRot
    
  } else {
    // Lab Transf.
    if ( ( fPhi == 0 )  && ( fTheta==0 )  && ( fPsi == 0 )
	 &&
	 ( fX == 0 ) && ( fY == 0 ) && ( fZ == 0 )
	 )  return fRef;
    
    
    return ( new TGeoCombiTrans(*fGlobalTrans,*fGlobalRot) );
    
  }
}

