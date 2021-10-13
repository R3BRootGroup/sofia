 /*  
 *  Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
 *  @since October 20, 2019
 * 
 */

typedef struct EXT_STR_h101_t {
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_MUSIC_onion_t music;
  EXT_STR_h101_SOFSCI_onion_t sci;
  EXT_STR_h101_SOFTOFW_onion_t tofw;
  EXT_STR_h101_SOFTWIM_onion_t twim;
  EXT_STR_h101_SOFMWPC_onion_t mwpc;
} EXT_STR_h101;

void eng_online() 
{
  TStopwatch timer;
  timer.Start();
  
  //const Int_t nev = -1; // number of events to read, -1 - until CTRL+C
  const Int_t nev = -1; // Only nev events to read  

  // Create source using ucesb for input ------------------
  
  TString filename = "--stream=lxir123:7803";
  //TString filename = "~/lmd/sofia2019/main0079_0001.lmd";
  //TString filename = "/media/audrey/COURGE/SOFIA/ANALYSE/SOFIA3/data/main0028_0001.lmd";

  TString outputFileName = "data_online.root";
  //TString outputFileName = "../SofMacrosOutput/201911_online/data_online.root";
  
  TString ntuple_options = "RAW";
  TString ucesb_dir = getenv("UCESB_DIR");
  TString ucesb_path = "/u/land/lynx.landexp/201911_eng/upexps/201911_eng2/201911_eng2 --allow-errors --input-buffer=100Mi";
  //TString ucesb_path = "/u/land/sofia/unpacker/upexps/201911_eng2/201911_eng2 --allow-errors --input-buffer=100Mi";
  //TString ucesb_path = ucesb_dir + "/../upexps/201911_eng2/201911_eng2 --allow-errors --input-buffer=100Mi";
  ucesb_path.ReplaceAll("//","/");
  
  EXT_STR_h101 ucesb_struct;

  R3BUcesbSource* source = new R3BUcesbSource(filename, ntuple_options,
					      ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
  source->SetMaxEvents(nev);

  // Definition of reader --------------------------------- 
  R3BUnpackReader* unpackreader = new R3BUnpackReader((EXT_STR_h101_unpack*)&ucesb_struct,
					offsetof(EXT_STR_h101, unpack));

  R3BMusicReader* unpackmusic = new R3BMusicReader((EXT_STR_h101_MUSIC_t*)&ucesb_struct.music,
					offsetof(EXT_STR_h101, music));

  R3BSofSciReader* unpacksci = new R3BSofSciReader((EXT_STR_h101_SOFSCI_t*)&ucesb_struct.sci,
					offsetof(EXT_STR_h101, sci));

  R3BSofMwpcReader* unpackmwpc = new R3BSofMwpcReader((EXT_STR_h101_SOFMWPC_t*)&ucesb_struct.mwpc,
					offsetof(EXT_STR_h101, mwpc));

  R3BSofTwimReader* unpacktwim = new R3BSofTwimReader((EXT_STR_h101_SOFTWIM_t*)&ucesb_struct.twim,
					offsetof(EXT_STR_h101, twim));

  R3BSofToFWReader* unpacktofw = new R3BSofToFWReader((EXT_STR_h101_SOFTOFW_t*)&ucesb_struct.tofw,
					offsetof(EXT_STR_h101, tofw));


  // Add readers ------------------------------------------ 
  source->AddReader(unpackreader);
  source->AddReader(unpackmusic);
  source->AddReader(unpacksci);
  source->AddReader(unpackmwpc);
  source->AddReader(unpacktwim);
  source->AddReader(unpacktofw);


  // Create online run ------------------------------------ 
  FairRunOnline* run = new FairRunOnline(source);
  run->SetRunId(1);
  run->SetSink(new FairRootFileSink(outputFileName));
  Int_t refresh = 1;
  Int_t port=8888;
  run->ActivateHttpServer(refresh, port);


  // Runtime data base ------------------------------------ 
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();//Ascii
  parIo1->open("./parameters/CalibParam.par","in");
  rtdb->setFirstInput(parIo1);
  rtdb->print();

  // Add analysis task ------------------------------------

  //Musics
  R3BMusicMapped2Cal* MusMap2Cal = new R3BMusicMapped2Cal();
  //MusMap2Cal->SetOnline(true);
  run->AddTask(MusMap2Cal);

  R3BMusicCal2Hit* MusCal2Hit = new R3BMusicCal2Hit();
  //MusMap2Cal->SetOnline(true);
  run->AddTask(MusCal2Hit);

  R3BSofMwpc0Mapped2Cal* MW0Map2Cal = new R3BSofMwpc0Mapped2Cal();
  //MW0Map2Cal->SetOnline(true);
  run->AddTask(MW0Map2Cal);

  R3BSofMwpc0Cal2Hit* MW0Cal2Hit = new R3BSofMwpc0Cal2Hit();
  //MW0Cal2Hit->SetOnline(true);
  //run->AddTask(MW0Cal2Hit);

  //TWIM
  R3BSofTwimMapped2Cal* TwimMap2Cal = new R3BSofTwimMapped2Cal();
  run->AddTask(TwimMap2Cal);

  R3BSofTwimCal2Hit* TwimCal2Hit = new R3BSofTwimCal2Hit();
  run->AddTask(TwimCal2Hit);

  R3BSofMwpc2Mapped2Cal* MW2Map2Cal = new R3BSofMwpc2Mapped2Cal();
  //MW2Map2Cal->SetOnline(true);
  run->AddTask(MW2Map2Cal);

  //R3BSofMwpc2Cal2Hit* Calh2 = new R3BSofMwpc2Cal2Hit();
  //Map2Cal->SetOnline(true);
  //run->AddTask(Calh2);

  R3BSofMwpc3Mapped2Cal* MW3Map2Cal = new R3BSofMwpc3Mapped2Cal();
  //Map2Cal->SetOnline(true);
  run->AddTask(MW3Map2Cal);

  // --- Mapped 2 Tcal for SofSci
  R3BSofSciMapped2Tcal* SofSciMap2Tcal = new R3BSofSciMapped2Tcal();
  run->AddTask(SofSciMap2Tcal);

  // --- Mapped 2 Tcal for SofToFW
  R3BSofToFWMapped2Tcal* SofToFWMap2Tcal = new R3BSofToFWMapped2Tcal();
  run->AddTask(SofToFWMap2Tcal);
  
  // --- Tcal 2 SingleTcal for SofSci
  R3BSofSciTcal2SingleTcal* SofSciTcal2STcal = new R3BSofSciTcal2SingleTcal();
  run->AddTask(SofSciTcal2STcal);


  // Add online task ------------------------------------
  //R3BSofAtOnlineSpectra* atonline= new R3BSofAtOnlineSpectra();
  //run->AddTask(atonline);

  R3BMusicOnlineSpectra* musonline= new R3BMusicOnlineSpectra();
  run->AddTask(musonline);
  R3BSofTwimOnlineSpectra* twonline= new R3BSofTwimOnlineSpectra();
  run->AddTask(twonline);
  R3BSofMwpcOnlineSpectra* mw0online= new R3BSofMwpcOnlineSpectra("SofMwpc0OnlineSpectra",1,"Mwpc0");
  run->AddTask(mw0online);
  R3BSofMwpcCorrelationOnlineSpectra* mw0mw2online= new R3BSofMwpcCorrelationOnlineSpectra("SofMwpc0_2CorrelationOnlineSpectra",1,"Mwpc0","Mwpc2");
  run->AddTask(mw0mw2online);
  R3BSofMwpcOnlineSpectra* mw2online= new R3BSofMwpcOnlineSpectra("SofMwpc2OnlineSpectra",1,"Mwpc2");
  run->AddTask(mw2online);
  R3BSofMwpcOnlineSpectra* mw3online= new R3BSofMwpcOnlineSpectra("SofMwpc3OnlineSpectra",1,"Mwpc3");
  run->AddTask(mw3online);
  R3BSofSciOnlineSpectra* scionline= new R3BSofSciOnlineSpectra();
  run->AddTask(scionline);
  R3BSofToFWOnlineSpectra* tofwonline= new R3BSofToFWOnlineSpectra();
  run->AddTask(tofwonline);

  R3BSofOnlineSpectra* sofonline= new R3BSofOnlineSpectra();
  run->AddTask(sofonline);


  // Initialize ------------------------------------------- 
  run->Init();
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");


  // Run -------------------------------------------------- 
  run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);


  // Finish ----------------------------------------------- 
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Output file is " << outputFileName << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s"
            << std::endl << std::endl;
  //gApplication->Terminate();
}
