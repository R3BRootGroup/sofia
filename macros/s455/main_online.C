/*
 *  Macro to run the online for all the detectors simultaneously
 *
 *  One needs to set up the Phase0 experiments: s444, s467, or s455
 *
 *  at $UCESB_DIR/../upexps/yyyymm_s4xx
 *
 *
 *  Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
 *  @since Feb 20th, 2020
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

    EXT_STR_h101_SOFCORRM_onion_t corrm;
    EXT_STR_h101_SOFCORRV_onion_t corrv;

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
    const Int_t expId = 455;               // select experiment: 444, 467 or 455
    // *********************************** //
    
    // NumSoiSci, file names and paths -----------------------------
    Int_t sofiaWR_SE, sofiaWR_ME, NumSofSci, IdS2, IdS8;
    TString dir = gSystem->Getenv("VMCWORKDIR");
    //TString ntuple_options = "RAW";                     // ntuple_options for stitched events
    TString ntuple_options = "RAW,time-stitch=1000";  // ntuple_options for raw events
    TString ucesb_dir = getenv("UCESB_DIR");
    TString filename, outputFilename, upexps_dir, ucesb_path, sofiacaldir;

    if (expId == 455)
    {
        NumSofSci = 2;
        IdS2 = 1;
        IdS8 = 0;
        sofiaWR_SE = 0xe00;
        sofiaWR_ME = 0xf00;

        //filename = "--stream=lxlanddaq01:9000"; // 9000 is for stitched events 
        filename = "--stream=lxlanddaq01:9100"; // 9100 is for raw events (needed for multi-events unpacking)
        //filename = "/d/land5/202104_s455/lmd/main0420_0001.lmd*"; // 9100 is for raw events (needed for multi-events unpacking)

        TString outputpath = "/d/land5/202103_s455/rootfiles/sofia/";
        outputFilename = outputpath + "s455_data_sofia_online_" + oss.str() + ".root";

        //upexps_dir = ucesb_dir + "/../upexps/"; // for local computers
        upexps_dir = "/u/land/fake_cvmfs/9.13/upexps"; // for lxlandana computers
        ucesb_path = upexps_dir + "/202104_s455/202104_s455_part2 --allow-errors --input-buffer=70Mi";

        sofiacaldir = dir + "/sofia/macros/s455/parameters/";
    }
    else
    {
        std::cout << "Experiment was not selected" << std::endl;
        gApplication->Terminate();
    }
    
    // SOFIA parameters
    TString sofiacalfilename;
    if (NumSofSci == 1)
        sofiacalfilename = sofiacaldir + "CalibParam_onesci.par";
    else if (NumSofSci == 2)
        sofiacalfilename = sofiacaldir + "CalibParam_twosci.par";
    ucesb_path.ReplaceAll("//", "/");
    sofiacalfilename.ReplaceAll("//", "/");
    
    // store data or not ------------------------------------
    Bool_t fCal_level_califa = false;  // set true if there exists a file with the calibration parameters
    Bool_t NOTstoremappeddata = true; // if true, don't store mapped data in the root file
    Bool_t NOTstorecaldata = true;    // if true, don't store cal data in the root file
    Bool_t NOTstorehitdata = true;   // if true, don't store hit data in the root file

    // Online server configuration --------------------------
    Int_t refresh = 1; // Refresh rate for online histograms
    Int_t port = 8888; // Port number for the online visualization, example lxgXXXX:8888

    // Setup: Selection of detectors ------------------------
    // --- FRS --------------------------------------------------------------------------
    Bool_t fFrs = false;     // FRS for production of exotic beams (just scintillators)
    Bool_t fFrsTpcs = false; // Tpcs at FRS (S2) for scintillator calibration in position
    Bool_t fFrsMws = false;  // MWs at FRS (S8) for beam position
    Bool_t fFrsSci = true;   // Start: Plastic scintillators at FRS
    // --- R3B standard -----------------------------------------------------------------
    Bool_t fNeuland = false;  // NeuLAND for neutrons behind GLAD
    Bool_t fAms = false;     // AMS tracking detectors
    Bool_t fCalifa = false;  // Califa calorimeter
    Bool_t fMusic = false;    // R3B-Music: Ionization chamber for charge-Z
    // --- Sofia ------------------------------------------------------------------------
    Bool_t fMwpc0 = true;    // MWPC0 for tracking at entrance of Cave-C
    Bool_t fTrim = true;     // Triple-MUSIC for the HI beam charge-Z id, with charge-q states
    Bool_t fAt = true;       // Active Targer for Coulomb-induced fission
    Bool_t fSci = true;      // Start: Plastic scintillator for ToF
    Bool_t fMwpc1 = true;    // MWPC1 for tracking of fragments in front of target
    Bool_t fMwpc2 = true;    // MWPC2 for tracking of fragments before GLAD
    Bool_t fTwim = true;     // Twim: Ionization chamber for charge-Z of fragments
    Bool_t fMwpc3 = true;    // MWPC3 for tracking of fragments behind GLAD
    Bool_t fTofW = true;     // ToF-Wall for time-of-flight of fragments behind GLAD
    Bool_t fScalers = false;  // SIS3820 scalers at Cave C
    // --- Traking ----------------------------------------------------------------------
    Bool_t fTracking = true; // Tracking of fragments inside GLAD and before GLAD
    // --- Correlation signals between the different DAQ subsystems ---------------------
    Bool_t fCorrm = true;     // correlation and trigger signals on sofia_mesy at cave C
    Bool_t fCorrv = true;     // correlation signal on sofia_vftx at cave C

    // Calibration files ------------------------------------
    // Parameters for CALIFA mapping
    TString califadir = dir + "/macros/r3b/unpack/s455/califa/parameters/";
    TString califamapfilename = califadir + "Califa_Mapping_3March2021.par";
    califamapfilename.ReplaceAll("//", "/");
    // Parameters for CALIFA calibration in keV
    TString califacalfilename = califadir + "Califa_CalPar_4March2021.par";
    califacalfilename.ReplaceAll("//", "/");

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

		R3BSofCorrmReader* unpackcorrm;
		R3BSofCorrvReader* unpackcorrv;


    if (fFrsTpcs)
      unpackfrs= new R3BFrsReaderNov19((EXT_STR_h101_FRS*)&ucesb_struct.frs,
					     offsetof(EXT_STR_h101, frs));

    if (fMusic)
        unpackmusic = new R3BMusicReader((EXT_STR_h101_MUSIC_t*)&ucesb_struct.music, offsetof(EXT_STR_h101, music));

    if(fFrsSci) {
     unpackWRS2 = new R3BWhiterabbitS2Reader(
            (EXT_STR_h101_WRS2*)&ucesb_struct.wrs2, offsetof(EXT_STR_h101, wrs2), 0x200);
     unpackWRS8 = new R3BWhiterabbitS8Reader(
            (EXT_STR_h101_WRS8*)&ucesb_struct.wrs8, offsetof(EXT_STR_h101, wrs8), 0x800);
    }

    if (fSci)
    {
      unpacksci = new R3BSofSciReader((EXT_STR_h101_SOFSCI_t*)&ucesb_struct.sci, offsetof(EXT_STR_h101, sci),NumSofSci);
      unpackWRMaster = new R3BWhiterabbitMasterReader((EXT_STR_h101_WRMASTER*)&ucesb_struct.wrmaster, offsetof(EXT_STR_h101, wrmaster), 0x1000);
      unpackWRSofia = new R3BSofWhiterabbitReader((EXT_STR_h101_WRSOFIA*)&ucesb_struct.wrsofia, offsetof(EXT_STR_h101, wrsofia), sofiaWR_SE, sofiaWR_ME);
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

    if (fNeuland){
        //unpackneuland = new R3BNeulandTamexReader((EXT_STR_h101_raw_nnp_tamex_t*)&ucesb_struct.raw_nnp,
          //                                        offsetof(EXT_STR_h101, raw_nnp));

        unpackWRNeuland = new R3BWhiterabbitNeulandReader(
            (EXT_STR_h101_WRNEULAND*)&ucesb_struct.wrneuland, offsetof(EXT_STR_h101, wrneuland), 0x900);
    }

		if(fCorrm)
			unpackcorrm = new R3BSofCorrmReader((EXT_STR_h101_SOFCORRM*)&ucesb_struct.corrm, offsetof(EXT_STR_h101,corrm));

		if(fCorrv)
			unpackcorrv = new R3BSofCorrvReader((EXT_STR_h101_SOFCORRV*)&ucesb_struct.corrv, offsetof(EXT_STR_h101,corrv));


    // Add readers ------------------------------------------
    source->AddReader(new R3BUnpackReader(&ucesb_struct.unpack,offsetof(EXT_STR_h101, unpack)));
    source->AddReader(new R3BTrloiiTpatReader(&ucesb_struct.unpacktpat,offsetof(EXT_STR_h101, unpacktpat)));
	  
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

    if(fFrsSci) {
        unpackWRS2->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackWRS2);
        unpackWRS8->SetOnline(NOTstoremappeddata);
        source->AddReader(unpackWRS8);
    }

    if(fAt){
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

		if(fCorrm)
		{
			unpackcorrm->SetOnline(NOTstoremappeddata);
			source->AddReader(unpackcorrm);
		}
		if(fCorrv)
		{
			unpackcorrv->SetOnline(NOTstoremappeddata);
			source->AddReader(unpackcorrv);
		}

    // Create online run ------------------------------------
    FairRunOnline* run = new FairRunOnline(source);
    run->SetRunId(fRunId);
    run->SetSink(new FairRootFileSink(outputFilename));
    run->ActivateHttpServer(refresh, port);
    run->GetHttpServer()->CreateEngine(TString::Format("fastcgi:%d",
	1000 + port));

    // Runtime data base ------------------------------------
    FairRuntimeDb* rtdb = run->GetRuntimeDb();

    FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo(); // Ascii
    if (!fCalifa)
    {
        parIo1->open(sofiacalfilename, "in");
        rtdb->setFirstInput(parIo1);
        rtdb->print();
    }
    else
    {
        if (!fCal_level_califa)
        { // SOFIA and CALIFA mapping: Ascii files
            TList* parList1 = new TList();
            parList1->Add(new TObjString(sofiacalfilename));
            parList1->Add(new TObjString(califamapfilename));
            parIo1->open(parList1);
            rtdb->setFirstInput(parIo1);
            rtdb->print();
        }
        else
        { // SOFIA, CALIFA mapping and CALIFA calibration parameters
           // parIo1->open(sofiacalfilename, "in"); // Ascii file
           //rtdb->setFirstInput(parIo1);
           // rtdb->print();
           // Bool_t kParameterMerged = kFALSE;
           // FairParRootFileIo* parIo2 = new FairParRootFileIo(kParameterMerged); // Root file
           // TList* parList2 = new TList();
           // parList2->Add(new TObjString(califacalfilename));
           // parIo2->open(parList2);
           // rtdb->setSecondInput(parIo2);
	   //
            TList* parList1 = new TList();
            parList1->Add(new TObjString(sofiacalfilename));
	    parList1->Add(new TObjString(califacalfilename));
	    parIo1->open(parList1);
	    rtdb->setFirstInput(parIo1);
	    rtdb->print();
        }
    }

    // Add analysis task ------------------------------------
    // TPCs at S2
    if (fFrsTpcs)
    {
      R3BTpcMapped2Cal* TpcMap2Cal = new R3BTpcMapped2Cal();
      TpcMap2Cal->SetOnline(NOTstorecaldata);
      run->AddTask(TpcMap2Cal);
      R3BTpcCal2Hit* TpcCal2Hit = new R3BTpcCal2Hit();
      TpcCal2Hit->SetOnline(NOTstorehitdata);
      run->AddTask(TpcCal2Hit);
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
        
	// --- SingleTcal 2 Cal for SofSci
        R3BSofSciSingleTcal2Cal* SofSciSTcal2Cal = new R3BSofSciSingleTcal2Cal();
        SofSciSTcal2Cal->SetOnline(NOTstorecaldata);
        run->AddTask(SofSciSTcal2Cal);
	
	// --- SingleTcal 2 Hit for SofSci
        R3BSofSciSingleTcal2Hit* SofSciSTcal2Hit = new R3BSofSciSingleTcal2Hit();
        SofSciSTcal2Hit->SetOnline(NOTstorehitdata);
        SofSciSTcal2Hit->SetCalParams(675.,-1922.);//ToF calibration at Cave-C
        run->AddTask(SofSciSTcal2Hit);
    }

    // Triple-MUSIC
    if(fTrim)
    {   
        // --- Mapped 2 Cal
        R3BSofTrimMapped2Cal* SofTrimMap2Cal = new R3BSofTrimMapped2Cal();
        SofTrimMap2Cal->SetOnline(NOTstorecaldata);
        run->AddTask(SofTrimMap2Cal);
   
	// --- Cal 2 Hit
	R3BSofTrimCal2Hit* SofTrimCal2Hit = new R3BSofTrimCal2Hit();
	SofTrimCal2Hit->SetOnline(NOTstorehitdata);	
	SofTrimCal2Hit->SetTriShape(kTRUE);
	run->AddTask(SofTrimCal2Hit);
    }

    // FRS
    if (fMwpc0 && fSci && fMusic && fFrs)
    {
        R3BSofFrsAnalysis* FrsAna = new R3BSofFrsAnalysis();
        FrsAna->SetOnline(NOTstorehitdata);
        run->AddTask(FrsAna);
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
        CalifaCal2Hit->SetCrystalThreshold(100.); // 100keV
        CalifaCal2Hit->SetDRThreshold(10000.);    // 10MeV
        CalifaCal2Hit->SetOnline(NOTstorehitdata);
        run->AddTask(CalifaCal2Hit);
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
        R3BSofTofWSingleTCal2Hit* SofTofWSingleTcal2Hit = new R3BSofTofWSingleTCal2Hit();
        SofTofWSingleTcal2Hit->SetOnline(NOTstorehitdata);
        SofTofWSingleTcal2Hit->SetExpId(expId);
        SofTofWSingleTcal2Hit->SetTofLISE(33.);
        run->AddTask(SofTofWSingleTcal2Hit);
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


    // TWIM
    if (fTwim)
    {
        R3BSofTwimMapped2Cal* TwimMap2Cal = new R3BSofTwimMapped2Cal();
        TwimMap2Cal->SetOnline(NOTstorecaldata);
        TwimMap2Cal->SetExpId(expId);
        run->AddTask(TwimMap2Cal);

        R3BSofTwimCal2Hit* TwimCal2Hit = new R3BSofTwimCal2Hit();
        TwimCal2Hit->SetOnline(NOTstorehitdata);
        run->AddTask(TwimCal2Hit);
    }

    // Add online task ------------------------------------
    if (fFrsTpcs)
    {
       FrsTpcOnlineSpectra* tpconline= new FrsTpcOnlineSpectra();
       run->AddTask(tpconline);
    }

    if (fScalers)
    {
        R3BSofScalersOnlineSpectra* scalersonline = new R3BSofScalersOnlineSpectra();
        run->AddTask(scalersonline);
    }
    if (fFrs && fMusic && fSci)
    {
        R3BSofFrsOnlineSpectra* frsonline = new R3BSofFrsOnlineSpectra();
        run->AddTask(frsonline);
    }
    if (fMwpc0)
    {
        R3BSofMwpcOnlineSpectra* mw0online = new R3BSofMwpcOnlineSpectra("SofMwpc0OnlineSpectra", 1, "Mwpc0");
        run->AddTask(mw0online);
    }

    if (fMusic)
    {
        R3BMusicOnlineSpectra* musonline = new R3BMusicOnlineSpectra();
        run->AddTask(musonline);
        if (fMwpc0)
        {
          R3BSofMwpcvsMusicOnlineSpectra* mw0vsmusiconline= new R3BSofMwpcvsMusicOnlineSpectra("SofMwpc0vsMusicOnlineSpectra", 1, "Mwpc0");
          run->AddTask(mw0vsmusiconline);
        }
    }

    if (fTrim)
    {
        R3BSofTrimOnlineSpectra* trimonline = new R3BSofTrimOnlineSpectra();
        run->AddTask(trimonline);
    }

    if (fSci)
    {
       R3BSofSciOnlineSpectra* scionline = new R3BSofSciOnlineSpectra();
	     scionline->SetNbDetectors(NumSofSci);
	     scionline->SetNbChannels(3);
	     scionline->SetIdS2(IdS2);
	     scionline->SetIdS8(IdS8);
             scionline->SetCalTofS2min(535,0);
	     scionline->SetCalTofS2max(550,0);
       run->AddTask(scionline);
	if(fTrim)
	{
	  R3BSofSciVsTrimOnlineSpectra* scivstrimonline = new R3BSofSciVsTrimOnlineSpectra();
	  scivstrimonline->SetNbDetectors(NumSofSci);
	  scivstrimonline->SetEmin(0,22000);
	  scivstrimonline->SetEmax(0,22400);
	  scivstrimonline->SetEmin(1,22000);
	  scivstrimonline->SetEmax(1,22400);
	  scivstrimonline->SetEmin(2,22000);
	  scivstrimonline->SetEmax(2,22400);
	  scivstrimonline->SetEmin(3,22000);
	  scivstrimonline->SetEmax(3,22400);
	  scivstrimonline->SetDispersionS2(7250.);
	  scivstrimonline->SetBrho0(11.0955);   // 199At
	  //scivstrimonline->SetBrho0(11.5067);   // 221Pa
	  //scivstrimonline->SetBrho0(12.3352);   // 238U calibration point with plastic#2 at S2
	  //scivstrimonline->SetBrho0(11.0864);   // 204Fr
	  //scivstrimonline->SetBrho0(10.6814);   // 175Pt
	  //scivstrimonline->SetBrho0(10.9558);   // 184Hg
	  //scivstrimonline->SetBrho0(10.8138);   // 187Pb
	  //scivstrimonline->SetBrho0(10.8183);   // 182Hg
	  //scivstrimonline->SetBrho0(10.9476);   // 189Pb
	  //scivstrimonline->SetBrho0(10.6814);   // 180Hg
	  //scivstrimonline->SetBrho0(11.2712); // 207Fr
	  //scivstrimonline->SetBrho0(11.3418); // 226,216Th
	  //scivstrimonline->SetBrho0(12.3354); //ATTENTION PARAMETERS CALCULATED WITH (12.3255) TO BE DONE AGAIN WITH THE GOOD VALUE; // FRS S2 to Cave C ToF calibration point 3 with plastic#1 at S2
	  //scivstrimonline->SetBrho0(12.8719); // FRS S2 to Cave C ToF calibration point 2 with plastic#1 at S2
	  //scivstrimonline->SetBrho0(14.1008); // FRS S2 to Cave C ToF calibration point 1 with plastic#1 at S2
	  //scivstrimonline->SetBrho0(9.3148);  // run 204, 205
	  //scivstrimonline->SetBrho0(14.1007);
	  run->AddTask(scivstrimonline);
	}
	if(fMwpc0)
	{
	  R3BSofSciVsMwpc0OnlineSpectra* scivsmw0online = new R3BSofSciVsMwpc0OnlineSpectra();
	  scivsmw0online->SetNbDetectors(NumSofSci);
	  run->AddTask(scivsmw0online);
	}
    }

    if (fAt)
    {
        R3BSofAtOnlineSpectra* atonline = new R3BSofAtOnlineSpectra();
        TCutG* cut1 = new TCutG();
	cut1->SetName("cut_section1");
	cut1->SetFillColorAlpha(0,0);
	cut1->SetPoint(0,1000,13000);
	cut1->SetPoint(1,9000,13000);
	cut1->SetPoint(2,9000,22000);
	cut1->SetPoint(3,1000,22000);
	cut1->SetPoint(4,1000,13000);
	atonline->SetSelection(1,cut1);

	TCutG* cut2 = new TCutG();
	cut2->SetName("cut_section2");
	cut2->SetFillColorAlpha(0,0);
	cut2->SetPoint(0,40000,1000);
	cut2->SetPoint(1,53000,1000);
	cut2->SetPoint(2,53000,22000);
	cut2->SetPoint(3,43000,22000);
	cut2->SetPoint(4,40000,1000);
	atonline->SetSelection(2,cut2);
	
	TCutG* cut3 = new TCutG();
	cut3->SetFillColorAlpha(0,0);
	cut3->SetName("cut_section3");
	cut3->SetPoint(0,26000,42000);
	cut3->SetPoint(1,36000,42000);
	cut3->SetPoint(2,36000,54000);
	cut3->SetPoint(3,26000,54000);
	cut3->SetPoint(4,26000,42000);
	atonline->SetSelection(3,cut3);
	
	run->AddTask(atonline);
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

    if (fMusic && fCalifa && fTwim)
    {
        R3BAmsCorrelationOnlineSpectra* CalifaAmsOnline = new R3BAmsCorrelationOnlineSpectra();
        CalifaAmsOnline->SetZproj(82.0); // Projectile atomic number
        CalifaAmsOnline->SetCalifa_bins_maxrange(500, 300000); // 300000 -> 300MeV
        run->AddTask(CalifaAmsOnline);
    }

    if (fTwim)
    {
        R3BSofTwimOnlineSpectra* twonline = new R3BSofTwimOnlineSpectra();
        twonline->SetExpId(expId);
        run->AddTask(twonline);
        // Twim-Music correlations
        if (fMusic)
        {
            R3BSofTwimvsMusicOnlineSpectra* twmusonline = new R3BSofTwimvsMusicOnlineSpectra();
            run->AddTask(twmusonline);
        }
        if (fTrim)
        {
            R3BSofTwimvsTrimOnlineSpectra* twtronline = new R3BSofTwimvsTrimOnlineSpectra();
            run->AddTask(twtronline);        
        }
        
    }

    if (fMwpc1)
    {
        R3BSofMwpcOnlineSpectra* mw1online = new R3BSofMwpcOnlineSpectra("SofMwpc1OnlineSpectra", 1, "Mwpc1");
        run->AddTask(mw1online);
    }

    if (fMwpc0 && fMwpc1)
    {
        R3BSofMwpcCorrelationOnlineSpectra* mw0mw1online =
            new R3BSofMwpcCorrelationOnlineSpectra("SofMwpc0_1CorrelationOnlineSpectra", 1, "Mwpc0", "Mwpc1");
        run->AddTask(mw0mw1online);
    }

    if (fMwpc1 && fMwpc2)
    {
        R3BSofMwpcCorrelationOnlineSpectra* mw1mw2online =
            new R3BSofMwpcCorrelationOnlineSpectra("SofMwpc1_2CorrelationOnlineSpectra", 1, "Mwpc1", "Mwpc2");
        run->AddTask(mw1mw2online);
    }

    if (fMwpc2)
    {
        R3BSofMwpcOnlineSpectra* mw2online = new R3BSofMwpcOnlineSpectra("SofMwpc2OnlineSpectra", 1, "Mwpc2");
        run->AddTask(mw2online);
    }

    if (fMwpc0 && fMwpc2)
    {
        R3BSofMwpcCorrelationOnlineSpectra* mw0mw2online =
            new R3BSofMwpcCorrelationOnlineSpectra("SofMwpc0_2CorrelationOnlineSpectra", 1, "Mwpc0", "Mwpc2");
        run->AddTask(mw0mw2online);
    }

    if (fMwpc0 && fMwpc3)
    {
        R3BSofMwpcCorrelationOnlineSpectra* mw0mw3online =
            new R3BSofMwpcCorrelationOnlineSpectra("SofMwpc0_3CorrelationOnlineSpectra", 1, "Mwpc0", "Mwpc3");
        run->AddTask(mw0mw3online);
    }
   
    if (fMwpc1 && fMwpc3)
    {
        R3BSofMwpcCorrelationOnlineSpectra* mw1mw3online =
            new R3BSofMwpcCorrelationOnlineSpectra("SofMwpc1_3CorrelationOnlineSpectra", 1, "Mwpc1", "Mwpc3");
        run->AddTask(mw1mw3online);
    }

    if (fMwpc2 && fMwpc3)
    {
        R3BSofMwpcCorrelationOnlineSpectra* mw2mw3online =
            new R3BSofMwpcCorrelationOnlineSpectra("SofMwpc2_3CorrelationOnlineSpectra", 1, "Mwpc2", "Mwpc3");
        run->AddTask(mw2mw3online);
    }

    if (fMwpc3)
    {
        R3BSofMwpcOnlineSpectra* mw3online = new R3BSofMwpcOnlineSpectra("SofMwpc3OnlineSpectra", 1, "Mwpc3");
        run->AddTask(mw3online);
    }

    if (fTofW)
    {
      R3BSofTofWOnlineSpectra* tofwonline = new R3BSofTofWOnlineSpectra();
      tofwonline->Set_TwimvsTof_range(-97.,-65.);
      tofwonline->Set_IdSofSciCaveC(NumSofSci);
      run->AddTask(tofwonline);
    }

    if (fMwpc2 && fMwpc3 && fTofW && fTwim && fSci && fTracking)
    {
        R3BSofFissionAnalysis* TrackingAna = new R3BSofFissionAnalysis();
        TrackingAna->SetOnline(NOTstorehitdata);
        run->AddTask(TrackingAna); 

        R3BSofTrackingFissionOnlineSpectra* Trackingonline = new R3BSofTrackingFissionOnlineSpectra();
        Trackingonline->Set_Charge_range(10.,94.);
        run->AddTask(Trackingonline); 
    }

		if (fCorrm && fCorrv)
		{
			R3BSofCorrOnlineSpectra* corronline = new R3BSofCorrOnlineSpectra();
			corronline->SetFirstXCorrv(125);
			corronline->SetLastXCorrv(919);
			corronline->SetNsPerBin_Corrv();
			corronline->SetTrefId_Corrv(1);
			run->AddTask(corronline);
		}


    R3BSofOnlineSpectra* sofonline = new R3BSofOnlineSpectra();
    run->AddTask(sofonline);

    // Initialize -------------------------------------------
    run->Init();
    FairLogger::GetLogger()->SetLogScreenLevel("INFO");
    //FairLogger::GetLogger()->SetLogScreenLevel("DEBUG1");

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
    //gApplication->Terminate();    
}

