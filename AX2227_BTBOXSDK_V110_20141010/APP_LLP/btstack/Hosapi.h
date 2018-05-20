#ifndef __HOSAPI_H__
#define __HOSAPI_H__

#include "global.h"

void os_free (void *point);     //2014-3-5, 放到BANK节省公共区
void *os_malloc (u16 len);
void Osapi_Init(void);


#endif
