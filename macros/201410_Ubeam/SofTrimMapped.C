struct EXT_STR_h101_t
{
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_SOFTRIM_MADCVFTX_onion_t trim;
};

void SofTrimMapped(Int_t First=1320)
{
  TString runNumber = Form("%04d", First);
  TStopwatch timer;
  timer.Start();

  const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */
  const Int_t max_events_ucesb = 1000000;
  TString max_events = Form("%d", max_events_ucesb);

  /* Create source using ucesb for input ------------------ */
  TString filename = "/media/audrey/COURGE/SOFIA/ANALYSE/SOFIA2/data/lmd_timestitched/run" + runNumber + ".lmd";
  TString outputFileName = "../SofMacrosOutput/201410_Ubeam/SofTrimMapped_run"+runNumber+".root";
  TString ntuple_options = "UNPACK:EVENTNO,UNPACK:TRIGGER,RAW";
  TString ucesb_dir = getenv("UCESB_DIR");
  TString ucesb_path = ucesb_dir + "/../upexps/sofia2014oct/sofia2014oct"
    " --max-events=" + max_events;
  
  EXT_STR_h101 ucesb_struct;
  R3BUcesbSource* source = new R3BUcesbSource(filename, ntuple_options,
					      ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
  source->SetMaxEvents(nev);
  
  source->AddReader(new R3BUnpackReader((EXT_STR_h101_unpack_t *)&ucesb_struct,offsetof(EXT_STR_h101, unpack)));
  source->AddReader(new R3BSofTrimMadcVftxReader((EXT_STR_h101_SOFTRIM_MADCVFTX_t *)&ucesb_struct.trim,offsetof(EXT_STR_h101, trim)));

  const Int_t refresh = 100;  /* refresh rate for saving */
  
  /* Create online run ------------------------------------ */
#define RUN_ONLINE
#ifdef RUN_ONLINE
  FairRunOnline* run = new FairRunOnline(source);
  run->SetRunId(First);
#ifdef USE_HTTP_SERVER
  run->ActivateHttpServer(refresh);
#endif
#else
  /* Create analysis run ---------------------------------- */
  FairRunAna* run = new FairRunAna();
#endif
  /* Set output file name --------------------------------- */
  run->SetOutputFile(outputFileName.Data());
  /* ------------------------------------------------------ */

  FairRuntimeDb* rtdb1 = run->GetRuntimeDb();

  /* Initialize ------------------------------------------- */
  run->Init();
  
  /* Set log level either to WARNING, DEBUG or ERROR */
  FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
  
  /* Run -------------------------------------------------- */
  run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);

  /* Cleanup ---------------------------------------------- */
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is " << outputFileName << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s"
       << endl << endl;
}

