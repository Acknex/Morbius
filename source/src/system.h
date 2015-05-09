#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define PRAGMA_ZERO

#include <acknex.h>
#include <litec.h>
#include <windows.h>
#include <stdio.h>
#include <mtlView.c>
#include <mtlfx.c>

typedef BOOL bool;
#define NULL 0
#define null 0
#define true 1
#define false 0

void sys_init();

#include "system.c"

#endif