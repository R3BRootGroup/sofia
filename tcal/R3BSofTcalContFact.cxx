// ----------------------------------------------------------------------
// -----          R3BFrsContFact source file                        -----
// -----             Created 22/09/18  by J.L. Rodriguez-Sanchez    -----
// ----------------------------------------------------------------------
//
//  R3BFrsContFact
//
//  Factory for the parameter containers in libR3BFrs
//

#include "R3BSofTcalContFact.h"

#include "FairLogger.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"
#include "FairRuntimeDb.h"

#include "R3BSofTcalPar.h"

#include "TClass.h"

static R3BSofTcalContFact gR3BSofTcalContFact;

R3BSofTcalContFact::R3BSofTcalContFact()
{
    // Constructor (called when the library is loaded)
    fName = "R3BSofTcalContFact";
    fTitle = "Factory for Tcal parameter containers in libR3BSofTcal";
    setAllContainers();
    LOG(INFO) << "Enter in the R3BSofTcalContFact constructor";
    FairRuntimeDb::instance()->addContFactory(this);
}

void R3BSofTcalContFact::setAllContainers()
{
    /** Creates the Container objects with all accepted contexts and adds them to
     *  the list of containers for the STS library.*/

    // --- ---------------------------------- --- //
    // --- for scintillators at S2 and Cave C --- //
    // --- ---------------------------------- --- //
    FairContainer* p1 = new FairContainer("SofSciTcalPar", "Sof Tcal Parameters", "SofTcalParContext");
    p1->addContext("SofTcalParContext");
    containers->Add(p1);

    // --- ----------------------- --- //
    // --- for time-of-flight wall --- //
    // --- ----------------------- --- //
    FairContainer* p2 = new FairContainer("SofTofWTcalPar", "Sof Tcal Parameters", "SofTcalParContext");
    p2->addContext("SofTcalParContext");
    containers->Add(p2);
}

FairParSet* R3BSofTcalContFact::createContainer(FairContainer* c)
{
    /** Trals the constructor of the corresponding parameter container.
     * For an actual context, which is not an empty string and not the default context
     * of this container, the name is concatinated with the context. */

    const char* name = c->GetName();
    LOG(INFO) << "R3BSofTcalContFact: Create container name: " << name;
    FairParSet* p = 0;

    if (strcmp(name, "SofSciTcalPar") == 0)
    {
        p = new R3BSofTcalPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }
    if (strcmp(name, "SofTofWTcalPar") == 0)
    {
        p = new R3BSofTcalPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }

    return p;
}

void R3BSofTcalContFact::activateParIo(FairParIo* io)
{
    // activates the input/output class for the parameters
    // needed by the Sts
}

ClassImp(R3BSofTcalContFact)
