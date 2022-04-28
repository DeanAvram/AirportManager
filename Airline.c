#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Airline.h"
#include "Airport.h"
#include "General.h"
#include "BinaryFile.h"

void generalAirlineInit(Airline *pComp)
{
	pComp->flightArr = NULL;
	L_init(&pComp->allDates);
	pComp->type = eNoSorted;
	pComp->flightCount = 0;
}


int	initAirlineFromComFile(Airline* pComp, char* fileName, int isCompress)
{
	generalAirlineInit(pComp);
	int res = readAirlineFromComBFile(pComp, fileName, isCompress);
	if (!res)
	{
		res = initAirlineFromFile(pComp, fileName);
		if (!res)
			initAirline(pComp);
	}
	return res;
}


int	initAirlineFromFile(Airline* pComp, char* fileName)
{
	generalAirlineInit(pComp);
	int res = readAirlineFromBFile(pComp, fileName);
	if (!res)
		initAirline(pComp);
	return res;
}


void	initAirline(Airline* pComp)
{
	printf("-----------  Init Airline\n");
	generalAirlineInit(pComp);
	pComp->name = getStrExactName("Enter Airline name");
}

int	addFlight(Airline* pComp,const AirportManager* pManager)
{
	if (pManager->airportsCount < 2)
	{
		printf("There are not enough airport to set a flight\n");
		return 0;
	}
	pComp->flightArr = (Flight**)realloc(pComp->flightArr, (pComp->flightCount+1) * sizeof(Flight*));
	if (!pComp->flightArr)
		return 0;
	pComp->flightArr[pComp->flightCount] = (Flight*)calloc(1,sizeof(Flight));
	if (!pComp->flightArr[pComp->flightCount])
		return 0;
	initFlight(pComp->flightArr[pComp->flightCount],pManager);
	Date* d = &pComp->flightArr[pComp->flightCount]->date;
	insertFlightToListSorted(&pComp->allDates, d);
	pComp->flightCount++;
	return 1;
}


int	insertFlightToListSorted(LIST* pAllDates, Date* d)
{
	NODE* tmp = &pAllDates->head;
	while (tmp->next)
	{
		int res = dateCmp(tmp->next->key, d);
		if (res == 0)
			return 0;
		if (res < 0)
			tmp = tmp->next;
		if (res > 0)
			break;
	}
	if (L_insert(tmp, d) == NULL)
		return 0;
	return 1;
}

void printCompany(const Airline* pComp)
{
	printf("Airline %s\n", pComp->name);
	printf("Has %d flights\n", pComp->flightCount);
	printFlightArr(pComp->flightArr, pComp->flightCount);
	printf("\nFlight Date List:");
	printDates(&pComp->allDates);
}

void printDates(const LIST* pDates)
{
	L_print(pDates, printDate);
}

void	doCountFlightsFromName(const Airline* pComp)
{
	if (pComp->flightCount == 0)
	{
		printf("No flight to search\n");
		return;
	}

	char* tempName = getStrExactName("Please enter origin airport name");

	int count = 0;
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (isFlightFromSourceName(pComp->flightArr[i], tempName))
			count++;
	}

	free(tempName);

	if (count != 0)
		printf("There are %d ", count);
	else
		printf("There are No ");

	printf("flights from this airport\n");
}



void	printFlightArr(Flight** pFlight, int size)
{
	generalArrayFunction(pFlight, size, sizeof(Flight*), printPointerFlight);
}

void	doPrintFlightsWithPlaneCode(const Airline* pComp)
{
	char code[MAX_STR_LEN];
	getPlaneCode(code);
	printf("All flights with plane code %s:\n",code);
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (isPlaneCodeInFlight(pComp->flightArr[i], code))
			printFlight(pComp->flightArr[i]);
	}
	printf("\n");

}

void	doPrintFlightsWithPlaneType(const Airline* pComp)
{
	ePlaneType type = getPlaneType();
	printf("All flights with plane type %s:\n", PlaneTypeStr[type]);
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (isPlaneTypeInFlight(pComp->flightArr[i], type))
			printFlight(pComp->flightArr[i]);
	}
	printf("\n");
}

void	freeFlightArr(Flight** arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		freeFlight(arr[i]);
	}
}

void	freeCompany(Airline* pComp)
{
	freeFlightArr(pComp->flightArr, pComp->flightCount);
	free(pComp->flightArr);
	free(pComp->name);
}

int comapreBySource(const void *f1, const void *f2)
{
	const Flight* flight1 = *(const Flight**)f1;
	const Flight* flight2 = *(const Flight**)f2;
	return strcmp(flight1->nameSource, flight2->nameSource);
}

int comapreByDest(const void *f1, const void *f2)
{
	const Flight* flight1 = *(const Flight**)f1;
	const Flight* flight2 = *(const Flight**)f2;
	return strcmp(flight1->nameDest, flight2->nameDest);
}

int comapreByDate(const void *f1, const void *f2)
{
	const Flight* flight1 = *(const Flight**)f1;
	const Flight* flight2 = *(const Flight**)f2;
	return dateCmp(&flight1->date, &flight2->date);
}

int comapreByCode(const void *f1, const void *f2)
{
	const Flight* flight1 = *(const Flight**)f1;
	const Flight* flight2 = *(const Flight**)f2;
	return strcmp(flight1->thePlane.code, flight2->thePlane.code);
}

void sortFlights(Airline* pAir)
{
	int choice;
	printf("Base on what field do you want to sort?\n"
			"Enter 1 for Source Name\n"
			"Enter 2 for Dest Name\n"
			"Enter 3 for Date\n"
			"Enter 4 for Plane Code\n");
	scanf("%d", &choice);
	switch (choice)
	{
	case 1:
		qsort(pAir->flightArr, pAir->flightCount, sizeof(Flight*), comapreBySource);
		pAir->type = eSource;
		break;
	case 2:
		qsort(pAir->flightArr, pAir->flightCount, sizeof(Flight*), comapreByDest);
		pAir->type = eDest;
		break;
	case 3:
		qsort(pAir->flightArr, pAir->flightCount, sizeof(Flight*), comapreByDate);
		pAir->type = eDate;
		break;
	case 4:
		qsort(pAir->flightArr, pAir->flightCount, sizeof(Flight*), comapreByCode);
		pAir->type = ePlaneCode;
		break;
	default:
		break;
	}
}


void searchBySortType(Airline* pAir)
{
	Flight ptr;
	Flight* temp = &ptr;
	int(*compare)(const void* f1, const void* f2) = NULL;
	if (pAir->flightCount < 1)
	{
		printf("Error, need at least 1 flight...");
		return;
	}
	if (pAir->type == eNoSorted)
	{
		printf("Error, not sorted yet\n");
		return;
	}
	eSortType searchType = pAir->type;
	Flight** lookFlight = NULL;
	switch (searchType)
	{
	case eDate:
		getCorrectDate(&temp->date);
		compare = comapreByDate;
		break;
	case ePlaneCode:
		getPlaneCode(temp->thePlane.code);
		compare = comapreByCode;
		break;
	case eSource:
		temp->nameSource = getStrExactName("Enter Source name:\n");
		compare = comapreBySource;
		break;
	case eDest:
		temp->nameDest = getStrExactName("Enter Dest name:\n");
		compare = comapreByDest;
	default:
		break;
	}
	lookFlight = bsearch(&temp, pAir->flightArr, pAir->flightCount, sizeof(Flight*), compare);

	if (!lookFlight)
		printf("Did not found the flight...\n");
	else
		printFlight(*lookFlight);
}