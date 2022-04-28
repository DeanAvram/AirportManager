#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "AirportManager.h"
#include "General.h"
#include "TextFile.h"

void generalManagerInit(AirportManager* pManager)
{
	pManager->airportsCount = 0;
	pManager->airportsArr = NULL;
}

int	initManagerFromFile(AirportManager* pManager, const char* fileName)
{
	generalManagerInit(pManager);
	int res = readAirportManagerFromFile(pManager, fileName);
	if (!res)
		initManager(pManager);
	return res;
}

int	initManager(AirportManager* pManager)
{	

	printf("-----------  Init airport Manager\n");
	generalManagerInit(pManager);
	char op;
	while (1)
	{
		printf("Do you want to enter an airport? y/Y, anything else to exit!!\t");
		do {
			scanf("%c", &op);
		} while (isspace(op));
		getchar(); //clean the enter
		if (op != 'y' && op != 'Y')
			break;
		if (!addAirport(pManager))
			return 0;
	}

	return 1;
}

int	addAirport(AirportManager* pManager)
{
	pManager->airportsArr = (Airport*)realloc(pManager->airportsArr, (pManager->airportsCount + 1) * sizeof(Airport));
	if (!pManager->airportsArr)
		return 0;

	initAirport(&pManager->airportsArr[pManager->airportsCount], pManager);
	pManager->airportsCount++;
	return 1;
}

void  initAirport(Airport* pPort, AirportManager* pManager)
{
	while (1)
	{
		pPort->name = getStrExactName("Enter airport name");
		if (checkUniqeName(pPort->name, pManager))
			break;

		printf("This name already in use - enter a different name\n");
		//free the memory is not good
		free(pPort->name);
	}

	initAirportNoName(pPort);
}

Airport* findAirportByName(const AirportManager* pManager, const char* name)
{
	for (int i = 0; i < pManager->airportsCount; i++)
	{
		if (isAirportName(&pManager->airportsArr[i], name))
			return &pManager->airportsArr[i];
	}
	return NULL;
}

int checkUniqeName(const char* name,const AirportManager* pManager)
{
	Airport* port = findAirportByName(pManager, name);

	if (port != NULL)
		return 0;

	return 1;
}

void	printAirports(const AirportManager* pManager)
{
	printf("there are %d airports\n", pManager->airportsCount);
	generalArrayFunction(pManager->airportsArr, pManager->airportsCount, sizeof(Airport), printAirport);
}

void	freeManager(AirportManager* pManager)
{
	for (int i = 0; i < pManager->airportsCount; i++)
		freeAirport(&pManager->airportsArr[i]);
	free(pManager->airportsArr);
}
