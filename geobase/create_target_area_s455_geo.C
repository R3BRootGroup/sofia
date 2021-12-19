//
//   ----- General Macro for R3B-SOFIA simulations
//
//         Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
//
//         Last Update: 03/04/20 (Jose Luis)
//
//         Comments:
//

#include "TGeoManager.h"
#include "TMath.h"
#include <iomanip>
#include <iostream>

void create_target_area_s455_geo(const char* geoTag = "s455_v21")
{
    // --------------------------------------------------------------------------
    // Configurable geometry for the target vacuum chamber according to the
    // e-mail on 1st October 2019.
    // Use this macro to create root files with the different configurations
    // and positions of the vacuum chamber.
    //
    // Execute macro:  root -l
    //                 .L create_target_area_s455_geo.C
    //                 create_target_area_s455_geo()
    // --------------------------------------------------------------------------

    TGeoRotation* fRefRot = NULL;
    TGeoManager* gGeoMan = NULL;

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
    TString geoFileName = geoPath + "/geometry/target_area_";
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

    FairGeoMedium* mSi = geoMedia->getMedium("silicon");
    if (!mSi)
        Fatal("Main", "FairMedium silicon not found");
    geoBuild->createMedium(mSi);
    TGeoMedium* pMedSi = gGeoMan->GetMedium("silicon");
    if (!pMedSi)
        Fatal("Main", "Medium silicon not found");

    FairGeoMedium* mGold = geoMedia->getMedium("gold");
    if (!mGold)
        Fatal("Main", "FairMedium gold not found");
    geoBuild->createMedium(mGold);
    TGeoMedium* pMedGold = gGeoMan->GetMedium("gold");
    if (!pMedGold)
        Fatal("Main", "Medium gold not found");

    // --------------------------------------------------------------------------

    // --------------   Create geometry and top volume  -------------------------
    gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
    gGeoMan->SetName("TARGETAREAgeom");
    TGeoVolume* top = new TGeoVolumeAssembly("TOP");
    gGeoMan->SetTopVolume(top);
    // --------------------------------------------------------------------------

    // Defintion of the Mother Volume
    TGeoShape* pVCTube = new TGeoTube("CenterPart", 0., 29.26, 140.2 / 2.0);

    TGeoCombiTrans* t_tube = new TGeoCombiTrans("t_tube", 0., 0., 0., fRefRot);
    t_tube->RegisterYourself();

    TGeoShape* pVCCone = new TGeoCone("Cone", (33.688 + 4.) / 2., 0., 4.6, 0., 7.8);

    TGeoCombiTrans* t_cone = new TGeoCombiTrans("t_cone", 0., 0., 442. / 2.0 + (33.688 + 4.) / 2., fRefRot);
    t_cone->RegisterYourself();

    TGeoCompositeShape* pVCWorld = new TGeoCompositeShape("VCbox", "CenterPart:t_tube + Cone:t_cone");

    TGeoVolume* pWorld = new TGeoVolume("VCWorld", pVCWorld, pMedVac);

    TGeoCombiTrans* t0 = new TGeoCombiTrans(0., 0., 0., fRefRot);

    // add the sphere as Mother Volume
    top->AddNode(pWorld, 0, t0);

    Double_t dx, dy, dz;
    Double_t thx, thy, thz;
    Double_t phx, phy, phz;

    Double_t offsetZ = -0.50; // offset with respect to LH2 target cell, entrance at (0,0,0)
    // Mylar windows for LH2 target
    Double_t thinMylar1 = 0.012 / 2.0;            // 120micra entrance window
    Double_t thinMylar2 = 0.018 / 2.0;            // 180micra exit window
    Double_t LH2targetR = 2.1 - thinMylar2 * 2.0; // Radius of 1.5 cm - mylar cylindre
    Double_t LH2targetL = 1.5;                    // Length 1.5cm

    //-------- Chamber -------------------------------------------------------------------------
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

    dx = 0.000000;
    dy = 0.000000;
    dz = 6.775000 - 0.9;
    TGeoCombiTrans* pMatrix177 = new TGeoCombiTrans("", dx, dy, dz, rot0);

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

    dx = 0.000000;
    dy = 0.000000;
    dz = 6.775000 - 0.9 + 28.812028 + 33.688 / 2.0;
    TGeoCombiTrans* pMatrix178 = new TGeoCombiTrans("", dx, dy, dz, rot0);

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

    //-------- AMS detectors ----------------------------------------------------
    //
    // AMS detectors have two sides, S and K, with a thickness of 150micras
    // S-side - 640 strips of 110 micras: 70.4 mm long
    // K-side - 384 strips of 104 micras: 39.936 mm long

    Int_t nbdet = 6;
    Double_t offsetZ_target_ams = 0.;

    TGeoVolume* AmsDet[nbdet];

    TGeoVolume* StripX[nbdet];
    TGeoVolume* StripY[nbdet];
    Char_t buffer[126];

    // Detector definition
    for (Int_t n = 0; n < nbdet; n++)
    {

        sprintf(buffer, "Det%i", n + 1);
        AmsDet[n] = gGeoManager->MakeBox(buffer, pMedVac, 7.05 / 2., 4. / 2., 0.0302 / 2.);
    }

    // Strip definition
    for (Int_t i = 0; i < nbdet; i++)
    {
        dx = 0.0110;
        dy = 3.9936;
        dz = 0.0150;
        sprintf(buffer, "Strip_X%i", i + 1);
        StripX[i] = gGeoManager->MakeBox(buffer, pMedSi, dx / 2., dy / 2., dz / 2.);
        StripX[i]->SetVisLeaves(kTRUE);
        StripX[i]->SetLineColor(19);
        // -----------
        sprintf(buffer, "Strip_Y%i", i + 1);
        dx = 7.040;
        dy = 0.0104;
        dz = 0.0150;
        StripY[i] = gGeoManager->MakeBox(buffer, pMedSi, dx / 2., dy / 2., dz / 2.);
        StripY[i]->SetVisLeaves(kTRUE);
        StripY[i]->SetLineColor(14);
    }

    // Adding strips to AMS detectors
    for (Int_t i = 0; i < 1024; i++)
    {
        if (i < 640)
        {
            dx = +7.040 / 2.0 - 0.0110 / 2.0 - 0.0110 * i;
            dy = 0.000;
            dz = -0.0150 / 2.0;
            TGeoCombiTrans* pMatrix = new TGeoCombiTrans("", dx, dy, dz, fRefRot);
            for (Int_t n = 0; n < nbdet; n++)
                AmsDet[n]->AddNode(StripX[n], i + 1, pMatrix);
        }
        else
        {
            dx = 0.000;
            dy = -3.9936 / 2.0 + 0.0104 / 2.0 + 0.0104 * (i - 640);
            dz = 0.0150 / 2.0;
            TGeoCombiTrans* pMatrix = new TGeoCombiTrans("", dx, dy, dz, fRefRot);
            for (Int_t n = 0; n < nbdet; n++)
                AmsDet[n]->AddNode(StripY[n], i + 1, pMatrix);
        }
    }

    // Set up the AMS detectors in the World
    dx = -4.889000;
    dy = 0.000000;
    dz = 2.683000 + offsetZ_target_ams;
    TGeoRotation* arot1 = new TGeoRotation();
    arot1->RotateY(-45.0);
    TGeoCombiTrans* pMatrixd1 = new TGeoCombiTrans("", dx, dy, dz, arot1);
    pWorld->AddNode(AmsDet[0], 1, pMatrixd1);

    dx = -7.189000;
    dy = 3.9936 / 2.;
    dz = 7.834000 + offsetZ_target_ams;
    TGeoRotation* arot2 = new TGeoRotation();
    arot2->RotateY(-45.0);
    TGeoCombiTrans* pMatrixd2 = new TGeoCombiTrans("", dx, dy, dz, arot2);
    pWorld->AddNode(AmsDet[1], 1, pMatrixd2);

    dx = -7.189000;
    dy = -3.9936 / 2.;
    dz = 7.834000 + offsetZ_target_ams;
    TGeoRotation* arot3 = new TGeoRotation();
    arot3->RotateZ(180.0);
    arot3->RotateY(-45.0);
    TGeoCombiTrans* pMatrixd3 = new TGeoCombiTrans("", dx, dy, dz, arot3);
    pWorld->AddNode(AmsDet[2], 1, pMatrixd3);

    dx = 4.889000;
    dy = 0.000000;
    dz = 2.683000 + offsetZ_target_ams;
    TGeoRotation* arot4 = new TGeoRotation();
    arot4->RotateZ(180.0);
    arot4->RotateY(45.0);
    TGeoCombiTrans* pMatrixd4 = new TGeoCombiTrans("", dx, dy, dz, arot4);
    pWorld->AddNode(AmsDet[3], 1, pMatrixd4);

    dx = 7.189000;
    dy = 3.9936 / 2.;
    dz = 7.834000 + offsetZ_target_ams;
    TGeoRotation* arot5 = new TGeoRotation();
    arot5->RotateY(45.0);
    TGeoCombiTrans* pMatrixd5 = new TGeoCombiTrans("", dx, dy, dz, arot5);
    pWorld->AddNode(AmsDet[4], 1, pMatrixd5);

    dx = 7.189000;
    dy = -3.9936 / 2.;
    dz = 7.834000 + offsetZ_target_ams;
    TGeoRotation* arot6 = new TGeoRotation();
    arot6->RotateZ(180.0);
    arot6->RotateY(45.0);
    TGeoCombiTrans* pMatrixd6 = new TGeoCombiTrans("", dx, dy, dz, arot6);
    pWorld->AddNode(AmsDet[5], 1, pMatrixd6);

    TGeoVolume* Shielding;
    sprintf(buffer, "Shielding%i", 1);
    Shielding = gGeoManager->MakeBox(buffer, pMedGold, 7.4 / 2.0, 4.1 / 2., 0.0010 / 2.);
    Shielding->SetLineColor(kYellow);
    Shielding->SetVisLeaves(kTRUE);
    dx = -4.000;
    dy = 0.000000;
    dz = 2.683000 + offsetZ_target_ams - 1.1;
    TGeoRotation* arots1 = new TGeoRotation();
    arots1->RotateY(-45.0);
    TGeoCombiTrans* pMatrixs1 = new TGeoCombiTrans("", dx, dy, dz, arots1);
    pWorld->AddNode(Shielding, 1, pMatrixs1);

    dx = 4.000;
    dy = 0.000000;
    dz = 2.683000 + offsetZ_target_ams - 1.1;
    TGeoRotation* arots2 = new TGeoRotation();
    arots2->RotateY(45.0);
    TGeoCombiTrans* pMatrixs2 = new TGeoCombiTrans("", dx, dy, dz, arots2);
    pWorld->AddNode(Shielding, 2, pMatrixs2);

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
