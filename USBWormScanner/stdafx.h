// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#define STRICT
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files
#include <Windows.h>
#include <process.h>
#include <TlHelp32.h>
#include <shellapi.h>
#include <ShlObj.h>
#include <comutil.h>
#include <Dbt.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <strsafe.h>

// C++ stuff
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <string>

// TODO: reference additional headers your program requires here
