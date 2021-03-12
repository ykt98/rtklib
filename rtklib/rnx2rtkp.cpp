//// rtklib.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
///*------------------------------------------------------------------------------
//* convbin.c : convert receiver binary log file to rinex obs / nav, sbas messages
//*
//* Copyright(C) 2007 - 2018 by T.TAKASU, All rights reserved.
//*
//* options : -DWIN32 use windows file path separator
//*
//*version : $Revision : 1.1 $ $Date : 2008 / 07 / 17 22 : 13 : 04 $
//* history : 2008 / 06 / 22 1.0 new
//* 2009 / 06 / 17 1.1 support glonass
//* 2009 / 12 / 19 1.2 fix bug on disable of glonass
//* fix bug on inproper header for rtcm2and rtcm3
//* 2010 / 07 / 18 1.3 add option - v, -t, -h, -x
//* 2011 / 01 / 15 1.4 add option - ro, -hc, -hm, -hn, -ht, -ho, -hr, -ha,
//* -hp, -hd, -y, -c, -q
//* support gw10 and javad receiver, galileo, qzss
//* support rinex file name convention
//* 2012 / 10 / 22 1.5 add option - scan, -oi, -ot, -ol
//* change default rinex version to 2.11
//* fix bug on default output directory(/ -> .)
//* support galileo nav(LNAV) output
//* support compass
//* 2012 / 11 / 19 1.6 fix bug on setting code mask in rinex options
//* 2013 / 02 / 18 1.7 support binex
//* 2013 / 05 / 19 1.8 support auto format for file path with wild - card
//* 2014 / 02 / 08 1.9 add option - span - trace - mask
//* 2014 / 08 / 26 1.10 add Trimble RT17 support
//* 2014 / 12 / 26 1.11 add option - nomask
//* 2016 / 01 / 23 1.12 enable septentrio
//* 2016 / 05 / 25 1.13 fix bug on initializing output file paths in
//* convbin()
//* 2016 / 06 / 09 1.14 fix bug on output file with - v 3.02
//* 2016 / 07 / 01 1.15 support log format CMR / CMR +
//*2016 / 07 / 31 1.16 add option - halfc
//* 2017 / 05 / 26 1.17 add input format tersus
//* 2017 / 06 / 06 1.18 fix bug on output beidou and irnss nav files
//* add option - tt
//* 2018 / 10 / 10 1.19 default options are changed.
//* scan input file : off - on
//* number of freq : 2 -> 3
//* add option - noscan
//* ---------------------------------------------------------------------------- - */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "rtklib.h"
#include <iostream>


#define PRGNAME   "CONVBIN"
#define TRACEFILE "convbin.trace"
#define NOUTFILE        9       /* number of output files */


extern void  settspan(gtime_t ts, gtime_t te) {}
extern void settime(gtime_t time) {}


extern int showmsg(char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	vfprintf(stderr, format, arg);
	va_end(arg);
	fprintf(stderr, "\r");
	return 0;
}

 
int main(void)
{
    int opt2[8] = { 10000, 10000, 1000, 32768, 10, 1000, 30, 0 };
    int strs[2] = { STR_NTRIPCLI ,STR_FILE };
    double nemapos[3] = { 0 };
    char* path[8], * cmd[4] = { 0 }, * cmds_p[4] = { 0 };
    strconv_t* conv[4] = { 0 };

    strsvr_t* svr = (strsvr_t*)malloc(sizeof(strsvr_t));
    for (int i = 0; i < 8; i++)
        path[i] = (char*)malloc(sizeof(char) * 1024);

    /*printf("请输入输出路径:");
    scanf_s("%s", path[1]);*/

    path[0] = "zsc:13986159292@base.devecent.com:6060/CUGZX:NULL";
    path[1] = "D:\\project\\20200927\\data";

    /* path[0] = string("%1:%2@%3:%4/%5:%6").arg(User_Text).arg(Passwd_Text)
         .arg(Addr_Text).arg(Port_Text).arg(MntPnt_Text)
         .arg(Str_Text);*/

    strsvrinit(svr, 0);
    strsvrstart(svr, opt2, strs, path, conv, cmd, cmds_p, nemapos);
    FILE* fp;
    unsigned int npb0 = 0;
    while ((fp = fopen(path[1], "w")) == NULL) {
        printf("无法打开文件\n");
        exit(0);
    }
    while (1)
    {
        strlock(&svr->stream[0]);
        for (unsigned int i = 0; i < svr->stream[0].inb - npb0; i++)
        {
            //printf("%c", svr->buff[i]);
            fputc(svr->buff[i], fp);
        }
        if (npb0 < svr->stream[0].inb) {
            npb0 = svr->stream[0].inb;
        }

        strunlock(&svr->stream[0]);
        sleepms(1);
    }
    fclose(fp);


    return EXIT_SUCCESS;
}

