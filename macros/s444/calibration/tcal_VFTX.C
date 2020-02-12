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

  /* Create source using ucesb for input ------------------ */
  
  // Create input -----------------------------------------
  //TString filename = "--stream=lxir123:7803";
  TString filename = "/lustre/land/202002_s444/lustre/r3b/202002_s444/main0007_0001.lmd";
  //TString filename = "~/lmd/califa2020/data_0312.lmd";
    
  // Output file ------------------------------------------
  TString outputFileName = "tcaldata.root";

  // UCESB configuration ----------------------------------
  TString ntuple_options = "RAW";
  TString ucesb_dir = getenv("UCESB_DIR");
  TString upexps_dir = "/u/land/fake_cvmfs/upexps";
  TString ucesb_path;
  //ucesb_path = "/u/land/lynx.landexp/202002_s444/upexps/202002_s444/202002_s444 --allow-errors --input-buffer=100Mi";
  ucesb_path = upexps_dir + "/202002_s444/202002_s444 --allow-errors --input-buffer=100Mi";
  ucesb_path.ReplaceAll("//","/");
 
  // Definition of reader --------------------------------------------
  EXT_STR_h101 ucesb_struct;
  R3BUcesbSource* source = new R3BUcesbSource(filename, ntuple_options,ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
  source->SetMaxEvents(nev);
  source->AddReader(new R3BUnpackReader((EXT_STR_h101_unpack_t *)&ucesb_struct,offsetof(EXT_STR_h101, unpack)));
  source->AddReader(new R3BSofSciReader((EXT_STR_h101_SOFSCI_t *)&ucesb_struct.sci,offsetof(EXT_STR_h101, sci)));
  source->AddReader(new R3BSofToFWReader((EXT_STR_h101_SOFTOFW_t *)&ucesb_struct.tofw,offsetof(EXT_STR_h101, tofw)));
    
  /* Create online run ------------------------------------ */
  FairRunOnline* run = new FairRunOnline(source);
  run->SetRunId(1);
  run->SetOutputFile(outputFileName);

  R3BSofTcalContFact needToConstructTcalContFact;
  
  /* Runtime data base ------------------------------------ */
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  /* Calibrate SofSci ---------------------------------------- */
  R3BSofSciMapped2TcalPar* sciTcalibrator = new R3BSofSciMapped2TcalPar("R3BSofSciMapped2TcalPar");
  sciTcalibrator->SetNumDetectors(1); // //1 for eng run, 2 when S2
  sciTcalibrator->SetNumChannels(3);
  sciTcalibrator->SetNumSignals(1,3);
  sciTcalibrator->SetMinStatistics(1000000);
  //sciTcalibrator->SetMinStatistics(250000);
  run->AddTask(sciTcalibrator);

  /* Calibrate time-of-flight wall  ---------------------------------------- */
  R3BSofToFWMapped2TcalPar* tofwTcalibrator = new R3BSofToFWMapped2TcalPar("R3BSofToFWMapped2TcalPar");
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
