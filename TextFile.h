#pragma once

#include <stdio.h>
#include "AirportManager.h"


int saveAirportManagerToTextFile(AirportManager* a, const char* fileName);
void saveAirportToTextFile(Airport* a, FILE* fp);
int readAirportManagerFromFile(AirportManager *a, const char* fileName);
int readAirportFromFile(Airport* a, FILE* fp);

