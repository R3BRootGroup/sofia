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


void create_tofsofia_geo(const char* geoTag="v0")
{

// --------------------------------------------------------------------------
// Configurable geometry for the SOFIA ToF-Wall.
// Use this macro to create root files with the different configurations 
// and positions/angles of the detector.
//
// Execute macro:  root -l
//                 .L create_tofsofia_geo.C
//                 create_tofsofia_geo()
// --------------------------------------------------------------------------

   Double_t dx,dy,dz;
   Double_t a;
   Double_t thx, phx, thy, phy, thz, phz;
   Double_t tx,ty,tz;
   Double_t z, density, w;
   Int_t nel, numed;
   fGlobalTrans->SetTranslation(0.0,0.0,0.0);

   TGeoRotation * zeroRot = new TGeoRotation; //zero rotation
   TGeoCombiTrans * tZero = new TGeoCombiTrans("tZero", 0., 0., 0., zeroRot);
   tZero->RegisterYourself();

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
  TString geoFileName = geoPath + "/sofia/geometry/sof_tof_";
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

/*
  // Mixture: plasticForTOF
  TGeoMedium * pMed34=NULL;
   if (gGeoManager->GetMedium("plasticForTOF") ){
       pMed34=gGeoManager->GetMedium("plasticForTOF");
   }else{
     nel     = 2;
     density = 1.032000;
     TGeoMixture*
	 pMat34 = new TGeoMixture("plasticForTOF", nel,density);
     a = 12.010700;   z = 6.000000;   w = 0.914708;  // C
     pMat34->DefineElement(0,a,z,w);
     a = 1.007940;   z = 1.000000;   w = 0.085292;  // H
     pMat34->DefineElement(1,a,z,w);
     pMat34->SetIndex(33);
     // Medium: plasticForTOF
     numed   = 33;  // medium number
     Double_t par[8];
     par[0]  = 0.000000; // isvol
     par[1]  = 0.000000; // ifield
     par[2]  = 0.000000; // fieldm
     par[3]  = 0.000000; // tmaxfd
     par[4]  = 0.000000; // stemax
     par[5]  = 0.000000; // deemax
     par[6]  = 0.000100; // epsil
     par[7]  = 0.000000; // stmin
     pMed34 = new TGeoMedium("plasticForTOF", numed,pMat34,par);
   }
*/

  // --------------------------------------------------------------------------



  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("TRAgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  // --------------------------------------------------------------------------


/*
        // WORLD
	TGeoVolume *pAWorld  =  gGeoManager->GetTopVolume();
    
        // Defintion of the Mother Volume
	Double_t lengthX = 110.;
        Double_t lengthY = 110.;
        Double_t lengthZ =  4.;
	
	TGeoShape *pCBWorld = new TGeoBBox("FFs_TOF_box",
							lengthX/2.0,
							lengthY/2.0,
							lengthZ/2.0);
	
	TGeoVolume*
	pWorld  = new TGeoVolume("FFsTOF_World",pCBWorld, pMedVac);

        // Position ToF: Translation and rotation
        Double_t PositionX = -120.7-60.-15.-45-16.;
        Double_t PositionY = 0.;
        Double_t PositionZ = 690.+20.;
        TGeoRotation *rot_tof = new TGeoRotation("Tofrot");
        rot_tof->RotateY(-45.0);

	TGeoCombiTrans *t0 = new TGeoCombiTrans(PositionX, PositionY, PositionZ, rot_tof);
	TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);
	
	// Add the box as Mother Volume
	pAWorld->AddNodeOverlap(pWorld, 0, pGlobalc);
*/

   // Shape: World type: TGeoBBox
   TGeoVolume* pWorld = gGeoManager->GetTopVolume();
   pWorld->SetVisLeaves(kTRUE);

  
   // Shape: TOFBox type: TGeoBBox
   dx = 81.0000;
   dy = 60.0000;
   dz = 0.500000;
   TGeoShape *pTOFBox = new TGeoBBox("TOFBox", dx/2.0,dy/2.0,dz/2.0);
   // Volume: TOFLog
   TGeoVolume*
   pTOFLog = new TGeoVolume("TOF_FFs",pTOFBox, pMedAir);
   pTOFLog->SetVisLeaves(kTRUE);
   pTOFLog->SetLineColor(kBlue-8);

   // SHAPES, VOLUMES AND GEOMETRICAL HIERARCHY
   // Shape: TargetEnveloppe type: TGeoTubeSeg
   Double_t rmin = 0.0000;
   Double_t rmax = 1.35;
   dz   = 12.00000;
   Double_t phi1 = 0.000000;
   Double_t phi2 = 360.0000;
   TGeoShape *pPM = new TGeoTubeSeg("PM",rmin,rmax,dz/2.,phi1,phi2);

   TGeoRotation *rot_PM = new TGeoRotation("PMrot");
   rot_PM->RotateX(90.0);

   TGeoVolume* pPM_nbu[27];
   TGeoCombiTrans* pGlobalPMsu[27];
   TGeoCombiTrans* pMatrixPMsu[27];

   TGeoVolume* pPM_nbd[27];
   TGeoCombiTrans* pGlobalPMsd[27];
   TGeoCombiTrans* pMatrixPMsd[27];

   char buf[126];

   for(int i=0;i<27;i++){

   sprintf(buf,"PM_up_%i",i+1);

   dx=3.*i-39.;
   dy=35.2;
   dz=0.00;

   pMatrixPMsu[i] = new TGeoCombiTrans("", dx,dy,dz,rot_PM);
     

   pPM_nbu[i] = new TGeoVolume(buf,pPM, pMedVac);
   pPM_nbu[i]->SetVisLeaves(kTRUE);
   pPM_nbu[i]->SetLineColor(kGray);

  
   pGlobalPMsu[i] = GetGlobalPosition(pMatrixPMsu[i]);

   //pWorld->AddNode(pPM_nbu[i], i, pGlobalPMsu[i] );


   sprintf(buf,"PM_down_%i",i+1);

   dx=3.*i-39.;
   dy=-36.5;
   dz=0.00;

   pMatrixPMsd[i] = new TGeoCombiTrans("", dx,dy,dz,rot_PM);

   pPM_nbd[i] = new TGeoVolume(buf,pPM, pMedVac);
   pPM_nbd[i]->SetVisLeaves(kTRUE);
   pPM_nbd[i]->SetLineColor(kGray);

   pGlobalPMsd[i] = GetGlobalPosition(pMatrixPMsd[i]);
   //pWorld->AddNode(pPM_nbd[i], i, pGlobalPMsd[i] );


   }


    dx = 100.0;
    dy = 4.0;
    dz = 5.0;
    TGeoShape * plane = new TGeoBBox("plane1", dx/2.,dy/2.,dz/2.);
    TGeoVolume* subplane = new TGeoVolume("",plane, pMedVac);
    subplane->SetVisLeaves(kTRUE);
    subplane->SetLineColor(5);

    dx=0.00;
    dy=43.5;
    dz=0.00;
    TGeoCombiTrans* pMatrix3 = new TGeoCombiTrans("", dx,dy,dz,0);
    TGeoCombiTrans* pGlobal3 = GetGlobalPosition(pMatrix3);
   // pWorld->AddNode(subplane, 0, pGlobal3 );

    dx=0.00;
    dy=-43.5;
    dz=0.00;
    TGeoCombiTrans* pMatrix4 = new TGeoCombiTrans("", dx,dy,dz,0);
    TGeoCombiTrans* pGlobal4 = GetGlobalPosition(pMatrix4);
    //pWorld->AddNode(subplane, 0, pGlobal4 );



   
    dx = 3.8;
    dy = 165.0;
    dz = 5.0;
    TGeoShape * plane2 = new TGeoBBox("plane2", dx/2.,dy/2.,dz/2.);
    TGeoVolume* subplane2 = new TGeoVolume("",plane2, pMedVac);
    subplane2->SetVisLeaves(kTRUE);
    subplane2->SetLineColor(5);

    dx=52.00;
    dy=-36.00;
    dz=0.00;
    TGeoCombiTrans* pMatrix5 = new TGeoCombiTrans("", dx,dy,dz,0);
    TGeoCombiTrans* pGlobal5 = GetGlobalPosition(pMatrix5);
    //pWorld->AddNode(subplane2, 0, pGlobal5 );

    dx=-52.00;
    dy=-36.00;
    dz=0.00;
    TGeoCombiTrans* pMatrix6 = new TGeoCombiTrans("", dx,dy,dz,0);
    TGeoCombiTrans* pGlobal6 = GetGlobalPosition(pMatrix6);
    //pWorld->AddNode(subplane2, 0, pGlobal6 );
  


  /************ Assembling everything together ****************/
  TGeoVolume *Tof = new TGeoVolumeAssembly("SOFIATOFWALL");
  Tof->AddNode(pTOFLog, 1, 0 );


  TGeoRotation *rotg = new TGeoRotation();
  rotg->RotateX(0.);
  rotg->RotateY(0.);
  rotg->RotateZ(0.);
  dx=tx=0.0;
  dy=ty=0.0;
  dz=tz=0.0;
  
  TGeoCombiTrans *t_zero = new TGeoCombiTrans("t_zero");

  TGeoCombiTrans *t0 = new TGeoCombiTrans(tx,ty,tz,rotg);
  pWorld->AddNode(Tof, 1, GetGlobalPosition(t0));
  
   
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


