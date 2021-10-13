/*
 *  Macro to run the twim calibration
 *
 *  One needs to set up the 2021 experiment: s455, the unpacker is:
 *
 *  at $UCESB_DIR/../upexps/202104_s455 and $UCESB_DIR/../upexps/202104_s455
 *
 *
 *  Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
 *  @since March 6th, 2021
 *
 */

typedef struct EXT_STR_h101_t
{
    EXT_STR_h101_unpack_t unpack;
    EXT_STR_h101_SOFMWPC_onion_t mwpc;
    EXT_STR_h101_SOFSCI_onion_t sci;
    EXT_STR_h101_SOFTWIM_onion_t twim;
} EXT_STR_h101;

void finder_twim_CalPar()
{
    TStopwatch timer;
    timer.Start();

    // const Int_t nev = -1; // number of events to read, -1 - until CTRL+C
    const Int_t nev = 1000000; // Only nev events to read
    const Int_t fRunId = 1;

    const Int_t expId = 455; // select experiment: 444, 455 or 467

    // Create input -----------------------------------------
    TString filename = "--stream=lxlanddaq01:9000";
    // TString filename = "~/lmd/sofia2020/main0075_0001.lmd";

    // Output file ------------------------------------------
    TString outputFileName = "data_s455_twimCal.root";
    Bool_t NOTstoremappeddata = false; // if true, don't store mapped data in the root file
    Bool_t NOTstorecaldata = false;    // if true, don't store cal data in the root file
    Bool_t NOTstorehitdata = false;    // if true, don't store hit data in the root file

    // UCESB configuration ----------------------------------
    TString ntuple_options = "RAW";
    TString ucesb_dir = getenv("UCESB_DIR");
    //TString upexps_dir = ucesb_dir + "/../upexps/";
    //TString upexps_dir = "/u/land/lynx.landexp/202002_s444/upexps/"; //for lxg computers
    TString upexps_dir = "/u/land/fake_cvmfs/9.13/upexps";           //for lxlandana computers
    TString ucesb_path;
    if (expId == 444)
    {
         ucesb_path = upexps_dir + "/202002_s444/202002_s444 --allow-errors --input-buffer=100Mi";
    }
    else if (expId == 455)
    {
         ucesb_path = upexps_dir + "/202104_s455/202104_s455 --allow-errors --input-buffer=100Mi";
    }
    else if (expId == 467)
    {
         ucesb_path = upexps_dir + "/202002_s467/202002_s467 --allow-errors --input-buffer=100Mi";
    }
    else
    {
        std::cout << "Experiment was not selected!" << std::endl;
        gApplication->Terminate();
    }
    ucesb_path.ReplaceAll("//", "/");

    // Setup: Selection of detectors ------------------------
    Bool_t fMwpc0 = false;       // MWPC0 for tracking at entrance of Cave-C
    Float_t PosZ_MWPC0 = 0.0;    // mm
    Bool_t fSci = false;         // Start: Plastic scintillator for ToF
    Bool_t fMwpc1 = true;        // MWPC1 for tracking of fragments in front of target
    Float_t PosZ_MWPC1 = 930.0;  // mm
    Bool_t fTwim = true;         // Twim: Ionization chamber for charge-Z of fragments
    Float_t PosZ_Twim = 1274.0;  // mm
    Bool_t fMwpc2 = true;        // MWPC2 for tracking of fragments before GLAD
    Float_t PosZ_MWPC2 = 1601.0; // mm

    // Calibration files ------------------------------------
    TString dir = gSystem->Getenv("VMCWORKDIR");
    // Parameters for SOFIA detectors
    TString sofiacaldir = dir + "/sofia/macros/s455/parameters/";
    TString sofiacalfilename = sofiacaldir + "CalibParam.par";
    sofiacalfilename.ReplaceAll("//", "/");

    // Create source using ucesb for input ------------------
    EXT_STR_h101 ucesb_struct;

    R3BUcesbSource* source =
        new R3BUcesbSource(filename, ntuple_options, ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
    source->SetMaxEvents(nev);

    // Definition of reader ---------------------------------
    R3BUnpackReader* unpackreader =
        new R3BUnpackReader((EXT_STR_h101_unpack*)&ucesb_struct, offsetof(EXT_STR_h101, unpack));
    R3BSofSciReader* unpacksci;
    R3BSofMwpcReader* unpackmwpc;
    R3BSofTwimReader* unpacktwim;

    if (fSci)
        unpacksci = new R3BSofSciReader((EXT_STR_h101_SOFSCI_t*)&ucesb_struct.sci, offsetof(EXT_STR_h101, sci));

    if (fMwpc0 || fMwpc1 || fMwpc2)
        unpackmwpc = new R3BSofMwpcReader((EXT_STR_h101_SOFMWPC_t*)&ucesb_struct.mwpc, offsetof(EXT_STR_h101, mwpc));

    if (fTwim)
        unpacktwim = new R3BSofTwimReader((EXT_STR_h101_SOFTWIM_t*)&ucesb_struct.twim, offsetof(EXT_STR_h101, twim));

    // Add readers ------------------------------------------
    source->AddReader(unpackreader);
    if (fSci)
    {
        unpacksci->SetOnline(NOTstoremappeddata);
        source->AddReader(unpacksci);
    }
    if (fMwpc0 || fMwpc1 || fMwpc2)
    {
        unpackmwpc->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackmwpc);
    }
    if (fTwim)
    {
        unpacktwim->SetOnline(NOTstoremappeddata);
        source->AddReader(unpacktwim);
    }

    // Create online run ------------------------------------
    FairRunOnline* run = new FairRunOnline(source);
    run->SetRunId(fRunId);
    run->SetSink(new FairRootFileSink(outputFileName));

    // Runtime data base ------------------------------------
    FairRuntimeDb* rtdb = run->GetRuntimeDb();

    FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo(); // Ascii
    parIo1->open(sofiacalfilename, "in");
    rtdb->setFirstInput(parIo1);
    rtdb->print();

    // Add analysis task ------------------------------------
    // MWPC0
    if (fMwpc0)
    {
        R3BSofMwpc0Mapped2Cal* MW0Map2Cal = new R3BSofMwpc0Mapped2Cal();
        MW0Map2Cal->SetOnline(NOTstorecaldata);
        run->AddTask(MW0Map2Cal);

        R3BSofMwpc0Cal2Hit* MW0Cal2Hit = new R3BSofMwpc0Cal2Hit();
        MW0Cal2Hit->SetOnline(NOTstorehitdata);
        run->AddTask(MW0Cal2Hit);
    }
    // MWPC1
    if (fMwpc1)
    {
        R3BSofMwpc1Mapped2Cal* MW1Map2Cal = new R3BSofMwpc1Mapped2Cal();
        MW1Map2Cal->SetOnline(NOTstorecaldata);
        run->AddTask(MW1Map2Cal);

        R3BSofMwpc1Cal2Hit* MW1Cal2Hit = new R3BSofMwpc1Cal2Hit();
        MW1Cal2Hit->SetOnline(NOTstorehitdata);
        run->AddTask(MW1Cal2Hit);
    }
    // MWPC2
    if (fMwpc2)
    {
        R3BSofMwpc2Mapped2Cal* MW2Map2Cal = new R3BSofMwpc2Mapped2Cal();
        MW2Map2Cal->SetOnline(NOTstorecaldata);
        run->AddTask(MW2Map2Cal);

        R3BSofMwpc2Cal2Hit* MW2Cal2Hit = new R3BSofMwpc2Cal2Hit();
        MW2Cal2Hit->SetOnline(NOTstorehitdata);
        run->AddTask(MW2Cal2Hit);
    }

    // Add calibration task ------------------------------------
    if (fMwpc0 && fMwpc2)
    {
        R3BSofTwimMapped2CalPar* TwimMap2CalPar =
            new R3BSofTwimMapped2CalPar("Angle-XZ Twim calibrator", 1, "Mwpc0", "Mwpc2");
        TwimMap2CalPar->SetPosMwpcA(PosZ_MWPC0);
        TwimMap2CalPar->SetPosMwpcB(PosZ_MWPC2);
        TwimMap2CalPar->SetPosTwim(PosZ_Twim);
        TwimMap2CalPar->SetFitLimits(2000., 22000.);
        run->AddTask(TwimMap2CalPar);
    }
    else if (fMwpc1 && fMwpc2)
    {
        R3BSofTwimMapped2CalPar* TwimMap2CalPar =
            new R3BSofTwimMapped2CalPar("Angle-XZ Twim calibrator", 1, "Mwpc1", "Mwpc2");
        TwimMap2CalPar->SetPosMwpcA(PosZ_MWPC1);
        TwimMap2CalPar->SetPosMwpcB(PosZ_MWPC2);
        TwimMap2CalPar->SetPosTwim(PosZ_Twim);
        TwimMap2CalPar->SetFitLimits(2000., 22000.);
        run->AddTask(TwimMap2CalPar);
    }

    // Initialize -------------------------------------------
    run->Init();
    FairLogger::GetLogger()->SetLogScreenLevel("INFO");

    // Ascii file with the Calibartion Parameters
    FairParAsciiFileIo* parIo = new FairParAsciiFileIo();
    parIo->open("Twim_CalPar_10March2021.par", "out");
    rtdb->setOutput(parIo);

    // Run --------------------------------------------------
    run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);

    // Save parameters (if needed) --------------------------
    rtdb->saveOutput();

    // Finish -----------------------------------------------
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    std::cout << std::endl << std::endl;
    std::cout << "Macro finished succesfully." << std::endl;
    std::cout << "Output file is " << outputFileName << std::endl;
    std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl << std::endl;
}
