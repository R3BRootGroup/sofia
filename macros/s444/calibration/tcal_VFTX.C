typedef struct EXT_STR_h101_t
{
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_SOFSCI_onion_t sci;
  EXT_STR_h101_SOFTOFW_onion_t tofw;
}EXT_STR_h101;

void tcal_VFTX()
{
  TStopwatch timer;
  timer.Start();

  const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */

  // *********************************** //
  // PLEASE CHANGE THE EXPERIMENT NUMBER //
  // *********************************** //
  const Int_t expId = 444;               // 
  // *********************************** //  
  

  // NumSofSci, lmd input file ------------------------------------
  UShort_t NumSofSci;
  TString ntuple_options = "RAW";
  TString ucesb_dir = getenv("UCESB_DIR");
  TString filename, upexps_dir, ucesb_path;

  if (expId==444){
    NumSofSci = 1; // s444: PRIMARY BEAM EXP, 1 SofSci at CAVE C ONLY

    //filename = "--stream=lxir123:7803";
    filename = "/lustre/land/202002_s444/lustre/r3b/202002_s444/main004*_0001.lmd";

    upexps_dir = ucesb_dir + "/../upexps/";                      // for local computers
    // upexps_dir = "/u/land/fake_cvmfs/upexps";                 // for lxlandana computers
    // upexps_dir = "/u/land/lynx.landexp/202002_s444/upexps/";  // for lxg computers
    ucesb_path = upexps_dir + "/202002_s444/202002_s444 --allow-errors --input-buffer=100Mi";
  }
  else if (expId==467){
    NumSofSci = 4; // s467: SECONDARY BEAM EXP, 2 at S2, 1 at S8, 1 at CAVE C

    //filename = "--stream=lxir123:7803";
    filename = "/lustre/land/202002_s467/lustre/r3b/202002_s444/main0341_0001.lmd";

    upexps_dir = ucesb_dir + "/../upexps/";                      // for local computers
    // upexps_dir = "/u/land/fake_cvmfs/upexps";                 // for lxlandana computers
    // upexps_dir = "/u/land/lynx.landexp/202002_s467/upexps/";  // for lxg computers
    ucesb_path = upexps_dir + "/202002_s467/202002_s467 --allow-errors --input-buffer=100Mi";
  }
  else{
    std::cout << "Experiment was not selected" << std::endl;
    gApplication->Terminate();
  }
  ucesb_path.ReplaceAll("//","/");
  
  // Output file ------------------------------------------
  TString outputFileName = "tcaldata.root";

  // UCESB configuration ----------------------------------
 
  // Definition of reader --------------------------------------------
  EXT_STR_h101 ucesb_struct;
  R3BUcesbSource* source = new R3BUcesbSource(filename, ntuple_options,ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
  source->SetMaxEvents(nev);
  source->AddReader(new R3BUnpackReader((EXT_STR_h101_unpack_t *)&ucesb_struct,offsetof(EXT_STR_h101, unpack)));
  source->AddReader(new R3BSofSciReader((EXT_STR_h101_SOFSCI_t *)&ucesb_struct.sci,offsetof(EXT_STR_h101, sci),NumSofSci));
  source->AddReader(new R3BSofTofWReader((EXT_STR_h101_SOFTOFW_t *)&ucesb_struct.tofw,offsetof(EXT_STR_h101, tofw)));
    
  /* Create online run ------------------------------------ */
  FairRunOnline* run = new FairRunOnline(source);
  run->SetRunId(1);
  run->SetOutputFile(outputFileName);

  R3BSofTcalContFact needToConstructTcalContFact;
  
  /* Runtime data base ------------------------------------ */
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  /* Calibrate SofSci ---------------------------------------- */
  R3BSofSciMapped2TcalPar* sciTcalibrator = new R3BSofSciMapped2TcalPar("R3BSofSciMapped2TcalPar");
  sciTcalibrator->SetNumSci(NumSofSci); 
  sciTcalibrator->SetNumChannels(3);
  sciTcalibrator->SetNumSignals(NumSofSci,3);
  //sciTcalibrator->SetMinStatistics(1000000);
  sciTcalibrator->SetMinStatistics(250000);
  run->AddTask(sciTcalibrator);

  /* Calibrate time-of-flight wall  ---------------------------------------- */
  R3BSofTofWMapped2TcalPar* tofwTcalibrator = new R3BSofTofWMapped2TcalPar("R3BSofTofWMapped2TcalPar");
  tofwTcalibrator->SetNumDetectors(28);
  tofwTcalibrator->SetNumChannels(2);
  tofwTcalibrator->SetNumSignals(28,2);
  tofwTcalibrator->SetMinStatistics(100000);
  run->AddTask(tofwTcalibrator);
  
  /* Initialize ------------------------------------------- */
  run->Init();
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  //FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
  /* ------------------------------------------------------ */
  
  // Ascii file with the Calibration Parameters
  FairParAsciiFileIo* parOut = new FairParAsciiFileIo();
  TString outputFileNamePar = "tcal_VFTX.par";
  parOut->open(outputFileNamePar,"out");
  rtdb->setOutput(parOut);
  
  /* Run -------------------------------------------------- */
  run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);
  rtdb->saveOutput();
  /* ------------------------------------------------------ */

  
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is " << outputFileName << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s"
       << endl << endl;



}
