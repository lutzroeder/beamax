#ifndef __Stdafx__
#define __Stdafx__

#include <afxwin.h>
#include <afxext.h> 
#include <afxdisp.h>

#define CLASSOF(_instance, _class) (strcmp(_instance->GetRuntimeClass()->m_lpszClassName, _class) == 0 ? TRUE : FALSE)

#include "Analysis.h"

#endif