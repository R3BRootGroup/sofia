/** --------------------------------------------------------------------
 **
 **  Define the SOFIA simulation setup for p2p-fission experiments
 **  Author: <joseluis.rodriguez.sanchez@usc.es>
 **
 **  Last Update: 26/12/19
 **  Comments:
 **         - 29/10/19 : Initial setup
 **         - 26/12/19 : Added new califa tasks and mwpc0 geometry
 **         - 05/01/21 : Added container with detector parameters
 **
 **  Configuration:
 **  (1) Select the right generator "fGenerator"
 **  (2) Select the detectors that you wish for the simulation, for instance, "fCalifa = true"
 **  (3) Look at the file "s455_setup.par" that the positions of your detectors are right
 **
 **  Execute it as follows:
 **  root -l 'runsim.C(1000)'
 **  where 1000 means the number of events
 **
 **/

void runsim(Int_t nEvents = 0)
{
    // ----------- Configuration area ----------------------------------

    TString OutFile = "sim.root"; // Output file for data
    TString ParFile = "par.root"; // Output file for params

    Bool_t fVis = true;             // Store tracks for visualization
    Bool_t fUserPList = false;      // Use of R3B special physics list
    Bool_t fR3BMagnet = false;      // Magnetic field definition
    Bool_t fCalifaDigitizer = true; // Apply hit digitizer task
    Bool_t fCalifaHitFinder = true; // Apply hit finder task
    Bool_t fSofiaDigitizer = true;  // Apply hit digitizer task

    // MonteCarlo engine: TGeant4, TFluka           --------------------
    TString fMC = "TGeant4";

    // Event generator type: box for particles or ascii&inclroot for p2p-fission
    TString generator1 = "box";
    TString generator2 = "ascii";
    TString generator3 = "inclroot";
    TString fGenerator = generator2;

    // Input event file in the case of ascii generator
    TString fEventFile;
    if (fGenerator.CompareTo("ascii") == 0)
        fEventFile = "p2p_U238_500.txt";
    else if (fGenerator.CompareTo("inclroot") == 0)
        fEventFile = "p_U236_650.root";

    Int_t fFieldMap = -1;          // Magentic field map selector
    Double_t fMeasCurrent = 2000.; // Magnetic field current
    Float_t fFieldScale = -0.82;   // Magnetic field scale factor

    // ---------  Detector selection: true - false ---------------------
    // ---- R3B and SOFIA detectors as well as passive elements

    Bool_t fR3BMusic = false; // R3B Music Detector
    TString fR3BMusicGeo = "music_s467.geo.root";

    Bool_t fR3BTripleMusic = true; // R3B Triple-Music Detector
    TString fR3BTripleMusicGeo = "trim_v21.geo.root";

    Bool_t fMwpc0 = true; // MWPC0 Detector
    TString fMwpc0Geo = "mwpc_0.geo.root";

    Bool_t fSciStart = true; // Start Detector
    TString fSciStartGeo = "sof_sci_v21.geo.root";

    Bool_t fTracker = true; // AMS-Tracker + Vacuum chamber + LH2 target
    TString fTrackerGeo = "target_area_s455_v21.geo.root";

    Bool_t fCalifa = true; // Califa Calorimeter
    TString fCalifaGeo = "califa_2020.geo.root";
    Int_t fCalifaGeoVer = 2020;
    Double_t fCalifaCryTh = 0.000080;  // In GeV: 0.000080 means 80 keV per crystal
    Double_t fCalifaHitEnergyTh = 0.0; // Threshold in GeV per cluster hit
    Double_t fCalifaNonU = 1.0;        // Non-uniformity: 1 means +-1% max deviation

    Bool_t fMwpc1 = true; // MWPC1 Detector
    TString fMwpc1Geo = "mwpc_1.geo.root";

    Bool_t fTwim = true; // Twin-Music Detector
    TString fTwimGeo = "twinmusic_v21.geo.root";

    Bool_t fMwpc2 = true; // MWPC2 Detector
    TString fMwpc2Geo = "mwpc_2.geo.root";

    Bool_t fAladin = false; // Aladin Magnet
    TString fAladinGeo = "aladin_v13a.geo.root";

    Bool_t fGlad = true; // Glad Magnet
    TString fGladGeo = "glad_s455.geo.root";

    Bool_t fMwpc3 = true; // MWPC3 Detector
    TString fMwpc3Geo = "mwpc_3.geo.root";

    Bool_t fSofTofWall = true; // Sofia ToF-Wall
    TString fSofTofWallGeo = "sof_tof_v21.geo.root";

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

    // ----    Debug option   -------------------------------------------------
    gDebug = 0;

    // -----   Timer   --------------------------------------------------------
    TStopwatch timer;
    timer.Start();

    // -----   Create simulation run   ----------------------------------------
    FairRunSim* run = new FairRunSim();
    run->SetName(fMC);                           // Transport engine
    run->SetSink(new FairRootFileSink(OutFile)); // Output file

    // -----   Runtime data base   --------------------------------------------
    FairRuntimeDb* rtdb = run->GetRuntimeDb();

    // -----   Load detector parameters    ------------------------------------
    FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
    parIo1->open("s455_setup.par", "in");
    rtdb->setFirstInput(parIo1);
    rtdb->print();

    // ----- Containers
    R3BTGeoPar* mwpc0Par = NULL;
    R3BTGeoPar* trimPar = NULL;
    R3BTGeoPar* scistartPar = NULL;
    R3BTGeoPar* targetPar = NULL;
    R3BTGeoPar* califaPar = NULL;
    R3BTGeoPar* mwpc1Par = NULL;
    R3BTGeoPar* twimPar = NULL;
    R3BTGeoPar* mwpc2Par = NULL;
    R3BTGeoPar* mwpc3Par = NULL;
    R3BTGeoPar* tofwPar = NULL;

    targetPar = (R3BTGeoPar*)rtdb->getContainer("TargetGeoPar");
    if (fMwpc0)
        mwpc0Par = (R3BTGeoPar*)rtdb->getContainer("Mwpc0GeoPar");
    if (fR3BTripleMusic)
        trimPar = (R3BTGeoPar*)rtdb->getContainer("TrimGeoPar");
    if (fSciStart)
        scistartPar = (R3BTGeoPar*)rtdb->getContainer("SofSciGeoPar");
    if (fCalifa)
        califaPar = (R3BTGeoPar*)rtdb->getContainer("CalifaGeoPar");
    if (fMwpc1)
        mwpc1Par = (R3BTGeoPar*)rtdb->getContainer("Mwpc1GeoPar");
    if (fTwim)
        twimPar = (R3BTGeoPar*)rtdb->getContainer("TwimGeoPar");
    if (fMwpc2)
        mwpc2Par = (R3BTGeoPar*)rtdb->getContainer("Mwpc2GeoPar");
    if (fMwpc3)
        mwpc3Par = (R3BTGeoPar*)rtdb->getContainer("Mwpc3GeoPar");
    if (fSofTofWall)
        tofwPar = (R3BTGeoPar*)rtdb->getContainer("TofwGeoPar");

    UInt_t runId = 1;
    rtdb->initContainers(runId);

    // -----   R3B Special Physics List in G4 case
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
    cave->SetGeometryFileName("r3b_cave.geo");
    run->AddModule(cave);

    // MWPC0 definition
    if (fMwpc0)
    {
        if (mwpc0Par)
        {
            mwpc0Par->printParams();
            TGeoRotation* rmwpc0 = new TGeoRotation("Mwpc0rot");
            rmwpc0->RotateX(mwpc0Par->GetRotX());
            rmwpc0->RotateY(mwpc0Par->GetRotY());
            rmwpc0->RotateZ(mwpc0Par->GetRotZ());
            run->AddModule(
                new R3BSofMwpc0(fMwpc0Geo, { mwpc0Par->GetPosX(), mwpc0Par->GetPosY(), mwpc0Par->GetPosZ(), rmwpc0 }));
        }
        else
            run->AddModule(new R3BSofMwpc0(fMwpc0Geo, { 0., 0., -190. }));
        R3BSofMwpcDigitizer* mw0_digitizer = new R3BSofMwpcDigitizer("Mwpc0", 1);
        if (fSofiaDigitizer)
            run->AddTask(mw0_digitizer);
    }

    // R3B-Music definition
    if (fR3BMusic)
    {
        run->AddModule(new R3BMusic(fR3BMusicGeo, { 0., 0., -220.5 }));
    }

    // R3B-Triple-Music definition
    if (fR3BTripleMusic)
    {
        if (trimPar)
        {
            trimPar->printParams();
            TGeoRotation* rtrim = new TGeoRotation("Trimrot");
            rtrim->RotateX(trimPar->GetRotX());
            rtrim->RotateY(trimPar->GetRotY());
            rtrim->RotateZ(trimPar->GetRotZ());
            run->AddModule(new R3BSofTrim(fR3BTripleMusicGeo,
                                          { trimPar->GetPosX(), trimPar->GetPosY(), trimPar->GetPosZ(), rtrim }));
        }
        else
            run->AddModule(new R3BSofTrim(fR3BTripleMusicGeo, { 0., 0., -220.5 }));
    }

    // Start scintillator
    if (fSciStart)
    {
        if (scistartPar)
        {
            scistartPar->printParams();
            TGeoRotation* rsci = new TGeoRotation("Scirot");
            rsci->RotateX(scistartPar->GetRotX());
            rsci->RotateY(scistartPar->GetRotY());
            rsci->RotateZ(scistartPar->GetRotZ());
            run->AddModule(new R3BSofSci(
                fSciStartGeo, { scistartPar->GetPosX(), scistartPar->GetPosY(), scistartPar->GetPosZ(), rsci }));
        }
        else
            run->AddModule(new R3BSofSci(fSciStartGeo, { 0., 0., -150. }));
    }

    // Tracker, vacuum chamber and LH2 target definitions
    if (fTracker)
    {
        if (targetPar)
        {
            targetPar->printParams();
            TGeoRotation* rtarget = new TGeoRotation("Targetrot");
            rtarget->RotateX(targetPar->GetRotX());
            rtarget->RotateY(targetPar->GetRotY());
            rtarget->RotateZ(targetPar->GetRotZ());
            R3BTra* tra =
                new R3BTra(fTrackerGeo, { targetPar->GetPosX(), targetPar->GetPosY(), targetPar->GetPosZ(), rtarget });
            tra->SetEnergyCut(1e-6); // 1 keV
            run->AddModule(tra);
        }
        else
        {
            R3BTra* tra = new R3BTra(fTrackerGeo, { 0., 0., -0.75 - 65. });
            tra->SetEnergyCut(1e-6); // 1 keV
            run->AddModule(tra);
        }
    }

    // CALIFA Calorimeter
    if (fCalifa)
    {
        if (califaPar)
        {
            califaPar->printParams();
            TGeoRotation* rcalifa = new TGeoRotation("Califarot");
            rcalifa->RotateX(califaPar->GetRotX());
            rcalifa->RotateY(califaPar->GetRotY());
            rcalifa->RotateZ(califaPar->GetRotZ());
            R3BCalifa* califa = new R3BCalifa(
                fCalifaGeo, { califaPar->GetPosX(), califaPar->GetPosY(), califaPar->GetPosZ(), rcalifa });
            califa->SelectGeometryVersion(fCalifaGeoVer);
            run->AddModule(califa);
        }
        else
        {

            R3BCalifa* califa = new R3BCalifa(fCalifaGeo, { 0., 0., -65. });
            califa->SelectGeometryVersion(fCalifaGeoVer);
            run->AddModule(califa);
        }
        
        
            // ----- Initialize CalifaDigitizer task (from Point Level to Cal Level)
    if (fCalifaDigitizer)
    {
        R3BCalifaDigitizer* califaDig = new R3BCalifaDigitizer();
        califaDig->SetNonUniformity(fCalifaNonU);
        califaDig->SetExpEnergyRes(5.0); // 5. means 5% at 1 MeV
        califaDig->SetComponentRes(5.0);
        califaDig->SetDetectionThreshold(fCalifaHitEnergyTh);
        run->AddTask(califaDig);
        
    // ----- Initialize Califa HitFinder task (from CrystalCal Level to Hit Level)
    if (fCalifaHitFinder)
    {
        R3BCalifaCrystalCal2Hit* califaHF = new R3BCalifaCrystalCal2Hit();
        califaHF->SetCrystalThreshold(fCalifaCryTh);
        califaHF->SetSquareWindowAlg(0.25, 0.25); //[0.25 around 14.3 degrees, 3.2 for the complete calorimeter]
        run->AddTask(califaHF);
    }
    }
        
        
        
    }

    // MWPC1 definition
    if (fMwpc1)
    {
        if (mwpc1Par)
        {
            mwpc1Par->printParams();
            TGeoRotation* rmwpc1 = new TGeoRotation("Mwpc1rot");
            rmwpc1->RotateX(mwpc1Par->GetRotX());
            rmwpc1->RotateY(mwpc1Par->GetRotY());
            rmwpc1->RotateZ(mwpc1Par->GetRotZ());
            run->AddModule(
                new R3BSofMwpc1(fMwpc1Geo, { mwpc1Par->GetPosX(), mwpc1Par->GetPosY(), mwpc1Par->GetPosZ(), rmwpc1 }));
        }
        else
            run->AddModule(new R3BSofMwpc1(fMwpc1Geo, { 0., 0., 42. }));
        R3BSofMwpcDigitizer* mw1_digitizer = new R3BSofMwpcDigitizer("Mwpc1", 1);
        if (fSofiaDigitizer)
            run->AddTask(mw1_digitizer);
    }

    // Twim definition
    if (fTwim)
    {
        if (twimPar)
        {
            twimPar->printParams();
            TGeoRotation* rtwim = new TGeoRotation("Twimrot");
            rtwim->RotateX(twimPar->GetRotX());
            rtwim->RotateY(twimPar->GetRotY());
            rtwim->RotateZ(twimPar->GetRotZ());
            run->AddModule(
                new R3BSofTwim(fTwimGeo, { twimPar->GetPosX(), twimPar->GetPosY(), twimPar->GetPosZ(), rtwim }));
        }
        else
            run->AddModule(new R3BSofTwim(fTwimGeo, { 0., 0., 69. }));
        R3BSofTwimDigitizer* twim_digitizer = new R3BSofTwimDigitizer("Twim", 1);
        if (fSofiaDigitizer)
            run->AddTask(twim_digitizer);
    }

    // MWPC2 definition
    if (fMwpc2)
    {
        if (mwpc2Par)
        {
            mwpc2Par->printParams();
            TGeoRotation* rmwpc2 = new TGeoRotation("Mwpc2rot");
            rmwpc2->RotateX(mwpc2Par->GetRotX());
            rmwpc2->RotateY(mwpc2Par->GetRotY());
            rmwpc2->RotateZ(mwpc2Par->GetRotZ());
            run->AddModule(
                new R3BSofMwpc2(fMwpc2Geo, { mwpc2Par->GetPosX(), mwpc2Par->GetPosY(), mwpc2Par->GetPosZ(), rmwpc2 }));
        }
        else
            run->AddModule(new R3BSofMwpc2(fMwpc2Geo, { 0., 0., 100. }));
        R3BSofMwpcDigitizer* mw2_digitizer = new R3BSofMwpcDigitizer("Mwpc2", 1);
        if (fSofiaDigitizer)
            run->AddTask(mw2_digitizer);
    }

    // Aladin Magnet definition
    if (fAladin && !fGlad)
    {
        fFieldMap = 0;
        fR3BMagnet = true;
        run->AddModule(new R3BAladinMagnet(fAladinGeo));
    }

    // Glad Magnet definition
    if (fGlad && !fAladin)
    {
        fFieldMap = 1;
        fR3BMagnet = true;
        run->AddModule(new R3BGladMagnet(fGladGeo));
    }

    // MWPC3 definition
    if (fMwpc3)
    {
        TGeoRotation* rmwpc3 = new TGeoRotation("Mwpc3rot");
        if (mwpc3Par)
        {
            mwpc3Par->printParams();
            rmwpc3->RotateX(mwpc3Par->GetRotX());
            rmwpc3->RotateY(mwpc3Par->GetRotY());
            rmwpc3->RotateZ(mwpc3Par->GetRotZ());
            run->AddModule(
                new R3BSofMwpc3(fMwpc3Geo, { mwpc3Par->GetPosX(), mwpc3Par->GetPosY(), mwpc3Par->GetPosZ(), rmwpc3 }));
        }
        else
        {
            rmwpc3->RotateY(-18.);
            run->AddModule(new R3BSofMwpc3(fMwpc3Geo, { -300., 0., 749., rmwpc3 }));
        }
        R3BSofMwpcDigitizer* mw3_digitizer = new R3BSofMwpcDigitizer("Mwpc3", 1);
        if (fSofiaDigitizer)
            run->AddTask(mw3_digitizer);
    }

    // Sofia ToF-Wall definition
    if (fSofTofWall)
    {
        TGeoRotation* rtof = new TGeoRotation("Tofrot");
        if (tofwPar)
        {
            tofwPar->printParams();
            rtof->RotateX(tofwPar->GetRotX());
            rtof->RotateY(tofwPar->GetRotY());
            rtof->RotateZ(tofwPar->GetRotZ());
            run->AddModule(
                new R3BSofTofW(fSofTofWallGeo, { tofwPar->GetPosX(), tofwPar->GetPosY(), tofwPar->GetPosZ(), rtof }));
        }
        else
        {
            rtof->RotateY(-18.);
            run->AddModule(new R3BSofTofW(fSofTofWallGeo, { -330., 0., 817., rtof }));
        }
        R3BSofTofWDigitizer* tofw_digitizer = new R3BSofTofWDigitizer();
        if (fSofiaDigitizer)
            run->AddTask(tofw_digitizer);
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
        // Define the BOX generator
        Int_t pdgId = 2212;      // proton beam
        Double32_t theta1 = 22.; // polar angle distribution
        Double32_t theta2 = 90.;
        Double32_t momentum = 0.8;
        FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
        boxGen->SetThetaRange(theta1, theta2);
        boxGen->SetPRange(momentum, 2.0 * momentum);
        boxGen->SetPhiRange(0., 360.);
        boxGen->SetXYZ(0.0, 0.0, -65.0);
        // primGen->AddGenerator(boxGen);

        // 208-Pb fragment
        FairIonGenerator* ionGen = new FairIonGenerator(82, 208, 82, 1, 0., 0., 1.09, 0., 0., -75.);
        primGen->AddGenerator(ionGen);
    }

    if (fGenerator.CompareTo("ascii") == 0)
    {
        R3BAsciiGenerator* gen = new R3BAsciiGenerator((dir + "/sofia/input/" + fEventFile).Data());
        gen->SetXYZ(targetPar->GetPosX(), targetPar->GetPosY(), targetPar->GetPosZ());
        gen->SetDxDyDz(0., 0., 0.);
        primGen->AddGenerator(gen);
    }

    if (fGenerator.CompareTo("inclroot") == 0)
    {
        R3BINCLRootGenerator* gen = new R3BINCLRootGenerator((dir + "/sofia/input/" + fEventFile).Data());
        gen->SetOnlyFission(kTRUE);
        // gen->SetOnlyP2pFission(kTRUE);
        gen->SetXYZ(targetPar->GetPosX(), targetPar->GetPosY(), targetPar->GetPosZ());
        gen->SetDxDyDz(0., 0., 0.);
        primGen->AddGenerator(gen);
    }

    run->SetGenerator(primGen);

    //-------Set visualisation flag to true------------------------------------
    run->SetStoreTraj(fVis);

    FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

    if (fMwpc1&&fMwpc2&&fMwpc3&&fSofTofWall&&fSofiaDigitizer)
    {
        R3BSofFissionAnalysis* fissiontracking = new R3BSofFissionAnalysis();
        run->AddTask(fissiontracking);
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
    Double_t rtime = timer.RealTime() / 60.;
    Double_t ctime = timer.CpuTime() / 60.;
    cout << endl << endl;
    cout << "Macro finished succesfully." << endl;
    cout << "Output file is " << OutFile << endl;
    cout << "Parameter file is " << ParFile << endl;
    cout << "Real time " << rtime << " min, CPU time " << ctime << " min" << endl << endl;

    cout << " Test passed" << endl;
    cout << " All ok " << endl;
    gApplication->Terminate();
}
