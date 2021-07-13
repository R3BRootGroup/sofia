/*
 *  Macro to run the VFTX calibration for the SCI and ToF Wall detectors
 *
 *  One needs to set up the Phase0 experiments: s444, s467, or s455
 *
 *  at $UCESB_DIR/../upexps/yyyymm_s4xx
 *
 *
 *  Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
 *  @since May 27th, 2021
 *
 */

typedef struct EXT_STR_h101_t
{
    EXT_STR_h101_unpack_t unpack;
    EXT_STR_h101_SOFSCI_onion_t sci;
    EXT_STR_h101_SOFTOFW_onion_t tofw;
} EXT_STR_h101;

/* number of events to read, -1 - until CTRL+C */
void finder_tcal_VFTX(const Int_t nev = -1)
{
    TStopwatch timer;
    timer.Start();

    // *********************************** //
    // PLEASE CHANGE THE EXPERIMENT NUMBER //
    // *********************************** //
    const Int_t expId = 455; //
    // *********************************** //

    // Configuration  ------------------------------------
    UShort_t NumSofSci;
    Bool_t NOTstoremappeddata;
    TString ntuple_options = "RAW";
    TString ucesb_dir = getenv("UCESB_DIR");
    TString filename, upexps_dir, ucesb_path;

    if (expId == 455)
    {
        NumSofSci = 1;

        filename = "~/lmd/s455/main0273_0010_stitched.lmd";

        upexps_dir = ucesb_dir + "/../upexps/";
        ucesb_path = upexps_dir + "/202103_s455/202103_s455 --allow-errors --input-buffer=70Mi";
        NOTstoremappeddata = true; // if true, don't store mapped data in the root file
    }
    else
    {
        std::cout << "Experiment was not selected" << std::endl;
        gApplication->Terminate();
    }
    ucesb_path.ReplaceAll("//", "/");

    // Output file ------------------------------------------
    TString outputFileName = "data_tcal.root";

    // UCESB configuration ----------------------------------
    EXT_STR_h101 ucesb_struct;
    
    // Create online run ------------------------------------
    FairRunOnline* run = new FairRunOnline();
    R3BEventHeader* EvntHeader = new R3BEventHeader();
    run->SetEventHeader(EvntHeader);
    run->SetRunId(1);
    run->SetSink(new FairRootFileSink(outputFileName));
    
    // Create source using ucesb for input ------------------
    R3BUcesbSource* source =
        new R3BUcesbSource(filename, ntuple_options, ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
    source->SetMaxEvents(nev);

    // Definition of reader --------------------------------------------
    source->AddReader(new R3BUnpackReader(&ucesb_struct.unpack, offsetof(EXT_STR_h101, unpack)));
    R3BSofSciReader* unpacksci =
        new R3BSofSciReader((EXT_STR_h101_SOFSCI_t*)&ucesb_struct.sci, offsetof(EXT_STR_h101, sci), NumSofSci);
    R3BSofTofWReader* unpacktofw =
        new R3BSofTofWReader((EXT_STR_h101_SOFTOFW_t*)&ucesb_struct.tofw, offsetof(EXT_STR_h101, tofw));
    unpacksci->SetOnline(NOTstoremappeddata);
    source->AddReader(unpacksci);
    unpacktofw->SetOnline(NOTstoremappeddata);
    source->AddReader(unpacktofw);
    run->SetSource(source);

    // Runtime data base ------------------------------------
    FairRuntimeDb* rtdb = run->GetRuntimeDb();

    // Calibrate SofSci ----------------------------------------
    R3BSofSciMapped2TcalPar* sciTcalibrator = new R3BSofSciMapped2TcalPar("R3BSofSciMapped2TcalPar");
    sciTcalibrator->SetNumSci(NumSofSci);
    sciTcalibrator->SetMinStatistics(25000);
    run->AddTask(sciTcalibrator);

    // Calibrate time-of-flight wall  ----------------------------------------
    R3BSofTofWMapped2TcalPar* tofwTcalibrator = new R3BSofTofWMapped2TcalPar("R3BSofTofWMapped2TcalPar");
    tofwTcalibrator->SetNumDetectors(28);
    tofwTcalibrator->SetNumChannels(2);
    tofwTcalibrator->SetMinStatistics(200);
    run->AddTask(tofwTcalibrator);

    // Initialize -------------------------------------------
    run->Init();
    FairLogger::GetLogger()->SetLogScreenLevel("INFO");

    // Ascii file with the Calibration Parameters
    FairParAsciiFileIo* parOut = new FairParAsciiFileIo();
    TString outputFileNamePar = "tcal_VFTX.par";
    parOut->open(outputFileNamePar, "out");
    rtdb->setOutput(parOut);

    // Run --------------------------------------------------
    run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);
    rtdb->saveOutput();

    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    cout << endl << endl;
    cout << "Macro finished succesfully." << endl;
    cout << "Output file is " << outputFileName << endl;
    cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl << endl;
    gApplication->Terminate();
}
