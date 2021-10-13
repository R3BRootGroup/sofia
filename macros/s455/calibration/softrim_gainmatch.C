typedef struct EXT_STR_h101_t
{
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_SOFTRIM_onion_t trim;
}EXT_STR_h101;

void softrim_gainmatch()
{
  TStopwatch timer;
  timer.Start();

  const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */

  // SofSci detector config, lmd input file -------------------------------
  TString ntuple_options = "RAW";
  TString ucesb_dir = getenv("UCESB_DIR");
  UShort_t NumSofSci ;
  TString filename, upexps_dir, ucesb_path;

  filename = "~/data/s455/stitched/EvsBeta/main0202*.lmd";

  upexps_dir = ucesb_dir + "/../upexps/";                      // for local computers
  // upexps_dir = "/u/land/fake_cvmfs/upexps";                 // for lxlandana computers
  ucesb_path = upexps_dir + "/202103_s455/202103_s455_part2 --allow-errors --input-buffer=100Mi";
  ucesb_path.ReplaceAll("//","/");

  // Output file ----------------------------------------------------
  TString outputFileName = "calsoftrim_gainmatch_data.root";

  // Definition of reader --------------------------------------------
  EXT_STR_h101 ucesb_struct;
  R3BUcesbSource* source = new R3BUcesbSource(filename, ntuple_options,ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
  source->AddReader(new R3BUnpackReader((EXT_STR_h101_unpack_t *)&ucesb_struct,offsetof(EXT_STR_h101, unpack)));
  source->AddReader(new R3BSofTrimReader((EXT_STR_h101_SOFTRIM_t*)&ucesb_struct.trim, offsetof(EXT_STR_h101, trim)));

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
  
  // === Mapped 2 Cal Trim === //
  R3BSofTrimMapped2Cal* TrimMap2Cal = new R3BSofTrimMapped2Cal();
  run->AddTask(TrimMap2Cal);

  // === Calculate Gain Matiching Parameters with primary beam === //
  R3BSofTrimCalculateMatchGainPar* calcmgain = new R3BSofTrimCalculateMatchGainPar("R3BSofTrimCalculateMatchGainPar");
  calcmgain->SetEpsilon(0.02);
  // section 1
  calcmgain->SetGainMin(0.9,0); 
  calcmgain->SetGainMin(0.9,1);
  calcmgain->SetGainMin(0.84,2);
  // section 2
  calcmgain->SetGainMin(0.9,3);
  calcmgain->SetGainMin(0.9,4);
  calcmgain->SetGainMin(0.82,5);
  // section 3
  calcmgain->SetGainMin(0.92,6);
  calcmgain->SetGainMin(0.9,7);
  calcmgain->SetGainMin(0.9,8);
  
  run->AddTask(calcmgain);

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
  TString outputFileNamePar = "out_softrim_matchgain.par";
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
