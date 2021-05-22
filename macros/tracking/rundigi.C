//--------------------------------------------------------------------
//
// Digitizer for p2p-fission experiments
// Author: <joseluis.rodriguez.sanchez@usc.es>
//
// Last Update: 05/11/19 (Jose Luis)
// Comments:
//
//--------------------------------------------------------------------

void rundigi()
{
    // ----- Files ---------------------------------------------------------------
    TString inFile = "../p2psim/sim.root";
    TString parFile = "../p2psim/par.root";
    TString outFile = "digi.root";
    // ---------------------------------------------------------------------------

    // ----- Timer ---------------------------------------------------------------
    TStopwatch timer;
    timer.Start();
    // ---------------------------------------------------------------------------

    // ----- Digitization --------------------------------------------------------
    FairRunAna* run = new FairRunAna();
    run->SetSource(new FairFileSource(inFile));
    run->SetSink(new FairRootFileSink(outFile));

    // ----- Runtime DataBase info -----------------------------------------------
    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    
    // -----   Load detector parameters    ------------------------------------
    FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
    parIo1->open("s455_setup.par", "in");
    rtdb->setFirstInput(parIo1);
    rtdb->print();
    // ----- Containers
    R3BTGeoPar* mwpc0Par = (R3BTGeoPar*)rtdb->getContainer("Mwpc0GeoPar");
    R3BTGeoPar* targetPar = (R3BTGeoPar*)rtdb->getContainer("TargetGeoPar");
    R3BTGeoPar* califaPar = (R3BTGeoPar*)rtdb->getContainer("CalifaGeoPar");
    R3BTGeoPar* mwpc1Par = (R3BTGeoPar*)rtdb->getContainer("Mwpc1GeoPar");
    R3BTGeoPar* twimPar = (R3BTGeoPar*)rtdb->getContainer("TwimGeoPar");
    R3BTGeoPar* mwpc2Par = (R3BTGeoPar*)rtdb->getContainer("Mwpc2GeoPar");
    R3BTGeoPar* mwpc3Par = (R3BTGeoPar*)rtdb->getContainer("Mwpc3GeoPar");
    R3BTGeoPar* tofwPar = (R3BTGeoPar*)rtdb->getContainer("TofwGeoPar");
    UInt_t runId = 1;
    rtdb->initContainers(runId);

    // ----- Connect the Digitization Task ---------------------------------------
    // MWPC1
    R3BSofMwpcDigitizer* mw1_digitizer = new R3BSofMwpcDigitizer("Mwpc1",1);
    run->AddTask(mw1_digitizer);

    // TWIM
    R3BSofTwimDigitizer* twim_digitizer = new R3BSofTwimDigitizer("TWIM",1);
    //run->AddTask(twim_digitizer);

    // MWPC2
    R3BSofMwpcDigitizer* mw2_digitizer = new R3BSofMwpcDigitizer("Mwpc2",1);
    //run->AddTask(mw2_digitizer);

    // MWPC3
    R3BSofMwpcDigitizer* mw3_digitizer = new R3BSofMwpcDigitizer("Mwpc3",1);
    run->AddTask(mw3_digitizer);

    // TOF-Wall
    R3BSofTofWDigitizer* tofw_digitizer = new R3BSofTofWDigitizer();
    run->AddTask(tofw_digitizer);

    // ----- Intialise and run ---------------------------------------------------
    run->Init();
    run->Run();
    // ---------------------------------------------------------------------------

    // ----- Finish --------------------------------------------------------------
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    cout << endl << endl;
    cout << "Macro finished succesfully." << endl;
    cout << "Output file writen:  " << outFile << endl;
    cout << "Parameter file writen " << parFile << endl;
    cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
    cout << endl;
    cout << " Test passed" << endl;
    cout << " All ok " << endl;
    cout << " Digitization successful." << endl;

}
