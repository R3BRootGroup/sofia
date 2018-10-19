#define AT_MAPPED     1
#define COMREF_MAPPED 1
#define SCI_MAPPED    1
#define TOFW_MAPPED   1
#define TRIM_MAPPED   1
#define TWIM_MAPPED   1

struct EXT_STR_h101_t
{
  EXT_STR_h101_unpack_t unpack;
#if AT_MAPPED
  EXT_STR_h101_SOFAT_MADC_onion_t at;
#endif
#if COMREF_MAPPED
  EXT_STR_h101_SOFCOMREF_onion_t comref;
#endif
#if SCI_MAPPED
  EXT_STR_h101_SOFSCI_onion_t sci;
#endif
#if TOFW_MAPPED
  EXT_STR_h101_SOFTOFW_onion_t tofw;
#endif
#if TRIM_MAPPED
  EXT_STR_h101_SOFTRIM_MADCVFTX_onion_t trim;
#endif
#if TWIM_MAPPED
  EXT_STR_h101_SOFTWIM_MADCVFTX_onion_t twim;
#endif

};

void analysis01_unpack(Int_t FirstRun=1365)
{
  TString runNumber = Form("%04d", FirstRun);
  TStopwatch timer;
  timer.Start();

  const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */
  const Int_t max_events_ucesb = 1000000;
  TString max_events = Form("%d", max_events_ucesb);

  // --- ---------------------- --- //
  // --- input and output names --- //
  // --- ---------------------- --- //
  //TString filename = "--stream=lxg0898:6002";
  TString filename;
  TString outputFileName;
  if(FirstRun==1331){
    filename = "/media/audrey/COURGE/SOFIA/ANALYSE/SOFIA2/data/lmd_timestitched/Fr207/*.lmd";
    outputFileName = "../SofMacrosOutput/201410_Ubeam/unpack_Fr207.root";
  }
  else if (FirstRun==1355){
    filename = "/media/audrey/COURGE/SOFIA/ANALYSE/SOFIA2/data/lmd_timestitched/Bi194/*.lmd";
    outputFileName = "../SofMacrosOutput/201410_Ubeam/unpack_Bi194.root";
  }
  else if(FirstRun==1365){
    filename = "/media/audrey/COURGE/SOFIA/ANALYSE/SOFIA2/data/lmd_timestitched/Tl187/*.lmd";
    outputFileName = "../SofMacrosOutput/201410_Ubeam/unpack_Tl187.root";
  }
  else{
    filename = "/media/audrey/COURGE/SOFIA/ANALYSE/SOFIA2/data/lmd_timestitched/"+runNumber+".lmd";
    outputFileName = "../SofMacrosOutput/201410_Ubeam/unpack_run"+runNumber+".root";
  }
  
  // --- ----------------------------------- --- //
  // --- Create source using ucesb for input --- //
  // --- ----------------------------------- --- //
  TString ntuple_options = "UNPACK:EVENTNO,UNPACK:TRIGGER,RAW";
  TString ucesb_dir = getenv("UCESB_DIR");
  TString ucesb_path = ucesb_dir + "/../upexps/sofia2014oct/sofia2014oct --max-events=" + max_events;
  EXT_STR_h101 ucesb_struct;
  R3BUcesbSource* source = new R3BUcesbSource(filename, ntuple_options,ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
  source->SetMaxEvents(nev);


  // --- -------------------------- --- //
  // --- define and add the readers --- //  
  // --- -------------------------- --- //
  source->AddReader(new R3BUnpackReader((EXT_STR_h101_unpack_t *)&ucesb_struct,offsetof(EXT_STR_h101, unpack)));
#if AT_MAPPED
  source->AddReader(new R3BSofATMadcReader((EXT_STR_h101_SOFAT_MADC_t *)&ucesb_struct.at,offsetof(EXT_STR_h101, at)));
#endif
#if COMREF_MAPPED
  source->AddReader(new R3BSofComRefReader((EXT_STR_h101_SOFCOMREF_t *)&ucesb_struct.comref,offsetof(EXT_STR_h101, comref)));
#endif
#if SCI_MAPPED
  source->AddReader(new R3BSofSciReader((EXT_STR_h101_SOFSCI_t *)&ucesb_struct.sci,offsetof(EXT_STR_h101, sci)));
#endif
#if TOFW_MAPPED
  source->AddReader(new R3BSofToFWReader((EXT_STR_h101_SOFTOFW_t *)&ucesb_struct.tofw,offsetof(EXT_STR_h101, tofw)));
#endif
#if TRIM_MAPPED
  source->AddReader(new R3BSofTrimMadcVftxReader((EXT_STR_h101_SOFTRIM_MADCVFTX_t *)&ucesb_struct.trim,offsetof(EXT_STR_h101, trim)));
#endif
#if TWIM_MAPPED
  source->AddReader(new R3BSofTwimMadcVftxReader((EXT_STR_h101_SOFTWIM_MADCVFTX_t *)&ucesb_struct.twim,offsetof(EXT_STR_h101, twim)));
#endif

  
  // --- ----------------- --- //
  // --- Create online run --- //
  // --- ----------------- --- //
  FairRunOnline* run = new FairRunOnline(source);
  run->SetRunId(FirstRun);
  run->SetOutputFile(outputFileName.Data());
#ifdef USE_HTTP_SERVER
  const Int_t refresh = 100; 
  run->ActivateHttpServer(refresh);
#endif

  // --- ------------------- --- //
  // --- Create analysis run --- //
  // --- ------------------- --- //
  //FairRunAna* run = new FairRunAna();


  // --- ----------------- --- //
  // --- Runtime data base --- //
  // --- ----------------- --- //
  FairRuntimeDb* rtdb1 = run->GetRuntimeDb();

  // --- ---------- --- //
  // --- Initialize --- //
  // --- ---------- --- //
  run->Init();
  FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
  
  // --- --- --- //
  // --- Run --- //
  // --- --- --- //
  run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);
  
  // --- ------------------- --- //
  // --- Cleanup and finnish --- //
  // --- ------------------- --- //
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is " << outputFileName << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s"
       << endl << endl;
}

