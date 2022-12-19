// --------------------------------------------------------------------
//								    -----
//  Factory for the parameter containers in libR3BSofTofW         -----
//         Created 03/11/19  by J.L. Rodriguez-Sanchez            -----
//								    -----
// --------------------------------------------------------------------

#include "R3BSofTofWContFact.h"

#include "FairLogger.h"
#include "FairParSet.h"    // for FairParSet
#include "FairRuntimeDb.h" // for FairRuntimeDb
#include "R3BSofTcalPar.h"
#include "R3BSofTofWHitPar.h"
#include "R3BTGeoPar.h"

static R3BSofTofWContFact gR3BSofTofWContFact;

R3BSofTofWContFact::R3BSofTofWContFact()
{
    // Constructor (called when the library is loaded)
    fName = "R3BSofTofWContFact";
    fTitle = "Factory for parameter containers in libR3BSofTofW";
    setAllContainers();
    FairRuntimeDb::instance()->addContFactory(this);
}

void R3BSofTofWContFact::setAllContainers()
{
    // Creates the Container objects with all accepted contexts and adds them to
    // the list of containers for the TofW library.

    FairContainer* p1 = new FairContainer("TofwGeoPar", "TofW geometry parameters", "GeometryParameterContext");
    p1->addContext("GeometryParameterContext");
    containers->Add(p1);

    FairContainer* p2 = new FairContainer("tofwCalPar", "TofW Cal Parameters", "TofWCalParContext");
    p2->addContext("TofWCalParContext");
    containers->Add(p2);

    FairContainer* p3 = new FairContainer("tofwHitPar", "TofW Hit Parameters", "TTofWHitParContext");
    p3->addContext("TofWHitParContext");
    containers->Add(p3);

    FairContainer* p4 = new FairContainer("SofTofWTcalPar", "Sof Tcal Parameters", "SofTcalParContext");
    p4->addContext("SofTcalParContext");
    containers->Add(p4);
}

FairParSet* R3BSofTofWContFact::createContainer(FairContainer* c)
{
    // Trals the constructor of the corresponding parameter container.
    // For an actual context, which is not an empty string and not the default context
    // of this container, the name is concatinated with the context.

    const char* name = c->GetName();
    LOG(info) << "R3BSofTofWContFact: Create container name: " << name;
    FairParSet* p = 0;

    if (strcmp(name, "tofwHitPar") == 0)
    {
        p = new R3BSofTofWHitPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }

    else if (strcmp(name, "TofwGeoPar") == 0)
    {
        p = new R3BTGeoPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }

    else if (strcmp(name, "SofTofWTcalPar") == 0)
    {
        p = new R3BSofTcalPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }

    return p;
}

ClassImp(R3BSofTofWContFact);
