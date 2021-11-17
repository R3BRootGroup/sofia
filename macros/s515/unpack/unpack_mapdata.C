/**
 **  Macro to run the offline for all the detectors simultaneously
 **
 **  The unpacker is at $UCESB_DIR/../upexps/202104_s515
 **
 **  This macro generates a root file with all the data at mapped level using
 **  a lmd file as input
 **
 **  Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
 **  @since November 17th, 2021
 **
 **/

typedef struct EXT_STR_h101_t
{
    EXT_STR_h101_unpack_t unpack;
    EXT_STR_h101_TPAT_t unpacktpat;
    EXT_STR_h101_SCI2_t s2;
    EXT_STR_h101_MUSIC_onion_t music;
    EXT_STR_h101_AMS_onion_t ams;
    EXT_STR_h101_CALIFA_t califa;
    EXT_STR_h101_LOS_t los;
    EXT_STR_h101_TOFD_onion_t tofd;
    EXT_STR_h101_PSP_onion_t psp;
    EXT_STR_h101_raw_nnp_tamex_t raw_nnp;

    EXT_STR_h101_SOFMWPC_onion_t mwpc;

    EXT_STR_h101_WRMASTER_t wrmaster;
    EXT_STR_h101_WRS2_t wrs2;
    EXT_STR_h101_WRLOS_t wrlos;
    EXT_STR_h101_TIMESTAMP_PSPX_t wrpspx;
    EXT_STR_h101_WRMUSIC_t wrmusic;
    EXT_STR_h101_WRCALIFA_t wrcalifa;
    EXT_STR_h101_WRNEULAND_t wrneuland;
} EXT_STR_h101;

void unpack_mapdata(const Int_t fRunId = 484, const Int_t nev = -1, const Int_t fExpId = 515)
{
    TString cRunId = Form("%04d", fRunId);
    TString cExpId = Form("%03d", fExpId);

    TStopwatch timer;
    timer.Start();

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d_%H%M%S");

    // File names and paths -----------------------------
    TString dir = gSystem->Getenv("VMCWORKDIR");
    TString ntuple_options = "RAW"; // For stitched data
    // TString ntuple_options = "RAW,time-stitch=1000";
    TString ucesb_dir = getenv("UCESB_DIR");
    TString filename, outputFilename, upexps_dir, ucesb_path;

    if (fExpId == 515)
    {
        filename = "~/lmd/s515/main" + cRunId + "_*.lmd";

        TString outputpath = "/path_to_data/";
        // outputFilename = outputpath + "s515_mapdata_" + oss.str() + ".root";
        outputFilename = "s515_mapdata_" + oss.str() + ".root";
        outputFilename.ReplaceAll("//", "/");

        upexps_dir = ucesb_dir + "/../upexps"; // for local computers
        // upexps_dir = "/u/land/fake_cvmfs/9.13/upexps"; // for lxlandana computers
        ucesb_path = upexps_dir + "/202104_s515/202104_s515 --allow-errors --input-buffer=70Mi";
        ucesb_path.ReplaceAll("//", "/");
    }
    else
    {
        std::cout << "Experiment was not selected" << std::endl;
        gApplication->Terminate();
    }

    // Setup: Selection of detectors ------------------------
    // --- FRS --------------------------------------------------------------------------
    Bool_t fFrsSci = true; // Start: Plastic scintillators at FRS
    // --- R3B standard -----------------------------------------------------------------
    Bool_t fPsp = true;      // Psp: Silicon detectors for tracking
    Bool_t fLos = true;      // Los scintillator for R3B experiments
    Bool_t fAms = false;     // AMS tracking detectors
    Bool_t fCalifa = false;  // Califa calorimeter
    Bool_t fMusic = true;    // R3B-Music: Ionization chamber for charge-Z before GLAD
    Bool_t fTofD = false;    // ToF-Wall for time-of-flight of fragments behind GLAD
    Bool_t fNeuland = false; // NeuLAND for neutrons behind GLAD
    // --- Sofia ------------------------------------------------------------------------
    Bool_t fMwpc0 = false; // MWPC0 for tracking at Cave-C entrance

    // Create online run ------------------------------------
    R3BEventHeader* EvntHeader = new R3BEventHeader();
    FairRunOnline* run = new FairRunOnline();
    run->SetEventHeader(EvntHeader);
    run->SetRunId(fRunId);
    run->SetSink(new FairRootFileSink(outputFilename));

    // Create source using ucesb for input ------------------
    EXT_STR_h101 ucesb_struct;

    R3BUcesbSource* source =
        new R3BUcesbSource(filename, ntuple_options, ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
    source->SetMaxEvents(nev);

    // Definition of reader ---------------------------------
    R3BSci2Reader* unpacks2;
    R3BLosReader* unpacklos;
    R3BPspxReader* unpackpsp;
    R3BAmsReader* unpackams;
    R3BCalifaFebexReader* unpackcalifa;
    R3BSofMwpcReader* unpackmwpc;
    R3BMusicReader* unpackmusic;
    R3BTofdReader* unpacktofd;
    R3BNeulandTamexReader* unpackneuland;

    R3BWhiterabbitMasterReader* unpackWRMaster;
    R3BWhiterabbitS2Reader* unpackWRS2;
    R3BWhiterabbitPspReader* unpackWRpsp;
    R3BWhiterabbitLosReader* unpackWRlos;
    R3BWhiterabbitMusicReader* unpackWRmusic;
    R3BWhiterabbitCalifaReader* unpackWRCalifa;
    R3BWhiterabbitNeulandReader* unpackWRNeuland;

    // Add readers ------------------------------------------
    source->AddReader(new R3BUnpackReader(&ucesb_struct.unpack, offsetof(EXT_STR_h101, unpack)));
    source->AddReader(new R3BTrloiiTpatReader(&ucesb_struct.unpacktpat, offsetof(EXT_STR_h101, unpacktpat)));
    unpackWRMaster = new R3BWhiterabbitMasterReader(
        (EXT_STR_h101_WRMASTER*)&ucesb_struct.wrmaster, offsetof(EXT_STR_h101, wrmaster), 0x1000);
    unpackWRMaster->SetOnline(true);
    source->AddReader(unpackWRMaster);

    if (fFrsSci)
    {
        unpackWRS2 =
            new R3BWhiterabbitS2Reader((EXT_STR_h101_WRS2*)&ucesb_struct.wrs2, offsetof(EXT_STR_h101, wrs2), 0x200);
        unpacks2 = new R3BSci2Reader(&ucesb_struct.s2, offsetof(EXT_STR_h101_t, s2));
    }

    if (fMusic)
    {
        unpackWRmusic = new R3BWhiterabbitMusicReader(
            (EXT_STR_h101_WRMUSIC*)&ucesb_struct.wrmusic, offsetof(EXT_STR_h101, wrmusic), 0xe00);
        unpackmusic = new R3BMusicReader((EXT_STR_h101_MUSIC_t*)&ucesb_struct.music, offsetof(EXT_STR_h101, music));
    }

    if (fLos)
    {
        unpackWRlos = new R3BWhiterabbitLosReader(
            (EXT_STR_h101_WRLOS*)&ucesb_struct.wrlos, offsetof(EXT_STR_h101, wrlos), 0x1100);
        unpacklos = new R3BLosReader((EXT_STR_h101_LOS_t*)&ucesb_struct.los, offsetof(EXT_STR_h101, los));
    }

    if (fPsp)
    {
        unpackWRpsp = new R3BWhiterabbitPspReader(
            (EXT_STR_h101_TIMESTAMP_PSPX*)&ucesb_struct.wrpspx, offsetof(EXT_STR_h101, wrpspx), 0xc00);
        unpackpsp = new R3BPspxReader((EXT_STR_h101_PSP*)&ucesb_struct.psp, offsetof(EXT_STR_h101, psp));
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
    if (fMwpc0)
        unpackmwpc = new R3BSofMwpcReader((EXT_STR_h101_SOFMWPC_t*)&ucesb_struct.mwpc, offsetof(EXT_STR_h101, mwpc));

    if (fTofD)
        unpacktofd = new R3BTofdReader((EXT_STR_h101_TOFD*)&ucesb_struct.tofd, offsetof(EXT_STR_h101, tofd));

    if (fNeuland)
    {
        // unpackneuland = new R3BNeulandTamexReader((EXT_STR_h101_raw_nnp_tamex_t*)&ucesb_struct.raw_nnp,
        //                                        offsetof(EXT_STR_h101, raw_nnp));

        unpackWRNeuland = new R3BWhiterabbitNeulandReader(
            (EXT_STR_h101_WRNEULAND*)&ucesb_struct.wrneuland, offsetof(EXT_STR_h101, wrneuland), 0x900);
    }

    if (fFrsSci)
    {
        source->AddReader(unpackWRS2);
        source->AddReader(unpacks2);
    }

    if (fMusic)
    {
        source->AddReader(unpackWRmusic);
        source->AddReader(unpackmusic);
    }

    if (fLos)
    {
        source->AddReader(unpackWRlos);
        source->AddReader(unpacklos);
    }

    if (fPsp)
    {
        source->AddReader(unpackWRpsp);
        source->AddReader(unpackpsp);
    }

    if (fMwpc0)
    {
        source->AddReader(unpackmwpc);
    }
    if (fAms)
    {
        source->AddReader(unpackams);
    }
    if (fCalifa)
    {
        source->AddReader(unpackcalifa);
        source->AddReader(unpackWRCalifa);
    }
    if (fTofD)
    {
        source->AddReader(unpacktofd);
    }
    if (fNeuland)
    {
        source->AddReader(unpackWRNeuland);
    }
    run->SetSource(source);

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
    std::cout << "Real time " << rtime / 60. << " min, CPU time " << ctime / 60. << " min" << std::endl << std::endl;
    gApplication->Terminate();
}
