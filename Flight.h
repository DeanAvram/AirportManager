#ifndef __FLIGHT__
#define __FLIGHT__

#include "AirportManager.h"
#include "Date.h"
#include "General.h"
#include "Plane.h"

typedef struct
{
	char*		nameSource;
	char*		nameDest;
	Plane		thePlane;
	Date		date;
}Flight;

void	initFlight(Flight* pFlight,const AirportManager* pManager);
int		isFlightFromSourceName(const Flight* pFlight, const char* nameSource);
int		isFlightToDestName(const Flight* pFlight, const char* nameDest);
void	printFlight(const Flight* pFlight);
Airport*	setAiportToFlight(const AirportManager* pManager, const char* msg);
int		isPlaneCodeInFlight(const Flight* pFlight, const char*  code);
int		isPlaneTypeInFlight(const Flight* pFlight, ePlaneType type);
void	freeFlight(Flight* pFlight);
void printPointerFlight(const void** pF);

#endif
