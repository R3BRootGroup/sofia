#include "TGeoManager.h"

void create_at_geo(const char* geoTag)
{
    // -------   Load media from media file   -----------------------------------
    FairGeoLoader* geoLoad = new FairGeoLoader("TGeo", "FairGeoLoader");
    FairGeoInterface* geoFace = geoLoad->getGeoInterface();
    TString geoPath = gSystem->Getenv("VMCWORKDIR");
    TString medFile = geoPath + "/sofia/geometry/media_r3b.geo";
    geoFace->setMediaFile(medFile);
    geoFace->readMedia();
    TGeoManager* gGeoMan = gGeoManager;
    // --------------------------------------------------------------------------

    // -------   Geometry file name (output)   ----------------------------------
    TString geoFileName = geoPath + "/sofia/geometry/sof_at_";
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
    TGeoMedium* pMed21 = gGeoMan->GetMedium("aluminium");
    if (!pMed21)
        Fatal("Main", "Medium aluminium not found");
    // --------------------------------------------------------------------------

    // --------------   Create geometry and top volume  -------------------------
    gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
    gGeoMan->SetName("SOFATgeom");
    TGeoVolume* top = new TGeoVolumeAssembly("TOP");
    gGeoMan->SetTopVolume(top);
    // --------------------------------------------------------------------------
    
    TGeoVolumeAssembly* world = new TGeoVolumeAssembly("SOFATLogWorld");

    // out-of-file geometry definition
    Double_t dx, dy, dz;
    Double_t thx, phx, thy, phy, thz, phz;

    dx = 30.00000;
    dy = 30.00000;
    dz = 0.50000;
    TGeoShape* pATBox = new TGeoBBox("SOFATBox", dx, dy, dz);
    TGeoVolume* pATLog = new TGeoVolume("SOFATLog", pATBox, pMed2);
    pATLog->SetVisLeaves(kTRUE);

    TGeoRotation* rot0 = new TGeoRotation("");
    TGeoCombiTrans* pMatrix0 = new TGeoCombiTrans(0., 0., 0., rot0);
    TGeoCombiTrans* pMatrix1 = new TGeoCombiTrans(0., 0., 20., rot0);

    world->AddNode(pATLog, 0, pMatrix0);
    
    top->AddNode(world, 0, pMatrix1);

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
