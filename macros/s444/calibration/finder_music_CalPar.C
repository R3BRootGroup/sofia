/*
 *  Macro to run the music calibration
 *
 *  One needs to set up the 2020 experiments: s444 or s467, the unpackers are:
 *
 *  at $UCESB_DIR/../upexps/202002_s444 and $UCESB_DIR/../upexps/202002_s467
 *
 *
 *  Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
 *  @since Jan 28th, 2020
 *
 */

typedef struct EXT_STR_h101_t
{
    EXT_STR_h101_unpack_t unpack;
    EXT_STR_h101_SOFMWPC_onion_t mwpc;
    EXT_STR_h101_MUSIC_onion_t music;
    EXT_STR_h101_SOFSCI_onion_t sci;
    EXT_STR_h101_SOFTWIM_onion_t twim;
} EXT_STR_h101;

void finder_music_CalPar()
{
    TStopwatch timer;
    timer.Start();

    // const Int_t nev = -1; // number of events to read, -1 - until CTRL+C
    const Int_t nev = -1; // Only nev events to read
    const Int_t fRunId = 1;

    const Int_t expId = 444; // select experiment: 444 or 467

    // Create input -----------------------------------------
    // TString filename = "--stream=lxir123:7803";
    //TString filename = "~/lmd/sofia2019/main0079_0001.lmd";
    TString filename = "~/lmd/sofia2020/main0014_0001.lmd";

    // Output file ------------------------------------------
    TString outputFileName = "data_s444_musicCal.root";
    Bool_t NOTstoremappeddata = false; // if true, don't store mapped data in the root file
    Bool_t NOTstorecaldata = false;    // if true, don't store cal data in the root file
    Bool_t NOTstorehitdata = false;    // if true, don't store hit data in the root file

    // UCESB configuration ----------------------------------
    TString ntuple_options = "RAW";
    TString ucesb_dir = getenv("UCESB_DIR");
    TString upexps_dir = ucesb_dir + "/../upexps/";
    //TString upexps_dir = "/u/land/fake_cvmfs/upexps";
    TString ucesb_path;
    if (expId == 444)
    {
         ucesb_path = upexps_dir + "/202002_s444/202002_s444 --allow-errors --input-buffer=100Mi";
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
    // Distances with respect to MWPC0 in mm
    Bool_t fMwpc0 = true;        // MWPC0 for tracking at entrance of Cave-C
    Float_t PosZ_MWPC0 = 0.0;    // mm
    Bool_t fMusic = true;        // R3B-Music: Ionization chamber for charge-Z
    Float_t PosZ_Music = 325.0;  // mm
    Bool_t fSci = false;         // Start: Plastic scintillator for ToF
    Bool_t fMwpc1 = true;        // MWPC1 for tracking of fragments in front of target
    Float_t PosZ_MWPC1 = 2835.0; // mm
    Bool_t fTwim = false;        // Twim: Ionization chamber for charge-Z of fragments
    Bool_t fMwpc2 = false;       // MWPC2 for tracking of fragments before GLAD
    Float_t PosZ_MWPC2 = 3493.0; // mm

    // Calibration files ------------------------------------
    TString dir = gSystem->Getenv("VMCWORKDIR");
    // Parameters for SOFIA detectors
    TString sofiacaldir = dir + "/sofia/macros/s444/parameters/";
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

    R3BMusicReader* unpackmusic;
    R3BSofSciReader* unpacksci;
    R3BSofMwpcReader* unpackmwpc;
    R3BSofTwimReader* unpacktwim;

    if (fMusic)
        unpackmusic = new R3BMusicReader((EXT_STR_h101_MUSIC_t*)&ucesb_struct.music, offsetof(EXT_STR_h101, music));

    if (fSci)
        unpacksci = new R3BSofSciReader((EXT_STR_h101_SOFSCI_t*)&ucesb_struct.sci, offsetof(EXT_STR_h101, sci));

    if (fMwpc0 || fMwpc1 || fMwpc2)
        unpackmwpc = new R3BSofMwpcReader((EXT_STR_h101_SOFMWPC_t*)&ucesb_struct.mwpc, offsetof(EXT_STR_h101, mwpc));

    if (fTwim)
        unpacktwim = new R3BSofTwimReader((EXT_STR_h101_SOFTWIM_t*)&ucesb_struct.twim, offsetof(EXT_STR_h101, twim));

    // Add readers ------------------------------------------
    source->AddReader(unpackreader);
    if (fMusic)
    {
        unpackmusic->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackmusic);
    }
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
    // TWIM
    if (fTwim)
    {
        R3BSofTwimMapped2Cal* TwimMap2Cal = new R3BSofTwimMapped2Cal();
        run->AddTask(TwimMap2Cal);

        R3BSofTwimCal2Hit* TwimCal2Hit = new R3BSofTwimCal2Hit();
        run->AddTask(TwimCal2Hit);
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
    if (fMwpc2 && fMwpc0)
    {
        R3BMusicMapped2CalPar* MusMap2CalPar = new R3BMusicMapped2CalPar("Angle-XZ calibrator", 1, "Mwpc0", "Mwpc2");
        MusMap2CalPar->SetPosMwpcA(PosZ_MWPC0);
        MusMap2CalPar->SetPosMwpcB(PosZ_MWPC2);
        MusMap2CalPar->SetPosMusic(PosZ_Music);
        run->AddTask(MusMap2CalPar);
    }
    else if (fMwpc1 && fMwpc0)
    {
        R3BMusicMapped2CalPar* MusMap2CalPar = new R3BMusicMapped2CalPar("Angle-XZ calibrator", 1, "Mwpc0", "Mwpc1");
        MusMap2CalPar->SetPosMwpcA(PosZ_MWPC0);
        MusMap2CalPar->SetPosMwpcB(PosZ_MWPC1);
        MusMap2CalPar->SetPosMusic(PosZ_Music);
        run->AddTask(MusMap2CalPar);
    }
    else
        std::cout << "Calibration is not possible since second mwpc was not defined" << std::endl;

    // Initialize -------------------------------------------
    run->Init();
    FairLogger::GetLogger()->SetLogScreenLevel("INFO");

    // Ascii file with the Calibartion Parameters
    FairParAsciiFileIo* parIo = new FairParAsciiFileIo();
    parIo->open("Music_CalPar_Feb2020.par", "out");
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
