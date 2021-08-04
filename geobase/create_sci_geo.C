// --------------------------------------------------------------
//         Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
//
//         Last Update: 12/08/21 (Jose Luis)
//         Comments:
// --------------------------------------------------------------

#include "TGeoManager.h"
#include "TMath.h"
#include <iomanip>
#include <iostream>

void create_sci_geo(const char* geoTag = "v21")
{
    // --------------------------------------------------------------------------
    // Configurable geometry for the SOFIA start scintillator.
    // Use this macro to create root files with the different configurations
    // and positions/angles of the detector.
    //
    // Execute macro:  root -l
    //                 .L create_sci_geo.C
    //                 create_sci_geo()
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
    TString geoFileName = geoPath + "/geometry/sof_sci_";
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
    gGeoMan->SetName("SCIgeom");
    TGeoVolume* top = new TGeoVolumeAssembly("TOP");
    gGeoMan->SetTopVolume(top);
    // --------------------------------------------------------------------------

    // out-of-file geometry definition
    Double_t dx, dy, dz;
    Double_t a;
    Double_t z, density, w;
    Int_t nel, numed;

    TGeoVolumeAssembly* psci = new TGeoVolumeAssembly("SCI");

    // Top Volume
    TGeoVolume* pWorld = gGeoManager->GetTopVolume();
    pWorld->SetVisLeaves(kTRUE);

    // --------------- Detector --------------------------------------

    // SHAPES, VOLUMES AND GEOMETRICAL HIERARCHY
    // Shape: Plastic scintillator type: TGeoBBox
    dx = 5.00;
    dy = 3.00;
    dz = 0.10;
    TGeoShape* pSci = new TGeoBBox("plastic", dx / 2., dy / 2., dz / 2.);
    // Volume: TOFLog
    TGeoVolume* pSciLog = new TGeoVolume("Sci_1", pSci, pMed34);
    pSciLog->SetVisLeaves(kTRUE);
    pSciLog->SetLineColor(kBlue - 8);

    psci->AddNode(pSciLog, 0, tZero);

    // SHAPES, VOLUMES AND GEOMETRICAL HIERARCHY
    // Shape: PMs type: TGeoTubeSeg
    Double_t rmin = 0.00;
    Double_t rmax = 1.50;
    dz = 10.00;
    Double_t phi1 = 0.000000;
    Double_t phi2 = 360.0000;
    TGeoShape* pPM = new TGeoTubeSeg("PM", rmin, rmax, dz / 2., phi1, phi2);

    TGeoRotation* rot_PM = new TGeoRotation("PMrot");
    rot_PM->RotateY(90.0);

    TGeoVolume* pmt1 = new TGeoVolume("Pmt1", pPM, pMedAl);
    pmt1->SetVisLeaves(kTRUE);
    pmt1->SetLineColor(kGray);
    TGeoCombiTrans* pMatrix1 = new TGeoCombiTrans("", dx / 2.0 + dz / 2.0, 0., 0., rot_PM);
    psci->AddNode(pmt1, 0, pMatrix1);

    TGeoVolume* pmt2 = new TGeoVolume("Pmt2", pPM, pMedAl);
    pmt2->SetVisLeaves(kTRUE);
    pmt2->SetLineColor(kGray);
    TGeoCombiTrans* pMatrix2 = new TGeoCombiTrans("", -dx / 2.0 - dz / 2.0, 0., 0., rot_PM);
    psci->AddNode(pmt2, 0, pMatrix2);

    pWorld->AddNode(psci, 0, tZero);

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
