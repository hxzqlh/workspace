#include <stdio.h>
#include "CObject.h"

char szCObject [] = "CObject";

struct CRuntimeClass CObject::classCObject = {
    szCObject,  sizeof (CObject),  0xffff,  NULL,  NULL,  NULL};

static AFX_CLASSINIT _init_CObject (&CObject::classCObject);

CRuntimeClass * CRuntimeClass::pFirstClass =  NULL;

CRuntimeClass * CObject::GetRuntimeClass () const
{
    return &CObject::classCObject;
}

bool CObject::IsKindOf (const CRuntimeClass * pClass)  const
{
    CRuntimeClass * pClassThis = GetRuntimeClass ();
    while (pClassThis) {
        if (pClassThis == pClass)   
            return true;

        pClassThis = pClassThis->m_pBaseClass;
    }

    return  false;
}
