#pragma once

#include <stdio.h>
#include "Airline.h"

typedef unsigned char BYTE;
#define AIRLINE_FILE_NAME "Airline.bin"

int saveAirlineToBFile(Airline* a, const char* fileName);
int saveFlightToBFile(Flight* f, FILE *fp);
int saveAirlineToComBFile(Airline *a, const char* filename, int isCompress);
int saveFlightToComBFile(Flight *f, FILE* fp, int isCompress);
int readAirlineFromBFile(Airline* a, const char* fileName);
int readAirlineFromComBFile(Airline *a, const char* filename, int isCompress);
int readFlightFromBFile(Flight* f, FILE *fp);
int readFlightFromComBFile(Flight *f, FILE* fp, int isCompress);
