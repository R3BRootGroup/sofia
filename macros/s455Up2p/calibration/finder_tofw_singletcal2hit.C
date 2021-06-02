/*
 *  Macro to calibrate the 28 scintillators of the ToF-Wall
 *
 *  One needs to set up the 2021 experiment: s455, the unpacker is:
 *
 *  at $UCESB_DIR/../upexps/202103_s455
 *
 *
 *  Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
 *  @since June 2nd, 2021
 *
 */

typedef struct EXT_STR_h101_t
{
    EXT_STR_h101_unpack_t unpack;
    EXT_STR_h101_TPAT_t unpacktpat;

    EXT_STR_h101_SOFMWPC_onion_t mwpc;
    EXT_STR_h101_SOFTRIM_onion_t trim;
    EXT_STR_h101_SOFSCI_onion_t sci;
    EXT_STR_h101_SOFTWIM_onion_t twim;
    EXT_STR_h101_SOFTOFW_onion_t tofw;

    EXT_STR_h101_WRMASTER_t wrmaster;
    EXT_STR_h101_WRSOFIA_t wrsofia;
    EXT_STR_h101_WRS2_t wrs2;
    EXT_STR_h101_WRS8_t wrs8;
} EXT_STR_h101;

// const Int_t nev = -1; // number of events to read, -1 - until CTRL+C

void finder_tofw_singletcal2hit(const Int_t nev = -1)
{
    TStopwatch timer;

    const Int_t fRunId = 1;

    // *********************************** //
    // PLEASE CHANGE THE EXPERIMENT NUMBER //
    // *********************************** //
    const Int_t expId = 455; // select experiment: 444, 455 or 467
    // *********************************** //

    // NumSofSci, file names and paths -----------------------------
    Int_t sofiaWR_SE, sofiaWR_ME, NumSofSci, IdS2, IdS8;
    TString dir = gSystem->Getenv("VMCWORKDIR");
    TString ntuple_options = "RAW";
    TString ucesb_dir = getenv("UCESB_DIR");
    TString filename, outputFilename, upexps_dir, ucesb_path, sofiacaldir;

    if (expId == 455)
    {
        NumSofSci = 1;
        IdS2 = 0;
        IdS8 = 0;
        sofiaWR_SE = 0xe00;
        sofiaWR_ME = 0xf00;

        filename = "~/lmd/s455/main0273_0010*.lmd";
        outputFilename = "data_s455_singletcal2hit.root";

        upexps_dir = ucesb_dir + "/../upexps/"; // for local computers
        // upexps_dir = "/u/land/fake_cvmfs/9.13/upexps";                  // for lxlandana computers
        // upexps_dir = "/u/land/lynx.landexp/202002_s467/upexps/";     // for lxg computers
        ucesb_path = upexps_dir + "/202103_s455/202103_s455 --allow-errors --input-buffer=70Mi";
        ucesb_path.ReplaceAll("//", "/");
        sofiacaldir = dir + "/sofia/macros/s455Up2p/parameters/";
    }
    else
    {
        std::cout << "Experiment was not selected" << std::endl;
        gApplication->Terminate();
    }

    // Calibration files ------------------------------------
    TString sofiacalfilename = sofiacaldir + "CalibParam.par";
    sofiacalfilename.ReplaceAll("//", "/");

    // store data or not ------------------------------------
    Bool_t fCal_level_califa = false; // set true if there exists a file with the calibration parameters
    Bool_t NOTstoremappeddata = true; // if true, don't store mapped data in the root file
    Bool_t NOTstorecaldata = true;    // if true, don't store cal data in the root file
    Bool_t NOTstorehitdata = true;    // if true, don't store hit data in the root file

    // Setup: Selection of detectors ------------------------
    // --- FRS --------------------------------------------------------------------------
    Bool_t fFrsSci = false; // Start: Plastic scintillators at FRS

    // --- Sofia ------------------------------------------------------------------------
    Bool_t fMwpc0 = false; // MWPC0 for tracking at entrance of Cave-C
    Bool_t fSci = true;    // Start: Plastic scintillator for ToF
    Bool_t fMwpc1 = false; // MWPC1 for tracking of fragments in front of target
    Bool_t fMwpc2 = false; // MWPC2 for tracking of fragments before GLAD
    Bool_t fTwim = false;  // Twim: Ionization chamber for charge-Z of fragments
    Bool_t fMwpc3 = false; // MWPC3 for tracking of fragments behind GLAD
    Bool_t fTofW = true;   // ToF-Wall for time-of-flight of fragments behind GLAD

    // Source fpr ucesb  ------------------------------------
    EXT_STR_h101 ucesb_struct;

    // Create online run ------------------------------------
    FairRunOnline* run = new FairRunOnline();
    R3BEventHeader* EvntHeader = new R3BEventHeader();
    run->SetEventHeader(EvntHeader);
    run->SetRunId(fRunId);
    run->SetSink(new FairRootFileSink(outputFilename));

    // Create source using ucesb for input ------------------
    R3BUcesbSource* source =
        new R3BUcesbSource(filename, ntuple_options, ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
    source->SetMaxEvents(nev);

    // Definition of reader ---------------------------------
    R3BSofSciReader* unpacksci;
    R3BWhiterabbitS2Reader* unpackWRS2;
    R3BWhiterabbitS8Reader* unpackWRS8;
    R3BWhiterabbitMasterReader* unpackWRMaster;
    R3BSofWhiterabbitReader* unpackWRSofia;
    R3BSofMwpcReader* unpackmwpc;
    R3BSofTwimReader* unpacktwim;
    R3BSofTofWReader* unpacktofw;

    // Add readers ------------------------------------------
    source->AddReader(new R3BUnpackReader(&ucesb_struct.unpack, offsetof(EXT_STR_h101, unpack)));
    source->AddReader(new R3BTrloiiTpatReader(&ucesb_struct.unpacktpat, offsetof(EXT_STR_h101, unpacktpat)));

    if (fFrsSci)
    {
        unpackWRS2 =
            new R3BWhiterabbitS2Reader((EXT_STR_h101_WRS2*)&ucesb_struct.wrs2, offsetof(EXT_STR_h101, wrs2), 0x200);
        unpackWRS8 =
            new R3BWhiterabbitS8Reader((EXT_STR_h101_WRS8*)&ucesb_struct.wrs8, offsetof(EXT_STR_h101, wrs8), 0x800);
    }

    if (fSci)
    {
        unpacksci =
            new R3BSofSciReader((EXT_STR_h101_SOFSCI_t*)&ucesb_struct.sci, offsetof(EXT_STR_h101, sci), NumSofSci);
        unpackWRMaster = new R3BWhiterabbitMasterReader(
            (EXT_STR_h101_WRMASTER*)&ucesb_struct.wrmaster, offsetof(EXT_STR_h101, wrmaster), 0x1000);
        unpackWRSofia = new R3BSofWhiterabbitReader(
            (EXT_STR_h101_WRSOFIA*)&ucesb_struct.wrsofia, offsetof(EXT_STR_h101, wrsofia), sofiaWR_SE, sofiaWR_ME);
    }

    if (fMwpc0 || fMwpc1 || fMwpc2 || fMwpc3)
        unpackmwpc = new R3BSofMwpcReader((EXT_STR_h101_SOFMWPC_t*)&ucesb_struct.mwpc, offsetof(EXT_STR_h101, mwpc));

    if (fTwim)
        unpacktwim = new R3BSofTwimReader((EXT_STR_h101_SOFTWIM_t*)&ucesb_struct.twim, offsetof(EXT_STR_h101, twim));

    if (fTofW)
        unpacktofw = new R3BSofTofWReader((EXT_STR_h101_SOFTOFW_t*)&ucesb_struct.tofw, offsetof(EXT_STR_h101, tofw));

    if (fSci)
    {
        unpacksci->SetOnline(NOTstoremappeddata);
        source->AddReader(unpacksci);
        unpackWRMaster->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackWRMaster);
        unpackWRSofia->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackWRSofia);
    }

    if (fFrsSci)
    {
        unpackWRS2->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackWRS2);
        unpackWRS8->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackWRS8);
    }

    if (fMwpc0 || fMwpc1 || fMwpc2 || fMwpc3)
    {
        unpackmwpc->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackmwpc);
    }

    if (fTwim)
    {
        unpacktwim->SetOnline(NOTstoremappeddata);
        source->AddReader(unpacktwim);
    }
    if (fTofW)
    {
        unpacktofw->SetOnline(NOTstoremappeddata);
        source->AddReader(unpacktofw);
    }
    run->SetSource(source);

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

    // SCI
    if (fSci)
    {
        // --- Mapped 2 Tcal for SofSci
        R3BSofSciMapped2Tcal* SofSciMap2Tcal = new R3BSofSciMapped2Tcal();
        SofSciMap2Tcal->SetOnline(NOTstorecaldata);
        run->AddTask(SofSciMap2Tcal);

        // --- Tcal 2 SingleTcal for SofSci
        R3BSofSciTcal2SingleTcal* SofSciTcal2STcal = new R3BSofSciTcal2SingleTcal();
        SofSciTcal2STcal->SetOnline(NOTstorecaldata);
        run->AddTask(SofSciTcal2STcal);
        // --- SingleTcal 2 Hit for SofSci
        R3BSofSciSingleTcal2Hit* SofSciSTcal2Hit = new R3BSofSciSingleTcal2Hit();
        SofSciSTcal2Hit->SetOnline(NOTstorehitdata);
        SofSciSTcal2Hit->SetCalParams(675., -1922.); // ToF calibration at Cave-C
        run->AddTask(SofSciSTcal2Hit);
    }

    // FRS
    if (fMwpc0 && fSci)
    {
        R3BSofFrsAnalysis* FrsAna = new R3BSofFrsAnalysis();
        FrsAna->SetOnline(NOTstorehitdata);
        run->AddTask(FrsAna);
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
        TwimMap2Cal->SetOnline(NOTstorecaldata);
        run->AddTask(TwimMap2Cal);

        R3BSofTwimCal2Hit* TwimCal2Hit = new R3BSofTwimCal2Hit();
        TwimCal2Hit->SetOnline(NOTstorehitdata);
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

    // MWPC3
    if (fMwpc3)
    {
        R3BSofMwpc3Mapped2Cal* MW3Map2Cal = new R3BSofMwpc3Mapped2Cal();
        MW3Map2Cal->SetOnline(NOTstorecaldata);
        run->AddTask(MW3Map2Cal);

        R3BSofMwpc3Cal2Hit* MW3Cal2Hit = new R3BSofMwpc3Cal2Hit();
        MW3Cal2Hit->SetOnline(NOTstorehitdata);
        run->AddTask(MW3Cal2Hit);
    }

    // ToF-Wall
    if (fTofW)
    {
        // --- Mapped 2 Tcal for SofTofW
        R3BSofTofWMapped2Tcal* SofTofWMap2Tcal = new R3BSofTofWMapped2Tcal();
        SofTofWMap2Tcal->SetOnline(NOTstorecaldata);
        run->AddTask(SofTofWMap2Tcal);

        // --- Tcal 2 SingleTcal for SofTofW
        R3BSofTofWTcal2SingleTcal* SofTofWTcal2STcal = new R3BSofTofWTcal2SingleTcal();
        SofTofWTcal2STcal->SetOnline(NOTstorecaldata);
        run->AddTask(SofTofWTcal2STcal);

        // --- SingleTcal 2 Hit for SofTofW
        R3BSofTofWSingleTCal2HitPar* SofTofWSingleTcal2Hit = new R3BSofTofWSingleTCal2HitPar();
        SofTofWSingleTcal2Hit->SetFitLimitsTof(-140, -60);
        run->AddTask(SofTofWSingleTcal2Hit);
    }

    // Initialize -------------------------------------------
    timer.Start();
    run->Init();
    FairLogger::GetLogger()->SetLogScreenLevel("INFO");

    FairParAsciiFileIo* parOut = new FairParAsciiFileIo();
    TString outputFileNamePar = "singletcal2hit_softofw.par";
    parOut->open(outputFileNamePar, "out");
    rtdb->setOutput(parOut);
    rtdb->print();

    // Run --------------------------------------------------
    run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);
    rtdb->saveOutput();

    // Finish -----------------------------------------------
    timer.Stop();
    Double_t rtime = timer.RealTime() / 60.;
    Double_t ctime = timer.CpuTime() / 60.;
    std::cout << std::endl << std::endl;
    std::cout << "Macro finished succesfully." << std::endl;
    std::cout << "Output file is " << outputFilename << std::endl;
    std::cout << "Real time " << rtime << " min, CPU time " << ctime << " min" << std::endl << std::endl;
    gApplication->Terminate();
}
