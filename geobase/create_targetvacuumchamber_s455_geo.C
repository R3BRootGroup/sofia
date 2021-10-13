//
//   ----- General Macro for R3B-SOFIA simulations
//
//         Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
//
//         Last Update: 02/10/19 (Jose Luis)
//
//         Comments:
//

#include "TGeoManager.h"
#include "TMath.h"
#include <iomanip>
#include <iostream>

// Create Matrix Unity
TGeoRotation* fGlobalRot = new TGeoRotation();

// Create a null translation
TGeoTranslation* fGlobalTrans = NULL;
TGeoRotation* fRefRot = NULL;
TGeoManager* gGeoMan = NULL;

Double_t fThetaX = 0.;
Double_t fThetaY = 0.;
Double_t fThetaZ = 0.;
Double_t fPhi = 0.;
Double_t fTheta = 0.;
Double_t fPsi = 0.;
Double_t fX = 0.;
Double_t fY = 0.;
Double_t fZ = 0.;
Bool_t fLocalTrans = kFALSE;

TGeoCombiTrans* GetGlobalPosition(TGeoCombiTrans* fRef);

void create_targetvacuumchamber_geo(const char* geoTag = "s455")
{
    // --------------------------------------------------------------------------
    // Configurable geometry for the target vacuum chamber according to the
    // e-mail on 1st October 2019.
    // Use this macro to create root files with the different configurations
    // and positions of the vacuum chamber.
    //
    // Execute macro:  root -l
    //                 .L create_targetvacuumchamber_s455_geo.C
    //                 create_targetvacuumchamber_geo()
    // --------------------------------------------------------------------------

    fGlobalTrans = new TGeoTranslation();
    fGlobalTrans->SetTranslation(0.0, 0.0, 0.0);

    // -------   Load media from media file   -----------------------------------
    FairGeoLoader* geoLoad = new FairGeoLoader("TGeo", "FairGeoLoader");
    FairGeoInterface* geoFace = geoLoad->getGeoInterface();
    TString geoPath = gSystem->Getenv("VMCWORKDIR");
    TString medFile = geoPath + "/geometry/media_r3b.geo";
    geoFace->setMediaFile(medFile);
    geoFace->readMedia();
    gGeoMan = gGeoManager;
    // --------------------------------------------------------------------------

    // -------   Geometry file name (output)   ----------------------------------
    TString geoFileName = geoPath + "/geometry/targetvacuumchamber_";
    geoFileName = geoFileName + geoTag + ".geo.root";
    // --------------------------------------------------------------------------

    // -----------------   Get and create the required media    -----------------
    FairGeoMedia* geoMedia = geoFace->getMedia();
    FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

    FairGeoMedium* mVac = geoMedia->getMedium("vacuum");
    if (!mVac)
        Fatal("Main", "FairMedium vacuum not found");
    geoBuild->createMedium(mVac);
    TGeoMedium* pMedVac = gGeoMan->GetMedium("vacuum");
    if (!pMedVac)
        Fatal("Main", "Medium vacuum not found");

    FairGeoMedium* mCu = geoMedia->getMedium("copper");
    if (!mCu)
        Fatal("Main", "FairMedium copper not found");
    geoBuild->createMedium(mCu);
    TGeoMedium* pMed1 = gGeoMan->GetMedium("copper");
    if (!pMed1)
        Fatal("Main", "Medium copper not found");

    FairGeoMedium* mAl = geoMedia->getMedium("aluminium");
    if (!mAl)
        Fatal("Main", "FairMedium aluminium not found");
    geoBuild->createMedium(mAl);
    TGeoMedium* pMed2 = gGeoMan->GetMedium("aluminium");
    if (!pMed2)
        Fatal("Main", "Medium aluminium not found");

    FairGeoMedium* mSteel = geoMedia->getMedium("Steel");
    if (!mSteel)
        Fatal("Main", "FairMedium Steel not found");
    geoBuild->createMedium(mSteel);
    TGeoMedium* pMed3 = gGeoMan->GetMedium("Steel");
    if (!pMed3)
        Fatal("Main", "Medium Steel not found");

    FairGeoMedium* mMylar = geoMedia->getMedium("mylar");
    if (!mMylar)
        Fatal("Main", "FairMedium mylar not found");
    geoBuild->createMedium(mMylar);
    TGeoMedium* pMed4 = gGeoMan->GetMedium("mylar");
    if (!pMed4)
        Fatal("Main", "Medium mylar not found");

    FairGeoMedium* mH2 = geoMedia->getMedium("H2");
    if (!mH2)
        Fatal("Main", "FairMedium H2 not found");
    geoBuild->createMedium(mH2);
    TGeoMedium* pMed5 = gGeoMan->GetMedium("H2");
    if (!pMed5)
        Fatal("Main", "Medium H2 not found");

    // --------------------------------------------------------------------------

    // --------------   Create geometry and top volume  -------------------------
    gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
    gGeoMan->SetName("VACCHAMBERgeom");
    TGeoVolume* top = new TGeoVolumeAssembly("TOP");
    gGeoMan->SetTopVolume(top);
    // --------------------------------------------------------------------------

    // Defintion of the Mother Volume
    TGeoShape* pVCTube = new TGeoTube("CenterPart", 0., 29.26, 442. / 2.0);

    TGeoCombiTrans* t_tube = new TGeoCombiTrans("t_tube", 0., 0., 0., fRefRot);
    t_tube->RegisterYourself();

    TGeoShape* pVCCone = new TGeoCone("Cone", (33.688 + 4.) / 2., 0., 4.6, 0., 7.8);

    TGeoCombiTrans* t_cone = new TGeoCombiTrans("t_cone", 0., 0., 442. / 2.0 + (33.688 + 4.) / 2., fRefRot);
    t_cone->RegisterYourself();

    TGeoCompositeShape* pVCWorld = new TGeoCompositeShape("VCbox", "CenterPart:t_tube + Cone:t_cone");

    TGeoVolume* pWorld = new TGeoVolume("VCWorld", pVCWorld, pMedVac);

    TGeoCombiTrans* t0 = new TGeoCombiTrans();
    TGeoCombiTrans* pGlobalc = GetGlobalPosition(t0);

    // add the sphere as Mother Volume
    top->AddNode(pWorld, 0, pGlobalc);

    Double_t dx, dy, dz;
    Double_t thx, thy, thz;
    Double_t phx, phy, phz;

    // TGeoVolumeAssembly *top1 = new TGeoVolumeAssembly("VACCHAMBER");

    Double_t offsetZ = -0.50; // offset with respect to LH2 target cell, entrance at (0,0,0)
    // Mylar windows for LH2 target
    Double_t thinMylar1 = 0.012 / 2.0;            // 120micra entrance window
    Double_t thinMylar2 = 0.018 / 2.0;            // 180micra exit window
    Double_t LH2targetR = 1.5 - thinMylar2 * 2.0; // Radius of 1.5 cm - mylar cylindre
    Double_t LH2targetL = 1.5;                    // Length 1.5cm

    // chamber
    Double_t parChamberBarrel[3] = { 26.1, 26.4, (46.3 / 2.) };
    TGeoTube* pChamberBarrel = new TGeoTube(parChamberBarrel);
    TGeoVolume* pChamberBarrelLog = new TGeoVolume("ChamberBarrelLog", pChamberBarrel, pMed2);
    pChamberBarrelLog->SetVisLeaves(kTRUE);
    pChamberBarrelLog->SetLineColor(34);

    dx = 0.000000;
    dy = 0.000000;
    dz = -42.1250 + 1.7 + 46.3 / 2. + offsetZ;
    TGeoRotation* rot0 = new TGeoRotation();
    TGeoCombiTrans* pMatrix169 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberBarrelLog, 0, pMatrix169);

    // chamber Inner
    Double_t parChamberBarrelIn[3] = { 9.625, (9.625 + 2.2), (37.57 / 4.) };
    TGeoTube* pChamberBarrelIn = new TGeoTube(parChamberBarrelIn);
    TGeoVolume* pChamberBarrelInLog = new TGeoVolume("ChamberBarrelInLog", pChamberBarrelIn, pMed2);
    pChamberBarrelInLog->SetVisLeaves(kTRUE);
    pChamberBarrelInLog->SetLineColor(34);

    dx = 0.000000;
    dy = 0.000000;
    dz = -37.54000 - 37.57 / 4. + offsetZ;
    TGeoCombiTrans* pMatrix170 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberBarrelInLog, 0, pMatrix170);

    // Inner support
    Double_t parChamberIn[3] = { 0., 0.6, ((37.57 - 0.9) / 2.) };
    TGeoTube* pChamberIn = new TGeoTube(parChamberIn);
    TGeoVolume* pChamberInLog = new TGeoVolume("ChamberInLog", pChamberIn, pMed2);
    pChamberInLog->SetVisLeaves(kTRUE);
    pChamberInLog->SetLineColor(34);

    dx = 9.6 - 0.6;
    dy = 0.000000;
    dz = -37.54000 + 0.45 + offsetZ;
    TGeoCombiTrans* pMatrix100 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberInLog, 0, pMatrix100);

    dx = -(9.6 - 0.6);
    dy = 0.000000;
    dz = -37.54000 + 0.45 + offsetZ;
    TGeoCombiTrans* pMatrix101 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberInLog, 1, pMatrix101);

    dy = -(9.6 - 0.6);
    dx = 0.000000;
    dz = -37.54000 + 0.45 + offsetZ;
    TGeoCombiTrans* pMatrix102 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberInLog, 2, pMatrix102);

    dy = (9.6 - 0.6);
    dx = 0.000000;
    dz = -37.54000 + 0.45 + offsetZ;
    TGeoCombiTrans* pMatrix103 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberInLog, 3, pMatrix103);

    Double_t parChamberIn2[3] = { 7.6, 9.625, (0.9 / 2.) };
    TGeoTube* pChamberIn2 = new TGeoTube(parChamberIn2);
    TGeoVolume* pChamberIn2Log = new TGeoVolume("ChamberIn2Log", pChamberIn2, pMed2);
    pChamberIn2Log->SetVisLeaves(kTRUE);
    pChamberIn2Log->SetLineColor(17);

    dx = 0.000000;
    dy = 0.000000;
    dz = -17.8550 - 0.9 - 37.57 + 0.45 + offsetZ;
    TGeoCombiTrans* pMatrix104 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberIn2Log, 3, pMatrix104);

    Double_t parChamberEndPlate[3] = { (9.625 + 2.2), 29.25, 1.7 };
    TGeoTube* pChamberEndPlate = new TGeoTube(parChamberEndPlate);
    TGeoVolume* pChamberEndPlateLog = new TGeoVolume("ChamberEndPlateLog", pChamberEndPlate, pMed3);
    pChamberEndPlateLog->SetVisLeaves(kTRUE);
    pChamberEndPlateLog->SetLineColor(34);

    dx = 0.000000;
    dy = 0.000000;
    dz = -42.1250 + offsetZ;
    TGeoCombiTrans* pMatrix174 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberEndPlateLog, 0, pMatrix174);

    Double_t parChamberEndPlateIn[3] = { (3.8), (9.625 + 2.2), 0.45 };
    TGeoTube* pChamberEndPlateIn = new TGeoTube(parChamberEndPlateIn);
    TGeoVolume* pChamberEndPlateInLog = new TGeoVolume("ChamberEndPlateInLog", pChamberEndPlateIn, pMed2);
    pChamberEndPlateInLog->SetVisLeaves(kTRUE);
    pChamberEndPlateInLog->SetLineColor(34);

    dx = 0.000000;
    dy = 0.000000;
    dz = -17.8550 - 0.45 + offsetZ;
    TGeoCombiTrans* pMatrix175 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberEndPlateInLog, 0, pMatrix175);

    Double_t parTargetEndPlateIn[3] = { (LH2targetR + thinMylar2 * 2.0), 2.44, 0.75 };
    TGeoTube* pTargetEndPlateIn = new TGeoTube(parTargetEndPlateIn);
    TGeoVolume* pTargetEndPlateInLog = new TGeoVolume("TargetEndPlateLog", pTargetEndPlateIn, pMed1);
    pTargetEndPlateInLog->SetVisLeaves(kTRUE);
    pTargetEndPlateInLog->SetLineColor(46);

    dx = 0.000000;
    dy = 0.000000;
    dz = 0.75000 + offsetZ;
    TGeoCombiTrans* pMatrix176 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pTargetEndPlateInLog, 0, pMatrix176);

    TGeoSphere* SphereChamber = new TGeoSphere("SphereChamber", 26.1, 26.4, 15.26, 90., 0., 360.);
    // TGeoVolume* pSphereChamberLog = new TGeoVolume("pSphereChamberLog",SphereChamber, pMed2);
    // pSphereChamberLog->SetVisLeaves(kTRUE);
    // pSphereChamberLog->SetLineColor(34);

    dx = 0.000000;
    dy = 0.000000;
    dz = 6.775000 - 0.9;
    TGeoCombiTrans* pMatrix177 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    // pWorld->AddNode(pSphereChamberLog,0,pMatrix177);

    Double_t parTargetEndPlateOut[3] = { 7.0, 10., 1. };
    TGeoTube* pTargetEndPlateOut = new TGeoTube(parTargetEndPlateOut);
    TGeoVolume* pTargetEndPlateOutLog = new TGeoVolume("TargetEndPlateOutLog", pTargetEndPlateOut, pMed2);
    pTargetEndPlateOutLog->SetVisLeaves(kTRUE);
    pTargetEndPlateOutLog->SetLineColor(34);

    dx = 0.000000;
    dy = 0.000000;
    dz = 6.775000 - 0.9 + 28.812028 + 33.688 + 1. + offsetZ;
    TGeoCombiTrans* pMatrix179 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pTargetEndPlateOutLog, 0, pMatrix179);

    Double_t parRingOut[5] = { (4.301 + 2.348 + 0.1), 2.048 + 0.3, 2.348 + 0.3, 0., 360. };
    TGeoTorus* pRingOut = new TGeoTorus(parRingOut);

    Double_t parCone[5] = { (1.9), 4.601, 10., 4.601, 10. };
    TGeoCone* pCone = new TGeoCone(parCone);

    dx = 0.000000;
    dy = 0.000000;
    dz = 1.;
    TGeoCombiTrans* pMatrixSub = new TGeoCombiTrans("", dx, dy, dz, rot0);

    TGeoSubtraction* pSub1 = new TGeoSubtraction(pRingOut, pCone, 0, pMatrixSub);

    TGeoSphere* Sphere = new TGeoSphere("Sphere", 26.4, 30., 15.2, 25., 0., 360.);

    TGeoShape* pROutb = new TGeoCompositeShape("pROutb", pSub1);

    dx = 0.000000;
    dy = 0.000000;
    dz = -26.4 - 1.5;
    TGeoCombiTrans* pMatrixSub1 = new TGeoCombiTrans("", dx, dy, dz, rot0);

    TGeoSubtraction* pSub2 = new TGeoSubtraction(pROutb, Sphere, 0, pMatrixSub1);

    TGeoShape* pRingOutb1 = new TGeoCompositeShape("pRingOutb1", pSub2);

    Double_t parChamberCone[5] = { (33.688 / 2.), 4.301, 4.601, 7.0, 7.3 };
    TGeoCone* pChamberCone = new TGeoCone(parChamberCone);

    /*  TGeoVolume* pChamberConeLog = new TGeoVolume("ChamberConeLog",pChamberCone, pMed2);
      pChamberConeLog->SetVisLeaves(kTRUE);
      pChamberConeLog->SetLineColor(34);
    */
    dx = 0.000000;
    dy = 0.000000;
    dz = 6.775000 - 0.9 + 28.812028 + 33.688 / 2.0;
    TGeoCombiTrans* pMatrix178 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    /*  pWorld->AddNode(pChamberConeLog,0,pMatrix178);
     */

    dx = 0.0000;
    dy = 0.0000;
    dz = 6.775000 - 0.9 + 28.812028 - 2.348 / 2. + 0.15;
    TGeoCombiTrans* pMatrix180 = new TGeoCombiTrans("", dx, dy, dz, rot0);

    TGeoUnion* pBoolNode0 = new TGeoUnion(pChamberCone, pRingOutb1, pMatrix178, pMatrix180);

    TGeoShape* pRingOutb = new TGeoCompositeShape("pNode0", pBoolNode0);

    TGeoUnion* pBoolNode1 = new TGeoUnion(pRingOutb, SphereChamber, 0, pMatrix177);

    TGeoShape* pFrontChamber = new TGeoCompositeShape("pFrontChamber", pBoolNode1);

    TGeoVolume* pFrontChamber_log = new TGeoVolume("FrontChamber", pFrontChamber, pMed2);
    pFrontChamber_log->SetVisLeaves(kTRUE);
    pFrontChamber_log->SetLineColor(34);

    dx = 0.0000;
    dy = 0.0000;
    dz = offsetZ;
    TGeoCombiTrans* pMatrix220 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pFrontChamber_log, 0, pMatrix220);

    Double_t parCoolRingIn[5] = { 2., 0.2, 0.35, 0., 90. };
    TGeoTorus* pCoolRingIn = new TGeoTorus(parCoolRingIn);
    TGeoVolume* pCoolRingIn_log = new TGeoVolume("CoolRingIn", pCoolRingIn, pMed1);
    pCoolRingIn_log->SetVisLeaves(kTRUE);
    pCoolRingIn_log->SetLineColor(46);

    dx = 0.000000;
    dy = 2.440000;
    dz = 0.75 - 2. + offsetZ;
    TGeoRotation* rot1 = new TGeoRotation();
    rot1->RotateY(-90.0);
    TGeoCombiTrans* pMatrix5 = new TGeoCombiTrans("", dx, dy, dz, rot1);
    pWorld->AddNode(pCoolRingIn_log, 0, pMatrix5);

    Double_t parCoolRingIn2[5] = { 2., 0.2, 0.35, 270., 90. };
    TGeoTorus* pCoolRingIn2 = new TGeoTorus(parCoolRingIn2);
    TGeoVolume* pCoolRingIn2_log = new TGeoVolume("CoolRingIn2", pCoolRingIn2, pMed1);
    pCoolRingIn2_log->SetVisLeaves(kTRUE);
    pCoolRingIn2_log->SetLineColor(46);

    dx = 0.000000;
    dy = -2.44000;
    dz = 0.75 - 2. + offsetZ;
    TGeoCombiTrans* pMatrix6 = new TGeoCombiTrans("", dx, dy, dz, rot1);
    pWorld->AddNode(pCoolRingIn2_log, 0, pMatrix6);

    Double_t parCoolPipeIn[3] = { 0.2, 0.35, ((17.855 - 1.25) / 2.) };
    TGeoTube* pCoolPipeIn = new TGeoTube(parCoolPipeIn);
    TGeoVolume* pCoolPipeIn_log = new TGeoVolume("CoolPipeIn", pCoolPipeIn, pMed1);
    pCoolPipeIn_log->SetVisLeaves(kTRUE);
    pCoolPipeIn_log->SetLineColor(46);

    dx = 0.000000;
    dy = -2.44000 - 2.;
    dz = -(17.855 - 1.25) / 2. - 1.25 + offsetZ;
    TGeoCombiTrans* pMatrix7 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pCoolPipeIn_log, 0, pMatrix7);

    dy = 2.44000 + 2.;
    TGeoCombiTrans* pMatrix8 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pCoolPipeIn_log, 1, pMatrix8);

    // LH2 target --------------------------------------------------------------
    Double_t parTargetH2[3] = { 0., LH2targetR, (LH2targetL / 2.0) };
    TGeoTube* pTargetH2 = new TGeoTube(parTargetH2);
    TGeoVolume* pTargetH2Log = new TGeoVolume("TargetH2Log", pTargetH2, pMed5);
    pTargetH2Log->SetVisLeaves(kTRUE);
    pTargetH2Log->SetLineColor(3);

    dx = 0.000000;
    dy = 0.000000;
    dz = LH2targetL / 2.0;
    TGeoCombiTrans* pMatrix9 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pTargetH2Log, 0, pMatrix9);

    // Mylar windows for LH2 target

    Double_t parMylar1[3] = { 0., LH2targetR + thinMylar2 * 2., thinMylar1 };
    TGeoTube* pMylar1 = new TGeoTube(parMylar1);
    TGeoVolume* pMylar1Log = new TGeoVolume("MylarWindowEntranceLog", pMylar1, pMed4);
    pMylar1Log->SetVisLeaves(kTRUE);
    pMylar1Log->SetLineColor(12);

    dx = 0.000000;
    dy = 0.000000;
    dz = -thinMylar1;
    TGeoCombiTrans* pMatrix10 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pMylar1Log, 0, pMatrix10);

    Double_t parMylar2[3] = { 0., LH2targetR + thinMylar2 * 2., thinMylar2 };
    TGeoTube* pMylar2 = new TGeoTube(parMylar2);
    TGeoVolume* pMylar2Log = new TGeoVolume("MylarWindowExitLog", pMylar2, pMed4);
    pMylar2Log->SetVisLeaves(kTRUE);
    pMylar2Log->SetLineColor(12);
    dx = 0.000000;
    dy = 0.000000;
    dz = LH2targetL + thinMylar2;
    TGeoCombiTrans* pMatrix11 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pMylar2Log, 0, pMatrix11);

    Double_t parMylar3[3] = { LH2targetR, LH2targetR + thinMylar2 * 2., (LH2targetL / 2.0) };
    TGeoTube* pMylar3 = new TGeoTube(parMylar3);
    TGeoVolume* pMylar3Log = new TGeoVolume("MylarCylindreLog", pMylar3, pMed4);
    pMylar3Log->SetVisLeaves(kTRUE);
    pMylar3Log->SetLineColor(12);
    dx = 0.000000;
    dy = 0.000000;
    dz = LH2targetL / 2.0;
    TGeoCombiTrans* pMatrix12 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pMylar3Log, 0, pMatrix12);

    // Exit mylar window for the vacuum chamber
    Double_t parWin[3] = { 0., 7., 0.0100 / 2.0 }; // 100 micras
    TGeoTube* pWin1 = new TGeoTube(parWin);
    TGeoVolume* pWin1Log = new TGeoVolume("ExitWinLog", pWin1, pMed4);
    pWin1Log->SetVisLeaves(kTRUE);
    // pWin1Log->SetLineColor(2);

    dx = 0.000000;
    dy = 0.000000;
    dz = 6.775000 - 0.9 + 28.812028 + 33.688 + 1. + offsetZ;
    TGeoCombiTrans* pMatrix13 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pWin1Log, 0, pMatrix13);

    // ---------------   Finish   -----------------------------------------------
    gGeoMan->CloseGeometry();
    gGeoMan->CheckOverlaps(0.001);
    gGeoMan->PrintOverlaps();
    gGeoMan->Test();

    TFile* geoFile = new TFile(geoFileName, "RECREATE");
    top->Write();
    geoFile->Close();
    std::cout << "\033[34m Creating geometry:\033[0m "
              << "\033[33m" << geoFileName << " \033[0m" << std::endl;
    gApplication->Terminate();
}

TGeoCombiTrans* GetGlobalPosition(TGeoCombiTrans* fRef)
{
    if (fLocalTrans == kTRUE)
    {

        if ((fThetaX == 0) && (fThetaY == 0) && (fThetaZ == 0) && (fX == 0) && (fY == 0) && (fZ == 0))
            return fRef;

        // X axis
        Double_t xAxis[3] = { 1., 0., 0. };
        Double_t yAxis[3] = { 0., 1., 0. };
        Double_t zAxis[3] = { 0., 0., 1. };
        // Reference Rotation
        fRefRot = fRef->GetRotation();

        if (fRefRot)
        {
            Double_t mX[3] = { 0., 0., 0. };
            Double_t mY[3] = { 0., 0., 0. };
            Double_t mZ[3] = { 0., 0., 0. };

            fRefRot->LocalToMasterVect(xAxis, mX);
            fRefRot->LocalToMasterVect(yAxis, mY);
            fRefRot->LocalToMasterVect(zAxis, mZ);

            Double_t a[4] = { mX[0], mX[1], mX[2], fThetaX };
            Double_t b[4] = { mY[0], mY[1], mY[2], fThetaY };
            Double_t c[4] = { mZ[0], mZ[1], mZ[2], fThetaZ };

            ROOT::Math::AxisAngle aX(a, a + 4);
            ROOT::Math::AxisAngle aY(b, b + 4);
            ROOT::Math::AxisAngle aZ(c, c + 4);

            ROOT::Math::Rotation3D fMatX(aX);
            ROOT::Math::Rotation3D fMatY(aY);
            ROOT::Math::Rotation3D fMatZ(aZ);

            ROOT::Math::Rotation3D fRotXYZ = (fMatZ * (fMatY * fMatX));

            // cout << fRotXYZ << endl;

            Double_t fRotable[9] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
            fRotXYZ.GetComponents(fRotable[0],
                                  fRotable[3],
                                  fRotable[6],
                                  fRotable[1],
                                  fRotable[4],
                                  fRotable[7],
                                  fRotable[2],
                                  fRotable[5],
                                  fRotable[8]);
            TGeoRotation* pRot = new TGeoRotation();
            pRot->SetMatrix(fRotable);
            TGeoCombiTrans* pTmp = new TGeoCombiTrans(*fGlobalTrans, *pRot);

            // ne peut pas etre applique ici
            // il faut differencier trans et rot dans la multi.
            TGeoRotation rot_id;
            rot_id.SetAngles(0.0, 0.0, 0.0);

            TGeoCombiTrans c1;
            c1.SetRotation(rot_id);
            const Double_t* t = pTmp->GetTranslation();
            c1.SetTranslation(t[0], t[1], t[2]);

            TGeoCombiTrans c2;
            c2.SetRotation(rot_id);
            const Double_t* tt = fRefRot->GetTranslation();
            c2.SetTranslation(tt[0], tt[1], tt[2]);

            TGeoCombiTrans cc = c1 * c2;

            TGeoCombiTrans c3;
            c3.SetRotation(pTmp->GetRotation());
            TGeoCombiTrans c4;
            c4.SetRotation(fRefRot);

            TGeoCombiTrans ccc = c3 * c4;

            TGeoCombiTrans pGlobal;
            pGlobal.SetRotation(ccc.GetRotation());
            const Double_t* allt = cc.GetTranslation();
            pGlobal.SetTranslation(allt[0], allt[1], allt[2]);

            return (new TGeoCombiTrans(pGlobal));
        }
        else
        {

            cout << "-E- R3BDetector::GetGlobalPosition() \
	      No. Ref. Transformation defined ! "
                 << endl;
            cout << "-E- R3BDetector::GetGlobalPosition() \
	      cannot create Local Transformation "
                 << endl;
            return NULL;
        } //! fRefRot
    }
    else
    {
        // Lab Transf.
        if ((fPhi == 0) && (fTheta == 0) && (fPsi == 0) && (fX == 0) && (fY == 0) && (fZ == 0))
            return fRef;

        return (new TGeoCombiTrans(*fGlobalTrans, *fGlobalRot));
    }
}
