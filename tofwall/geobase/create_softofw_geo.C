//
//   ----- Macro for generating the SOFIA ToF-Wall root geometry
//
//         Author: Jose Luis <j.l.rodriguez.sanchez@udc.es>
//
//         Last Update: 29/12/25
//
//         Comments:
//

#include <TGeoManager.h>
#include <TMath.h>
#include <iomanip>
#include <iostream>
#include <vector>

void create_softofw_geo(const TString geoTag = "v2021.3")
{
    // --------------------------------------------------------------------------
    // Configurable geometry for the SOFIA ToF-Wall.
    // Use this macro to create root files with the different configurations
    // and positions/angles of the detector.
    //
    // Execute macro:  root -l
    //                 .L create_softofw_geo.C
    //                 create_softofw_geo()
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
    gGeoMan->SetName("TOFWgeom");
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
    dy = 66.0;
    dz = 0.50;
    auto* pTOFBox = new TGeoBBox("TOFBox", dx / 2., dy / 2., dz / 2.);
    auto* pTOFLog = new TGeoVolume("TOF_FFs", pTOFBox, pMed34);
    pTOFLog->SetVisLeaves(kTRUE);
    pTOFLog->SetLineColor(kAzure + 10);

    // Shape: PMTs
    dz = 12.0;
    auto* pPM = new TGeoTubeSeg("PMT", 0., 1.35, dz / 2., 0., 360.);

    auto* rot_PM = new TGeoRotation("PMrot");
    rot_PM->RotateX(90.0);

    std::vector<TGeoVolume*> pPM_nbu;
    std::vector<TGeoCombiTrans*> pGlobalPMsu;
    std::vector<TGeoCombiTrans*> pMatrixPMsu;

    std::vector<TGeoVolume*> pPM_nbd;
    std::vector<TGeoCombiTrans*> pGlobalPMsd;
    std::vector<TGeoCombiTrans*> pMatrixPMsd;

    std::vector<TGeoCombiTrans*> pGlobalSci;
    std::vector<TGeoCombiTrans*> pMatrixSci;

    for (size_t i = 0; i < 28; i++)
    {
        TString name = Form("PM_up_%zu", i + 1);

        dx = -3.15 * i + 3.15 * 13. + 3.15 / 2.;
        dy = 66. / 2. + 6.001;
        dz = 0.00;

        pMatrixPMsu.push_back(new TGeoCombiTrans("", dx, dy, dz, rot_PM));

        pPM_nbu.push_back(new TGeoVolume(name.Data(), pPM, pMedAl));
        pPM_nbu[i]->SetVisLeaves(kTRUE);
        pPM_nbu[i]->SetLineColor(kGray + 3);

        ptof->AddNode(pPM_nbu[i], i, pMatrixPMsu[i]);

        dy = 0.00;

        pMatrixSci.push_back(new TGeoCombiTrans("", dx, dy, dz, rotg));
        ptof->AddNode(pTOFLog, i, pMatrixSci[i]);

        name = Form("PM_down_%zu", i + 1);

        dy = -66. / 2. - 6.001;

        pMatrixPMsd.push_back(new TGeoCombiTrans("", dx, dy, dz, rot_PM));

        pPM_nbd.push_back(new TGeoVolume(name.Data(), pPM, pMedAl));
        pPM_nbd[i]->SetVisLeaves(kTRUE);
        pPM_nbd[i]->SetLineColor(kGray + 3);

        ptof->AddNode(pPM_nbd[i], i, pMatrixPMsd[i]);
    }

    // --------------- Supports --------------------------------------
    dx = 100.0;
    dy = 6.0;
    dz = 6.0;
    auto* profile = new TGeoBBox("Profile1", dx / 2., dy / 2., dz / 2.);
    auto* profile1vol = new TGeoVolume("", profile, pMedAl);
    profile1vol->SetVisLeaves(kTRUE);
    profile1vol->SetLineColor(204);

    dx = 0.00;
    dy = 66. / 2. + 12. + 3.;
    dz = 0.00;
    auto* pMatrix3 = new TGeoCombiTrans("", dx, dy, dz, 0);
    ptof->AddNode(profile1vol, 1, pMatrix3);

    auto* pMatrix4 = new TGeoCombiTrans("", dx, -dy, dz, 0);
    ptof->AddNode(profile1vol, 2, pMatrix4);

    dx = 6.0;
    dy = 102.0;
    dz = 6.0;
    auto* profile2 = new TGeoBBox("Profile2", dx / 2., dy / 2., dz / 2.);
    auto* profile2vol = new TGeoVolume("", profile2, pMedAl);
    profile2vol->SetVisLeaves(kTRUE);
    profile2vol->SetLineColor(204);

    dx = 53.;
    dy = 0.;
    dz = 0.;
    auto* pMatrix5 = new TGeoCombiTrans("", dx, dy, dz, 0);
    ptof->AddNode(profile2vol, 0, pMatrix5);

    auto* pMatrix6 = new TGeoCombiTrans("", -dx, dy, dz, 0);
    ptof->AddNode(profile2vol, 0, pMatrix6);

    pWorld->AddNode(ptof, 0, pMatrix2);

    // ---------------   Finish   -----------------------------------------------
    gGeoMan->CloseGeometry();
    gGeoMan->CheckOverlaps(0.001);
    gGeoMan->PrintOverlaps();
    gGeoMan->Test();

    TFile geoFile(geoFileName, "RECREATE");
    top->Write();
    // top->Draw("ogl");
    geoFile.Close();

    std::cout << "\033[34m Creating geometry:\033[0m "
              << "\033[33m" << geoFileName << " \033[0m" << std::endl;
    std::cout << "Macro finished successfully." << std::endl;
    gApplication->Terminate();
}
