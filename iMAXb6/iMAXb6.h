#ifndef IMAXB6_H
#define IMAXB6_H

#ifdef IMAXB6_INNER_DLL
#define IMAXB6_FUNC __declspec(dllexport)
#else
#define IMAXB6_FUNC __declspec(dllimport)
#endif

IMAXB6_FUNC int test();



#endif
