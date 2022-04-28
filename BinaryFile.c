#include <stdlib.h>
#include <string.h>
#include "BinaryFile.h"

int saveAirlineToBFile(Airline* a, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "wb+");
	if (!fp) {
		printf("Error open file to write\n");
		return 0;
	}
	int len = (int)strlen(a->name) + 1;
	if (fwrite(&len, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(a->name, sizeof(char), len, fp) != len)
		return 0;
	if (fwrite(&a->type, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(&a->flightCount, sizeof(int), 1, fp) != 1)
		return 0;


	for (int i = 0; i < a->flightCount; i++)
	{
		if (!saveFlightToBFile(a->flightArr[i], fp))
		{
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;
}

int saveFlightToBFile(Flight *f, FILE *fp)
{
	int sourceLen = (int)strlen(f->nameSource) + 1;
	if (fwrite(&sourceLen, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(f->nameSource, sizeof(char), sourceLen, fp) != sourceLen)
		return 0;
	int destLen = (int)strlen(f->nameDest) + 1;
	if (fwrite(&destLen, sizeof(int), 1, fp) != 1)
		return 0;
	if (fwrite(f->nameDest, sizeof(char), destLen, fp) != destLen)
		return 0;
	if (fwrite(&f->thePlane, sizeof(Plane), 1, fp) != 1)
		return 0;
	if (fwrite(&f->date, sizeof(Date), 1, fp) != 1)
		return 0;
	return 1;
}

int saveAirlineToComBFile(Airline *a, const char* filename, int isCompress)
{
	if (!isCompress)
		return saveAirlineToBFile(a, filename);
	FILE* fp = fopen(filename, "wb");
	if (!fp)
		return 0;
	BYTE data[2];
	int len = (int)strlen(a->name);
	data[0] = a->flightCount >> 1;
	data[1] = a->flightCount << 7 | a->type << 4 | len;
	if (fwrite(&data, sizeof(BYTE), 2, fp) != 2)
		return 0;
	if (fwrite(a->name, sizeof(char), len, fp) != len)
		return 0;

	for (int i = 0; i < a->flightCount; i++)
	{
		if (!saveFlightToComBFile(a->flightArr[i], fp, isCompress))
		{
			fclose(fp);
			return 0;
		}
	}

	fclose(fp);
	return 1;
}

int saveFlightToComBFile(Flight *f,FILE* fp, int isCompress)
{
	if (!isCompress)
		return saveFlightToBFile(f, fp);
	BYTE data1[2] = { 0 };
	int srcLen = (int)strlen(f->nameSource);
	int destLen = (int)strlen(f->nameDest);
	data1[0] = srcLen << 3 | (destLen >> 2);
	data1[1] = destLen << 6 | (f->thePlane.type << 4) | (f->date.month);
	if (fwrite(&data1, sizeof(BYTE), 2, fp) != 2)
		return 0;
	BYTE data2[3] = { 0 };
	int c0 = (int)f->thePlane.code[0] - 'A';
	int c1 = (int)f->thePlane.code[1] - 'A';
	int c2 = (int)f->thePlane.code[2] - 'A';
	int c3 = (int)f->thePlane.code[3] - 'A';
	data2[0] = c0 << 3 | c1 >> 2;
	data2[1] = ((c1 & 0x3) << 6) | (c2 << 1) | c3 >> 4;
	data2[2] = c3 << 4 | (f->date.year - 2021);
	BYTE data3 = { 0 };
	data3 = f->date.day;
	
	if (fwrite(&data2, sizeof(BYTE), 3, fp) != 3)
		return 0;
	if (fwrite(&data3, sizeof(BYTE), 1, fp) != 1)
		return 0;
	if (fwrite(f->nameSource, sizeof(char), srcLen, fp) != srcLen)
		return 0;
	if (fwrite(f->nameDest, sizeof(char), destLen, fp) != destLen)
		return 0;
	return 1;
}


int readAirlineFromBFile(Airline* a, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	if (!fp) {
		printf("Error open file to read\n");
		return 0;
	}
	int len;
	if (fread(&len, sizeof(int), 1, fp) != 1)
	{
		fclose(fp);
		return 0;
	}
	a->name = (char*)calloc(len, sizeof(char));
	if (!a->name)
	{
		fclose(fp);
		return 0;
	}
	if (fread(a->name, sizeof(char), len, fp) != len)
	{
		free(a->name);
		fclose(fp);
		return 0;
	}
	if (fread(&a->type, sizeof(int), 1, fp) != 1)
	{
		free(a->name);
		fclose(fp);
		return 0;
	}
	if (fread(&a->flightCount, sizeof(int), 1, fp) != 1)
	{
		free(a->name);
		fclose(fp);
		return 0;
	}
	a->flightArr = (Flight**)malloc(a->flightCount * sizeof(Flight*));
	for (int i = 0; i < a->flightCount; i++)
	{
		a->flightArr[i] = (Flight*)calloc(1, sizeof(Flight));
		if (!a->flightArr[i])
		{
			freeFlightArr(a->flightArr, i);
			free(a->flightArr);
			free(a->name);
			fclose(fp);
			return 0;
		}
		int res = readFlightFromBFile(a->flightArr[i], fp);

		if (!res)
		{
			freeFlightArr(a->flightArr, i);
			free(a->flightArr);
			free(a->name);
			fclose(fp);
			return 0;
		}
		insertFlightToListSorted(&a->allDates, &a->flightArr[i]->date);
	}
	fclose(fp);
	return 1;
}

int readAirlineFromComBFile(Airline *a, const char* filename, int isCompress)
{
	if (!isCompress)
	{
		return readAirlineFromBFile(a, filename);
	}
	FILE* fp = fopen(filename, "rb");
	BYTE data[2];
	if (fread(data, sizeof(BYTE), 2, fp) != 2)
	{
		return 0;
	}
	a->flightCount = (data[0] << 1) | (data[1] >> 7);
	if (data[0] >> 7 == 1)
		a->flightCount += 256;//for the 9th bit
	
	a->type = (data[1] >> 4) & 0x2;
	int len = data[1] & 0xF;
	a->name = (char*)calloc(len + 1, sizeof(char));
	if (fread(a->name, sizeof(char), len, fp) != len)
	{
		free(a->name);
		fclose(fp);
		return 0;
	}
	a->flightArr = (Flight**)malloc(a->flightCount * sizeof(Flight*));
	for (int i = 0; i < a->flightCount; i++)
	{
		a->flightArr[i] = (Flight*)calloc(1, sizeof(Flight));
		if (!a->flightArr[i])
		{
			freeFlightArr(a->flightArr, i);
			free(a->flightArr);
			free(a->name);
			fclose(fp);
			return 0;
		}
		int res = readFlightFromComBFile(a->flightArr[i], fp, isCompress);
		if (!res)
		{
			freeFlightArr(a->flightArr, i);
			free(a->flightArr);
			free(a->name);
			fclose(fp);
			return 0;
		}
		insertFlightToListSorted(&a->allDates, &a->flightArr[i]->date);
	}
	fclose(fp);
	return 1;
}

int readFlightFromBFile(Flight* f, FILE *fp)
{
	int len;
	if (fread(&len, sizeof(int), 1, fp) != 1)
	{
		fclose(fp);
		return 0;
	}
	f->nameSource = (char*)malloc(len * sizeof(char));
	if (!f->nameSource)
		return 0;
	if (fread(f->nameSource, sizeof(char), len, fp) != len)
	{
		free(f->nameSource);
		return 0;
	}
	if (fread(&len, sizeof(int), 1, fp) != 1)
		return 0;
	f->nameDest = (char*)calloc(len, sizeof(char));
	if (fread(f->nameDest, sizeof(char), len, fp) != len)
	{
		free(f->nameSource);
		free(f->nameDest);
		return 0;
	}
	if (fread(&f->thePlane, sizeof(Plane), 1, fp) != 1)
	{
		free(f->nameSource);
		free(f->nameDest);
		return 0;
	}
	if (fread(&f->date, sizeof(Date), 1, fp) != 1)
	{
		free(f->nameSource);
		free(f->nameDest);
		return 0;
	}
	return 1;
}

int readFlightFromComBFile(Flight *f, FILE* fp, int isCompress)
{
	if (!isCompress)
		return readFlightFromBFile(f, fp);
	BYTE data1[2];
	BYTE data2[3];
	BYTE data3;

	if (fread(&data1, sizeof(BYTE), 2, fp) != 2)
		return 0;
	if (fread(&data2, sizeof(BYTE), 3, fp) != 3)
		return 0;
	if (fread(&data3, sizeof(BYTE), 1, fp) != 1)
		return 0;

	int srcLen = data1[0] >> 3;
	int destLen = ((data1[0] &0x7) << 2) | (data1[1] >> 6);

	f->thePlane.type = (data1[1] >> 4) &0x3;
	f->date.month = data1[1] & 0XF;	
	f->thePlane.code[0] = (data2[0] >> 3) + 'A';
	f->thePlane.code[1] = (((data2[0] & 0x7) << 2) | (data2[1] >> 6)) + 'A';
	f->thePlane.code[2] = ((data2[1] >> 1) & 0x1F) + 'A';
	f->thePlane.code[3] = (((data2[1] & 0x1) << 4) | (data2[2] >> 4)) + 'A';
	f->date.year = (data2[2] & 0xF) + 2021;
	f->date.day = data3 & 0x1F;

	f->nameSource = (char*)calloc(srcLen + 1, sizeof(char));
	if (!f->nameSource)
		return 0;
	f->nameDest = (char*)calloc(destLen + 1, sizeof(char));
	if (!f->nameDest)
	{
		free(f->nameSource);
		return 0;
	}

	if (fread(f->nameSource, sizeof(char), srcLen, fp) != srcLen)
	{
		free(f->nameDest);
		free(f->nameSource);
		return 0;
	}

	if (fread(f->nameDest, sizeof(char), destLen, fp) != destLen)
	{
		free(f->nameDest);
		free(f->nameSource);
		return 0;
	}
	return 1;
}