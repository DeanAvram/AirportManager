#ifndef __MYDATE__
#define __MYDATE__

#define MIN_YEAR 2022

typedef struct
{
	int			day;
	int			month;
	int			year;
}Date;

void	getCorrectDate(Date* pDate);
int		checkDate(char* date, Date* pDate);
void	printDate(const Date* pDate);
int dateCmp(const Date* d1, const Date* d2);

#endif
