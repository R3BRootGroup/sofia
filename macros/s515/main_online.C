/*
 *  Macro to run the online for all the detectors simultaneously
 *
 *  One needs to set up the Phase0 experiments, in this case s515
 *
 *  at $UCESB_DIR/../upexps/202104_s515
 *
 *
 *  Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
 *  @since April 19th, 2021
 *
 */

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
    EXT_STR_h101_WRCALIFA_t wrcalifa;
    EXT_STR_h101_WRNEULAND_t wrneuland;
    EXT_STR_h101_WRSOFIA_t wrsofia;
    EXT_STR_h101_WRS2_t wrs2;

} EXT_STR_h101;

void main_online()
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
    const Int_t expId = 515; // select experiment 515
    // *********************************** //

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // AoverQ for FRS-ID
    // -------------------------------------------------------------
    // Run 484
    Double_t fBrho = 13.0721;
    Double_t fTofOffset = -5644.95117;
    // Run 473
    // Double_t fBrho = 10.2096;
    // Double_t fTofOffset = -16824.97;
    // Run 458
    // Double_t fBrho = 11.9891;
    // Double_t fTofOffset = 0.0;
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // NumSoiSci, file names and paths -----------------------------
    Int_t sofiaWR_SE, sofiaWR_ME, NumSofSci, IdS2, IdS8;
    TString dir = gSystem->Getenv("VMCWORKDIR");
    // TString ntuple_options = "RAW"; // For stitched data
    TString ntuple_options = "RAW,time-stitch=1000"; // For no stitched data
    TString ucesb_dir = getenv("UCESB_DIR");
    TString filename, outputFilename, upexps_dir, ucesb_path, sofiacaldir;

    if (expId == 515)
    {
        // filename = "--stream=lxlanddaq01:9001";
        filename = "~/lmd/s515/main0484_0001.lmd";

        TString outputpath = "/d/land5/202104_s515/rootfiles/sofia/";
        // outputFilename = outputpath + "s515_data_sofia_online_" + oss.str() + ".root";
        outputFilename = "s515_data_sofia_online_" + oss.str() + ".root";

        upexps_dir = ucesb_dir + "/../upexps"; // for local computers
        // upexps_dir = "/u/land/fake_cvmfs/9.13/upexps"; // for lxlandana computers
        // upexps_dir = "/u/land/lynx.landexp/202104_s515/upexps/";  // for lxg computers
        ucesb_path = upexps_dir + "/202104_s515/202104_s515 --allow-errors --input-buffer=100Mi";
        ucesb_path.ReplaceAll("//", "/");
        sofiacaldir = dir + "/sofia/macros/s515/parameters/";
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
    Int_t port = 8888; // Port number for the online visualization, example lxgXXXX:8888

    // Setup: Selection of detectors ------------------------
    // --- FRS --------------------------------------------------------------------------
    Bool_t fFrsid = true;   // FRS for production of exotic beams (just scintillators)
    Bool_t fFrsSci = false; // Start: Plastic scintillators at FRS
    // --- R3B standard -----------------------------------------------------------------
    Bool_t fNeuland = false; // NeuLAND for neutrons behind GLAD
    Bool_t fLos = true;      // Los scintillator for R3B experiments
    Bool_t fAms = false;     // AMS tracking detectors
    Bool_t fCalifa = false;  // Califa calorimeter
    Bool_t fMusic = true;    // R3B-Music: Ionization chamber for charge-Z before GLAD
    Bool_t fPsp = false;     // Psp: Silicon detectors for tracking
    // --- Sofia ------------------------------------------------------------------------
    Bool_t fMwpc0 = false; // MWPC0 for tracking at entrance of GLAD
    Bool_t fTofD = false;  // ToF-Wall for time-of-flight of fragments behind GLAD
    // --- Traking ----------------------------------------------------------------------
    Bool_t fTracking = false; // Tracking of fragments inside GLAD and before GLAD

    // Calibration files ------------------------------------
    // SOFIA parameters
    TString sofiacalfilename = sofiacaldir + "CalibParam_sofia.par";
    sofiacalfilename.ReplaceAll("//", "/");

    // Parameters for CALIFA mapping
    TString califadir = dir + "/macros/macros/r3b/unpack/s515/califa/parameters/";
    TString califamapfilename = califadir + "CALIFA_mapping_s515.par";
    califamapfilename.ReplaceAll("//", "/");
    // Parameters for CALIFA calibration in keV
    TString califacalfilename = califadir + "Califa_CalPar_s515_New.par";
    califacalfilename.ReplaceAll("//", "/");

    // Parameters for LOS
    TString pardir = dir + "/sofia/macros/s515/parameters/";
    TString loscalfilename = pardir + "tcal_los_pulser.root";
    loscalfilename.ReplaceAll("//", "/");

    // Parameters for S2 scintillator
    TString scis2calfilename = pardir + "tcal_s2.root";
    scis2calfilename.ReplaceAll("//", "/");

    // Parameters for ToFD
    TString tofdcalfilename = pardir + "TofdTcalPar_main0019.root";
    tofdcalfilename.ReplaceAll("//", "/");

    // Create online run ------------------------------------
    R3BEventHeader* EvntHeader = new R3BEventHeader();
    FairRunOnline* run = new FairRunOnline();
    run->SetEventHeader(EvntHeader);
    run->SetRunId(fRunId);
    run->SetSink(new FairRootFileSink(outputFilename));
    run->ActivateHttpServer(refresh, port);

    // Create source using ucesb for input ------------------
    EXT_STR_h101 ucesb_struct;

    R3BUcesbSource* source =
        new R3BUcesbSource(filename, ntuple_options, ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
    source->SetMaxEvents(nev);

    // Definition of reader ---------------------------------
    R3BMusicReader* unpackmusic;
    R3BLosReader* unpacklos;
    R3BAmsReader* unpackams;
    R3BCalifaFebexReader* unpackcalifa;
    R3BPspxReader* unpackpsp;
    R3BTofdReader* unpacktofd;
    R3BNeulandTamexReader* unpackneuland;
    R3BSci2Reader* unpacks2;
    R3BSofMwpcReader* unpackmwpc;

    R3BWhiterabbitS2Reader* unpackWRS2;
    R3BWhiterabbitMasterReader* unpackWRMaster;
    R3BWhiterabbitCalifaReader* unpackWRCalifa;
    R3BSofWhiterabbitReader* unpackWRSofia;
    R3BWhiterabbitNeulandReader* unpackWRNeuland;

    unpackWRMaster = new R3BWhiterabbitMasterReader(
        (EXT_STR_h101_WRMASTER*)&ucesb_struct.wrmaster, offsetof(EXT_STR_h101, wrmaster), 0x1000);

    if (fFrsid)
        unpacks2 = new R3BSci2Reader(&ucesb_struct.s2, offsetof(EXT_STR_h101_t, s2));

    if (fMusic)
        unpackmusic = new R3BMusicReader((EXT_STR_h101_MUSIC_t*)&ucesb_struct.music, offsetof(EXT_STR_h101, music));

    if (fLos)
    {
        unpacklos = new R3BLosReader((EXT_STR_h101_LOS_t*)&ucesb_struct.los, offsetof(EXT_STR_h101, los));
    }

    if (fPsp)
        unpackpsp = new R3BPspxReader((EXT_STR_h101_PSP*)&ucesb_struct.psp, offsetof(EXT_STR_h101, psp));

    if (fFrsSci)
    {
        unpackWRS2 =
            new R3BWhiterabbitS2Reader((EXT_STR_h101_WRS2*)&ucesb_struct.wrs2, offsetof(EXT_STR_h101, wrs2), 0x200);
        unpackWRSofia = new R3BSofWhiterabbitReader(
            (EXT_STR_h101_WRSOFIA*)&ucesb_struct.wrsofia, offsetof(EXT_STR_h101, wrsofia), 0x1100, 0);
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

    // Add readers ------------------------------------------
    source->AddReader(new R3BUnpackReader(&ucesb_struct.unpack, offsetof(EXT_STR_h101, unpack)));
    source->AddReader(new R3BTrloiiTpatReader(&ucesb_struct.unpacktpat, offsetof(EXT_STR_h101, unpacktpat)));

    if (fFrsid)
    {
        source->AddReader(unpacks2);
    }
    if (fMusic)
    {
        unpackmusic->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackmusic);
    }

    if (fLos)
    {
        unpacklos->SetOnline(NOTstoremappeddata);
        source->AddReader(unpacklos);
    }

    if (fPsp)
    {
        unpackpsp->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackpsp);
    }

    if (fFrsSci)
    {
        unpackWRS2->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackWRS2);
        unpackWRSofia->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackWRSofia);
    }

    if (fMwpc0)
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
    if (fTofD)
    {
        unpacktofd->SetOnline(NOTstoremappeddata);
        source->AddReader(unpacktofd);
    }
    if (fNeuland)
    {
        unpackWRNeuland->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackWRNeuland);
    }
    run->SetSource(source);

    // Runtime data base ------------------------------------
    FairRuntimeDb* rtdb = run->GetRuntimeDb();

    FairParAsciiFileIo* parIo1;

    if (!fCalifa)
    {
        if (fFrsid || fLos)
        {
            // Root file
            Bool_t kParameterMerged = kTRUE;
            FairParRootFileIo* parIo2 = new FairParRootFileIo(kParameterMerged);
            TList* parList2 = new TList();
            if (fLos)
                parList2->Add(new TObjString(loscalfilename));
            if (fFrsid)
                parList2->Add(new TObjString(scis2calfilename));
            if (fTofD)
                parList2->Add(new TObjString(tofdcalfilename));
            parIo2->open(parList2);
            rtdb->setFirstInput(parIo2);

            parIo1 = new FairParAsciiFileIo(); // Ascii
            parIo1->open(sofiacalfilename, "in");
            rtdb->setSecondInput(parIo1);

            rtdb->print();

            if (fFrsid || fLos || fTofD)
                rtdb->addRun(fRunId);
            if (fFrsid && fLos)
            {
                rtdb->getContainer("LosTCalPar");
                rtdb->setInputVersion(fRunId, (char*)"LosTCalPar", 1, 1);
                rtdb->getContainer("Sci2TCalPar");
                rtdb->setInputVersion(fRunId, (char*)"Sci2TCalPar", 1, 1);
            }
            if (fTofD)
            {
                rtdb->getContainer("TofdTCalPar");
                rtdb->setInputVersion(fRunId, (char*)"TofdTCalPar", 1, 1);
            }
        }
        else
        {

            parIo1 = new FairParAsciiFileIo(); // Ascii
            parIo1->open(sofiacalfilename, "in");
            rtdb->setFirstInput(parIo1);
        }
    }
    else
    {
        if (!fCal_level_califa)
        { // SOFIA and CALIFA mapping: Ascii files
            if (fFrsid || fLos)
            {
                // Root file
                Bool_t kParameterMerged = kTRUE;
                FairParRootFileIo* parIo2 = new FairParRootFileIo(kParameterMerged);
                TList* parList2 = new TList();
                if (fLos)
                    parList2->Add(new TObjString(loscalfilename));
                if (fFrsid)
                    parList2->Add(new TObjString(scis2calfilename));
                if (fTofD)
                    parList2->Add(new TObjString(tofdcalfilename));
                parIo2->open(parList2);
                rtdb->setFirstInput(parIo2);

                parIo1 = new FairParAsciiFileIo(); // Ascii
                TList* parList1 = new TList();
                parList1->Add(new TObjString(sofiacalfilename));
                parList1->Add(new TObjString(califamapfilename));
                parIo1->open(parList1);
                rtdb->setSecondInput(parIo1);

                rtdb->print();
                if (fFrsid || fLos || fTofD)
                    rtdb->addRun(fRunId);
                if (fFrsid && fLos)
                {
                    rtdb->getContainer("LosTCalPar");
                    rtdb->setInputVersion(fRunId, (char*)"LosTCalPar", 1, 1);
                    rtdb->getContainer("Sci2TCalPar");
                    rtdb->setInputVersion(fRunId, (char*)"Sci2TCalPar", 1, 1);
                }
                if (fTofD)
                {
                    rtdb->getContainer("TofdTCalPar");
                    rtdb->setInputVersion(fRunId, (char*)"TofdTCalPar", 1, 1);
                }
            }
            else
            {
                parIo1 = new FairParAsciiFileIo(); // Ascii
                TList* parList1 = new TList();
                parList1->Add(new TObjString(sofiacalfilename));
                parList1->Add(new TObjString(califamapfilename));
                parIo1->open(parList1);
                rtdb->setFirstInput(parIo1);
                rtdb->print();
            }
        }
        else
        { // SOFIA, CALIFA mapping and CALIFA calibration parameters

            if (fFrsid || fLos)
            {
                // Root file
                Bool_t kParameterMerged = kTRUE;
                FairParRootFileIo* parIo2 = new FairParRootFileIo(kParameterMerged);
                TList* parList2 = new TList();
                if (fLos)
                    parList2->Add(new TObjString(loscalfilename));
                if (fFrsid)
                    parList2->Add(new TObjString(scis2calfilename));
                parIo2->open(parList2);
                rtdb->setFirstInput(parIo2);

                parIo1 = new FairParAsciiFileIo(); // Ascii
                TList* parList1 = new TList();
                parList1->Add(new TObjString(sofiacalfilename));
                parList1->Add(new TObjString(califacalfilename));
                parIo1->open(parList1);
                rtdb->setSecondInput(parIo1);

                rtdb->print();
                if (fFrsid && fLos)
                {
                    rtdb->addRun(fRunId);
                    rtdb->getContainer("LosTCalPar");
                    rtdb->setInputVersion(fRunId, (char*)"LosTCalPar", 1, 1);
                    rtdb->getContainer("Sci2TCalPar");
                    rtdb->setInputVersion(fRunId, (char*)"Sci2TCalPar", 1, 1);
                }
            }
            else
            {
                parIo1 = new FairParAsciiFileIo(); // Ascii
                TList* parList1 = new TList();
                parList1->Add(new TObjString(sofiacalfilename));
                parList1->Add(new TObjString(califacalfilename));
                parIo1->open(parList1);
                rtdb->setFirstInput(parIo1);
                rtdb->print();
            }
        }
    }

    // Add analysis task ------------------------------------
    // S2 Mapped -> Tcal
    if (fFrsid)
    {
        R3BSci2Mapped2Tcal* s2Mapped2Tcal = new R3BSci2Mapped2Tcal();
        s2Mapped2Tcal->SetOnline(NOTstorecaldata);
        run->AddTask(s2Mapped2Tcal);
        
        R3BSci2Tcal2Hit* sci2Tcal2Hit = new R3BSci2Tcal2Hit();
        sci2Tcal2Hit->SetOnline(NOTstorehitdata);
        run->AddTask(sci2Tcal2Hit);
    }

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

    // R3B-MUSIC
    if (fMusic)
    {
        R3BMusicMapped2Cal* MusMap2Cal = new R3BMusicMapped2Cal();
        MusMap2Cal->SetOnline(NOTstorecaldata);
        run->AddTask(MusMap2Cal);

        R3BMusicCal2Hit* MusCal2Hit = new R3BMusicCal2Hit();
        MusCal2Hit->SetOnline(NOTstorehitdata);
        run->AddTask(MusCal2Hit);
    }

    // R3B-Scintillator LOS
    if (fLos)
    {
        R3BLosMapped2Cal* losMapped2Cal = new R3BLosMapped2Cal("R3BLosMapped2Cal", 1);
        losMapped2Cal->SetNofModules(1, 8);
        losMapped2Cal->SetTrigger(1);
        losMapped2Cal->SetOnline(NOTstorecaldata);
        run->AddTask(losMapped2Cal);
    }

    // Psp silicon detectors
    if (fPsp)
    {
        R3BPspxMapped2Precal* pspxMapped2Precal = new R3BPspxMapped2Precal();
        pspxMapped2Precal->SetOnline(NOTstorecaldata);
        run->AddTask(pspxMapped2Precal);

        R3BPspxPrecal2Cal* pspxPrecal2Cal = new R3BPspxPrecal2Cal();
        pspxPrecal2Cal->SetOnline(NOTstorecaldata);
        run->AddTask(pspxPrecal2Cal);

        R3BPspxCal2Hit* pspxCal2Hit = new R3BPspxCal2Hit();
        pspxCal2Hit->SetOnline(NOTstorehitdata);
        run->AddTask(pspxCal2Hit);
    }

    // AMS
    if (fAms)
    {
        R3BAmsMapped2StripCal* AmsMap2Cal = new R3BAmsMapped2StripCal();
        AmsMap2Cal->SetOnline(NOTstorecaldata);
        run->AddTask(AmsMap2Cal);
        R3BAmsStripCal2Hit* AmsCal2Hit = new R3BAmsStripCal2Hit();
        AmsCal2Hit->SetOnline(NOTstorehitdata);
        run->AddTask(AmsCal2Hit);
    }

    // CALIFA
    if (fCalifa && fCal_level_califa)
    {
        // R3BCalifaMapped2CrystalCal ---
        R3BCalifaMapped2CrystalCal* CalifaMap2Cal = new R3BCalifaMapped2CrystalCal();
        CalifaMap2Cal->SetOnline(NOTstorecaldata);
        run->AddTask(CalifaMap2Cal);
        // R3BCalifaCrystalCal2Hit ---
        R3BCalifaCrystalCal2Hit* CalifaCal2Hit = new R3BCalifaCrystalCal2Hit();
        CalifaCal2Hit->SetCrystalThreshold(350.); // 100keV
        CalifaCal2Hit->SetDRThreshold(20000.);    // 10MeV
        CalifaCal2Hit->SelectGeometryVersion(2021);
        CalifaCal2Hit->SetOnline(NOTstorehitdata);
        run->AddTask(CalifaCal2Hit);
    }

    if (fTofD)
    {
        R3BTofdMapped2Cal* tofdMapped2Cal = new R3BTofdMapped2Cal();
        tofdMapped2Cal->SetNofModules(4, 44);
        run->AddTask(tofdMapped2Cal);
    }

    // Add online task ------------------------------------

    if (fFrsid)
    {
        // --- SCI2 STANDALONE
        R3BOnlineSpectraSci2* s2online = new R3BOnlineSpectraSci2();
        s2online->SetNbDetectors(1);
        s2online->SetNbChannels(3);
        run->AddTask(s2online);
    }

    if (fLos)
    {
        if (fFrsid)
        {
            // --- LOS VS SCI2
            R3BOnlineSpectraLosVsSci2* loss2online = new R3BOnlineSpectraLosVsSci2("R3BOnlineSpectraLosVsSci2", 1);
            loss2online->SetNofLosModules(1); // 1 or 2 LOS detectors
            //  Set parameters for X,Y calibration
            //  (offsetX, offsetY,VeffX,VeffY)
            loss2online->SetLosXYTAMEX(0, 0, 1, 1, 0, 0, 1, 1);
            loss2online->SetLosXYMCFD(1.011, 1.216, 1.27, 1.88, 0, 0, 1, 1); //(0.9781,1.152,1.5,1.5,0,0,1,1);
            loss2online->SetLosXYToT(
                -0.002373, 0.007423, 2.27, 3.22, 0, 0, 1, 1); //(-0.02054,-0.02495,2.5,3.6,0,0,1,1);
            loss2online->SetEpileup(180.);                    // Events with ToT>Epileup are not considered
            loss2online->SetTrigger(1);                       // -1 = no trigger selection
            loss2online->SetTpat(0);                          // if 0, no tpat selection
            // AoQ calibration :
            loss2online->SetToFoffset(fTofOffset);
            loss2online->SetToFmin(-8050);
            loss2online->SetToFmax(8050);
            loss2online->SetTof2InvV_p0(67.69245);
            loss2online->SetTof2InvV_p1(0.007198663);
            loss2online->SetFlightLength(139.915);
            loss2online->SetPos_p0(126.451);
            loss2online->SetPos_p1(56.785);
            loss2online->SetDispersionS2(7000);
            loss2online->SetBrho0_S2toCC(fBrho);
            // 3 parameters for velocity correction, primary beam charge and offset in Z.
            // loss2online->SetBetaCorrectionForZ(-2.12371e7, 4.9473e7,-2.87635e7,50.,-17.3);
            run->AddTask(loss2online);
        }
        else
        {
            R3BOnlineSpectraLosStandalone* r3bOnlineSpectra =
                new R3BOnlineSpectraLosStandalone("R3BOnlineSpectraLosStandalone", 1);
            r3bOnlineSpectra->SetNofLosModules(1); // 1 or 2 LOS detectors
            r3bOnlineSpectra->SetLosXYTAMEX(0, 0, 1, 1, 0, 0, 1, 1);
            r3bOnlineSpectra->SetLosXYMCFD(1.011, 1.216, 1.27, 1.88, 0, 0, 1, 1);
            r3bOnlineSpectra->SetLosXYToT(-0.002373, 0.007423, 2.27, 3.22, 0, 0, 1, 1);
            // Events with ToT>Epileup are not considered
            r3bOnlineSpectra->SetEpileup(180.);
            // -1 = no trigger selection
            r3bOnlineSpectra->SetTrigger(1);
            // if 0, no tpat selection
            r3bOnlineSpectra->SetTpat(0);
            run->AddTask(r3bOnlineSpectra);
        }
    }

    if (fPsp)
    {
        R3BPspxOnlineSpectra* pspOnline = new R3BPspxOnlineSpectra("PspxOnlineSpectra", 1);
        run->AddTask(pspOnline);
        /*
        if (fSci && NumSofSci == 2)
        {
            R3BSofSciVsPspxOnlineSpectra* pspsciOnline = new R3BSofSciVsPspxOnlineSpectra();
            pspsciOnline->SetDispersionS2(7250.);
            pspsciOnline->SetBrho0(9.3148);
            run->AddTask(pspsciOnline);
        }*/
    }

    if (fMwpc0)
    {
        R3BSofMwpcOnlineSpectra* mw0online = new R3BSofMwpcOnlineSpectra("SofMwpc0OnlineSpectra", 1, "Mwpc0");
        run->AddTask(mw0online);
    }

    if (fMusic)
    {
        R3BMusicOnlineSpectra* musonline = new R3BMusicOnlineSpectra();
        musonline->SetExpId(expId);
        run->AddTask(musonline);
        if (fMwpc0)
        {
            R3BSofMwpcvsMusicOnlineSpectra* mw0vsmusiconline =
                new R3BSofMwpcvsMusicOnlineSpectra("SofMwpc0vsMusicOnlineSpectra", 1, "Mwpc0");
            run->AddTask(mw0vsmusiconline);
        }
    }

    if (fAms)
    {
        R3BAmsOnlineSpectra* AmsOnline = new R3BAmsOnlineSpectra();
        run->AddTask(AmsOnline);
    }

    if (fCalifa)
    {
        R3BCalifaOnlineSpectra* CalifaOnline = new R3BCalifaOnlineSpectra();
        CalifaOnline->SetRange_max(30000); // 30000 -> 30MeV
        CalifaOnline->SetBinChannelFebex(500);
        CalifaOnline->SetMaxBinFebex(10000); // 10000 -> 10MeV
        run->AddTask(CalifaOnline);
    }

    R3BSofOnlineSpectra* sofonline = new R3BSofOnlineSpectra();
    run->AddTask(sofonline);

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
    // gApplication->Terminate();
}
