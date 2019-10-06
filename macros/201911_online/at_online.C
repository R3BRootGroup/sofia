/* Additional info:
 * To generate the header file used for the R3BUcesbSource (ext_h101.h), use:
 *
 * ./201911_s455 --ntuple=RAW:AT,id=h101_AT,ext_h101_at.h
 *
 * at $UCESB_DIR/upexps/201911_s455
 *
 * Put this header file into the 'r3bsource' directory and recompile.
 *
 -
 - NOT READY! Reader and structures must be fixed by Audrey!!!
 -
 * Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
 * @since October 05, 2019
 * */

typedef struct EXT_STR_h101_t {
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_AT_t at; //FIXME
} EXT_STR_h101;

void at_online() {
  TStopwatch timer;
  timer.Start();
  
  const Int_t nev = -1; // number of events to read, -1 - until CTRL+C
  //const Int_t nev = 10; // Only 10 events to read  

  /* Create source using ucesb for input ------------------ */
  
  TString filename = "--stream=lxg0898:6002";//FIXME
  //TString filename = "~/lmd/name.lmd";
  
  TString outputFileName = "data_at_online.root";
  
  TString ntuple_options = "RAW";
  TString ucesb_dir = getenv("UCESB_DIR");
  TString ucesb_path = ucesb_dir + "/../upexps/201911_s455/201911_s455";//FIXME
  ucesb_path.ReplaceAll("//","/");
  
  EXT_STR_h101 ucesb_struct;
  
  R3BUcesbSource* source = new R3BUcesbSource(filename, ntuple_options,
					      ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
  source->SetMaxEvents(nev);
  

  /* Definition of reader --------------------------------- */
  R3BUnpackReader* unpackreader = new R3BUnpackReader((EXT_STR_h101_unpack*)&ucesb_struct,
					offsetof(EXT_STR_h101, unpack));

  R3BAtReader* unpackat= new R3BAtReader((EXT_STR_h101_AT*)&ucesb_struct.at,
					     offsetof(EXT_STR_h101, at));   //FIXME


  /* Add readers ------------------------------------------ */
  source->AddReader(unpackreader);
  //unpackat->SetOnline(true);
  source->AddReader(unpackat);

  
  /* Create online run ------------------------------------ */
  FairRunOnline* run = new FairRunOnline(source);
  run->SetRunId(1);
  //run->SetSink(new FairRootFileSink(outputFileName));
  Int_t refresh = 1;
  Int_t port=8044;
  run->ActivateHttpServer(refresh, port);


  /* Runtime data base ------------------------------------ */
  FairRuntimeDb* rtdb = run->GetRuntimeDb();


  /* Load parameters   ------------------------------------ */ 
/*  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  parIo1->open("At_CalibParam.root","in");
  rtdb->setFirstInput(parIo1);
  rtdb->print();
*/

  /* Add analysis task ------------------------------------ */  
/*  R3BAtMapped2Cal* AtMap2Cal = new R3BAtMapped2Cal();
  //AtMap2Cal->SetOnline(true);
  run->AddTask(AtMap2Cal);
*/

  /* Add online tasks ------------------------------------- */
  R3BSofOnlineSpectra* sofonline= new R3BSofOnlineSpectra();
  run->AddTask(sofonline);
  R3BSofAtOnlineSpectra* atonline= new R3BSofAtOnlineSpectra();
  run->AddTask(atonline);


  /* Initialize ------------------------------------------- */
  run->Init();
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  //FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
  //FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  /* ------------------------------------------------------ */


  /* Run -------------------------------------------------- */
  run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);


  /* Finish ----------------------------------------------- */
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
