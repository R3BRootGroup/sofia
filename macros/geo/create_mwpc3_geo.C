//
//   ----- General Macro for R3B simulation
//
//         Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
//
//         Last Update: 07/10/19 (Jose Luis)
//
//         Comments:
//

#include "TGeoArb8.h"
#include "TGeoBBox.h"
#include "TGeoBoolNode.h"
#include "TGeoCompositeShape.h"
#include "TGeoCone.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMatrix.h"
#include "TGeoMedium.h"
#include "TGeoPgon.h"
#include "TGeoSphere.h"
#include "TGeoTube.h"
#include "TMath.h"
#include <iomanip>
#include <iostream>

// Create Matrix Unity
TGeoRotation* fGlobalRot = new TGeoRotation();

// Create a null translation
TGeoTranslation* fGlobalTrans = new TGeoTranslation();
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
Bool_t fLabTrans = kTRUE;

TGeoCombiTrans* GetGlobalPosition(TGeoCombiTrans* fRef);

void create_mwpc3_geo(const char* geoTag = "3")
{
    // --------------------------------------------------------------------------
    // Configurable geometry for the MWPC3 detectors.
    // Use this macro to create root files with the different configurations
    // and positions of the MWPC3.
    //
    // Execute macro:  root -l
    //                 .L create_mwpc3_geo.C
    //                 create_mwpc3_geo()
    // --------------------------------------------------------------------------

    fGlobalTrans->SetTranslation(0.0, 0.0, 0.0);

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
    TString geoFileName = geoPath + "/sofia/geometry/mwpc_";
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

    FairGeoMedium* mSi = geoMedia->getMedium("silicon");
    if (!mSi)
        Fatal("Main", "FairMedium silicon not found");
    geoBuild->createMedium(mSi);
    TGeoMedium* pMedSi = gGeoMan->GetMedium("silicon");
    if (!pMedSi)
        Fatal("Main", "Medium silicon not found");

    FairGeoMedium* mCopper = geoMedia->getMedium("copper");
    if (!mCopper)
        Fatal("Main", "FairMedium copper not found");
    geoBuild->createMedium(mCopper);
    TGeoMedium* pMedCu = gGeoMan->GetMedium("copper");
    if (!pMedCu)
        Fatal("Main", "Medium copper not found");

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
    TGeoMedium* pMedMylar = gGeoMan->GetMedium("mylar");
    if (!pMedMylar)
        Fatal("Main", "Medium Mylar not found");
    // --------------------------------------------------------------------------

    // --------------   Create geometry and top volume  -------------------------
    gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
    gGeoMan->SetName("TRAgeom");
    TGeoVolume* top = new TGeoVolumeAssembly("TOP");
    gGeoMan->SetTopVolume(top);
    // --------------------------------------------------------------------------

    // out-of-file geometry definition
    Double_t dx, dy, dz;
    Double_t rmin, rmax, rmin1, rmax1, rmin2, rmax2;
    Double_t thx, phx, thy, phy, thz, phz;
    Double_t phi1, phi2;
    Double_t tx, ty, tz;

    TGeoRotation* zeroRot = new TGeoRotation; // zero rotation
    TGeoCombiTrans* tZero = new TGeoCombiTrans("tZero", 0., 0., 0., zeroRot);
    tZero->RegisterYourself();
    //-------------------------------------------------------------------

    // WORLD
    TGeoVolume* pAWorld = gGeoManager->GetTopVolume();

    TGeoRotation* rot_mwpc = new TGeoRotation("MWPCrot");

    // Defintion of the Mother Volume
    TGeoShape* pCBWorld = new TGeoBBox("MWPC_box", 120.0 / 2.0, 80.0 / 2.0, 9.0 / 2.0);

    TGeoVolume* pWorld = new TGeoVolume("MWPCWorld_3", pCBWorld, pMedAir);

    TGeoCombiTrans* t0 = new TGeoCombiTrans(0., 0., 0., rot_mwpc);
    TGeoCombiTrans* pGlobalc = GetGlobalPosition(t0);

    // add as Mother Volume
    pAWorld->AddNodeOverlap(pWorld, 0, pGlobalc);

    // TRANSFORMATION MATRICES
    // Combi transformation:
    dx = 0.000;
    dy = 0.000;
    dz = 0.000;
    TGeoCombiTrans* pMatrix1 = new TGeoCombiTrans("", dx, dy, dz, 0);
    //
    dx = 119.9;
    dy = 79.9;
    dz = 5.0;
    TGeoShape* basea = new TGeoBBox("BaseAl", dx / 2., dy / 2., dz / 2.);
    dx = 90.;
    dy = 60.0;
    dz = 5.5;
    TGeoShape* baseb = new TGeoBBox("BaseAls", dx / 2., dy / 2., dz / 2.);
    TGeoSubtraction* basesub = new TGeoSubtraction(basea, baseb, pMatrix1, pMatrix1);
    TGeoShape* basesub_c = new TGeoCompositeShape("GE", basesub);
    TGeoVolume* basesub_log = new TGeoVolume("", basesub_c, pMedAl);
    basesub_log->SetVisLeaves(kTRUE);
    basesub_log->SetLineColor(29);

    // Position
    TGeoCombiTrans* pGlobal2 = GetGlobalPosition(pMatrix1);
    pWorld->AddNode(basesub_log, 0, pGlobal2);

    // Shape
    dx = 90.;
    dy = 60.;
    dz = 3.0;
    TGeoShape* Detector1 = new TGeoBBox("Detector_1", dx / 2., dy / 2., dz / 2.);
    // Volume:
    TGeoVolume* Detector_log1 = new TGeoVolume("MWPC3", Detector1, pMedAr);
    Detector_log1->SetVisLeaves(kTRUE);
    Detector_log1->SetLineColor(3);
    pWorld->AddNode(Detector_log1, 0, pGlobal2);

    dx = 0.000;
    dy = 0.000;
    dz = -1.5 - 0.002 / 2.;
    TGeoCombiTrans* pMatrix20 = new TGeoCombiTrans("", dx, dy, dz, 0);

    // Shape
    dx = 90.;
    dy = 60.;
    dz = 0.0020;
    TGeoShape* basemylar = new TGeoBBox("basemylar", dx / 2., dy / 2., dz / 2.);
    // Volume:
    TGeoVolume* basemylar_log1 = new TGeoVolume("basemylar_log1", basemylar, pMedMylar); // first Kapton
    basemylar_log1->SetVisLeaves(kTRUE);
    basemylar_log1->SetLineColor(28);

    // Position
    TGeoCombiTrans* pGlobal8 = GetGlobalPosition(pMatrix20);
    pWorld->AddNode(basemylar_log1, 0, pGlobal8);

    // Volume:
    TGeoVolume* basemylar_log2 = new TGeoVolume("basemylar_log2", basemylar, pMedMylar); // Second Kapton
    basemylar_log2->SetVisLeaves(kTRUE);
    basemylar_log2->SetLineColor(28);

    dx = 0.000;
    dy = 0.000;
    dz = 1.5 + 0.002 / 2.;
    TGeoCombiTrans* pMatrix21 = new TGeoCombiTrans("", dx, dy, dz, 0);

    // Position
    TGeoCombiTrans* pGlobal9 = GetGlobalPosition(pMatrix21);
    pWorld->AddNode(basemylar_log2, 0, pGlobal9);

    // ---------------   Finish   -----------------------------------------------
    gGeoMan->CloseGeometry();
    gGeoMan->CheckOverlaps(0.001);
    gGeoMan->PrintOverlaps();
    gGeoMan->Test();

    TFile* geoFile = new TFile(geoFileName, "RECREATE");
    top->Write();
    geoFile->Close();
    std::cout << "Creating geometry: " << geoFileName << std::endl;
    // --------------------------------------------------------------------------
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
