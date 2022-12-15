
#include "R3BSofSciContFact.h"

#include "FairLogger.h"
#include "FairParSet.h"
#include "FairRuntimeDb.h"
#include "R3BSofSciCalPosPar.h"
#include "R3BSofSciCalTofPar.h"
#include "R3BSofSciRawPosPar.h"
#include "R3BSofSciRawTofPar.h"
#include "R3BSofTcalPar.h"
#include "R3BTGeoPar.h"

static R3BSofSciContFact gR3BSofSciContFact;

R3BSofSciContFact::R3BSofSciContFact()
{
    // Constructor (called when the library is loaded)
    fName = "R3BSofSciContFact";
    fTitle = "Factory for Sci parameter containers in libR3BSofSci";
    setAllContainers();
    FairRuntimeDb::instance()->addContFactory(this);
}

void R3BSofSciContFact::setAllContainers()
{
    /** Creates the Container objects with all accepted contexts and adds them fto
     *  the list of containers for the STS library.*/
    FairContainer* p1 = new FairContainer("SofSciRawPosPar", "Sof Sci RawPos Parameters", "SofSciRawPosParContext");
    p1->addContext("SofSciRawPosParContext");
    containers->Add(p1);

    FairContainer* p2 = new FairContainer("SofSciRawTofPar", "Sof Sci RawTof Parameters", "SofSciRawTofParContext");
    p2->addContext("SofSciRawTofParContext");
    containers->Add(p2);

    FairContainer* p3 = new FairContainer("SofSciCalPosPar", "Sof Sci CalPos Parameters", "SofSciCalPosParContext");
    p3->addContext("SofSciCalPosParContext");
    containers->Add(p3);

    FairContainer* p4 = new FairContainer("SofSciCalTofPar", "Sof Sci CalTof Parameters", "SofSciCalTofParContext");
    p4->addContext("SofSciCalTofParContext");
    containers->Add(p4);

    FairContainer* p5 = new FairContainer("SofSciTcalPar", "Sof Tcal Parameters", "SofTcalParContext");
    p5->addContext("SofTcalParContext");
    containers->Add(p5);

    FairContainer* p6 = new FairContainer("SofSciGeoPar", "Sof Sci geometry parameters", "GeometryParameterContext");
    p6->addContext("GeometryParameterContext");
    containers->Add(p6);
}

FairParSet* R3BSofSciContFact::createContainer(FairContainer* c)
{
    /** Trals the constructor of the corresponding parameter container.
     * For an actual context, which is not an empty string and not the default context
     * of this container, the name is concatinated with the context. */

    const char* name = c->GetName();
    LOG(info) << "R3BSofSciContFact: Create container name: " << name;
    FairParSet* p = 0;

    if (strcmp(name, "SofSciTcalPar") == 0) {
        p = new R3BSofTcalPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    } else if (strcmp(name, "SofSciRawPosPar") == 0) {
        p = new R3BSofSciRawPosPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    } else if (strcmp(name, "SofSciRawTofPar") == 0) {
        p = new R3BSofSciRawTofPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    } else if (strcmp(name, "SofSciCalPosPar") == 0) {
        p = new R3BSofSciCalPosPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    } else if (strcmp(name, "SofSciCalTofPar") == 0) {
        p = new R3BSofSciCalTofPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    } else if (strcmp(name, "SofSciGeoPar") == 0) {
        p = new R3BTGeoPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }
    return p;
}

ClassImp(R3BSofSciContFact);
