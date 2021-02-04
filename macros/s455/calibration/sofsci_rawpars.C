typedef struct EXT_STR_h101_t
{
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_SOFSCI_onion_t sci;
}EXT_STR_h101;

void sofsci_rawpars()
{
  TStopwatch timer;
  timer.Start();

  const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */

  // *********************************** //
  // PLEASE COMPLETE THE FOLLOWING LINES //
  // *********************************** //
  const Int_t expId = 455; // select experiment: 444 or 467
  // *********************************** //

  // SofSci detector config, lmd input file -------------------------------
  TString ntuple_options = "RAW";
  TString ucesb_dir = getenv("UCESB_DIR");
  UShort_t NumSofSci, IdCaveC, IdS2, IdS8;
  TString filename, upexps_dir, ucesb_path;
  if (expId==444){
    NumSofSci = 1; // s444: PRIMARY BEAM EXP, 1 SofSci at CAVE C ONLY
    IdS2 = 0;
    IdS8 = 0;

    //filename = "--stream=lxir123:7803";
    filename = "/lustre/land/202002_s444/lustre/r3b/202002_s444/main004*_0001.lmd";

    upexps_dir = ucesb_dir + "/../upexps/";                      // for local computers
    // upexps_dir = "/u/land/fake_cvmfs/upexps";                 // for lxlandana computers
    // upexps_dir = "/u/land/lynx.landexp/202002_s444/upexps/";  // for lxg computers
    ucesb_path = upexps_dir + "/202002_s444/202002_s444 --allow-errors --input-buffer=100Mi";
  }
  else if(expId==467){
    NumSofSci = 4; // s467: SECONDARY BEAM EXP, 2 at S2, 1 at S8, 1 at CAVE C
    IdS2 = 2;
    IdS8 = 3;

    //filename = "--stream=lxir123:7803";
    filename = "/lustre/land/202002_s467/lustre/r3b/202002_s467/main0341_0001.lmd";

    upexps_dir = ucesb_dir + "/../upexps/";                      // for local computers
    // upexps_dir = "/u/land/fake_cvmfs/upexps";                 // for lxlandana computers
    // upexps_dir = "/u/land/lynx.landexp/202002_s467/upexps/";  // for lxg computers
    ucesb_path = upexps_dir + "/202002_s467/202002_s467 --allow-errors --input-buffer=100Mi";
  }
  else if (expId==455){
    NumSofSci = 2;

    //filename = "--stream=lxir123:7803";
    filename = "/lustre/land/202002_s467/stitched/main0338_0001.lmd";

    upexps_dir = ucesb_dir + "/../upexps/";                      // for local computers
    // upexps_dir = "/u/land/fake_cvmfs/upexps";                 // for lxlandana computers
    // upexps_dir = "/u/land/lynx.landexp/202002_s467/upexps/";  // for lxg computers
    ucesb_path = upexps_dir + "/202005_s455/202005_s455 --allow-errors --input-buffer=100Mi";
  }
  else{
    std::cout << "Experiment was not selected" << std::endl;
    gApplication->Terminate();
  }
  IdCaveC = NumSofSci; // cave C is the last SofSci detector
  ucesb_path.ReplaceAll("//","/");

  // Output file ----------------------------------------------------
  TString outputFileName = "rawsofsci_PosTof_data.root";

  // Definition of reader --------------------------------------------
  EXT_STR_h101 ucesb_struct;
  R3BUcesbSource* source = new R3BUcesbSource(filename, ntuple_options,ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
  source->AddReader(new R3BUnpackReader((EXT_STR_h101_unpack_t *)&ucesb_struct,offsetof(EXT_STR_h101, unpack)));
  source->AddReader(new R3BSofSciReader((EXT_STR_h101_SOFSCI_t *)&ucesb_struct.sci,offsetof(EXT_STR_h101, sci),NumSofSci));
    
  // --- ----------------- --- //
  // --- Create online run --- //
  // --- ----------------- --- //
  FairRunOnline* run = new FairRunOnline(source);
  run->SetRunId(1);
  run->SetOutputFile(outputFileName);

  // --- ----------------- --- //
  // --- Runtime data base --- //
  // --- ----------------- --- //
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
 
  // --- ------------------------------------------- --- //
  // --- Input parameters :                          --- //
  // ---  ascii file with the calibration parameters --- //
  // --- ------------------------------------------- --- //
  FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();//Ascii
  parIo1->open("../parameters/CalibParam.par","in");
  rtdb->setFirstInput(parIo1);
  rtdb->print();

  // --- ----------------- --- //
  // --- Add analysis task --- //
  // --- ----------------- --- //

  // === Mapped 2 Tcal for SofSci === //
  R3BSofSciMapped2Tcal* SofSciMap2Tcal = new R3BSofSciMapped2Tcal();
  run->AddTask(SofSciMap2Tcal);

  // === Tcal2RawPos for SofSci === //
  R3BSofSciTcal2RawPosPar* sci_poscalibrator = new R3BSofSciTcal2RawPosPar("R3BSofSciTcal2RawPosPar");
  sci_poscalibrator->SetNumDets(NumSofSci); 
  sci_poscalibrator->SetNumPmts(3); // number of Pmts (2) + reference signal (1)
  sci_poscalibrator->SetNumSignals();
  sci_poscalibrator->SetNumParsPerSignal(2);
  sci_poscalibrator->SetMinStatistics(10000);
  run->AddTask(sci_poscalibrator);

  // === Tcal2RawTof for SofSci === //
  R3BSofSciTcal2RawTofPar* sci_tofcalibrator = new R3BSofSciTcal2RawTofPar("R3BSofSciTcal2RawTofPar");
  sci_tofcalibrator->SetNumDets(NumSofSci); 
  sci_tofcalibrator->SetNumChannels(3); // number of Pmts (2) + reference signal (1)
  sci_tofcalibrator->SetDetIdS2(IdS2);
  sci_tofcalibrator->SetDetIdS8(IdS8);
  sci_tofcalibrator->SetDetIdCaveC(IdCaveC);
  sci_tofcalibrator->SetNumSignals();
  sci_tofcalibrator->SetNumParsPerSignal(2);
  sci_tofcalibrator->SetMinStatistics(1000);
  run->AddTask(sci_tofcalibrator);

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
  TString outputFileNamePar = "out_sofsci.par";
  parOut->open(outputFileNamePar,"out");
  rtdb->setOutput(parOut);
  rtdb->print();

  // --- --- --- //
  // --- Run --- //
  // --- --- --- //
  run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);
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
