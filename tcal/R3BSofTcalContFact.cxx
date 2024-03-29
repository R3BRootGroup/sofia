// ----------------------------------------------------------------
// -----        R3BSofTcalContFact source file                -----
// -----    Created 22/09/18  by J.L. Rodriguez-Sanchez       -----
// ----------------------------------------------------------------
//
//  R3BSofTcalContFact
//
//  Factory for the parameter containers
//

#include "R3BSofTcalContFact.h"

#include "FairLogger.h"
#include "FairParSet.h"    // for FairParSet
#include "FairRuntimeDb.h" // for FairRuntimeDb
#include "R3BSofTcalPar.h"

static R3BSofTcalContFact gR3BSofTcalContFact;

R3BSofTcalContFact::R3BSofTcalContFact()
{
    // Constructor (called when the library is loaded)
    fName = "R3BSofTcalContFact";
    fTitle = "Factory for Tcal parameter containers in libR3BSofTcal";
    setAllContainers();
    FairRuntimeDb::instance()->addContFactory(this);
}

void R3BSofTcalContFact::setAllContainers()
{
    /** Creates the Container objects with all accepted contexts and adds them to
     *  the list of containers for the STS library.*/

    // --- --------------------------------------------------- --- //
    // --- VFTX calibration for scintillators at S2 and Cave C --- //
    // --- --------------------------------------------------- --- //
    /* FairContainer* p1 = new FairContainer("SofSciTcalPar", "Sof Tcal Parameters", "SofTcalParContext");
     p1->addContext("SofTcalParContext");
     containers->Add(p1);

     // --- ---------------------------------------- --- //
     // --- VFTX calibration for time-of-flight wall --- //
     // --- ---------------------------------------- --- //
     FairContainer* p2 = new FairContainer("SofTofWTcalPar", "Sof Tcal Parameters", "SofTcalParContext");
     p2->addContext("SofTcalParContext");
     containers->Add(p2);

     // --- ----------------------------------------------- --- //
     // --- Clock offset for scintillators at S2 and Cave C --- //
     // --- ----------------------------------------------- --- //
     FairContainer* p3 = new FairContainer("SofSciClockOffsetPar", "Sof Tcal Parameters", "SofTcalParContext");
     p3->addContext("SofTcalParContext");
     containers->Add(p3);

     // --- ------------------------------------ --- //
     // --- Clock offset for time-of-flight wall --- //
     // --- ------------------------------------ --- //
     FairContainer* p4 = new FairContainer("SofTofWClockOffsetPar", "Sof Tcal Parameters", "SofTcalParContext");
     p4->addContext("SofTcalParContext");
     containers->Add(p4);*/
}

FairParSet* R3BSofTcalContFact::createContainer(FairContainer* c)
{
    /** Trals the constructor of the corresponding parameter container.
     * For an actual context, which is not an empty string and not the default context
     * of this container, the name is concatinated with the context. */

    const char* name = c->GetName();
    LOG(info) << "R3BSofTcalContFact: Create container name: " << name;
    FairParSet* p = 0;
    /*
        if (strcmp(name, "SofSciTcalPar") == 0)
        {
            p = new R3BSofTcalPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
        }

        else if (strcmp(name, "SofTofWTcalPar") == 0)
        {
            p = new R3BSofTcalPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
        }

        else if (strcmp(name, "SofSciClockOffsetPar") == 0)
        {
            p = new R3BSofTcalPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
        }

        else if (strcmp(name, "SofTofWClockOffsetPar") == 0)
        {
            p = new R3BSofTcalPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
        }*/

    return p;
}

ClassImp(R3BSofTcalContFact);
