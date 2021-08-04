//
//   ----- General Macro for R3B simulation
//
//         Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
//
//         Last Update: 29/09/19 (Jose Luis)
//
//         Comments:
//

#include "TGeoManager.h"
#include "TMath.h"
#include <iomanip>
#include <iostream>

void create_tofsofia_geo(const char* geoTag = "v21")
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

    TGeoRotation* fRefRot = NULL;
    TGeoManager* gGeoMan = NULL;

    TGeoRotation* zeroRot = new TGeoRotation; // zero rotation
    TGeoCombiTrans* tZero = new TGeoCombiTrans("tZero", 0., 0., 0., zeroRot);
    tZero->RegisterYourself();

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
    TString geoFileName = geoPath + "/geometry/sof_tof_";
    geoFileName = geoFileName + geoTag + ".geo.root";
    // --------------------------------------------------------------------------

    // -----------------   Get and create the required media    -----------------
    FairGeoMedia* geoMedia = geoFace->getMedia();
    FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

    FairGeoMedium* mAir = geoMedia->getMedium("Air");
    if (!mAir)
        Fatal("Main", "FairMedium Air not found");
    geoBuild->createMedium(mAir);
    TGeoMedium* pMed2 = gGeoMan->GetMedium("Air");
    if (!pMed2)
        Fatal("Main", "Medium Air not found");

    FairGeoMedium* mAl = geoMedia->getMedium("aluminium");
    if (!mAl)
        Fatal("Main", "FairMedium aluminium not found");
    geoBuild->createMedium(mAl);
    TGeoMedium* pMedAl = gGeoMan->GetMedium("aluminium");
    if (!pMedAl)
        Fatal("Main", "Medium aluminium not found");

    FairGeoMedium* mFe = geoMedia->getMedium("iron");
    if (!mFe)
        Fatal("Main", "FairMedium iron not found");
    geoBuild->createMedium(mFe);
    TGeoMedium* pMedFe = gGeoMan->GetMedium("iron");
    if (!pMedFe)
        Fatal("Main", "Medium iron not found");

    FairGeoMedium* mTof = geoMedia->getMedium("plasticForTOF");
    if (!mTof)
        Fatal("Main", "FairMedium plasticForTOF not found");
    geoBuild->createMedium(mTof);
    TGeoMedium* pMed34 = gGeoMan->GetMedium("plasticForTOF");
    if (!pMed34)
        Fatal("Main", "Medium plasticForTOF not found");
    // --------------------------------------------------------------------------

    // --------------   Create geometry and top volume  -------------------------
    gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
    gGeoMan->SetName("TOFgeom");
    TGeoVolume* top = new TGeoVolumeAssembly("TOP");
    gGeoMan->SetTopVolume(top);
    // --------------------------------------------------------------------------

    // out-of-file geometry definition
    Double_t dx, dy, dz;
    Double_t a;
    Double_t z, density, w;
    Int_t nel, numed;

    TGeoVolumeAssembly* ptof = new TGeoVolumeAssembly("TOF");

    // TRANSFORMATION MATRICES
    // Combi transformation:
    dx = -257;
    dy = 0.00000;
    dz = 690. + 20.;

    TGeoRotation* rotg = new TGeoRotation();
    TGeoRotation* rot_tof = new TGeoRotation("Tofrot");
    rot_tof->RotateY(-34.0);

    TGeoCombiTrans* pMatrix2 = new TGeoCombiTrans("", dx, dy, dz, rot_tof);

    // Top Volume
    TGeoVolume* pWorld = gGeoManager->GetTopVolume();
    pWorld->SetVisLeaves(kTRUE);

    // --------------- Detector --------------------------------------

    // SHAPES, VOLUMES AND GEOMETRICAL HIERARCHY
    // Shape: Plastic scintillator type: TGeoBBox
    dx = 3.15;
    dy = 66.0000;
    dz = 0.500000;
    TGeoShape* pTOFBox = new TGeoBBox("TOFBox", dx / 2., dy / 2., dz / 2.);
    // Volume: TOFLog
    TGeoVolume* pTOFLog = new TGeoVolume("TOF_FFs", pTOFBox, pMed34);
    pTOFLog->SetVisLeaves(kTRUE);
    pTOFLog->SetLineColor(kBlue - 8);

    // SHAPES, VOLUMES AND GEOMETRICAL HIERARCHY
    // Shape: PMs type: TGeoTubeSeg
    Double_t rmin = 0.0000;
    Double_t rmax = 1.35;
    dz = 12.00000;
    Double_t phi1 = 0.000000;
    Double_t phi2 = 360.0000;
    TGeoShape* pPM = new TGeoTubeSeg("PM", rmin, rmax, dz / 2., phi1, phi2);

    TGeoRotation* rot_PM = new TGeoRotation("PMrot");
    rot_PM->RotateX(90.0);

    TGeoVolume* pPM_nbu[28];
    TGeoCombiTrans* pGlobalPMsu[28];
    TGeoCombiTrans* pMatrixPMsu[28];

    TGeoVolume* pPM_nbd[28];
    TGeoCombiTrans* pGlobalPMsd[28];
    TGeoCombiTrans* pMatrixPMsd[28];

    TGeoCombiTrans* pGlobalSci[28];
    TGeoCombiTrans* pMatrixSci[28];

    char buf[126];

    for (int i = 0; i < 28; i++)
    {

        sprintf(buf, "PM_up_%i", i + 1);

        dx = -3.15 * i + 3.15 * 13. + 3.15 / 2.;
        dy = 66. / 2. + 6.001;
        dz = 0.00;

        pMatrixPMsu[i] = new TGeoCombiTrans("", dx, dy, dz, rot_PM);

        pPM_nbu[i] = new TGeoVolume(buf, pPM, pMedAl);
        pPM_nbu[i]->SetVisLeaves(kTRUE);
        pPM_nbu[i]->SetLineColor(kGray);

        ptof->AddNode(pPM_nbu[i], i, pMatrixPMsu[i]);

        dx = -3.15 * i + 3.15 * 13. + 3.15 / 2.;
        dy = 0.00;
        dz = 0.00;

        pMatrixSci[i] = new TGeoCombiTrans("", dx, dy, dz, rotg);
        ptof->AddNode(pTOFLog, i, pMatrixSci[i]);

        sprintf(buf, "PM_down_%i", i + 1);

        dx = -3.15 * i + 3.15 * 13. + 3.15 / 2.;
        dy = -66. / 2. - 6.001;
        dz = 0.00;

        pMatrixPMsd[i] = new TGeoCombiTrans("", dx, dy, dz, rot_PM);

        pPM_nbd[i] = new TGeoVolume(buf, pPM, pMedAl);
        pPM_nbd[i]->SetVisLeaves(kTRUE);
        pPM_nbd[i]->SetLineColor(kGray);

        ptof->AddNode(pPM_nbd[i], i, pMatrixPMsd[i]);
    }

    // --------------- Supports --------------------------------------
    dx = 100.0;
    dy = 4.0;
    dz = 5.0;
    TGeoShape* plane = new TGeoBBox("plane1", dx / 2., dy / 2., dz / 2.);
    TGeoVolume* subplane = new TGeoVolume("", plane, pMedAl);
    subplane->SetVisLeaves(kTRUE);
    subplane->SetLineColor(5);

    dx = 0.00;
    dy = 66. / 2. + 12. + 2.;
    dz = 0.00;
    TGeoCombiTrans* pMatrix3 = new TGeoCombiTrans("", dx, dy, dz, 0);
    ptof->AddNode(subplane, 0, pMatrix3);

    dx = 0.00;
    dy = -dy;
    dz = 0.00;
    TGeoCombiTrans* pMatrix4 = new TGeoCombiTrans("", dx, dy, dz, 0);
    ptof->AddNode(subplane, 0, pMatrix4);

    dx = 3.99;
    dy = 180.0;
    dz = 5.0;
    TGeoShape* plane2 = new TGeoBBox("plane2", dx / 2., dy / 2., dz / 2.);
    TGeoVolume* subplane2 = new TGeoVolume("", plane2, pMedAl);
    subplane2->SetVisLeaves(kTRUE);
    subplane2->SetLineColor(5);

    dx = 52.00;
    dy = -45.00 + 4;
    dz = 0.00;
    TGeoCombiTrans* pMatrix5 = new TGeoCombiTrans("", dx, dy, dz, 0);
    ptof->AddNode(subplane2, 0, pMatrix5);

    dx = -52.00;
    dy = -45.00 + 4;
    dz = 0.00;
    TGeoCombiTrans* pMatrix6 = new TGeoCombiTrans("", dx, dy, dz, 0);
    ptof->AddNode(subplane2, 0, pMatrix6);

    pWorld->AddNode(ptof, 0, pMatrix2);

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
