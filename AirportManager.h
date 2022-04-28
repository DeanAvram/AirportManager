#ifndef __AIR_MANAGER__
#define __AIR_MANAGER__

#include "Airport.h"

typedef struct
{
	Airport*	airportsArr;
	int			airportsCount;
}AirportManager;

void generalManagerInit(AirportManager* pManager);
int		initManagerFromFile(AirportManager* pManager, const char* fileNmae);
int		initManager(AirportManager* pManager);
int		addAirport(AirportManager* pManager);
void	initAirport(Airport* pPort, AirportManager* pManager);
Airport* findAirportByName(const AirportManager* pManager, const char* name);
int		checkUniqeName(const char* name, const AirportManager* pManager);
void	printAirports(const AirportManager* pManager);
void	freeManager(AirportManager* pManager);
#endif