typedef struct EXT_STR_h101_t
{
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_SOFTRIM_onion_t trim;
  EXT_STR_h101_SOFMWPC_onion_t mwpc;
}EXT_STR_h101;

void softrim_dtoffset()
{
  TStopwatch timer;
  timer.Start();

  const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */

  // SofSci detector config, lmd input file -------------------------------
  TString ntuple_options = "RAW";
  TString ucesb_dir = getenv("UCESB_DIR");
  UShort_t NumSofSci ;
  TString filename, upexps_dir, ucesb_path;

  filename = "~/data/s455/stitched/EvsBeta/main0197_0001.lmd";

  upexps_dir = ucesb_dir + "/../upexps/";                      // for local computers
  ucesb_path = upexps_dir + "/202103_s455/202103_s455_part2 --allow-errors --input-buffer=100Mi";
  ucesb_path.ReplaceAll("//","/");

  // Output file ----------------------------------------------------
  TString outputFileName = "calsoftrim_dtoffset_data.root";

  // Definition of reader --------------------------------------------
  EXT_STR_h101 ucesb_struct;
  R3BUcesbSource* source = new R3BUcesbSource(filename, ntuple_options,ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
  source->AddReader(new R3BUnpackReader((EXT_STR_h101_unpack_t *)&ucesb_struct,offsetof(EXT_STR_h101, unpack)));
  source->AddReader(new R3BSofMwpcReader((EXT_STR_h101_SOFMWPC_t*)&ucesb_struct.mwpc, offsetof(EXT_STR_h101, mwpc)));
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
  
  // === Mapped 2 Cal Mwpc0 === //
  R3BSofMwpc0Mapped2Cal* Mw0Map2Cal = new R3BSofMwpc0Mapped2Cal();
  run->AddTask(Mw0Map2Cal);

  // === Cal 2 Hit Mwpc0 === //
  R3BSofMwpc0Cal2Hit* Mw0Cal2Hit = new R3BSofMwpc0Cal2Hit();
  run->AddTask(Mw0Cal2Hit);
  
  // === Mapped 2 Cal Mwpc0 === //
  R3BSofMwpc1Mapped2Cal* Mw1Map2Cal = new R3BSofMwpc1Mapped2Cal();
  run->AddTask(Mw1Map2Cal);

  // === Cal 2 Hit Mwpc0 === //
  R3BSofMwpc1Cal2Hit* Mw1Cal2Hit = new R3BSofMwpc1Cal2Hit();
  run->AddTask(Mw1Cal2Hit);
  
  // === Mapped 2 Cal Trim === //
  R3BSofTrimMapped2Cal* TrimMap2Cal = new R3BSofTrimMapped2Cal();
  run->AddTask(TrimMap2Cal);

  // === Calculate Drift Time Offset Parameters === //
  R3BSofTrimCalculateDriftTimeOffsetPar* calcdtoff = new R3BSofTrimCalculateDriftTimeOffsetPar("R3BSofTrimCalculateDriftTimeOffsetPar");
  calcdtoff->SetDistInterSection(50.);  // mm : to be confirmed by Bernd
  calcdtoff->SetDriftVelocity(45.);     // mm/cm
  run->AddTask(calcdtoff);

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
  TString outputFileNamePar = "out_softrim_dtoffset.par";
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
