#include <stdio.h>
#include <stdexcept>
#include <string.h>

#define RUNTIME_CLASS(class_name) \
    (&class_name::class##class_name)

#define DECLARE_DYNAMIC(class_name) \
public:\
    static  CRuntimeClass  class##class_name;\
    virtual CRuntimeClass  * GetRuntimeClass ()  const;

#define _IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew) \
    static char _lpsz##class_name[] = #class_name; \
    CRuntimeClass class_name::class##class_name = { \
        _lpsz##class_name, sizeof(class_name), wSchema, pfnNew, \
            RUNTIME_CLASS (base_class_name),  NULL};  \
    static AFX_CLASSINIT _init_##class_name (&class_name::class##class_name); \
    CRuntimeClass * class_name::GetRuntimeClass ()  const \
        {return &class_name::class##class_name;}

#define IMPLEMENT_DYNAMIC(class_name, base_class_name) \
    _IMPLEMENT_RUNTIMECLASS (class_name, base_class_name, 0xFF, NULL)

#define DECLARE_DYNCREATE(class_name)  \
    DECLARE_DYNAMIC(class_name)  \
    static CObject* CreateObject ();

#define IMPLEMENT_DYNCREATE(class_name, base_class_name) \
    CObject* class_name::CreateObject () \
        {return new class_name;} \
    _IMPLEMENT_RUNTIMECLASS (class_name, base_class_name, 0xFFF, \
        class_name::CreateObject)

struct CRuntimeClass;

class CObject
{
public:
    virtual ~ CObject () {}
    
    virtual CRuntimeClass * GetRuntimeClass () const;
    bool IsKindOf (const CRuntimeClass * pClass) const;

public:
    static CRuntimeClass classCObject;
};

struct CRuntimeClass
{
    char *                 m_lpszClassName;//用来保存类的名称
    unsigned               m_nObjectSize;// 类对象的大小
    unsigned               m_wSchema;
    CObject *              (* m_pfnCreateObject) ();
    CRuntimeClass *        m_pBaseClass;  //指向基类的CRuntimeClass 的指针
    CRuntimeClass *        m_pNextClass;   //下一个 CRuntimeClass结点

    static CRuntimeClass * pFirstClass;  //静态成员，整个链表的头结点

    static CObject * Create (const char * class_name)
    {
        CRuntimeClass * pClass = NULL;
        bool found_class       = false;
        for (pClass = pFirstClass; pClass; pClass = pClass->m_pNextClass) {
            if (strcmp (class_name, pClass->m_lpszClassName) == 0) {
                found_class = true;
                break;
            }
        }
        
        if (!found_class) 
            throw std::runtime_error ("invalid dynamic class");

        if (!pClass->m_pfnCreateObject)
            throw std::runtime_error ("the class doesn't support dynamic creation");

        CObject * pObject = (*pClass->m_pfnCreateObject) ();
        return pObject;
    }
};

struct AFX_CLASSINIT
{
    AFX_CLASSINIT (CRuntimeClass * pNewClass)
    {
        pNewClass->m_pNextClass    = CRuntimeClass::pFirstClass;
        CRuntimeClass::pFirstClass = pNewClass;
    }
};
