#ifndef __SNTP_H
#define __SNTP_H


#include <time.h>
#include "rl_net_lib.h"
#include <stdio.h>
#include "RTC.h"


extern struct tm t_SNTP;

void get_time (void);
void init_Pulsador (void);

#endif
