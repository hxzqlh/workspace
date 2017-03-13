#include <iostream>
#include "CObject.h"

using namespace std;

class CView: public CObject
{    
    // DECLARE_DYNAMIC (CView)
    DECLARE_DYNCREATE (CView)

public:
    CView () {}
    virtual ~ CView () {cout << "~CView()" << endl;}
};

// IMPLEMENT_DYNAMIC (CView, CObject)
IMPLEMENT_DYNCREATE (CView, CObject)

int main ()
{
    CObject * p = new CView;
    if (p) {
        cout << p->IsKindOf (RUNTIME_CLASS (CObject)) << endl;
        delete p;
    }

    p = CRuntimeClass::Create ("CView");
    if (p) {
        cout << p->IsKindOf (RUNTIME_CLASS (CObject)) << endl;
        delete p;
    }

    p = CRuntimeClass::Create ("Unknown");

    return 0;
}

