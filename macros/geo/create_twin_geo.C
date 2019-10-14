//
//   ----- General Macro for R3B simulation
//
//         Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
//
//         Last Update: 29/09/19 (Jose Luis)
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


void create_twin_geo(const char* geoTag = "v0")
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

  FairGeoMedium* mMylar      = geoMedia->getMedium("mylar");
  if ( ! mMylar ) Fatal("Main", "FairMedium mylar not found");
  geoBuild->createMedium(mMylar);
  TGeoMedium* pMed4 = gGeoMan->GetMedium("mylar");
  if ( ! pMed4 ) Fatal("Main", "Medium mylar not found");

  // --------------------------------------------------------------------------


  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("TWIMgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  // --------------------------------------------------------------------------

  // out-of-file geometry definition
  Double_t dx,dy,dz;
  TGeoRotation *rotg = new TGeoRotation();
  TGeoCombiTrans*
  pMatrix0 = new TGeoCombiTrans("",dx,dy,dz,rotg);

  // Defintion of the Mother Volume
  TGeoShape *pbox1 = new TGeoBBox("twimbox1", 12./2.0, 23./2.0, 22.1);
  TGeoCombiTrans *t_box1 = new TGeoCombiTrans("t_box1",6.,0.,0.,fRefRot);
  t_box1->RegisterYourself();

  TGeoShape *pbox2 = new TGeoBBox("twimbox2", 12./2.0, 23./2.0, 22.1);
  TGeoCombiTrans *t_box2 = new TGeoCombiTrans("t_box2",-6.,0.,0.,fRefRot);
  t_box2->RegisterYourself();
  
  TGeoCompositeShape *pMWorld = new TGeoCompositeShape("TWIMbox", "twimbox1:t_box1 + twimbox2:t_box2");
  
  TGeoVolume* pWorld = new TGeoVolume("TWIMWorld", pMWorld, pMedAr);
  
  TGeoCombiTrans *t0 = new TGeoCombiTrans();
  TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);
  
  // Adding the Mother Volume
  top->AddNode(pWorld, 0, pGlobalc);


  // --------------- Detector --------------------------------------

  TGeoVolumeAssembly *twim = new TGeoVolumeAssembly("TWIMDet");

  // SHAPES, VOLUMES AND GEOMETRICAL HIERARCHY
  // Shape: Anode type: TGeoBBox
  dx = 11.000;
  dy = 11.000;
  dz = 2.5000;
  Float_t xcathode=0.05;//mm
  TGeoShape *AnodeBox = new TGeoBBox("TWIMAnode", dx/2.,dy/2.,dz/2.);
  // Volume: TOFLog
  TGeoVolume*
  AnodeLog = new TGeoVolume("TwinLog",AnodeBox,pMedAr);
  AnodeLog->SetVisLeaves(kTRUE);
  AnodeLog->SetLineColor(2);

  Int_t nbSections=4;
  Int_t nbAnodes=16;
  TGeoCombiTrans* pGlobalAnode[nbSections*nbAnodes];
  TGeoCombiTrans* pMatrixAnode[nbSections*nbAnodes];

  Int_t nbanode=0;
  Float_t offsetx=11./2.+xcathode/2.;
  Float_t secposx[4]={-offsetx,-offsetx,offsetx,offsetx};
  Float_t secposy[4]={11./2.,-11./2.,-11./2.,11./2.};
  for(Int_t i=0;i<nbSections;i++){//Sections
   dx=secposx[i];
   dy=secposy[i];
   for(Int_t j=0;j<nbAnodes;j++){//Anodes per section
   
   nbanode=i*nbAnodes+j;
   dz=2.5*(-7.5+j);

   pMatrixAnode[nbanode] = new TGeoCombiTrans("", dx,dy,dz,rotg);
   pGlobalAnode[nbanode] = GetGlobalPosition(pMatrixAnode[nbanode]);
   twim->AddNode(AnodeLog, nbanode, pGlobalAnode[nbanode]);
   }
  }

   dx = xcathode;
   dy = 22.0;
   dz = nbAnodes*2.5+2.*2.;
   TGeoShape *twin1 = new TGeoBBox("", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   twin_log = new TGeoVolume("cathode",twin1, pMedCu);
   twin_log->SetVisLeaves(kTRUE);
   twin_log->SetLineColor(3);

   // Position Mother Volume
   TGeoCombiTrans *pMatrix1 = new TGeoCombiTrans("", 0.,0.,0.,rotg);
   TGeoCombiTrans* pGlobal1 = GetGlobalPosition(pMatrix1);
   twim->AddNode(twin_log, 0, pGlobal1);

  TGeoCombiTrans *pGlobal = GetGlobalPosition(pMatrix0);
  if (pGlobal){
    pWorld->AddNode(twim, 0, pGlobal);
  }else{
    pWorld->AddNode(twim, 0, pMatrix0);
  }


  // Screening anodes, Shape & volume: TGeoBBox
  dx = 11.0;
  dy = 22.0;
  dz = 2.00;
  TGeoVolume *SAnode = gGeoManager->MakeBox("TwimScreening",pMedAr,dx/2.,dy/2.,dz/2.);
  SAnode->SetVisLeaves(kTRUE);
  SAnode->SetLineColor(5);

  dx = secposx[0];
  dy = 0.000;
  dz = -21.0;
  TGeoRotation *arot1 = new TGeoRotation();
  TGeoCombiTrans* pMatrixd1 = new TGeoCombiTrans("",dx,dy,dz,arot1);
  pWorld->AddNode(SAnode,0,pMatrixd1);

  dx = secposx[2];
  dy = 0.000;
  dz = -21.0;
  TGeoCombiTrans* pMatrixd1b = new TGeoCombiTrans("",dx,dy,dz,arot1);
  pWorld->AddNode(SAnode,1,pMatrixd1b);


  dx = secposx[0];
  dz = 21.0;
  TGeoRotation *arot2 = new TGeoRotation();
  TGeoCombiTrans* pMatrixd2 = new TGeoCombiTrans("",dx,dy,dz,arot2);
  pWorld->AddNode(SAnode,2,pMatrixd2);

  dx = secposx[2];
  dz = 21.0;
  TGeoCombiTrans* pMatrixd2b = new TGeoCombiTrans("",dx,dy,dz,arot2);
  pWorld->AddNode(SAnode,3,pMatrixd2b);


  // Windows, Shape & volume: TGeoBBox
  dx = 22.0;
  dy = 22.0;
  dz = 0.0025;
  TGeoVolume *Wind = gGeoManager->MakeBox("TwimWindow",pMed4,dx/2.,dy/2.,dz/2.);
  Wind->SetVisLeaves(kTRUE);
  Wind->SetLineColor(13);

  dx = 0.000;
  dy = 0.000;
  dz = -22.0-0.0025/2.;
  TGeoRotation *arot3 = new TGeoRotation();
  TGeoCombiTrans* pMatrixd3 = new TGeoCombiTrans("",dx,dy,dz,arot3);
  pWorld->AddNode(Wind,0,pMatrixd3);

  dz = 22.0+0.0025/2.;
  TGeoRotation *arot4 = new TGeoRotation();
  TGeoCombiTrans* pMatrixd4 = new TGeoCombiTrans("",dx,dy,dz,arot4);
  pWorld->AddNode(Wind,1,pMatrixd4);


   
  // ---------------   Finish   -----------------------------------------------
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
  std::cout << "Creating geometry: "<<geoFileName<< std::endl;
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

