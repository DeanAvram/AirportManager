#include <stdlib.h>
#include <string.h>
#include "TextFile.h"
#include "General.h"


int saveAirportManagerToTextFile(AirportManager* a, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "w");
	if (!fp) {
		printf("Error open file to write\n");
		return 0;
	}
	fprintf(fp, "%d\n", a->airportsCount);
	for (int i = 0; i < a->airportsCount; i++)
	{
		saveAirportToTextFile(&a->airportsArr[i], fp);
	}
	fclose(fp);
	return 1;
}

void saveAirportToTextFile(Airport* a, FILE* fp)
{
	fprintf(fp, "%s\n%s\n", a->name, a->address);
}

int readAirportManagerFromFile(AirportManager *a, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "r");
	if (!fp) {
		printf("Error open file to read\n");
		return 0;
	}
	if (fscanf(fp, "%d", &a->airportsCount) != 1)
	{
		fclose(fp);
		return 0;
	}
	char buffer[MAX_STR_LEN];
	fgets(buffer, MAX_STR_LEN, fp);
	a->airportsArr = (Airport*)malloc(a->airportsCount * sizeof(Airport));
	for (int i = 0; i < a->airportsCount; i++)
	{
		if (readAirportFromFile(&a->airportsArr[i], fp) == 0)
		{
			fclose(fp);
			return 0;
		}
	}
	return 1;
}

int readAirportFromFile(Airport* a, FILE* fp) 
{
	char name[MAX_STR_LEN];
	char address[MAX_STR_LEN];
	
	myGets(name, MAX_STR_LEN, fp);
	a->name = _strdup(name);
	myGets(address, MAX_STR_LEN, fp);
	a->address = _strdup(address);
	if (!a->name || !a->address)
	{
		fclose(fp);
		return 0;
	}
	return 1;
	
}