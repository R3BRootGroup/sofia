//--------------------------------------------------------------------
//
// Digitizer for p2p-fission experiments
// Author: <joseluis.rodriguez.sanchez@usc.es>
//
// Last Update: 05/11/19 (Jose Luis)
// Comments:
//
//
//
//--------------------------------------------------------------------

void RemoveGeoManager();

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
    // ---------------------------------------------------------------------------

    // ----- Connect the Digitization Task ---------------------------------------

    // MWPC1
    R3BSofMwpcDigitizer* mw1_digitizer = new R3BSofMwpcDigitizer("Mwpc1",1);
    mw1_digitizer->SetPosZ(177.75);
    run->AddTask(mw1_digitizer);

    // TWIM
    R3BSofTwimDigitizer* twim_digitizer = new R3BSofTwimDigitizer("TWIM",1);
    //twim_digitizer->SetPosZ(16.);
    //run->AddTask(twim_digitizer);

    // MWPC2
    R3BSofMwpcDigitizer* mw2_digitizer = new R3BSofMwpcDigitizer("Mwpc2",1);
    //mw2_digitizer->SetPosZ(95.);
    //run->AddTask(mw2_digitizer);

    // MWPC3
    R3BSofMwpcDigitizer* mw3_digitizer = new R3BSofMwpcDigitizer("Mwpc3",1);
    //mw3_digitizer->SetPosX(-243.);
    //mw3_digitizer->SetPosZ(689.);
    //mw3_digitizer->SetAngle(-29.);
    mw3_digitizer->SetPosX(-84.72);
    mw3_digitizer->SetPosZ(749.);
    mw3_digitizer->SetAngle(-13.178);
    mw3_digitizer->SetSigma_y(0.1);//1mm
    run->AddTask(mw3_digitizer);

    // TOF-Wall
    R3BSofTofWDigitizer* tofw_digitizer = new R3BSofTofWDigitizer();
    //tofw_digitizer->SetPosX(-257.);
    //tofw_digitizer->SetPosZ(710.);
    //tofw_digitizer->SetAngle(-29.);
    tofw_digitizer->SetPosX(-100.72);
    tofw_digitizer->SetPosZ(817.30);
    tofw_digitizer->SetAngle(-13.178);
    run->AddTask(tofw_digitizer);

    // ----- Runtime DataBase info -----------------------------------------------
    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    FairParRootFileIo* parIo1 = new FairParRootFileIo();
    parIo1->open(parFile.Data());
    rtdb->setFirstInput(parIo1);
    rtdb->setOutput(parIo1);
    rtdb->saveOutput();
    // ---------------------------------------------------------------------------

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
    // ---------------------------------------------------------------------------

    RemoveGeoManager();
}

/**
 * \ function RemoveGeoManager
 * There are some problems when deleting our geometries. In some cases
 * or combinations of geometries there is a double free of some memory
 * which results in a crash of ROOT. To avoid this we have patched one
 * ROOT class. With the newest ROOT6 version this isn't done any longer.
 * As a workaround to avoid the crash we delete two TObjArrays ourself
 * and then call the destructor of the TGeoManager at the end of the
 * macro. To simplify this one also can use this function.
 */
void RemoveGeoManager()
{
    if (gROOT->GetVersionInt() >= 60602)
    {
        gGeoManager->GetListOfVolumes()->Delete();
        gGeoManager->GetListOfShapes()->Delete();
        delete gGeoManager;
    }
}
