#define AT_MAPPED     1
#define SCI_MAPPED    1
#define TOFW_MAPPED   1

#define SAVE_TCALPAR_ROOT 0

#define SCI_TCAL  1
#define TOFW_TCAL 0

struct EXT_STR_h101_t
{
  EXT_STR_h101_unpack_t unpack;
#if AT_MAPPED
  EXT_STR_h101_SOFAT_MADC_onion_t at;
#endif
#if SCI_MAPPED
  EXT_STR_h101_SOFSCI_onion_t sci;
#endif
#if TOFW_MAPPED
  EXT_STR_h101_SOFTOFW_onion_t tofw;
#endif
};

void analysis02_tcal(Int_t FirstRun=1355)
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
    outputFileName = "../SofMacrosOutput/201410_Ubeam/tcal_Fr207.root";
  }
  else if (FirstRun==1355){
    filename = "/media/audrey/COURGE/SOFIA/ANALYSE/SOFIA2/data/lmd_timestitched/Bi194/*.lmd";
    outputFileName = "../SofMacrosOutput/201410_Ubeam/tcal_Bi194.root";
  }
  else if(FirstRun==1365){
    filename = "/media/audrey/COURGE/SOFIA/ANALYSE/SOFIA2/data/lmd_timestitched/Tl187/*.lmd";
    outputFileName = "../SofMacrosOutput/201410_Ubeam/tcal_Tl187.root";
  }
  else{
    filename = "/media/audrey/COURGE/SOFIA/ANALYSE/SOFIA2/data/lmd_timestitched/"+runNumber+".lmd";
    outputFileName = "../SofMacrosOutput/201410_Ubeam/tcal_run"+runNumber+".root";
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
#if SCI_MAPPED
  source->AddReader(new R3BSofSciReader((EXT_STR_h101_SOFSCI_t *)&ucesb_struct.sci,offsetof(EXT_STR_h101, sci)));
#endif
#if TOFW_MAPPED
  source->AddReader(new R3BSofToFWReader((EXT_STR_h101_SOFTOFW_t *)&ucesb_struct.tofw,offsetof(EXT_STR_h101, tofw)));
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



  // --- ----------------- --- //
  // --- Runtime data base --- //
  // --- ----------------- --- //
  FairRuntimeDb* rtdb = run->GetRuntimeDb();


#if SCI_TCAL || TOFW_TCAL 

  R3BSofTcalContFact needToConstructTcalContFact;

  // --- ------------------------- --- //
  // --- Load VFTX tcal parameters --- //
  // --- ------------------------- --- //
#if SAVE_TCALPAR_ROOT
  FairParRootFileIo* parIn = new FairParRootFileIo();
  parIn->open("../SofMacrosOutput/201410_Ubeam/Parameters/tcal_VFTX_SofSci.root");
#else
  FairParAsciiFileIo* parIn = new FairParAsciiFileIo();
  parIn->open("../SofMacrosOutput/201410_Ubeam/Parameters/tcal_VFTX_All.par","in");
#endif
  rtdb->setFirstInput(parIn);

  // --- ----------------------------- --- //
  // --- Add Mapped2Tcal analysis task --- //
  // --- ----------------------------- --- //
#if SCI_TCAL
  R3BSofSciMapped2Tcal* sciMapped2Tcal = new R3BSofSciMapped2Tcal();
  run->AddTask(sciMapped2Tcal);
#endif

#endif


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

