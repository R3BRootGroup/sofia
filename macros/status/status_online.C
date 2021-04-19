/*
 *  Macro to run the online for all the detectors simultaneously
 *
 *  One needs to set up the Phase0 experiments: s444, s467, or s455
 *
 *  at $UCESB_DIR/../upexps/yyyymm_s4xx
 *
 *
 *  Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
 *  @since March 15th, 2021
 *
 */

typedef struct EXT_STR_h101_t
{
    EXT_STR_h101_unpack_t unpack;
    EXT_STR_h101_TPAT_t unpacktpat;

    EXT_STR_h101_MUSIC_onion_t music;
    EXT_STR_h101_AMS_onion_t ams;
    EXT_STR_h101_CALIFA_t califa;
    EXT_STR_h101_raw_nnp_tamex_t raw_nnp;

    EXT_STR_h101_SOFMWPC_onion_t mwpc;
    EXT_STR_h101_SOFTRIM_onion_t trim;
    EXT_STR_h101_SOFAT_onion_t at;
    EXT_STR_h101_SOFSCI_onion_t sci;
    EXT_STR_h101_SOFTWIM_onion_t twim;
    EXT_STR_h101_SOFTOFW_onion_t tofw;
    EXT_STR_h101_SOFSCALERS_onion_t scalers;

    EXT_STR_h101_FRS_t frs;

    EXT_STR_h101_WRMASTER_t wrmaster;
    EXT_STR_h101_WRCALIFA_t wrcalifa;
    EXT_STR_h101_WRNEULAND_t wrneuland;
    EXT_STR_h101_WRSOFIA_t wrsofia;
    EXT_STR_h101_WRS2_t wrs2;
    EXT_STR_h101_WRS8_t wrs8;
    EXT_STR_h101_WRAMS_t wrams;
} EXT_STR_h101;

void status_online()
{
    TStopwatch timer;
    timer.Start();

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d_%H%M%S");

    // const Int_t nev = -1; // number of events to read, -1 - until CTRL+C
    const Int_t nev = -1; // Only nev events to read
    const Int_t fRunId = 1;

    // *********************************** //
    // PLEASE CHANGE THE EXPERIMENT NUMBER //
    // *********************************** //
    const Int_t expId = 455; // select experiment: 444, 467 or 455
    // *********************************** //

    // NumSofSci, file names and paths -----------------------------
    Int_t sofiaWR_SE, sofiaWR_ME, NumSofSci, IdS2, IdS8;
    TString dir = gSystem->Getenv("VMCWORKDIR");
    TString ntuple_options = "RAW";
    TString ucesb_dir = getenv("UCESB_DIR");
    TString filename, outputFilename, upexps_dir, ucesb_path, sofiacaldir;

    if (expId == 444)
    {
        NumSofSci = 1; // s444: PRIMARY BEAM EXP, 1 SofSci at CAVE C ONLY
        IdS2 = 0;
        IdS8 = 0;
        sofiaWR_SE = 0x500;

        // filename = "--stream=lxir123:7803";
        filename = "/lustre/land/202002_s444/stitched/main0040_0001.lmd";
        outputFilename = "data_s444_online.root";

        upexps_dir = ucesb_dir + "/../upexps/"; // for local computers
        // upexps_dir = "/u/land/fake_cvmfs/upexps";                 // for lxlandana computers
        // upexps_dir = "/u/land/lynx.landexp/202002_s444/upexps/";  // for lxg computers
        ucesb_path = upexps_dir + "/202002_s444/202002_s444 --allow-errors --input-buffer=100Mi";

        sofiacaldir = dir + "/sofia/macros/s444/parameters/";
    }
    else if (expId == 467)
    {
        NumSofSci = 4; // s467: SECONDARY BEAM EXP, 2 at S2, 1 at S8, 1 at CAVE C
        IdS2 = 2;
        IdS8 = 3;
        sofiaWR_SE = 0xe00;

        filename = "--stream=lxir123:7803";
        // filename = "~/lmd/s467/main0*.lmd";
        outputFilename = "data_s467_online.root";

        upexps_dir = ucesb_dir + "/../upexps/"; // for local computers
        // upexps_dir = "/u/land/fake_cvmfs/upexps";                 // for lxlandana computers
        // upexps_dir = "/u/land/lynx.landexp/202002_s467/upexps/";  // for lxg computers
        ucesb_path = upexps_dir + "/202002_s467/202002_s467 --allow-errors --input-buffer=100Mi";

        sofiacaldir = dir + "/sofia/macros/s467/parameters/";
    }
    else if (expId == 455)
    {
        NumSofSci = 1;
        IdS2 = 0;
        IdS8 = 0;
        sofiaWR_SE = 0xe00;
        sofiaWR_ME = 0xf00;

        filename = "--stream=lxir133:9001";
        // filename = "~/lmd/s455/main0079_0001.lmd";

        TString outputpath = "/d/land4/202103_s455/rootfiles/sofia/";
        outputFilename = outputpath + "s455_data_status_online_" + oss.str() + ".root";
        // outputFilename = "s455_data_status_online_" + oss.str() + ".root";

        // upexps_dir = ucesb_dir + "/../upexps/"; // for local computers
        upexps_dir = "/u/land/fake_cvmfs/9.13/upexps";                 // for lxlandana computers
        // upexps_dir = "/u/land/lynx.landexp/202002_s467/upexps/";  // for lxg computers
        ucesb_path = upexps_dir + "/202103_s455/202103_s455 --allow-errors --input-buffer=100Mi";

        sofiacaldir = dir + "/sofia/macros/s455/parameters/";
    }
    else
    {
        std::cout << "Experiment was not selected" << std::endl;
        gApplication->Terminate();
    }

    // store data or not ------------------------------------
    Bool_t fCal_level_califa = false; // set true if there exists a file with the calibration parameters
    Bool_t NOTstoremappeddata = true; // if true, don't store mapped data in the root file
    Bool_t NOTstorecaldata = true;    // if true, don't store cal data in the root file
    Bool_t NOTstorehitdata = true;    // if true, don't store hit data in the root file

    // Online server configuration --------------------------
    Int_t refresh = 1; // Refresh rate for online histograms
    Int_t port = 8889; // Port number for the online visualization, example lxgXXXX:8889

    // Setup: Selection of detectors ------------------------
    // --- FRS --------------------------------------------------------------------------
    Bool_t fFrs = false;     // FRS for production of exotic beams (just scintillators)
    Bool_t fFrsTpcs = false; // Tpcs at FRS (S2) for scintillator calibration in position
    Bool_t fFrsMws = false;  // MWs at FRS (S8) for beam position
    Bool_t fFrsSci = false;  // Start: Plastic scintillators at FRS
    // --- R3B standard -----------------------------------------------------------------
    Bool_t fNeuland = true; // NeuLAND for neutrons behind GLAD
    Bool_t fAms = true;     // AMS tracking detectors
    Bool_t fCalifa = true;  // Califa calorimeter
    Bool_t fMusic = false;  // R3B-Music: Ionization chamber for charge-Z
    // --- Sofia ------------------------------------------------------------------------
    Bool_t fMwpc0 = true;    // MWPC0 for tracking at entrance of Cave-C
    Bool_t fTrim = true;     // Triple-MUSIC for the HI beam charge-Z id, with charge-q states
    Bool_t fAt = false;      // Active Targer for Coulomb-induced fission
    Bool_t fSci = true;      // Start: Plastic scintillator for ToF
    Bool_t fMwpc1 = true;    // MWPC1 for tracking of fragments in front of target
    Bool_t fMwpc2 = true;    // MWPC2 for tracking of fragments before GLAD
    Bool_t fTwim = true;     // Twim: Ionization chamber for charge-Z of fragments
    Bool_t fMwpc3 = true;    // MWPC3 for tracking of fragments behind GLAD
    Bool_t fTofW = true;     // ToF-Wall for time-of-flight of fragments behind GLAD
    Bool_t fScalers = false; // SIS3820 scalers at Cave C
    // --- Traking ----------------------------------------------------------------------
    Bool_t fTracking = false; // Tracking of fragments inside GLAD and before GLAD

    // Create source using ucesb for input ------------------
    EXT_STR_h101 ucesb_struct;

    R3BUcesbSource* source =
        new R3BUcesbSource(filename, ntuple_options, ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
    source->SetMaxEvents(nev);

    // Definition of reader ---------------------------------
    R3BFrsReaderNov19* unpackfrs;

    R3BMusicReader* unpackmusic;
    R3BAmsReader* unpackams;
    R3BCalifaFebexReader* unpackcalifa;
    R3BNeulandTamexReader* unpackneuland;

    R3BSofTrimReader* unpacktrim;
    R3BSofSciReader* unpacksci;
    R3BSofAtReader* unpackat;
    R3BSofMwpcReader* unpackmwpc;
    R3BSofTwimReader* unpacktwim;
    R3BSofTofWReader* unpacktofw;
    R3BSofScalersReader* unpackscalers;

    R3BWhiterabbitS2Reader* unpackWRS2;
    R3BWhiterabbitS8Reader* unpackWRS8;
    R3BWhiterabbitMasterReader* unpackWRMaster;
    R3BWhiterabbitCalifaReader* unpackWRCalifa;
    R3BSofWhiterabbitReader* unpackWRSofia;
    R3BWhiterabbitNeulandReader* unpackWRNeuland;
    R3BWhiterabbitAmsReader* unpackWRAms;

    if (fFrsTpcs)
        unpackfrs = new R3BFrsReaderNov19((EXT_STR_h101_FRS*)&ucesb_struct.frs, offsetof(EXT_STR_h101, frs));

    if (fMusic)
        unpackmusic = new R3BMusicReader((EXT_STR_h101_MUSIC_t*)&ucesb_struct.music, offsetof(EXT_STR_h101, music));

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

    if (fAms)
    {
        unpackams = new R3BAmsReader((EXT_STR_h101_AMS_onion*)&ucesb_struct.ams, offsetof(EXT_STR_h101, ams));
        unpackWRAms =
            new R3BWhiterabbitAmsReader((EXT_STR_h101_WRAMS*)&ucesb_struct.wrams, offsetof(EXT_STR_h101, wrams), 0x600);
    }

    if (fCalifa)
    {
        unpackcalifa =
            new R3BCalifaFebexReader((EXT_STR_h101_CALIFA*)&ucesb_struct.califa, offsetof(EXT_STR_h101, califa));
        unpackWRCalifa = new R3BWhiterabbitCalifaReader(
            (EXT_STR_h101_WRCALIFA*)&ucesb_struct.wrcalifa, offsetof(EXT_STR_h101, wrcalifa), 0xa00, 0xb00);
    }
    if (fMwpc0 || fMwpc1 || fMwpc2 || fMwpc3)
        unpackmwpc = new R3BSofMwpcReader((EXT_STR_h101_SOFMWPC_t*)&ucesb_struct.mwpc, offsetof(EXT_STR_h101, mwpc));

    if (fTrim)
        unpacktrim = new R3BSofTrimReader((EXT_STR_h101_SOFTRIM_t*)&ucesb_struct.trim, offsetof(EXT_STR_h101, trim));

    if (fAt)
        unpackat = new R3BSofAtReader((EXT_STR_h101_SOFAT_t*)&ucesb_struct.at, offsetof(EXT_STR_h101, at));

    if (fTwim)
        unpacktwim = new R3BSofTwimReader((EXT_STR_h101_SOFTWIM_t*)&ucesb_struct.twim, offsetof(EXT_STR_h101, twim));

    if (fTofW)
        unpacktofw = new R3BSofTofWReader((EXT_STR_h101_SOFTOFW_t*)&ucesb_struct.tofw, offsetof(EXT_STR_h101, tofw));

    if (fScalers)
        unpackscalers =
            new R3BSofScalersReader((EXT_STR_h101_SOFSCALERS_t*)&ucesb_struct.scalers, offsetof(EXT_STR_h101, scalers));

    if (fNeuland)
    {
        unpackWRNeuland = new R3BWhiterabbitNeulandReader(
            (EXT_STR_h101_WRNEULAND*)&ucesb_struct.wrneuland, offsetof(EXT_STR_h101, wrneuland), 0x1100);
    }

    // Add readers ------------------------------------------
    source->AddReader(new R3BUnpackReader(&ucesb_struct.unpack, offsetof(EXT_STR_h101, unpack)));
    source->AddReader(new R3BTrloiiTpatReader(&ucesb_struct.unpacktpat, offsetof(EXT_STR_h101, unpacktpat)));

    if (fFrsTpcs)
    {
        unpackfrs->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackfrs);
    }

    if (fMusic)
    {
        unpackmusic->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackmusic);
    }
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

    if (fAt)
    {
        unpackat->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackat);
    }

    if (fMwpc0 || fMwpc1 || fMwpc2 || fMwpc3)
    {
        unpackmwpc->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackmwpc);
    }
    if (fAms)
    {
        unpackWRAms->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackWRAms);
        unpackams->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackams);
    }
    if (fCalifa)
    {
        unpackcalifa->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackcalifa);
        unpackWRCalifa->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackWRCalifa);
    }
    if (fTrim)
    {
        unpacktrim->SetOnline(NOTstoremappeddata);
        source->AddReader(unpacktrim);
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
    if (fScalers)
    {
        unpackscalers->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackscalers);
    }
    if (fNeuland)
    {
        unpackWRNeuland->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackWRNeuland);
    }

    // Create online run ------------------------------------
    FairRunOnline* run = new FairRunOnline(source);
    run->SetRunId(fRunId);
    run->SetSink(new FairRootFileSink(outputFilename));
    run->ActivateHttpServer(refresh, port);

    // Runtime data base ------------------------------------
    FairRuntimeDb* rtdb = run->GetRuntimeDb();

    R3BSofStatusOnlineSpectra* sofonline = new R3BSofStatusOnlineSpectra();
    run->AddTask(sofonline);

    // Initialize -------------------------------------------
    run->Init();
    FairLogger::GetLogger()->SetLogScreenLevel("INFO");

    // Informations about portnumber and main data stream.
    cout<<"\n\n"<<endl;
    cout<<"Data stream is: "<<filename<<endl;
    cout<<"Portnumber for status online is: "<<port<<endl;
    cout<<"\n\n"<<endl;

    // Run --------------------------------------------------
    run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);

    // Finish -----------------------------------------------
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    std::cout << std::endl << std::endl;
    std::cout << "Macro finished succesfully." << std::endl;
    std::cout << "Output file is " << outputFilename << std::endl;
    std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl << std::endl;
    // gApplication->Terminate();
}
