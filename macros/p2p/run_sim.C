
//--------------------------------------------------------------------
//
// Define the SOFIA simulation Setup for p2p-fission experiments
// Author: <joseluis.rodriguez.sanchez@usc.es>
//
// Last Update: 14/12/17 (Jose Luis)
// Comments:
//         - 14/12/17 : Initial setup
//
//
//--------------------------------------------------------------------

void run_sim(Int_t nEvents = 0)
{
  //-------------------------------------------------
  // Monte Carlo type     |           (TString)
  //-------------------------------------------------
    TString transport = "TGeant4";

  // Output files
    TString outFile = "sim.root";
    TString parFile = "par.root";

  // Magnetic field
    Bool_t magnet = kTRUE;
    Float_t fieldScale = -1.0;

  //-------------------------------------------------
  // Primaries generation
  // Event Generator Type |        (TString)
  //-------------------------------------------------
    TString generator1 = "box";
    TString generator2 = "ascii";
    TString generator3 = "r3b";
    TString generator = generator1;
    //TString inputFile = "p2p_U238_300.txt";
    TString inputFile = "p2p_U238_500.txt";

    Bool_t storeTrajectories = kTRUE;
    //Int_t randomSeed = 335566; // 0 for time-dependent random numbers

    // Target type
    Bool_t  fTarget = false;
    TString target1 = "LeadTarget";
    TString target2 = "Para";
    TString target3 = "Para45";
    TString target4 = "LiH";
    TString targetType = target4;

    Bool_t  fTracker = true;          // Tracker
    TString fTrackerGeo = "targetvacuumchamber_ams_s455.geo.root";

    // ------------------------------------------------------------------------
    // Stable part ------------------------------------------------------------

    TString dir = getenv("VMCWORKDIR");
    TString geodir = dir+"/sofia/geometry/";
    TString inputdir = dir+"/sofia/input/";

    // ----    Debug option   -------------------------------------------------
    gDebug = 0;

    // -----   Timer   --------------------------------------------------------
    TStopwatch timer;
    timer.Start();

    // -----   Create simulation run   ----------------------------------------
    FairRunSim* run = new FairRunSim();
    run->SetName(transport);                     // Transport engine
    run->SetSink(new FairRootFileSink(outFile)); // Output file
    FairRuntimeDb* rtdb = run->GetRuntimeDb();

    // -----   Create media   -------------------------------------------------
    run->SetMaterials("media_r3b.geo"); // Materials

    // -----   Create R3B geometry --------------------------------------------
    // R3B Cave definition
    FairModule* cave = new R3BCave("CAVE");
    cave->SetGeometryFileName("r3b_cave.geo");
    run->AddModule(cave);

    // To skip the detector comment out the line with: run->AddModule(...

    //Vacuum chamber and target definition
    if(fTarget && !fTracker){
    TString fVacuumChamberGeo = "targetvacuumchamber_s455.geo.root";
    run->AddModule(new R3BVacVesselCool(targetType,geodir+fVacuumChamberGeo, { 0., 0., -65.5 }));
    }

    // Tracker
    if (fTracker) {
    R3BTra* tra = new R3BTra(geodir+fTrackerGeo, { 0., 0., -65.5 });
    tra->SetEnergyCut(1e-6);    
    run->AddModule(tra);
    }

    // GLAD
    run->AddModule(new R3BGladMagnet("glad_v17_flange.geo.root")); // GLAD should not be moved or rotated

    // R3B detectors
    // PSP
    //run->AddModule(new R3BPsp("psp_v13a.geo.root", {}, -221., -89., 94.1));

    // STaRTrack
    //run->AddModule(new R3BStartrack("startrack_v16-300_2layers.geo.root", { 0., 0., -50. +20.}));
    //run->AddModule(new R3BSTaRTra("startra_v13a.geo.root", { 0., 0., -50. }));

    // CALIFA
    R3BCalifa* califa = new R3BCalifa("califa_demo.geo.root", { 0., 0., -65.5 });
    califa->SelectGeometryVersion(10);
    // Selecting the Non-uniformity of the crystals (1 means +-1% max deviation)
    califa->SetNonUniformity(1.0);
    run->AddModule(califa);

    // NeuLAND
    //run->AddModule(new R3BLand("neuland_s2018.geo.root", { 0., 0., 1400. + 12 * 5. }));

    // --- SOFIA + (p2p) detectors ---

    //run->AddModule(new R3BSofMwpc0(geodir+"mwpc_0.geo.root", { 0., 0., -110. }));

    run->AddModule(new R3BSofMwpc1(geodir+"mwpc_1.geo.root", { 0., 0., 16. }));

    run->AddModule(new R3BSofTWIM(geodir+"twinmusic_v0.geo.root", { 0., 0., 50. }));

    run->AddModule(new R3BSofMwpc2(geodir+"mwpc_2.geo.root", { 0., 0., 95. }));

    run->AddModule(new R3BSofMwpc3(geodir+"mwpc_3.geo.root"));

    run->AddModule(new R3BSofTofWall(geodir+"sof_tof_v19.geo.root"));

    // -----   Create R3B  magnetic field ----------------------------------------
    // NB: <D.B>
    // If the Global Position of the Magnet is changed
    // the Field Map has to be transformed accordingly
    R3BGladFieldMap* magField = new R3BGladFieldMap("R3BGladMap");
    magField->SetScale(fieldScale);

    if (magnet == kTRUE)
    {
        run->SetField(magField);
    }
    else
    {
        run->SetField(NULL);
    }

    // -----   Create PrimaryGenerator   --------------------------------------
    // 1 - Create the Main API class for the Generator
    FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

    if (generator.CompareTo("box") == 0)
    {
        // 2- Define the BOX generator
        Int_t pdgId = 2212;     // proton beam
        Double32_t theta1 = 22.; // polar angle distribution
        Double32_t theta2 = 85.;
        Double32_t momentum = 1.5;
        FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
        boxGen->SetThetaRange(theta1, theta2);
        boxGen->SetPRange(momentum, momentum * 1.2);
        boxGen->SetPhiRange(0.,360.);
        boxGen->SetXYZ(0.0, 0.0, -65.5);
        //primGen->AddGenerator(boxGen);

        // 128-Sn fragment
        R3BIonGenerator* ionGen = new R3BIonGenerator(50, 128, 50, 1, 0., 0., 1.39/1.3-1.39/1.3*0.01);
        ionGen->SetSpotRadius(0.1, +65.5, 0.);
        primGen->AddGenerator(ionGen);

        // neutrons
        FairBoxGenerator* boxGen_n = new FairBoxGenerator(2112, 3);
        boxGen_n->SetThetaRange(theta1, theta2);
        boxGen_n->SetPRange(momentum, momentum * 1.2);
        boxGen_n->SetPhiRange(0, 360);
        boxGen_n->SetXYZ(0.0, 0.0, -1.5);
        //primGen->AddGenerator(boxGen_n);
    }

    if (generator.CompareTo("ascii") == 0)
    {
        R3BAsciiGenerator* gen = new R3BAsciiGenerator(inputdir+inputFile);
        gen->SetXYZ(0.,0.,-65.5);
        //gen->SetDxDyDz(0.45,0.45,1.2/2.0);
        primGen->AddGenerator(gen);


        // 238-U fragment at 500AMeV
        R3BIonGenerator* ionGen = new R3BIonGenerator(92, 238, 92, 1, 0., 0., 258.);
        ionGen->SetSpotRadius(0.1, -65.5, 0.);
       // primGen->AddGenerator(ionGen);


    }

    if (generator.CompareTo("r3b") == 0)
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

        pR3bGen->SetTargetType(targetType.Data());
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

    run->SetStoreTraj(storeTrajectories);

    FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
    //FairLogger::GetLogger()->SetLogScreenLevel("INFO");

    // Add analysis task ------------------------------------
    //R3BAmsStripCal2Hit* Cal2Hit = new R3BAmsStripCal2Hit();
    //run->AddTask(Cal2Hit);

    // -----   Initialize simulation run   ------------------------------------
    run->Init();


    // -----   Runtime database   ---------------------------------------------
    R3BFieldPar* fieldPar = (R3BFieldPar*)rtdb->getContainer("R3BFieldPar");
    if (NULL != magField)
    {
        fieldPar->SetParameters(magField);
        fieldPar->setChanged();
    }
    Bool_t kParameterMerged = kTRUE;
    FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
    parOut->open(parFile.Data());
    rtdb->setOutput(parOut);
    rtdb->saveOutput();
    rtdb->print();


    // -----   Start run   ----------------------------------------------------
    if (nEvents > 0) run->Run(nEvents);

    // -----   Finish   -------------------------------------------------------
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    cout << endl << endl;
    cout << "Macro finished succesfully." << endl;
    cout << "Output file is " << outFile << endl;
    cout << "Parameter file is " << parFile << endl;
    cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << endl << endl;

    cout << " Test passed" << endl;
    cout << " All ok " << endl;
}
