#ifndef __COMP__
#define __COMP__

#include "Flight.h"
#include "AirportManager.h"
#include "List.h"
#include "Date.h"

typedef enum {
	eSource, eDest, eDate, ePlaneCode, eNoSorted, eNumOfTypes
} eSortType;


static const char* sortTypes[eNumOfTypes];

typedef struct
{
	char* name;
	int flightCount;
	Flight** flightArr;
	LIST allDates;
	eSortType type;

}Airline;

void    generalAirlineInit(Airline *pComp);
int	    initAirlineFromComFile(Airline* pComp,char* fileName, int isCompress);
int	    initAirlineFromFile(Airline* pComp, char* fileName);
void	initAirline(Airline* pComp);
int		addFlight(Airline* pComp,const AirportManager* pManager);
void	printCompany(const Airline* pComp);
void	printFlightArr(Flight** pFlight, int size);
void	doCountFlightsFromName(const Airline* pComp);
void	doPrintFlightsWithPlaneCode(const Airline* pComp);
void	doPrintFlightsWithPlaneType(const Airline* pComp);
void	freeFlightArr(Flight** arr, int size);
void	freeCompany(Airline* pComp);

void printDates(const LIST* pDates);
int	insertFlightToListSorted(LIST* pAllDates, Date* d);
int comapreBySource(const void* f1, const void* f2);
int comapreByDest(const void *f1, const void *f2);
int comapreByDate(const void *f1, const void *f2);
int comapreByCode(const void *f1, const void *f2);
void sortFlights(Airline* pAir);
void searchBySortType(Airline* pAir);


#endif

