#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

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

TGeoCombiTrans* GetGlobalPosition(TGeoCombiTrans *fRef);

void create_at_geo(const char* geoTag)
{
    // -------   Load media from media file   -----------------------------------
    FairGeoLoader* geoLoad = new FairGeoLoader("TGeo", "FairGeoLoader");
    FairGeoInterface* geoFace = geoLoad->getGeoInterface();
    TString geoPath = gSystem->Getenv("VMCWORKDIR");
    TString medFile = geoPath + "/sofia/geometry/media_r3b.geo";
    geoFace->setMediaFile(medFile);
    geoFace->readMedia();
    gGeoMan = gGeoManager;
    // --------------------------------------------------------------------------

    // -------   Geometry file name (output)   ----------------------------------
    TString geoFileName = geoPath + "/sofia/geometry/sof_at_";
    geoFileName = geoFileName + geoTag + ".geo.root";
    // --------------------------------------------------------------------------

    // -----------------   Get and create the required media    -----------------
    FairGeoMedia* geoMedia = geoFace->getMedia();
    FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

    FairGeoMedium* mAir = geoMedia->getMedium("Air");
    if (!mAir) Fatal("Main", "FairMedium Air not found");
    geoBuild->createMedium(mAir);
    TGeoMedium* pMed2 = gGeoMan->GetMedium("Air");
    if (!pMed2) Fatal("Main", "Medium Air not found");

    FairGeoMedium* mAl = geoMedia->getMedium("aluminium");
    if (!mAl) Fatal("Main", "FairMedium aluminium not found");
    geoBuild->createMedium(mAl);
    TGeoMedium* pMed3 = gGeoMan->GetMedium("aluminium");
    if (!pMed3) Fatal("Main", "Medium aluminium not found");

    FairGeoMedium* mMylar      = geoMedia->getMedium("mylar");
    if ( ! mMylar ) Fatal("Main", "FairMedium mylar not found");
    geoBuild->createMedium(mMylar);
    TGeoMedium* pMed4 = gGeoMan->GetMedium("mylar");
    if ( ! pMed4 ) Fatal("Main", "Medium mylar not found");

    FairGeoMedium* mAr      = geoMedia->getMedium("ArCO2");
    if ( ! mAr ) Fatal("Main", "FairMedium ArCO2 not found");
    geoBuild->createMedium(mAr);
    TGeoMedium* pMed5 = gGeoMan->GetMedium("ArCO2");
    if ( ! pMed5 ) Fatal("Main", "Medium ArCO2 not found");

    FairGeoMedium* mC      = geoMedia->getMedium("carbon");
    if ( ! mC ) Fatal("Main", "FairMedium carbon not found");
    geoBuild->createMedium(mC);
    TGeoMedium* pMed6 = gGeoMan->GetMedium("carbon");
    if ( ! pMed6 ) Fatal("Main", "Medium carbon not found");

    FairGeoMedium* mU      = geoMedia->getMedium("uranium");
    if ( ! mU ) Fatal("Main", "FairMedium uranium not found");
    geoBuild->createMedium(mU);
    TGeoMedium* pMed7 = gGeoMan->GetMedium("uranium");
    if ( ! pMed7 ) Fatal("Main", "Medium uranium not found");

    // --------------------------------------------------------------------------

    // --------------   Create geometry and top volume  -------------------------
    gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
    gGeoMan->SetName("SOFATgeom");
    TGeoVolume* top = new TGeoVolumeAssembly("TOP");
    gGeoMan->SetTopVolume(top);
    // --------------------------------------------------------------------------
    

  // Defintion of the Mother Volume
  TGeoShape *pVCTube = new TGeoTube("atbox1", 0., 10., 20.2/2.0);
  TGeoCombiTrans *t_tube = new TGeoCombiTrans("t_box1",0.,0.,0.,fRefRot);
  t_tube->RegisterYourself();

  TGeoShape *pbox2 = new TGeoBBox("atbox2", 3/2.0, 3./2.0, 0.1/2.0);
  TGeoCombiTrans *t_box2 = new TGeoCombiTrans("t_box2",0.,0.,-20.2/2.0-0.1/2.0,fRefRot);
  t_box2->RegisterYourself();
  
  TGeoCompositeShape *pMWorld = new TGeoCompositeShape("ATbox", "atbox1:t_box1 + atbox2:t_box2");
  
  TGeoVolume* pWorld = new TGeoVolume("ATWorld", pMWorld, pMed2);
  
  TGeoCombiTrans *t0 = new TGeoCombiTrans();
  TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);
  
  // Adding the Mother Volume
  top->AddNode(pWorld, 0, pGlobalc);

  Double_t dx, dy, dz;
  Double_t thx, thy, thz;
  Double_t phx, phy, phz;

  TGeoVolumeAssembly *AtDet = new TGeoVolumeAssembly("AtDet");

  Double_t parA[3] = {0.,10.,(5./2.-0.06/2.)};
  TGeoTube *pA = new TGeoTube(parA);
  TGeoVolume* pALog = new TGeoVolume("SOFATLog",pA, pMed5);
  pALog->SetVisLeaves(kTRUE);
  pALog->SetLineColor(4);

  dx = 0.000000;
  dy = 0.000000;
  TGeoRotation *rot0 = new TGeoRotation();
  dz = -7.5;
  TGeoCombiTrans* pMatrix0 = new TGeoCombiTrans("",dx,dy,dz,rot0);
  AtDet->AddNode(pALog,0,pMatrix0);
  dz = -2.5;
  TGeoCombiTrans* pMatrix1 = new TGeoCombiTrans("",dx,dy,dz,rot0);
  AtDet->AddNode(pALog,1,pMatrix1);
  dz = 2.5;
  TGeoCombiTrans* pMatrix2 = new TGeoCombiTrans("",dx,dy,dz,rot0);
  AtDet->AddNode(pALog,2,pMatrix2);
  dz = 7.5;
  TGeoCombiTrans* pMatrix3 = new TGeoCombiTrans("",dx,dy,dz,rot0);
  AtDet->AddNode(pALog,3,pMatrix3);

  dx = 0.000;
  dy = 0.000;
  dz = 0.000;
  TGeoRotation *arot0 = new TGeoRotation();
  TGeoCombiTrans* pMatrixd100 = new TGeoCombiTrans("",dx,dy,dz,arot0);
  pWorld->AddNode(AtDet,0,pMatrixd100);

  // Windows, Shape & volume: TGeoTube
  dz = 0.0025;
  TGeoVolume *Wind = gGeoManager->MakeTube("Window",pMed4,0.,5.,dz/2.);
  Wind->SetVisLeaves(kTRUE);
  Wind->SetLineColor(13);

  dz = -10.+0.06/2.-0.0025/2.;
  TGeoRotation *arot3 = new TGeoRotation();
  TGeoCombiTrans* pMatrixd3 = new TGeoCombiTrans("",dx,dy,dz,arot3);
  pWorld->AddNode(Wind,0,pMatrixd3);

  dz = 10.-0.06/2.+0.0025/2.;
  TGeoCombiTrans* pMatrixd4 = new TGeoCombiTrans("",dx,dy,dz,arot3);
  pWorld->AddNode(Wind,1,pMatrixd4);

  // Targets, Shape & volume: TGeoTube
  dz = 0.060;
  TGeoVolume *UT = gGeoManager->MakeTube("Utarget",pMed7,0.,7.,dz/2.);
  UT->SetVisLeaves(kTRUE);
  UT->SetLineColor(5);

  dz = -5.0;
  TGeoCombiTrans* pMatrixd5 = new TGeoCombiTrans("",dx,dy,dz,arot3);
  pWorld->AddNode(UT,0,pMatrixd5);

  dz = 0.0;
  TGeoCombiTrans* pMatrixd6 = new TGeoCombiTrans("",dx,dy,dz,arot3);
  pWorld->AddNode(UT,1,pMatrixd6);

  dz = 0.060;
  TGeoVolume *CT = gGeoManager->MakeTube("Ctarget",pMed6,0.,7.,dz/2.);
  CT->SetVisLeaves(kTRUE);
  CT->SetLineColor(1);

  dz = 5.0;
  TGeoCombiTrans* pMatrixd7 = new TGeoCombiTrans("",dx,dy,dz,arot3);
  pWorld->AddNode(CT,0,pMatrixd7);


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
