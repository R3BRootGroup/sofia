//--------------------------------------------------------------------
//
// Define the SOFIA simulation setup for p2p-fission experiments
// Author: <joseluis.rodriguez.sanchez@usc.es>
//
// Last Update: 29/10/19 (Jose Luis)
// Comments:
//         - 29/10/19 : Initial setup
//
//
//--------------------------------------------------------------------

void runsim(Int_t nEvents = 0)
{
    // =========== Configuration area =============================

    TString OutFile = "sim.root"; // Output file for data
    TString ParFile = "par.root"; // Output file for params

    Bool_t fVis = true;              // Store tracks for visualization
    Bool_t fUserPList = false;       // Use of R3B special physics list
    Bool_t fR3BMagnet = true;        // Magnetic field definition
    Bool_t fCalifaHitFinder = false; // Apply hit finder task

    TString fMC = "TGeant4"; // MonteCarlo engine: TGeant3, TGeant4, TFluka

    TString generator1 = "box";
    TString generator2 = "ascii";
    TString generator3 = "r3b";
    TString fGenerator = generator1; // Event generator type: box, gammas, r3b, ion, ascii

    TString fEventFile = "p2p_U238_500.txt"; // Input event file in the case of ascii generator

    Int_t fFieldMap = -1;          // Magentic field map selector
    Double_t fMeasCurrent = 2000.; // Magnetic field current
    Float_t fFieldScale = -0.8;    // Magnetic field scale factor

    // ---------------  Detector selection: true - false ----------------------
    // ---- R3B and SOFIA detectors as well as passive elements

    Bool_t fR3BMusic = false; // R3B Music Detector
    TString fR3BMusicGeo = "music_s467.geo.root";

    Bool_t fMwpc0 = false; // MWPC0 Detector
    TString fMwpc0Geo = "";

    Bool_t fTracker = false; // AMS-Tracker + Vacuum chamber + LH2 target
    TString fTrackerGeo = "targetvacuumchamber_ams_s455.geo.root";

    Bool_t fCalifa = false; // Califa Calorimeter
    TString fCalifaGeo = "califa_2020.geo.root";
    Int_t fCalifaGeoVer = 10;
    Double_t fCalifaNonU = 1.0; // Non-uniformity: 1 means +-1% max deviation

    Bool_t fMwpc1 = true; // MWPC1 Detector
    TString fMwpc1Geo = "mwpc_1.geo.root";

    Bool_t fTwim = true; // Twin-Music Detector
    TString fTwimGeo = "twinmusic_v19a.geo.root";

    Bool_t fMwpc2 = true; // MWPC2 Detector
    TString fMwpc2Geo = "mwpc_2.geo.root";

    Bool_t fAladin = false; // Aladin Magnet
    TString fAladinGeo = "aladin_v13a.geo.root";

    Bool_t fGlad = true; // Glad Magnet
    TString fGladGeo = "glad_v17_flange.geo.root";

    Bool_t fMwpc3 = true; // MWPC3 Detector
    TString fMwpc3Geo = "mwpc_3.geo.root";

    Bool_t fSofTofWall = true; // Sofia ToF-Wall
    TString fSofTofWallGeo = "sof_tof_v19.geo.root";

    Bool_t fNeuLand = false; // NeuLand Detector
    TString fNeuLandGeo = "neuland_v12a_14m.geo.root";

    // ---- End of Configuration area   ---------------------------------------

    // ---- Stable part   -----------------------------------------------------
    TString dir = gSystem->Getenv("VMCWORKDIR");
    TString r3bdir = dir + "/macros/";
    r3bdir.ReplaceAll("//", "/");

    TString r3b_geomdir = dir + "/geometry/";
    gSystem->Setenv("GEOMPATH", r3b_geomdir.Data());
    r3b_geomdir.ReplaceAll("//", "/");

    TString r3b_confdir = dir + "/gconfig/";
    gSystem->Setenv("CONFIG_DIR", r3b_confdir.Data());
    r3b_confdir.ReplaceAll("//", "/");

    char str[1000];
    sprintf(str, "GEOMPATH=%s/sofia/geometry", dir.Data());
    putenv(str);

    // ----    Debug option   -------------------------------------------------
    gDebug = 0;

    // -----   Timer   --------------------------------------------------------
    TStopwatch timer;
    timer.Start();

    // -----   Create simulation run   ----------------------------------------
    FairRunSim* run = new FairRunSim();
    run->SetName(fMC);                           // Transport engine
    run->SetSink(new FairRootFileSink(OutFile)); // Output file
    FairRuntimeDb* rtdb = run->GetRuntimeDb();

    //  R3B Special Physics List in G4 case
    if ((fUserPList) && (fMC.CompareTo("TGeant4") == 0))
    {
        run->SetUserConfig("g4R3bConfig.C");
        run->SetUserCuts("SetCuts.C");
    }

    // -----   Create media   -------------------------------------------------
    run->SetMaterials("media_r3b.geo"); // Materials

    // -----   Create R3B geometry --------------------------------------------

    // Cave definition
    FairModule* cave = new R3BCave("CAVE");
    cave->SetGeometryFileName("r3b_cave_vacuum.geo");
    run->AddModule(cave);

    // R3B-Music definition
    if (fR3BMusic)
    {
        run->AddModule(new R3BMusic(fR3BMusicGeo, { 0., 0., -160.5 }));
    }

    // MWPC0 definition
    if (fMwpc0)
    {
        run->AddModule(new R3BSofMwpc0(fMwpc0Geo, { 0., 0., -110. }));
    }

    // Tracker, vacuum chamber and LH2 target definitions
    if (fTracker)
    {
        R3BTra* tra = new R3BTra(fTrackerGeo, { 0., 0., -65.5 });
        tra->SetEnergyCut(1e-6); // 1 keV
        run->AddModule(tra);
    }

    // CALIFA Calorimeter
    if (fCalifa)
    {
        R3BCalifa* califa = new R3BCalifa(fCalifaGeo, { 0., 0., -66.5 });
        califa->SelectGeometryVersion(fCalifaGeoVer);
        califa->SetNonUniformity(fCalifaNonU);
        run->AddModule(califa);
    }

    // MWPC1 definition
    if (fMwpc1)
    {
        run->AddModule(new R3BSofMwpc1(fMwpc1Geo, { 0., 0., 16. }));
    }

    // Twim definition
    if (fTwim)
    {
        run->AddModule(new R3BSofTWIM(fTwimGeo, { 2., 0., 50. }));
    }

    // MWPC2 definition
    if (fMwpc2)
    {
        run->AddModule(new R3BSofMwpc2(fMwpc2Geo, { 0., 0., 95. }));
    }

    // Aladin Magnet definition
    if (fAladin && !fGlad)
    {
        fFieldMap = 0;
        run->AddModule(new R3BAladinMagnet(fAladinGeo));
    }

    // Glad Magnet definition
    if (fGlad && !fAladin)
    {
        fFieldMap = 1;
        // run->AddModule(new R3BGladMagnet(fGladGeo));
    }

    // MWPC3 definition
    if (fMwpc3)
    {
        TGeoRotation* rmw3 = new TGeoRotation("Mwpc3rot");
        rmw3->RotateY(-29.0);
        run->AddModule(new R3BSofMwpc3(fMwpc3Geo, { -243., 0., 689., rmw3 }));
    }

    // Sofia ToF-Wall definition
    if (fSofTofWall)
    {
        TGeoRotation* rtof = new TGeoRotation("Tofrot");
        rtof->RotateY(-29.0);
        run->AddModule(new R3BSofTofWall(fSofTofWallGeo, { -257., 0., 710., rtof }));
    }

    // NeuLand Scintillator Detector
    if (fNeuLand)
    {
        run->AddModule(new R3BLand(fNeuLandGeo));
    }

    // ----- Create R3B  magnetic field ---------------------------------------
    // If the Global Position of the Magnet is changed
    // the Field Map has to be transformed accordingly
    if (fFieldMap == 0)
    {
        R3BAladinFieldMap* magField = new R3BAladinFieldMap("AladinMaps");
        magField->SetCurrent(fMeasCurrent);
        magField->SetScale(fFieldScale);

        if (fR3BMagnet == kTRUE)
        {
            run->SetField(magField);
            R3BFieldPar* fieldPar = (R3BFieldPar*)rtdb->getContainer("R3BFieldPar");
            fieldPar->SetParameters(magField);
            fieldPar->setChanged();
        }
        else
        {
            run->SetField(NULL);
        }
    }
    else if (fFieldMap == 1)
    {
        R3BGladFieldMap* magField = new R3BGladFieldMap("R3BGladMap");
        magField->SetScale(fFieldScale);

        if (fR3BMagnet == kTRUE)
        {
            run->SetField(magField);
            R3BFieldPar* fieldPar = (R3BFieldPar*)rtdb->getContainer("R3BFieldPar");
            fieldPar->SetParameters(magField);
            fieldPar->setChanged();
        }
        else
        {
            run->SetField(NULL);
        }
    }
    // ---- End of field map section

    // -----   Create PrimaryGenerator   --------------------------------------

    // 1 - Create the Main API class for the Generator
    FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

    if (fGenerator.CompareTo("box") == 0)
    {
        // 2- Define the BOX generator
        Int_t pdgId = 2212;     // proton beam
        Double32_t theta1 = 0.; // polar angle distribution
        Double32_t theta2 = 2.;
        Double32_t momentum = 1.;
        FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
        boxGen->SetThetaRange(theta1, theta2);
        boxGen->SetPRange(momentum, momentum * 1.2);
        boxGen->SetPhiRange(0, 360);
        boxGen->SetXYZ(0.0, 0.0, -1.5);
        // primGen->AddGenerator(boxGen);

        // 128-Sn fragment
        R3BIonGenerator* ionGen = new R3BIonGenerator(50, 132, 50, 1, 0.0, 0., 0.951);
        ionGen->SetSpotRadius(0.0, -65.5, 0.);
        primGen->AddGenerator(ionGen);

        // 128-Sn fragment
        R3BIonGenerator* ionGen1 = new R3BIonGenerator(18, 39, 18, 1, 0., 0., 0.951);
        ionGen1->SetSpotRadius(0.0, -65.5, 0.);
        //primGen->AddGenerator(ionGen1);

        R3BIonGenerator* ionGen2 = new R3BIonGenerator(17, 39, 17, 1, -0.05, 0., 0.951);
        ionGen2->SetSpotRadius(0.0, -65.5, 0.);
        //primGen->AddGenerator(ionGen2);

        // neutrons
        FairBoxGenerator* boxGen_n = new FairBoxGenerator(2112, 3);
        boxGen_n->SetThetaRange(theta1, theta2);
        boxGen_n->SetPRange(momentum, momentum * 1.2);
        boxGen_n->SetPhiRange(0, 360);
        boxGen_n->SetXYZ(0.0, 0.0, -1.5);
        // primGen->AddGenerator(boxGen_n);
    }

    if (fGenerator.CompareTo("ascii") == 0)
    {
        R3BAsciiGenerator* gen = new R3BAsciiGenerator((dir + "/sofia/input/" + fEventFile).Data());
        primGen->AddGenerator(gen);
    }

    if (fGenerator.CompareTo("r3b") == 0)
    {
        Int_t pdg = 2212;
        Float_t beamEnergy = 1.;
        R3BSpecificGenerator* pR3bGen = new R3BSpecificGenerator(pdg, beamEnergy);

        // R3bGen properties
        pR3bGen->SetBeamInteractionFlag("off");
        pR3bGen->SetBeamInteractionFlag("off");
        pR3bGen->SetRndmFlag("off");
        pR3bGen->SetRndmEneFlag("off");
        pR3bGen->SetBoostFlag("off");
        pR3bGen->SetReactionFlag("on");
        pR3bGen->SetGammasFlag("off");
        pR3bGen->SetDecaySchemeFlag("off");
        pR3bGen->SetDissociationFlag("off");
        pR3bGen->SetBackTrackingFlag("off");
        pR3bGen->SetSimEmittanceFlag("off");

        // R3bGen Parameters
        pR3bGen->SetSigmaBeamEnergy(1.e-03); // Sigma(Ebeam) GeV
        pR3bGen->SetEnergyPrim(0.3);         // Particle Energy in MeV
        Int_t fMultiplicity = 50;
        pR3bGen->SetNumberOfParticles(fMultiplicity); // Mult.

        // Reaction type
        //        1: "Elas"
        //        2: "iso"
        //        3: "Trans"
        pR3bGen->SetReactionType("Elas");

        // Target  type
        //        1: "LeadTarget"
        //        2: "Parafin0Deg"
        //        3: "Parafin45Deg"
        //        4: "LiH"

        TString fTargetType = "LiH"; // Target selection: LeadTarget, Para, Para45, LiH

        pR3bGen->SetTargetType(fTargetType.Data());
        Double_t thickness = (0.11 / 2.) / 10.;         // cm
        pR3bGen->SetTargetHalfThicknessPara(thickness); // cm
        pR3bGen->SetTargetThicknessLiH(3.5);            // cm
        pR3bGen->SetTargetRadius(1.);                   // cm

        pR3bGen->SetSigmaXInEmittance(1.);          // cm
        pR3bGen->SetSigmaXPrimeInEmittance(0.0001); // cm

        // Dump the User settings
        pR3bGen->PrintParameters();
        primGen->AddGenerator(pR3bGen);
    }

    run->SetGenerator(primGen);

    //-------Set visualisation flag to true------------------------------------
    run->SetStoreTraj(fVis);

    FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

    // ----- Initialize Califa HitFinder task (from CrystalCal Level to Hit Level)
    if (fCalifaHitFinder)
    {
        R3BCalifaCrystalCal2Hit* califaHF = new R3BCalifaCrystalCal2Hit();
        // califaHF->SetDetectionThreshold(0.000050);//50 KeV
        // califaHF->SetExperimentalResolution(5.);  //5% at 1 MeV
        // califaHF->SetAngularWindow(3.2,3.2);      //[0.25 around 14.3 degrees, 3.2 for the complete calorimeter]
        run->AddTask(califaHF);
    }

    // -----   Initialize simulation run   ------------------------------------
    run->Init();

    // -----   Runtime database   ---------------------------------------------
    Bool_t kParameterMerged = kTRUE;
    FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
    parOut->open(ParFile.Data());
    rtdb->setOutput(parOut);
    rtdb->saveOutput();
    rtdb->print();

    // -----   Start run   ----------------------------------------------------
    if (nEvents > 0)
        run->Run(nEvents);

    // -----   Finish   -------------------------------------------------------
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    cout << endl << endl;
    cout << "Macro finished succesfully." << endl;
    cout << "Output file is " << OutFile << endl;
    cout << "Parameter file is " << ParFile << endl;
    cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << endl << endl;

    cout << " Test passed" << endl;
    cout << " All ok " << endl;
}
