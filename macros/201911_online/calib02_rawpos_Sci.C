struct EXT_STR_h101_t
{
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_SOFSCI_onion_t sci;
};

void calib02_rawpos_Sci(Int_t First=1320)
{
  TString runNumber = Form("%03d", First);
  TStopwatch timer;
  timer.Start();

  const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */
  const Int_t max_events_ucesb = 1000000; 
  TString max_events = Form("%d",max_events_ucesb);

  // --- ----------------------------------- --- //
  // --- Create source using ucesb for input --- //
  // --- ----------------------------------- --- //
  //TString filename = " --stream=lxir123 ";
  TString filename = "/media/audrey/COURGE/SOFIA/ANALYSE/SOFIA3/data/main0028*.lmd";
  TString outputFileName = "../SofMacrosOutput/201911_online/calib_output_rawpos_Sci.root";
  TString ucesb_dir = getenv("UCESB_DIR");
  TString ucesb_path = ucesb_dir + "/../upexps/201911_eng/201911_eng --allow-errors --input-buffer=100M";
  ucesb_path.ReplaceAll("//","/");
  TString ntuple_options = "RAW";
  TString outputFileNamePar = "../SofMacrosOutput/201911_online/Parameters/tcal2rawpos_Sci";

  EXT_STR_h101 ucesb_struct;
  R3BUcesbSource* source = new R3BUcesbSource(filename, ntuple_options,ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
  //source->SetMaxEvents(nev);
  source->SetMaxEvents(max_events_ucesb);
  source->AddReader(new R3BUnpackReader((EXT_STR_h101_unpack_t *)&ucesb_struct,offsetof(EXT_STR_h101, unpack)));
  source->AddReader(new R3BSofSciReader((EXT_STR_h101_SOFSCI_t *)&ucesb_struct.sci,offsetof(EXT_STR_h101, sci)));
    
  const Int_t refresh = 50000;  /* refresh rate for saving */

  // --- ----------------- --- //
  // --- Create online run --- //
  // --- ----------------- --- //
  FairRunOnline* run = new FairRunOnline(source);

  // --- ------------------- --- //
  // --- Set ouput file name --- //
  // --- ------------------- --- //
  run->SetOutputFile(outputFileName);


  // --- ----------------- --- //
  // --- Add analysis task --- //
  // --- ----------------- --- //

  // === Mapped 2 Tcal for SofSci === //
  R3BSofSciMapped2Tcal* SofSciMap2Tcal = new R3BSofSciMapped2Tcal();
  run->AddTask(SofSciMap2Tcal);

  // === Tcal2RawPos for SofSci === //
  R3BSofSciContFact needToConstructSofSciContFact;
  R3BSofSciTcal2RawPosPar* sci_calibrator = new R3BSofSciTcal2RawPosPar("R3BSofSciTcal2RawPosPar");
  sci_calibrator->SetNumDetectors(1); 
  sci_calibrator->SetNumChannels(3);
  sci_calibrator->SetNumSignals();
  sci_calibrator->SetNumParsPerSignal(2);
  sci_calibrator->SetMinStatistics(10000);
  run->AddTask(sci_calibrator);

  // --- ----------------- --- //
  // --- Runtime data base --- //
  // --- ----------------- --- //
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  // --- ------------------------------------------- --- //
  // --- Input parameters :                          --- //
  // ---  ascii file with the calibration parameters --- //
  // --- ------------------------------------------- --- //
  FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();//Ascii
  parIo1->open("./parameters/CalibParam.par","in");
  rtdb->setFirstInput(parIo1);
  rtdb->print();

  // --- ---------- --- //
  // --- Initialize --- //
  // --- ---------- --- //
  run->Init();
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  //FairLogger::GetLogger()->SetLogScreenLevel("WARNING");

  // --- ------------------------------------------- --- //
  // --- output parameters :                         --- //
  // ---  ascii file with the calibration parameters --- //
  // --- ------------------------------------------- --- //
  FairParAsciiFileIo* parOut = new FairParAsciiFileIo();
  outputFileNamePar = outputFileNamePar+ ".par";
  parOut->open(outputFileNamePar,"out");
  rtdb->setOutput(parOut);
  rtdb->print();

  // --- --- --- //
  // --- Run --- //
  // --- --- --- //
  run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);
  if (rtdb->getCurrentRun()) cout << "have run" << endl;
  else cout << "have no run" << endl;
  rtdb->saveOutput();

  // --- ------- --- //
  // --- Cleanup --- //
  // --- ------- --- //
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is " << outputFileName << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s"
       << endl << endl;







  
  

  



}
