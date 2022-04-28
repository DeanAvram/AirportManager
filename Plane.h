#pragma once

#define CODE_LENGTH 4

typedef enum { 
	eCommercial, eCargo, eMilitary, eNofPlaneTypes 
} ePlaneType;


static const char* PlaneTypeStr[eNofPlaneTypes];

typedef struct
{
	ePlaneType	type;
	char	code[CODE_LENGTH + 1];
}Plane;

void		initPlane(Plane* pPlane);
ePlaneType	getPlaneType();
void		getPlaneCode(char* code);
void		printPlane(const Plane* pPlane);

