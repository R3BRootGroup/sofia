// -------------------------------------------------------------------
//         Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
//
//         Last Update: 12/08/21 (Jose Luis)
//
//         Comments:
// -------------------------------------------------------------------

#include "TGeoManager.h"
#include "TMath.h"
#include <iomanip>
#include <iostream>

void create_trim_geo(const char* geoTag = "v21")
{
    // --------------------------------------------------------------------------
    // Configurable geometry for the SOFIA triple-MUSIC.
    // Use this macro to create root files with the different configurations
    // and positions/angles of the detector.
    //
    // Execute macro:  root -l
    //                 .L create_trim_geo.C
    //                 create_trim_geo()
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
    TString geoFileName = geoPath + "/geometry/trim_";
    geoFileName = geoFileName + geoTag + ".geo.root";
    // --------------------------------------------------------------------------

    // -----------------   Get and create the required media    -----------------
    FairGeoMedia* geoMedia = geoFace->getMedia();
    FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

    FairGeoMedium* mAir = geoMedia->getMedium("Air");
    if (!mAir)
        Fatal("Main", "FairMedium Air not found");
    geoBuild->createMedium(mAir);
    TGeoMedium* pMedAir = gGeoMan->GetMedium("Air");
    if (!pMedAir)
        Fatal("Main", "Medium Air not found");

    FairGeoMedium* mVac = geoMedia->getMedium("vacuum");
    if (!mVac)
        Fatal("Main", "FairMedium vacuum not found");
    geoBuild->createMedium(mVac);
    TGeoMedium* pMedVac = gGeoMan->GetMedium("vacuum");
    if (!pMedVac)
        Fatal("Main", "Medium vacuum not found");

    FairGeoMedium* mAl = geoMedia->getMedium("aluminium");
    if (!mAl)
        Fatal("Main", "FairMedium aluminium not found");
    geoBuild->createMedium(mAl);
    TGeoMedium* pMedAl = gGeoMan->GetMedium("aluminium");
    if (!pMedAl)
        Fatal("Main", "Medium aluminium not found");

    FairGeoMedium* mNb = geoMedia->getMedium("niobium");
    if (!mNb)
        Fatal("Main", "FairMedium niobium not found");
    geoBuild->createMedium(mNb);
    TGeoMedium* pMedNb = gGeoMan->GetMedium("niobium");
    if (!pMedNb)
        Fatal("Main", "Medium niobium not found");

    FairGeoMedium* mAr = geoMedia->getMedium("ArCO2");
    if (!mAr)
        Fatal("Main", "FairMedium ArCO2 not found");
    geoBuild->createMedium(mAr);
    TGeoMedium* pMedAr = gGeoMan->GetMedium("ArCO2");
    if (!pMedAr)
        Fatal("Main", "Medium ArCO2 not found");

    FairGeoMedium* mMylar = geoMedia->getMedium("mylar");
    if (!mMylar)
        Fatal("Main", "FairMedium mylar not found");
    geoBuild->createMedium(mMylar);
    TGeoMedium* pMed4 = gGeoMan->GetMedium("mylar");
    if (!pMed4)
        Fatal("Main", "Medium mylar not found");

    // --------------   Create geometry and top volume  -------------------------
    gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
    gGeoMan->SetName("TRIMgeom");
    TGeoVolume* top = new TGeoVolumeAssembly("TOP");
    gGeoMan->SetTopVolume(top);
    // --------------------------------------------------------------------------

    // out-of-file geometry definition
    Double_t dx = 0., dy = 0., dz = 0.;
    TGeoRotation* rotg = new TGeoRotation();
    TGeoCombiTrans* pMatrix0 = new TGeoCombiTrans();

    // Defintion of the Mother Volume
    TGeoShape* pMWorld = new TGeoBBox("trimbox", 20. / 2.0, 20. / 2.0, 60.005 / 2.0);
    TGeoVolume* pWorld = new TGeoVolume("TRIMWorld", pMWorld, pMedAr);

    // Adding the Mother Volume
    top->AddNode(pWorld, 0, pMatrix0);

    // --------------- Detector --------------------------------------

    TGeoVolumeAssembly* trim = new TGeoVolumeAssembly("TRIMDet");

    // SHAPES, VOLUMES AND GEOMETRICAL HIERARCHY
    // Shape: Anode type: TGeoBBox
    dx = 20.000;
    dy = 20.000;
    dz = 5.000;
    Double_t* fver = new Double_t[16];

    fver[0] = -5. / 2.;
    fver[1] = -10.;

    fver[2] = -5. / 2.;
    fver[3] = 10.;

    fver[4] = 5. / 2.;
    fver[5] = 10.;

    fver[6] = 5. / 2.;
    fver[7] = -10.;

    fver[8] = -5. / 2.;
    fver[9] = -10.;

    fver[10] = -5. / 2.;
    fver[11] = 10.;

    fver[12] = -5. / 2.;
    fver[13] = 10.;

    fver[14] = -5. / 2.;
    fver[15] = -10.;

    TGeoVolume* AnodeLog = gGeoManager->MakeArb8("TrimAnode", pMedAr, 20. / 2., fver);
    AnodeLog->SetVisLeaves(kTRUE);
    AnodeLog->SetLineColor(2);

    TGeoRotation* rotw = new TGeoRotation();
    rotw->RotateZ(180.);
    rotw->RotateY(90.);
    rotw->RotateZ(180.);

    // Screening anodes, Shape & volume: TGeoBBox
    dz = 2.0;
    dy = 20.0;
    dx = 20.00;
    TGeoVolume* SAnode = gGeoManager->MakeBox("TrimScreening", pMedAr, dx / 2., dy / 2., dz / 2.);
    SAnode->SetVisLeaves(kTRUE);
    SAnode->SetLineColor(5);

    Int_t nbSections = 3;
    Int_t nbPairs = 3;
    Int_t nbAnodes = 6;
    TGeoCombiTrans* pMatrixAnode[nbSections * nbAnodes];
    TGeoCombiTrans* pMatrixSAnode[nbSections * 2];

    Int_t nbanode = 0;
    dx = 0.;
    dy = 0.;
    dz = 0.;
    Int_t sanode = 0;
    for (Int_t i = 0; i < nbSections; i++)
    {
        sanode = i * 2;
        dz = 15.0 * (i - 1) + 5.5 * (i - 1) - 7.5 - 1.0;
        pMatrixSAnode[sanode] = new TGeoCombiTrans("", dx, dy, dz, rotg);
        trim->AddNode(SAnode, sanode + 1, pMatrixSAnode[sanode]);
        sanode = i * 2 + 1;
        dz = 15.0 * (i - 1) + 5.5 * (i - 1) + 7.5 + 1.0;
        pMatrixSAnode[sanode] = new TGeoCombiTrans("", dx, dy, dz, rotg);
        trim->AddNode(SAnode, sanode + 1, pMatrixSAnode[sanode]);

        nbanode = i * nbAnodes;
        for (Int_t j = 0; j < nbPairs; j++)
        { // Anodes per section
            dz = -5.0 + 5.0 * j + 15.0 * (i - 1) + 5.5 * (i - 1);
            pMatrixAnode[nbanode] = new TGeoCombiTrans("", dx, dy, dz, rotw);
            trim->AddNode(AnodeLog, nbanode + 1, pMatrixAnode[nbanode]);
            nbanode++;
            pMatrixAnode[nbanode] = new TGeoCombiTrans("", dx, dy, dz, rotw);
            pMatrixAnode[nbanode]->RotateY(180.);
            trim->AddNode(AnodeLog, nbanode + 1, pMatrixAnode[nbanode]);
            nbanode++;
        }
    }
    pWorld->AddNode(trim, 0, pMatrix0);

    // Nb foils, Shape & volume: TGeoBBox
    dx = 20.0;
    dy = 20.0;
    dz = 0.0080;
    TGeoVolume* Nbfoil = gGeoManager->MakeBox("Nbfoil", pMedNb, dx / 2., dy / 2., dz / 2.);
    Nbfoil->SetVisLeaves(kTRUE);
    Nbfoil->SetLineColor(4);

    dz = -7.5 - 2.0 - 1.5 / 2.0;
    dy = 0.000;
    dx = 0.;
    TGeoCombiTrans* pMatrixd1 = new TGeoCombiTrans("", dx, dy, dz, rotg);
    pWorld->AddNode(Nbfoil, 0, pMatrixd1);

    dz = 7.5 + 2.0 + 1.5 / 2.0;
    TGeoCombiTrans* pMatrixd2 = new TGeoCombiTrans("", dx, dy, dz, rotg);
    pWorld->AddNode(Nbfoil, 1, pMatrixd2);

    // Windows, Shape & volume: TGeoBBox
    dx = 20.0;
    dy = 20.0;
    dz = 0.0025;
    TGeoVolume* Wind = gGeoManager->MakeBox("TrimWindow", pMed4, dx / 2., dy / 2., dz / 2.);
    Wind->SetVisLeaves(kTRUE);
    Wind->SetLineColor(13);

    dz = -30.000 - 0.0025 / 2.0;
    dy = 0.000;
    dx = 0.;
    TGeoCombiTrans* pMatrixd3 = new TGeoCombiTrans("", dx, dy, dz, rotg);
    pWorld->AddNode(Wind, 0, pMatrixd3);

    dz = 30. + 0.0025 / 2.0;
    TGeoCombiTrans* pMatrixd4 = new TGeoCombiTrans("", dx, dy, dz, rotg);
    pWorld->AddNode(Wind, 1, pMatrixd4);

    // ---------------   Finish   -----------------------------------------------
    gGeoMan->CloseGeometry();
    gGeoMan->CheckOverlaps(0.001);
    gGeoMan->PrintOverlaps();
    gGeoMan->Test();

    TFile* geoFile = new TFile(geoFileName, "RECREATE");
    top->Write();
    // top->Draw();
    geoFile->Close();
    std::cout << "\033[34m Creating geometry:\033[0m "
              << "\033[33m" << geoFileName << " \033[0m" << std::endl;
    gApplication->Terminate();
}
