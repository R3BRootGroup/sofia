/*
 *  Macro to run the offline for all the detectors simultaneously
 *
 *  One needs to set up the Phase0 experiments: s444, s467, or s455
 *
 *  at $UCESB_DIR/../upexps/yyyymm_s4xx
 *
 *  This macro generates a root file with all the data at mapped level using
 *  a lmd file as input
 *
 *  Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
 *  @since May 4th, 2021
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

    EXT_STR_h101_WRMASTER_t wrmaster;
    EXT_STR_h101_WRCALIFA_t wrcalifa;
    EXT_STR_h101_WRNEULAND_t wrneuland;
    EXT_STR_h101_WRSOFIA_t wrsofia;
    EXT_STR_h101_WRS2_t wrs2;
} EXT_STR_h101;

void unpack_offline()
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

    // NumSoiSci, file names and paths -----------------------------
    Int_t sofiaWR_SE, sofiaWR_ME, NumSofSci, IdS2;
    TString dir = gSystem->Getenv("VMCWORKDIR");
    TString ntuple_options = "RAW"; // For stitched data
    // TString ntuple_options = "RAW,time-stitch=1000"; // For no stitched data
    TString ucesb_dir = getenv("UCESB_DIR");
    TString filename, outputFilename, upexps_dir, ucesb_path, sofiacaldir;

    if (expId == 455)
    {
        NumSofSci = 1;
        IdS2 = 0;
        sofiaWR_SE = 0xe00;
        sofiaWR_ME = 0xf00;

        // Input file
        filename = "~/lmd/s455/main0273_0010_stitched.lmd";

        TString outputpath = "/path/to/your/disk/";
        // outputFilename = outputpath + "s455_map_data_offline_" + oss.str() + ".root";
        outputFilename = "s455_map_data_offline_" + oss.str() + ".root";

        upexps_dir = ucesb_dir + "/../upexps"; // for local computers
        // upexps_dir = "/u/land/fake_cvmfs/9.13/upexps"; // for lxlandana computers
        ucesb_path = upexps_dir + "/202103_s455/202103_s455 --allow-errors --input-buffer=70Mi";
        ucesb_path.ReplaceAll("//", "/");
        sofiacaldir = dir + "/sofia/macros/s455Up2p/parameters/";
    }
    else
    {
        std::cout << "Experiment was not selected" << std::endl;
        gApplication->Terminate();
    }

    // store data or not ------------------------------------
    Bool_t fCal_level_califa = false;  // set true if there exists a file with the calibration parameters
    Bool_t NOTstoremappeddata = false; // if true, don't store mapped data in the root file

    // Setup: Selection of detectors ------------------------
    // --- FRS --------------------------------------------------------------------------
    Bool_t fFrs = false;    // FRS for production of exotic beams (just scintillators)
    Bool_t fFrsSci = false; // Start: Plastic scintillators at FRS
    // --- R3B standard -----------------------------------------------------------------
    Bool_t fNeuland = false; // NeuLAND for neutrons behind GLAD
    Bool_t fAms = false;     // AMS tracking detectors
    Bool_t fCalifa = false;  // Califa calorimeter
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

    // Calibration files for SOFIA ----------------------------------------------
    TString sofiacalfilename = sofiacaldir + "CalibParam.par";
    sofiacalfilename.ReplaceAll("//", "/");
    // Parameters for CALIFA mapping  -------------------------------------------
    TString califadir = dir + "/macros/r3b/unpack/s455/califa/parameters/";
    TString califamapfilename = califadir + "Califa_Mapping_3March2021.par";
    califamapfilename.ReplaceAll("//", "/");
    // Parameters for CALIFA calibration in keV  --------------------------------
    TString califacalfilename = califadir + "Califa_CalPar_4March2021.par";
    califacalfilename.ReplaceAll("//", "/");
    // Parameters for AMS   -----------------------------------------------------
    TString amscalfilename = sofiacaldir + "Ams_CalPar_20210312.par";
    amscalfilename.ReplaceAll("//", "/");

    // Create source using ucesb for input ------------------
    EXT_STR_h101 ucesb_struct;

    R3BUcesbSource* source =
        new R3BUcesbSource(filename, ntuple_options, ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
    source->SetMaxEvents(nev);

    // Definition of reader ---------------------------------
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
    R3BWhiterabbitMasterReader* unpackWRMaster;
    R3BWhiterabbitCalifaReader* unpackWRCalifa;
    R3BSofWhiterabbitReader* unpackWRSofia;
    R3BWhiterabbitNeulandReader* unpackWRNeuland;

    if (fFrsSci)
    {
        unpackWRS2 =
            new R3BWhiterabbitS2Reader((EXT_STR_h101_WRS2*)&ucesb_struct.wrs2, offsetof(EXT_STR_h101, wrs2), 0x200);
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
        unpackams = new R3BAmsReader((EXT_STR_h101_AMS_onion*)&ucesb_struct.ams, offsetof(EXT_STR_h101, ams));

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
        // unpackneuland = new R3BNeulandTamexReader((EXT_STR_h101_raw_nnp_tamex_t*)&ucesb_struct.raw_nnp,
        //                                        offsetof(EXT_STR_h101, raw_nnp));

        unpackWRNeuland = new R3BWhiterabbitNeulandReader(
            (EXT_STR_h101_WRNEULAND*)&ucesb_struct.wrneuland, offsetof(EXT_STR_h101, wrneuland), 0x900);
    }

    // Add readers ------------------------------------------
    source->AddReader(new R3BUnpackReader(&ucesb_struct.unpack, offsetof(EXT_STR_h101, unpack)));
    source->AddReader(new R3BTrloiiTpatReader(&ucesb_struct.unpacktpat, offsetof(EXT_STR_h101, unpacktpat)));

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

    // Initialize -------------------------------------------
    run->Init();
    FairLogger::GetLogger()->SetLogScreenLevel("INFO");

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
    gApplication->Terminate();
}
