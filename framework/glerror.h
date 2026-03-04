#ifndef _GL_ERROR_
#define _GL_ERROR_
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <libheaders.h>
#include <fw_config.h>

// *** DEBUG-KRAM KOMPLETT AUS ***
// Früher:
// #ifdef CGA2_DEBUG
// #define GLERR printglerror(__FILE__, __LINE__);
// #else
// #define GLERR
// #endif

// Jetzt: GLERR macht GAR NICHTS mehr
#define GLERR

// Optional: Dummy-Implementierungen, falls irgendwo aufgerufen
void printglerror(const char* file, int line);
bool checkglerror_(const char* file, int line);

#define checkglerror() checkglerror_(__FILE__, __LINE__)

#endif
